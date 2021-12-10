#ifndef GLUT_TEXTURED_SOLID_CUBE_H
#define GLUT_TEXTURED_SOLID_CUBE_H

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

void glutTexturedSolidCube(GLdouble size);

#endif