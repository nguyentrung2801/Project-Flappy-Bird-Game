param(
    [string]$SdkRoot = $env:FLAPPY_SDK_ROOT,
    [string]$ToolchainBin = $env:ARM_GCC_BIN,
    [ValidateSet('Debug', 'Release')][string]$Configuration = 'Debug'
)
$ErrorActionPreference = 'Stop'
if (!$SdkRoot) { throw 'Pass -SdkRoot or set FLAPPY_SDK_ROOT to the SDK directory.' }
if (!$ToolchainBin) { throw 'Pass -ToolchainBin or set ARM_GCC_BIN to the ARM GCC bin directory.' }
$gcc = Join-Path $ToolchainBin 'arm-none-eabi-gcc.exe'
$sdk = (Resolve-Path -LiteralPath $SdkRoot).Path
$outputDir = Join-Path $PSScriptRoot "build/$Configuration"
New-Item -ItemType Directory -Force -Path $outputDir | Out-Null
$includeDirs = @('Drivers/CMSIS/Include', 'Drivers/STM32F401RE_StdPeriph_Driver/inc',
    'Middle/button', 'Middle/buzzer', 'Middle/flash', 'Middle/led', 'Middle/rtos',
    'Middle/sensor', 'Middle/serial', 'Middle/ucglib', 'Utilities')
$flags = @('-mcpu=cortex-m4', '-mthumb', '-mfpu=fpv4-sp-d16', '-mfloat-abi=hard',
    '-std=gnu11', '-ffunction-sections', '-fdata-sections', '--specs=nano.specs',
    '-DNUCLEO_F401RE', '-DSTM32', '-DSTM32F401RETx', '-DSTM32F4', '-Wall', '-Wextra')
if ($Configuration -eq 'Debug') { $flags += @('-Og', '-g3', '-DDEBUG') }
else { $flags += '-Os' }
$flags += '-I' + (Join-Path $PSScriptRoot 'Inc')
foreach ($dir in $includeDirs) { $flags += '-I' + (Join-Path $sdk "shared/$dir") }
$sources = @(Get-ChildItem (Join-Path $PSScriptRoot 'Src') -Recurse -Filter '*.c')
$sources += Get-Item (Join-Path $PSScriptRoot 'Startup/startup_stm32f401retx.s')
$sources += Get-Item (Join-Path $sdk 'shared/Drivers/CMSIS/Source/system_stm32f4xx.c')
$sources += Get-ChildItem (Join-Path $sdk 'shared/Drivers/STM32F401RE_StdPeriph_Driver/src') -Filter '*.c'
$sources += Get-ChildItem (Join-Path $sdk 'shared/Middle') -Recurse -Filter '*.c'
$sources += Get-ChildItem (Join-Path $sdk 'shared/Utilities') -Filter '*.c'
$objects = @()
$index = 0
foreach ($source in $sources) {
    $obj = Join-Path $outputDir ("{0}_{1}.o" -f $index++, $source.BaseName)
    $sourceFlags = $flags
    # Third-party warnings remain visible; application code must be warning-free.
    if ($source.FullName.StartsWith((Join-Path $PSScriptRoot 'Src'))) { $sourceFlags += '-Werror' }
    & $gcc @sourceFlags '-c' $source.FullName '-o' $obj
    if ($LASTEXITCODE -ne 0) { throw "Compilation failed: $($source.FullName)" }
    $objects += '"' + $obj.Replace('\', '/') + '"'
}
$responseFile = Join-Path $outputDir 'objects.rsp'
[IO.File]::WriteAllLines($responseFile, $objects, (New-Object Text.UTF8Encoding($false)))
$elf = Join-Path $outputDir 'Flappybirdgame.elf'
$linkFlags = @('-mcpu=cortex-m4', '-mthumb', '-mfpu=fpv4-sp-d16', '-mfloat-abi=hard',
    '--specs=nosys.specs', '--specs=nano.specs', '-Wl,--gc-sections',
    ('-T' + (Join-Path $PSScriptRoot 'STM32F401RETX_FLASH.ld')),
    ('-Wl,-Map=' + (Join-Path $outputDir 'Flappybirdgame.map')))
& $gcc @linkFlags "@$responseFile" '-o' $elf '-Wl,--start-group' '-lc' '-lm' '-Wl,--end-group'
if ($LASTEXITCODE -ne 0) { throw 'Link failed.' }
& (Join-Path $ToolchainBin 'arm-none-eabi-objcopy.exe') '-O' 'ihex' $elf (Join-Path $outputDir 'Flappybirdgame.hex')
if ($LASTEXITCODE -ne 0) { throw 'HEX conversion failed.' }
& (Join-Path $ToolchainBin 'arm-none-eabi-size.exe') $elf
if ($LASTEXITCODE -ne 0) { throw 'Size check failed.' }
Write-Output "Built $elf"
