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

## Game Controls and Instructions
- Press space to start the game from main menu, move on from loss and win screens
- use `w`, `a`, `s`, `d` to move the block
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

## Prototype works and features
- Block class 
  - position (block position as well as origin from where to draw the block)
  - bounding coordinates (this is basically two position points for now, top left and bottom right of block (base of block))
  - orientation (quaternion to keep track of total rotation)
  - rotation information (quaternion to keep track of rotation of animation)
- Platform class
  - consists of tile map (2D vector)
  - draws the tiles if their values are `1`
- Quaternion class (For faster rotations and cleaner animations)
  - We used quaternions instead of euler angles for rotation to keep consistent track of the orientation of the block
  - They also helped us to avoid having to deal with the ambiguity of euler angles. 
  - We implemented the SLERP algorithm with quaternions to make a much cleaner and smoother animation.
- Keyboard controls 
  - Use W, A, S, D to roll the block on the platform
- Basic camera model 
  - We just implemented a basic orthogonal camera in the positive corner for now (we focused more on smooth animations and block logic instead).
  - We also added some simple lighting to the block and tiles.
- Basic local light

 ## Things to Do/ We might add (This is not an extensive list and we might add more things)
 - Undo functionality
   - We can add a basic undo functionality by keeping track of rotations and playing reverse rotations in undo (easier to do with quaternions)
 - Textures
   - Currently, the block is drawn with glutSolidCube. We are thinking of just taking the source code for this routine and then modifing it to add texture coords
   - Tiles also will be textured
 - Game Win/lose functionality
    - Our protoype already has state information (the platform and block coords), we just need to implement a logic system to detect game win/loss
    - Have to implement falling animations and game winning animation
 - Main menu and Block selection
    - Implement a main menu for game along with a simple block selection (selecting different textures)
 - Level design and additional mechanics
    - Implement logic to play multiple levels (we already have the basic tools in our prototype, just need to design levels)
    - Adding additional features like tiles turning on and off at certain intervals or breaking after one pass through or having switches on tiles to trigger things
 
 ## Things to Optimize
 - Cleaning up code
    - Our current Poc can be modularized and refined further; using Point3D classes for positions, pruning mathLib3D to keep nesecary functions only.
 - Optimizing Quaternions
    - Currently, the quaternions use full formulas for calculations (not unit quaternions), even though we only work with unit quaternions.
    - Using too many quaternions in rotations; currently using two (extra) quaterions to rotate, one for origin point rotation and another for full rotation.
    - Refactoring file structure
