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
#include <cmath>
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
    this->posX1 = this->platform.startingX;
    this->posZ1 = this->platform.startingY;

    //position of bottom right corner of block
    this->posX2 = this->posX1 + this->baseLength;
    this->posZ2 = this->posZ1 + this->baseLength;


    //Both these points are normalized around origin
    //origin point of block in real time (keeps track of origin in between animations as well)
    this->offset.setPoint(this->baseLength / 2, this->heightLength / 2, this->baseLength / 2);

    //origin point of block (only keeps track of origin inbetween animations)
    this->origin.setPoint(this->baseLength / 2, this->heightLength / 2, this->baseLength / 2);

    //orientation of block (starts out standing)
    this->orientation = Standing;

    //direction of rotation (NA when not in animation rotation)
    this->direction = NA;

    //rotation increments and t (neeeded for SLERP interpolation). rotIncrement = 1/x where x is number of frames for animation.
    this->rotIncrement = 1.0f / 10;
    this->currentT = 0;

    // offsets to make sure right and down rotate properly since their axis of rotation is not at origin
    this->RightRotationOffset = this->baseLength;
    this->DownRotationOffset = this->baseLength;

    //move history (store as undo moves)
    this->undoMoveHist = std::vector<Direction>();

    //number of moves
    this->numMoves = 0;

    //game state, playing = 0, losingAnimation = 1, Lost = 2, winning Animation = 3, Won = 4
    this->gameState = 0;

    // falling animation type, -1 when not set
    this->fallingAnimationType = -1;
    
    //setTextureId
    this->textureID = 0;
}

void Block::reset(Platform platform){
    this->platform = platform;

    this->quat = Quaternion();
    this->rotQuat = Quaternion();
    this->rotQuat.populateRotationMatrix(this->rotationMatrix);

    this->platform = platform;


    //base length and height of block
    this->baseLength = 1;
    this->heightLength = 2;

    //postion of top left corner of block
    this->posX1 = this->platform.startingX;
    this->posZ1 = this->platform.startingY;

    //position of bottom right corner of block
    this->posX2 = this->posX1 + this->baseLength;
    this->posZ2 = this->posZ1 + this->baseLength;


    //Both these points are normalized around origin
    //origin point of block in real time (keeps track of origin in between animations as well)
    this->offset.setPoint(this->baseLength / 2, this->heightLength / 2, this->baseLength / 2);

    //origin point of block (only keeps track of origin inbetween animations)
    this->origin.setPoint(this->baseLength / 2, this->heightLength / 2, this->baseLength / 2);

    //orientation of block (starts out standing)
    this->orientation = Standing;

    //direction of rotation (NA when not in animation rotation)
    this->direction = NA;

    //rotation increments and t (neeeded for SLERP interpolation). rotIncrement = 1/x where x is number of frames for animation.
    this->rotIncrement = 1.0f / 10;
    this->currentT = 0;

    // offsets to make sure right and down rotate properly since their axis of rotation is not at origin
    this->RightRotationOffset = this->baseLength;
    this->DownRotationOffset = this->baseLength;

    //move history (store as undo moves)
    this->undoMoveHist = std::vector<Direction>();

    //game state, playing = 0, losingAnimation = 1, Lost = 2, winning Animation = 3, Won = 4
    this->gameState = 0;

    // falling animation type, -1 when not set
    this->fallingAnimationType = -1;

    //setTextureId
    this->textureID = 0;
}

void Block::drawBlock(){
    glPushMatrix();
        //translate matrix to current origin of block
        glTranslatef(this->posX1 + this->offset.mX, this->offset.mY, this->posZ1 + this->offset.mZ);

        //apply rotation transformations to model view matrix
        glMultMatrixf(this->rotationMatrix);

        //scale block and draw
        glScalef(this->baseLength, this->heightLength, this->baseLength);
        glutTexturedSolidCube(1);
    glPopMatrix();
}

