#include <Windows.h>
#include <cstdio>

#include <GL/gl.h>
#include <GL/freeglut.h>

#define USE_DIMENCO_OPENGL_INTERFACE

#ifdef USE_DIMENCO_OPENGL_INTERFACE
	#include "dimenco.h"
#endif

static void keyboard(unsigned char key, int x, int y);
static void display(void);
static void idle(void);

static void initState(void);
static void view3D();

static int count = 0;
int screenWidth = 640;
int screenHeight = 480;
static int escKey = 27;

int main(int argc, char *argv[])
{

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(screenWidth, screenHeight);
	glutCreateWindow(argv[0]);

#ifdef USE_DIMENCO_OPENGL_INTERFACE
	glutFullScreen();
	dimencoInit();
#endif

	glutKeyboardFunc(keyboard);
	glutDisplayFunc(display);
    glutIdleFunc(idle);

	initState();

	glutMainLoop();

	return 0;
}

static void initState(void)
{
#ifdef USE_DIMENCO_OPENGL_INTERFACE
	dimencoInitState();
#endif

	glDisable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0, (double)screenWidth/(double)screenHeight, 3.0f, 100.0);

	glPushMatrix();
}

void view3D()
{
	//"""3d表示に切り替え"""
	//#Zバッファ、カリング有効化
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
 
	glMatrixMode(GL_PROJECTION);
	//#透視変換行列を取り出す
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	//ポリゴンの表面のみ描画
	glDisable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_DEPTH_TEST);

}

void display(void)
{
	float rot;

	glDisable(GL_LIGHTING);

	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#ifdef USE_DIMENCO_OPENGL_INTERFACE
	dimencoSetBackgroundState();
#endif
	view3D();

	glLoadIdentity();

	gluLookAt(0.0f, 0.0f, 60.0f, 0.0f, 0.0f, 0.0f, 
		0.0f, 1.0f, 0.0f);

	rot = count * 1.0f;

	glPushMatrix();
	glTranslatef(0, 0, 20.0);
	glRotatef(rot, 0.0f, 1.0f, 0.0f);
	glTranslatef(0, 0, -15.0);
	glColor3f(1.0f, 0.0f, 0.0f);
	glutSolidTeapot(5.0);
	glPopMatrix();

#ifdef USE_DIMENCO_OPENGL_INTERFACE
	dimencoSetZBufState();
#endif

	glutSwapBuffers();
}

static void idle(void)
{
	if(count == 360) count = 0;
	count++;

	glutPostRedisplay();
}

static void keyboard(unsigned char key, int x, int y)
{
	if (key == 'q' || key == 'Q' || key == escKey)
	{
		exit(0);
	}
}