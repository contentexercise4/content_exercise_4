#pragma comment(linker, "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "XInput9_1_0.lib ")
#pragma comment(lib, "Gdiplus.lib")
#pragma comment( lib, "OpenAL32.lib" )
#pragma comment( lib, "alut.lib" )

#include <math.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cmath>
#include <sys/stat.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <stdio.h>
#include <time.h>
#include <windows.h>
#include <XInput.h>

#include <objbase.h>
#include <algorithm>
using std::min;
using std::max;
#include <GL/freeglut.h>
#include <stdlib.h>
#include <gdiplus.h>

#define USE_DIMENCO_OPENGL_INTERFACE

#ifdef USE_DIMENCO_OPENGL_INTERFACE
#include "dimenco.h"
#endif

//OpenAL
#include "alut.h"
ALuint bgm1, bgm2, bgm3 ,se03, seGun, seReload;
ALuint sbgm1, sbgm2, sbgm3 ,sse03, sseGun, sseReload;
int cccc = 0;
int bom_m = 0;

//png(テクスチャ)読み込み
using namespace Gdiplus;
GdiplusStartupInput gdiPSI;
ULONG_PTR gdiPT;
GLuint sky;
//GLuint game_over;

//パーティクルエフェクト
#include "MagicCircle.h"
MagicCircle magicCircle(10000, 0);
int hit = 0;

//エンジン炎
#include "Fire.h"
GLfloat fireColor[3] = { 1.0, 0.1, 0.0 };
Fire fire(100, fireColor, 1.0);

//爆発
#include "Smoke.h"
GLfloat smokeColor[3] = { 1.0, 0.3, 0.1 };
Smoke smoke(100, smokeColor);

//パルス
#include "Pulse.h"
//ダメージ用
Pulse pulse;
int hit2 = 0;

//カウントダウン用
Pulse countdown;
double countdown2;
double cowntdown_tmp;
int gamestart = 0;
int gamestart2 = 0;

//ゲームクリア
Pulse gameclear;

//ゲームオーバー
Pulse gameover;

//bmp(テクスチャ)読み込み
#include "BMPLoader.h"
BMPImage texture;
BMPImage number1;
BMPImage number2;
BMPImage number3;
BMPImage number4;
BMPImage number5;
BMPImage number6;
BMPImage number7;
BMPImage number8;
BMPImage number9;
BMPImage number0;
BMPImage colon;       //11番目
BMPImage countdown_1;
BMPImage countdown_2;
BMPImage countdown_3;
BMPImage game_clear;  //15番目
BMPImage game_over;   //16番目
BMPImage player_name; //17番目
BMPImage enemy_name;  //18番目
BMPImage result;      //19番目

//obj(3Dモデル)読み込み
#include "OBJLoader.h"
OBJMESH mesh;
OBJMESH mesh_ene;
OBJMESH beam;

//FPS計算
#include "FPS.h"
FPS *fps = new FPS(60); //秒間30フレーム(可変sleep)

using namespace std;

//-------------------------------------------------- コントローラ用

#define MAX_CONTROLLERS  4 //XInputが認識できるのは4つまで
#define Threshold  65535/4 //しきい値

struct CONTROLER_STATE
{
	XINPUT_STATE state;
	bool bConnected;
};

CONTROLER_STATE GAME_PAD[MAX_CONTROLLERS];

//接続されているか調べる
HRESULT UpdateControllerState()
{
	DWORD dwResult;
	for (DWORD i = 0; i < MAX_CONTROLLERS; i++)
	{
		dwResult = XInputGetState(i, &GAME_PAD[i].state);

		if (dwResult == ERROR_SUCCESS)
			GAME_PAD[i].bConnected = true;
		else
			GAME_PAD[i].bConnected = false;
	}

	return S_OK;
}

//-------------------------------------------------- 各種変数

float pos_x = 0.0, pos_y = 0.0, pos_z = 0.0;               //移動管理座標(コントローラ入力した値)
float pos_x2 = -300.0, pos_y2 = -200.0, pos_z2 = 0.0;       //カメラ用自機座標 (自機中心軸)
float pos_x3 = -300.0, pos_y3 = -200.0, pos_z3 = 0.0;       //自機座標

float enepos_x = 0.0, enepos_y = 0.0, enepos_z = 0.0;     //移動管理座標(行動パターン)
float enepos_x2 = -300.0, enepos_y2 = 0.0, enepos_z2 = 0.0; //カメラ用自機座標 (敵機中心軸)
float enepos_x3 = -300.0, enepos_y3 = 0.0, enepos_z3 = 0.0; //敵機座標

//移動管理
float pos_r = 0.0;
float enepos_r = 0.0;
int turn = 0;

//移動経路管理
//int enekey = 971;
//int key = 971;
int enekey = 0;
int key = 0;
int playkey;
int playkey_ene;
double angle = -180.0;  //敵機の角度
double theta;
double angle2 = -180.0; //自機の角度
double theta2;

//カメラ位置計算
double bul_dx;          //自機と敵機のx座標の差
double bul_dy;
double bul_r;
double bul_cos;
double bul_sin;

double lookat_x = -300;
double lookat_y = -105;
double lookat_z = 0;

//弾道計算(ホーミング)
double theta3;
double tarpos_x;
double tarpos_y;

//HP管理
double HP_ene = 100.0;
double HP = 100.0;

//経過タイム計測
DWORD start_time;
DWORD end_time;
DWORD current_time;
int time_flag = 0;

//経路の重み
double weight[20] = {};

//敵機の移動タイミング
int set = 50;
int shoot = 0;

//ループ用
int j;

//ウィンドウ位置調整
double standard_w;
double standard_h;

//メニューで使う変数
double kakudo = 0;//カメラの回転角
int kaasoru = 0;
int a[3] = { 1,0,0 };//メニューカーソル位置制御の配列
int k = 0;           //a[0]ゲーム画面、a[1]チュートリアル画面、a[2]？？？

//スコア管理変数score
int score[4] = { 0 };//[0],[1],[2]はそれぞれTop1,2,3を保持。[3]は最新スコアを保持。
int s3 = 0, s2 = 0, s1 = 0, s0 = 0;
int position = 0;

int tmp_score;
int ranking_count = 0;

int sound_check = 0;

double HP_var;
double HP_var_ene;

//------------------------------------------------------------ ディスプレイ設定

int  WindowPositionX = 100;         //生成するウィンドウ位置のX座標
int  WindowPositionY = 100;         //生成するウィンドウ位置のY座標
int  WindowWidth  = 1280;           //生成するウィンドウの幅
int  WindowHeight = 720;            //生成するウィンドウの高さ
char WindowTitle[] = "3D_Shooting"; //ウィンドウのタイトル

double ViewPointX = -300.0;
double ViewPointY = -160.0;
double ViewPointZ = 10.0;

int MenuFlag = 0;

//------------------------------------------------------------ 弾速度変数(自機)
#define bullet_num 501; //弾の上限

struct {
	double x, y, z;
	double check, angle, theta, radius, count;
	double vx, vy, vz;
	int key_tmp; //弾を発射した時点のkey
}p[501];
int pn = 0;
int pn2 = 0;

int tn = 0;
double t = 0;
double dt = 0.1;

double ax = 0.0, ay = 0.0, az = 0.0;
double vx = 40.0, vy = 40.0, vz = 20.0;
double hanpatu = 0.9;

//------------------------------------------------------------ 弾速度変数(敵機)2

struct {
	double x, y, z;
	double check, angle, theta, radius, count;
	double vx, vy, vz;
	int key_tmp; //弾を発射した時点のkey
}ep[501];
int epn = 0;
int epn2 = 0;

int etn = 0;
double et = 0;
double edt = 0.1;

double eax = 0.0, eay = 0.0, eaz = 0.0;
double evx = 40.0, evy = 40.0, evz = 20.0;

//------------------------------------------------------------ 変数宣言終了

//----------------------------------------------------
// 物質質感の定義
//----------------------------------------------------
struct MaterialStruct {
	GLfloat ambient[4];
	GLfloat diffuse[4];
	GLfloat specular[4];
	GLfloat shininess;
};
//jade(翡翠)
MaterialStruct ms_jade = {
	{ 0.135,     0.2225,   0.1575,   1.0 },
{ 0.54,      0.89,     0.63,     1.0 },
{ 0.316228,  0.316228, 0.316228, 1.0 },
12.8 };
//ruby(ルビー)
MaterialStruct ms_ruby = {
	{ 0.1745,   0.01175,  0.01175,   1.0 },
{ 0.61424,  0.04136,  0.04136,   1.0 },
{ 0.727811, 0.626959, 0.626959,  1.0 },
76.8 };

//----------------------------------------------------
// 色の定義の定義
//----------------------------------------------------
GLfloat red[] = { 0.8, 0.2, 0.2, 1.0 }; //赤色
GLfloat green[] = { 0.2, 0.8, 0.2, 1.0 };//緑色
GLfloat blue[] = { 0.2, 0.2, 0.8, 1.0 };//青色
GLfloat yellow[] = { 0.8, 0.8, 0.2, 1.0 };//黄色
GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };//白色
GLfloat black[] = { 0.0, 0.0, 0.0, 1.0 };//黒色
GLfloat shininess = 30.0;//光沢の強さ

//----------------------------------------------------
// 関数プロトタイプ（後に呼び出す関数名と引数の宣言）
//----------------------------------------------------
void LoadImage(const wchar_t* filename, GLuint &texture);

//<共通>
void Initialize(void); // 初期設定の関数
void Display(void); // 描画の関数
void Idle(); // アイドル時に呼び出される関数
void Keyboard(unsigned char key, int x, int y); // キーボード入力時に呼び出される関数
void special_key(int key, int x, int y); // 特殊キーの割り当て
void XInput(); // コントローラ入力

//<音楽>
void soundPlay(ALuint buf, ALuint *sorce, bool loop);
void soundStop(ALuint *sorce);
void soundInit();

//<プレイ画面>
void SetLighting(); // ライトの位置を設定
void camerawork(); // カメラ位置を設定
void route();      // 移動経路の作成
void movement(int _key);        // 自機の移動を制御
void movement_ene(int ene_key); // 敵機の移動を制御
void Shutdown(); // obj(3Dモデル)が読み込めなかった場合の強制終了
void teapot(float px, float py, float pz);  // teapotを描画
void obj(float px, float py, float pz);     // 自機モデルを読み込み、描画
void obj_ene(float px, float py, float pz); // 敵機モデルを読み込み、描画
void skydome();    // 背景の描画
void Ground(void); // 大地の描画 + バレットライン
void bullet();     // 弾の描画
void bullet2();    // 弾の描画(敵機)
void bullet_straight(); // 弾の管理(直線)
void bullet_straight2(); // 弾の管理(直線 敵機)
void bullet_curve();    // 弾の管理(曲線)
void bullet_curve2();    // 弾の管理(曲線 敵機)
void bullet_stay();
void bullet_homing1();  // 弾の管理(ホーミング1)
void bullet_homing2();  // 弾の管理(ホーミング2)
bool curve_decision(int key);
void time_texture(int time, int digit);

void Draw_2D();         // 2Dの描画(HPゲージ)
void glutRenderText(void* bitmapfont, const char*text); // テキストを表示

//<メニュー画面>
void Reshape(int x, int y);
void GameDisplay(void); // プレイ画面
void MenuDisplay(void); // メニュー画面
void DISPLAY_TEXT(int x, int y, const char *string, void *font); //2D出力設定(メニュー画面)
void DRAW_STRING(int x, int y, const char *string, void *font);  //文字の出力(メニュー画面)

//<リザルト画面>
void ResultDisplay();
int calculateScore();//スコアscore計算
void ranking();//スコアランクソート


void GameInit();
void keyboardUp(unsigned char key, int x, int y);
//----------------------------------------------------
// メイン関数
//----------------------------------------------------
int main(int argc, char *argv[]) {
	srand((unsigned)time(NULL));

	alutInit(&argc, argv);

	glutInit(&argc, argv); //環境の初期化
	glutInitWindowPosition(WindowPositionX, WindowPositionY);  //ウィンドウの位置の指定
	glutInitWindowSize(WindowWidth, WindowHeight);             //ウィンドウサイズの指定
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE); //ディスプレイモードの指定
	glutCreateWindow(WindowTitle); //ウィンドウの作成

#ifdef USE_DIMENCO_OPENGL_INTERFACE
	glutFullScreen();
	dimencoInit();
#endif

	glutDisplayFunc(Display);      //描画時に呼び出される関数を指定する（関数名：Display）
	glutReshapeFunc(Reshape);
	glutIdleFunc(Idle);            //プログラムアイドル状態時に呼び出される関数
	glutKeyboardFunc(Keyboard);    //キーボード入力時に呼び出される関数を指定する（関数名：Keyboard）
	//glutKeyboardUpFunc(keyboardUp);
	glutSpecialFunc(special_key);  //特殊キーの設定

	Initialize(); //初期設定の関数を呼び出す
	glutMainLoop();
	return 0;
}

//----------------------------------------------------
// 初期設定の関数
//----------------------------------------------------
void Initialize(void) {
#ifdef USE_DIMENCO_OPENGL_INTERFACE
	dimencoInitState();
#endif

	soundInit();
	

	//　GLEWの初期化
	if (glewInit() != GLEW_OK)
	{
		cout << "Error : GLEWの初期化に失敗しました\n";
		exit(0);
	}

	//　塗りつぶし色
	glClearColor(0.3, 0.3, 1.0, 1.0);

	//　深度テスト無効
	glEnable(GL_DEPTH_TEST);
	
	//　スムーズシェイディング
	glShadeModel(GL_SMOOTH);
	
	//光源の設定--------------------------------------
	SetLighting();
	
	//objファイルの読み込み
	if (!mesh.LoadFile("Mesh/fighter_color1.obj")) Shutdown();    //自機
	if (!mesh_ene.LoadFile("Mesh/deyuo1.obj")) Shutdown();        //敵機(deyuo) 
	//if (!mesh_ene.LoadFile("Mesh/sahaquel_v3.obj")) Shutdown(); //敵機(sahaquel)
	if (!beam.LoadFile("Mesh/beam.obj")) Shutdown();              //ビーム
	//if (!beam.LoadFile("Mesh/missile.obj")) Shutdown();         //ミサイル

	//テクスチャの読み込み
	if (!number1.Load("bmp/1.bmp"));
	if (!number2.Load("bmp/2.bmp"));
	if (!number3.Load("bmp/3.bmp"));
	if (!number4.Load("bmp/4.bmp"));
	if (!number5.Load("bmp/5.bmp"));
	if (!number6.Load("bmp/6.bmp"));
	if (!number7.Load("bmp/7.bmp"));
	if (!number8.Load("bmp/8.bmp"));
	if (!number9.Load("bmp/9.bmp"));
	if (!number0.Load("bmp/0.bmp"));
	if (!colon.Load("bmp/colon.bmp"));
	if (!countdown_3.Load("bmp/3.bmp"));    //12番目のテクスチャ
	if (!countdown_2.Load("bmp/2.bmp"));    //13番目のテクスチャ
	if (!countdown_1.Load("bmp/1.bmp"));    //14番目のテクスチャ
	if (!game_clear.Load("bmp/gameclear_full.bmp"));     //15番目のテクスチャ
	if (!game_over.Load("bmp/gameover_full.bmp"));     //16番目のテクスチャ
	//LoadImage(L"png/gameover_full.png", game_over);
	if (!player_name.Load("bmp/player_full.bmp"));     //15番目のテクスチャ
	if (!enemy_name.Load("bmp/enemy_full.bmp"));     //16番目のテクスチャ

	if (!texture.Load("bmp/321_8k.bmp")); //背景画像
	if (!result.Load("bmp/meisai7.bmp"));
	
	fire.LoadTexture("sample.dds");
	smoke.LoadTexture("sample.dds");


	GdiplusStartup(&gdiPT, &gdiPSI, NULL);
	LoadImage(L"bmp/321_8k.bmp", sky);
	//LoadImage(L"png/sample.png", tex);

	//cout << "tex : " << sky << endl;
	//cout << "Fire.texture.ID : " << fire.texture.ID << endl;
	//cout << "texture.ID : " << texture.ID << endl;
}

