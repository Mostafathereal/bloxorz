# bloxorz

### Group Members 
---
- Mostafa Mohsen - mohsem1
- Purv Patel - patep11
- Hassan Jasim - jasima

## Overview
We implemented a block-character based game using Opengl. The player plays as a 1 by 1 by 2 block and he must navigate the block from the starting position to the endpoint marked by the black tile. There are currently three levels in our game. If any part of the block goes outside of the platform, the block will fall off the platform and the player will lose the game. The score is the number of moves that the player takes to finish the game (clear all three levels). A lower score is better since that means the player completed the game in fewer moves. The game also offers an undo functionality in case players want to retry/rethink a move. If the player loses the game, he must start from the first level, this is because our game only has three levels so its reasonable.

## Dev Environments
- MacOS - Apple silicon
- Windows 10

## Running Instructions
Within the project repository, run this
```
make
```

## Game Controls and Instructions
- Press space to start the game from main menu, move on from loss and win screens
- use `w`, `a`, `s`, `d` to move the block
- use `1` and `2` to swap between different textures for the block (during any time)
- press `q` to quit the game at anytime 
- if block is outside the platform, you will lose
- Pink tiles can only be travesed once, they will disappear afterwards
- correctly get the block in the upright position onto the winning Black tile (has to be upright)
- If you win, you will advance to the next level, unless you are at the last level, then you win the game

## Game Mechanic Features Promised from Prototype
- Smooth animations for moving block using Quaternions and SLERP
- Smooth animations for block falling or winning
- Displaying text on top of screen (overlaying 3D with gluOrtho2D)
- Have multiple levels along with special tiles
- materials and textures for tiles and block
- Main Menu and quit functionality
- Undo functionality

## Optimizations Made From prototype
- Cleaned up Block code using mathLib3D library
- modularized and cleaned up quaternion rotations
