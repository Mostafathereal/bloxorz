#ifndef BLOCK_H
#define BLOCK_H

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif
#include <vector>
#include "../quaternion.h"
#include "../blockOrientations.h"
#include "../texture2D.h"

class Block{
    public:
    //quaternions for orientation and rotation
    Quaternion quat;
    Quaternion rotQuat;

    //rotation matrix to apply rotation transformations to model view
    GLfloat* rotationMatrix;
    
    //position of top left corner of block
    float posX1;
    float posZ1;

    //position of bottom right corner of block
    float posX2;
    float posZ2;

    //normialized origin of block (real time, includes animational changes)
    Point3D offset;

    //normialized origin of block (does not include animational changes, only stationary positions)
    Point3D origin;

    //length of base of block
    float baseLength;

    //length of height of block
    float heightLength;

    //orientation of block (standing, laying down in x axis, laying down in z axis)
    Orientation orientation;

    //direction of rotation when block is being animated
    Direction direction;

    //t and value to increment t/ per call. Used for SLERP interpolation in animation
    float rotIncrement;
    float currentT;

    //offsets for right and down directions (need them to make sure block rotates over its right and down edges)
    float RightRotationOffset;
    float DownRotationOffset;

    //move history for undos
    std::vector<Direction> undoMoveHist;

    //number of moves made
    int numMoves;

    //texture
    Texture2D texture;

    //game state
    int gameState;

    Block(GLfloat* initMatrix);

    void drawBlock();

    void setOrientation();
    void update();
    void updatePosition();
    void setDirection(Direction directionToRoll);
    void undoMove();
    void setOrigin();
    void changeTexture(char* file);
    void fallingAnimation();
    Quaternion blockRotation(Quaternion rotateQuaternion, float XOffset, float ZOffset);
};

#endif