//----------------------------------------------------
// 描画の関数
//----------------------------------------------------
void Display(void) {
	//ResultDisplay();
	//cout << score[0] << " : " << score[1] << " : " << score[2] << " : " << score[3] << endl;
	
	if (MenuFlag <= 0) { //メニュー画面
		if (sound_check == 0) {
			soundStop(&sbgm3);
		}

		MenuDisplay();

		if (sound_check == 0) {
			soundPlay(bgm1, &sbgm1, true);
			sound_check = 1;
		}
	}
	else if(MenuFlag == 1){                //プレイ画面
		if (sound_check == 1) {
			soundStop(&sbgm1);
		}

		GameDisplay();

		if (sound_check == 1) {
			soundPlay(bgm2, &sbgm2, true);
			sound_check = 2;
		}
	}
	else {
		if (sound_check == 2) {
			soundStop(&sbgm2);
		}

		ResultDisplay();

		if (sound_check == 2) {
			soundPlay(bgm3, &sbgm3, true);
			sound_check = 0;
		}
	}
}

//----------------------------------------------------
// アイドル時に呼び出される関数
//----------------------------------------------------
void Idle()
{
	fps->GetFPS(); //FPSを得る

	if (fps->draw) { //秒間30フレームのタイミングで描画

		XInput();            //コントローラ操作
		glutPostRedisplay(); //glutDisplayFunc()を１回実行する
	}
}

//----------------------------------------------------
// キーボード入力時に呼び出される関数
//----------------------------------------------------
void Keyboard(unsigned char key, int x, int y) {
	switch (key)
	{
	case '\033':
		exit(0);
		break;

	case 'h':
		if (MenuFlag == 0 && HP > 0) {
			soundStop(&sse03);
			soundPlay(se03, &sse03, false);
			MenuFlag += 1;
		}

		if (HP <= 0 || HP_ene <= 0) {
			if (HP <= 0) {
				HP = 0;
			}
			else {
				HP_ene = 0;
			}
			playkey = 0;
			playkey_ene = 0;
			if (MenuFlag == 1) {
				soundStop(&sseReload);
				soundStop(&sse03);
				soundPlay(se03, &sse03, false);
				MenuFlag = 2;
			}
		}

		//連打調整
		if (MenuFlag >= 2) {
			MenuFlag++;
		}

		if (MenuFlag >= 32) {
			MenuFlag = -30;
			GameInit();
		}

		if (MenuFlag < 0) {
			MenuFlag++;
		}

		break;

	case 'x':
		if (playkey == 112 || playkey == 223 || playkey == 334 || playkey == 441 || playkey == 121 || playkey == 414 || playkey == 343 || playkey == 232) {
			bullet_curve();
		}
		if (playkey == 874 || playkey == 545 || playkey == 656 || playkey == 767 || playkey == 847 || playkey == 776 || playkey == 665 || playkey == 554) {
			bullet_curve();
		}
		if (playkey == 971 || playkey == 1053) {
			bullet_straight();
		}
		if (playkey == 917 || playkey == 1035) {
			bullet_straight();
		}
		break;

	default:
		break;
	}
}

//----------------------------------------------------
// 文字キーが離されたら呼び出される関数
//----------------------------------------------------
void keyboardUp(unsigned char key, int x, int y) {
	switch (key)
	{
	case 'h':
		break;

	default:
		break;
	}
}

//----------------------------------------------------
// 特殊キーの割り当て
//----------------------------------------------------
void special_key(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_LEFT:
		if (playkey == 971 || playkey == 1053) {
			if (pos_x >= -15.0) {
				pos_x -= 1.0;
			}
		}
		if (playkey == 917 || playkey == 1035) {
			if (pos_x <= 15.0) {
				pos_x += 1.0;
			}
		}
		if (playkey == 112 || playkey == 223 || playkey == 334 || playkey == 441 || playkey == 874 || playkey == 545 || playkey == 656 || playkey == 767) {
			if (pos_r <= 15.0) {
				pos_r += 1.0;
			}
		}
		if (playkey == 121 || playkey == 414 || playkey == 343 || playkey == 232 || playkey == 847 || playkey == 776 || playkey == 665 || playkey == 554) {
			if (pos_r >= -15.0) {
				pos_r -= 1.0;
			}
		}
		break;

	case GLUT_KEY_RIGHT:
		if (playkey == 971 || playkey == 1053) {
			if (pos_x <= 15.0) {
				pos_x += 1.0;
			}
		}
		if (playkey == 917 || playkey == 1035) {
			if (pos_x >= -15.0) {
				pos_x -= 1.0;
			}
		}
		if (playkey == 112 || playkey == 223 || playkey == 334 || playkey == 441 || playkey == 874 || playkey == 545 || playkey == 656 || playkey == 767) {
			if (pos_r >= -15.0) {
				pos_r -= 1.0;
			}
		}
		if (playkey == 121 || playkey == 414 || playkey == 343 || playkey == 232 || playkey == 847 || playkey == 776 || playkey == 665 || playkey == 554) {
			if (pos_r <= 15.0) {
				pos_r += 1.0;
			}
		}
		break;

	case GLUT_KEY_UP:
		break;
	case GLUT_KEY_DOWN:
		break;
	case GLUT_KEY_PAGE_UP:
		break;
	case GLUT_KEY_PAGE_DOWN:
		break;
	}
}

//----------------------------------------------------
// コントローラのボタン割り当て
//----------------------------------------------------
void XInput()
{
	UpdateControllerState();

	for (DWORD i = 0; i < MAX_CONTROLLERS; i++) {
		if (GAME_PAD[i].bConnected) {
			WORD Buttons = GAME_PAD[i].state.Gamepad.wButtons;

			//アナログ方向キー
			if (GAME_PAD[i].state.Gamepad.sThumbLY > Threshold);
			if (GAME_PAD[i].state.Gamepad.sThumbLY < -Threshold);
			if (GAME_PAD[i].state.Gamepad.sThumbLX > Threshold);
			if (GAME_PAD[i].state.Gamepad.sThumbLX < -Threshold);

			//デジタル方向キー(十字キー)
			if (Buttons & XINPUT_GAMEPAD_DPAD_UP);
			if (Buttons & XINPUT_GAMEPAD_DPAD_DOWN);
			if (Buttons & XINPUT_GAMEPAD_DPAD_LEFT) {
				if (playkey == 971 || playkey == 1053) {
					if (pos_x >= -15.0) {
						pos_x -= 1.0;
					}
				}
				if (playkey == 917 || playkey == 1035) {
					if (pos_x <= 15.0) {
						pos_x += 1.0;
					}
				}
				if (playkey == 112 || playkey == 223 || playkey == 334 || playkey == 441 || playkey == 874 || playkey == 545 || playkey == 656 || playkey == 767) {
					if (pos_r <= 15.0) {
						pos_r += 1.0;
					}
				}
				if (playkey == 121 || playkey == 414 || playkey == 343 || playkey == 232 || playkey == 847 || playkey == 776 || playkey == 665 || playkey == 554) {
					if (pos_r >= -15.0) {
						pos_r -= 1.0;
					}
				}
				break;
			}
			if (Buttons & XINPUT_GAMEPAD_DPAD_RIGHT) {
				if (playkey == 971 || playkey == 1053) {
					if (pos_x <= 15.0) {
						pos_x += 1.0;
					}
				}
				if (playkey == 917 || playkey == 1035) {
					if (pos_x >= -15.0) {
						pos_x -= 1.0;
					}
				}
				if (playkey == 112 || playkey == 223 || playkey == 334 || playkey == 441 || playkey == 874 || playkey == 545 || playkey == 656 || playkey == 767) {
					if (pos_r >= -15.0) {
						pos_r -= 1.0;
					}
				}
				if (playkey == 121 || playkey == 414 || playkey == 343 || playkey == 232 || playkey == 847 || playkey == 776 || playkey == 665 || playkey == 554) {
					if (pos_r <= 15.0) {
						pos_r += 1.0;
					}
				}
				break;
			}

			//各種ボタン 空制御 コピペ用
			if (Buttons & XINPUT_GAMEPAD_A);
			if (Buttons & XINPUT_GAMEPAD_B) {
				if (playkey == 112 || playkey == 223 || playkey == 334 || playkey == 441 || playkey == 121 || playkey == 414 || playkey == 343 || playkey == 232) {
					if (HP > 0 && HP_ene > 0) {
						soundStop(&sseGun);
						soundPlay(seGun, &sseGun, false);
					}
					bullet_curve();
				}
				if (playkey == 874 || playkey == 545 || playkey == 656 || playkey == 767 || playkey == 847 || playkey == 776 || playkey == 665 || playkey == 554) {
					if (HP > 0 && HP_ene > 0) {
						soundStop(&sseGun);
						soundPlay(seGun, &sseGun, false);
					}
					bullet_curve();
				}
				if (playkey == 971 || playkey == 1053) {
					if (HP > 0 && HP_ene > 0) {
						soundStop(&sseGun);
						soundPlay(seGun, &sseGun, false);
					}
					bullet_straight();
				}
				if (playkey == 917 || playkey == 1035) {
					if (HP > 0 && HP_ene > 0) {
						soundStop(&sseGun);
						soundPlay(seGun, &sseGun, false);
					}
					bullet_straight();
				}
				break;
			}
			if (Buttons & XINPUT_GAMEPAD_X);
			if (Buttons & XINPUT_GAMEPAD_Y);
			if (Buttons & XINPUT_GAMEPAD_START) {
				if (MenuFlag == 0 && HP > 0) {
					soundStop(&sse03);
					soundPlay(se03, &sse03, false);
					MenuFlag += 1;
				}

				if (HP <= 0 || HP_ene <= 0) {
					if (HP <= 0) {
						HP = 0;
					}
					else {
						HP_ene = 0;
					}
					playkey = 0;
					playkey_ene = 0;
					if (MenuFlag == 1) {
						soundStop(&sseReload);
						soundStop(&sse03);
						soundPlay(se03, &sse03, false);
						MenuFlag = 2;
					}
				}

				//連打調整
				if (MenuFlag >= 2) {
					MenuFlag++;
				}

				if (MenuFlag >= 32) {
					MenuFlag = -30;
					GameInit();
				}

				if (MenuFlag < 0) {
					MenuFlag++;
				}

				break;
			}
			if (Buttons & XINPUT_GAMEPAD_BACK);
			if (Buttons & XINPUT_GAMEPAD_LEFT_THUMB);    //左アナログ方向キーをパッド奥に押した場合
			if (Buttons & XINPUT_GAMEPAD_RIGHT_THUMB);   //右アナログ方向キーをパッド奥に押した場合
			if (Buttons & XINPUT_GAMEPAD_LEFT_SHOULDER);
			if (Buttons & XINPUT_GAMEPAD_RIGHT_SHOULDER);
			if (GAME_PAD[i].state.Gamepad.bLeftTrigger);
			if (GAME_PAD[i].state.Gamepad.bRightTrigger);
		}
		else {
			//接続されていない場合
		}
	}
	Sleep(1);
}

//----------------------------------------------------
// ライトの位置を設定
//----------------------------------------------------
void SetLighting()
{
	GLfloat light_position[] = { 0.0, 300.0, 300.0, 1.0 };
	glLightfv(GL_LIGHT1, GL_POSITION, light_position);

	//static GLfloat lightDiffuse[3] = { 0.03, 0.03, 0.03 };
	//glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse);

	GLfloat red[] = { 1.0, 0.4, 0.3, 1.0 };
	GLfloat blue[] = { 0.2, 0.4, 1.0, 1.0 };
	glLightfv(GL_LIGHT1, GL_DIFFUSE, red);

	/*
	// 見本
	static GLfloat lightPosition[4] = { 50.0, 0.0, 100.0, 1.0 }; //光源の位置
	static GLfloat lightDiffuse[3] = { 1.0,   1.0, 1.0 }; //拡散光
	static GLfloat lightAmbient[3] = { 0.25, 0.25, 0.25 }; //環境光
	static GLfloat lightSpecular[3] = { 1.0,   1.0, 1.0 }; //鏡面光
	GLfloat spotDirrection[] = { -0.5, 0.0, -1.0 }; //スポットライトを向ける方向

	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.01);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spotDirrection); //スポットライトの向ける方向（デフォルト (0,0,-1.0)）
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 30.0);// スポットライトの絞り（デフォルト 180.0）
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 0.1);// スポットライトの中心からの減衰の度合い（デフォルト 0）
	*/
}

//----------------------------------------------------
// カメラ位置の設定
//----------------------------------------------------
void camerawork() {
	//視点の移動
	double dx;
	double dy;
	double d;
	double scale;

	dx = enepos_x2 - pos_x2;
	dy = enepos_y2 - pos_y2;
	d = sqrt((dx*dx) + (dy*dy));
	scale = 33.0 / d;

	if (enepos_x2 > pos_x2) {
		ViewPointX = pos_x2 - dx * scale;
		lookat_x = pos_x2 + dx * 0.3;
	}
	if (enepos_x2 < pos_x2) {
		ViewPointX = pos_x2 - dx * scale;
		lookat_x = pos_x2 + dx * 0.3;
	}
	if (enepos_y2 > pos_y2) {
		ViewPointY = pos_y2 - dy * scale;
		lookat_y = pos_y2 + dy * 0.3;
	}
	if (enepos_y2 < pos_y2) {
		ViewPointY = pos_y2 - dy * scale;
		lookat_y = pos_y2 + dy * 0.3;
	}

	//cout << "X " << ViewPointX << "Y " << ViewPointY << endl;
}

