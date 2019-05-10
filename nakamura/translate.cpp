
#include "pch.h"
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glut.h>

double posx = 3.0, posy = 2.0, posz = 8.0;
double zoomx = 0.0, zoomy = 0.0, zoomz = 0.0;

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);       	// カラーバッファを初期化する
	glColor3f(0.0f, 0.0f, 1.0f);		// これから描画する図形の色は青
	glutWireCube(5.0);			// ワイヤーフレーム立方体を描画

	glFlush();				// 描画を行う
}



// ウィンドウのサイズ変更時に呼ばれる

void reshape(int width, int height) {

    glViewport(0, 0, width, height); 
	glMatrixMode(GL_PROJECTION);			// 射影行列を操作する
	glLoadIdentity();						// 行列を初期化
	gluPerspective(60.0, (double)width / height, 1.0, 100.0);		

	glMatrixMode(GL_MODELVIEW);				// モデルビュー行列の設定
	glLoadIdentity();						// 行列を初期化

	gluLookAt(posx, posy, posz, zoomx, zoomy, zoomz, 0.0, 1.0, 0.0);

}


void initialize(void)
{
	glClearColor(1.0, 1.0, 1.0, 1.0);			    	// 画面を白にする
}

void aswz(unsigned char key, int x,int y)
{
	switch (key)
	{
	case 'a':
		std::cout << "aが入力された\n";
		glRotated(1, 0, 1, 0);
		glutPostRedisplay();
		break;
	case 's':
		std::cout << "sが入力された\n";
		glRotated(1, 0, -1, 0);
		glutPostRedisplay();
		break;
	case 'w':
		std::cout << "wが入力された\n";
		glRotated(1, 1, 0, 0);
		glutPostRedisplay();
		break;
	case 'z':
		std::cout << "zが入力された\n";
		glRotated(1, -1, 0, 0);
		glutPostRedisplay();
		break;
	default:
		break;
	}
}

void hokokey(int key, int x, int y)
{
	if (key == GLUT_KEY_LEFT) {
		std::cout << "左が入力された\n";
		glTranslated(-0.1, 0, 0);
		glutPostRedisplay();
	}
	if (key == GLUT_KEY_RIGHT) {
		std::cout << "右が入力された\n"; 
		glTranslated(0.1, 0, 0);
		glutPostRedisplay();
	}
	if (key == GLUT_KEY_UP) {
		std::cout << "上が入力された\n";
		glTranslated(0, 0, -0.1);
		glutPostRedisplay();
	}
	if (key == GLUT_KEY_DOWN) {
		std::cout << "下が入力された\n";
		glTranslated(0, 0, 0.1);
		glutPostRedisplay();
	}
}


int main(int argc, char** argv)

{
	glutInit(&argc, argv);						// GLUTを初期化する
	glutInitWindowSize(800, 600);				     	// 画面サイズを指定する
	glutInitWindowPosition(800, 300);			    	// 画面の初期位置を指定する
	glutInitDisplayMode(GLUT_RGBA);				    	// 表示モード設定
	glutCreateWindow("translate");		  		// ウィンドウの名前

	// コールバック関数の設定

	glutSpecialFunc(hokokey);
	glutKeyboardFunc(aswz);

	glutDisplayFunc(display);					// 描画処理が必要なときに呼ばれる
	glutReshapeFunc(reshape);					// reshapeが必要なときに呼ばれる

	initialize();							// 初期化
	glutMainLoop();							// 毎フレームのLoop

	return 0;

}