# Green Top: Verdigris
This is the C implementation of [Green Top](https://rayo75.itch.io/green-top).

# Building
Platform dependent build instructions are listed below.
As of now, platform independent resources that are loaded by the game during runtime are expected to be found in a folder in the project root directory named `assets`.

TODO: add compilation instructions for every OS I plan to compile the game from,
but in all likeliness its going to be running `build_<platform>.shell_script_extension` anyway

## Windows, MSVC
Note: As of now this is a makeshift testing build
- Download raylib-5.0_win64_msvc16 from the raylib git repo and extract it to a folder named `third_party`
- Open the visual studio developer command prompt and navigate it to this directory
- Execute `build_cradle.bat && build_application.bat`

# TODO
- [ ] Recreate the 1.0 demo
    - [ ] Hot reload
        - [x] Basic hot reload
        - [ ] Data layout change detection
    - [ ] Asset loader
        - [x] Hardcoded image loader
        - [x] Generic image palettizer
    - [ ] Gameplay
        - [ ] 2D movement mechanics
            - [x] Basic 2D movement (walking, jumping, gravity, collision detection)
            - [ ] The rest of https://2dengine.com/doc/platformers.html
            - [ ] Wall climbing
            - [ ] Rocket boosting
    - [ ] Implement the concept of a map
        - [ ] Editable platforms
            - [ ] Platform editor
        - [ ] Level serializer (for saving/loading)
    - [ ] Artwork
        - [x] Indexed color framebuffer impl
        - [ ] Environments
            - [ ] Sprite editor?
            - [ ] Non color cycling demo
            - [ ] Color cycled backgrounds/tiles
        - [ ] Main character (Try not to steal from the source game even if the dev is nice)
            - [ ] Decide on whether to use frames or color cycling to animate this
    - [ ] Shipping
        - [ ] Win32
            - [ ] Build the shipped game statically
        - [ ] Android
            - [ ] Implement touchscreen controls
        - [ ] Web
            - [ ] Do some stuff with uhh emscripten or something
