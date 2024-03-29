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
#include "platform.h"
#include "../TextureList.h"
#include <iostream>
#include "../blockOrientations.h"


float mat_ambient[] ={ 1,1,1,1 };
float mat_diffuse[] = { 1,1,1,1 };
float mat_specular[] ={ 1,1,1,1 };
float shine[] = {32};

Platform::Platform(std::vector<std::vector<int>> &map, int startX, int startY){
    this->tiles = map;
    this->startingX = startX;
    this->startingY = startY;
}

Platform::Platform(){
    this->tiles = std::vector<std::vector<int>>();
}


void Platform::drawPlatform(){
    glPushMatrix();
    glNormal3f(0, 1, 0);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);

        for(int row = 0; row < this->tiles.size(); ++row){
            for(int col = 0; col < this->tiles[0].size(); ++col){
                if (this->tiles[row][col] != 0){
                    TextureList[this->tiles[row][col] - 1].setTexture();
                    glBegin(GL_QUADS);
                        glTexCoord2f(0, 0);
                        glVertex3f(row + 0.05, 0, col + 0.05);
                        glTexCoord2f(0, 1);
                        glVertex3f(row + 0.05, 0, col + 0.95);
                        glTexCoord2f(1, 1);
                        glVertex3f(row + 0.95, 0, col + 0.95);
                        glTexCoord2f(1, 0);
                        glVertex3f(row + 0.95, 0, col + 0.05);
                    glEnd();
                }
            }
        }
    glPopMatrix();
}

// standing fall = full fall, no fall
// sideways: half fall, full fall, no fall
// no fall = 0, half fall right = 1, half fall left = 2, full fall = 3, win = 4

int Platform::checkFall(int posX1, int posZ1, int posX2, int posZ2, Orientation orientation){

    int fallType = 0;

    if (orientation == Standing){
        // full fall condition
        if (this->tiles[posX1][posZ1] == 0){ fallType = 3; }
        // winning condition
        if (this->tiles[posX1][posZ1]  == 3){ fallType = 4; }
        //update one-time use tiles
        if (this->tiles[posX1][posZ1] == 2){ this->tiles[posX1][posZ1] = 0; }
    }

    else if (orientation == HorizontalInX){
        bool missingLeft = this->tiles[posX1][posZ1] == 0;
        bool missingRight = this->tiles[posX1 + 1][posZ1] == 0;
        if (missingLeft && missingRight){ fallType = 3;}
        else if (missingLeft){ fallType = 2; }
        else if (missingRight){ fallType = 1; }
        //update one-time use tiles
        if (this->tiles[posX1][posZ1] == 2){ this->tiles[posX1][posZ1] = 0; }
        if (this->tiles[posX1 + 1][posZ1] == 2){ this->tiles[posX1 + 1][posZ1] = 0; }
    }
    else{
        bool missingUp = this->tiles[posX1][posZ1] == 0;
        bool missingDown = this->tiles[posX1][posZ1 + 1] == 0;
        if (missingUp && missingDown){ fallType = 3 ;}
        else if (missingUp){ fallType = 2; }
        else if (missingDown){ fallType = 1; }
        //update one-time use tiles
        if (this->tiles[posX1][posZ1] == 2){ this->tiles[posX1][posZ1] = 0; }
        if (this->tiles[posX1][posZ1 + 1] == 2){ this->tiles[posX1][posZ1 + 1] = 0; }
    }

    return fallType;
}