void Block::setOrientation(){
    if(this->orientation == Standing){
        if(abs(this->posX2 - this->posX1) > 1){
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

    if(this->orientation == Standing){
        std::cout << "Standing" << std::endl;
    }
    else if (this->orientation == HorizontalInX){
        std::cout << "Horizontal in X" << std::endl;
    }
    else{
        std::cout << "Horizontal in Z" << std::endl;
    }
}
void Block::setOrigin(){
    //set the cube origin coords and rotation offsets to normalized values based their orientation
    if(this->orientation == Standing){
        this->origin.setPoint(this->baseLength / 2, this->heightLength / 2, this->baseLength / 2);
        this->offset.setPoint(this->baseLength / 2, this->heightLength / 2, this->baseLength / 2);

        this->RightRotationOffset = this->baseLength;
        this->DownRotationOffset = this->baseLength;
    }
    else if(this->orientation == HorizontalInX){
        this->origin.setPoint(this->heightLength / 2, this->baseLength / 2, this->baseLength / 2);
        this->offset.setPoint(this->heightLength / 2, this->baseLength / 2, this->baseLength / 2);

        this->RightRotationOffset = this->heightLength;
        this->DownRotationOffset = this->baseLength;
    }
    else{
        this->origin.setPoint(this->baseLength / 2, this->baseLength / 2, this->heightLength / 2);
        this->offset.setPoint(this->baseLength / 2, this->baseLength / 2, this->heightLength / 2);

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
    // if game is over
    else if (this->gameState == 2){
        std::cout << "game Over!!" << std::endl;
        return;
    }
    // if game is won
    else if (this->gameState == 4){
        std::cout << "game Won!!!" << std::endl;
        return;
    }
    // if game is lost and loss animation needs to be displayed
    else if (this->gameState == 1){
        fallingAnimation();
        return;
    }
    //if game is won and winning animation needs to be displayed
    else if (this->gameState == 3){
        winningAnimation();
        return;
    }
    else
        this->currentT += this->rotIncrement;
    

    //declare tempQuat to record next quaternion in rotation
    Quaternion tempQuat;
    switch(this->direction){
        case UndoDown:
        case Up:{
            tempQuat = blockRotation(Quaternion(90, -1, 0, 0), 0, 0);
            break;
        }
        case UndoUp:
        case Down:{
            tempQuat = blockRotation(Quaternion(90, 1, 0, 0), 0, this->DownRotationOffset);
            break;
        }
        case UndoRight:
        case Left:{
            tempQuat = blockRotation(Quaternion(90, 0, 0, 1), 0, 0);
            break;
        }
        case UndoLeft:
        case Right:{
            tempQuat = blockRotation(Quaternion(90, 0, 0, -1), this->RightRotationOffset, 0);
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


        // get state of block after move: either the block is on platform (0), its off of platform (1, 2, 3 for different fall types), or its properly on winning tile (4)
        int gameStateUpdate = this->platform.checkFall((int)this->posX1, (int)this->posZ1, (int)this->posX2, (int)this->posZ2, this->orientation);
        if (gameStateUpdate == 0){
            this->direction = NA;
        }
        else if (gameStateUpdate <= 3){
            this->fallingAnimationType = gameStateUpdate;
            this->gameState = 1;
        }
        else{
            this->gameState = 3;
        }   
        // ...................

        
        
    }
}

void Block::updatePosition(){

    //update logic to update both the positions
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

// void Block::changeTexture(char* file){
//     free(this->texture.img);
//     this->texture = Texture2D(file);
// }

void Block::fallingAnimation(){
    int fallType = this->fallingAnimationType;
    Quaternion tempQuat;

    //if rotating fall animation is done, start free fall animation
    if (this->currentT >= 1){
        this->freeFallAnimation();
        return;
    }

    //update t for rotating fall animation
    this->currentT += this->rotIncrement;

    // if block is completely off the platform
    if(fallType == 3){
        switch(this->direction){
            case Up:{
                tempQuat = blockRotation(Quaternion(90, -1, 0, 0), 0, this->DownRotationOffset);
                break;
            }
            case Down:{
                tempQuat = blockRotation(Quaternion(90, 1, 0, 0), 0, 0);
                break;
            }
            case Left:{
                tempQuat = blockRotation(Quaternion(90, 0, 0, 1), this->RightRotationOffset, 0);
                break;
            }
            case Right:{ 
                tempQuat = blockRotation(Quaternion(90, 0, 0, -1), 0, 0);
                break;
            }
            default:{}
        }
    }
    // if block's bottom or right side is on platform
    else if(fallType == 2){
        if(this->orientation == HorizontalInZ){
            tempQuat = blockRotation(Quaternion(90, -1, 0, 0), 0, this->DownRotationOffset/2);
        }
        else if(this->orientation == HorizontalInX){
            tempQuat = blockRotation(Quaternion(90, 0, 0, 1), this->RightRotationOffset/2, 0);
        }
    }
    // if block's top or left side is on platform
    else{
        if(this->orientation == HorizontalInZ){
            tempQuat = blockRotation(Quaternion(90, 1, 0, 0), 0, this->DownRotationOffset/2);
        }
        else if(this->orientation == HorizontalInX){
            tempQuat = blockRotation(Quaternion(90, 0, 0, -1), this->RightRotationOffset/2, 0);
        }
    }
    
    //set rot quaternion and populate rotation matrix accordingly
    this->rotQuat = tempQuat;
    this->rotQuat.populateRotationMatrix(this->rotationMatrix);

    if(this->currentT >= 1){
        //need to correct falling direction when half of block is on platform
        if(fallType == 2){
            if(this->orientation == HorizontalInZ){
                this->direction = Up;
            }
            else if(this->orientation == HorizontalInX){
                this->direction = Left;
            }
        }
        else if (fallType == 1){
            if(this->orientation == HorizontalInZ){
                this->direction = Down;
            }
            else if(this->orientation == HorizontalInX){
                this->direction = Right;
            }
        }
    }
}

void Block::freeFallAnimation(){
    Quaternion tempQuat;

    //rotate block in falling direction
    switch(this->direction){
        case Up:{
            tempQuat = Quaternion(6, -1, 0, 0);
            break;
        }
        case Down:{
            tempQuat = Quaternion(6, 1, 0, 0);
            break;
        }
        case Left:{
            tempQuat = Quaternion(6, 0, 0, 1);
            break;
        }
        case Right:{ 
            tempQuat = Quaternion(6, 0, 0, -1);
            break;
        }
        default:{}
    }

    this->rotQuat = tempQuat * this->rotQuat;
    this->rotQuat.populateRotationMatrix(this->rotationMatrix);
    this->offset.mY -= 0.1;

    //falling animations over, set game state to lost
    if(this->offset.mY <= -10)
        this->gameState = 2;
}

void Block::winningAnimation(){
    this->offset.mY -= 0.02;

    //winning animation over, set game state to won
    if(this->offset.mY <= -this->heightLength/2)
        this->gameState = 4;
}

Quaternion Block::blockRotation(Quaternion rotateQuaternion, float XOffset, float ZOffset){
    // used 2 quaternions here. blockRotationQuat is for the total rotation of the block (from the beginning). While pointRotationQuat is for rotating the normalized block point
    Quaternion blockRotationQuat = slerp(this->quat, rotateQuaternion * this->quat, this->currentT);
    Quaternion pointRotationQuat = slerp(Quaternion(), rotateQuaternion, this->currentT);
    
    Point3D rotatedPoint = rotateAboutUnitQuaternion(pointRotationQuat, Point3D(this->origin.mX - XOffset, this->origin.mY, this->origin.mZ - ZOffset));
    this->offset.setPoint(rotatedPoint.mX + XOffset, rotatedPoint.mY, rotatedPoint.mZ + ZOffset);
    return blockRotationQuat;
}