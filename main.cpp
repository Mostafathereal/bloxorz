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


/* Display screens*/
bool mainMenu = true;
bool displayObjects = false;
bool winScreen = false;
bool looseScreen = false;


/* camera */
float camPos[] = {8, 10, 10}; 

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

//Polished bronze
float ambMat2[4] ={0.25f, 0.148f, 0.06475f, 1.0f  };
float diffMat2[4] ={0.4f, 0.2368f, 0.1036f, 1.0f };
float specMat2[4] ={0.774597f, 0.458561f, 0.200621f, 1.0f };
float shine2 =76.8f ;

// Jade
// float ambMat3[4] = {0.5,0.5,0.5,1};
// float diffMat3[4] = {0,1,0,1};
// float specMat3[4] = {0,1,0,1};
// float shine3 = 27.0f;

// float ambMat2[4] = {0.5,0.5,0.5,1};
// float diffMat2[4] = {0,1,0,1}; 
// float specMat2[4] = {0,1,0,1};


/* Block */
float platformSize = 200;
float base[3] = {0, -1, 0};

/******
 * Platform object 
 */
std::vector<std::vector<int>> platform_map = {
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 1, 1, 1, 1, 1, 1, 0, 0},
	{0, 0, 1, 3, 1, 1, 1, 1, 0, 0},
	{0, 0, 1, 1, 1, 1, 1, 1, 0, 0},
	{0, 0, 1, 1, 1, 1, 2, 1, 0, 0},
	{0, 0, 1, 1, 1, 2, 1, 1, 0, 0},
	{0, 0, 1, 1, 1, 1, 1, 1, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

std::vector<std::vector<int>> platform_map1 = {
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 1, 1, 1, 1, 1, 1, 0, 0},
	{0, 0, 1, 3, 1, 1, 2, 1, 0, 0},
	{0, 0, 1, 1, 1, 0, 1, 1, 0, 0},
	{0, 0, 1, 1, 1, 1, 2, 1, 0, 0},
	{0, 0, 1, 2, 1, 2, 1, 1, 0, 0},
	{0, 0, 1, 1, 1, 1, 1, 1, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

std::vector<std::vector<int>> platform_map2 = {
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 1, 1, 1, 1, 1, 0, 0, 0},
	{0, 0, 1, 3, 1, 0, 0, 1, 0, 0},
	{0, 0, 2, 2, 1, 1, 1, 1, 0, 0},
	{0, 0, 1, 1, 1, 1, 2, 1, 0, 0},
	{0, 0, 1, 1, 1, 2, 1, 1, 0, 0},
	{0, 0, 1, 1, 0, 1, 1, 2, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

Platform platform(platform_map, 2, 2);
Platform platform1(platform_map1, 2, 2);
Platform platform2(platform_map2, 2, 2);

std::vector<Platform> platformList = {platform, platform1, platform2};
int platformLevel = 0; 

/*
 *block object
 */
GLfloat initMatrix[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
Block block(initMatrix, platform);

void keyboard(unsigned char key, int x, int y)
{

	if (key == 'q'){
		exit(0);
	}

	/* key presses move the cube, if it isn't at the extents (hard-coded here) */
	if (displayObjects){
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
				break;
		}
	}

	if (mainMenu || looseScreen || winScreen){
		switch (key)
		{
			case ' ':
				mainMenu = false;
				displayObjects = true;
				winScreen = false;
				looseScreen = false;
				break;
		}
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
	glOrtho(-13, 13, -13, 13, -13, 80);

	glEnable(GL_TEXTURE_2D);

    block.changeTexture("textures/wood_texture.ppm");
	block.texture.setTexture();
    
    
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//glDisable(GL_LIGHTING);
	//glDisable(GL_LIGHT0);
	// gluPerspective(45, 1, 1, 100);
} 

void displayText(std::string s, std::string position)
{
    glColor3f(0.0f, 0.0f, 0.0f);
	if (position == "TopRight") {
	    glRasterPos3i(4, 11, -4);
	}
	else if (position == "BottomRight") {
	    glRasterPos3i(4, -11, -4);
	}
	else if (position == "TopLeft") {
	    glRasterPos3i(-7, 11, 7);
	}
	else if (position == "BottomLeft") {
	    glRasterPos3i(-7, -11, 7);
	}
	else if (position == "Center") {
	    glRasterPos3i(1, 5, 5);
	}
	else {
	    glRasterPos3i(0, 0, 0);
	}
    for (std::string::iterator i = s.begin(); i != s.end(); ++i)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *i);
    }
}

void showText(std::string s, Point3D pos, Point3D color)
{
    glColor3f(color.mX, color.mY, color.mZ);
	glRasterPos3i(pos.mX, pos.mY, pos.mZ);
    for (std::string::iterator i = s.begin(); i != s.end(); ++i)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *i);
    }
}

