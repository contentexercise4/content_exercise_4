#pragma once
#ifndef LIB_H
#define LIB_H

#include<iostream>
#include<vector>
#include<fstream>
#include<sstream>
#include<string>
#include <time.h>
#include <algorithm>
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include <glm/gtx/transform.hpp> // <glm/glm.hpp>の後
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include<GL/glut.h>
//#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <vcclr.h>
//#include <Windows.h>
#include <MMSystem.h>
#include <cstdio>
//#include <GL/gl.h>
#include <GL/freeglut.h>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <math.h>

using std::min;
using std::max;

//#pragma comment(lib,"winmm.li")

#define N 9

using namespace glm;

struct objFlags {
	//--主人公のカメラの向きの移動フラグ--//
	bool xposInc;		//x座標を増やす
	bool xposDec;		//x座標を減らす
	bool yposInc;		//y座標を増やす
	bool yposDec;		//y座標を減らす

	//--FoVの値変更のフラグ--//
	bool FoVInc;		//FoV = initialFoV - 5 * 3.0f
	bool FoVDec;		//FoV = initialFoV + 5 * 3.0f

	//--主人公の位置座標変更のフラグ--//
	bool keyUP;			//前進する
	bool keyDOWN;		//後退する
	bool keyRIGHT;		//右に進む
	bool keyLEFT;		//左に進む

	//--主人公の位置座標変更のフラグ--//
	bool hitUP;			//前進したら壁に衝突した
	bool hitDOWN;		//後退したら壁に衝突した
	bool hitRIGHT;		//右に進んだら壁に衝突した
	bool hitLEFT;		//左に進んだら壁に衝突した
};

objFlags objflag;

double lastTime;
glm::mat4 ProjectionM;
glm::mat4 ViewM;
//glm::mat4 ModelM;

//----------------------------------------------------
// カメラ設定
//----------------------------------------------------
// 水平角、-Z方向
float horizontalAngle = 3.14f;
// 鉛直角、0、水平線を眺めている
float verticalAngle = 0.0f;
// 位置
glm::vec3 position = glm::vec3(0, 0, 5);

// 方向：球面座標から直角座標に変換します。
glm::vec3 direction(0, 0, 0);

// 右ベクトル
glm::vec3 right = glm::vec3(0, 0, 0);

// 上ベクトル：右と前ベクトルに垂直
glm::vec3 up;

double rot = 0;

int vercount = 0;
int vercount2 = 0;
double cube[47][4] = { 0 }; //cube[47][x_max,x_min,z_max,z_min]
int ModeSelect = 0;			//画面遷移に用いる. 0:スタート画面 1:プレイ画面 2:リザルト画面
int screenWidth = 960;
int screenHeight = 1080;

int ver[N] = { 0 };					// 関数 loadOBJnoUV() の中でインクリメントされる      
GLuint vertexbuffer[N];				// これが頂点バッファを指し示すものとなります。
std::vector<glm::vec3> vertices[N];	// .objファイルを読み込むのに使います
std::vector<glm::vec3>  normals[N]; // すぐには使いません。
std::vector<glm::vec2>  uvs[N];		// なんだろこれ.
GLuint uvbuffer[N];					// これがUVバッファを指し示すものとなります。
GLuint VertexArrayID[N];			// .objと.bmpとを結びつけるためのID

std::string OBJFile[N];				//.objファイルを格納する
GLuint Texture[N];					//.bmpファイルを読み込み, 関数 loadBMP_custom() の中でIDを割り当てられる. (すなわち, IDを持つ)

GLuint MatrixID;
GLuint programID;
glm::mat4 Projection;
glm::mat4 View;
glm::mat4 Model;

glm::mat4 ProjectionMatrix;
glm::mat4 ViewMatrix;
glm::mat4 ModelMatrix;
glm::mat4 MVP;

glm::mat4 ModelM[N];

int p;
int flush;
double movecount;

//keyboad
float initialFoV;

float speed; // 3 units / second
float mouseSpeed;

double currentTime;
float deltaTime;
float FoV;
float hspeed;

const int escKey = 27;

//------------------------------------------------------------ ディスプレイ設定

int  WindowPositionX = 6;			//生成するウィンドウ位置のX座標
int  WindowPositionY = 0;			//生成するウィンドウ位置のY座標
int  WindowWidth = 1960;			//生成するウィンドウの幅
int  WindowHeight = 1080;           //生成するウィンドウの高さ
char WindowTitle[] = "TMF";			//ウィンドウのタイトル

//double ViewPointX = -300.0;
//double ViewPointY = -160.0;
//double ViewPointZ = 10.0;
//
//int MenuFlag = 0;




// 3頂点を表す3つのベクトルの配列
// 頂点。3つの連続する数字は3次元の頂点です。3つの連続する頂点は三角形を意味します。
// 立方体は、それぞれが2枚の三角形からなる6面から成ります。だから6*2=12枚の三角形と12*3個の頂点を作ります。
static const GLfloat g_vertex_buffer_data[] = {
	-1.0f,-1.0f,-1.0f, // 三角形1:開始
	-1.0f,-1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f, // 三角形1:終了
	1.0f, 1.0f,-1.0f, // 三角形2:開始
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f, // 三角形2:終了
	1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	1.0f,-1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f
};

