#ifndef TEXTURE_2D_H
#define TEXTURE_2D_H

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

class Texture2D{
    public:
    GLubyte* img;
    int width;
    int height;
    int max;

    Texture2D();
    Texture2D(char* file);
    void setTexture();
};

GLubyte* LoadPPM(char* file, int* width, int* height, int* max);

#endif