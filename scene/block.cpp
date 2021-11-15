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

Block::Block(GLfloat* initMatrix){    
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
    this->direction = NA;
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

void Block::setDirection(Direction directionToRoll){
    this->direction = directionToRoll;
    std::cout << this->posX1 << ", " << this->posZ1 << std::endl;
}

void Block::update(){

    if(this->direction == NA)
        return;

    Quaternion tempQuat;
    switch(this->direction){
        case Up:{
            tempQuat = slerp(this->quat, Quaternion(90, -1, 0, 0), 1);

            this->posZ2 = this->posZ1;
            if(this->orientation == Standing)
                this->posZ1 -= 2;
            else
                this->posZ1 -= 1;

            this->offsetZ = this->posZ1; 

            
            break;
        }
        case Down:{
            tempQuat = slerp(this->quat, Quaternion(90, 1, 0, 0), 1);

            this->posZ1 = this->posZ2;
            if(this->orientation == Standing)
                this->posZ2 += 2;
            else
                this->posZ2 += 1;

            this->offsetZ = this->posZ1; 
            break;
        }
        case Left:{
            tempQuat = slerp(this->quat, Quaternion(90, 0, 0, -1), 1);

            this->posX2 = this->posX1;
            if(this->orientation == Standing)
                this->posX1 -= 2;
            else
                this->posX1 -= 1;

            
            this->offsetX = this->posX1;
            break;
        }
        case Right:{
            tempQuat = slerp(this->quat, Quaternion(90, 0, 0, 1), 1);

            this->posX1 = this->posX2;
            if(this->orientation == Standing)
                this->posX2 += 2;
            else
                this->posX2 += 1;

            this->offsetX = this->posX1;
            break;
        }
    }

    this->setOrientation();
    this->quat = tempQuat * this->quat;
    this->quat.populateRotationMatrix(this->rotationMatrix);

    this->setDirection(NA);
}