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

Platform::Platform(std::vector<std::vector<int>> &map){
    this->tiles = map;
}

void Platform::drawPlatform(){
    glPushMatrix();
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