//----------------------------------------------------
// 移動経路の作成
//----------------------------------------------------
void route() {
	double ra = (double)rand() / (RAND_MAX + 1);

	//point1--------------------------------------------------
	if (enepos_x2 <= -300 && enepos_y2 >= 300 && enekey == 971) { //7から来た場合
		enepos_r = (-1.0) * enepos_x; //引き渡し
		enekey = 112; angle = -180.0;
	}

	if (enepos_x2 <= -300 && enepos_y2 >= 300 && enekey == 441) { //4から来た場合
		enekey = 112; angle = -180.0;
	}

	if (enepos_x2 <= -300 && enepos_y2 >= 300 && enekey == 121) { //2から来た場合
		if (ra > 0.7) {
			enekey = 414; angle = -180.0;
		}
		else {
			enepos_x = (-1.0) * enepos_r; //引き渡し
			enekey = 917;
		}
	}

	//point2--------------------------------------------------
	if (enepos_x2 >= -0.45 && enepos_x2 <= 0.45 && enepos_y2 >= 600 && enekey == 112) { //1から来た場合
		enekey = 223; angle = 90.0;
	}

	if (enepos_x2 >= -0.45 && enepos_x2 <= 0.45 && enepos_y2 >= 600 && enekey == 232) { //3から来た場合
		enekey = 121; angle = 90.0;
	}

	//point3--------------------------------------------------
	if (enepos_x2 >= 300 && enepos_y2 >= 300 && enekey == 223) { //2から来た場合
		if (ra > 0.7) {
			enekey = 334; angle = 0.0;
		}
		else {
			enepos_x = enepos_r; //引き渡し
			enekey = 1035;
		}
	}

	if (enepos_x2 >= 300 && enepos_y2 >= 300 && enekey == 343) { //4から来た場合
		enekey = 232; angle = 0.0;
	}

	if (enepos_x2 >= 300 && enepos_y2 >= 300 && enekey == 1053) { //5から来た場合
		enepos_r = enepos_x; //引き渡し
		enekey = 232; angle = 0.0;
	}

	//point4--------------------------------------------------
	if (enepos_x2 >= -0.45 && enepos_x2 <= 0.45 && enepos_y2 >= -0.45 && enepos_y2 <= 0.45 && enekey == 334) { //3から来た場合
		if (ra > 0.7) {
			enekey = 441; angle = -90.0;
		}
		else {
			enepos_r = (-1.0) * enepos_r; //引き渡し
			enekey = 847; angle = 90.0;
		}
	}

	if (enepos_x2 >= -0.45 && enepos_x2 <= 0.45 && enepos_y2 >= -0.45 && enepos_y2 <= 0.45 && enekey == 414) { //1から来た場合
		if (ra > 0.7) {
			enekey = 343; angle = -90.0;
		}
		else {
			enepos_r = (-1.0) * enepos_r; //引き渡し
			enekey = 545; angle = 90.0;
		}
	}

	if (enepos_x2 >= -0.45 && enepos_x2 <= 0.45 && enepos_y2 >= -0.45 && enepos_y2 <= 0.45 && enekey == 874) { //7から来た場合
		if (ra > 0.3) {
			enepos_r = (-1.0) * enepos_r; //引き渡し
			enekey = 343; angle = -90.0;
		}
		else {
			enekey = 545; angle = 90.0;
		}
	}

	if (enepos_x2 >= -0.45 && enepos_x2 <= 0.45 && enepos_y2 >= -0.45 && enepos_y2 <= 0.45 && enekey == 554) { //5から来た場合
		if (ra > 0.3) {
			enepos_r = (-1.0) * enepos_r; //引き渡し
			enekey = 441; angle = -90.0;
		}
		else {
			enekey = 847; angle = 90.0;
		}
	}

	//point5--------------------------------------------------
	if (enepos_x2 >= 300 && enepos_y2 <= -300 && enekey == 545) { //4から来た場合
		enekey = 656; angle = 0.0;
	}

	if (enepos_x2 >= 300 && enepos_y2 <= -300 && enekey == 1035) { //3から来た場合
		enepos_r = enepos_x; //引き渡し
		enekey = 656; angle = 0.0;
	}

	if (enepos_x2 >= 300 && enepos_y2 <= -300 && enekey == 665) { //6から来た場合
		if (ra > 0.7) {
			enekey = 554; angle = 0.0;
		}
		else {
			enepos_x = enepos_r; //引き渡し
			enekey = 1053;
		}
	}

	//point6--------------------------------------------------
	if (enepos_x2 >= -0.45 && enepos_x2 <= 0.45 && enepos_y2 <= -600  && enekey == 656) { //5から来た場合
		enekey = 767; angle = -90.0;
	}

	if (enepos_x2 >= -0.45 && enepos_x2 <= 0.45 && enepos_y2 <= -600 && enekey == 776) { //7から来た場合
		enekey = 665; angle = -90.0;
	}

	//point7--------------------------------------------------
	if (enepos_x2 <= -300 && enepos_y2 <= -300 && enekey == 767) { //6から来た場合
		if (ra > 0.7) {
			enekey = 874; angle = -180.0;
		}
		else {
			enepos_x = (-1.0) * enepos_r; //引き渡し
			enekey = 971;
		}
	}

	if (enepos_x2 <= -300 && enepos_y2 <= -300 && enekey == 847) { //4から来た場合
		enekey = 776; angle = -180.0;
	}

	if (enepos_x2 <= -300 && enepos_y2 <= -300 && enekey == 917) { //1から来た場合
		enepos_r = (-1.0) * enepos_x; //引き渡し
		enekey = 776; angle = -180.0;
	}



	//移動方法--------------------------------------------------

	movement_ene(enekey); //敵機を移動させる

	if (enekey != 0) {
		playkey_ene = enekey;
	}

	if (pos_x2 <= -300 && pos_y2 >= 300) { //point1
		if (key == 971) {
			pos_r = (-1.0) * pos_x; //引き渡し
		}
		if (key == 121 && enekey == 917) {
			pos_x = (-1.0) * pos_r; //引き渡し
		}

		key = enekey;
		if (key == 112 || key == 414) {
			angle2 = -180.0;
		}
	}

	if (pos_y2 >= 600) { //point2
		key = enekey; angle2 = 90.0;
	}

	if (pos_x2 >= 300 && pos_y2 >= 300) { //point3
		if (key == 1053) {
			pos_r = pos_x; //引き渡し
		}
		if (key == 223 && enekey == 1035) {
			pos_x = pos_r; //引き渡し
		}
		
		key = enekey;
		if (key == 334 || key == 232) {
			angle2 = 0.0;
		}
	}

	if (pos_x2 >= -0.45 && pos_x2 <= 0.45 && pos_y2 >= -0.45 && pos_y2 <= 0.45) { //point4
		if (key == 334 && enekey ==847) {
			pos_r = (-1.0) * pos_r; //引き渡し
		}
		if (key == 414 && enekey == 545) {
			pos_r = (-1.0) * pos_r; //引き渡し
		}
		if (key == 554 && enekey == 441) {
			pos_r = (-1.0) * pos_r; //引き渡し
		}
		if (key == 874 && enekey == 343) {
			pos_r = (-1.0) * pos_r; //引き渡し
		}

		key = enekey;
		if (key == 847 || key == 545) {
			angle2 = 90.0;
		}
		else {
			angle2 = -90.0;
		}
	}

	if (pos_x2 >= 300 && pos_y2 <= -300) { //point5
		if (key == 1035) {
			pos_r = pos_x; //引き渡し
		}
		if (key == 665 && enekey == 1053) {
			pos_x = pos_r; //引き渡し
		}

		key = enekey;
		if (key == 656 || key == 554) {
			angle2 = 0.0;
		}
	}

	if (pos_y2 <= -600) { //point6
		key = enekey; angle2 = -90.0;
	}

	if (pos_x2 <= -300 && pos_y2 <= -300) { //point7
		if (key == 917) {
			pos_r = (-1.0) * pos_x; //引き渡し
		}
		if (key == 767 && enekey == 971) {
			pos_x = (-1.0) * pos_r; //引き渡し
		}

		key = enekey;
		if (key == 874 || key == 776) {
			angle2 = -180.0;
		}
	}

	movement(key); //自機を移動させる

	if (key != 0) {
		playkey = key;
	}
	//cout << key << " " << pos_x << " " << pos_y << " " << pos_r << endl;
	//cout << enekey << endl;
	//cout << enepos_x2 << "  " << enepos_y2 <<  "  " << enekey << "  "<< key << "  " << pos_x2 << "  " << pos_y2 << endl;
}

//----------------------------------------------------
// 自機の移動を制御
//----------------------------------------------------
void movement(int _key) {
	if (_key == 112 || _key == 223 || _key == 334 || _key == 441 || _key == 545 || _key == 656 || _key == 767 || _key == 874) {
		angle2 -= 0.4;
		theta2 = angle2 * acos(-1.0) / 180.0;

		//中心軸
		pos_x2 = 300 * cos(theta2);
		if (_key == 112 || _key == 223 || _key == 334 || _key == 441) { //時計回り(上)
			pos_y2 = 300 * sin(theta2) + 300;
		}
		else {
			pos_y2 = 300 * sin(theta2) - 300;
		}
		pos_x = 0;
		pos_y = 0;


		//実際の位置
		pos_x3 = (300 + pos_r) * cos(theta2);
		if (_key == 112 || _key == 223 || _key == 334 || _key == 441) {
			pos_y3 = (300 + pos_r) * sin(theta2) + 300;
		}
		else {
			pos_y3 = (300 + pos_r) * sin(theta2) - 300;
		}
	}

	if (_key == 121 || _key == 414 || _key == 343 || _key == 232 || _key == 847 || _key == 776 || _key == 665 || _key == 554) {
		angle2 += 0.4;
		theta2 = angle2 * acos(-1.0) / 180.0;

		//中心軸
		pos_x2 = 300 * cos(theta2);
		if (_key == 121 || _key == 414 || _key == 343 || _key == 232) { //反時計回り(上)
			pos_y2 = 300 * sin(theta2) + 300;
		}
		else {
			pos_y2 = 300 * sin(theta2) - 300;
		}
		pos_x = 0;
		pos_y = 0;

		//実際の位置
		pos_x3 = (300 + pos_r) * cos(theta2);
		if (_key == 121 || _key == 414 || _key == 343 || _key == 232) {
			pos_y3 = (300 + pos_r) * sin(theta2) + 300;
		}
		else {
			pos_y3 = (300 + pos_r) * sin(theta2) - 300;
		}
	}

	if (_key == 971 || _key == 1053) {
		pos_y2 += 2.0;
		if (_key == 971) {
			pos_x2 = -300.0;
		}
		else {
			pos_x2 = 300.0;
		}

		pos_y = 0;
		pos_r = 0;

		//実際の位置
		pos_y3 = pos_y2;
		pos_x3 = pos_x2 + pos_x;
	}
	if (_key == 917 || _key == 1035) {
		pos_y2 -= 2.0;
		if (_key == 917) {
			pos_x2 = -300.0;
		}
		else {
			pos_x2 = 300.0;
		}

		pos_y = 0;
		pos_r = 0;

		//実際の位置
		pos_y3 = pos_y2;
		pos_x3 = pos_x2 + pos_x;
	}
	
	//cout << "pos_x " << pos_x << "   pos_y " << pos_y << "   pos_r " << pos_r << endl;
}

//----------------------------------------------------
// 敵機の移動を制御
//----------------------------------------------------
void movement_ene(int ene_key) {
	//横移動
	if (gamestart == 1) {
		if (turn == 0) {
			if (playkey_ene == 971 || playkey_ene == 1053) {
				enepos_x += 1.0;
				if (abs(pos_x - enepos_x) <= 20.0 && enepos_x <= 49.0) {
					enepos_x += 1.0; set++;
				}
			}
			if (playkey_ene == 917 || playkey_ene == 1035) {
				enepos_x -= 1.0;
				if (abs(pos_x - enepos_x) <= 20.0 && enepos_x >= -49.0) {
					enepos_x -= 1.0; set++;
				}
			}
			if (playkey_ene == 112 || playkey_ene == 223 || playkey_ene == 334 || playkey_ene == 441 || playkey_ene == 874 || playkey_ene == 545 || playkey_ene == 656 || playkey_ene == 767) {
				enepos_r -= 1.0;
				if (abs(pos_r - enepos_r) <= 20.0 && enepos_r >= -49.0) {
					enepos_r -= 1.0; set++;
				}
			}
			if (playkey_ene == 121 || playkey_ene == 414 || playkey_ene == 343 || playkey_ene == 232 || playkey_ene == 847 || playkey_ene == 776 || playkey_ene == 665 || playkey_ene == 554) {
				enepos_r += 1.0;
				if (abs(pos_r - enepos_r) <= 20.0 && enepos_r <= 49.0) {
					enepos_r += 1.0; set++;
				}
			}

			set++;
			if (set >= 100) {
				turn = 1;
				set = 0;
			}
		}
		else { //(turn == 1)
			if (playkey_ene == 971 || playkey_ene == 1053) {
				enepos_x -= 1.0;
				if (abs(pos_x - enepos_x) <= 20.0 && enepos_x >= -49.0) {
					enepos_x -= 1.0; set++;
				}
			}
			if (playkey_ene == 917 || playkey_ene == 1035) {
				enepos_x += 1.0;
				if (abs(pos_x - enepos_x) <= 20.0 && enepos_x <= 49.0) {
					enepos_x += 1.0; set++;
				}
			}
			if (playkey_ene == 112 || playkey_ene == 223 || playkey_ene == 334 || playkey_ene == 441 || playkey_ene == 874 || playkey_ene == 545 || playkey_ene == 656 || playkey_ene == 767) {
				enepos_r += 1.0;
				if (abs(pos_r - enepos_r) <= 20.0 && enepos_r <= 49.0) {
					enepos_r += 1.0; set++;
				}
			}
			if (playkey_ene == 121 || playkey_ene == 414 || playkey_ene == 343 || playkey_ene == 232 || playkey_ene == 847 || playkey_ene == 776 || playkey_ene == 665 || playkey_ene == 554) {
				enepos_r -= 1.0;
				if (abs(pos_r - enepos_r) <= 20.0 && enepos_r >= -49.0) {
					enepos_r -= 1.0; set++;
				}
			}

			set++;
			if (set >= 100) {
				turn = 0;
				set = 0;
			}
		}
	}
	//cout << "turn :" << turn << "  enepos_x :" << enepos_x << "  enepos_r :" << enepos_r << "  enekey :"<< playkey_ene << endl;
	
	//ルート移動
	if (ene_key == 112 || ene_key == 223 || ene_key == 334 || ene_key == 441 ) {
		angle -= 0.4;
		theta = angle * acos(-1.0) / 180.0;
		//中心軸
		enepos_x2 = 300 * cos(theta);
		enepos_y2 = 300 * sin(theta) + 300;

		//実際の位置
		enepos_x3 = (300 + enepos_r) * cos(theta);
		enepos_y3 = (300 + enepos_r) * sin(theta) + 300;

	}
	if (ene_key == 545 || ene_key == 656 || ene_key == 767 || ene_key == 874) {
		angle -= 0.4;
		theta = angle * acos(-1.0) / 180.0;
		enepos_x2 = 300 * cos(theta);
		enepos_y2 = 300 * sin(theta) - 300;

		//実際の位置
		enepos_x3 = (300 + enepos_r) * cos(theta);
		enepos_y3 = (300 + enepos_r) * sin(theta) - 300;
	}

	if (ene_key == 121 || ene_key == 414 || ene_key == 343 || ene_key == 232 ) {
		angle += 0.4;
		theta = angle * acos(-1.0) / 180.0;
		enepos_x2 = 300 * cos(theta);
		enepos_y2 = 300 * sin(theta) + 300;

		//実際の位置
		enepos_x3 = (300 + enepos_r) * cos(theta);
		enepos_y3 = (300 + enepos_r) * sin(theta) + 300;
	}

	if (ene_key == 847 || ene_key == 776 || ene_key == 665 || ene_key == 554 ) {
		angle += 0.4;
		theta = angle * acos(-1.0) / 180.0;
		enepos_x2 = 300 * cos(theta);
		enepos_y2 = 300 * sin(theta) - 300;

		//実際の位置
		enepos_x3 = (300 + enepos_r) * cos(theta);
		enepos_y3 = (300 + enepos_r) * sin(theta) - 300;
	}

	if (ene_key == 971 || ene_key == 1053) {
		enepos_y2 += 2.0;
		if (ene_key == 971) {
			enepos_x2 = -300.0;
		}
		else {
			enepos_x2 = 300.0;
		}

		//実際の位置
		enepos_x3 = enepos_x2 + enepos_x;
		enepos_y3 = enepos_y2 + enepos_y;
	}
	if (ene_key == 917 || ene_key == 1035) {
		enepos_y2 -= 2.0;
		if (ene_key == 917) {
			enepos_x2 = -300.0;
		}
		else {
			enepos_x2 = 300.0;
		}

		//実際の位置
		enepos_x3 = enepos_x2 + enepos_x;
		enepos_y3 = enepos_y2 + enepos_y;
	}
	
	//cout << "enepos_x " << enepos_x << "   enepos_r " << enepos_r << "   turn " << turn << "   set " << set << endl;
}

