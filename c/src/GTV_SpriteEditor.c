typedef struct GTV_SpriteEditor GTV_SpriteEditor; // TODO
typedef struct GTV_LevelEditor GTV_LevelEditor; // TODO

byte g_spredit_background_color = 0x00;
GTV_Sprite *g_spredit_curr_sprite;// = g_player_sprite;

/* Layout
    +---+
    |1|2|
    +-+-+
    |3|4|
    +---+

    1: Sprite viewer
    2: Color viewer
    3: Info box
    4: Tool selector
*/

/* Color picker
    2x2 pixel, 3x3 repeating pattern, 3n+1 x 3n+1 total size
    +--- ---
    |cc. cc.
    |cc. cc.
    |... ...

    |cc. cc.
    |cc. cc.
    |... ...
    ---- ---

    to fit 128x128 space,
    3n+1 = 128
    n = 127/3 = 42 rem 1
*/

/* Color viewer
    Uses same repeating pattern as above
    32 color viewer (97 x 4), 4 views

    +-------------------+
    |  last used: c     |
    |  history: cccccc  |
    |  viewing: c       |
    |                   |
    |                   |
    |  +-------------+  |
    | < ccccccccccccc > |
    |      1/4 FF       |
    +-------------------+
*/
