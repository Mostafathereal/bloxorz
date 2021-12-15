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
#include "../texture2D.h"
#include "../blockOrientations.h"
#include "../mathLib3D.h"
#include "../glutTexturedSolidCube.h"
#include "platform.h"

// Block::Block(){   
    
// }

Block::Block(GLfloat* initMatrix, Platform platform){   
    //initialize quaternions to keep track of orientation and rotation between animations
    this->quat = Quaternion();
    this->rotQuat = Quaternion();
    this->rotationMatrix = initMatrix;
    this->rotQuat.populateRotationMatrix(this->rotationMatrix);

    this->platform = platform;


    //base length and height of block
    this->baseLength = 1;
    this->heightLength = 2;

    //postion of top left corner of block
    this->posX1 = 2;
    this->posZ1 = 2;

    //position of bottom right corner of block
    this->posX2 = this->posX1 + this->baseLength;
    this->posZ2 = this->posZ1 + this->baseLength;


    //Both these origin points are normalized around origin
    //origin point of block in real time (keeps track of origin in between animations as well)
    this->offsetX = this->baseLength / 2;
    this->offsetY = this->heightLength / 2;
    this->offsetZ = this->baseLength / 2;

    //origin point of block (only keeps track of origin inbetween animations)
    this->originX = this->baseLength / 2;
    this->originY = this->heightLength / 2;
    this->originZ = this->baseLength / 2;

    //orientation of block (starts out standing)
    this->orientation = Standing;

    //direction of rotation (NA when not in animation rotation)
    this->direction = NA;

    //rotation increments and t (neeeded for SLERP interpolation)
    this->rotIncrement = 1.0f / 10;
    this->currentT = 0;

    // offsets to make sure right and down rotate properly since their axis of rotation is not at origin
    this->RightRotationOffset = this->baseLength;
    this->DownRotationOffset = this->baseLength;

    //move history (store as undo moves)
    this->undoMoveHist = std::vector<Direction>();

    //number of moves
    this->numMoves = 0;

    //default texture is already set (no need to do it here, we use malloc for texture array)
}

void Block::drawBlock(){
    glPushMatrix();
        //translate matrix to current origin of block
        glTranslatef(this->posX1 + this->offsetX, this->offsetY, this->posZ1 + this->offsetZ);

        //apply rotation transformations to model view matrix
        glMultMatrixf(this->rotationMatrix);

        //scale block adn draw
        glScalef(this->baseLength, this->heightLength, this->baseLength);
        glutTexturedSolidCube(1);
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
    }
    else if (this->orientation == HorizontalInX){
        if (this->posX2 - this->posX1 == 1){
            this->orientation = Standing;
        }
    }
    else{
        if (this->posZ2 - this->posZ1 == 1){
            this->orientation = Standing;
        }
    }
}
void Block::setOrigin(){
    //set the cube origin coords and rotation offsets to normalized values based their orientation
    if(this->orientation == Standing){
        this->originX = this->baseLength / 2;
        this->originY = this->heightLength / 2;
        this->originZ = this->baseLength / 2;

        this->offsetX = this->baseLength / 2;
        this->offsetY = this->heightLength / 2;
        this->offsetZ = this->baseLength / 2;

        this->RightRotationOffset = this->baseLength;
        this->DownRotationOffset = this->baseLength;
    }
    else if(this->orientation == HorizontalInX){
        this->originX = this->heightLength / 2;
        this->originY = this->baseLength / 2;
        this->originZ = this->baseLength / 2;

        this->offsetX = this->heightLength / 2;;
        this->offsetY = this->baseLength / 2;
        this->offsetZ = this->baseLength / 2;

        this->RightRotationOffset = this->heightLength;
        this->DownRotationOffset = this->baseLength;
    }
    else{
        this->originX = this->baseLength / 2;
        this->originY = this->baseLength / 2;
        this->originZ = this->heightLength / 2;;

        this->offsetX = this->baseLength / 2;
        this->offsetY = this->baseLength / 2;
        this->offsetZ = this->heightLength / 2;;

        this->RightRotationOffset = this->baseLength;
        this->DownRotationOffset = this->heightLength;
    }
}

void Block::setDirection(Direction directionToRoll){
    //set direction if direction is not set already (a shitty asnychronous wait)
    if(this->direction == NA)
        this->direction = directionToRoll;
}

void Block::undoMove(){
    if(this->direction == NA && this->undoMoveHist.size() > 0){
        this->direction = this->undoMoveHist.back();
        this->undoMoveHist.pop_back();
    }
    return;
        
}

