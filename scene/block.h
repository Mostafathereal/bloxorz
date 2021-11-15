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

class Block{
    public:
    Quaternion quat;
    GLfloat* rotationMatrix;
    float posX1;
    float posZ1;
    float posX2;
    float posZ2;
    float offsetX;
    float offsetY;
    float offsetZ;
    Orientation orientation;
    Direction direction;
    

    Block(GLfloat* initMatrix);

    void drawBlock();

    void setOrientation();
    void update();
    void setDirection(Direction directionToRoll);
};

#endif