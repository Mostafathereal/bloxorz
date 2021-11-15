#ifndef QUATERNION_H
#define QUATERNION_H

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

class Quaternion{
    public:
    float w;
    float x;
    float y;
    float z;

    Quaternion();
    Quaternion(float angle, float x, float y, float z);
    Quaternion(float rotX, float rotY, float rotZ);
    Quaternion operator*(Quaternion otherQuat);
    void populateRotationMatrix(GLfloat* matrix);
    void normalize();
};

Quaternion slerp(Quaternion qa, Quaternion qb, float t);

#endif