// 各頂点に一つの色。これらはランダムに作られました。
static const GLfloat g_color_buffer_data[] = {
	0.583f,  0.771f,  0.014f,
	0.609f,  0.115f,  0.436f,
	0.327f,  0.483f,  0.844f,
	0.822f,  0.569f,  0.201f,
	0.435f,  0.602f,  0.223f,
	0.310f,  0.747f,  0.185f,
	0.597f,  0.770f,  0.761f,
	0.559f,  0.436f,  0.730f,
	0.359f,  0.583f,  0.152f,
	0.483f,  0.596f,  0.789f,
	0.559f,  0.861f,  0.639f,
	0.195f,  0.548f,  0.859f,
	0.014f,  0.184f,  0.576f,
	0.771f,  0.328f,  0.970f,
	0.406f,  0.615f,  0.116f,
	0.676f,  0.977f,  0.133f,
	0.971f,  0.572f,  0.833f,
	0.140f,  0.616f,  0.489f,
	0.997f,  0.513f,  0.064f,
	0.945f,  0.719f,  0.592f,
	0.543f,  0.021f,  0.978f,
	0.279f,  0.317f,  0.505f,
	0.167f,  0.620f,  0.077f,
	0.347f,  0.857f,  0.137f,
	0.055f,  0.953f,  0.042f,
	0.714f,  0.505f,  0.345f,
	0.783f,  0.290f,  0.734f,
	0.722f,  0.645f,  0.174f,
	0.302f,  0.455f,  0.848f,
	0.225f,  0.587f,  0.040f,
	0.517f,  0.713f,  0.338f,
	0.053f,  0.959f,  0.120f,
	0.393f,  0.621f,  0.362f,
	0.673f,  0.211f,  0.457f,
	0.820f,  0.883f,  0.371f,
	0.982f,  0.099f,  0.879f
};

// 各頂点に2つの値、これらはBlenderで作りました。どうやって作るかはこれから説明します。
static const GLfloat g_uv_buffer_data[] = {
	0.000059f, 1.0f - 0.000004f,
	0.000103f, 1.0f - 0.336048f,
	0.335973f, 1.0f - 0.335903f,
	1.000023f, 1.0f - 0.000013f,
	0.667979f, 1.0f - 0.335851f,
	0.999958f, 1.0f - 0.336064f,
	0.667979f, 1.0f - 0.335851f,
	0.336024f, 1.0f - 0.671877f,
	0.667969f, 1.0f - 0.671889f,
	1.000023f, 1.0f - 0.000013f,
	0.668104f, 1.0f - 0.000013f,
	0.667979f, 1.0f - 0.335851f,
	0.000059f, 1.0f - 0.000004f,
	0.335973f, 1.0f - 0.335903f,
	0.336098f, 1.0f - 0.000071f,
	0.667979f, 1.0f - 0.335851f,
	0.335973f, 1.0f - 0.335903f,
	0.336024f, 1.0f - 0.671877f,
	1.000004f, 1.0f - 0.671847f,
	0.999958f, 1.0f - 0.336064f,
	0.667979f, 1.0f - 0.335851f,
	0.668104f, 1.0f - 0.000013f,
	0.335973f, 1.0f - 0.335903f,
	0.667979f, 1.0f - 0.335851f,
	0.335973f, 1.0f - 0.335903f,
	0.668104f, 1.0f - 0.000013f,
	0.336098f, 1.0f - 0.000071f,
	0.000103f, 1.0f - 0.336048f,
	0.000004f, 1.0f - 0.671870f,
	0.336024f, 1.0f - 0.671877f,
	0.000103f, 1.0f - 0.336048f,
	0.336024f, 1.0f - 0.671877f,
	0.335973f, 1.0f - 0.335903f,
	0.667969f, 1.0f - 0.671889f,
	1.000004f, 1.0f - 0.671847f,
	0.667979f, 1.0f - 0.335851f
};

GLuint loadBMP_custom(const char * imagepath);
GLuint loadTGA_glfw(const char * imagepath);
GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path);
bool hit(double cube[47][4]);
void decideCube(std::vector<glm::vec3> &vertices);
//void computeMatricesFromInputs(GLFWwindow* window);
glm::mat4  getProjectionMatrix();
glm::mat4 getViewMatrix();
bool loadOBJ(const char * path, std::vector<glm::vec3>  & out_vertices, std::vector<glm::vec2>  & out_uvs, std::vector<glm::vec3>  & out_normals, int &ver);
bool loadOBJnoUV(const char * path, std::vector<glm::vec3>  & out_vertices, std::vector<glm::vec3>  & out_normals, int &ver);
void prosessingOfOBJ(int *ver, GLuint *vertexbuffer, std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals, std::vector<glm::vec2>& uvs, std::string OBJFile, GLuint *uvbuffer, GLuint *VertexArrayID);
void prosessingOfMoveOBJ(int *ver, GLuint *vertexbuffer, std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals, std::vector<glm::vec2>& uvs, std::string OBJFile, GLuint *uvbuffer, GLuint *VertexArrayID);
void camerawork();
glm::mat4 ObjRoll(int i, double RadianAngle);
glm::mat4 ObjMove(int i);
glm::mat4 getModelMatrix(int i);
glm::mat4 ObjMoveRollWithCamera(int i, glm::vec3 position, float hA, float vA);



//6面ディスプレイ対応
//<共通>
void Initialize(void); // 初期設定の関数
void Display(void); // 描画の関数
void Idle(); // アイドル時に呼び出される関数
void Keyboard(unsigned char key, int x, int y); // キーボード入力時に呼び出される関数
void special_key(int key, int x, int y); // 特殊キーの割り当て
void KeyboadUP(unsigned char key, int x, int y);
void special_keyUP(int key, int x, int y);
void Reshape(int x, int y);
void initState();						//dimenco.hに関係している

void StartMode();
void PlayMode();
void moveOBJ();
void InitObjFlag();
#endif