//----------------------------------------------------
// obj(3Dモデル)が読み込めなかった場合の強制終了
//----------------------------------------------------
void Shutdown()
{
	mesh.Release(); //メッシュファイルの後片付け

	exit(0);
}

//----------------------------------------------------
// teapotの描画
//----------------------------------------------------
void teapot(float px, float py, float pz) {
	glPushMatrix(); /* 幾何変換開始(level 2, 但しlevel 1の変換情報は維持される) */

	glMaterialfv(GL_FRONT, GL_AMBIENT, ms_jade.ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, ms_jade.diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, ms_jade.specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, &ms_jade.shininess);

	glTranslatef(px, py, pz);
	glRotatef(0, 1.0, 1.0, 0.0); /* 回転軸(1, 1, 0)、回転角theta */
	glutSolidTeapot(1.0);        /* サイズ1のティーポット */
	glPopMatrix();               /* 幾何変換終了(level 2から抜けてlevel 1へ) */
}

//----------------------------------------------------
// 自機の描画
//----------------------------------------------------
void obj(float px, float py, float pz) {

	glPushMatrix();
	glTranslatef(px, py, pz);

	if (playkey == 112 || playkey == 223 || playkey == 334 || playkey == 441 || playkey == 874 || playkey == 545 || playkey == 656 || playkey == 767) {
		glRotatef(180  + angle2, 0.0, 0.0, 1.0);
		glRotatef(20, 0.0, 1.0, 0.0); //傾き
	}
	if (playkey == 121 || playkey == 414 || playkey == 343 || playkey == 232 || playkey == 847 || playkey == 776 || playkey == 665 || playkey == 554) {
		glRotatef( angle2, 0.0, 0.0, 1.0);
		glRotatef(-20, 0.0, 1.0, 0.0); //傾き
	}

	if (playkey == 971 || playkey == 1053) {
		
	}
	if (playkey == 917 || playkey == 1035) {
		glRotatef(-180, 0.0, 0.0, 1.0);
	}

	if (HP > 0) {
		//機体
		glPushMatrix();

		glScalef(0.4, 0.4, 0.4);
		glRotatef(90, 1.0, 0.0, 0.0);
		glRotatef(90, 0.0, 1.0, 0.0);
		mesh.Draw();

		glPopMatrix();

		//エフェクト
		glPushMatrix();
		glTranslatef(0.0, -4.7, 0.0);

		glPushMatrix();
		glRotatef(180, 0.0, 0.0, 1.0);
		glRotatef(20, 0.0, 1.0, 0.0);
		glScalef(1.3, 1.3, 1.3);
		fire.Move();
		fire.Render();
		glPopMatrix();

		glPushMatrix();
		glRotatef(180, 0.0, 0.0, 1.0);
		glRotatef(160, 0.0, 1.0, 0.0);
		glScalef(1.3, 1.3, 1.3);
		fire.Move();
		fire.Render();
		glPopMatrix();

		glPopMatrix();

		//エフェクト(パルス)
		if (hit2 >= 1) {

			pulse.SetColor(1.0f, 0.0f, 0.2f, 1.2f);
			pulse.SetSpeed(1.0f);
			pulse.SetPulse(true);
			pulse.Render(0.0f);

			hit2++;
			if (hit2 == 10) {
				hit2 = 0;
			}
		}
	}

	if (HP <= 0) {
		//爆発1
		glPushMatrix();
		glRotatef(90, 1.0, 0.0, 0.0);
		glScalef(4.0, 4.0, 4.0);
		smoke.Move();
		smoke.Render();
		glPopMatrix();

		//爆発2
		glPushMatrix();
		glRotatef(90, 1.0, 1.0, 0.0);
		glScalef(4.0, 4.0, 4.0);
		smoke.Move();
		smoke.Render();
		glPopMatrix();
	}

	if (bom_m == 0 && HP <= 0) {
		soundPlay(seReload, &sseReload, false);
		bom_m++;
	}

	glPopMatrix();

}

//----------------------------------------------------
// 敵機の描画
//----------------------------------------------------
void obj_ene(float px, float py, float pz) {
	//deyuo
	glPushMatrix();
	glTranslatef(px, py, pz);
	
	if (gamestart == 0) {
		glRotatef(200, 0.0, 0.0, 1.0); //停止用
	}

	/*
	if (enekey == 112 || enekey == 223 || enekey == 334 || enekey == 441 || enekey == 545 || enekey == 656 || enekey == 767 || enekey == 874) {
		glRotatef(20 + angle, 0.0, 0.0, 1.0);
	}
	if (enekey == 121 || enekey == 414 || enekey == 343 || enekey == 232 || enekey == 847 || enekey == 776 || enekey == 665 || enekey == 554) {
		glRotatef(-160 + angle, 0.0, 0.0, 1.0);
	}

	if (enekey == 971 || enekey == 1053) {
		glRotatef(200, 0.0, 0.0, 1.0);
	}
	if (enekey == 917 || enekey == 1035) {
		glRotatef(20, 0.0, 0.0, 1.0);
	}
	*/

	if (playkey_ene == 112 || playkey_ene == 223 || playkey_ene == 334 || playkey_ene == 441 || playkey_ene == 545 || playkey_ene == 656 || playkey_ene == 767 || playkey_ene == 874) {
		glRotatef(20 + angle, 0.0, 0.0, 1.0);
	}
	if (playkey_ene == 121 || playkey_ene == 414 || playkey_ene == 343 || playkey_ene == 232 || playkey_ene == 847 || playkey_ene == 776 || playkey_ene == 665 || playkey_ene == 554) {
		glRotatef(-160 + angle, 0.0, 0.0, 1.0);
	}

	if (playkey_ene == 971 || playkey_ene == 1053) {
		glRotatef(200, 0.0, 0.0, 1.0);
	}
	if (playkey_ene == 917 || playkey_ene == 1035) {
		glRotatef(20, 0.0, 0.0, 1.0);
	}
	
	if (HP_ene > 0) {
		//機体
		glPushMatrix();
		glScalef(1.5, 1.5, 1.5);
		glRotatef(90, 1.0, 0.0, 0.0);
		glTranslatef(-0.65, 0, 0); //中心の調整
		mesh_ene.Draw();
		glPopMatrix();

		//エフェクト
		if (hit >= 1) {
			glPushMatrix();
			glScalef(10.0, 10.0, 10.0);
			glTranslatef(-1.0, -1.0, -0.85);
			magicCircle.Update();
			magicCircle.Render();
			glPopMatrix();
			hit++;
			if (hit == 15) {
				hit = 0;
			}
		}
	}

	if (HP_ene <= 0) {

		//爆発1
		glPushMatrix();
		glRotatef(90, 1.0, 0.0, 0.0);
		glScalef(10.0, 10.0, 10.0);
		smoke.Move();
		smoke.Render();
		glPopMatrix();

		//爆発2
		glPushMatrix();
		glRotatef(90, 1.0, 1.0, 0.0);
		glScalef(10.0, 10.0, 10.0);
		smoke.Move();
		smoke.Render();
		glPopMatrix();
	}

	if (bom_m == 0 && HP_ene <= 0) {
		soundPlay(seReload, &sseReload, false);
		bom_m++;
	}

	glPopMatrix();
	
	

	/*
	//sahaquel
	glPushMatrix();
	glTranslatef(px, py, pz);
	
	if (enekey == 313 || enekey == 734 || enekey == 442 || enekey == 221) {
	glRotatef(90 + angle, 0.0, 0.0, 1.0);
	}
	if (enekey == 212 || enekey == 424 || enekey == 743 || enekey == 331) {
	glRotatef(-90 + angle, 0.0, 0.0, 1.0);
	}

	if (enekey == 151 || enekey == 845) {
	glRotatef(-90, 0.0, 0.0, 1.0);
	}
	if (enekey == 115 || enekey == 854) {
	glRotatef(90, 0.0, 0.0, 1.0);
	}
	if (enekey == 635 || enekey == 552) {
	glRotatef(0, 0.0, 0.0, 1.0);
	}
	if (enekey == 525 || enekey == 653) {
	glRotatef(180, 0.0, 0.0, 1.0);
	}
	

	//機体
	glPushMatrix();
	glScalef(4.0,4.0,4.0);
	glRotatef(90, 1.0, 0.0, 0.0);
	glTranslatef(-0.65, 0, 0); //中心の調整
	mesh_ene.Draw();
	glPopMatrix();

	//エフェクト
	if (hit >= 1) {
		glPushMatrix();
		glScalef(7.0, 7.0, 7.0);
		glTranslatef(-1.45, -1.0, -1.05);
		magicCircle.Update();
		magicCircle.Render();
		glPopMatrix();
		hit++;
		if (hit == 10) {
			hit = 0;
		}
	}
	glPopMatrix();
	*/
}

//----------------------------------------------------
// 背景(スカイドーム)の描画
//----------------------------------------------------
void skydome() {

	// テクスチャマッピング有効化 (追加 bmp)
	glEnable(GL_TEXTURE_2D);
	//　テクスチャをバインド (追加 bmp)
	//glBindTexture(GL_TEXTURE_2D, texture.ID);
	glBindTexture(GL_TEXTURE_2D, sky);

	glColor4fv(white);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, white);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, white);
	glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, white);

	glPushMatrix();
	GLUquadricObj* sphere;
	sphere = gluNewQuadric();
	gluQuadricDrawStyle(sphere, GLU_FILL);
	gluQuadricNormals(sphere, GLU_SMOOTH);
	gluQuadricTexture(sphere, GL_TRUE);
	//glRotatef(180, 0.0, 0.0, 1.0); //傾き
	glRotatef(180, 1.0, 0.0, 0.0); //傾き
	gluSphere(sphere, 900, 32, 32);
	glPopMatrix();

	gluDeleteQuadric(sphere);

	glBindTexture(GL_TEXTURE_2D, 0);
	//　テクスチャマッピング無効化
	glDisable(GL_TEXTURE_2D);
}

//----------------------------------------------------
// 大地の描画
//----------------------------------------------------
void Ground(void) {
	double ground_max_x = 400.0;
	double ground_max_y = 700.0;
	double angle_g = 0.0;
	double theta_g;

	glDisable(GL_LIGHTING);

	//バレットラインの表示------------------------------------------------------------

	glBegin(GL_LINES);
	
	for (double lx = -ground_max_x; lx <= ground_max_x; lx += 1.0) { //縦線(ライン)
		if (key == 971 || key == 1053 || key == 917 || key == 1035) {
			if (pos_x3 == lx) {
				glColor3d(1.0, 0.0, 0.0);
				glVertex3d(lx, ground_max_y, 0);
				glVertex3d(lx, -ground_max_y, 0);
			}
		}
	}
	glEnd();

	for (double radius = 10; radius <= 600; radius += 1.0) { //上円(ライン)
		if (key == 112 || key == 223 || key == 334 || key == 441 || key == 121 || key == 414 || key == 343 || key == 232) {
			if (300 + pos_r == radius) {
				glColor3d(1.0, 0.0, 0.0);
				glBegin(GL_LINE_LOOP);
				for (double i = angle_g; i <= 360.0; i++) {
					theta_g = i * acos(-1.0) / 180.0;
					glVertex2f(radius * cos(theta_g), radius * sin(theta_g) + 300);
				}
				glEnd();
			}
		}
	}

	for (double radius = 10; radius <= 600; radius += 1.0) { //下円(ライン)
		if (key == 874 || key == 545 || key == 656 || key == 767 || key == 847 || key == 776 || key == 665 || key == 554) {
			if (300 + pos_r == radius) {
				glColor3d(1.0, 0.0, 0.0);
				glBegin(GL_LINE_LOOP);
				for (double i = angle_g; i <= 360.0; i++) {
					theta_g = i * acos(-1.0) / 180.0;
					glVertex2f(radius * cos(theta_g), radius * sin(theta_g) - 300);
				}
				glEnd();
			}
		}
	}


	
	//大地の表示------------------------------------------------------------
	glColor3d(0.9, 0.8, 0.8); // 大地の色
	glBegin(GL_LINES);
	for (double ly = -ground_max_y; ly <= ground_max_y; ly += 10.0) { //横線
		glVertex3d(-ground_max_x, ly, 0);
		glVertex3d(ground_max_x, ly, 0);
	}
	for (double lx = -ground_max_x; lx <= ground_max_x; lx += 10.0) { //縦線
		glVertex3d(lx, ground_max_y, 0);
		glVertex3d(lx, -ground_max_y, 0);
	}
	glEnd();

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0); //光源0を利用
	glEnable(GL_LIGHT1); //光源1を利用
}

//----------------------------------------------------
// 弾の描画(自機)
//----------------------------------------------------
void bullet() {
	int i;
	
	if (pn > pn2) {
		pn2 = pn;
		//cout << pn2 << endl;
	}
	//再描写ごとに変位-------------------------
	for (i = 1; i <= pn2; i++) {
		//p[i].vx += ax * dt;
		//p[i].vy += ay * dt;
		//p[i].vz += az * dt;

		if (p[i].check == 0 && p[i].count <= 60) { //弾道が直線の場合 かつ 描画開始から240f以下
			p[i].x += p[i].vx * dt;
			p[i].y += p[i].vy * dt;
			p[i].z += p[i].vz * dt;
			p[i].count++;
		}

		if (p[i].check == 1 && p[i].count <= 60) { //弾道が曲線の場合 かつ 描画開始から240f以下
			if (p[i].key_tmp == 112 || p[i].key_tmp == 223 || p[i].key_tmp == 334 || p[i].key_tmp == 441 || p[i].key_tmp == 874 || p[i].key_tmp == 545 || p[i].key_tmp == 656 || p[i].key_tmp == 767) {
				p[i].angle -= 2.0;
			}
			if (p[i].key_tmp == 121 || p[i].key_tmp == 414 || p[i].key_tmp == 343 || p[i].key_tmp == 232 || p[i].key_tmp == 847 || p[i].key_tmp == 776 || p[i].key_tmp == 665 || p[i].key_tmp == 554) {
				p[i].angle += 2.0;
			}
			p[i].theta = p[i].angle * acos(-1.0) / 180.0;

			p[i].x = (300 + p[i].radius) * cos(p[i].theta);
			if (p[i].key_tmp == 112 || p[i].key_tmp == 223 || p[i].key_tmp == 334 || p[i].key_tmp == 441 || p[i].key_tmp == 121 || p[i].key_tmp == 414 || p[i].key_tmp == 343 || p[i].key_tmp == 232) {
				p[i].y = (300 + p[i].radius) * sin(p[i].theta) + 300;
			}
			else {
				p[i].y = (300 + p[i].radius) * sin(p[i].theta) - 300;
			}
			p[i].count++;
		}

		if(p[i].count > 60) { //表示する必要がない弾を逃がす(240f後)
			if (p[i].y == 1000 && p[i].y == 0) {

			}
			else {
				p[i].x = 1000;
				p[i].y = 0;
			}
		}
		
		//当たり判定(敵機)
		if ((p[i].x - enepos_x3)*(p[i].x - enepos_x3) + (p[i].y - enepos_y3)*(p[i].y - enepos_y3) < 10.0) {
			if (HP_ene > 0) {
				HP_ene -= 3;
			}
			hit = 1;

			//重みを追加
			if (curve_decision(p[i].key_tmp)) {
				for (j = 1; j <= 8; j++) {
					weight[j] += 0.2;
				}
			}
			else {
				weight[9]  += 0.2;
				weight[10] += 0.2;
			}
			weight[p[i].key_tmp / 100] += 0.8;

			for (j = 0; j <= 10; j++) {
				//cout << "weight[" << j << "] : " << weight[j] << endl;
			}
			//cout << "weight " << p[i].key_tmp / 100 << " :" << weight[p[i].key_tmp / 100] << endl;
			//cout << "Hit   " << "HP : " << HP_ene << endl;
		}

		glPushMatrix();
		if (p[i].count <= 60) {
			glTranslated(p[i].x, p[i].y, p[i].z); //弾を移動
			
			if (p[i].key_tmp == 112 || p[i].key_tmp == 223 || p[i].key_tmp == 334 || p[i].key_tmp == 441 || p[i].key_tmp == 874 || p[i].key_tmp == 545 || p[i].key_tmp == 656 || p[i].key_tmp == 767) {
				glRotatef(p[i].angle, 0.0, 0.0, 1.0);
			}
			if (p[i].key_tmp == 121 || p[i].key_tmp == 414 || p[i].key_tmp == 343 || p[i].key_tmp == 232 || p[i].key_tmp == 847 || p[i].key_tmp == 776 || p[i].key_tmp == 665 || p[i].key_tmp == 554) {
				glRotatef(p[i].angle, 0.0, 0.0, 1.0);
			}
			if (p[i].key_tmp == 971 || p[i].key_tmp == 1053) {
				
			}
			if (p[i].key_tmp == 917 || p[i].key_tmp == 1035) {
				
			}
			//glutSolidSphere(1.0, 20, 20);
			beam.Draw();
		}
		glPopMatrix();
	}
}