/* display function - GLUT display callback function
 *		clears the screen, sets the camera position, draws the ground plane and movable box
 */
void display(void){

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(camPos[0], camPos[1], camPos[2], 0,0,0, 0,1,0);
	glColor3f(1,1,1);

	// displayText("This is text", "TopRight");
	// displayText("This is text", "BottomRight");
	// displayText("This is text", "TopLeft");
	// displayText("This is text", "BottomLeft");
	// displayText("This is text", "Center");

	if(displayObjects){
		glClearColor(0.5, 0.5, 0.5, 0);


		glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
		glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, diff);
		glLightfv(GL_LIGHT0, GL_SPECULAR, spec);

		// draw platform
		block.platform.drawPlatform();

		// glDisable(GL_LIGHTING);
		block.update();
		glPushMatrix();
			// glMultMatrixf(block.rotationMatrix);
			//glRotatef(90, 0, 0, 1);
			block.texture.setTexture();
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambMat2);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffMat2);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specMat2);
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shine2);
			block.drawBlock();
		glPopMatrix();

		// glBegin(GL_LINES);

		// glColor3f (1.0, 1.0, 0.0);
		// glVertex3f(0.0, 0.0, 0.0);
		// glVertex3f(5.0, 0.0, 0.0);

		// glColor3f (1.0, 1.0, 0.0);
		// glVertex3f(0.0, 0.0, 0.0);
		// glVertex3f(0.0, 5.0, 0.0);

		// glColor3f (1.0, 1.0, 0.0);
		// glVertex3f(0.0, 0.0, 0.0);
		// glVertex3f(0.0, 0.0, 5.0);
		// glEnd();

	}

	if (mainMenu){
		// glMatrixMode(GL_PROJECTION);
		glClearColor(0.403, 0.094, 0.094, 0);
		showText("Main menu", Point3D(2.2, 17, 5), Point3D(0.964, 0.905, 0.572));
	}

	if (winScreen){
		// glMatrixMode(GL_PROJECTION);
		glClearColor(0.403, 0.094, 0.094, 0);
		showText("You Won!!!", Point3D(2.2, 17, 5), Point3D(0.964, 0.905, 0.572));
	}

	if (looseScreen){
		// glMatrixMode(GL_PROJECTION);
		glClearColor(0.403, 0.094, 0.094, 0);
		showText("You lost :(", Point3D(2.2, 17, 5), Point3D(0.964, 0.905, 0.572));

	}

	glutSwapBuffers();
}

void FPS(int value){
	glutPostRedisplay();
	glutTimerFunc(17, FPS, 0);
	if (block.gameState == 2){
		displayObjects = false;
		mainMenu = false;
		winScreen = false;
		looseScreen = true;
		platformLevel = 0;
		block.reset(platformList[platformLevel]);

	}
	else if (block.gameState == 4){
		if (platformLevel == platformList.size() - 1){
			displayObjects = false;
			mainMenu = false;
			winScreen = true;
			looseScreen = false;
		}
		platformLevel = (platformLevel + 1) % platformList.size();
		block.reset(platformList[platformLevel]);


	}
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
