# Green Top: Verdigris Hardware Reference
## About
TODO


## Memory Model
### 0x0000 -> 0x0FFF
This is the suggested memory region to store programs in.

### 0x1000 -> 0xFFEF
This is the suggested memory region to store data in.

### 0xFFF0 -> 0xFFF9
This is currently reserved for chiptune audio.

### 0xFFFA -> 0xFFFB
This memory reigon is directly connected to the input buttons.
- 4 bits for the developer console
- 4 bits for direction keys
- 4 bits for action keys (a, b, x, y)
- 4 bits reserved

### 0xFFFC -> 0xFFFF
This memory region is reserved for the GU.
- Format: dccc cccc aaaa aaaa
  - d: If 1, send draw command
  - c: R5 G5 B5 encoded color
  - a: 16 bit address for 256x256 display


## Components
### Graphics Unit (GU)
TODO

### Input
TODO

### Sound
TODO