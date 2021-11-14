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
#include <iostream>
#include "block.h"
#include "../blockOrientations.h"

Block::Block(std::vector<std::vector<float>> &vertices, std::vector<std::vector<int>> &faceIndexBuffer, float faceLength, float heightLength, GLfloat* initMatrix){
    this->vertices = std::vector<std::vector<float>>();
    for (auto &vertex : vertices){
        this->vertices.push_back(vertex);
    }

    this->faceIndexBuffer = std::vector<std::vector<int>>();
    for (auto &face : faceIndexBuffer){
        this->faceIndexBuffer.push_back(face);
    }

    this->faceLength = faceLength;
    this->heightLength = heightLength;

    this->normals = std::vector<std::vector<float>>(6, std::vector<float>(3));
    
    this->quat = Quaternion();
    this->rotationMatrix = initMatrix;
    this->quat.populateRotationMatrix(this->rotationMatrix);


    this->posX1 = 0;
    this->posZ1 = 0;

    this->posX2 = 1;
    this->posZ2 = 1;

    this->offsetX = 0;
    this->offsetY = 1;
    this->offsetZ = 0;

    this->orientation = Standing;
    
}

void Block::drawBlock(){
    glPushMatrix();
        if(this->orientation == Standing)
            glTranslatef(this->offsetX + 0.5, this->offsetY, this->offsetZ + 0.5);
        else if(this->orientation ==  HorizontalInX)
            glTranslatef(this->offsetX + 1, this->offsetY, this->offsetZ + 0.5);
        else
            glTranslatef(this->offsetX + 0.5, this->offsetY, this->offsetZ + 1);
        glMultMatrixf(this->rotationMatrix);
        glScalef(1, 2, 1);
        glutSolidCube(1);
    glPopMatrix();
}

void Block::setOrientation(){
    if(this->orientation == Standing){
        if(this->posX2 - this->posX1 > 1){
            this->orientation = HorizontalInX;
        }
        else{
            this->orientation = HorizontalInZ;
        }
        this->offsetY = 0.5;
    }
    else if (this->orientation == HorizontalInX){
        if (this->posX2 - this->posX1 == 1){
            this->orientation = Standing;
            this->offsetY = 1;
        }
        else{
            this->offsetY = 0.5;
        }
    }
    else{
        if (this->posZ2 - this->posZ1 == 1){
            this->orientation = Standing;
            this->offsetY = 1;
        }
        else{
            this->offsetY = 0.5;
        }
    }
}

void Block::move(int key){
    Quaternion tempQuat;
    switch(key){
        case 'u':
        case 'U':{
            tempQuat = Quaternion(90, -1, 0, 0);

            this->posZ2 = this->posZ1;
            if(this->orientation == Standing)
                this->posZ1 -= 2;
            else
                this->posZ1 -= 1;

            this->offsetZ = this->posZ1; 

            this->setOrientation();
            break;
        }
        case 'd':
        case 'D':{
            tempQuat = Quaternion(90, 1, 0, 0);

            this->posZ1 = this->posZ2;
            if(this->orientation == Standing)
                this->posZ2 += 2;
            else
                this->posZ2 += 1;

            this->offsetZ = this->posZ1; 

            this->setOrientation();
            break;
        }
        case 'l':
        case 'L':{
            tempQuat = Quaternion(90, 0, 0, -1);

            this->posX2 = this->posX1;
            if(this->orientation == Standing)
                this->posX1 -= 2;
            else
                this->posX1 -= 1;

            
            this->offsetX = this->posX1;

            this->setOrientation();
            break;
        }
        case 'r':
        case 'R':{
            tempQuat = Quaternion(90, 0, 0, 1);

            this->posX1 = this->posX2;
            if(this->orientation == Standing)
                this->posX2 += 2;
            else
                this->posX2 += 1;

            this->offsetX = this->posX1;

            this->setOrientation();
            break;
        }
    }
    this->quat = tempQuat * this->quat;
    this->quat.populateRotationMatrix(this->rotationMatrix);
}