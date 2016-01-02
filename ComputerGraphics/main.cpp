#include <iostream>
#include <glut.h>
#include <math.h>
#include <vector>
#include <Windows.h>

#define INSTRUCTION_STRING "Use Z, X and Y keys to rotate the cube over corresponding axis.\nUse O to make it rotate automatically in all three dimensions.\nUse UP/DOWN keys to change rotation speed.\nUse LEFT/RIGHT to change color.\nBy Jakub Szubert and Sebastian Szyller"
#define GREY	0
#define RED		1
#define GREEN	2
#define BLUE	3
#define CYAN	4
#define MAGENTA	5
#define YELLOW	6
#define BLACK	7
#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT WINDOW_WIDTH

using namespace std;

static char *windowName = "Computer Graphics - Szubert, Szyller";
static int x_tick = 0;
static int y_tick = 0;
static int z_tick = 0;
static int mode = -1;
std::vector<unsigned int> COLORS;
unsigned int index = 0;
unsigned int ANIMATION_DELAY = 40;

static float materialColor[8][4] =
{
	{ 0.8, 0.8, 0.8, 1.0 },
	{ 0.8, 0.0, 0.0, 1.0 },
	{ 0.0, 0.8, 0.0, 1.0 },
	{ 0.0, 0.0, 0.8, 1.0 },
	{ 0.0, 0.8, 0.8, 1.0 },
	{ 0.8, 0.0, 0.8, 1.0 },
	{ 0.8, 0.8, 0.0, 1.0 },
	{ 0.0, 0.0, 0.0, 0.6 },
};

static float lightPos[4] =
{ 2.0, 4.0, 2.0, 1.0 };
static float lightAmb[4] =
{ 0.2, 0.2, 0.2, 1.0 };
static float lightDiff[4] =
{ 0.8, 0.8, 0.8, 1.0 };
static float lightSpec[4] =
{ 0.4, 0.4, 0.4, 1.0 };

static float groundPlane[4] =
{ 0.0, 1.0, 0.0, 1.499 };

static float backPlane[4] =
{ 0.0, 0.0, 1.0, 0.899 };

static float cube_vertexes[6][4][4] =
{
	{
		{ -0.1, -0.1, -0.1, 0.1 },
		{ -0.1, -0.1, 0.1, 0.1 },
		{ -0.1, 0.1, 0.1, 0.1 },
		{ -0.1, 0.1, -0.1, 0.1 } },

		{
			{ 0.1, 0.1, 0.1, 0.1 },
			{ 0.1, -0.1, 0.1, 0.1 },
			{ 0.1, -0.1, -0.1, 0.1 },
			{ 0.1, 0.1, -0.1, 0.1 } },

			{
				{ -0.1, -0.1, -0.1, 0.1 },
				{ 0.1, -0.1, -0.1, 0.1 },
				{ 0.1, -0.1, 0.1, 0.1 },
				{ -0.1, -0.1, 0.1, 0.1 } },

				{
					{ 0.1, 0.1, 0.1, 0.1 },
					{ 0.1, 0.1, -0.1, 0.1 },
					{ -0.1, 0.1, -0.1, 0.1 },
					{ -0.1, 0.1, 0.1, 0.1 } },

					{
						{ -0.1, -0.1, -0.1, 0.1 },
						{ -0.1, 0.1, -0.1, 0.1 },
						{ 0.1, 0.1, -0.1, 0.1 },
						{ 0.1, -0.1, -0.1, 0.1 } },

						{
							{ 0.1, 0.1, 0.1, 0.1 },
							{ -0.1, 0.1, 0.1, 0.1 },
							{ -0.1, -0.1, 0.1, 0.1 },
							{ 0.1, -0.1, 0.1, 0.1 } }
};

static float cube_normals[6][4] =
{
	{ -0.1, 0.0, 0.0, 0.0 },
	{ 0.1, 0.0, 0.0, 0.0 },
	{ 0.0, -0.1, 0.0, 0.0 },
	{ 0.0, 0.1, 0.0, 0.0 },
	{ 0.0, 0.0, -0.1, 0.0 },
	{ 0.0, 0.0, 0.1, 0.0 }
};

static void
setColor(int c)
{
	glMaterialfv(GL_FRONT_AND_BACK,
	GL_AMBIENT_AND_DIFFUSE, &materialColor[c][0]);
}

