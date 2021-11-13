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
#include "block.h"
#include <iostream>

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

    this->calculateNormals();

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

    this->standing = true;
    
}

void Block::drawBlock(){
    //std::cout << this->vertices[0][2] << std::endl;
    // float cols[6][3] = { {1,0,0}, {0,1,1}, {1,1,0}, {0,1,0}, {0,0,1}, {1,0,1} };
    // int i = 0;

    // for(auto &face : this->faceIndexBuffer){
    //     glColor3fv(cols[i]);
    //     i++;
    //     glBegin(GL_POLYGON);
    //         glVertex3fv(this->vertices[face[0]].data());
    //         glVertex3fv(this->vertices[face[1]].data());
    //         glVertex3fv(this->vertices[face[2]].data());
    //         glVertex3fv(this->vertices[face[3]].data());
    //     glEnd();
        
    // }
    glPushMatrix();
    std::cout << this->offsetX << ", " << this->offsetY << ", " << this->offsetZ << std::endl;
    glTranslatef(this->offsetX + 0.5, this->offsetY, this->offsetZ + 0.5);
    glMultMatrixf(this->rotationMatrix);
    glScalef(1, 2, 1);
    glutSolidCube(1);
    glPopMatrix();
}

void Block::calculateNormals(){
    // for(int i = 0; i < 6; ++i){
        
    // }
}

void Block::rotateRight(){
    Quaternion tempQuat(90, 0, 0, 1);

    this->posX1 = this->posX2;
    if(this->standing)
        this->posX2 += 2;
    else
        this->posX2 += 1;

    this->offsetX = this->posX1;
    //this->offsetZ = this->posZ1; 

    if(this->standing || this->posZ2 - this->posZ1 > 1){
        this->standing = false;
        this->offsetY = 0.5;
    }   
    else{
        this->standing = true;
        this->offsetY = 1;
    }
        

    this->quat = tempQuat * this->quat;
    this->quat.populateRotationMatrix(this->rotationMatrix);
}
void Block::rotateLeft(){
    Quaternion tempQuat(90, 0, 0, -1);

    this->posX2 = this->posX1;
    if(this->standing)
        this->posX1 -= 2;
    else
        this->posX1 -= 1;

    
    this->offsetX = this->posX1;
    //this->offsetZ = this->posZ1; 

    if(this->standing || this->posZ2 - this->posZ1 > 1){
        this->standing = false;
        this->offsetY = 0.5;
    }
    else{
        this->standing = true;
        this->offsetY = 1;
    }
        

    this->quat = tempQuat * this->quat;
    this->quat.populateRotationMatrix(this->rotationMatrix);
}
void Block::rotateDown(){
    Quaternion tempQuat(90, 0, 0, 1);

    this->posZ1 = this->posZ2;
    if(this->standing)
        this->posZ2 += 2;
    else
        this->posZ2 += 1;

    
    //this->offsetX = this->posX1;
    this->offsetZ = this->posZ1; 

    if(this->standing || this->posX2 - this->posX1 > 1){
        this->standing = false;
        this->offsetY = 0.5;
    }
    else{
        this->standing = true;
        this->offsetY = 1;
    }

    this->quat = tempQuat * this->quat;
    this->quat.populateRotationMatrix(this->rotationMatrix);
}
void Block::rotateUp(){
    Quaternion tempQuat(90, 0, 0, -1);

    this->posZ2 = this->posZ1;
    if(this->standing)
        this->posZ1 -= 2;
    else
        this->posZ1 -= 1;

    
    //this->offsetX = this->posX1;
    this->offsetZ = this->posZ1; 

    if(this->standing || this->posX2 - this->posX1 > 1){
        this->standing = false;
        this->offsetY = 0.5;
    }
    else{
        this->standing = true;
        this->offsetY = 1;
    }

    this->quat = tempQuat * this->quat;
    this->quat.populateRotationMatrix(this->rotationMatrix);
}