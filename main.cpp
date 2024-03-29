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
#include <string>
#include "scene/block.h"
#include <iostream>
#include "scene/platform.h"
#include "texture2D.h"


/* Display screens*/
bool mainMenu = true;
bool displayObjects = false;
bool winScreen = false;
bool looseScreen = false;

//screen width and height
int screenWidth = 800;
int screenHeight = 800;

/* camera */
float camPos[] = {20, 5, 15}; 
float lookAt[] = {9.5, 0, 9.5};

/* ortho plane size */
float orthoDist = 12;

/* LIGHTING */

float light_pos[4] = {5,2,5,1};
float amb[4] = {1,1,1,1};
float diff[4] = {1,1,1,1};
float spec[4] = {1,1,1,1};

/* Materials */
float ambMat[4] = {0.5,0.5,0.5,1};
float diffMat[4] = {1,0,0,1};
float specMat[4] = {0,1,0,1};

//Polished bronze
float ambMat2[4] ={0.25f, 0.148f, 0.06475f, 1.0f  };
float diffMat2[4] ={0.4f, 0.2368f, 0.1036f, 1.0f };
float specMat2[4] ={0.774597f, 0.458561f, 0.200621f, 1.0f };
float shine2 =76.8f ;

/* Block */
float platformSize = 200;
float base[3] = {0, -1, 0};


/* block textures */
Texture2D woodBlock("textures/wood.ppm");
Texture2D lavaBlock("textures/lava.ppm");

std::vector<Texture2D> blockTextures = {woodBlock, lavaBlock};

/******
 * Platform object 
 */
std::vector<std::vector<int>> platform_map = {
	{0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0},
	{0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0},
	{0,0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0,0},
	{0,0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 3, 1, 0,0},
	{0,0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0,0},
	{0,0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0,0},
	{0,0, 1, 1, 1, 1, 2, 2, 1, 1, 1, 1, 2, 2, 1, 1, 1, 0,0},
	{0,0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,0},
	{0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0},
	{0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0}
};

std::vector<std::vector<int>> platform_map1 = {
	{0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0},
	{0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0},
	{0,0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,0},
	{0,0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0,0},
	{0,0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0,0},
	{0,0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 3, 1, 0,0},
	{0,0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0,0},
	{0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0,0},
	{0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0},
	{0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0}
};

std::vector<std::vector<int>> platform_map2 = {
	{0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0},
	{0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0},
	{0,0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0,0},
	{0,0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0,0},
	{0,0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0,0},
	{0,0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0,0},
	{0,0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0,0},
	{0,0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 2, 2, 2, 0, 0, 0,0},
	{0,0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 2, 2, 2, 0, 0, 0,0},
	{0,0, 0, 0, 0, 0, 0, 1, 3, 1, 0, 0, 0, 0, 0, 0, 0,0},
	{0,0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,0},
	{0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0},
	{0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0}
};

Platform platform(platform_map, 3, 4);
Platform platform1(platform_map1, 3, 4);
Platform platform2(platform_map2, 7, 3);

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
				block.setDirection(Down);
				break;

			case 'w':
			case 'W':
				block.setDirection(Left);
				break;

			case 'd':
			case 'D':
				block.setDirection(Up);
				break;

			case 's':
			case 'S':
				block.setDirection(Right);
				break;
			case 'z':
			case 'Z':
				block.undoMove();
				break;
			case '1':
				block.textureID = 0;
				break;
			case '2':
				block.textureID = 1;
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
				block.numMoves = 0;
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
	glOrtho(-orthoDist, orthoDist, -orthoDist, orthoDist, -13, 80);

	glEnable(GL_TEXTURE_2D);
    
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

void showText(std::string s, Point3D pos, Point3D color)
{
    glColor3f(color.mX, color.mY, color.mZ);
	glRasterPos2f(pos.mX, pos.mY);
    for (std::string::iterator i = s.begin(); i != s.end(); ++i)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *i);
    }
}

