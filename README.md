# Green Top: Verdigris
This is an unofficial remake of [Green Top](https://rayo75.itch.io/green-top),
a submission for the Pirate Software Game Jam.

# Building
Platform dependent build instructions are listed below.
As of now, platform independent resources that are loaded by the game during runtime are expected to be found in a folder in the project root directory named `loaded_resources`.

## Macos
As of now, SDL2 is required to build this game. Enter the following commands to build:
```sh
export LIBRARY_PATH="$LIBRARY_PATH:$(brew --prefix)/lib"
cargo build
```

## Windows, MSVC
- Add sdl2_ttf.lib and sdl2.lib to your rustup lib file (Example: `C:\Users\YourUsername\.rustup\toolchains\YourToolchain\lib\rustlib\YourToolChain\lib`)
- Make sure their respective DLLs are either in the project root folder or beside the executable when shipping.
```bat
cargo.exe build
```