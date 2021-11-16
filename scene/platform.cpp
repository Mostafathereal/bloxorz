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
#include <iostream>

float mat_ambient[] ={ 0.24725f, 0.1995f, 0.0745f, 1.0f };
float mat_diffuse[] = {0.75164f, 0.60648f, 0.22648f, 1.0f };
float mat_specular[] ={0.628281f, 0.555802f, 0.366065f, 1.0f };
float shine[] = {51.2f} ;

Platform::Platform(std::vector<std::vector<int>> &map){
    this->tiles = map;
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
                if (this->tiles[row][col] == 1){
                    glColor3fv(colors[((row%2) + col)%2]);
                    // std::cout << colors[((row%2) + col)%2] << std::endl;
                    glBegin(GL_QUADS);
                        glVertex3f(row + 0.1, 0, col + 0.1);
                        glVertex3f(row + 0.1, 0, col + 0.9);
                        glVertex3f(row + 0.9, 0, col + 0.9);
                        glVertex3f(row + 0.9, 0, col + 0.1);
                    glEnd();

                }
            }
        }
    glPopMatrix();
    

}


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