/* display function - GLUT display callback function
 *		clears the screen, sets the camera position, draws the ground plane and movable box
 */
void display(void){

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-orthoDist, orthoDist, -orthoDist, orthoDist, -13, 80);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(camPos[0], camPos[1], camPos[2], lookAt[0], lookAt[1], lookAt[2], 0,1,0);
	glColor3f(1,1,1);

	if(displayObjects){
		glClearColor(0.5, 0.56862745, 0.56862745, 0);

		glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
		glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, diff);
		glLightfv(GL_LIGHT0, GL_SPECULAR, spec);

		// draw platform
		block.platform.drawPlatform();

		// glDisable(GL_LIGHTING);
		block.update();
		glPushMatrix();
			blockTextures[block.textureID].setTexture();
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambMat2);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffMat2);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specMat2);
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shine2);
			block.drawBlock();
		glPopMatrix();

		// clear all transformations, load gluOrtho2D for screen coords and display score
		glLoadIdentity();
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0, screenWidth, 0, screenHeight);
		showText("Score: " + std::to_string(block.numMoves), Point3D(screenWidth - 150, screenHeight - 25, 0), Point3D(0.964, 0.905, 0.572));
	}
	else{
		// clear all transformations, load gluOrtho2D for screen coords and display screen
		glLoadIdentity();
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0, screenWidth, 0, screenHeight);
		if (mainMenu){
			glMatrixMode(GL_PROJECTION);
			glClearColor(0.5, 0.56862745, 0.56862745, 0);
			showText("Main menu", Point3D(screenWidth / 2 - 50, screenHeight * 3 / 4, 0), Point3D(0.964, 0.905, 0.572));
			showText("Press <space> to start", Point3D(screenWidth/2 - 90, screenHeight / 3, 0), Point3D(0.964, 0.905, 0.572));
		}

		if (winScreen){
			glMatrixMode(GL_PROJECTION);
			glClearColor(0.5, 0.56862745, 0.56862745, 0);
			showText("You Won!!!", Point3D(screenWidth / 2 - 50, screenHeight * 3 / 4, 0), Point3D(0.964, 0.905, 0.572));
			showText("Score: " + std::to_string(block.numMoves), Point3D(screenWidth/2 - 50, screenHeight / 2, 0), Point3D(0.964, 0.905, 0.572));
			showText("Press <space> to restart", Point3D(screenWidth/2 - 120, screenHeight / 3, 0), Point3D(0.964, 0.905, 0.572));
		}

		if (looseScreen){
			glMatrixMode(GL_PROJECTION);
			glClearColor(0.5, 0.56862745, 0.56862745, 0);
			showText("You lost :(", Point3D(screenWidth / 2 - 60, screenHeight * 3 / 4, 0), Point3D(0.964, 0.905, 0.572));
			showText("Score: " + std::to_string(block.numMoves), Point3D(screenWidth/2 - 50, screenHeight / 2, 0), Point3D(0.964, 0.905, 0.572));
			showText("Press <space> to restart", Point3D(screenWidth/2 - 120, screenHeight / 3, 0), Point3D(0.964, 0.905, 0.572));
		}
	}
	glutSwapBuffers();
}

void FPS(int value){
	glutPostRedisplay();
	glutTimerFunc(17, FPS, 0);
	if (block.gameState == 0){return;}
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
    
	
	glutInitWindowSize(screenWidth, screenHeight);
	glutInitWindowPosition(100, 100);

	glutCreateWindow("Bloxorz");	//creates the window

	glutDisplayFunc(display);	//registers "display" as the display callback function
	glutKeyboardFunc(keyboard);

	glutTimerFunc(17, FPS, 0);

	glEnable(GL_DEPTH_TEST);
    
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
    
	init();

	glutMainLoop();				//starts the event loop

	return(0);					//return may not be necessary on all compilers
}
