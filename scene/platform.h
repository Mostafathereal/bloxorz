#ifndef PLATFORM_H
#define PLATFORM_H

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
#include "../blockOrientations.h"


class Platform{
    public:
        std::vector<std::vector<int>> tiles;

        Platform(std::vector<std::vector<int>> &map);
        Platform();

        void drawPlatform();

        int checkFall(int posX1, int posZ1, int posX2, int posZ2, Orientation orientation);

};

#endif