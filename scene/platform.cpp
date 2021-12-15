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


// float mat_ambient[] ={ 0.24725f, 0.1995f, 0.0745f, 1.0f };
// float mat_diffuse[] = {0.75164f, 0.60648f, 0.22648f, 1.0f };
// float mat_specular[] ={0.628281f, 0.555802f, 0.366065f, 1.0f };
// float shine[] = {51.2f} ;
float mat_ambient[] ={ 1,1,1,1 };
float mat_diffuse[] = { 1,1,1,1 };
float mat_specular[] ={ 1,1,1,1 };
float shine[] = {32};

Platform::Platform(std::vector<std::vector<int>> &map){
    this->tiles = map;
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

    float colors[2][3] = {{1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}};

        for(int row = 0; row < this->tiles.size(); ++row){
            for(int col = 0; col < this->tiles[0].size(); ++col){
                if (this->tiles[row][col] != 0){
                    // glColor3fv(colors[((row%2) + col)%2]);
                    TextureList[this->tiles[row][col] - 1].setTexture();
                    // std::cout << colors[((row%2) + col)%2] << std::endl;
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
    std::cout << posX1 << ", " << posZ1 << ", " << posX2 << ", " << posZ2 << ", " << orientation << std::endl;

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

/*
types of fall = {full fall, half fall, no fall}
{type of fall} func(bounds_of_box, possibly orientation){

    if block is upright{
        check the 1 one block it is on
        if no tile{
            return fall attributes 
        }
    }
    if block is sideways{
        check block
        check other block
        if no tile{
            rotation along which axis?
        }
    }
}
*/

/*
POINTERS

define a pointer

int x = 8;

int *p = x;
is the same as ...
int *p;
p = &x;

*p -> will give 8
p -> adress of x
&p -> adress of pointer itself (not really relevent)
*/