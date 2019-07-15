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
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include<GL/glut.h>
#include <stdlib.h>
#include <string.h>
//#include <vcclr.h>
#include <MMSystem.h>
#include <cstdio>
//#include <GL/gl.h>
#include <GL/freeglut.h>
#include <math.h>
#include <sys/stat.h>

using std::min;
using std::max;

//#pragma comment(lib,"winmm.li")
#define N 39

using namespace glm;

//bmp(テクスチャ)読み込み
#include "BMPLoader.h"
//BMPImage::BMPImage();
BMPImage Texture[N];

//obj(3Dモデル)読み込み
#include "OBJLoader.h"
//OBJMESH::OBJMESH();
OBJMESH OBJFile[N];
//FPS計算
#include "FPS.h"
FPS *fps = new FPS(120); //秒間60フレーム(可変sleep)

//---------------------------------------------------
// 手元のノートPC環境 : MYPC
// dimenco Display    : DIMENCO
//---------------------------------------------------
enum PCEnviroment {
	MYPC = 0,
	DIMENCO = 1,
};
int selectEnviroment = MYPC;
//int selectEnviroment = DIMENCO;
//---------------------------------------------------

//---------------------------------------------------
// ゲーム難易度選択
// EASY	  : ブタが追いかけてこない
// NORMAL : ブタがゆっくり追いかけてくる
// HARD	  : リンゴを取るたびにブタの速さが大きくなる. もちろんブタは追いかけてくる
//---------------------------------------------------
enum DegreeOfDifficulty {
	EASY = 0,
	NORMAL = 1,
	HARD = 2,
};

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

//----------------------------------------------------
// シェーディング全般
//----------------------------------------------------
//glm::mat4 Movemat= glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
time_t cuTime;
time_t laTime;
time_t faTime;
double defTime;
double lastTime;
glm::mat4 ProjectionM;
glm::mat4 ViewM;
//glm::mat4 ModelM;
int ver[N] = { 0 };					// 関数 loadOBJnoUV() の中でインクリメントされる      
GLuint vertexbuffer[N];				// これが頂点バッファを指し示すものとなります。
std::vector<glm::vec3> vertices[N];	// .objファイルを読み込むのに使います
std::vector<glm::vec3>  normals[N]; // すぐには使いません。
std::vector<glm::vec2>  uvs[N];		// なんだろこれ.
GLuint uvbuffer[N];					// これがUVバッファを指し示すものとなります。
GLuint VertexArrayID[N];			// .objと.bmpとを結びつけるためのID
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

//----------------------------------------------------
// カメラ設定
//----------------------------------------------------
float horizontalAngle = 180.0f;				// 水平角、-Z方向
float verticalAngle = 0.0f;					// 鉛直角、0、水平線を眺daめている
glm::vec3 position = glm::vec3(0, 0, 0);	// 位置
glm::vec3 direction(0, 0, 0);				// 方向：球面座標から直角座標に変換します。
glm::vec3 right_vec3 = glm::vec3(0, 0, 0);	// 右ベクトル
glm::vec3 up;								// 上ベクトル：右と前ベクトルに垂直
double rot = 0;								// 回転に用いる(今は使ってない)

//----------------------------------------------------
// 不明
//----------------------------------------------------
int p;							// 不明(今はたぶん使ってない)
double movecount;				// 不明(今は多分使ってない)
int dbg_counter = 0;			// デバッグ用に使うカウンター

//----------------------------------------------------
// 壁
//----------------------------------------------------
double cube[29][4] = { 0 };		//cube[47][x_max,x_min,z_max,z_min]
std::string calc_OBJFile[N];	// 計算用の.objファイルを格納する

//----------------------------------------------------
// 画面
//----------------------------------------------------
int flush_cnt = 0;				//START画面で難易度選択文字を点滅させるかもしれない
int ModeSelect = 0;				//画面遷移に用いる. 0:スタート画面 1:プレイ画面 2:リザルト画面
int digreeOfDifficulty = -1;	//ゲーム難易度

//----------------------------------------------------
// keyboad
//----------------------------------------------------
const int escKey = 27;			//ESCAPキーの設定

//----------------------------------------------------
// 主人公関連
//----------------------------------------------------
objFlags objflag;				// 当たり判定のフラグ
int changePos;					// カメラを動かす速さ
float speed;					// 主人公が動く速さ
float initialFoV;				// 不明
float mouseSpeed;				// 不明
float FoV;						// 不明
float hspeed;					// 不明(今は使ってない)

//----------------------------------------------------
// ブタ関連
//----------------------------------------------------
int disOfNoticePig;						// ブタと自分がこの値以下になるとブタが近づいてくる
double obj_speed;						// ブタの速さ
double change_obj_speed;				// HARD時にブタが速くなる程度
bool accessPig;							// ブタが近づいてくるか否か trueなら近づいてくる falseなら固定行動のみ
bool changePigSpeedFlag[4] = { false }; // リンゴの取得数に応じてブタの速さを変えるときのためのフラグ(HARDのみ
double disFromPig2 = 0;					//ブタのちょっと前からの自分との距離
bool pigNoticeFlag[4] = { false };

//----------------------------------------------------
// りんご関連
//----------------------------------------------------
bool appleFlag[4];				//どのりんごが取得済みかを表すフラグ
int applecount = 0;				//取得したりんごの数

