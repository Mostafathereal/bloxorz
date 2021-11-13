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

class Block{
    public:
    std::vector<std::vector<float>> vertices;
    std::vector<std::vector<float>> normals;
    std::vector<std::vector<int>> faceIndexBuffer;
    Quaternion quat;
    //Quaternion orient;
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
    bool standing;

    Block(std::vector<std::vector<float>> &vertices, std::vector<std::vector<int>> &faceIndexBuffer, float faceLength, float heightLength, GLfloat* initMatrix);

    void drawBlock();

    void calculateNormals();

    void rotateLeft();
    void rotateRight();
    void rotateUp();
    void rotateDown();
};

#endif