//----------------------------------------------------
// 弾道(直線)
//----------------------------------------------------
void bullet_straight() {
	pn = (pn + 1) % bullet_num;
	if (pn == 0) {
		pn = 1;
	}
	p[pn].x = pos_x2 + pos_x;
	p[pn].y = pos_y2 + pos_y;
	p[pn].z = pos_z2 + pos_z;
	p[pn].check = 0;
	p[pn].count = 0;
	p[pn].key_tmp = key;

	if (key == 971 || key == 1053) {
		p[pn].vx = 0.0;
		p[pn].vy = 100.0;
	}
	if (key == 917 || key == 1035) {
		p[pn].vx = 0.0;
		p[pn].vy = (-1.0) * 100.0;
	}
}

//----------------------------------------------------
// 弾道(曲線)
//----------------------------------------------------
void bullet_curve() {
	pn = (pn + 1) % bullet_num;
	if (pn == 0) {
		pn = 1;
	}
	p[pn].angle = angle2;
	p[pn].radius = pos_r;
	p[pn].check = 1;
	p[pn].count = 0;
	p[pn].key_tmp = key;
}

//----------------------------------------------------
// 弾の描画(敵機)2
//----------------------------------------------------
void bullet2() {
	int i;

	if (epn > epn2) {
		epn2 = epn;
		//cout << epn2 << endl;
	}
	//再描写ごとに変位-------------------------
	for (i = 1; i <= epn2; i++) {
		//p[i].vx += ax * dt;
		//p[i].vy += ay * dt;
		//p[i].vz += az * dt;

		if (ep[i].check == 0 && ep[i].count <= 70) { //弾道が直線の場合 かつ 描画開始から240f以下
			ep[i].x += ep[i].vx * edt;
			ep[i].y += ep[i].vy * edt;
			ep[i].z += ep[i].vz * edt;
			ep[i].count++;
		}

		if (ep[i].check == 1 && ep[i].count <= 70) { //弾道が曲線の場合 かつ 描画開始から240f以下
													  //角度決定
			if (ep[i].key_tmp == 112 || ep[i].key_tmp == 223 || ep[i].key_tmp == 334 || ep[i].key_tmp == 441 || ep[i].key_tmp == 874 || ep[i].key_tmp == 545 || ep[i].key_tmp == 656 || ep[i].key_tmp == 767) {
				ep[i].angle += 0.5;
			}
			if (ep[i].key_tmp == 121 || ep[i].key_tmp == 414 || ep[i].key_tmp == 343 || ep[i].key_tmp == 232 || ep[i].key_tmp == 847 || ep[i].key_tmp == 776 || ep[i].key_tmp == 665 || ep[i].key_tmp == 554) {
				ep[i].angle -= 0.5;
			}
			ep[i].theta = ep[i].angle * acos(-1.0) / 180.0;

			//座標計算
			ep[i].x = (300 + ep[i].radius) * cos(ep[i].theta);
			if (ep[i].key_tmp == 112 || ep[i].key_tmp == 223 || ep[i].key_tmp == 334 || ep[i].key_tmp == 441 || ep[i].key_tmp == 121 || ep[i].key_tmp == 414 || ep[i].key_tmp == 343 || ep[i].key_tmp == 232) {
				ep[i].y = (300 + ep[i].radius) * sin(ep[i].theta) + 300;
			}
			if (ep[i].key_tmp == 874 || ep[i].key_tmp == 545 || ep[i].key_tmp == 656 || ep[i].key_tmp == 767 || ep[i].key_tmp == 847 || ep[i].key_tmp == 776 || ep[i].key_tmp == 665 || ep[i].key_tmp == 554) {
				ep[i].y = (300 + ep[i].radius) * sin(ep[i].theta) - 300;
			}

			ep[i].count++;
		}

		if (ep[i].check == 2 && ep[i].count <= 70) {
			ep[i].x = ep[i - 1].x - 0.5 * ep[i].count;
			ep[i].y = ep[i - 1].y - 0.5 * ep[i].count;
			ep[i].count++;
		}

		if (ep[i].check == 3 && ep[i].count <= 70) {
			ep[i].x = ep[i - 1].x - 1.0 * ep[i].count;
			ep[i].y = ep[i - 1].y - 1.0 * ep[i].count;
			ep[i].count++;
		}

		if (ep[i].check == 4 && ep[i].count <= 70) {
			ep[i].x = ep[i - 1].x + 0.5 * ep[i].count;
			ep[i].y = ep[i - 1].y + 0.5 * ep[i].count;
			ep[i].count++;
		}

		if (ep[i].check == 5 && ep[i].count <= 70) { //弾道が停滞の場合 かつ 描画開始から240f以下
			ep[i].count++;
		}

		if (ep[i].count > 70) { //表示する必要がない弾を逃がす(240f後)
			if (ep[i].y == 1000 && ep[i].y == 0) {

			}
			else {
				ep[i].x = 1000;
				ep[i].y = 0;
			}
		}



		//当たり判定(敵機)
		if (ep[i].count <= 70) {
			if ((ep[i].x - pos_x3)*(ep[i].x - pos_x3) + (ep[i].y - pos_y3)*(ep[i].y - pos_y3) < 10.0) {
				if (HP > 0) {
					HP -= 8;
				}
				hit2 = 1;
				//weight[p[i].key_tmp / 100]++;

				//cout << "weight " << p[i].key_tmp / 100 << " :" << weight[p[i].key_tmp / 100] << endl;
				//cout << "Hit   " << "HP : " << HP_ene << endl;
			}
		}

		glPushMatrix();

		//glMaterialfv(GL_FRONT, GL_AMBIENT, ms_ruby.ambient);
		//glMaterialfv(GL_FRONT, GL_DIFFUSE, ms_ruby.diffuse);
		//glMaterialfv(GL_FRONT, GL_SPECULAR, ms_ruby.specular);
		//glMaterialfv(GL_FRONT, GL_SHININESS, &ms_ruby.shininess);
		if (ep[i].count <= 70) {
			glTranslated(ep[i].x, ep[i].y, ep[i].z);//平行移動値の設定

			if (ep[i].key_tmp == 112 || ep[i].key_tmp == 223 || ep[i].key_tmp == 334 || ep[i].key_tmp == 441 || ep[i].key_tmp == 874 || ep[i].key_tmp == 545 || ep[i].key_tmp == 656 || ep[i].key_tmp == 767) {
				glRotatef(ep[i].angle, 0.0, 0.0, 1.0);
			}
			if (ep[i].key_tmp == 121 || ep[i].key_tmp == 414 || ep[i].key_tmp == 343 || ep[i].key_tmp == 232 || ep[i].key_tmp == 847 || ep[i].key_tmp == 776 || ep[i].key_tmp == 665 || ep[i].key_tmp == 554) {
				glRotatef(ep[i].angle, 0.0, 0.0, 1.0);
			}
			if (ep[i].key_tmp == 971 || ep[i].key_tmp == 1053) {

			}
			if (ep[i].key_tmp == 917 || ep[i].key_tmp == 1035) {

			}

			glMaterialfv(GL_FRONT, GL_AMBIENT, ms_ruby.ambient);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, ms_ruby.diffuse);
			glMaterialfv(GL_FRONT, GL_SPECULAR, ms_ruby.specular);
			glMaterialfv(GL_FRONT, GL_SHININESS, &ms_ruby.shininess);

			if (ep[i].check == 5) {
				glMaterialfv(GL_FRONT, GL_AMBIENT, ms_jade.ambient);
				glMaterialfv(GL_FRONT, GL_DIFFUSE, ms_jade.diffuse);
				glMaterialfv(GL_FRONT, GL_SPECULAR, ms_jade.specular);
				glMaterialfv(GL_FRONT, GL_SHININESS, &ms_jade.shininess);
			}

			//beam.Draw();
			glutSolidSphere(1.0, 20, 20);//引数：(半径, Z軸まわりの分割数, Z軸に沿った分割数)
		}
		glPopMatrix();
	}
}

//----------------------------------------------------
// 弾道(直線)2
//----------------------------------------------------
void bullet_straight2() {
	/*
	//単発
	epn = (epn + 1) % bullet_num;
	if (epn == 0) {
	epn = 1;
	}
	ep[epn].x = enepos_x2 + enepos_x;
	ep[epn].y = enepos_y2 + enepos_y;
	ep[epn].z = enepos_z2 + enepos_z;
	ep[epn].check = 0;
	ep[epn].count = 0;
	ep[epn].key_tmp = enekey;

	if (enekey == 971 || enekey == 1053) {
	ep[epn].vx = 0.0;
	ep[epn].vy = (-1.0) * 40.0;
	}
	if (enekey == 917 || enekey == 1035) {
	ep[epn].vx = 0.0;
	ep[epn].vy = 40.0;
	}
	*/

	/*
	//3連
	epn = (epn + 1) % bullet_num;
	if (epn == 0) {
		epn = 1;
	}
	for (j = epn; j < epn + 3; j++) {
		ep[j].x = enepos_x2 + enepos_x;
		ep[j].y = enepos_y2 + enepos_y;
		ep[j].z = enepos_z2 + enepos_z;
		ep[j].check = 0;
		ep[j].count = 0;
		ep[j].key_tmp = enekey;
	}
	for (j = epn; j < epn + 3; j++) {
		if (enekey == 971 || enekey == 1053) {
			if (enepos_x < pos_x) {
				ep[j].vx = -5.0 + 5.0 * (j - epn) + abs(pos_x - enepos_x) * 0.26;
				ep[j].vy = (-1.0) * 20.0;
			}
			else {
				ep[j].vx = -5.0 + 5.0 * (j - epn) - abs(pos_x - enepos_x) * 0.26;
				ep[j].vy = (-1.0) * 20.0;
			}

			//ep[j].vx = -7.5 + 5.0 * (j - epn);
			//ep[j].vy = (-1.0) * 20.0;
		}
		if (enekey == 917 || enekey == 1035) {
			if (enepos_x > pos_x) {
				ep[j].vx = -5.0 + 5.0 * (j - epn) + abs(pos_x - enepos_x) * 0.26;
				ep[j].vy = (-1.0) * 20.0;
			}
			else {
				ep[j].vx = -5.0 + 5.0 * (j - epn) - abs(pos_x - enepos_x) * 0.26;
				ep[j].vy = (-1.0) * 20.0;
			}
		}
	}
	epn = epn + 3;
	*/

	
	//4連
	epn = (epn + 1) % bullet_num;
	if (epn == 0) {
		epn = 1;
	}
	for (j = epn; j < epn + 4; j++) {
		ep[j].x = enepos_x2 + enepos_x;
		ep[j].y = enepos_y2 + enepos_y;
		ep[j].z = enepos_z2 + enepos_z;
		ep[j].check = 0;
		ep[j].count = 0;
		ep[j].key_tmp = enekey;
	}
	for (j = epn; j < epn + 4; j++) {
		if (enekey == 971 || enekey == 1053) {
			ep[j].vx = -7.5 + 5.0 * (j - epn);
			ep[j].vy = (-1.0) * 25.0;
		}
		if (enekey == 917 || enekey == 1035) {
			ep[j].vx = -7.5 + 5.0 * (j - epn);
			ep[j].vy = 25.0;
		}
	}
	epn = epn + 3;
	
}

//----------------------------------------------------
// 弾道(曲線)2
//----------------------------------------------------
void bullet_curve2() {
	/*
	epn = (epn + 1) % bullet_num;
	if (epn == 0) {
		epn = 1;
	}
	ep[epn].angle = angle;
	ep[epn].radius = enepos_r;
	ep[epn].check = 1;
	ep[epn].count = 0;
	ep[epn].key_tmp = enekey;
	*/

	epn = (epn + 1) % bullet_num;
	if (epn == 0) {
		epn = 1;
	}

	for (j = epn; j < epn + 4; j++) {
		ep[j].angle = angle;
		ep[j].radius = enepos_r;
		ep[j].check = (j - epn) + 1;
		ep[j].count = 0;
		ep[j].key_tmp = enekey;
	}
	epn = epn + 3;

	//3連
	/*
	for (j = epn; j < epn + 3; j++) {
		ep[j].angle = angle;
		ep[j].radius = enepos_r;
		ep[j].check = (j - epn) + 1;
		ep[j].count = 0;
		ep[j].key_tmp = enekey;
	}
	epn = epn + 2;
	*/
}

//----------------------------------------------------
// 弾道(停滞)
//----------------------------------------------------
void bullet_stay() {
	epn = (epn + 1) % bullet_num;
	if (epn == 0) {
		epn = 1;
	}

	if (curve_decision(enekey)) {
		ep[epn].x = (300 + enepos_r) * cos(theta);
		if (enekey == 112 || enekey == 223 || enekey == 334 || enekey == 441 || enekey == 121 || enekey == 414 || enekey == 343 || enekey == 232) {
			ep[epn].y = (300 + enepos_r) * sin(theta) + 300;
		}
		else {
			ep[epn].y = (300 + enepos_r) * sin(theta) - 300;
		}

		ep[epn].check = 5;
		ep[epn].count = 0;
		ep[epn].key_tmp = enekey;
	}
	else {
		ep[epn].x = enepos_x2 + enepos_x;
		ep[epn].y = enepos_y2 + enepos_y;
		ep[epn].z = enepos_z2 + enepos_z;
		ep[epn].check = 5;
		ep[epn].count = 0;
		ep[epn].key_tmp = enekey;
	}
}

//----------------------------------------------------
// 弾道(ホーミング1)
//----------------------------------------------------
void bullet_homing1(){
	pn = (pn + 1) % bullet_num;
	p[pn].x = pos_x2;
	p[pn].y = pos_y2;
	p[pn].z = pos_z2;

	theta3 = (angle - 12.0) * acos(-1.0) / 180.0;
	tarpos_x = 300 * cos(theta3);
	tarpos_y = 300 * sin(theta3);
	//cout << tarpos_x << "  " << tarpos_y << endl;

	if (tarpos_y > pos_y2) {
		bul_dy = tarpos_y - pos_y2;
	}
	else if (pos_y2 > tarpos_y) {
		bul_dy = pos_y2 - tarpos_y;
	}
	else {
		bul_dy = 0;
	}

	if (tarpos_x > pos_x2) {
		bul_dx = tarpos_x - pos_x2;
	}
	else if (pos_x2 > tarpos_x) {
		bul_dx = pos_x2 - tarpos_x;
	}
	else {
		bul_dx = 0;
	}



	if (tarpos_x > pos_x2) {
		p[pn].vx = bul_dx / 6;
	}
	else if (pos_x2 > tarpos_x) {
		p[pn].vx = (-1.0) * bul_dx / 6;
	}
	else {
		p[pn].vx = 0;
	}

	if (tarpos_y > pos_y2) {
		p[pn].vy = bul_dy / 6;
	}
	else if (pos_y2 > tarpos_y) {
		p[pn].vy = (-1.0) * bul_dy / 6;
	}
	else {
		p[pn].vy = 0;
	}

	//cout << p[pn].vx << "  " << p[pn].vy << endl;
}

