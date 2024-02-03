# Green Top: Verdigris
This is an unofficial remake of [Green Top](https://rayo75.itch.io/green-top),
a submission for the Pirate Software Game Jam.

# Building
## Macos
As of now, SDL2 is required to build this game. Enter the following commands to build:
```sh
export LIBRARY_PATH="$LIBRARY_PATH:$(brew --prefix)/lib"
cargo build
```