void Block::update(){

    // if direction is not set (not currently animating), return or increment t to get next quaternion in the animation
    if(this->direction == NA)
        return;
    else
        this->currentT += this->rotIncrement;
    

    //declare tempQuat to record next quaternion in rotation
    Quaternion tempQuat;
    switch(this->direction){
        case UndoDown:
        case Up:{

            // used 2 quaternions here tempQuat is total rotations while tempRotQuat records rotation along the current animation (rotating up in this case)
            tempQuat = slerp(this->quat, Quaternion(90, -1, 0, 0)  * this->quat, this->currentT);
            Quaternion tempRotQuat = slerp(Quaternion(), Quaternion(90, -1, 0, 0), this->currentT);

            Point3D rotatedPoint = rotateAboutUnitQuaternion(tempRotQuat, Point3D(this->originX, this->originY, this->originZ));
            this->offsetX = rotatedPoint.mX;
            this->offsetY = rotatedPoint.mY;
            this->offsetZ = rotatedPoint.mZ;
            break;
        }
        case UndoUp:
        case Down:{
            tempQuat = slerp(this->quat, Quaternion(90, 1, 0, 0)  * this->quat, this->currentT);
            Quaternion tempRotQuat = slerp(Quaternion(), Quaternion(90, 1, 0, 0), this->currentT);

            Point3D rotatedPoint = rotateAboutUnitQuaternion(tempRotQuat, Point3D(this->originX, this->originY, this->originZ - this->DownRotationOffset));
            this->offsetX = rotatedPoint.mX;
            this->offsetY = rotatedPoint.mY;
            this->offsetZ = rotatedPoint.mZ + this->DownRotationOffset;
            break;
        }
        case UndoRight:
        case Left:{
            tempQuat = slerp(this->quat, Quaternion(90, 0, 0, 1)  * this->quat, this->currentT);
            Quaternion tempRotQuat = slerp(Quaternion(), Quaternion(90, 0, 0, 1), this->currentT);

            Point3D rotatedPoint = rotateAboutUnitQuaternion(tempRotQuat, Point3D(this->originX, this->originY, this->originZ));
            this->offsetX = rotatedPoint.mX;
            this->offsetY = rotatedPoint.mY;
            this->offsetZ = rotatedPoint.mZ;
            break;
        }
        case UndoLeft:
        case Right:{
            tempQuat = slerp(this->quat, Quaternion(90, 0, 0, -1) * this->quat, this->currentT);
            Quaternion tempRotQuat = slerp(Quaternion(), Quaternion(90, 0, 0, -1), this->currentT);
            
            Point3D rotatedPoint = rotateAboutUnitQuaternion(tempRotQuat, Point3D(this->originX - this->RightRotationOffset, this->originY, this->originZ));
            this->offsetX = rotatedPoint.mX + this->RightRotationOffset;
            this->offsetY = rotatedPoint.mY;
            this->offsetZ = rotatedPoint.mZ;
            break;
        }
        case NA:{}
    }

    //set rot quaternion and populate rotation matrix accordingly
    this->rotQuat = tempQuat;
    this->rotQuat.populateRotationMatrix(this->rotationMatrix);

    //if rotation is done, perform clean (reset directions. update positions, orientations, quat, and t)
    if (this->currentT >= 1){
        this->updatePosition();
        this->setOrientation();
        this->setOrigin();

        this->currentT = 0;
        this->quat.w = this->rotQuat.w;
        this->quat.x = this->rotQuat.x;
        this->quat.y = this->rotQuat.y;
        this->quat.z = this->rotQuat.z;

        this->numMoves += 1;
        if(this->direction == Up)
            this->undoMoveHist.push_back(UndoUp);
        else if (this->direction == Down)
            this->undoMoveHist.push_back(UndoDown);
        else if (this->direction == Left)
            this->undoMoveHist.push_back(UndoLeft);
        else if (this->direction == Right)
            this->undoMoveHist.push_back(UndoRight);
        else
            // -2 because 1 is added every time for moves
            this->numMoves -= 2;

        // ....................
        std::cout << this->platform.checkFall((int)this->posX1, (int)this->posZ1, (int)this->posX2, (int)this->posZ2, this->orientation) << std::endl; 
        // ...................

        this->direction = NA;
        
    }
}

void Block::updatePosition(){

    //update logic to update both the positions (will be used for collision later)
    switch(this->direction){
        case UndoDown:
        case Up:{
            this->posZ2 = this->posZ1;
            if(this->orientation == Standing)
                this->posZ1 -= this->heightLength;
            else
                this->posZ1 -= this->baseLength;
            break;
        }
        case UndoUp:
        case Down:{
            this->posZ1 = this->posZ2;
            if(this->orientation == Standing)
                this->posZ2 += this->heightLength;
            else
                this->posZ2 += this->baseLength;
            break;
        }
        case UndoRight:
        case Left:{
            this->posX2 = this->posX1;
            if(this->orientation == Standing)
                this->posX1 -= this->heightLength;
            else
                this->posX1 -= this->baseLength;
            break;
        }
        case UndoLeft:
        case Right:{
            this->posX1 = this->posX2;
            if(this->orientation == Standing)
                this->posX2 += this->heightLength;
            else
                this->posX2 += this->baseLength;
            break;
        }
        case NA:{}
    }
}

void Block::changeTexture(char* file){
    free(this->texture.img);
    this->texture = Texture2D(file);
}