//----------------------------------------------------
// 弾道(ホーミング2)
//----------------------------------------------------
void bullet_homing2(){
	pn = (pn + 1) % bullet_num;
	p[pn].x = pos_x2;
	p[pn].y = pos_y2;
	p[pn].z = pos_z2;

	theta3 = (angle + 12.0) * acos(-1.0) / 180.0;
	tarpos_x = 300 * cos(theta3);
	tarpos_y = 300 * sin(theta3);
	//cout << tarpos_x << "  " << tarpos_y << endl;

	if (tarpos_y > pos_y2) {
		bul_dy = tarpos_y - pos_y2;
	}
	else if (pos_y2 > tarpos_y) {
		bul_dy = pos_y2 - tarpos_y;
	}
	else {
		bul_dy = 0;
	}

	if (tarpos_x > pos_x2) {
		bul_dx = tarpos_x - pos_x2;
	}
	else if (pos_x2 > tarpos_x) {
		bul_dx = pos_x2 - tarpos_x;
	}
	else {
		bul_dx = 0;
	}



	if (tarpos_x > pos_x2) {
		p[pn].vx = bul_dx / 6;
	}
	else if (pos_x2 > tarpos_x) {
		p[pn].vx = (-1.0) * bul_dx / 6;
	}
	else {
		p[pn].vx = 0;
	}

	if (tarpos_y > pos_y2) {
		p[pn].vy = bul_dy / 6;
	}
	else if (pos_y2 > tarpos_y) {
		p[pn].vy = (-1.0) * bul_dy / 6;
	}
	else {
		p[pn].vy = 0;
	}
}

//----------------------------------------------------
// 2Dの描画(HPゲージ)
//----------------------------------------------------
void Draw_2D() {
	// 3D　→　2D (2Dに描画を切り替える)
	bool isLighting = false;

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, WindowWidth, WindowHeight, 0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	if (glIsEnabled(GL_LIGHTING))
	{
		isLighting = true;
		glDisable(GL_LIGHTING);
	}

	standard_w = WindowWidth / 128;   // standard_w = 10
	standard_h = standard_w /10;  // standard_h =  1
	//cout << "w : " << standard_w << "  h : " << standard_h << endl;

	// 文字の描画--------------------------------------

	glColor4f(1.0, 1.0, 1.0, 1.0);
	glRasterPos2i(930, 45);
	//glutRenderText(GLUT_BITMAP_HELVETICA_12, "Enemy");
	glRasterPos2i(30, 45);
	//glutRenderText(GLUT_BITMAP_HELVETICA_12, "Player");

	//glRasterPos2i(15, 45);
	//glutRenderText(GLUT_BITMAP_HELVETICA_12, "check3");
	//glRasterPos2i(15, 60);
	//glutRenderText(GLUT_BITMAP_HELVETICA_12, "check4");


	// 図形の描画--------------------------------------

	HP_var = HP;
	HP_var_ene = HP_ene;

	if (HP_var < 0) {
		HP_var = 0;
	}
	if (HP_var_ene < 0) {
		HP_var_ene = 0;
	}
	//残りHP(自機)
	if (HP > 50) {
		glColor4f(0.0, 1.0, 0.4, 1.0); //緑
	}
	else if (HP > 25) {
		glColor4f(1.0, 1.0, 0.0, 1.0); //黄
	}
	else {
		glColor4f(1.0, 0.0, 0.0, 1.0); //赤
	}

	glBegin(GL_QUADS);
	glVertex2f(standard_w * 10 + (100 - HP_var)*2.9 * standard_h, standard_h * 35);
	glVertex2f(standard_w * 39, standard_h * 35);
	glVertex2f(standard_w * 39, standard_h * 45);
	glVertex2f(standard_w * 10 + (100 - HP_var)*2.9 * standard_h, standard_h * 45);
	glEnd();

	//減少したHP(自機)
	glColor4f(0.4, 0.4, 0.4, 1.0);
	glBegin(GL_QUADS);
	glVertex2f(standard_w * 10, standard_h * 35);
	glVertex2f(standard_w * 10 + (100 - HP_var) * 2.9 * standard_h, standard_h * 35);
	glVertex2f(standard_w * 10 + (100 - HP_var) * 2.9 * standard_h, standard_h * 45);
	glVertex2f(standard_w * 10, standard_h * 45);
	glEnd();

	//HP枠 1(自機)
	glColor4f(0.4, 0.4, 0.4, 1.0);
	glBegin(GL_QUADS);
	glVertex2f(standard_w * 10 - 1, standard_h * 34);
	glVertex2f(standard_w * 39 + 1, standard_h * 34);
	glVertex2f(standard_w * 39 + 1, standard_h * 46);
	glVertex2f(standard_w * 10 - 1, standard_h * 46);
	glEnd();

	//HP枠 2(自機)
	glColor4f(0.8, 0.8, 0.8, 1.0);
	glBegin(GL_QUADS);
	glVertex2f(standard_w * 10 - 2, standard_h * 33);
	glVertex2f(standard_w * 39 + 2, standard_h * 33);
	glVertex2f(standard_w * 39 + 2, standard_h * 47);
	glVertex2f(standard_w * 10 - 2, standard_h * 47);
	glEnd();


	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, player_name.ID);
	glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, white);

	//名前(自機)
	glColor4f(0.4, 0.4, 0.4, 2.0);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 1.0); glVertex2f(standard_w * 5 - 4, standard_h * 35);
	glTexCoord2f(1.0, 1.0); glVertex2f(standard_w * 9 + 4, standard_h * 35);
	glTexCoord2f(1.0, 0.0); glVertex2f(standard_w * 9 + 4, standard_h * 45);
	glTexCoord2f(0.0, 0.0); glVertex2f(standard_w * 5 - 4, standard_h * 45);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);


	//ゲージ(自機)
	glColor4f(0.6, 0.6, 1.0, 1.0);
	glBegin(GL_QUADS);
	glVertex2f(standard_w * 4, standard_h * 25);
	glVertex2f(standard_w * 40, standard_h * 25);
	glVertex2f(standard_w * 40, standard_h * 55);
	glVertex2f(standard_w * 4, standard_h * 55);
	glEnd();

	//ゲージ枠1(自機)
	glColor4f(0.8, 0.8, 0.8, 1.0);
	glBegin(GL_QUADS);
	glVertex2f(standard_w * 4 - 2, standard_h * 23);
	glVertex2f(standard_w * 40 + 2, standard_h * 23);
	glVertex2f(standard_w * 40 + 2, standard_h * 57);
	glVertex2f(standard_w * 4 - 2, standard_h * 57);
	glEnd();

	//ゲージ枠2(自機)
	glColor4f(0.9, 0.9, 0.9, 1.0);
	glBegin(GL_QUADS);
	glVertex2f(standard_w * 4 - 4, standard_h * 22);
	glVertex2f(standard_w * 40 + 4, standard_h * 22);
	glVertex2f(standard_w * 40 + 4, standard_h * 58);
	glVertex2f(standard_w * 4 - 4, standard_h * 58);
	glEnd();



	//残りHP(敵)
	if (HP_ene > 50) {
		glColor4f(0.0, 1.0, 0.4, 1.0); //緑
	}
	else if (HP_ene > 25) {
		glColor4f(1.0, 1.0, 0.0, 1.0); //黄
	}
	else {
		glColor4f(1.0, 0.0, 0.0, 1.0); //赤
	}

	glBegin(GL_QUADS);
	glVertex2f(standard_w * 94 + (100 - HP_var_ene)*2.9 * standard_h, standard_h * 35);
	glVertex2f(standard_w * 123, standard_h * 35);
	glVertex2f(standard_w * 123, standard_h * 45);
	glVertex2f(standard_w * 94 + (100 - HP_var_ene)*2.9 * standard_h, standard_h * 45);
	glEnd();

	//減少したHP(敵)
	glColor4f(0.4, 0.4, 0.4, 1.0);
	glBegin(GL_QUADS);
	glVertex2f(standard_w * 94, standard_h * 35);
	glVertex2f(standard_w * 94 + (100 - HP_var_ene) * 2.9 * standard_h, standard_h * 35);
	glVertex2f(standard_w * 94 + (100 - HP_var_ene) * 2.9 * standard_h, standard_h * 45);
	glVertex2f(standard_w * 94, standard_h * 45);
	glEnd();

	//HP枠 1(敵)
	glColor4f(0.4, 0.4, 0.4, 1.0);
	glBegin(GL_QUADS);
	glVertex2f(standard_w * 94 - 1, standard_h * 34);
	glVertex2f(standard_w * 123 + 1, standard_h * 34);
	glVertex2f(standard_w * 123 + 1, standard_h * 46);
	glVertex2f(standard_w * 94 - 1, standard_h * 46);
	glEnd();

	//HP枠 2(敵)
	glColor4f(0.8, 0.8, 0.8, 1.0);
	glBegin(GL_QUADS);
	glVertex2f(standard_w * 94 - 2, standard_h * 33);
	glVertex2f(standard_w * 123 + 2, standard_h * 33);
	glVertex2f(standard_w * 123 + 2, standard_h * 47);
	glVertex2f(standard_w * 94 - 2, standard_h * 47);
	glEnd();


	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, enemy_name.ID);
	glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, white);

	//名前(敵機)
	glColor4f(0.4, 0.4, 0.4, 2.0);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 1.0); glVertex2f(standard_w * 89 - 4, standard_h * 35);
	glTexCoord2f(1.0, 1.0); glVertex2f(standard_w * 93 + 4, standard_h * 35);
	glTexCoord2f(1.0, 0.0); glVertex2f(standard_w * 93 + 4, standard_h * 45);
	glTexCoord2f(0.0, 0.0); glVertex2f(standard_w * 89 - 4, standard_h * 45);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);


	//ゲージ(敵)
	glColor4f(1.0, 0.6, 0.6, 1.0);
	glBegin(GL_QUADS);
	glVertex2f(standard_w * 88, standard_h * 25);
	glVertex2f(standard_w * 124, standard_h * 25);
	glVertex2f(standard_w * 124, standard_h * 55);
	glVertex2f(standard_w * 88, standard_h * 55);
	glEnd();

	//ゲージ枠1(敵)
	glColor4f(0.8, 0.8, 0.8, 1.0);
	glBegin(GL_QUADS);
	glVertex2f(standard_w * 88 - 2, standard_h * 23);
	glVertex2f(standard_w * 124 + 2, standard_h * 23);
	glVertex2f(standard_w * 124 + 2, standard_h * 57);
	glVertex2f(standard_w * 88 - 2, standard_h * 57);
	glEnd();

	//ゲージ枠2(敵)
	glColor4f(0.9, 0.9, 0.9, 1.0);
	glBegin(GL_QUADS);
	glVertex2f(standard_w * 88 - 4, standard_h * 22);
	glVertex2f(standard_w * 124 + 4, standard_h * 22);
	glVertex2f(standard_w * 124 + 4, standard_h * 58);
	glVertex2f(standard_w * 88 - 4, standard_h * 58);
	glEnd();

	//経過タイム
	if (HP_ene > 0 && HP > 0) {
		end_time = timeGetTime();
	}
	//cout << (end_time - start_time) / 60000 << " " << (((end_time - start_time) % 60000) / 1000) << " " << (((end_time - start_time) % 1000) / 10) << endl;
	current_time = end_time - start_time;
	
	//タイム調整
	if (current_time <= 3000) {
		current_time = 0;
	}
	else {
		current_time -= 3000;
	}


	//1
	glEnable(GL_TEXTURE_2D);
	time_texture(current_time / 60000, 1);
	//glBindTexture(GL_TEXTURE_2D, player_name.ID);
	glColor4f(0.0, 0.0, 0.0, 1.0);
	glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, white);

	glBegin(GL_QUADS);
	glTexCoord2f(1.0, 1.0); glVertex2f(standard_w * 52, standard_h * 10);
	glTexCoord2f(0.0, 1.0); glVertex2f(standard_w * 48, standard_h * 10);
	glTexCoord2f(0.0, 0.0); glVertex2f(standard_w * 48, standard_h * 70);
	glTexCoord2f(1.0, 0.0); glVertex2f(standard_w * 52, standard_h * 70);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);


	//2
	glEnable(GL_TEXTURE_2D);
	time_texture(current_time / 60000, 2);
	//glBindTexture(GL_TEXTURE_2D, number0.ID);
	glColor4f(0.0, 0.0, 0.0, 1.0);
	glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, white);

	glBegin(GL_QUADS);
	glTexCoord2f(1.0, 1.0); glVertex2f(standard_w * 56, standard_h * 10);
	glTexCoord2f(0.0, 1.0); glVertex2f(standard_w * 52, standard_h * 10);
	glTexCoord2f(0.0, 0.0); glVertex2f(standard_w * 52, standard_h * 70);
	glTexCoord2f(1.0, 0.0); glVertex2f(standard_w * 56, standard_h * 70);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);


	//2.5
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, colon.ID);
	glColor4f(0.0, 0.0, 0.0, 1.0);
	glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, white);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 1.0);  glVertex2f(standard_w * 56, standard_h * 10);
	glTexCoord2f(1.0, 1.0);  glVertex2f(standard_w * 60, standard_h * 10);
	glTexCoord2f(1.0, 0.0);  glVertex2f(standard_w * 60, standard_h * 70);
	glTexCoord2f(0.0, 0.0);  glVertex2f(standard_w * 56, standard_h * 70);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);


	//3
	glEnable(GL_TEXTURE_2D);
	time_texture(((current_time % 60000) / 1000), 1);
	//glBindTexture(GL_TEXTURE_2D, number1.ID);
	glColor4f(0.0, 0.0, 0.0, 1.0);
	glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, white);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 1.0); glVertex2f(standard_w * 60, standard_h * 10);
	glTexCoord2f(1.0, 1.0); glVertex2f(standard_w * 64, standard_h * 10);
	glTexCoord2f(1.0, 0.0); glVertex2f(standard_w * 64, standard_h * 70);
	glTexCoord2f(0.0, 0.0); glVertex2f(standard_w * 60, standard_h * 70);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);


	//4
	glEnable(GL_TEXTURE_2D);
	time_texture(((current_time % 60000) / 1000), 2);
	//glBindTexture(GL_TEXTURE_2D, number2.ID);
	glColor4f(0.0, 0.0, 0.0, 1.0);
	glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, white);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 1.0); glVertex2f(standard_w * 64, standard_h * 10);
	glTexCoord2f(1.0, 1.0); glVertex2f(standard_w * 68, standard_h * 10);
	glTexCoord2f(1.0, 0.0); glVertex2f(standard_w * 68, standard_h * 70);
	glTexCoord2f(0.0, 0.0); glVertex2f(standard_w * 64, standard_h * 70);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);


	//4.5
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, colon.ID);
	glColor4f(0.0, 0.0, 0.0, 1.0);
	glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, white);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 1.0); glVertex2f(standard_w * 68, standard_h * 10);
	glTexCoord2f(1.0, 1.0); glVertex2f(standard_w * 72, standard_h * 10);
	glTexCoord2f(1.0, 0.0); glVertex2f(standard_w * 72, standard_h * 70);
	glTexCoord2f(0.0, 0.0); glVertex2f(standard_w * 68, standard_h * 70);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);


	//5
	glEnable(GL_TEXTURE_2D);
	time_texture(((current_time % 1000) / 10), 1);
	//glBindTexture(GL_TEXTURE_2D, number5.ID);
	glColor4f(0.0, 0.0, 0.0, 1.0);
	glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, white);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 1.0); glVertex2f(standard_w * 72, standard_h * 10);
	glTexCoord2f(1.0, 1.0); glVertex2f(standard_w * 76, standard_h * 10);
	glTexCoord2f(1.0, 0.0); glVertex2f(standard_w * 76, standard_h * 70);
	glTexCoord2f(0.0, 0.0); glVertex2f(standard_w * 72, standard_h * 70);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);


	//6
	glEnable(GL_TEXTURE_2D);
	time_texture(((current_time % 1000) / 10), 2);
	//glBindTexture(GL_TEXTURE_2D, number4.ID);
	glColor4f(0.0, 0.0, 0.0, 1.0);
	glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, white);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 1.0); glVertex2f(standard_w * 76, standard_h * 10);
	glTexCoord2f(1.0, 1.0); glVertex2f(standard_w * 80, standard_h * 10);
	glTexCoord2f(1.0, 0.0); glVertex2f(standard_w * 80, standard_h * 70);
	glTexCoord2f(0.0, 0.0); glVertex2f(standard_w * 76, standard_h * 70);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);


	//ゲーム開始
	if (((current_time % 1000) / 10) >= 0.000001) {
		if (gamestart2 == 0) {
			enekey = 971;
			key = 971;
			gamestart = 1;
			gamestart2 = 1;
		}
	}


	//タイム
	glColor4f(0.6, 0.6, 0.6, 1.0);
	glBegin(GL_QUADS);
	glVertex2f(standard_w * 48 - 2, standard_h * 8);
	glVertex2f(standard_w * 80 + 2, standard_h * 8);
	glVertex2f(standard_w * 80 + 2, standard_h * 72);
	glVertex2f(standard_w * 48 - 2, standard_h * 72);
	glEnd();

	//タイム枠
	glColor4f(0.8, 0.8, 0.8, 1.0);
	glBegin(GL_QUADS);
	glVertex2f(standard_w * 48 - 4, standard_h * 6);
	glVertex2f(standard_w * 80 + 4, standard_h * 6);
	glVertex2f(standard_w * 80 + 4, standard_h * 74);
	glVertex2f(standard_w * 48 - 4, standard_h * 74);
	glEnd();

	if (!glIsEnabled(GL_LIGHTING))
	{
		if (isLighting)
			glEnable(GL_LIGHTING);
	}

	//　2D　→　3D (3Dに描画を戻す)
	glPopMatrix();
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

