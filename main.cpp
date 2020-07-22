//A program for lattice Boltzmann in D3Q19 mode.
#include <windows.h>
#include <GL\glew.h>
#include <GL/glut.h>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <omp.h>
#include <math.h>
#include <vector>
#include <string>
#include <GL\SOIL.h>
#include "Cloth.h"
#include "const.h"
using namespace std;
#define BMP_Header_Length 54
GLdouble winx1 = 0.0, winx2 = l, winy1 = 0.0, winy2 = m;//窗口边界
GLint control = 1; 
GLint case_index = 0;
Cloth cloth0;
GLfloat HoriRotate = 0;
GLfloat VertiRotate = 0;
static GLdouble fovy = 50, aspect = 1, zFar = 1.5, zNear = -1.0;//投影
static GLdouble eyex = -1.7, eyey = -1.5, eyez = 1.88, centerx = 0.05*l / 2, centery = 0.05*m / 2, centerz = 0, upx = 0, upy = 0.0, upz = 1.0;//三维观察

static void display(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();

	glLoadIdentity();
	glRotatef(HoriRotate, 1.0, 0.0, 0.0);
	glRotatef(VertiRotate, 0.0, 1.0, 0.0);
	glMatrixMode(GL_MODELVIEW);

	gluPerspective(fovy, aspect, zFar, zNear);
	gluLookAt(eyex, eyey, eyez, centerx, centery, centerz, upx, upy, upz);

	glBegin(GL_LINES);

	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(l, 0.0, 0.0);

	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, m, 0.0);

	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, h);

	glEnd();
	omp_set_num_threads(NUM_THREADS);
	if (control == 1)
	{
		cloth0.compute();
		cloth0.visualization();
	}

	glPopMatrix();

	glutSwapBuffers();
}

void chooseMode(GLint menuIteemNum)
{
	switch (menuIteemNum)
	{
	case 0:
		case_index = 0; break;
	case 1:
		case_index = 1; break;
	default:
		case_index = -1; break;
	}
	glutPostRedisplay();
}

static void idle(void)
{
	glutPostRedisplay();
}

void mouseFunc(GLint button, GLint action, GLint xMouse, GLint yMouse)
{
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		if (action == GLUT_DOWN)
			HoriRotate = ((int)HoriRotate + 10) % 360;
		break;
	case GLUT_RIGHT_BUTTON:
		if (action == GLUT_DOWN)
			VertiRotate = ((int)VertiRotate + 10) % 360;
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(10, 10);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	glutCreateWindow("Cloth");

	glutDisplayFunc(display);
	glutMouseFunc(mouseFunc);
	glutIdleFunc(idle);

	if (visual_style == Solid)
	{
		glEnable(GL_CULL_FACE);
		glEnable(GL_LIGHTING);
		const GLfloat light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
		const GLfloat light_diffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
		const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		const GLfloat light_position[] = { 3.0f, 3.0f, 3.0f, 1.0f };

		const GLfloat mat_ambient[] = { 1.0f, 0.0f, 0.0f, 1.0f };
		const GLfloat mat_diffuse[] = { 1.0f, 0.0f, 0.0f, 1.0f };
		const GLfloat mat_specular[] = { 0.5f, 0.5f, 0.5f, 1.0f };
		const GLfloat high_shininess[] = { 32.0f };
		glEnable(GL_LIGHT0);
		glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);

		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
	}

	glutMainLoop();

	return EXIT_SUCCESS;
}