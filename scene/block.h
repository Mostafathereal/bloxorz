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
    std::vector<std::vector<float>> vertices;
    std::vector<std::vector<float>> normals;
    std::vector<std::vector<int>> faceIndexBuffer;
    Quaternion quat;
    GLfloat* rotationMatrix;
    float faceLength;
    float heightLength;
    float posX1;
    float posZ1;
    float posX2;
    float posZ2;
    float offsetX;
    float offsetY;
    float offsetZ;
    Orientation orientation;

    Block(std::vector<std::vector<float>> &vertices, std::vector<std::vector<int>> &faceIndexBuffer, float faceLength, float heightLength, GLfloat* initMatrix);

    void drawBlock();

    void setOrientation();
    void move(int key);
};

#endif