//----------------------------------------------------
// テキストを表示
//----------------------------------------------------
void glutRenderText(void* bitmapfont, const char*text)
{
	for (int i = 0; i<(int)strlen(text); i++)
		glutBitmapCharacter(bitmapfont, (int)text[i]);
}

//----------------------------------------------------
// ゲーム画面
//----------------------------------------------------
void GameDisplay(void) {
	//スタート時刻
	if (time_flag == 0) {
		start_time = timeGetTime();
		time_flag = 1;
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //バッファの消去

	//透視変換行列の設定------------------------------
	glMatrixMode(GL_PROJECTION); //行列モードの設定（GL_PROJECTION : 透視変換行列の設定、GL_MODELVIEW：モデルビュー変換行列）
	glLoadIdentity();            //行列の初期化
	gluPerspective(30.0, (double)WindowWidth / (double)WindowHeight, 0.1, 1500.0); //透視投影法の視体積gluPerspactive(th, w/h, near, far);

	//モデルビュー変換行列の設定--------------------------
	glMatrixMode(GL_MODELVIEW); //行列モードの設定（GL_PROJECTION : 透視変換行列の設定、GL_MODELVIEW：モデルビュー変換行列）
	glLoadIdentity();           //行列の初期化
	glViewport(0, 0, WindowWidth, WindowHeight);

#ifdef USE_DIMENCO_OPENGL_INTERFACE
	dimencoSetBackgroundState();
#endif

	//陰影ON------------------------------------------
	glEnable(GL_LIGHTING); 
	glEnable(GL_LIGHT1);   //光源1を利用
	
	//視点の設定------------------------------
	gluLookAt(
	ViewPointX, ViewPointY, ViewPointZ, // 視点の位置x,y,z;
	//enepos_x2, enepos_y2, enepos_z2,    // 視界の調整(追加 6/12)
	lookat_x, lookat_y, lookat_z,
	0.0, 0.0, 1.0);                     // 視界の上方向のベクトルx,y,z

	//カメラの位置------------------------------------
	camerawork();

	//移動経路----------------------------------------
	route();
	
	//背景の表示--------------------------------------
	skydome();

	//大地の描画--------------------------------------
	Ground();

	//弾の表示----------------------------------------
	bullet();
	bullet2();

	//敵の攻撃----------------------------------------
	if (shoot == 10 ) {

		//cout << enekey << endl;
		//cout << fps->Frame << endl;
		if (curve_decision(enekey)) { //曲線の場合
			//cout << enekey << endl;
			bullet_curve2();
			//bullet_stay();
		}
		if(enekey == 971 || enekey == 917 || enekey == 1035 || enekey ==1053) {                        //直線の場合
			bullet_straight2();
		}
		shoot = 0;

	}
	shoot++;

	//teapot(自機)------------------------------------
	obj(pos_x3, pos_y3, pos_z3);

	//teapot2(敵機)-----------------------------------
	obj_ene(enepos_x3, enepos_y3, enepos_z3);

	//陰影OFF-----------------------------------------
	glDisable(GL_LIGHTING);

	//2Dの描画(HPゲージ)------------------------------
	Draw_2D();

	//カウントダウン----------------------------------
	if (HP_ene > 0 && current_time < 3500) {
		end_time = timeGetTime();
		current_time = end_time - start_time;
	}

	if (current_time < 3500) {
		cowntdown_tmp = ((current_time % 60000) / 1000); //秒

		if (cowntdown_tmp <= 2.0) {
			countdown2 = 0.8 - 0.008*((current_time % 1000) / 10);
			countdown.SetColor(0.0f, 0.0f, 0.0f, countdown2);
			countdown.SetSpeed(1.0f);
			countdown.SetPulse(false);
			if (cowntdown_tmp <= 0) {
				countdown.SetType(1);
			}
			else if (cowntdown_tmp <= 1.0) {
				countdown.SetType(2);
			}
			else {
				countdown.SetType(3);
			}

			countdown.Render(0.0f);

		}
	}

	//ゲームクリア------------------------------------
	if (HP_ene <= 0) {
		gameclear.SetColor(0.0f, 0.0f, 0.0f, 0.4f);
		gameclear.SetSpeed(1.0f);
		gameclear.SetPulse(false);
		gameclear.SetType(4);

		gameclear.Render(0.0f);

		enekey = 0;
		key = 0;
	}

	//ゲームオーバー------------------------------------
	if (HP <= 0) {
		gameover.SetColor(0.0f, 0.0f, 0.0f, 0.4f);
		gameover.SetSpeed(1.0f);
		gameover.SetPulse(false);
		gameover.SetType(5);

		gameover.Render(0.0f);

		enekey = 0;
		key = 0;
	}

#ifdef USE_DIMENCO_OPENGL_INTERFACE
	dimencoSetZBufState();
#endif

	glutSwapBuffers(); //glutInitDisplayMode(GLUT_DOUBLE)でダブルバッファリングを利用可
}

//----------------------------------------------------
// メニュー画面
//----------------------------------------------------
void MenuDisplay(void) {
	kakudo += 0.01;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //バッファの消去


														//透視変換行列の設定------------------------------
	glMatrixMode(GL_PROJECTION); //行列モードの設定（GL_PROJECTION : 透視変換行列の設定、GL_MODELVIEW：モデルビュー変換行列）
	glLoadIdentity();            //行列の初期化
	gluPerspective(30.0, (double)WindowWidth / (double)WindowHeight, 0.1, 1500.0); //透視投影法の視体積gluPerspactive(th, w/h, near, far);

																				   //モデルビュー変換行列の設定--------------------------
	glMatrixMode(GL_MODELVIEW); //行列モードの設定（GL_PROJECTION : 透視変換行列の設定、GL_MODELVIEW：モデルビュー変換行列）
	glLoadIdentity();           //行列の初期化
	glViewport(0, 0, WindowWidth, WindowHeight);

#ifdef USE_DIMENCO_OPENGL_INTERFACE
	dimencoSetBackgroundState();
#endif

	//陰影ON------------------------------------------
	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0); //光源0を利用
	//glEnable(GL_LIGHT1); //光源1を利用
	GLfloat red[] = { 1.0, 0.4, 0.3, 1.0 };
	GLfloat blue[] = { 0.2, 0.4, 1.0, 1.0 };
	//glLightfv(GL_LIGHT1, GL_DIFFUSE, red);

	//視点の設定------------------------------
	gluLookAt(
		pos_x3 + 8 * sin(kakudo), pos_y3 - 10 * cos(kakudo), pos_z3 + 8, // 視点の位置x,y,z;
		pos_x3, pos_y3, pos_z3,									  // 視界の調整(追加 6/12)
		0.0, 0.0, 1.0);                                           // 視界の上方向のベクトルx,y,z

																  //背景の表示--------------------------------------
	skydome();

	//teapot(自機)------------------------------------
	obj(pos_x3, pos_y3, pos_z3);

	//陰影OFF-----------------------------------------
	glDisable(GL_LIGHTING);
	DISPLAY_TEXT(60, 600, "High Maneuver", GLUT_BITMAP_TIMES_ROMAN_24);
	DISPLAY_TEXT(65 - 10 * a[0], 200, "PUSH : START", GLUT_BITMAP_TIMES_ROMAN_24);
	//DISPLAY_TEXT(65 - 10 * a[1], 180, "TUTORIAL", GLUT_BITMAP_TIMES_ROMAN_24);
	//DISPLAY_TEXT(65 - 10 * a[2], 160, "TUTORIAL", GLUT_BITMAP_TIMES_ROMAN_24);

#ifdef USE_DIMENCO_OPENGL_INTERFACE
	dimencoSetZBufState();
#endif

	glutSwapBuffers(); //glutInitDisplayMode(GLUT_DOUBLE)でダブルバッファリングを利用可
}

//----------------------------------------------------
// 2D出力設定(メニュー画面)
//----------------------------------------------------
int list;
void DISPLAY_TEXT(int x, int y, const char *string, void *font) {
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);

	glPushAttrib(GL_ENABLE_BIT);
	glMatrixMode(GL_PROJECTION);

	glPushMatrix();

	glLoadIdentity();
	gluOrtho2D(0, WindowWidth, 0, WindowHeight);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glColor3f(1, 1, 1);
	glCallList(list);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glPopAttrib();
	glMatrixMode(GL_MODELVIEW);
	list = glGenLists(1);
	glNewList(list, GL_COMPILE);

	DRAW_STRING(x, y, string, font);
	glEndList();

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}

//----------------------------------------------------
// 文字の出力(メニュー画面)
//----------------------------------------------------
void DRAW_STRING(int x, int y, const char *string, void *font) {
	int len, i;
	glRasterPos2f(x, y);
	len = (int)strlen(string);
	for (i = 0; i < len; i++) {
		glutBitmapCharacter(font, string[i]);
	}
}

//----------------------------------------------------
// Reshape サイズ変更
//----------------------------------------------------
void Reshape(int x, int y)
{
	WindowWidth = x;
	WindowHeight = y;
	if (WindowWidth < 1) WindowWidth = 1;
	if (WindowHeight < 1) WindowHeight = 1;

	glViewport(0, 0, WindowWidth, WindowHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50.0, (double)WindowWidth / (double)WindowHeight, 0.1, 1000.0);
}

//----------------------------------------------------
// 経路の直線、曲線判定
//----------------------------------------------------
bool curve_decision(int key) {
	if (key == 112 || key == 223 || key == 334 || key == 441 || key == 874 || key == 545 || key == 656 || key == 767) {
		return true;
	}
	if (key == 121 || key == 414 || key == 343 || key == 232 || key == 847 || key == 776 || key == 665 || key == 554) {
		return true;
	}

	if (key == 971 || key == 1053) {
		return false;
	}
	if (key == 917 || key == 1035) {
		return false;
	}
	if (key == 0) {
		return false;
	}
}

//----------------------------------------------------
// 時間表示用のテクスチャを決定
//----------------------------------------------------
void time_texture(int time, int digit) {
	int tmp;
	if (digit == 1) {
		tmp = time / 10;
	}
	else {
		tmp = time % 10;
	}

	if (tmp <= 0) {
		glBindTexture(GL_TEXTURE_2D, number0.ID);
	}
	if (tmp == 1) {
		glBindTexture(GL_TEXTURE_2D, number1.ID);
	}
	if (tmp == 2) {
		glBindTexture(GL_TEXTURE_2D, number2.ID);
	}
	if (tmp == 3) {
		glBindTexture(GL_TEXTURE_2D, number3.ID);
	}
	if (tmp == 4) {
		glBindTexture(GL_TEXTURE_2D, number4.ID);
	}
	if (tmp == 5) {
		glBindTexture(GL_TEXTURE_2D, number5.ID);
	}
	if (tmp == 6) {
		glBindTexture(GL_TEXTURE_2D, number6.ID);
	}
	if (tmp == 7) {
		glBindTexture(GL_TEXTURE_2D, number7.ID);
	}
	if (tmp == 8) {
		glBindTexture(GL_TEXTURE_2D, number8.ID);
	}
	if (tmp == 9) {
		glBindTexture(GL_TEXTURE_2D, number9.ID);
	}
}

//----------------------------------------------------
// 画像ファイルの読み込み
//----------------------------------------------------
void LoadImage(const wchar_t* filename, GLuint &texture)
{
	glEnable(GL_TEXTURE_2D);
	Bitmap bmp(filename);
	BitmapData data;
	bmp.LockBits(0, ImageLockModeRead, PixelFormat32bppARGB, &data);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, data.Width, data.Height, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, data.Scan0);
	glBindTexture(GL_TEXTURE_2D, 0);
	bmp.UnlockBits(&data);
}

//----------------------------------------------------------
// OpenAL関数
//----------------------------------------------------------

void soundPlay(ALuint buf, ALuint *source, bool loop) {
	alGenSources(1, source);
	alSourcei(*source, AL_BUFFER, buf); // 音データのバッファを指定
	if (loop) alSourcei(*source, AL_LOOPING, AL_TRUE); // ループの設定
	alSourcePlay(*source);
	//cout << "soundPlay" << buf << "," << source << endl;
}

void soundStop(ALuint *source) {
	//cout << "soundStop" << source << endl;
	alSourceStop(*source);
	alDeleteSources(1, source);
	source = 0;
}

void soundInit() {
	bgm1 = alutCreateBufferFromFile("music/Cry_Of_Night.wav");
	bgm2 = alutCreateBufferFromFile("music/Top_Speed.wav");
	bgm3 = alutCreateBufferFromFile("music/Alcool_et_amour.wav");
	seGun = alutCreateBufferFromFile("music/shot1.wav");
	se03 = alutCreateBufferFromFile("music/button1.wav");
	seReload = alutCreateBufferFromFile("music/bomb1.wav");
}

