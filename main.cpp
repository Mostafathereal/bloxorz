/*
 * Snowman code!
 * Draws a snowman using hierarchies.
 * Snowman moves with WASD
 * Includes a box with list of vertices for the floor
 */
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
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "scene/block.h"
#include <iostream>
#include "scene/platform.h"
#include "texture2D.h"


float camPos[] = {5, 5, 5};


/* LIGHTING */

float light_pos[4] = {5,2,5,1};
float amb[4] = {1,1,1,1};
float diff[4] = {1,1,1,1};
float spec[4] = {1,1,1,1};


/* Materials */

float ambMat[4] = {0.5,0.5,0.5,1};
float diffMat[4] = {1,0,0,1};
float specMat[4] = {0,1,0,1};


// Obsidian
// float ambMat2[4] ={ 0.05375f, 0.05f, 0.06625f, 0.82f };
// float diffMat2[4] ={ 0.18275f, 0.17f, 0.22525f, 0.82f};
// float specMat2[4] ={0.332741f, 0.328634f, 0.346435f, 0.82f };
// float shine2 = 38.4f ;

// Jade
float ambMat2[4] = {0.5,0.5,0.5,1};
float diffMat2[4] = {0,1,0,1};
float specMat2[4] = {0,1,0,1};
float shine2 = 27.0f;


/* Block */
float platformSize = 200;
float base[3] = {0, -1, 0};


/*
 *block object
 */
GLfloat initMatrix[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
Block block(initMatrix);

/******
 * Platform object 
 */
std::vector<std::vector<int>> platform_map = {
	{1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1}
};
Platform platform(platform_map);

void keyboard(unsigned char key, int x, int y)
{

	/* key presses move the cube, if it isn't at the extents (hard-coded here) */
	switch (key)
	{
		case 'a':
		case 'A':
			block.setDirection(Left);
			break;

		case 'w':
		case 'W':
			block.setDirection(Up);
			break;

		case 'd':
		case 'D':
			block.setDirection(Right);
			break;

		case 's':
		case 'S':
			block.setDirection(Down);
			break;
		case 'z':
		case 'Z':
			block.undoMove();
	}
}


void init(void)
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glClearColor(0.5, 0.5, 0.5, 0);
	glColor3f(1, 1, 1);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-10, 10, -10, 10, -10, 80);

	glEnable(GL_TEXTURE_2D);

    block.changeTexture("textures/lava_texture.ppm");
	block.texture.setTexture();
    
    
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//glDisable(GL_LIGHTING);
	//glDisable(GL_LIGHT0);
	// gluPerspective(45, 1, 1, 100);
} 


/* display function - GLUT display callback function
 *		clears the screen, sets the camera position, draws the ground plane and movable box
 */
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(camPos[0], camPos[1], camPos[2], 0,0,0, 0,1,0);
	glColor3f(1,1,1);

	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diff);
	glLightfv(GL_LIGHT0, GL_SPECULAR, spec);


	// draw platform
	platform.drawPlatform();

	// glDisable(GL_LIGHTING);
	block.update();
	glPushMatrix();
		// glMultMatrixf(block.rotationMatrix);
		//glRotatef(90, 0, 0, 1);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambMat2);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffMat2);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specMat2);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shine2);
        block.drawBlock();
	glPopMatrix();

	glBegin(GL_LINES);

    glColor3f (1.0, 1.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(5.0, 0.0, 0.0);

    glColor3f (1.0, 1.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 5.0, 0.0);

    glColor3f (1.0, 1.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 5.0);
    glEnd();



	glutSwapBuffers();
}

void FPS(int value){
	glutPostRedisplay();
	glutTimerFunc(17, FPS, 0);
}

/* main function - program entry point */
int main(int argc, char** argv)
{
	glutInit(&argc, argv);		//starts up GLUT
	
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
    
	
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(100, 100);

	glutCreateWindow("Bloxorz");	//creates the window

	glutDisplayFunc(display);	//registers "display" as the display callback function
	glutKeyboardFunc(keyboard);

	glutTimerFunc(17, FPS, 0);

	glEnable(GL_DEPTH_TEST);
    
	// glFrontFace(GL_CCW);
	// glCullFace(GL_BACK);
	// glEnable(GL_CULL_FACE);
    
	init();

	glutMainLoop();				//starts the event loop

	return(0);					//return may not be necessary on all compilers
}
