# Flappy Bird – STM32 Nucleo-F401RE

A Flappy Bird game written in C for the STM32 Nucleo-F401RE board, using
SPL/CMSIS and Ucglib to control the hardware and draw on the LCD.
The current SDK configuration uses a 128×128 ST7735 display, rotated
180°, with a 128×80 pixel play area.

## Controls

- **SW1 (PB5):** move the menu cursor to LEVEL.
- **SW5 (PB4):** move the cursor to START.
- **SW3 (PA4):** press on LEVEL to change the difficulty, or on START to
  begin playing. During a game, each press makes the bird flap once.
- Holding SW3 does not make the bird flap repeatedly. After pressing START,
  release the button before pressing it again to flap.
- When you lose, the game stops, the buzzer turns off, and your score appears
  on screen. The LED on PA11 blinks five times over about 1.2 seconds,
  then the game returns to the menu.

The buzzer is connected to PC9. The code simply switches the GPIO pin on
and off, so it works with a buzzer module that has a built-in oscillator.
If you use a passive buzzer, you will need to add PWM output to produce sound.

## Source Layout

Header files are in `Inc/`, and their implementations are in `Src/`.
Both folders follow the same layout to make files easy to find. For example,
`Inc/game/bird.h` goes with `Src/game/bird.c`.

| Module | What it does |
|---|---|
| app | Switches between the menu, gameplay, and game over; handles button actions and effects |
| game/config.h, level_config.c | Defines the play area, bird and pipe sizes, and settings for each level |
| game/bird | Updates the bird's position and velocity, and handles flapping |
| game/pipe | Creates new pipes and moves them across the screen |
| game/collision | Checks whether the bird hits a pipe, the ceiling, or the floor |
| game/game | Runs the game update steps, adds points, and determines when the game is over |
| gfx/render | Draws the bird, pipes, and border; redraws only the changed strips as pipes move |
| gfx/ui | Draws the menu and results screen |
| periph/input | Debounces buttons over 15 ms and records each press |
| periph/buzzer, led | Controls the LED and buzzer; the flap sound lasts about 60 ms |
| drivers/timebase | Reads the time in milliseconds and calls the SDK scheduler |
| board_config.h | Defines GPIO pins and the LCD color order |

To see how the game works, start with `app.c`, then read `game/game.c`.
Every 20 ms, the game updates the bird and pipe, checks for collisions,
and then updates the score. If drawing to the LCD takes longer, the game
catches up on the missing update steps before drawing the next frame,
keeping the bird and pipe on the same timing.

The `game/` code only works with game data; it does not call STM32 or
Ucglib functions directly. Button input and game updates both run in
the main loop. Timing comes from SysTick, initialized by the SDK's
`TimerInit()`; TIM2 is not used.

Pipe drawing and collision detection share `PIPE_GAP_HALF`, so the gap
you see on screen matches the space the bird can fly through. Rectangles
use the interval `[x, x + width)`: the starting edge is included, and
the ending edge is excluded. The bird can fit exactly against the gap's
edge, but moving beyond it ends the game. You earn one point when the
entire pipe has passed the bird.

## Building with PowerShell

You will need the `ThuVien_SDK_1.0.3_NUCLEO-F401RE-master` SDK and the
ARM GCC toolchain included with STM32CubeIDE. Open PowerShell in the
folder containing `build.ps1`, replace the compiler path below with
the one on your machine, and run:

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File .\build.ps1 -SdkRoot "C:\ThuVien_SDK_1.0.3_NUCLEO-F401RE-master" -ToolchainBin "PATH_TO_ARM_GCC\bin" -Configuration Debug
```

Change `Debug` to `Release` for a build optimized for size. To avoid
typing the paths each time, set the `FLAPPY_SDK_ROOT` and `ARM_GCC_BIN`
environment variables, then leave the path arguments out of the command.

After the build, you will find the ELF, HEX, and map files in
`build/Debug` or `build/Release`. The script enables
`-Wall -Wextra -Werror` for application code, so any warnings in that
code must be fixed before the build can succeed. SDK warnings are still
shown, but they are not treated as errors.

## STM32CubeIDE

After importing the project, open **Properties → Resource → Linked Resources**
and check the `ThuVien_SDK_1.0.3_NUCLEO-F401RE-master` link.
It currently points to `C:/ThuVien_SDK_1.0.3_NUCLEO-F401RE-master`.
If your SDK is elsewhere, update this link. Debug and Release are both
configured to use the same SDK header paths and source entries.

For the first build after updating the source, run
**Refresh → Project → Clean → Build**. This lets CubeIDE regenerate
the makefiles with the new file list, replacing references to old files
such as `entities.c`, `levels.c`, `loop.c`, and `game/ui.c`.
The IDE generates the makefiles in `Debug/`, so there is no need to
edit them by hand.

## Testing

This source has been compiled and linked in both Debug and Release
with ARM GCC 13.3.1, producing ELF and HEX files. Application code passes
`-Wall -Wextra -Werror`; the SDK still reports some unused-parameter warnings.

The C tests have only been checked for syntax. Their `assert` checks
have not been run because the machine used to update the source did
not have a native Windows C compiler available. The firmware has not
been flashed or tested on a physical board yet.

`tests/game_test.c` covers both levels: gap boundaries, ceiling and floor
collisions, pipe widths, scoring once per pipe, spawning a new pipe,
stopping after a collision, and starting a new game. To run it, you need
GCC for Windows, such as MinGW. Replace the path below with yours
(this is the compiler for your PC, not ARM GCC):

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File .\tests\run.ps1 -Compiler "PATH_TO_MINGW\gcc.exe"
```

After flashing the firmware to your board, try these checks:

1. Press SW3 in the menu just after powering on, then try again after losing
   a game. The button should respond normally in both cases.
2. Hold SW3 during a game: the bird should flap only once. Release and press
   it again for another flap.
3. Play both levels and try flying close to the gap edges. Pipes should be
   8 and 12 pixels wide respectively, and the bird should not lose while
   it is still inside the gap.
4. Pass a pipe and check the score at the end of the game. Each pipe should
   add only one point, and a new game should start at zero.
5. When you lose, check that the buzzer turns off and the bird stops.
   The LED should blink before the game returns to the menu.
6. Watch whether the bird and pipes move smoothly. This needs to be checked
   on the actual LCD, since SPI speed depends on the SDK and hardware.