//----------------------------------------------------
// 制限時間関連
//----------------------------------------------------
double currentTime;				// 不明
float deltaTime;				// 不明


//------------------------------------------------------------ 
// ディスプレイ設定
//------------------------------------------------------------ 
//int  WindowPositionX = 600;			//生成するウィンドウ位置のX座標
//int  WindowPositionY = 0;				//生成するウィンドウ位置のY座標
//int  WindowWidth = 960;				//生成するウィンドウの幅
//int  WindowHeight = 1080;				//生成するウィンドウの高さ
int  WindowPositionX = 0;				//生成するウィンドウ位置のX座標
int  WindowPositionY = 0;				//生成するウィンドウ位置のY座標
int  WindowWidth = 1960;				//生成するウィンドウの幅
int  WindowHeight = 1080;				//生成するウィンドウの高さ
char WindowTitle[] = "TMF_ver2.8.2";	//ウィンドウのタイトル

//double ViewPointX = -300.0;
//double ViewPointY = -160.0;
//double ViewPointZ = 10.0;
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

//--------------------------------------------------------------------------------------------------------
// シェーディング全般
//--------------------------------------------------------------------------------------------------------
GLuint loadBMP_custom(const char * imagepath);
GLuint loadTGA_glfw(const char * imagepath);
GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path);
glm::mat4  getProjectionMatrix();
glm::mat4 getViewMatrix();
bool loadOBJ(const char * path, std::vector<glm::vec3>  & out_vertices, std::vector<glm::vec2>  & out_uvs, std::vector<glm::vec3>  & out_normals, int &ver);
bool loadOBJnoUV(const char * path, std::vector<glm::vec3>  & out_vertices, std::vector<glm::vec3>  & out_normals, int &ver);
void prosessingOfOBJ(int *ver, GLuint *vertexbuffer, std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals, std::vector<glm::vec2>& uvs, std::string calc_OBJFile, GLuint *uvbuffer, GLuint *VertexArrayID);
void prosessingOfMoveOBJ(int *ver, GLuint *vertexbuffer, std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals, std::vector<glm::vec2>& uvs, std::string calc_OBJFile, GLuint *uvbuffer, GLuint *VertexArrayID);
glm::mat4 ObjRoll(int i, double RadianAngle);
glm::mat4 ObjMove(int i);
glm::mat4 getModelMatrix(int i);
glm::mat4 ObjMoveRollWithCamera(int i, glm::vec3 position, float hA, float vA);

//--------------------------------------------------------------------------------------------------------
// ブタ初期位置　{黄豚 , 緑豚 , 赤豚 , 青春ブタ野郎}
//--------------------------------------------------------------------------------------------------------
double pigx[4] = { -2, 20, 0, -12 };
double pigz[4] = { 26, 0, -26, 16.5 };
int pigflag[4] = { 0 };
double px[4] = { 22, 20,  -26, -26 };
double pz[4] = { 26, -26, -26, 16.5 };
double s_px[4] = { -2, 20, 0, -12 };
double s_pz[4] = { 26, 0, -26, 16.5 };

//--------------------------------------------------------------------------------------------------------
// 6面ディスプレイ対応
//--------------------------------------------------------------------------------------------------------
void Display(void);									// 描画の関数
void Idle();										// アイドル時に呼び出される関数
void Keyboard(unsigned char key, int x, int y);		// キーボード入力時に呼び出される関数
void special_key(int key, int x, int y);			// 特殊キーの割り当て
void KeyboadUP(unsigned char key, int x, int y);	// キーボードから手を離した時の処理
void special_keyUP(int key, int x, int y);			// 特殊キーから手を離したときの処理
void Reshape(int x, int y);							// dimenco.hに関係している
void initState();									// dimenco.hに関係している
void view3D();										// dimenco.hに関係している

//--------------------------------------------------------------------------------------------------------
// ゲームをプレイするための処理全般
//--------------------------------------------------------------------------------------------------------
void Initialize(void);								// 初期設定の関数
void StartMode();									// START画面を表示
void PlayMode();									// PLAY画面を表示
void ResultMode();									// RESULT画面を表示
void GameReset();									// ゲームをRESET
void moveOBJ();										// 主人公を動かす処理
void InitObjFlag();									// 主人公の当たり判定のフラグの初期化
void selectDegreeOfDifficulty(int dod);				// ゲーム難易度を選択. 引数は enum の DigreeOfDifficulty
void setPigSpeed(double pig_speed);					// ブタの速さをセット
void changePigSpeed(double change_pig_speed);		// ブタの速さを増減
bool hit(double cube[29][4]);
bool objhit(double xmin, double xmax, double zmin, double zmax, double cube[29][4]);
bool objhit2(double x, double z, double cube[29][4]);
void decideCube(std::vector<glm::vec3> &vertices);
void ObjMoveRoll_Camera(float hA, float vA, int i);
void PigRolling(double xspeed, double zspeed);

//--------------------------------------------------------------------------------------------------------
// オブジェクトの表示全般
//--------------------------------------------------------------------------------------------------------
void Wall();
void Floor();
void TimeLimit();
void NumOfApple();
void DirectionOfPig();
void Apple();
void Sky();
void AppleCheck();

#endif