static void
drawCube(int color)
{
	setColor(color);
	for (int i = 0; i < 6; ++i) {
		glNormal3fv(&cube_normals[i][0]);
		glBegin(GL_POLYGON);
		glVertex4fv(&cube_vertexes[i][0][0]);
		glVertex4fv(&cube_vertexes[i][1][0]);
		glVertex4fv(&cube_vertexes[i][2][0]);
		glVertex4fv(&cube_vertexes[i][3][0]);
		glEnd();
	}
}

static void
drawCheck(int w, int h, int evenColor, int oddColor)
{
	static float square_normal[4] =
	{ 0.0, 0.0, 1.0, 0.0 };
	static float square[4][4];
	int i, j;
	glNewList(glGenLists(1), GL_COMPILE_AND_EXECUTE);

	glNormal3fv(square_normal);
	glBegin(GL_QUADS);
	for (j = 0; j < h; ++j) {
		for (i = 0; i < w; ++i) {
			square[0][0] = -1.0 + 2.0 / w * i;
			square[0][1] = -1.0 + 2.0 / h * (j + 1);
			square[0][2] = 0.0;
			square[0][3] = 1.0;

			square[1][0] = -1.0 + 2.0 / w * i;
			square[1][1] = -1.0 + 2.0 / h * j;
			square[1][2] = 0.0;
			square[1][3] = 1.0;

			square[2][0] = -1.0 + 2.0 / w * (i + 1);
			square[2][1] = -1.0 + 2.0 / h * j;
			square[2][2] = 0.0;
			square[2][3] = 1.0;

			square[3][0] = -1.0 + 2.0 / w * (i + 1);
			square[3][1] = -1.0 + 2.0 / h * (j + 1);
			square[3][2] = 0.0;
			square[3][3] = 1.0;

			if (i & 1 ^ j & 1) {
				setColor(oddColor);
			}
			else {
				setColor(evenColor);
			}
			glVertex4fv(&square[0][0]);
			glVertex4fv(&square[1][0]);
			glVertex4fv(&square[2][0]);
			glVertex4fv(&square[3][0]);
		}
	}

	glEnd();
	glEndList();
}

static void
myShadowMatrix(float ground[4], float light[4])
{
	float dot;
	float shadowMat[4][4];

	dot = ground[0] * light[0] +
		ground[1] * light[1] +
		ground[2] * light[2] +
		ground[3] * light[3];

	shadowMat[0][0] = dot - light[0] * ground[0];
	shadowMat[1][0] = 0.0 - light[0] * ground[1];
	shadowMat[2][0] = 0.0 - light[0] * ground[2];
	shadowMat[3][0] = 0.0 - light[0] * ground[3];

	shadowMat[0][1] = 0.0 - light[1] * ground[0];
	shadowMat[1][1] = dot - light[1] * ground[1];
	shadowMat[2][1] = 0.0 - light[1] * ground[2];
	shadowMat[3][1] = 0.0 - light[1] * ground[3];

	shadowMat[0][2] = 0.0 - light[2] * ground[0];
	shadowMat[1][2] = 0.0 - light[2] * ground[1];
	shadowMat[2][2] = dot - light[2] * ground[2];
	shadowMat[3][2] = 0.0 - light[2] * ground[3];

	shadowMat[0][3] = 0.0 - light[3] * ground[0];
	shadowMat[1][3] = 0.0 - light[3] * ground[1];
	shadowMat[2][3] = 0.0 - light[3] * ground[2];
	shadowMat[3][3] = dot - light[3] * ground[3];

	glMultMatrixf((const GLfloat *)shadowMat);
}

void
idle(void)
{
	if (x_tick >= 120) {
		x_tick = 0;

	}
	if (y_tick >= 120) {
		y_tick = 0;
	}
	if (z_tick >= 120) {
		z_tick = 0;
	}
	if (mode == 1) {
		x_tick++;
		y_tick++;
		z_tick++;
	}
	glutPostRedisplay();
	Sleep(ANIMATION_DELAY);
}

/* ARGSUSED1 */
void
keyboard(unsigned char ch, int x, int y)
{
	switch (ch) {
	case 27:             /* escape */
		exit(0);
		break;
	case 'O':
	case 'o':
		mode = 1;
		break;
	case 'X':
	case 'x':
		mode = -1;
		++x_tick;
		glutPostRedisplay();
		break;
	case 'Y':
	case 'y':
		mode = -1;
		++y_tick;
		glutPostRedisplay();
		break;
	case 'Z':
	case 'z':
		mode = -1;
		z_tick++;
		glutPostRedisplay();
		break;

	}
}

