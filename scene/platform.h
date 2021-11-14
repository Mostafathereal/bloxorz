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

class Platform{
    public:
        std::vector<std::vector<int>> tiles;

        Platform(std::vector<std::vector<int>> &map);

        void drawPlatform();

};

#endif