//----------------------------------------------------
//スコア用テクスチャの決定
//----------------------------------------------------
void score_texture(int s) {
	int tmp = s;

	if (tmp <= 0) {
		glBindTexture(GL_TEXTURE_2D, number0.ID);
	}
	if (tmp == 1) {
		glBindTexture(GL_TEXTURE_2D, number1.ID);
	}
	if (tmp == 2) {
		glBindTexture(GL_TEXTURE_2D, number2.ID);
	}
	if (tmp == 3) {
		glBindTexture(GL_TEXTURE_2D, number3.ID);
	}
	if (tmp == 4) {
		glBindTexture(GL_TEXTURE_2D, number4.ID);
	}
	if (tmp == 5) {
		glBindTexture(GL_TEXTURE_2D, number5.ID);
	}
	if (tmp == 6) {
		glBindTexture(GL_TEXTURE_2D, number6.ID);
	}
	if (tmp == 7) {
		glBindTexture(GL_TEXTURE_2D, number7.ID);
	}
	if (tmp == 8) {
		glBindTexture(GL_TEXTURE_2D, number8.ID);
	}
	if (tmp == 9) {
		glBindTexture(GL_TEXTURE_2D, number9.ID);
	}
}

//-------------------------------------------------------
//数字Texture貼り付け君一号
//引数 (数字,左上x座標,左上y座標,テクスチャ縦,テクスチャ横)
//使用するテクスチャはscore__texture(int s)で決定
//-------------------------------------------------------
void DrawNumTex(int s, double x, double y, double w, double h) {
	standard_w = WindowWidth  / 1280.0;   // standard_w = 10
	standard_h = WindowHeight /  720.0;  // standard_h =  1
	//cout << standard_w << "   " << standard_h << endl;

	glEnable(GL_TEXTURE_2D);
	score_texture(s);
	glColor4f(0.0, 0.0, 0.0, 1.0);
	glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, white);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 1.0);  glVertex2f(x       * standard_w, y       * standard_h);
	glTexCoord2f(1.0, 1.0);  glVertex2f((x + w) * standard_w, y       * standard_h);
	glTexCoord2f(1.0, 0.0);  glVertex2f((x + w) * standard_w, (y + h) * standard_h);
	glTexCoord2f(0.0, 0.0);  glVertex2f(x       * standard_w, (y + h) * standard_h);

	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}

void ResultDisplay() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //バッファの消去
#ifdef USE_DIMENCO_OPENGL_INTERFACE
	dimencoSetBackgroundState();
#endif
	//透視変換行列の設定------------------------------
	glMatrixMode(GL_PROJECTION); //行列モードの設定（GL_PROJECTION : 透視変換行列の設定、GL_MODELVIEW：モデルビュー変換行列）
	glLoadIdentity();            //行列の初期化
	gluPerspective(30.0, (double)WindowWidth / (double)WindowHeight, 25, 1500.0); //透視投影法の視体積gluPerspactive(th, w/h, near, far);

																				  //モデルビュー変換行列の設定--------------------------
	glMatrixMode(GL_MODELVIEW); //行列モードの設定（GL_PROJECTION : 透視変換行列の設定、GL_MODELVIEW：モデルビュー変換行列）
	glLoadIdentity();           //行列の初期化
	glViewport(0, 0, WindowWidth, WindowHeight);
	//陰影ON------------------------------------------
	/*
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0); //光源0を利用
	glEnable(GL_LIGHT1); //光源1を利用
	GLfloat red[] = { 1.0, 0.4, 0.3, 1.0 };
	GLfloat blue[] = { 0.2, 0.4, 1.0, 1.0 };
	glLightfv(GL_LIGHT1, GL_DIFFUSE, red);
	*/


	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0); //光源0を利用
	//glEnable(GL_LIGHT1); //光源1を利用
	//glEnable(GL_LIGHT2); //光源2を利用
	//glEnable(GL_LIGHT3); //光源3を利用
	//glEnable(GL_LIGHT4); //光源4を利用

	//GLfloat red[] = { 1.0, 0.4, 0.3, 1.0 };
	//GLfloat blue[] = { 0.2, 0.4, 1.0, 1.0 };
	//glLightfv(GL_LIGHT1, GL_DIFFUSE, yellow);
	//glLightfv(GL_LIGHT2, GL_DIFFUSE, green);
	//glLightfv(GL_LIGHT3, GL_DIFFUSE, blue);
	//glLightfv(GL_LIGHT4, GL_DIFFUSE, blue);


	//視点の設定------------------------------
	gluLookAt(
		0, 0, 0,         // 視点の位置x,y,z;
		0, -10, 0,       // 視界の調整(追加 6/12)
		0.0, 0.0, 1.0);  // 視界の上方向のベクトルx,y,z

	//陰影OFF-----------------------------------------
	glDisable(GL_LIGHTING);
	bool isLighting = false;

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, WindowWidth, WindowHeight, 0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	if (glIsEnabled(GL_LIGHTING))
	{
		isLighting = true;
		glDisable(GL_LIGHTING);
	}

	//my score=================================================
	//s3 = score[3];
	//cout << s3 << endl;
	/*glEnable(GL_TEXTURE_2D);
	score_texture(s);
	glColor4f(0.0, 0.0, 0.0, 1.0);
	glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, white);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 1.0);  glVertex2f(100*position, 0);
	glTexCoord2f(1.0, 1.0);  glVertex2f(100+100*position, 0);
	glTexCoord2f(1.0, 0.0);  glVertex2f(100+100*position, 100);
	glTexCoord2f(0.0, 0.0);  glVertex2f(100*position, 100);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
	s = s / 10;*/

	int d5 = 100;
	int d0 = 50;

	int h = 60;

	int x = 900;
	int y = 350;

	score[0] = s0;
	score[1] = s1;
	score[2] = s2;

	score[3] = calculateScore();
	ranking();

	s0 = score[0];
	s1 = score[1];
	s2 = score[2];

	/*
	DrawNumTex(s3 % 10         , WindowWidth / 2 - w - d5    , WindowHeight / 2 - h, d5, d5);
	DrawNumTex(s3 / 10 % 10    , WindowWidth / 2 - w - d5 * 2, WindowHeight / 2 - h, d5, d5);
	DrawNumTex(s3 / 100 % 10   , WindowWidth / 2 - w - d5 * 3, WindowHeight / 2 - h, d5, d5);
	DrawNumTex(s3 / 1000 % 10  , WindowWidth / 2 - w - d5 * 4, WindowHeight / 2 - h, d5, d5);
	DrawNumTex(s3 / 10000 % 10 , WindowWidth / 2 - w - d5 * 5, WindowHeight / 2 - h, d5, d5);
	DrawNumTex(s3 / 100000 % 10, WindowWidth / 2 - w - d5 * 6, WindowHeight / 2 - h, d5, d5);
	*/
	
	DrawNumTex(score[3] % 10         , x - d5 * 0, y, d5, d5);
	DrawNumTex(score[3] / 10 % 10    , x - d5 * 1, y, d5, d5);
	DrawNumTex(score[3] / 100 % 10   , x - d5 * 2, y, d5, d5);
	DrawNumTex(score[3] / 1000 % 10  , x - d5 * 3, y, d5, d5);
	DrawNumTex(score[3] / 10000 % 10 , x - d5 * 4, y, d5, d5);
	DrawNumTex(score[3] / 100000 % 10, x - d5 * 5, y, d5, d5);

	//=========================================================

	x = 650;
	y = 460;

	//1st score================================================
	/*
	DrawNumTex(s0 % 10         , WindowWidth / 2 - w - d0    , WindowHeight / 2 - h, d0, d0);
	DrawNumTex(s0 / 10 % 10    , WindowWidth / 2 - w - d0 * 2, WindowHeight / 2 - h, d0, d0);
	DrawNumTex(s0 / 100 % 10   , WindowWidth / 2 - w - d0 * 3, WindowHeight / 2 - h, d0, d0);
	DrawNumTex(s0 / 1000 % 10  , WindowWidth / 2 - w - d0 * 4, WindowHeight / 2 - h, d0, d0);
	DrawNumTex(s0 / 10000 % 10 , WindowWidth / 2 - w - d0 * 5, WindowHeight / 2 - h, d0, d0);
	DrawNumTex(s0 / 100000 % 10, WindowWidth / 2 - w - d0 * 6, WindowHeight / 2 - h, d0, d0);
	*/

	DrawNumTex(score[0] % 10         , x - d0 * 0, y, d0, d0);
	DrawNumTex(score[0] / 10 % 10    , x - d0 * 1, y, d0, d0);
	DrawNumTex(score[0] / 100 % 10   , x - d0 * 2, y, d0, d0);
	DrawNumTex(score[0] / 1000 % 10  , x - d0 * 3, y, d0, d0);
	DrawNumTex(score[0] / 10000 % 10 , x - d0 * 4, y, d0, d0);
	DrawNumTex(score[0] / 100000 % 10, x - d0 * 5, y, d0, d0);
	//=========================================================


	//2nd score================================================
	/*
	DrawNumTex(s1 % 10         , WindowWidth / 2 - w - d0    , WindowHeight / 2 - h, d0, d0);
	DrawNumTex(s1 / 10 % 10    , WindowWidth / 2 - w - d0 * 2, WindowHeight / 2 - h, d0, d0);
	DrawNumTex(s1 / 100 % 10   , WindowWidth / 2 - w - d0 * 3, WindowHeight / 2 - h, d0, d0);
	DrawNumTex(s1 / 1000 % 10  , WindowWidth / 2 - w - d0 * 4, WindowHeight / 2 - h, d0, d0);
	DrawNumTex(s1 / 10000 % 10 , WindowWidth / 2 - w - d0 * 5, WindowHeight / 2 - h, d0, d0);
	DrawNumTex(s1 / 100000 % 10, WindowWidth / 2 - w - d0 * 6, WindowHeight / 2 - h, d0, d0);
	*/

	DrawNumTex(score[1] % 10         , x - d0 * 0, y + h * 1, d0, d0);
	DrawNumTex(score[1] / 10 % 10    , x - d0 * 1, y + h * 1, d0, d0);
	DrawNumTex(score[1] / 100 % 10   , x - d0 * 2, y + h * 1, d0, d0);
	DrawNumTex(score[1] / 1000 % 10  , x - d0 * 3, y + h * 1, d0, d0);
	DrawNumTex(score[1] / 10000 % 10 , x - d0 * 4, y + h * 1, d0, d0);
	DrawNumTex(score[1] / 100000 % 10, x - d0 * 5, y + h * 1, d0, d0);
	//=========================================================


	//3rd score================================================
	/*
	DrawNumTex(s2 % 10         , WindowWidth / 2 - w - d0    , WindowHeight / 2 - h, d0, d0);
	DrawNumTex(s2 / 10 % 10    , WindowWidth / 2 - w - d0 * 2, WindowHeight / 2 - h, d0, d0);
	DrawNumTex(s2 / 100 % 10   , WindowWidth / 2 - w - d0 * 3, WindowHeight / 2 - h, d0, d0);
	DrawNumTex(s2 / 1000 % 10  , WindowWidth / 2 - w - d0 * 4, WindowHeight / 2 - h, d0, d0);
	DrawNumTex(s2 / 10000 % 10 , WindowWidth / 2 - w - d0 * 5, WindowHeight / 2 - h, d0, d0);
	DrawNumTex(s2 / 100000 % 10, WindowWidth / 2 - w - d0 * 6, WindowHeight / 2 - h, d0, d0);
	*/

	DrawNumTex(score[2] % 10         , x - d0 * 0, y + h * 2, d0, d0);
	DrawNumTex(score[2] / 10 % 10    , x - d0 * 1, y + h * 2, d0, d0);
	DrawNumTex(score[2] / 100 % 10   , x - d0 * 2, y + h * 2, d0, d0);
	DrawNumTex(score[2] / 1000 % 10  , x - d0 * 3, y + h * 2, d0, d0);
	DrawNumTex(score[2] / 10000 % 10 , x - d0 * 4, y + h * 2, d0, d0);
	DrawNumTex(score[2] / 100000 % 10, x - d0 * 5, y + h * 2, d0, d0);
	//=========================================================
	

	//背景=====================================================
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, result.ID);
	glColor4f(0.0, 0.0, 0.0, 1.0);
	glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, white);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 1.0);  glVertex2f(0, 0);
	glTexCoord2f(1.0, 1.0);  glVertex2f(WindowWidth, 0);
	glTexCoord2f(1.0, 0.0);  glVertex2f(WindowWidth, WindowHeight);
	glTexCoord2f(0.0, 0.0);  glVertex2f(0, WindowHeight);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
	//==========================================================

	if (!glIsEnabled(GL_LIGHTING))
	{
		if (isLighting)
			glEnable(GL_LIGHTING);
	}

	//　2D　→　3D (3Dに描画を戻す)
	glPopMatrix();
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
#ifdef USE_DIMENCO_OPENGL_INTERFACE
	dimencoSetZBufState();
#endif
	glutSwapBuffers(); //glutInitDisplayMode(GLUT_DOUBLE)でダブルバッファリングを利用可

}

void ranking() {

	if (ranking_count == 0) {
		if (score[3] > score[2]) {
			score[2] = score[3];
		}

		if (score[2] > score[1]) {
			tmp_score = score[1];
			score[1] = score[2];
			score[2] = tmp_score;
		}

		if (score[1] > score[0]) {
			tmp_score = score[0];
			score[0] = score[1];
			score[1] = tmp_score;
		}

		ranking_count = 1;
	}
}

int calculateScore() {
	//cout << HP << "   " << HP_ene << endl;
	if (HP > 0) {
		return (180000 - current_time) + (HP * 1000);
	}
	else {
		return (100 - HP_ene) * 1000;
	}
}

void GameInit() {
	hit = 0;
	hit2 = 0;

	gamestart = 0;
	gamestart2 = 0;

	pos_x = 0.0, pos_y = 0.0, pos_z = 0.0;
	pos_x2 = -300.0, pos_y2 = -200.0, pos_z2 = 0.0;
	pos_x3 = -300.0, pos_y3 = -200.0, pos_z3 = 0.0;

	enepos_x = 0.0, enepos_y = 0.0, enepos_z = 0.0;
	enepos_x2 = -300.0, enepos_y2 = 0.0, enepos_z2 = 0.0;
	enepos_x3 = -300.0, enepos_y3 = 0.0, enepos_z3 = 0.0;

	pos_r = 0.0;
	enepos_r = 0.0;
	turn = 0;

	enekey = 0;
	key = 0;
	
	angle = -180.0;  //敵機の角度
	angle2 = -180.0; //自機の角度

	lookat_x = -300;
	lookat_y = -105;
	lookat_z = 0;

	HP_ene = 100.0;
	HP = 100.0;

	time_flag = 0;

	for (j = 0; j < 20; j++) {
		weight[20] = 0;
	}
	
	set = 50;
	shoot = 0;

	kakudo = 0;
	kaasoru = 0;
	
	k = 0;

	position = 0;
	ranking_count = 0;

	//------------------------------------------------------------ ディスプレイ設定

	WindowPositionX = 100;
	WindowPositionY = 100;

	ViewPointX = -300.0;
	ViewPointY = -160.0;
	ViewPointZ = 10.0;

	//------------------------------------------------------------ 弾速度変数(自機)

	pn = 0;
	pn2 = 0;

	tn = 0;
	t = 0;
	dt = 0.1;

	ax = 0.0, ay = 0.0, az = 0.0;
	vx = 40.0, vy = 40.0, vz = 20.0;
	hanpatu = 0.9;

	//------------------------------------------------------------ 弾速度変数(敵機)2

	epn = 0;
	epn2 = 0;

	etn = 0;
	et = 0;
	edt = 0.1;

	eax = 0.0, eay = 0.0, eaz = 0.0;
	evx = 40.0, evy = 40.0, evz = 20.0;

	bom_m = 0;

}