void advanceIndex(int direction){
	index += direction;
	if (index == -1) index = 7;
	if (index == 8) index = 0;
}

void
keyboardarrows(int ch, int x, int y)
{
	switch (ch) {
	case GLUT_KEY_UP:
		if (ANIMATION_DELAY > 1) {
			ANIMATION_DELAY -= 1;
		}
		break;
	case GLUT_KEY_DOWN:
		ANIMATION_DELAY += 1;
		break;
	case GLUT_KEY_LEFT:
		advanceIndex(-1);
		break;
	case GLUT_KEY_RIGHT:
		advanceIndex(1);
		break;
	}
}

void
display(void)
{
	GLfloat cubeXform[4][4];

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* draw ground */
	glPushMatrix();
	glTranslatef(0.0, -1.5, 0.0);
	glRotatef(-90.0, 1, 0, 0);
	glScalef(2.0, 2.0, 2.0);
	drawCheck(6, 6, BLACK, YELLOW);
	glPopMatrix();

	/* draw ceiling */
	glPushMatrix();
	glTranslatef(0.0, 1.5, 0.0);
	glRotatef(90.0, 1, 0, 0);
	glScalef(2.0, 2.0, 2.0);
	drawCheck(6, 6, RED, YELLOW);
	glPopMatrix();

	/* draw left */
	glPushMatrix();
	glTranslatef(-1.5, 0.0, 0.0);
	glRotatef(90.0, 0, 1, 0);
	glScalef(2.0, 2.0, 2.0);
	drawCheck(6, 6, CYAN, YELLOW);
	glPopMatrix();

	/* draw right */
	glPushMatrix();
	glTranslatef(1.5, 0.0, 0.0);
	glRotatef(-90.0, 0, 1, 0);
	glScalef(2.0, 2.0, 2.0);
	drawCheck(6, 6, GREY, YELLOW);
	glPopMatrix();

	/* draw back */
	glPushMatrix();
	glTranslatef(0.0, 0.0, -0.9);
	glScalef(2.0, 2.0, 2.0);
	drawCheck(6, 6, BLUE, YELLOW);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.5, 0.2, 0.0);
	glScalef(0.3, 0.3, 0.3);
	glRotatef((360.0 / (30 * 1)) * x_tick, 1, 0, 0);
	glRotatef((360.0 / (30 * 2)) * y_tick, 0, 1, 0);
	glRotatef((360.0 / (30 * 4)) * z_tick, 0, 0, 1);
	glScalef(1.0, 2.0, 1.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *)cubeXform);

	drawCube(COLORS[index]);        /* draw cube */
	glPopMatrix();

	/* draw ground shadow */
	glEnable(GL_BLEND); //opacity na cieniu
	glPushMatrix();
	myShadowMatrix(groundPlane, lightPos);
	glTranslatef(0.0, 0.0, 2.0);
	glMultMatrixf((const GLfloat *)cubeXform);
	drawCube(BLACK);      
	glPopMatrix();

	/* draw back shadow */
	glPushMatrix();
	myShadowMatrix(backPlane, lightPos);
	glTranslatef(0.0, 0.0, 2.0);
	glMultMatrixf((const GLfloat *)cubeXform);
	drawCube(BLACK);      
	glPopMatrix();

	glDisable(GL_BLEND);
	glutSwapBuffers();
}

int
main(int argc, char **argv)
{
	for (int i = 0; i <= 7; i++){
		COLORS.push_back(i);
	}

	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutCreateWindow(windowName);

	glutKeyboardFunc(keyboard);
	glutSpecialFunc(keyboardarrows);
	glutDisplayFunc(display);
	glutIdleFunc(idle);

	/* setup context */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 3.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -2.0);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiff);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpec);

	glEnable(GL_NORMALIZE);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glShadeModel(GL_SMOOTH);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0.0, 0.0, 0.0, 1);
	glClearIndex(0);
	glClearDepth(1);
	cout << INSTRUCTION_STRING << endl;
	glutMainLoop();
	return 0;             /* ANSI C requires main to return int. */
}