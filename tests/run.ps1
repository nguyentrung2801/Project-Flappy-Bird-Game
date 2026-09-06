param([string]$Compiler = 'gcc')
$ErrorActionPreference = 'Stop'
$root = Split-Path $PSScriptRoot -Parent
$outputDir = Join-Path $root 'build/tests'
New-Item -ItemType Directory -Force -Path $outputDir | Out-Null
$sources = @(Get-ChildItem (Join-Path $root 'Src/game') -Filter '*.c' | ForEach-Object FullName)
$exe = Join-Path $outputDir 'game_test.exe'
& $Compiler '-std=c11' '-Wall' '-Wextra' '-Werror' ('-I' + (Join-Path $root 'Inc')) @sources (Join-Path $PSScriptRoot 'game_test.c') '-o' $exe
if ($LASTEXITCODE -ne 0) { throw 'Test compilation failed.' }
& $exe
if ($LASTEXITCODE -ne 0) { throw 'Tests failed.' }
