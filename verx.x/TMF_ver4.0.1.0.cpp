// TMF_glut.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"
#include "lib.h"
#include "MusicController.h"


#define USE_DIMENCO_OPENGL_INTERFACE

#ifdef USE_DIMENCO_OPENGL_INTERFACE
#include "dimenco.h"
#endif

#pragma endregion

#pragma region Defines
//
// Defines
//

// RtlZeroMemory Macro
#ifndef RtlZeroMemory
#define RtlZeroMemory(Destination,Length) memset((Destination),0,(Length))
#endif

// ZeroMemory Macro
#ifndef ZeroMemory
#define ZeroMemory RtlZeroMemory
#endif

// SAFE_DELETE_ARRAY
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(x) { if (x) { delete [] (x); (x) = NULL; } }
#endif
#pragma endregion

using namespace std;


//　disable Warning C4996
#pragma warning(disable : 4996)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------------------------------------
// メイン関数
//--------------------------------------------------------------------------------------------------------
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
	//glutFullScreen();

	InitMusic();
	glutDisplayFunc(Display);			//描画時に呼び出される関数を指定する（関数名：Display）
	//glutReshapeFunc(Reshape);			//ごめんわからない
	glutIdleFunc(Idle);					//プログラムアイドル状態時に呼び出される関数
	glutKeyboardFunc(Keyboard);			//キーボード入力時に呼び出される関数を指定する（関数名：Keyboard）
	glutSpecialFunc(special_key);		//特殊キーの設定
	glutKeyboardUpFunc(KeyboadUP);		//キーボードから手を離したときに発動
	glutSpecialUpFunc(special_keyUP);	//特殊キーから手を離したときに発動

	initState();
	Initialize();						//初期設定の関数を呼び出す
	glutMainLoop();						//ループ関数. モード選択によってゲームへ面を遷移していく
	return 0;
}

//--------------------------------------------------------------------------------------------------------
// ゲームの初期化
//--------------------------------------------------------------------------------------------------------
void Initialize() {

#ifdef USE_DIMENCO_OPENGL_INTERFACE
	dimencoInitState();
#endif

	//　GLEWの初期化
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Error : GLEWの初期化に失敗しました\n";
		exit(0);
	}

	InitObjFlag();
	InitEndFlag();
	//------------------------------------------------
	// moveOBL()
	//------------------------------------------------
	initialFoV = 45.0f;
	mouseSpeed = 0.05f;
	FoV = 45.0f;				//なんか難しいやつ
	hspeed = 1.0f;
	disOfNoticePig = 5.5;		//主人公と豚との距離がdisOfNoticePig以下になるとブタが近づきながら鳴き声を上げる
	skycheck = true;

	timeflag = true;

	selectEnviroment = MYPC;
	//selectEnviroment = DIMENCO;



	if (selectEnviroment == MYPC) {
		Bigger = 1;			//自分の速さ   
		bigger = 1;			//カメラの速さ
		Bigger_pig = 1;		//ブタのはやさ
		//speed = 15.0f*Bigger;					//主人公の座標を動かす速さ
		//changePos = 40*bigger;					//カメラの向きを動かす速さ

		speed = 0.75f*Bigger;               //主人公の座標を動かす速さ
		changePos = 2 * bigger;				 //カメラの向きを動かす速さ
	}
	else if (selectEnviroment == DIMENCO) {
		Bigger = 7.5;			//自分の速さ   
		bigger = 20;			//カメラの速さ
		Bigger_pig = 10.0;		//ブタのはやさ
		//speed = 15.0f*Bigger;					//主人公の座標を動かす速さ
		//changePos = 40*bigger;					//カメラの向きを動かす速さ

		speed = 0.75f*Bigger;               //主人公の座標を動かす速さ
		changePos = 2 * bigger;				 //カメラの向きを動かす速さ
	}

	if (selectEnviroment == MYPC) {
		//--------------------------------------------
		// 手元のPC環境
		//--------------------------------------------
		WindowPositionX = 0;         //生成するウィンドウ位置のX座標
		WindowPositionY = 0;         //生成するウィンドウ位置のY座標
		WindowWidth = 1960;          //生成するウィンドウの幅
		WindowHeight = 1080;         //生成するウィンドウの高さ
		//speed = 0.75f;               //主人公の座標を動かす速さ
		//changePos = 2;				 //カメラの向きを動かす速さ
	}
	else if (selectEnviroment == DIMENCO) {
		//--------------------------------------------
		// Dimenco Display用環境
		//--------------------------------------------
		//ver1
		//int WindowWidth = 4320;
		//int WindowHeight = 11520;

		//ver2
		//下は普通だが上がやけに広い
		//int WindowWidth = 8640;
		//int WindowHeight = 3240;

		//ver3
		//左上2つが全く映らなくなった(heightを1/3=2/6減らした)
		//int WindowWidth = 8640;
		//int WindowHeight = 2160;

		//ver4
		//heightを3240から2700へ(1/6減らした)
		//int WindowWidth = 8640;
		//int WindowHeight = 2700;

		//ver5
		//widthを1/6減らした
		//int WindowWidth = 7200;
		//int WindowHeight = 3240;

		//ver5
		//前回の結果, 6面ディスプレイの左1つが1/6映らなくなった。その後紙面上で計算して出た値をwに代入した
		//現状これが一番良い。本番環境用
		WindowWidth = 8214;
		WindowHeight = 3240;

		//speed = 15.0f;					//主人公の座標を動かす速さ
		//changePos = 40;					//カメラの向きを動かす速さ
		//changePos = 50;				//カメラの向きを動かす速さ
	}

	//-----------------------------------------------------------------------
	// 描画に使うobjの初期化
	//-----------------------------------------------------------------------
	OBJFile[0].LoadFile("obj/Newwall2.obj");
	//OBJFile[0].LoadFile("obj/MapSmallEx.obj");
	//OBJFile[0].LoadFile("obj/Map01uv.obj");
	//OBJFile[0].LoadFile("obj/MapSmallComplete.obj");
	OBJFile[1].LoadFile("obj/Map01Bottomuv.obj");
	OBJFile[2].LoadFile("obj/time.obj");
	OBJFile[3].LoadFile("obj/start.obj");
	OBJFile[4].LoadFile("obj/title.obj");
	OBJFile[5].LoadFile("obj/enter.obj");
	OBJFile[6].LoadFile("obj/start.obj");
	//OBJFile[7].LoadFile("obj/hand1.obj");
	//OBJFile[7].LoadFile("obj/octo.obj");
	//OBJFile[7].LoadFile("obj/lowpig.obj");
	OBJFile[7].LoadFile("obj/lowpigO.obj");
	OBJFile[8].LoadFile("obj/lowapple.obj");
	OBJFile[9].LoadFile("obj/MapSmallCube.obj");
	//OBJFile[9].LoadFile("obj/skywall.obj");
	OBJFile[10].LoadFile("obj/applealert.obj");
	//OBJFile[11].LoadFile("obj/skywall.obj");

	//-----------------------------------------------------------------------
	// 描画に使うbmpの初期化
	//-----------------------------------------------------------------------
	//Texture[0].Load("bmp/stonewall.bmp");
	Texture[0].Load("bmp/thewall.bmp");
	//Texture[1].Load("bmp/mapbottom.bmp");
	Texture[1].Load("bmp/map4.bmp");
	Texture[2].Load("bmp/sampletex.bmp");
	Texture[3].Load("bmp/start.bmp");
	Texture[4].Load("bmp/black.bmp");
	Texture[5].Load("bmp/enter2.BMP");
	Texture[6].Load("bmp/result.bmp");
	//Texture[7].Load("bmp/subdermal_hand.bmp");
	//Texture[7].Load("bmp/OctoBody.bmp");
	//Texture[7].Load("bmp/pigfbx1.bmp");
	Texture[7].Load("bmp/lowpig.bmp");
	Texture[8].Load("bmp/lowapple.bmp");
	Texture[9].Load("bmp/sky.bmp");

	Texture[10].Load("bmp/apple/apple1.bmp");
	//Texture[10].Load("bmp/!6.bmp");
	//Texture[10].Load("bmp/apple/444444.bmp");
	Texture[11].Load("bmp/apple/apple_2ok.bmp");
	//Texture[11].Load("bmp/apple/apple_2.bmp");
	Texture[12].Load("bmp/apple/apple3.bmp");
	//Texture[13].Load("bmp/apple/444444.bmp");
	Texture[13].Load("bmp/apple/apple4.bmp");

	Texture[14].Load("bmp/alert/alert_down.bmp");
	Texture[15].Load("bmp/alert/alert_up.bmp");
	Texture[16].Load("bmp/alert/alert_left.bmp");
	Texture[17].Load("bmp/alert/alert_right.bmp");

	//Texture[18].Load("bmp/time/1m.bmp");
	Texture[18].Load("bmp/time/1_00.bmp");

	Texture[19].Load("bmp/gameresult/result0.bmp");
	Texture[20].Load("bmp/gameresult/result1.bmp");
	Texture[21].Load("bmp/gameresult/result2.bmp");
	Texture[22].Load("bmp/gameresult/result3.bmp");
	Texture[23].Load("bmp/gameresult/result4.bmp");

	//Texture[19].Load("bmp/time/1_30.bmp");
	//Texture[20].Load("bmp/time/3_00.bmp");
	//Texture[20].Load("bmp/time/2_00.bmp");
	//Texture[21].Load("bmp/time/2_30.bmp");
	//Texture[22].Load("bmp/time/3_00.bmp");
	//Texture[24].Load("bmp/time/2_00.bmp");
	Texture[24].Load("bmp/time/10sec.bmp");
	Texture[25].Load("bmp/time/3_00.bmp");

	//Texture[24].Load("bmp/time/3sec.bmp");
	//Texture[25].Load("bmp/time/3sec.bmp");
	//Texture[25].Load("bmp/time/!.bmp");

	Texture[26].Load("bmp/time/2_00.bmp");
	//Texture[26].Load("bmp/!5.bmp");

	//ここまではいける


	Texture[27].Load("bmp/!6.bmp");
	//Texture[27].Load("bmp/apple/apple1.bmp");

	//Texture[28].Load("bmp/gameover_full.bmp");
	//Texture[29].Load("bmp/gameclear_full.bmp");
	//Texture[30].Load("bmp/timeup.bmp");
	if (selectEnviroment == MYPC) {
		Texture[28].Load("bmp/gameover2.bmp");
		Texture[29].Load("bmp/gameclear2.bmp");
		Texture[30].Load("bmp/timeup2.bmp");
	}
	else if (selectEnviroment == DIMENCO) {
		Texture[28].Load("bmp/gameover4.bmp");
		Texture[29].Load("bmp/gameclear4.bmp");
		Texture[30].Load("bmp/timeup4.bmp");
	}
	//Texture[28].Load("bmp/gameover.bmp");
	//Texture[29].Load("bmp/gameclear.bmp");
	//Texture[28].Load("bmp/time/2sec.bmp");
	//Texture[29].Load("bmp/time/1sec.bmp");
	//Texture[30].Load("bmp/time/8sec.bmp");
	//Texture[31].Load("bmp/time/9sec.bmp");
	//Texture[32].Load("bmp/time/10sec.bmp");
	//Texture[33].Load("bmp/time/finish.bmp");


	//Texture[34].Load("bmp/gameresult/result0.bmp");
	//Texture[35].Load("bmp/gameresult/result1.bmp");
	//Texture[36].Load("bmp/gameresult/result2.bmp");
	//Texture[37].Load("bmp/gameresult/result3.bmp");
	//Texture[38].Load("bmp/gameresult/result4.bmp");


	//-----------------------------------------------------------------------
	// 計算に使うobjの初期化. 一応全部定義してるけどmapに関するものしか使わない
	//-----------------------------------------------------------------------
	calc_OBJFile[0] = "obj/Newwall.obj";
	//calc_OBJFile[0] = "obj/Map01uv.obj";
	calc_OBJFile[7] = "obj/lowpigO.obj";
	calc_OBJFile[8] = "obj/lowapple.obj";


	if (!loadOBJ(calc_OBJFile[0].c_str(), vertices[0], uvs[0], normals[0], ver[0])) {	// もしobjファイルを読み込めなかったら
		std::cout << "[ERROR] No File of " << calc_OBJFile[0] << '\n';					// エラー文を返す
	}

	if (!loadOBJ(calc_OBJFile[7].c_str(), vertices[7], uvs[7], normals[7], ver[7])) {	// もしobjファイルを読み込めなかったら
		std::cout << "[ERROR] No File of " << calc_OBJFile[7] << '\n';					// エラー文を返す
	}

	if (!loadOBJ(calc_OBJFile[8].c_str(), vertices[8], uvs[8], normals[8], ver[8])) {	// もしobjファイルを読み込めなかったら
		std::cout << "[ERROR] No File of " << calc_OBJFile[8] << '\n';					// エラー文を返す
	}


	//for (int i = 0; i < 9; i++) {
	//	if (i == 0 || i == 7 || i == 8) {
	//		std::cout << "i=" << i << std::endl;
	//		if (!loadOBJ(calc_OBJFile[i].c_str(), vertices[i], uvs[i], normals[i], ver[i])) {	// もしobjファイルを読み込めなかったら
	//			std::cout << "[ERROR] No File of " << calc_OBJFile[i] << '\n';					// エラー文を返す
	//		}
	//		//prosessingOfOBJ(&ver[i], &vertexbuffer[i], vertices[i], normals[i], uvs[i], calc_OBJFile[i], &uvbuffer[i], &VertexArrayID[i]);
	//	}
	//}


	//リンゴの存在フラグの初期化
	for (int i = 0; i < 4; i++) {
		appleFlag[i] = true;
	}

	//HARD用のブタの速度UPフラグの初期化
	for (int i = 0; i < 4; i++) {
		changePigSpeedFlag[i] = false;
	}

	//for (int i = 0; i < (int)vertices[0].size(); i++) {
	//	if (i <= 14 * 36) {
	//		if (i % 36 == 0) {
	//			std::cout << '\n';
	//		}
	//	}
	//	else {
	//		if (i >= 15 * 36 && i % 36 == 6) {
	//			std::cout << '\n';
	//		}
	//	}
	//	
	//	std::cout << i/36 << " " << vertices[0][i].x << ", " << vertices[0][i].y << ", " << vertices[0][i].z << '\n';
	//}
	//std::cout << "size = " << (int)vertices->size() << std::endl;

	decideCube(vertices[0]);			//当たり判定に用いる配列cube[][]に値を格納する

	//std::cout << "\n\n\nPosition of Pig\n";
	//for (int i = 0; i < (int)vertices[7].size(); i++) {
	//	std::cout << i << "(" << vertices[7][i].x <<  ", " << vertices[7][i].z << std::endl;
	//}

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//これいらんかも、初期化でいい
	gluLookAt(
		-5, 1, -12,								// ワールド空間でカメラは(4,3,3)にあります。
		//enepos_x2, enepos_y2, enepos_z2,		// 視界の調整(追加 6/12)
		1, 1, 1,
		0.0, 1.0, 0.0							// 視界の上方向のベクトルx,y,z
	);


	// 下でエスケープキーが押されるのを捉えるのを保証します。
	//glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	//position.x = -12, position.y = 0.5, position.z = -15.5;//カメラの位置
	position.x = 0, position.y = 0.5, position.z = 0;//カメラの位置
	direction.x = 0, direction.y = 0, direction.z = 0;//何でもいい
	horizontalAngle = 0.0f;//カメラの向き
	verticalAngle = 0.0f;
	movecount = 0;
	p = 0;
	flush_cnt = 0;		//START画面の点滅を制御する変数. インクリメントして200に到達したら0に戻す

#ifdef USE_DIMENCO_OPENGL_INTERFACE
	dimencoSetBackgroundState();
#endif

#ifdef USE_DIMENCO_OPENGL_INTERFACE
	dimencoSetZBufState();
#endif

	//time(&cuTime);
	//time(&laTime);
}

//--------------------------------------------------------------------------------------------------------
// escKeyが押されたら実行. すべてのバッファ等を削除したい
//--------------------------------------------------------------------------------------------------------
void GameAllDelete() {
	soundAllDelete();		//すべての音楽バッファを削除
	objAllDelete();			//すべてのオブジェクトバッファを削除
	textureAllDelete();		//すべてのテクスチャバッファを削除
}

//--------------------------------------------------------------------------------------------------------
// オブジェクトのすべてのバッファを削除したい
//--------------------------------------------------------------------------------------------------------
void objAllDelete() {
	for (int i = 0; i < 11; i++) {
		OBJFile[i].Release();
	}
}

//--------------------------------------------------------------------------------------------------------
// テクスチャのすべてのバッファを削除したい
//--------------------------------------------------------------------------------------------------------
void textureAllDelete() {
	for (int i = 0; i < N; i++) {
		Texture[i].~BMPImage();
	}
}

//--------------------------------------------------------------------------------------------------------
// dimencoに関連する初期化
//--------------------------------------------------------------------------------------------------------
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
	gluPerspective(30.0, (double)WindowWidth / (double)WindowHeight, 3.0f, 100.0);

	glPushMatrix();
}

//--------------------------------------------------------------------------------------------------------
// 自分の動作に影響を与える構造体(Objflag)の引数のフラグの初期化
//--------------------------------------------------------------------------------------------------------
void InitObjFlag() {
	objflag.xposInc = false;
	objflag.xposDec = false;
	objflag.yposInc = false;
	objflag.yposDec = false;
	objflag.FoVInc = false;
	objflag.FoVDec = false;
	objflag.keyUP = false;
	objflag.keyDOWN = false;
	objflag.keyRIGHT = false;
	objflag.keyLEFT = false;
	objflag.hitUP = false;
	objflag.hitDOWN = false;
	objflag.hitRIGHT = false;
	objflag.hitLEFT = false;
}

//--------------------------------------------------------------------------------------------------------
// ゲームオーバー・ゲームクリア・タイムアップのフラグ
//--------------------------------------------------------------------------------------------------------
void InitEndFlag() {
	endflag.gameoverFlag = false;
	endflag.gameclearFlag = false;
	endflag.timeupFlag = false;
}

//--------------------------------------------------------------------------------------------------------
// 描画の関数
//--------------------------------------------------------------------------------------------------------
void Display(void) {

	glClearColor(0.2f, 0.2f, 0.2f, 0.0f);					// おまじない
	glEnable(GL_DEPTH_TEST);								// デプステストを有効にする
	glDepthFunc(GL_LESS);									// 前のものよりもカメラに近ければ、フラグメントを受け入れる
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// スクリーンをクリアする

	glDisable(GL_LIGHTING);

	//透視変換行列の設定------------------------------
	glMatrixMode(GL_PROJECTION); //行列モードの設定（GL_PROJECTION : 透視変換行列の設定、GL_MODELVIEW：モデルビュー変換行列）
	glLoadIdentity();            //行列の初期化
	gluPerspective(30.0, (double)WindowWidth / (double)WindowHeight, 0.1, 1500.0); //透視投影法の視体積gluPerspactive(th, w/h, near, far);

	//モデルビュー変換行列の設定--------------------------
	glMatrixMode(GL_MODELVIEW); //行列モードの設定（GL_PROJECTION : 透視変換行列の設定、GL_MODELVIEW：モデルビュー変換行列）
	glLoadIdentity();           //行列の初期化
	glViewport(0, 0, WindowWidth, WindowHeight);

	//gluLookAt(15.8, 180, -7.4, 
	//		   15.8,   0, -7.4, 
	//	       0,   0, -1
	//);
	//std::cout << position.x << " " << position.z << std::endl;

#ifdef USE_DIMENCO_OPENGL_INTERFACE
	dimencoSetBackgroundState();
#endif
	view3D();
	//glLoadIdentity();

	applecount = 0;
	//残リンゴ数確認
	for (int i = 0; i < 4; i++) {
		if (appleFlag[i] == true) {
			applecount++;
		}
	}

	if (ModeSelect == 0) {
		if (sound_check == 0) {
			//std::cout << "GameReset\n";
			GameReset();
			allSoundStop();
		}

		StartMode();

		if (sound_check == 0) {
			soundPlay(bgmStart, &sbgmStart, true, volume_BGM);
			sound_check = 1;
		}
	}

	if (ModeSelect == 1) {
		if (sound_check == 1) {
			allSoundStop();
		}

		PlayMode();

		if (sound_check == 1) {
			soundPlay(bgmPlay, &sbgmPlay, true, volume_BGM);
			sound_check = 2;
		}
	}

	if (ModeSelect == 2) {
		if (sound_check == 2) {
			allSoundStop();
		}

		ResultMode();

		if (sound_check == 2) {
			soundPlay(bgmResult, &sbgmResult, true, volume_BGM);
			sound_check = 3;
		}
	}
	if (ModeSelect == 3) {
		if (sound_check == 3) {
			GameReset();
			//std::cout << "OK\n";
			//allSoundStop();
		}
		ViewMode();
		if (sound_check == 3) {
			sound_check = 0;
		}
	}

#ifdef USE_DIMENCO_OPENGL_INTERFACE
	dimencoSetZBufState();
#endif


	glutSwapBuffers(); //glutInitDisplayMode(GLUT_DOUBLE)でダブルバッファリングを利用可
}

//--------------------------------------------------------------------------------------------------------
// キーボードを押してる間の処理
//--------------------------------------------------------------------------------------------------------
void Keyboard(unsigned char key, int x, int y) {

	if (!gameEnd) {
		if (ModeSelect != 3) {
			if (key == 'a') {
				objflag.xposDec = true;
			}
			if (key == 'd') {
				objflag.xposInc = true;
			}
		}
	}
	//if (key == 's') {
	//	//ypos += 96;
	//	objflag.yposInc = true;
	//}
	//if (key == 'w') {
	//	//ypos -= 96;
	//	objflag.yposDec = true;
	//}

	if (key == 'z') {
		//FoV = initialFoV - 5 * 3.0f;
		objflag.FoVInc = true;
	}
	if (key == 'x') {
		//FoV = initialFoV + 5 * 3.0f;
		objflag.FoVDec = true;
	}
	//if (key == 'g') {
	//	ModeSelect = 1;
	//	time(&cuTime);
	//	time(&laTime);
	//	time(&faTime);
	//}
	if (key == 't') {
		//soundPlay(seGetItem, &sseGetItem, false, volume_GetItem);
		//std::cout << "cuTime=" << cuTime;
		//std::cout << "\nlaTime=" << laTime;
	}
	if (key == 'c') {
		//soundStop(&sseGetItem);
		//soundPlay(seCollisionWall, &sseCollisionWall, false, volume_WALL);
		//rot += 0.01;
		//std::cout << "rot = " << rot << std::endl;
	}
	//if (key == 'p') {
	//	soundPlay(sePig, &ssePig, false, 1.0f);
	//}
	//if (key == 'y') {
	//	soundPlay(seCollisionWall, &sseCollisionWall, false, volume_WALL);
	//}
	if (key == escKey) {
		GameAllDelete();
		exit(0);
	}
	if (key == Enter) {
		if (gameEnd && ModeSelect == 1) {
			ModeSelect = 2;
			sound_check = 2;
		}
		else if (gameEnd && ModeSelect == 2) {
			ModeSelect = 3;
			sound_check = 3;
		}
		else if (ModeSelect == 3) {
			ModeSelect = 0;
			sound_check = 0;
		}
	}
	/*
	if (key == Shift) {
		ShiftFlag = true;
		std::cout << "Shift = " << ShiftFlag << std::endl;
	}
	*/
	if (ModeSelect == 0) {
		if (key == 'e') {
			time(&cuTime);
			time(&laTime);
			time(&faTime);
			digreeOfDifficulty = EASY;
			selectDegreeOfDifficulty(EASY);
			ModeSelect = 1;
			//std::cout << "Mode = " << ModeSelect << std::endl;
			//std::cout << "sound = " << sound_check << std::endl;
		}
		if (key == 'n') {
			time(&cuTime);
			time(&laTime);
			time(&faTime);
			digreeOfDifficulty = NORMAL;
			selectDegreeOfDifficulty(NORMAL);
			ModeSelect = 1;
			//std::cout << "Mode = " << ModeSelect << std::endl;
			//std::cout << "sound = " << sound_check << std::endl;
		}
		if (key == 'h') {
			time(&cuTime);
			time(&laTime);
			time(&faTime);
			digreeOfDifficulty = HARD;
			selectDegreeOfDifficulty(HARD);
			ModeSelect = 1;
			//std::cout << "Mode = " << ModeSelect << std::endl;
			//std::cout << "sound = " << sound_check << std::endl;
		}
	}
}

//--------------------------------------------------------------------------------------------------------
// カーソルキーやF7キーを押してる間の処理
//--------------------------------------------------------------------------------------------------------
void special_key(int key, int x, int y) {

	if (!gameEnd) {
		if (ModeSelect != 3) {
			if (key == GLUT_KEY_UP) {
				//position += direction * deltaTime * speed;
				if (hit(cube) == true) {
					//playMusic(COLLISION, PLAY_INSTANCE);
					//position -= direction * deltaTime * speed;
					objflag.keyUP = false;
					objflag.hitUP = true;
				}
				else {
					objflag.keyUP = true;
					objflag.hitUP = false;
				}
			}
			if (key == GLUT_KEY_DOWN) {
				//position -= direction * deltaTime * speed;
				if (hit(cube) == true) {
					//playMusic(COLLISION, PLAY_INSTANCE);
					//position += direction * deltaTime * speed;
					objflag.hitDOWN = true;
					objflag.keyDOWN = false;
				}
				else {
					objflag.keyDOWN = true;
					objflag.hitDOWN = false;
				}
			}
			if (key == GLUT_KEY_RIGHT) {
				//position += right_vec3 * deltaTime * speed;
				if (hit(cube) == true) {
					//playMusic(COLLISION, PLAY_INSTANCE);
					//position -= right_vec3 * deltaTime * speed;
					objflag.hitRIGHT = true;
					objflag.keyRIGHT = false;
				}
				else {
					objflag.keyRIGHT = true;
					objflag.hitRIGHT = false;
				}
			}
			if (key == GLUT_KEY_LEFT) {
				//position -= right_vec3 * deltaTime * speed;
				if (hit(cube) == true) {
					//playMusic(COLLISION, PLAY_INSTANCE);
					//position += right_vec3 * deltaTime * speed;
					objflag.hitLEFT = true;
					objflag.keyLEFT = false;
				}
				else {
					objflag.keyLEFT = true;
					objflag.hitLEFT = false;
				}
			}
		}
	}

	//隠しコマンド Debug用
	if (key == GLUT_KEY_F7) {
		if (ModeSelect == 0) {
			//std::cout << "ModeSelect=0!!!!\n";
			//soundStop(&sbgmPlay);
			digreeOfDifficulty = NORMAL;
			selectDegreeOfDifficulty(NORMAL);
			ModeSelect = 1;
			sound_check = 1;
			time(&cuTime);
			time(&laTime);
			time(&faTime);
		}
		else if (ModeSelect == 1) {
			//std::cout << "ModeSelect=1!!!!\n";
			//soundStop(&sbgmPlay);
			ModeSelect = 2;
			sound_check = 2;
		}
		else if (ModeSelect == 2) {
			//std::cout << "ModeSelect=2!!!!\n";
			//soundStop(&sbgmPlay);
			ModeSelect = 3;
			sound_check = 3;
			//gameEnd = true;
		}
		else if (ModeSelect == 3) {
			ModeSelect = 0;
			sound_check = 0;
		}
		//std::cout << "Mode = " << ModeSelect << std::endl;
		//std::cout << "sound = " << sound_check << std::endl;
		//std::cout << '\n';
	}
	//if (chackDoubleSpecialKeyPush) {
	//	std::cout << "DoublePush\n";
	//}
	//std::cout << "--------------------------------------------------------------------------------------------------\n";
	//std::cout << objflag.hitUP    << " " << objflag.keyRIGHT << " " << objflag.keyLEFT << '\n';
	//std::cout << objflag.hitDOWN  << " " << objflag.keyRIGHT << " " << objflag.keyLEFT << '\n';
	//std::cout << objflag.hitRIGHT << " " << objflag.keyUP    << " " << objflag.keyDOWN << '\n';
	//std::cout << objflag.hitLEFT  << " " << objflag.keyUP    << " " << objflag.keyDOWN << '\n';

}

//--------------------------------------------------------------------------------------------------------
// キーボードから手を離したときの処理
//--------------------------------------------------------------------------------------------------------
void KeyboadUP(unsigned char key, int x, int y) {
	if (key == 'a') {
		//xpos -= 48;
		objflag.xposDec = false;
	}
	if (key == 'd') {
		//xpos += 48;
		objflag.xposInc = false;
	}
	if (key == 's') {
		//ypos += 96;
		objflag.yposInc = false;
	}
	if (key == 'w') {
		//ypos -= 96;
		objflag.yposDec = false;
	}
	if (key == 'z') {
		objflag.FoVInc = false;
	}
	if (key == 'x') {
		objflag.FoVDec = false;
	}
}

//--------------------------------------------------------------------------------------------------------
// カーソルキーやF7キーから手を離したときの処理
//--------------------------------------------------------------------------------------------------------
void special_keyUP(int key, int x, int y) {

	//std::cout << "count=" << countCollisionWall << std::endl;
	//std::cout << "KEYUP" << '\n';
	countCollisionWall = 0;

	if (key == GLUT_KEY_UP) {
		//position += direction * deltaTime * speed;
		objflag.keyUP = false;
		objflag.hitUP = false;
		//if (hit(cube) == true) {
			//playMusic(COLLISION, PLAY_INSTANCE);
			//position -= direction * deltaTime * speed;

		//}
	}
	if (key == GLUT_KEY_DOWN) {
		//position -= direction * deltaTime * speed;
		objflag.keyDOWN = false;
		objflag.hitDOWN = false;
		//if (hit(cube) == true) {
			//playMusic(COLLISION, PLAY_INSTANCE);
			//position += direction * deltaTime * speed;
		//}
	}
	if (key == GLUT_KEY_RIGHT) {
		//position += right_vec3 * deltaTime * speed;
		objflag.keyRIGHT = false;
		objflag.hitRIGHT = false;
		//if (hit(cube) == true) {
			//playMusic(COLLISION, PLAY_INSTANCE);
			//position -= right_vec3 * deltaTime * speed;
		//}
	}
	if (key == GLUT_KEY_LEFT) {
		//position -= right_vec3 * deltaTime * speed;
		objflag.keyLEFT = false;
		objflag.hitLEFT = false;
		//if (hit(cube) == true) {
			//playMusic(COLLISION, PLAY_INSTANCE);
			//position += right_vec3 * deltaTime * speed;
		//}
	}
}

//--------------------------------------------------------------------------------------------------------
// キーボード操作によって変更されたオヴジェクトフラグに沿って自分に関する動作を更新する
//--------------------------------------------------------------------------------------------------------
void moveOBJ() {
	initialFoV = 45.0f;
	mouseSpeed = 0.05f;
	int xpos = 512, ypos = 384;	//いまだに意味不明
	FoV = 45.0f;				//なんか難しいやつ
	//speed = 0.75f;              //主人公の座標を動かす速さ
	hspeed = 1.0f;
	//int changePos = 5;			//カメラの向きを動かす速さ

	if (!gameEnd) {
		//--カメラの向きの制御--//
		if (objflag.xposInc) {
			xpos += changePos;
		}
		if (objflag.xposDec) {
			xpos -= changePos;
		}
		if (objflag.yposInc) {
			ypos += changePos;
		}
		if (objflag.yposDec) {
			ypos -= changePos;
		}

		//--FoVの値を制御--//
		if (objflag.FoVInc) {
			FoV = initialFoV - 5 * 3.0f;
		}
		if (objflag.FoVDec) {
			FoV = initialFoV + 5 * 3.0f;
		}
		/*
		//--壁の衝突を制御--//
		if (objflag.hitUP) {
			position -= direction * deltaTime * hspeed;
		}
		if (objflag.hitDOWN) {
			position += direction * deltaTime * hspeed;
		}
		if (objflag.hitRIGHT) {
			position -= right_vec3 * deltaTime * hspeed;
		}
		if (objflag.hitLEFT) {
			position += right_vec3 * deltaTime * hspeed;
		}
		*/

		//--壁の衝突SEを制御--//
		if (countCollisionWall > initCountCollision) {
			countCollisionWall = 0;
		}

		if (hit(cube)) {
			//std::cout << "HIT\n";
		}

		//--主人公の座標を制御--//
		if (objflag.keyUP) {
			//	if (objflag.keyUP || position.x > 30 || position.x < -30 || position.z < -30 || position.z>28) {
			position += direction * deltaTime * speed;

			if (hit(cube) == true || position.x > 30 || position.x < -30 || position.z < -30 || position.z>28 || (position.x > 13 && position.x < 16 && position.z > -3.5 && position.z < 5) || (position.x > 24 && position.x < 26 && position.z > -18.8 && position.z < 13.2)) {
				if (countCollisionWall == 0) {
					allSeStop();
					//std::cout << "UP WALL\n";
					soundPlay(seCollisionWall, &sseCollisionWall, false, volume_WALL);
				}
				countCollisionWall++;
				position -= direction * deltaTime * speed;
			}
			else {
				if (!chackDoubleSpecialKeyPush) {
					countCollisionWall = 0;
				}
				else {
					countCollisionWall++;
				}
			}

		}
		//if (objflag.keyDOWN || position.x > 30 || position.x < -30 || position.z < -30 || position.z>28) {
		if (objflag.keyDOWN) {
			position -= direction * deltaTime * speed;

			if (hit(cube) == true || position.x > 30 || position.x < -30 || position.z < -30 || position.z>28 || (position.x > 13 && position.x < 16 && position.z > -3.5 && position.z < 5) || (position.x > 24 && position.x < 26 && position.z > -18.8 && position.z < 13.2)) {
				if (countCollisionWall == 0) {
					allSeStop();
					//std::cout << "DOWN WALL\n";
					soundPlay(seCollisionWall, &sseCollisionWall, false, volume_WALL);
				}
				countCollisionWall++;
				position += direction * deltaTime * speed;
			}
			else {
				if (!chackDoubleSpecialKeyPush) {
					countCollisionWall = 0;
				}
				else {
					countCollisionWall++;
				}
			}

		}

		//if (objflag.keyRIGHT || position.x > 30 || position.x < -30 || position.z < -30 || position.z>28) {
		if (objflag.keyRIGHT) {
			position += right_vec3 * deltaTime * speed;

			if (hit(cube) == true || position.x > 30 || position.x < -30 || position.z < -30 || position.z>28 || (position.x > 13 && position.x < 16 && position.z > -3.5 && position.z < 5) || (position.x > 24 && position.x < 26 && position.z > -18.8 && position.z < 13.2)) {
				if (countCollisionWall == 0) {
					allSeStop();
					//std::cout << "RIGHT WALL\n";
					soundPlay(seCollisionWall, &sseCollisionWall, false, volume_WALL);
				}
				countCollisionWall++;
				position -= right_vec3 * deltaTime * speed;
			}
			else {
				if (!chackDoubleSpecialKeyPush) {
					countCollisionWall = 0;
				}
				else {
					countCollisionWall++;
				}
			}

		}

		//if (objflag.keyLEFT || position.x > 30 || position.x < -30 || position.z < -30 || position.z>28) {
		if (objflag.keyLEFT) {
			position -= right_vec3 * deltaTime * speed;

			if (hit(cube) == true || position.x > 30 || position.x < -30 || position.z < -30 || position.z>28 || (position.x > 13 && position.x < 16 && position.z > -3.5 && position.z < 5) || (position.x > 24 && position.x < 26 && position.z > -18.8 && position.z < 13.2)) {
				if (countCollisionWall == 0) {
					allSeStop();
					//std::cout << "LEFT WALL\n";
					soundPlay(seCollisionWall, &sseCollisionWall, false, volume_WALL);
				}
				countCollisionWall++;
				position += right_vec3 * deltaTime * speed;
			}
			else {
				if (!chackDoubleSpecialKeyPush) {
					countCollisionWall = 0;
				}
				else {
					countCollisionWall++;
				}
			}
		}
	}

	// 新たな方向を計算します。
	horizontalAngle += mouseSpeed * deltaTime * float(1024 / 2 - xpos);
	verticalAngle += mouseSpeed * deltaTime * float(768 / 2 - ypos);

	// 方向：球面座標から直角座標に変換します。
	direction.x = cos(verticalAngle) * sin(horizontalAngle);
	direction.y = sin(verticalAngle);
	direction.z = cos(verticalAngle) * cos(horizontalAngle);

	// 右ベクトル
	right_vec3.x = sin(horizontalAngle - 3.14f / 2.0f);
	right_vec3.y = 0;
	right_vec3.z = cos(horizontalAngle - 3.14f / 2.0f);

	gluLookAt(
		position.x, position.y, position.z, // 視点の位置x,y,z;
		//enepos_x2, enepos_y2, enepos_z2,    // 視界の調整(追加 6/12)
		position.x + direction.x, position.y + direction.y, position.z + direction.z,
		0.0, 1.0, 0.0);                     // 視界の上方向のベクトルx,y,z
		//cout << position.x + direction.x << position.y + direction.y<< position.z + direction.z<<std::endl;
	//speed = 4.0f; // 3 units / second

	//リンゴのあたり判定
	if (abs(position.x - 28) < 3 && abs(position.z - (26)) < 3) {
		if (appleFlag[0]) {
			allSeStop();
			soundPlay(seGetItem, &sseGetItem, false, volume_GetItem);
			appleFlag[0] = false;
		}
		//appleFlag[0] = false;
	}
	if (abs(position.x - 11.5) < 3 && abs(position.z - (-27.5)) < 3) {
		if (appleFlag[1]) {
			allSeStop();
			soundPlay(seGetItem, &sseGetItem, false, volume_GetItem);
			appleFlag[1] = false;
		}
		//appleFlag[1] = false;
	}
	if (abs(position.x - 3.5) < 3 && abs(position.z - (-27.5)) < 3) {
		if (appleFlag[2]) {
			allSeStop();
			soundPlay(seGetItem, &sseGetItem, false, volume_GetItem);
			appleFlag[2] = false;
		}
		//appleFlag[2] = false;
	}
	if (abs(position.x - (-18.5)) < 3 && abs(position.z - 0) < 3) {
		if (appleFlag[3]) {
			allSeStop();
			soundPlay(seGetItem, &sseGetItem, false, volume_GetItem);
			appleFlag[3] = false;
		}
		//appleFlag[3] = false;
	}


}

//--------------------------------------------------------------------------------------------------------
// glutMainLoop() を通してディスプレイ コールバックが呼び出される
//--------------------------------------------------------------------------------------------------------
static void Idle(void) {
	fps->GetFPS();				//FPSを得る
	if (fps->draw) {			//秒間60フレームのタイミングで描画
		glutPostRedisplay();	//glutDisplayFunc()を１回実行する
	}
	glutPostRedisplay();
}

//--------------------------------------------------------------------------------------------------------
// 3D表示させるために必要（？）な関数. 森口先生のmain文から引用
//--------------------------------------------------------------------------------------------------------
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

//--------------------------------------------------------------------------------------------------------
// Reshape Windowのサイズ変更. 基本的には必要ない
//--------------------------------------------------------------------------------------------------------
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

//--------------------------------------------------------------------------------------------------------
// START画面を表示
//--------------------------------------------------------------------------------------------------------
void StartMode() {
	//#ifdef USE_DIMENCO_OPENGL_INTERFACE
	//	dimencoSetBackgroundState();
	//#endif

	if (selectEnviroment == MYPC) {
		gluLookAt(
			0.125, 0.08, 5.5,						// ワールド空間でカメラは(4,3,3)にあります。
			//enepos_x2, enepos_y2, enepos_z2,		// 視界の調整(追加 6/12)
			0.125, 0.08, 0,
			0.0, 1.0, 0.0);							// 視界の上方向のベクトルx,y,z
	}
	else if (selectEnviroment == DIMENCO) {
		gluLookAt(
			0.05, -0.02, 5.5,						// ワールド空間でカメラは(4,3,3)にあります。
			//enepos_x2, enepos_y2, enepos_z2,		// 視界の調整(追加 6/12)
			0.05, -0.02, 0,
			0.0, 1.0, 0.0);							// 視界の上方向のベクトルx,y,z
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//#ifdef USE_DIMENCO_OPENGL_INTERFACE
	//	dimencoSetZBufState();
	//#endif

	// テクスチャマッピング有効化 (追加 bmp)
	glEnable(GL_TEXTURE_2D);
	//　テクスチャをバインド (追加 bmp)
	glBindTexture(GL_TEXTURE_2D, Texture[3].ID);
	//glBindTexture(GL_TEXTURE_2D, Texture[2].ID);
	glPushMatrix();
	OBJFile[3].Draw();
	glPopMatrix();

	// テクスチャマッピング有効化 (追加 bmp)
	glEnable(GL_TEXTURE_2D);
	//　テクスチャをバインド (追加 bmp)
	glBindTexture(GL_TEXTURE_2D, Texture[4].ID);
	glPushMatrix();
	OBJFile[4].Draw();
	glPopMatrix();


	//if (flush_cnt < 100) {
	glEnable(GL_TEXTURE_2D);						// テクスチャマッピング有効化 (追加 bmp)
	glBindTexture(GL_TEXTURE_2D, Texture[5].ID);	//　テクスチャをバインド (追加 bmp)
	glPushMatrix();
	OBJFile[5].Draw();
	glPopMatrix();
	//}
	//else if (flush_cnt == 200) {
	//	flush_cnt = 0;
	//}
	//flush_cnt++;
	//std::cout << "flush = " << flush_cnt << std::endl;

	// テクスチャマッピング有効化 (追加 bmp)
	//glEnable(GL_TEXTURE_2D);
	//　テクスチャをバインド (追加 bmp)
	//glBindTexture(GL_TEXTURE_2D, Texture[6].ID);
	//glPushMatrix();
	//OBJFile[6].Draw();
	//glPopMatrix();
}

//--------------------------------------------------------------------------------------------------------
// PLAY画面を表示
//--------------------------------------------------------------------------------------------------------
void PlayMode() {
	//if (playflag == false) {
	//	std::cout << "asdsn";
	//	playflag = true;
	//	gluLookAt(
	//		-5, 1, -12, // ワールド空間でカメラは(4,3,3)にあります。
	//		//enepos_x2, enepos_y2, enepos_z2,		// 視界の調整(追加 6/12)
	//		1, 1, 1,
	//		0.0, 1.0, 0.0);							// 視界の上方向のベクトルx,y,z
	//}

	//gluLookAt(
	//	4, 1, 2,								// ワールド空間でカメラは(4,3,3)にあります。
	//	//enepos_x2, enepos_y2, enepos_z2,		// 視界の調整(追加 6/12)
	//	1, 1, 1,
	//	0.0, 1.0, 0.0
	//);											// 視界の上方向のベクトルx,y,z

	//decideVolumeOfPigCry(disFromPig);				//ブタと主人公との距離に応じて鳴き声の大きさを変更

	//time_t cuTime;
	//currentTime = glfwGetTime();
	//deltaTime = float(currentTime - lastTime);
	//lastTime = currentTime;
	if (timeflag == true) {
		time(&cuTime);
	}


	//deltaTime = float(cuTime - laTime);
	deltaTime = 0.016666;
	//laTime = cuTime;
	defTime = cuTime - faTime;//プレイスタートから経過時間

	//std::cout << cuTime << " " << faTime << std::endl;


	//動かすときに使ってる(仮)
	movecount += 0.001;
	moveOBJ();




	// カメラのほうを向いていない法線の三角形をカリングします。
	glEnable(GL_CULL_FACE);

	//glfwPollEvents();



	//---オブジェクト描画部---//
	Wall();					//壁
	Floor();				//床
	if (ModeSelect != 3) {	//ViewModeでないならば
		TimeLimit();		//制限時間
	}
	NumOfApple();			//現在のリンゴ取得数
	DirectionOfPig();		//近くにいるブタの方向(警告テクスチャ) & 4匹のブタ
	Apple();				//4つのリンゴ
	AppleCheck();			//リンゴアラート
		//リンゴがなくなってたらリザルトに
	if (appleFlag[0] == false && appleFlag[1] == false && appleFlag[2] == false && appleFlag[3] == false) {
		GameClear();
		//ModeSelect = 2;
		//sound_check = 2;
	}
	if (skycheck == true) {
		Sky();				//空
	}


	//リンゴがなくなってたらリザルトに
	//if (appleFlag[0] == false && appleFlag[1] == false && appleFlag[2] == false && appleFlag[3] == false) {
		//GameClear();
		//ModeSelect = 2;
		//sound_check = 2;
	//}

	//if (dbg_counter == 1000) {
	//	std::cout << "pig_speed = " << obj_speed << std::endl;
	//	std::cout << "apple = " << applecount << std::endl;
	//	std::cout << "  pig(" << changePigSpeedFlag[0] << changePigSpeedFlag[1] << changePigSpeedFlag[2] << changePigSpeedFlag[3] << ")\n";
	//	std::cout << "apple(" << appleFlag[0] << appleFlag[1] << appleFlag[2] << appleFlag[3] << ")\n";
	//	std::cout << "disFromPig =" << disFromPig << std::endl;
	//	std::cout << "volume_PIG = " << volume_PIG << std::endl;
	//	dbg_counter = 0;
	//}
	//dbg_counter++;
}

//--------------------------------------------------------------------------------------------------------
// RESULT画面を表示
//--------------------------------------------------------------------------------------------------------
void ResultMode() {

	if (selectEnviroment == MYPC) {
		gluLookAt(
			0.125, 0.08, 5.5, // ワールド空間でカメラは(4,3,3)にあります。
			//enepos_x2, enepos_y2, enepos_z2,    // 視界の調整(追加 6/12)
			0.125, 0.08, 0,
			0.0, 1.0, 0.0);                     // 視界の上方向のベクトルx,y,z
	}
	else if (selectEnviroment == DIMENCO) {
		//ver2.7.1.8
		//gluLookAt(
		//	0.06, 0.02, 5.5,						// ワールド空間でカメラは(4,3,3)にあります。
		//	//enepos_x2, enepos_y2, enepos_z2,		// 視界の調整(追加 6/12)
		//	0.06, 0.02, 0,							// 視界の上方向のベクトルx,y,z
		//	0.0, 1.0, 0.0
		//);        

		//ver2.7.1.9
		gluLookAt(
			0.07, 0.02, 5.5,						// ワールド空間でカメラは(4,3,3)にあります。
			//enepos_x2, enepos_y2, enepos_z2,		// 視界の調整(追加 6/12)
			0.07, 0.02, 0,							// 視界の上方向のベクトルx,y,z
			0.0, 1.0, 0.0
		);

		//ver2.7.1.10
		//gluLookAt(
		//	0.08, 0.02, 5.5,						// ワールド空間でカメラは(4,3,3)にあります。
		//	//enepos_x2, enepos_y2, enepos_z2,		// 視界の調整(追加 6/12)
		//	0.07, 0.02, 0,							// 視界の上方向のベクトルx,y,z
		//	0.0, 1.0, 0.0
		//);
	}

	applecount = 0;
	//残リンゴ数確認
	for (int i = 0; i < 4; i++) {
		if (appleFlag[i] == true) {
			applecount++;
		}
	}

	// テクスチャマッピング有効化 (追加 bmp)
	glEnable(GL_TEXTURE_2D);
	//　テクスチャをバインド (追加 bmp)
	//glBindTexture(GL_TEXTURE_2D, Texture[3].ID);

	if (applecount == 0) {
		glBindTexture(GL_TEXTURE_2D, Texture[23].ID);
	}
	else if (applecount == 1) {
		glBindTexture(GL_TEXTURE_2D, Texture[22].ID);
	}
	else if (applecount == 2) {
		glBindTexture(GL_TEXTURE_2D, Texture[21].ID);
	}
	else if (applecount == 3) {
		glBindTexture(GL_TEXTURE_2D, Texture[20].ID);
	}
	else if (applecount == 4) {
		glBindTexture(GL_TEXTURE_2D, Texture[19].ID);
	}

	glPushMatrix();
	glTranslatef(0.018f, -0.024f, 0.0f);
	if (selectEnviroment == MYPC) {
		glScalef(1.45f, 1.62f, 1.0f);
	}
	else if (selectEnviroment == DIMENCO) {
		glScalef(2.4f, 2.15f, 1.0f);
	}
	OBJFile[3].Draw();
	glPopMatrix();

	// テクスチャマッピング有効化 (追加 bmp)
	glEnable(GL_TEXTURE_2D);
	//　テクスチャをバインド (追加 bmp)
	glBindTexture(GL_TEXTURE_2D, Texture[4].ID);
	glPushMatrix();
	OBJFile[4].Draw();
	glPopMatrix();


}

//--------------------------------------------------------------------------------------------------------
// ゲームをリセットする
//--------------------------------------------------------------------------------------------------------
void GameReset() {
	gameEnd = false;
	InitEndFlag();
	InitObjFlag();
	position.x = 0, position.y = 0.5, position.z = 0;//カメラの位置
	direction.x = 0, direction.y = 0, direction.z = 0;//何でもいい
	horizontalAngle = 0.0f;//カメラの向き
	verticalAngle = 0.0f;
	movecount = 0;
	p = 0;
	applecount = 4;
	//リンゴの存在フラグの初期化
	for (int i = 0; i < 4; i++) {
		appleFlag[i] = true;
	}

	//HARD用のブタの速度UPフラグの初期化
	for (int i = 0; i < 4; i++) {
		changePigSpeedFlag[i] = false;
	}

	skycheck = true;
	timeflag = true;

	pigx[0] = -2;
	pigx[1] = 20;
	pigx[2] = -22;
	pigx[3] = -14;
	pigz[0] = 26;
	pigz[1] = -2.5;
	pigz[2] = -26.5;
	pigz[3] = 16, 5;
	pigy[0] = -1.0;
	pigy[1] = -1.0;
	pigy[2] = -1.0;
	pigy[3] = -1.0;
	pigflag[0] = 0;
	pigflag[1] = 0;
	pigflag[2] = 0;
	pigflag[3] = 0;
	px[0] = 22;
	px[1] = 20;
	px[2] = -26;
	px[3] = -26;
	pz[0] = 26;
	pz[1] = -26;
	pz[2] = -26.5;
	pz[3] = 16.5;
	s_px[0] = -2;
	s_px[1] = 20;
	s_px[2] = -2;
	s_px[3] = -14;
	s_pz[0] = 26;
	s_pz[1] = -2.5;
	s_pz[2] = -26.5;
	s_pz[3] = 16.5;


	////--------------------------------------------
	//// 手元のPC環境
	////--------------------------------------------
	//if (selectEnviroment == MYPC) {
	//	speed = 0.75f;              //主人公の座標を動かす速さ
	//	changePos = 2;				//カメラの向きを動かす速さ
	//}
	////--------------------------------------------
	//// Dimenco Display用環境
	////--------------------------------------------
	//else if (selectEnviroment == DIMENCO) {
	//	speed = 2.5f;               //主人公の座標を動かす速さ
	//	changePos = 5;				//カメラの向きを動かす速さ
	//}
}

//--------------------------------------------------------------------------------------------------------
// ステージを上から見下ろす
//--------------------------------------------------------------------------------------------------------
void ViewMode() {
	if (selectEnviroment == MYPC) {
		gluLookAt(15.8, 180, -7.4,
			15.8, 0, -7.4,
			0, 0, -1
		);
	}
	else if (selectEnviroment == DIMENCO) {
		//gluLookAt(20, 160, -10,
		//	20, 0, -10,
		//	0, 0, -1
		//);

		//gluLookAt(30, 160, -18,
		//	30, 0, -18,
		//	0, 0, -1
		//);

		//gluLookAt(10, 160, -2,
		//	10, 0, -2,
		//	0, 0, -1
		//);

		//gluLookAt(3, 160, 3,
		//	3, 0, 3,
		//	0, 0, -1
		//);

		gluLookAt(6, 160, 3,
			6, 0, 3,
			0, 0, -1
		);
	}
	time(&cuTime);
	time(&laTime);
	time(&faTime);
	PlayMode();
}

//--------------------------------------------------------------------------------------------------------
// ゲーム難易度を選択. 引数は enum の DigreeOfDifficulty
//--------------------------------------------------------------------------------------------------------
void selectDegreeOfDifficulty(int dod) {
	if (selectEnviroment == MYPC) {
		if (dod == EASY) {
			setPigSpeed(0.003*Bigger_pig);
			change_obj_speed = 0;
			accessPig = false;
		}
		else if (dod == NORMAL) {
			setPigSpeed(0.005*Bigger);
			change_obj_speed = 0;
			accessPig = true;
		}
		else if (dod == HARD) {
			setPigSpeed(0.005*Bigger_pig);
			change_obj_speed = 0.002;
			accessPig = true;
		}
	}
	else if (selectEnviroment == DIMENCO) {
		if (dod == EASY) {
			setPigSpeed(0.02*Bigger_pig);
			change_obj_speed = 0;
			accessPig = false;
		}
		else if (dod == NORMAL) {
			setPigSpeed(0.005*Bigger_pig);
			change_obj_speed = 0;
			accessPig = true;
		}
		else if (dod == HARD) {
			setPigSpeed(0.005*Bigger_pig);
			change_obj_speed = 0.002;
			accessPig = true;
		}
	}

	//if (dod == EASY) {
	//	setPigSpeed(0.02);
	//	change_obj_speed = 0;
	//	accessPig = false;
	//}
	//else if (dod == NORMAL) {
	//	setPigSpeed(0.025);
	//	change_obj_speed = 0;
	//	accessPig = true;
	//}
	//else if (dod == HARD) {
	//	setPigSpeed(0.025);
	//	change_obj_speed = 0.002;
	//	accessPig = true;
	//}

	//if (dod == EASY) {
	//	setPigSpeed(0.003);
	//	change_obj_speed = 0;
	//	accessPig = false;
	//}
	//else if (dod == NORMAL) {
	//	setPigSpeed(0.005);
	//	change_obj_speed = 0;
	//	accessPig = true;
	//}
	//else if (dod == HARD) {
	//	setPigSpeed(0.005);
	//	change_obj_speed = 0.002;
	//	accessPig = true;
	//}

}
//--------------------------------------------------------------------------------------------------------
// ブタの速さをセット
//--------------------------------------------------------------------------------------------------------
void setPigSpeed(double pig_speed) {
	obj_speed = pig_speed;
}

//--------------------------------------------------------------------------------------------------------
// ブタの速さを増減
//--------------------------------------------------------------------------------------------------------
void changePigSpeed(double change_pig_speed) {
	obj_speed += change_pig_speed;
}

//--------------------------------------------------------------------------------------------------------
// 主人公が壁に衝突しているか否かを返すbool型関数
//--------------------------------------------------------------------------------------------------------
bool hit(double cube[29][4]) {
	for (int i = 0; i < 29; i++) {
		if ((position.x < cube[i][0] && position.x > cube[i][1]) &&
			(position.z < cube[i][2] && position.z > cube[i][3])) {
			return true;
		}
	}
	return false;
}

//--------------------------------------------------------------------------------------------------------
// ブタが壁に衝突しているか否かを返すbool型関数1
//--------------------------------------------------------------------------------------------------------
bool objhit(double xmin, double xmax, double zmin, double zmax, double cube[29][4]) {
	for (int i = 0; i < 29; i++) {
		if ((xmin < cube[i][0] && xmax > cube[i][1]) &&
			(zmin < cube[i][2] && zmax > cube[i][3])) {
			return true;
		}
	}
	return false;
}

//--------------------------------------------------------------------------------------------------------
// ブタが壁に衝突しているか否かを返すbool型関数2
//--------------------------------------------------------------------------------------------------------
bool objhit2(double x, double z, double cube[29][4]) {

	for (int i = 0; i < 29; i++) {
		double x1 = x - 0.5, x2 = x + 1.6;
		double z1 = z - 0.9, z2 = z + 0.8;
		if ((x1 < cube[i][0] && x2 > cube[i][1]) &&
			(z1 < cube[i][2] && z2 > cube[i][3])) {
			return true;
		}
	}

	double x1 = x - 0.5, x2 = x + 1.6;
	double z1 = z - 0.9, z2 = z + 0.8;

	if (x1 > 13.5 && x1 < 16 && z1 > -3.5 && z1 < 5 ||
		x1 > 13.5 && x1 < 16 && z2 > -3.5 && z2 < 5 ||
		x2 > 13.5 && x2 < 16 && z1 > -3.5 && z1 < 5 ||
		x2 > 13.5 && x2 < 16 && z2 > -3.5 && z2 < 5) {
		return true;
	}
	return false;
}

//--------------------------------------------------------------------------------------------------------
// 豚を動きに合わせて回転させる
//--------------------------------------------------------------------------------------------------------
void PigRolling(double xspeed, double zspeed) {
	float sita;

	sita = atan(xspeed / zspeed);

	//std::cout << sita << std::endl;

	float pi = 3.1415;
	float onedegree = pi / 180.0f; //1°のラジアン
	float rolldegree = sita / onedegree;
	//if (xspeed> 0) {
	//	rolldegree = rolldegree +180;
	//}
	if (zspeed < 0) {
		rolldegree = rolldegree + 180;
	}
	glRotatef(rolldegree, 0.0f, 1.0f, 0.0f);
}

//--------------------------------------------------------------------------------------------------------
// 自分の動きに連動してオブジェクトを動かし、常にその場にオブジェクトがあるかのように見せる処理
//--------------------------------------------------------------------------------------------------------
void ObjMoveRoll_Camera(float hA, float vA, int i) {
	double x, y, z;
	float Ax, Az;
	//glm::vec3(4, 1, 2);
	x = position.x;
	y = position.y;
	z = position.z;

	float pi = 3.1415;
	float onedegree = pi / 180.0f; //1°のラジアン
	float rolldegree = hA / onedegree;

	//------------------------------------------------------
	// 手元のPC環境用
	//------------------------------------------------------
	if (selectEnviroment == MYPC) {
		if (i == 0) {

			Ax = 1.2*sin(hA + pi / 36.0);
			Az = 1.2*cos(hA + pi / 36.0);

			glTranslatef(x + Ax, y + 0.15, z + Az);
			//glTranslatef(x + Ax, y + 0.20, z + Az);
			//glTranslatef(x + Ax, y + 0.15, z + Az);
			//glTranslatef(x + Ax, y + 0.15, z + Az);
			glScalef(0.5f, 0.5f, 0.5f);
			glRotatef(rolldegree, 0.0f, 1.0f, 0.0f);

		}
		else if (i == 1) {


			Ax = sin(hA + pi / 24.0);
			Az = cos(hA + pi / 24.0);

			glTranslatef(x + Ax, y - 0.219, z + Az);
			glScalef(0.5f, 0.5f, 0.5f);
			glRotatef(rolldegree, 0.0f, 1.0f, 0.0f);

		}
		else if (i == 2) {

			Ax = sin(hA + pi / 48.0);
			Az = cos(hA + pi / 48.0);

			//glTranslatef(x + Ax, y - 0.25, z + Az);
			glTranslatef(x + Ax, y - 0.22, z + Az);
			glScalef(0.5f, 0.5f, 0.5f);
			glRotatef(rolldegree, 0.0f, 1.0f, 0.0f);
		}
		else if (i == 3) {

			Ax = sin(hA - pi / 600.0);
			Az = cos(hA - pi / 600.0);

			//glTranslatef(x + Ax, y - 0.25, z + Az);
			glTranslatef(x + Ax, y - 0.22, z + Az);
			glScalef(0.5f, 0.5f, 0.5f);
			glRotatef(rolldegree, 0.0f, 1.0f, 0.0f);
		}
	}
	//------------------------------------------------------
	// Dimenco Display環境用
	//------------------------------------------------------
	else if (selectEnviroment == DIMENCO) {
		if (i == 0) {

			Ax = 1.2*sin(hA + pi / 65.0);
			Az = 1.2*cos(hA + pi / 65.0);

			glTranslatef(x + Ax, y + 0.25, z + Az);
			glScalef(0.5f, 0.5f, 0.5f);
			glRotatef(rolldegree, 0.0f, 1.0f, 0.0f);

		}
		else if (i == 1) {


			Ax = sin(hA + pi / 35.0);
			Az = cos(hA + pi / 35.0);

			glTranslatef(x + Ax, y - 0.2, z + Az);
			glScalef(0.5f, 0.5f, 0.5f);
			glRotatef(rolldegree, 0.0f, 1.0f, 0.0f);

		}
		else if (i == 2) {

			Ax = sin(hA + pi / 110.0);
			Az = cos(hA + pi / 110.0);

			glTranslatef(x + Ax, y - 0.2, z + Az);
			glScalef(0.5f, 0.5f, 0.5f);
			glRotatef(rolldegree, 0.0f, 1.0f, 0.0f);
		}
		else if (i == 3) {

			Ax = sin(hA - pi / 100.0);
			Az = cos(hA - pi / 100.0);

			glTranslatef(x + Ax, y - 0.2, z + Az);
			glScalef(0.5f, 0.5f, 0.5f);
			glRotatef(rolldegree, 0.0f, 1.0f, 0.0f);
		}
	}
	//Debug
	//std::cout << "x=" << x + Ax << ",y=" << y+0.05 << ",z=" << z + Az << std::endl;
	//movemat = glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, x + Ax, y, z + Az, 1);
	//position.x = -12, position.y = 1, position.z = -15.5
	//glTranslatef(-11.0, 1.05, -15.6);
	//glRotatef(90.0f,0.0f,1.0f,0.0f);
}


//////////////////////////////////////////////////////////////////
// PlayMode()内のオブジェクトの表示関係
/////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------------------------------
// 壁
//--------------------------------------------------------------------------------------------------------
void Wall() {
	glEnable(GL_TEXTURE_2D);						// テクスチャマッピング有効化 (追加 bmp)
	glBindTexture(GL_TEXTURE_2D, Texture[0].ID);	// テクスチャをバインド (追加 bmp)
	glPushMatrix();									// 行列スタックの最上部をコピー
	OBJFile[0].Draw();								// オブジェクトを描画
	glPopMatrix();									// 行列スタックを破棄
}

//--------------------------------------------------------------------------------------------------------
// 床
//--------------------------------------------------------------------------------------------------------
void Floor() {
	glEnable(GL_TEXTURE_2D);						// テクスチャマッピング有効化 (追加 bmp)
	glBindTexture(GL_TEXTURE_2D, Texture[1].ID);	//　テクスチャをバインド (追加 bmp)
	glPushMatrix();
	glTranslatef(0.0, 0.0, -2.0);
	glScalef(0.78f, 1.0f, 0.85f);
	OBJFile[1].Draw();
	glPopMatrix();
}

//--------------------------------------------------------------------------------------------------------
// 制限時間
//--------------------------------------------------------------------------------------------------------
void TimeLimit() {
	bool debug = true;
	if (debug) {
		//std::cout << "defTime = " << defTime << std::endl;
	}
	if (defTime > 18 && !endflag.gameoverFlag && !endflag.gameclearFlag) {//ゲーム終了
		std::cout << "TIMEUP\n";
		TimeUp();
		//ModeSelect = 2;
		//sound_check = 2;
	}
	else if (defTime > 17) {
		glEnable(GL_TEXTURE_2D);								// テクスチャマッピング有効化 (追加 bmp)
		glBindTexture(GL_TEXTURE_2D, Texture[24].ID);			//　テクスチャをバインド (追加 bmp)
		glPushMatrix();
		ObjMoveRoll_Camera(horizontalAngle, verticalAngle, 0);
		//glTranslatef(-11.0, 1.05, -15.6);
		//glRotatef(90.0f,0.0f,1.0f,0.0f);
		OBJFile[2].Draw();
		glPopMatrix();
	}
	else if (defTime > 12) {
		glEnable(GL_TEXTURE_2D);								// テクスチャマッピング有効化 (追加 bmp)
		glBindTexture(GL_TEXTURE_2D, Texture[18].ID);			//　テクスチャをバインド (追加 bmp)
		glPushMatrix();
		ObjMoveRoll_Camera(horizontalAngle, verticalAngle, 0);
		//glTranslatef(-11.0, 1.05, -15.6);
		//glRotatef(90.0f,0.0f,1.0f,0.0f);
		OBJFile[2].Draw();
		glPopMatrix();
	}
	else if (defTime > 6) {
		glEnable(GL_TEXTURE_2D);								// テクスチャマッピング有効化 (追加 bmp)
		glBindTexture(GL_TEXTURE_2D, Texture[26].ID);			//　テクスチャをバインド (追加 bmp)
		glPushMatrix();
		ObjMoveRoll_Camera(horizontalAngle, verticalAngle, 0);
		//glTranslatef(-11.0, 1.05, -15.6);
		//glRotatef(90.0f,0.0f,1.0f,0.0f);
		OBJFile[2].Draw();
		glPopMatrix();
	}
	else {
		glEnable(GL_TEXTURE_2D);								// テクスチャマッピング有効化 (追加 bmp)
		glBindTexture(GL_TEXTURE_2D, Texture[25].ID);			//　テクスチャをバインド (追加 bmp)
		//glBindTexture(GL_TEXTURE_2D, Texture[26].ID);
		glPushMatrix();
		ObjMoveRoll_Camera(horizontalAngle, verticalAngle, 0);
		//glTranslatef(-11.0, 1.05, -15.6);
		//glRotatef(90.0f,0.0f,1.0f,0.0f);
		OBJFile[2].Draw();
		glPopMatrix();
	}
}
//--------------------------------------------------------------------------------------------------------
// 取得すべきリンゴの数
//--------------------------------------------------------------------------------------------------------
void NumOfApple() {
	glEnable(GL_TEXTURE_2D);										// テクスチャマッピング有効化 (追加 bmp)
	if (selectEnviroment == MYPC) {
		if (applecount == 4) {											// 取得すべきリンゴはあと4つ
			glBindTexture(GL_TEXTURE_2D, Texture[4].ID);				// テクスチャをバインド (追加 bmp)
			setPigSpeed(0.005*Bigger_pig);											// 初速
			//setPigSpeed(0.025);											// 初速
		}
		else if (applecount == 3) {										// 取得すべきリンゴはあと3つ
			glBindTexture(GL_TEXTURE_2D, Texture[10].ID);				// テクスチャをバインド (追加 bmp)
			if (!changePigSpeedFlag[3] && digreeOfDifficulty == HARD) {	// まだ速さを変えていないならば(HARDのみ)
				changePigSpeedFlag[3] = true;							// もう変更しない
				setPigSpeed(0.006*Bigger_pig);										// HARDのときのみ
				//setPigSpeed(0.035);											// 初速
			}
		}
		else if (applecount == 2) {										// 取得すべきリンゴはあと2つ
			glBindTexture(GL_TEXTURE_2D, Texture[11].ID);				// テクスチャをバインド (追加 bmp)
			if (!changePigSpeedFlag[2] && digreeOfDifficulty == HARD) {	// まだ速さを変えていないならば(HARDのみ)
				changePigSpeedFlag[2] = true;							// もう変更しない
				setPigSpeed(0.007*Bigger_pig);										// HARDのときのみ
				//setPigSpeed(0.045);											// 初速
			}
		}
		else if (applecount == 1) {										// 取得すべきリンゴはあと1つ
			glBindTexture(GL_TEXTURE_2D, Texture[12].ID);				// テクスチャをバインド (追加 bmp)
			if (!changePigSpeedFlag[1] && digreeOfDifficulty == HARD) {	// まだ速さを変えていないならば(HARDのみ)
				changePigSpeedFlag[1] = true;							// もう変更しない
				setPigSpeed(0.008*Bigger_pig);										// HARDのときのみ
				//setPigSpeed(0.05);											// 初速
			}
		}
		else if (applecount == 0) {										// 全部取得した
			glBindTexture(GL_TEXTURE_2D, Texture[13].ID);				// テクスチャをバインド (追加 bmp)
			if (!changePigSpeedFlag[0] && digreeOfDifficulty == HARD) {	// まだ速さを変えていないならば(HARDのみ)
				changePigSpeedFlag[0] = true;							// もう変更しない
			}
		}
	}
	else if (selectEnviroment == DIMENCO) {
		if (applecount == 4) {											// 取得すべきリンゴはあと4つ
			glBindTexture(GL_TEXTURE_2D, Texture[4].ID);				// テクスチャをバインド (追加 bmp)
			setPigSpeed(0.005*Bigger_pig);											// 初速
		}
		else if (applecount == 3) {										// 取得すべきリンゴはあと3つ
			glBindTexture(GL_TEXTURE_2D, Texture[10].ID);				// テクスチャをバインド (追加 bmp)
			if (!changePigSpeedFlag[3] && digreeOfDifficulty == HARD) {	// まだ速さを変えていないならば(HARDのみ)
				changePigSpeedFlag[3] = true;							// もう変更しない
				setPigSpeed(0.006*Bigger_pig);										// HARDのときのみ
			}
		}
		else if (applecount == 2) {										// 取得すべきリンゴはあと2つ
			glBindTexture(GL_TEXTURE_2D, Texture[11].ID);				// テクスチャをバインド (追加 bmp)
			if (!changePigSpeedFlag[2] && digreeOfDifficulty == HARD) {	// まだ速さを変えていないならば(HARDのみ)
				changePigSpeedFlag[2] = true;							// もう変更しない
				setPigSpeed(0.007*Bigger_pig);										// HARDのときのみ
			}
		}
		else if (applecount == 1) {										// 取得すべきリンゴはあと1つ
			glBindTexture(GL_TEXTURE_2D, Texture[12].ID);				// テクスチャをバインド (追加 bmp)
			if (!changePigSpeedFlag[1] && digreeOfDifficulty == HARD) {	// まだ速さを変えていないならば(HARDのみ)
				changePigSpeedFlag[1] = true;							// もう変更しない
				setPigSpeed(0.008*Bigger_pig);										// HARDのときのみ
			}
		}
		else if (applecount == 0) {										// 全部取得した
			glBindTexture(GL_TEXTURE_2D, Texture[13].ID);				// テクスチャをバインド (追加 bmp)
			if (!changePigSpeedFlag[0] && digreeOfDifficulty == HARD) {	// まだ速さを変えていないならば(HARDのみ)
				changePigSpeedFlag[0] = true;							// もう変更しない
			}
		}
	}
	glPushMatrix();
	ObjMoveRoll_Camera(horizontalAngle, verticalAngle, 1);
	//glTranslatef(-11.0, 1.05, -15.6);
	//glRotatef(90.0f,0.0f,1.0f,0.0f);
	OBJFile[10].Draw();
	glPopMatrix();
}


//--------------------------------------------------------------------------------------------------------
// 近くのブタの方向&ブタ
// disFromPig     : ブタと自分とのユークリッド距離
// disOfNoticePig : ブタが自分に気づく距離 
//--------------------------------------------------------------------------------------------------------
void DirectionOfPig() {
	for (int i = 0; i < 4; i++) {

		glEnable(GL_TEXTURE_2D);						// テクスチャマッピング有効化 (追加 bmp)
		glBindTexture(GL_TEXTURE_2D, Texture[7].ID);	// テクスチャをバインド (追加 bmp)
		glPushMatrix();

		//double obj_speed = 0.001;						// objectのスピード. setPigSpeed(double speed), changePigSpeed(double speed)で変更
		double dif_x, dif_z;							// 主人公と豚との座標の差
		double obj_xspeed, obj_zspeed;					// ブタの速さ
		int hitflag;									// ヒットフラグ
		double hittmp;									// ヒットフラグの一時保存先?

		glTranslatef(pigx[i], pigy[i], pigz[i]);			// ブタを移動

		dif_x = position.x - pigx[i];
		dif_z = position.z - pigz[i];
		disFromPig = sqrt(dif_x * dif_x + dif_z * dif_z);
		disFromPig2 = 0;
		double pointx, pointz; //pointと豚の差
		double dispoint;


		if (!gameEnd) {

			if (pigNoticeFlag[i] == true) {				//ブタの接近フラグがtrue(難易度EASYのみ) かつ, ブタと自分の距離がdisOfNoticePig以下
				obj_xspeed = (double)(dif_x / disFromPig * obj_speed);
				obj_zspeed = (double)(dif_z / disFromPig * obj_speed);;
			}
			else if (pigNoticeFlag[i] == false) {
				if (pigflag[i] == 0) {
					pointx = px[i] - pigx[i];
					pointz = pz[i] - pigz[i];
					dispoint = sqrt(pointx * pointx + pointz * pointz);
					obj_xspeed = (double)(pointx / dispoint * obj_speed);
					obj_zspeed = (double)(pointz / dispoint * obj_speed);
				}
				else if (pigflag[i] == 1) {
					pointx = s_px[i] - pigx[i];
					pointz = s_pz[i] - pigz[i];
					dispoint = sqrt(pointx * pointx + pointz * pointz);
					obj_xspeed = (double)(pointx / dispoint * obj_speed);
					obj_zspeed = (double)(pointz / dispoint * obj_speed);
				}
			}

			double xsize = 5.0 / obj_xspeed;
			double zsize = 5.0 / obj_zspeed;

			if (selectEnviroment == MYPC) {
				//disFromPig2 = sqrt((dif_x - obj_xspeed * 200) * (dif_x - obj_xspeed * 200) + (dif_z - obj_zspeed * 200) * (dif_z - obj_zspeed * 200));
				disFromPig2 = sqrt((dif_x - obj_xspeed * 1000) * (dif_x - obj_xspeed * 1000) + (dif_z - obj_zspeed * 1000) * (dif_z - obj_zspeed * 1000));
			}
			else if (selectEnviroment == DIMENCO) {
				//disFromPig2 = sqrt((dif_x - obj_xspeed * 200) * (dif_x - obj_xspeed * 200) + (dif_z - obj_zspeed * 200) * (dif_z - obj_zspeed * 200));
				disFromPig2 = sqrt((dif_x - obj_xspeed * 50) * (dif_x - obj_xspeed * 50) + (dif_z - obj_zspeed * 50) * (dif_z - obj_zspeed * 50));
			}
			/*
			if (disFromPig2 < disOfNoticePig) {
				decideVolumeOfPigCry(disFromPig2);
			}
			*/

			//ブタがエリア内だけ追いかける
			if (i == 0 && (disFromPig2 < disOfNoticePig || disFromPig < 3.5)) {
				if (position.x > -7 && position.z > 4) {
					areafind = true;
				}
				else areafind = false;
			}
			else if (i == 1 && (disFromPig2 < disOfNoticePig || disFromPig < 3.5)) {
				if (position.x > 9 && position.z < -4) {
					areafind = true;
				}
				else areafind = false;
			}
			else if (i == 2 && (disFromPig2 < disOfNoticePig || disFromPig < 3.5)) {
				if (position.x < 9 && position.z < -4) {
					areafind = true;
				}
				else areafind = false;
			}
			else if (i == 3 && (disFromPig2 < disOfNoticePig || disFromPig < 3.5)) {
				if ((position.x < -15 && position.z > 4) ||
					(position.x < -7 && position.z > 4)) {
					areafind = true;
				}
				else areafind = false;
			}
			else {
				areafind = false;
			}

			//std::cout << disFromPig << "\n";
			//std::cout << obj_xspeed << "\n";
			if (accessPig == true && areafind == true) {	//ブタの接近フラグがtrue(難易度EASYのみ) かつ, ブタと自分の距離がdisOfNoticePig以下
				//std::cout << position.x << "    " << position.z << "\n";
				obj_xspeed = (double)(dif_x / disFromPig * obj_speed);
				obj_zspeed = (double)(dif_z / disFromPig * obj_speed);

				decideVolumeOfPigCry(disFromPig2);

				pigx[i] += obj_xspeed;
				if (objhit2(pigx[i], pigz[i], cube) == true) {
					pigx[i] -= obj_xspeed;
				}
				pigz[i] += obj_zspeed;
				if (objhit2(pigx[i], pigz[i], cube) == true) {
					pigz[i] -= obj_zspeed;

				}
				pigNoticeFlag[i] = true;
			}
			else if (pigflag[i] == 0) {
				pointx = px[i] - pigx[i];
				pointz = pz[i] - pigz[i];
				dispoint = sqrt(pointx * pointx + pointz * pointz);
				obj_xspeed = (double)(pointx / dispoint * obj_speed);
				obj_zspeed = (double)(pointz / dispoint * obj_speed);
				pigx[i] += obj_xspeed;
				pigz[i] += obj_zspeed;
				pigy[i] = -1.0;
				if (objhit2(pigx[i], pigz[i], cube) == true) {
					pigy[i] = 3;
				}

				if ((pigx[i] + 0.05 > px[i] && pigx[i] - 0.05 < px[i]) &&
					(pigz[i] + 0.05 > pz[i] && pigz[i] - 0.05 < pz[i])) {
					pigflag[i] = 1;
				}
				pigNoticeFlag[i] = false;
			}
			else if (pigflag[i] == 1) {
				pointx = s_px[i] - pigx[i];
				pointz = s_pz[i] - pigz[i];
				dispoint = sqrt(pointx * pointx + pointz * pointz);
				obj_xspeed = (double)(pointx / dispoint * obj_speed);
				obj_zspeed = (double)(pointz / dispoint * obj_speed);
				pigx[i] += obj_xspeed;
				pigz[i] += obj_zspeed;
				pigy[i] = -1.0;
				if (objhit2(pigx[i], pigz[i], cube) == true) {
					pigy[i] = 3;
				}

				if ((pigx[i] + 0.05 > s_px[i] && pigx[i] - 0.05 < s_px[i]) &&
					(pigz[i] + 0.05 > s_pz[i] && pigz[i] - 0.05 < s_pz[i])) {
					pigflag[i] = 0;
				}
				pigNoticeFlag[i] = false;
			}
		}
		PigRolling(obj_xspeed, obj_zspeed);
		//glRotatef(-90.0f, 0.0f, 1.0f,0.0f);
		OBJFile[7].Draw();
		glPopMatrix();

		//豚の攻撃判定
		if (disFromPig < 2) {
			GameOver();
			//ModeSelect = 2;
			//sound_check = 2;
			skycheck = false;
			//std::cout << "Mode = " << ModeSelect << std::endl;
			//std::cout << "sound = " << sound_check << std::endl;
		}

		//std::cout << dif_x <<" "<< dif_z << std::endl;
		//std::cout << horizontalAngle << std::endl;

		//アラートの表示

		glEnable(GL_TEXTURE_2D);
		int houkou[2] = {};
		int cameraAngle;
		float pi = 3.1415;
		float onedegree = pi / 180.0f; //1°のラジアン
		float rolldegree = horizontalAngle / onedegree;
		if (rolldegree > 45) {
			rolldegree += 45;
		}
		if (rolldegree < 45) {
			rolldegree -= 45;
		}
		cameraAngle = (rolldegree) / 90;
		//std::cout << cameraAngle << std::endl;

		if (disFromPig < disOfNoticePig + 5) {

			if (dif_x < 0) {
				houkou[0] = -1;
			}
			else {
				houkou[0] = 1;
			}
			if (dif_z < 0) {
				houkou[1] = -1;
			}
			else {
				houkou[1] = 1;
			}
			if (rolldegree < 0) {
				if (houkou[0] == -1 && houkou[1] == -1 && abs(dif_x) < abs(dif_z)) {
					// glBindTexture(GL_TEXTURE_2D, Texture[14].ID);//下
					if (abs(cameraAngle) % 4 == 0) {
						glBindTexture(GL_TEXTURE_2D, Texture[15].ID);//上
					}
					else if (abs(cameraAngle) % 4 == 1) {
						glBindTexture(GL_TEXTURE_2D, Texture[16].ID);//左
					}
					else if (abs(cameraAngle) % 4 == 2) {
						glBindTexture(GL_TEXTURE_2D, Texture[14].ID);//下
					}
					else if (abs(cameraAngle) % 4 == 3) {
						glBindTexture(GL_TEXTURE_2D, Texture[17].ID);//右
					}
				}
				else if (houkou[0] == -1 && houkou[1] == -1 && abs(dif_x) >= abs(dif_z)) {
					//glBindTexture(GL_TEXTURE_2D, Texture[17].ID);//右
					if (abs(cameraAngle) % 4 == 0) {
						glBindTexture(GL_TEXTURE_2D, Texture[16].ID);//左
					}
					else if (abs(cameraAngle) % 4 == 1) {
						glBindTexture(GL_TEXTURE_2D, Texture[14].ID);//下
					}
					else if (abs(cameraAngle) % 4 == 2) {
						glBindTexture(GL_TEXTURE_2D, Texture[17].ID);//右
					}
					else if (abs(cameraAngle) % 4 == 3) {
						glBindTexture(GL_TEXTURE_2D, Texture[15].ID);//上
					}
				}

				if (houkou[0] == 1 && houkou[1] == -1 && abs(dif_x) < abs(dif_z)) {
					//glBindTexture(GL_TEXTURE_2D, Texture[14].ID);//下
					if (abs(cameraAngle) % 4 == 0) {
						glBindTexture(GL_TEXTURE_2D, Texture[15].ID);//上
					}
					else if (abs(cameraAngle) % 4 == 1) {
						glBindTexture(GL_TEXTURE_2D, Texture[16].ID);//左
					}
					else if (abs(cameraAngle) % 4 == 2) {
						glBindTexture(GL_TEXTURE_2D, Texture[14].ID);//下
					}
					else if (abs(cameraAngle) % 4 == 3) {
						glBindTexture(GL_TEXTURE_2D, Texture[17].ID);//右
					}
				}
				else if (houkou[0] == 1 && houkou[1] == -1 && abs(dif_x) >= abs(dif_z)) {
					//glBindTexture(GL_TEXTURE_2D, Texture[16].ID);//左
					if (abs(cameraAngle) % 4 == 0) {
						glBindTexture(GL_TEXTURE_2D, Texture[17].ID);//右
					}
					else if (abs(cameraAngle) % 4 == 1) {
						glBindTexture(GL_TEXTURE_2D, Texture[15].ID);//上
					}
					else if (abs(cameraAngle) % 4 == 2) {
						glBindTexture(GL_TEXTURE_2D, Texture[16].ID);//左
					}
					else if (abs(cameraAngle) % 4 == 3) {
						glBindTexture(GL_TEXTURE_2D, Texture[14].ID);//下
					}
				}

				if (houkou[0] == -1 && houkou[1] == 1 && abs(dif_x) < abs(dif_z)) {
					//glBindTexture(GL_TEXTURE_2D, Texture[15].ID);//上
					if (abs(cameraAngle) % 4 == 0) {
						glBindTexture(GL_TEXTURE_2D, Texture[14].ID);//下
					}
					else if (abs(cameraAngle) % 4 == 1) {
						glBindTexture(GL_TEXTURE_2D, Texture[17].ID);//右
					}
					else if (abs(cameraAngle) % 4 == 2) {
						glBindTexture(GL_TEXTURE_2D, Texture[15].ID);//上
					}
					else if (abs(cameraAngle) % 4 == 3) {
						glBindTexture(GL_TEXTURE_2D, Texture[16].ID);//左
					}

				}
				else if (houkou[0] == -1 && houkou[1] == 1 && abs(dif_x) >= abs(dif_z)) {
					//glBindTexture(GL_TEXTURE_2D, Texture[17].ID);//右
					if (abs(cameraAngle) % 4 == 0) {
						glBindTexture(GL_TEXTURE_2D, Texture[16].ID);//左
					}
					else if (abs(cameraAngle) % 4 == 1) {
						glBindTexture(GL_TEXTURE_2D, Texture[14].ID);//下
					}
					else if (abs(cameraAngle) % 4 == 2) {
						glBindTexture(GL_TEXTURE_2D, Texture[17].ID);//右
					}
					else if (abs(cameraAngle) % 4 == 3) {
						glBindTexture(GL_TEXTURE_2D, Texture[15].ID);//上
					}
				}

				if (houkou[0] == 1 && houkou[1] == 1 && abs(dif_x) < abs(dif_z)) {
					//glBindTexture(GL_TEXTURE_2D, Texture[15].ID);//上
					if (abs(cameraAngle) % 4 == 0) {
						glBindTexture(GL_TEXTURE_2D, Texture[14].ID);//下
					}
					else if (abs(cameraAngle) % 4 == 1) {
						glBindTexture(GL_TEXTURE_2D, Texture[17].ID);//右
					}
					else if (abs(cameraAngle) % 4 == 2) {
						glBindTexture(GL_TEXTURE_2D, Texture[15].ID);//上
					}
					else if (abs(cameraAngle) % 4 == 3) {
						glBindTexture(GL_TEXTURE_2D, Texture[16].ID);//左
					}
				}
				else if (houkou[0] == 1 && houkou[1] == 1 && abs(dif_x) >= abs(dif_z)) {
					//glBindTexture(GL_TEXTURE_2D, Texture[16].ID);//左
					if (abs(cameraAngle) % 4 == 0) {
						glBindTexture(GL_TEXTURE_2D, Texture[17].ID);//右
					}
					else if (abs(cameraAngle) % 4 == 1) {
						glBindTexture(GL_TEXTURE_2D, Texture[15].ID);//上
					}
					else if (abs(cameraAngle) % 4 == 2) {
						glBindTexture(GL_TEXTURE_2D, Texture[16].ID);//左
					}
					else if (abs(cameraAngle) % 4 == 3) {
						glBindTexture(GL_TEXTURE_2D, Texture[14].ID);//下
					}
				}
			}
			else if (rolldegree >= 0) {
				if (houkou[0] == -1 && houkou[1] == -1 && abs(dif_x) < abs(dif_z)) {
					// glBindTexture(GL_TEXTURE_2D, Texture[14].ID);//下
					if (abs(cameraAngle) % 4 == 0) {
						glBindTexture(GL_TEXTURE_2D, Texture[14].ID);//下
					}
					else if (abs(cameraAngle) % 4 == 1) {
						glBindTexture(GL_TEXTURE_2D, Texture[16].ID);//左
					}
					else if (abs(cameraAngle) % 4 == 2) {
						glBindTexture(GL_TEXTURE_2D, Texture[15].ID);//上
					}
					else if (abs(cameraAngle) % 4 == 3) {
						glBindTexture(GL_TEXTURE_2D, Texture[17].ID);//右
					}
				}
				else if (houkou[0] == -1 && houkou[1] == -1 && abs(dif_x) >= abs(dif_z)) {
					//glBindTexture(GL_TEXTURE_2D, Texture[17].ID);//右
					if (abs(cameraAngle) % 4 == 0) {
						glBindTexture(GL_TEXTURE_2D, Texture[16].ID);//左
					}
					else if (abs(cameraAngle) % 4 == 1) {
						glBindTexture(GL_TEXTURE_2D, Texture[15].ID);//上
					}
					else if (abs(cameraAngle) % 4 == 2) {
						glBindTexture(GL_TEXTURE_2D, Texture[17].ID);//右
					}
					else if (abs(cameraAngle) % 4 == 3) {
						glBindTexture(GL_TEXTURE_2D, Texture[14].ID);//下
					}
				}

				if (houkou[0] == 1 && houkou[1] == -1 && abs(dif_x) < abs(dif_z)) {
					//glBindTexture(GL_TEXTURE_2D, Texture[14].ID);//下
					if (abs(cameraAngle) % 4 == 0) {
						glBindTexture(GL_TEXTURE_2D, Texture[14].ID);//下
					}
					else if (abs(cameraAngle) % 4 == 1) {
						glBindTexture(GL_TEXTURE_2D, Texture[16].ID);//左
					}
					else if (abs(cameraAngle) % 4 == 2) {
						glBindTexture(GL_TEXTURE_2D, Texture[15].ID);//上
					}
					else if (abs(cameraAngle) % 4 == 3) {
						glBindTexture(GL_TEXTURE_2D, Texture[17].ID);//右
					}
				}
				else if (houkou[0] == 1 && houkou[1] == -1 && abs(dif_x) >= abs(dif_z)) {
					//glBindTexture(GL_TEXTURE_2D, Texture[16].ID);//左
					if (abs(cameraAngle) % 4 == 0) {
						glBindTexture(GL_TEXTURE_2D, Texture[17].ID);//右
					}
					else if (abs(cameraAngle) % 4 == 1) {
						glBindTexture(GL_TEXTURE_2D, Texture[14].ID);//下
					}
					else if (abs(cameraAngle) % 4 == 2) {
						glBindTexture(GL_TEXTURE_2D, Texture[16].ID);//左
					}
					else if (abs(cameraAngle) % 4 == 3) {
						glBindTexture(GL_TEXTURE_2D, Texture[15].ID);//上
					}
				}

				if (houkou[0] == -1 && houkou[1] == 1 && abs(dif_x) < abs(dif_z)) {
					//glBindTexture(GL_TEXTURE_2D, Texture[15].ID);//上
					if (abs(cameraAngle) % 4 == 0) {
						glBindTexture(GL_TEXTURE_2D, Texture[15].ID);//上
					}
					else if (abs(cameraAngle) % 4 == 1) {
						glBindTexture(GL_TEXTURE_2D, Texture[17].ID);//右
					}
					else if (abs(cameraAngle) % 4 == 2) {
						glBindTexture(GL_TEXTURE_2D, Texture[14].ID);//下
					}
					else if (abs(cameraAngle) % 4 == 3) {
						glBindTexture(GL_TEXTURE_2D, Texture[16].ID);//左
					}

				}
				else if (houkou[0] == -1 && houkou[1] == 1 && abs(dif_x) >= abs(dif_z)) {
					//glBindTexture(GL_TEXTURE_2D, Texture[17].ID);//右
					if (abs(cameraAngle) % 4 == 0) {
						glBindTexture(GL_TEXTURE_2D, Texture[16].ID);//左
					}
					else if (abs(cameraAngle) % 4 == 1) {
						glBindTexture(GL_TEXTURE_2D, Texture[15].ID);//上
					}
					else if (abs(cameraAngle) % 4 == 2) {
						glBindTexture(GL_TEXTURE_2D, Texture[17].ID);//右
					}
					else if (abs(cameraAngle) % 4 == 3) {
						glBindTexture(GL_TEXTURE_2D, Texture[14].ID);//下
					}
				}

				if (houkou[0] == 1 && houkou[1] == 1 && abs(dif_x) < abs(dif_z)) {
					//glBindTexture(GL_TEXTURE_2D, Texture[15].ID);//上
					if (abs(cameraAngle) % 4 == 0) {
						glBindTexture(GL_TEXTURE_2D, Texture[15].ID);//上
					}
					else if (abs(cameraAngle) % 4 == 1) {
						glBindTexture(GL_TEXTURE_2D, Texture[17].ID);//右
					}
					else if (abs(cameraAngle) % 4 == 2) {
						glBindTexture(GL_TEXTURE_2D, Texture[14].ID);//下
					}
					else if (abs(cameraAngle) % 4 == 3) {
						glBindTexture(GL_TEXTURE_2D, Texture[16].ID);//左
					}
				}
				else if (houkou[0] == 1 && houkou[1] == 1 && abs(dif_x) >= abs(dif_z)) {
					//glBindTexture(GL_TEXTURE_2D, Texture[16].ID);//左
					if (abs(cameraAngle) % 4 == 0) {
						glBindTexture(GL_TEXTURE_2D, Texture[17].ID);//右
					}
					else if (abs(cameraAngle) % 4 == 1) {
						glBindTexture(GL_TEXTURE_2D, Texture[14].ID);//下
					}
					else if (abs(cameraAngle) % 4 == 2) {
						glBindTexture(GL_TEXTURE_2D, Texture[16].ID);//左
					}
					else if (abs(cameraAngle) % 4 == 3) {
						glBindTexture(GL_TEXTURE_2D, Texture[15].ID);//上
					}
				}
			}

			glPushMatrix();
			ObjMoveRoll_Camera(horizontalAngle, verticalAngle, 2);
			//glTranslatef(-11.0, 1.05, -15.6);
			//glRotatef(90.0f,0.0f,1.0f,0.0f);
			OBJFile[10].Draw();
			glPopMatrix();
		}
	}
}

//--------------------------------------------------------------------------------------------------------
// りんご
//--------------------------------------------------------------------------------------------------------
void Apple() {
	if (appleFlag[0] == true) {
		//apple1
		// テクスチャマッピング有効化 (追加 bmp)
		glEnable(GL_TEXTURE_2D);
		//　テクスチャをバインド (追加 bmp)
		glBindTexture(GL_TEXTURE_2D, Texture[8].ID);
		glPushMatrix();
		glTranslatef(28.0, 0.5, 26.0);
		OBJFile[8].Draw();
		glPopMatrix();
	}

	if (appleFlag[1] == true) {
		//apple2
	// テクスチャマッピング有効化 (追加 bmp)
		glEnable(GL_TEXTURE_2D);
		//　テクスチャをバインド (追加 bmp)
		glBindTexture(GL_TEXTURE_2D, Texture[8].ID);
		glPushMatrix();
		glTranslatef(14.0, 0.5, -27.5);
		OBJFile[8].Draw();
		glPopMatrix();
	}

	if (appleFlag[2] == true) {
		//apple3
	// テクスチャマッピング有効化 (追加 bmp)
		glEnable(GL_TEXTURE_2D);
		//　テクスチャをバインド (追加 bmp)
		glBindTexture(GL_TEXTURE_2D, Texture[8].ID);
		glPushMatrix();
		glTranslatef(5.0, 0.5, -27.5);
		OBJFile[8].Draw();
		glPopMatrix();
	}

	if (appleFlag[3] == true) {
		//apple4
	// テクスチャマッピング有効化 (追加 bmp)
		glEnable(GL_TEXTURE_2D);
		//　テクスチャをバインド (追加 bmp)
		glBindTexture(GL_TEXTURE_2D, Texture[8].ID);
		glPushMatrix();
		glTranslatef(-18.5, 0.5, 0.0);
		OBJFile[8].Draw();
		glPopMatrix();
	}
}

//--------------------------------------------------------------------------------------------------------
// 空
//--------------------------------------------------------------------------------------------------------
void Sky() {
	//sky
	// テクスチャマッピング有効化 (追加 bmp)
	glEnable(GL_TEXTURE_2D);
	//　テクスチャをバインド (追加 bmp)
	glBindTexture(GL_TEXTURE_2D, Texture[9].ID);
	glPushMatrix();
	OBJFile[9].Draw();
	glPopMatrix();

	//skywal
// テクスチャマッピング有効化 (追加 bmp)
	//glEnable(GL_TEXTURE_2D);
	//　テクスチャをバインド (追加 bmp)
	//glBindTexture(GL_TEXTURE_2D, Texture[9].ID);
	//glPushMatrix();
	//OBJFile[11].Draw();
	//glPopMatrix();
}

//--------------------------------------------------------------------------------------------------------
// リンゴが近くにあると!がでる
//--------------------------------------------------------------------------------------------------------
void AppleCheck() {
	glEnable(GL_TEXTURE_2D);										// テクスチャマッピング有効化 (追加 bmp)

	bool  ACflag = false;
	//リンゴのあたり判定
	if (abs(position.x - 28) < 11 && abs(position.z - (26)) < 11) {
		if (appleFlag[0]) {
			glBindTexture(GL_TEXTURE_2D, Texture[27].ID);				// テクスチャをバインド (追加 bmp)
			ACflag = true;
		}

	}
	if (abs(position.x - 12.0) < 11 && abs(position.z - (-27.5)) < 11) {
		if (appleFlag[1]) {
			glBindTexture(GL_TEXTURE_2D, Texture[27].ID);				// テクスチャをバインド (追加 bmp)
			ACflag = true;
		}
		//ACflag = true;
	}
	if (abs(position.x - 3.0) < 11 && abs(position.z - (-27.5)) < 11) {
		if (appleFlag[2]) {
			glBindTexture(GL_TEXTURE_2D, Texture[27].ID);				// テクスチャをバインド (追加 bmp)
			ACflag = true;
		}
		//ACflag = true;
	}
	if (abs(position.x - (-18.5)) < 11 && abs(position.z - 0) < 11) {
		if (appleFlag[3]) {
			glBindTexture(GL_TEXTURE_2D, Texture[27].ID);				// テクスチャをバインド (追加 bmp)
			ACflag = true;
		}
		//ACflag = true;
	}
	if (ACflag == false) {
		glBindTexture(GL_TEXTURE_2D, Texture[4].ID);				// テクスチャをバインド (追加 bmp)
	}
	glPushMatrix();
	ObjMoveRoll_Camera(horizontalAngle, verticalAngle, 3);
	//glTranslatef(-11.0, 1.05, -15.6);
	//glRotatef(90.0f,0.0f,1.0f,0.0f);
	OBJFile[10].Draw();
	glPopMatrix();
}

//--------------------------------------------------------------------------------------------------------
// ゲームオーバー. ブタに捕まる
//--------------------------------------------------------------------------------------------------------
void GameOver() {
	endflag.gameoverFlag = true;
	allSeStop();
	soundStop(&ssePig);
	if (!gameEnd) {
		soundPlay(seGameover, &sseGameover, false, volume_gameover);
		gameEnd = true;
	}
	gameover.SetColor(0.0f, 0.0f, 0.0f, 0.4f);
	gameover.SetSpeed(1.0f);
	gameover.SetPulse(false);
	gameover.SetType(GAMEOVER);
	gameover.Render(0.0f);
	skycheck = false;
	timeflag = false;
}

//--------------------------------------------------------------------------------------------------------
// ゲームクリア. りんごを4つ取る.
//--------------------------------------------------------------------------------------------------------
void GameClear() {
	endflag.gameclearFlag = true;
	allSeStop();
	soundStop(&ssePig);
	if (!gameEnd) {
		soundPlay(seGameclear, &sseGameclear, false, volume_gameclear);
		gameEnd = true;
	}
	gameclear.SetColor(0.0f, 0.0f, 0.0f, 0.4f);
	gameclear.SetSpeed(1.0f);
	gameclear.SetPulse(false);
	gameclear.SetType(GAMECLEAR);
	gameclear.Render(0.0f);
	//skycheck = false;
	timeflag = false;
}

//--------------------------------------------------------------------------------------------------------
// タイムアップ. 時間切れ
//--------------------------------------------------------------------------------------------------------
void TimeUp() {
	endflag.timeupFlag = true;
	allSeStop();
	soundStop(&ssePig);
	if (!gameEnd) {
		soundPlay(seTimeup, &sseTimeup, false, volume_timeup);
		gameEnd = true;
	}
	gameclear.SetColor(0.0f, 0.0f, 0.0f, 0.4f);
	gameclear.SetSpeed(1.0f);
	gameclear.SetPulse(false);
	gameclear.SetType(TIMEUP);
	gameclear.Render(0.0f);
	skycheck = false;
	timeflag = false;
}

//---------------------------------------------------------------------------------------------------
// 指定したテクスチャを画面いっぱいに表示させる.
//---------------------------------------------------------------------------------------------------
void Pulse::Render(float fTime)
{
	static float phase = 0.0f;
	float r = 0.0f, g = 0.0f, b = 0.0f;

	glPushAttrib(GL_ENABLE_BIT | GL_DEPTH_BUFFER_BIT | GL_CURRENT_BIT | GL_LIGHTING_BIT | GL_TRANSFORM_BIT | GL_TEXTURE_BIT);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	if (m_bPulse)	phase = 2.0*(1.0f + (float)sin(fTime * m_fSpeed));
	else phase = (float)fmod(fTime * m_fSpeed, 4.0f);

	r = m_fColor[0] - ((m_fColor[0] / 3.0f) * phase);
	g = m_fColor[1] - ((m_fColor[1] / 3.0f) * phase);
	b = m_fColor[2] - ((m_fColor[2] / 3.0f) * phase);
	m_fColor[3] = m_fColor[3] - ((m_fColor[3] / 3.0f) * phase);
	glColor4f(r, g, b, m_fColor[3]);
	//cout << type << endl;

	if (type == GAMEOVER || type == GAMECLEAR || type == TIMEUP) {
		glEnable(GL_TEXTURE_2D);
		if (type == GAMEOVER) {
			glBindTexture(GL_TEXTURE_2D, 29);
		}
		if (type == GAMECLEAR) {
			glBindTexture(GL_TEXTURE_2D, 30);
		}
		if (type == TIMEUP) {
			glBindTexture(GL_TEXTURE_2D, 31);
		}
		//GLfloat color2[4] = { r, g, b, m_fColor[3] };
		//glColor4fv(color2);
		//glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color);
		//glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);
		//glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, color);
	}

	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(-1.0f, -1.0f, 0.0f);
	glTexCoord2f(1.0, 0.0); glVertex3f(1.0f, -1.0f, 0.0f);
	glTexCoord2f(1.0, 1.0); glVertex3f(1.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0, 1.0); glVertex3f(-1.0f, 1.0f, 0.0f);
	glEnd();

	if (type == GAMEOVER || type == GAMECLEAR || type == TIMEUP) {
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
	}

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glDisable(GL_BLEND);
	glPopAttrib();

}



//////////////////////////////////////////////////////////////////
// 音声関係
/////////////////////////////////////////////////////////////////
//----------------------------------------------------
// BGM/SEの初期化
//----------------------------------------------------
void InitMusic() {

	sound_check = 0;	//画面遷移時にどのBGMを再生するのかを決定する変数
	//se_pig_check = 0;	//ブタの鳴き声の初めの１回を正確に再生するため.
	se_pig_check = false;

	//---alutの機能の初期化(BGM)---//
	bgmStart = alutCreateBufferFromFile("BGM/Start.wav");
	bgmPlay = alutCreateBufferFromFile("BGM/Play2.wav");
	bgmResult = alutCreateBufferFromFile("BGM/Result1.wav");
	//bgmResult = alutCreateBufferFromFile("BGM/Result2.wav");


	//---alutの機能の初期化(SE)---//
	seCollisionWall = alutCreateBufferFromFile("SE/CollisionWall.wav");
	sePig = alutCreateBufferFromFile("SE/Pig_girl.wav");
	//sePig = alutCreateBufferFromFile("SE/Pig2.wav");
	//sePig = alutCreateBufferFromFile("SE/Pig3.wav");
	seGetItem = alutCreateBufferFromFile("SE/GetItem.wav");
	seGameover = alutCreateBufferFromFile("SE/gameover.wav");
	seGameclear = alutCreateBufferFromFile("SE/gameclear.wav");
	seTimeup = alutCreateBufferFromFile("SE/timeup.wav");

	volume_BGM = 0.8f;			//BGMの音量
	volume_PIG = 1.0f;			//ブタの鳴き声の音量
	volume_WALL = 1.0f;			//壁に衝突したときの音量
	volume_GetItem = 1.0f;		//アイテムを取得したときの音量
	volume_gameover = 1.0f;		//ブタに当たってゲームオーバーになるときの音量
	volume_gameclear = 1.0f;	//りんごを4つ取ってゲームクリアになるときの音量
	volume_timeup = 1.0f;		//制限時間切れになったときの音量


	//---1回音ならせばブタの鳴き声にもかき消されないことがわかったので初期化のタイミングでvolume0でならす---//
	//soundPlay(seCollisionWall, &sseCollisionWall, false, 0.0f);
	//soundPlay(seGetItem, &sseGetItem, false, 0.0f);
	//soundPlay(seGameover, &sseGameover, false, 0.0f);
	//soundPlay(seGameclear, &sseGameclear, false, 0.0f);
	//soundPlay(seTimeup, &sseTimeup, false, 0.0f);
}

//----------------------------------------------------
// 音楽を再生
//----------------------------------------------------
void soundPlay(ALuint buf, ALuint *source, bool loop, float volume) {
	bool music_dbg = false;
	//std::cout << "\nsoundPlay : " << source << std::endl << std::endl;
	ALenum error;
	alGetError();
	alGenSources(1, source);								// 音データのソースを生成
	if (music_dbg) {
		std::cout << "[1]";
		if ((error = alGetError()) != AL_NO_ERROR) {
			//std::cout << "source = " << &source << std::endl;
			noticeError(error);
		}
		else {
			std::cout << "最新のエラーは存在しません。\n";
		}
	}
	ALint state;
	alGetSourcei(*source, AL_SOURCE_STATE, &state);			// 音声データのステータスを確認する
	if (loop) {												// 再生する音声がLOOP再生するBGMならば
		//alGenSources(1, source);							// 音データのソースを生成
		alSourcef(*source, AL_GAIN, volume);				// 音量
		alSourcei(*source, AL_BUFFER, buf);					// 音データのバッファを指定
		alSourcei(*source, AL_LOOPING, AL_TRUE);			// ループの設定
		alSourcePlay(*source);								// 音の再生
	}
	else {													// 再生する音声がSEならば
		//std::cout << getOffset(*source) << '\n';
		//tmp++;
		//ALint state;
		//alGetSourcei(*source, AL_SOURCE_STATE, &state);
		//if (buf == seCollisionWall) {
		//std::cout << state << '\n';
		//if (state == AL_INITIAL) {
		//	//std::cout << "INITIAL\n";
		//}
		//else if (state == AL_PLAYING) {
		//	//std::cout << "PLAYING\n";
		//}
		//else if (state == AL_STOPPED) {
		//	//std::cout << "STOP\n";
		//}
		//else if (state == AL_PAUSED) {
		//	//std::cout << "PAUSE\n";
		//}
		//else {
		//	//std::cout << state << '\n';
		//}
		//}
		if (state == AL_INITIAL) {
			//std::cout << "PLAY1\n";
			if (buf == sePig) volume = 0;
			alGetError();
			alSourcef(*source, AL_GAIN, volume);			// 音量
			alSourcei(*source, AL_BUFFER, buf);				// 音データのバッファを指定
			if (music_dbg) {
				std::cout << "[2]";
				if ((error = alGetError()) != AL_NO_ERROR) {
					noticeError(error);
				}
				else {
					std::cout << "最新のエラーは存在しません。\n";
				}
			}
			alSourcePlay(*source);							// SEを再生する//std::cout << "PLAY1\n";
		}
		else {
			if ((state != AL_PLAYING) && (state != AL_INITIAL)) {		// 再生されていなければ
				//std::cout << "PLAY2" << std::endl;
				//if (getOffset(*source) == 0) {						// 再生されていなければ
					//soundStop(source);								// 一度不要なバッファを削除してから
				alDeleteBuffers(1, source);
				//volume_PIG = 0.0f;
				//std::cout << "volume=" << volume << std::endl;
				alGetError();
				alGenSources(1, source);						// 音データのソースを生成
				alSourcef(*source, AL_GAIN, volume);			// 音量
				alSourcei(*source, AL_BUFFER, buf);				// 音データのバッファを指定
				if (music_dbg) {
					std::cout << "[4]";
					if ((error = alGetError()) != AL_NO_ERROR) {
						noticeError(error);
					}
					else {
						std::cout << "最新のエラーは存在しません。\n";
					}
				}
				alSourcePlay(*source);							// SEを再生する
				//alDeleteSources(1, source);							// 音データのバッファの削除
			}
			else {
				//std::cout << "PLAY3\n";
			}
			//else if (getDuration(buf) <= getOffset(*source)-0.1) {	// 音声が再生終了していれば(再生中でなければ)
			//	soundStop(source);								// 一度不要なバッファを削除してから
			//	alGenSources(1, source);								// 音データのソースを生成
			//	alSourcef(*source, AL_GAIN, volume);			// 音量
			//	alSourcei(*source, AL_BUFFER, buf);				// 音データのバッファを指定
			//	alSourcePlay(*source);							// 改めてSEを再生する
			//	std::cout << "PLAY2\n";
			//}
		}
	}
	//std::cout << "soundPlay" << buf << "," << source << std::endl;
	//return state;
}

//----------------------------------------------------
// 音楽を停止
//----------------------------------------------------
void soundStop(ALuint *source) {
	//std::cout << "soundStop : " << source << std::endl;	
	alSourceStop(*source);								// 音の停止
	alDeleteSources(1, source);							// 音データのバッファの削除
	source = 0;											// 音データの初期化
}

//----------------------------------------------------
// すべての音楽を停止
//----------------------------------------------------
void allSoundStop() {
	soundStop(&ssePig);
	soundStop(&sseGetItem);
	soundStop(&sseCollisionWall);
	soundStop(&sbgmStart);
	soundStop(&sbgmPlay);
	soundStop(&sbgmResult);
	//std::cout << '\n';
}

//----------------------------------------------------
// すべてのSEを停止
//----------------------------------------------------
void allSeStop() {
	//soundStop(&ssePig);
	soundStop(&sseGetItem);
	soundStop(&sseCollisionWall);
	//std::cout << '\n';

	//---唐突に音が消えることが多すぎるためごり押し---//
	//seCollisionWall = alutCreateBufferFromFile("SE/CollisionWall.wav");
	////sePig = alutCreateBufferFromFile("SE/Pig3.wav");
	//sePig = alutCreateBufferFromFile("SE/Pig2.wav");
	//seGetItem = alutCreateBufferFromFile("SE/GetItem.wav");
	//seGameover = alutCreateBufferFromFile("SE/gameover.wav");
	//seGameclear = alutCreateBufferFromFile("SE/gameclear.wav");
	//seTimeup = alutCreateBufferFromFile("SE/timeup.wav");
}

//----------------------------------------------------
// すべての音楽バッファを削除
//----------------------------------------------------
void soundAllDelete() {
	soundStop(&ssePig);
	soundStop(&sseGetItem);
	soundStop(&sseCollisionWall);
	soundStop(&sbgmStart);
	soundStop(&sbgmPlay);
	soundStop(&sbgmResult);
}

//--------------------------------------------------------------------------------------------------------
//  音声データの尺を秒単位で取得
//--------------------------------------------------------------------------------------------------------
float getDuration(ALuint buf) {
	ALint size, frequency, channels, bits;
	alGetBufferi(buf, AL_SIZE, &size);
	alGetBufferi(buf, AL_FREQUENCY, &frequency);
	alGetBufferi(buf, AL_CHANNELS, &channels);
	alGetBufferi(buf, AL_BITS, &bits);

	return (float)size / (float)(frequency*channels*(bits / 8));
}

//--------------------------------------------------------------------------------------------------------
//  音声データの尺を秒単位で取得
//--------------------------------------------------------------------------------------------------------
float getOffset(ALuint source) {
	float offset;
	alGetSourcef(source, AL_SEC_OFFSET, &offset);

	return offset;
}

//--------------------------------------------------------------------------------------------------------
//  エラー内容の表示
//--------------------------------------------------------------------------------------------------------
void noticeError(ALenum error) {
	if (error == AL_INVALID_NAME) {
		std::cout << "不正なネームの引数です。\n";
	}
	else if (error == AL_INVALID_ENUM) {
		std::cout << "不正な列挙値の引数です。\n";
	}
	else if (error == AL_INVALID_VALUE) {
		std::cout << "不正な引数です\n";
	}
	else if (error == AL_INVALID_OPERATION) {
		std::cout << "禁止されている呼び出しです。\n";
	}
	else if (error == AL_OUT_OF_MEMORY) {
		std::cout << "メモリを割り当てる事が出来ません。\n";
	}
	else {
		std::cout << "原因不明です。\n";
	}
}


//--------------------------------------------------------------------------------------------------------
// 壁との当たり判定を行うための壁の端点座標を決定する関数
//--------------------------------------------------------------------------------------------------------
void decideCube(std::vector<glm::vec3> &vertices) {
	double x_max = 0, x_min = 0, z_max = 0, z_min = 0;
	double tmp;
	int ik = 0, il = 0;
	int k = 1, l = 1;
	double size = 0.15;	//壁に仮想的な厚みを付ける
	//double size = 5;	//壁に仮想的な厚みを付ける

	while (1) {
		if (ik % 36 == 0) {
			tmp = vertices[ik].x;
			ik++;
		}
		while (tmp == vertices[ik].x) {
			ik++;
		}
		if (tmp > vertices[ik].x) {
			x_max = tmp;
			x_min = vertices[ik].x;
		}
		else {
			x_max = vertices[ik].x;
			x_min = tmp;
		}
		//cube[k - 1][0] = x_max;
		//cube[k - 1][1] = x_min;
		cube[k - 1][0] = x_max + size;
		cube[k - 1][1] = x_min - size;
		ik = k * 36;
		k++;
		if (k == 29) { break; }
	}
	while (1) {
		if (il % 36 == 0) {
			tmp = vertices[il].z;
			il++;
		}
		while (tmp == vertices[il].z) {
			il++;
		}
		if (tmp > vertices[il].z) {
			z_max = tmp;
			z_min = vertices[il].z;
		}
		else {
			z_max = vertices[il].z;
			z_min = tmp;
		}
		cube[l - 1][2] = z_max + size;
		cube[l - 1][3] = z_min - size;
		il = l * 36;
		l++;
		if (l == 29) { break; }
	}




	//std::cout << "\n\n[decideCube]\n";
	//std::cout << "(k, l) = (" << k << ", " << l << ")\n";
	//std::cout << "vertices.size/36 = " << (int)(vertices.size()) / 36 << std::endl;
	//std::cout << std::endl;
}

//--------------------------------------------------------------------------------------------------------
// 壁に衝突したときのSEを制御するための多段ヒット検知
//--------------------------------------------------------------------------------------------------------
bool chackDoubleSpecialKeyPush() {
	bool flag = false;
	if ((objflag.hitUP    && objflag.keyRIGHT) || (objflag.hitUP    && objflag.keyLEFT) ||
		(objflag.hitDOWN  && objflag.keyRIGHT) || (objflag.hitDOWN  && objflag.keyLEFT) ||
		(objflag.hitRIGHT && objflag.keyUP) || (objflag.hitRIGHT && objflag.keyDOWN) ||
		(objflag.hitLEFT  && objflag.keyUP) || (objflag.hitLEFT  && objflag.keyDOWN)
		) {
		flag = true;
	}
	return flag;
}

//--------------------------------------------------------------------------------------------------------
// ブタと自分との距離に応じて鳴き声の大きさを更新する
//--------------------------------------------------------------------------------------------------------
void decideVolumeOfPigCry(double disFromPig) {
	if (disFromPig >= disOfNoticePig) {
		volume_PIG = 0.0f;
	}
	else {
		//volume_PIG = 1.0f / ((float)disFromPig) + 0.2f;
		volume_PIG = 1.5f - ((float)disFromPig2) / 10.0;
		soundPlay(sePig, &ssePig, false, volume_PIG);
	}
	//std::cout << "volume = " << volume_PIG << std::endl;
}









//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// オブジェクトの読み込みなどのブラックボックス化された関数たち
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


////////////////////////////////////////////////////////////////////////
//  Pluse class
////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------
// Name : Pulse()
// Desc : コンストラクタ#1
//---------------------------------------------------------------------------------------------------
Pulse::Pulse()
{
	m_fColor[0] = 1.0f;
	m_fColor[1] = 1.0f;
	m_fColor[2] = 1.0f;
	m_fColor[3] = 1.0f;
	m_fSpeed = 1.0f;
	m_bPulse = false;
}

//--------------------------------------------------------------------------------------------------
// Name : Pulse()
// Desc : コンストラクタ#2
//--------------------------------------------------------------------------------------------------
Pulse::Pulse(float r, float g, float b, float a, float speed, bool pulse)
{
	m_fColor[0] = r;
	m_fColor[1] = g;
	m_fColor[2] = b;
	m_fColor[3] = a;
	m_fSpeed = speed;
	m_bPulse = pulse;
}

//----------------------------------------------------------------------------------------------------
// Name : operator = ()
//----------------------------------------------------------------------------------------------------
Pulse& Pulse::operator =(const Pulse &pIn)
{
	m_fColor[0] = pIn.m_fColor[0];
	m_fColor[1] = pIn.m_fColor[1];
	m_fColor[2] = pIn.m_fColor[2];
	m_fColor[3] = pIn.m_fColor[3];
	m_fSpeed = pIn.m_fSpeed;
	m_bPulse = pIn.m_bPulse;
	return (*this);
}

//---------------------------------------------------------------------------------------------------
// Name : SetColor()
// Desc : 色の設定
//---------------------------------------------------------------------------------------------------
void Pulse::SetColor(float r, float g, float b, float a)
{
	m_fColor[0] = r;
	m_fColor[1] = g;
	m_fColor[2] = b;
	m_fColor[3] = a;
}

//---------------------------------------------------------------------------------------------------
// Name : SetSpeed()
// Desc : スピードの設定
//---------------------------------------------------------------------------------------------------
void Pulse::SetSpeed(float speed)
{
	m_fSpeed = speed;
}

//---------------------------------------------------------------------------------------------------
// Name : SetPulse()
// Desc : フラグの設定
//---------------------------------------------------------------------------------------------------
void Pulse::SetPulse(bool flag)
{
	m_bPulse = flag;
}

//---------------------------------------------------------------------------------------------------
// Name : SetPulse()
// Desc : タイプの設定
//---------------------------------------------------------------------------------------------------
void Pulse::SetType(float type_tmp)
{
	type = type_tmp;
}

//---------------------------------------------------------------------------------------------------
// Name : GetColor()
// Desc : 色の取得
//---------------------------------------------------------------------------------------------------
float* Pulse::GetColor()
{
	return m_fColor;
}

//---------------------------------------------------------------------------------------------------
// Name : GetSpeed()
// Desc : スピードの取得
//---------------------------------------------------------------------------------------------------
float Pulse::GetSpeed()
{
	return m_fSpeed;
}

//---------------------------------------------------------------------------------------------------
// Name : GetPulse()
// Desc : フラグの取得
//---------------------------------------------------------------------------------------------------
bool Pulse::GetPulse()
{
	return m_bPulse;
}

//------------------------------------------------------------------------
// Name : GetDirectionPath()
// Desc : 文字列からディレクトリを取得
//------------------------------------------------------------------------
char* GetDirectoryPath(const char* filename)
{
	char *strings = NULL;
	char *dir;
	dir = new char[strlen(filename) + 1];
	strcpy_s(dir, 100, filename);
	if (strings = strrchr(dir, '/')) strings[1] = '\0';
	else if (strings = strrchr(dir, '\\')) strings[1] = '\0';
	else dir[0] = '\0';
	return dir;
}

//-----------------------------------------------------------------------
// Name : CutDirectoryPath()
// Desc : 文字列からディレクトリを削除
//-----------------------------------------------------------------------
char* CutDirectoryPath(const char* filename)
{
	char *s = NULL;
	char *base = NULL;
	base = new char[strlen(filename) + 1];
	strcpy_s(base, 100, filename);
	if (s = strrchr(base, '\\'))
	{
		s++;
		return s;
	}
	else if (s = strrchr(base, '/'))
	{
		s++;
		return s;
	}
	else
	{
		return base;
	}
}

//-----------------------------------------------------------------------
// Name : SetDirectoryPath
// Desc : ディレクトリを前に付加して文字列を返す
//-----------------------------------------------------------------------
char* SetDirectoryPath(char* dest, char* directory)
{
	char* str;
	str = new char[strlen(directory) + strlen(dest) + 1];
	strcpy_s(str, 100, directory);
	strcat_s(str, 100, dest);
	//strcpy_s(str, sizeof(directory) / sizeof(directory[0]), directory);
	//strcat_s(str, sizeof(dest) / sizeof(dest[0]), dest);
	return str;
}

//-----------------------------------------------------------------------
// Name : InitMaterial()
// Desc : マテリアルの初期化
//-----------------------------------------------------------------------
void InitMaterial(OBJMATERIAL* pMaterial)
{
	ZeroMemory(pMaterial, sizeof(OBJMATERIAL));
	pMaterial->ambient = OBJVEC3(0.2f, 0.2f, 0.2f);
	pMaterial->diffuse = OBJVEC3(0.8f, 0.8f, 0.8f);
	pMaterial->specular = OBJVEC3(1.0f, 1.0f, 1.0f);
	pMaterial->shininess = 0.0f;
	pMaterial->alpha = 1.0f;
}

//-----------------------------------------------------------------------
// Name : SetMaterial()
// Desc : マテリアルを設定する
//-----------------------------------------------------------------------
void SetMaterial(OBJMATERIAL* pMaterial)
{
	glColor4f(pMaterial->diffuse.x, pMaterial->diffuse.y, pMaterial->diffuse.z, pMaterial->alpha);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, pMaterial->ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, pMaterial->diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, pMaterial->specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &pMaterial->shininess);
}

//-----------------------------------------------------------------------
// Name : OBJVEC3Scale()
// Desc : スケール倍する
//-----------------------------------------------------------------------
OBJVEC3 OBJVEC3Scale(OBJVEC3 v, float scale)
{
	return OBJVEC3(v.x * scale, v.y * scale, v.z * scale);
}

//-----------------------------------------------------------------------
// Name : OBJVEC3Add()
// Desc : 足し算する
//-----------------------------------------------------------------------
OBJVEC3 OBJVEC3Add(OBJVEC3 v1, OBJVEC3 v2)
{
	return OBJVEC3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

//-----------------------------------------------------------------------
// Name : OBJVEC3Substract()
// Desc : 引き算する
//-----------------------------------------------------------------------
OBJVEC3 OBJVEC3Substract(OBJVEC3 v1, OBJVEC3 v2)
{
	return OBJVEC3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

//-----------------------------------------------------------------------
// Name : OBJVEC3Length()
// Desc : 長さを求める
//-----------------------------------------------------------------------
float OBJVEC3Length(OBJVEC3 v)
{
	return sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
}

//-----------------------------------------------------------------------
// Name : OBJVEC3Max()
// Desc : 最大値を求める
//-----------------------------------------------------------------------
OBJVEC3 OBJVEC3Max(OBJVEC3 v, OBJVEC3 _max)
{
	OBJVEC3 result;
	result.x = (v.x > _max.x ? v.x : _max.x);
	result.y = (v.y > _max.y ? v.y : _max.y);
	result.z = (v.z > _max.z ? v.z : _max.z);
	return result;
}

//------------------------------------------------------------------------
// Name : OBJVEC3Min()
// Desc : 最小値を求める
//------------------------------------------------------------------------
OBJVEC3 OBJVEC3Min(OBJVEC3 v, OBJVEC3 _min)
{
	OBJVEC3 result;
	result.x = (v.x < _min.x ? v.x : _min.x);
	result.y = (v.y < _min.y ? v.y : _min.y);
	result.z = (v.z < _min.z ? v.z : _min.z);
	return result;
}

//////////////////////////////////////////////////////////////////////////
// OBJVEC2
//////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------
// Name : OBJVEC2()
// Desc : コンストラクタ
//------------------------------------------------------------------------
OBJVEC2::OBJVEC2(float nx, float ny)
	: x(nx), y(ny)
{
}

//-----------------------------------------------------------------------
// operator float*
//-----------------------------------------------------------------------
OBJVEC2::operator float * ()
{
	return (float*)&x;
}

//-----------------------------------------------------------------------
// operator const float*
//-----------------------------------------------------------------------
OBJVEC2::operator const float *() const
{
	return (const float*)&x;
}


//////////////////////////////////////////////////////////////////////////
// OBJVEC3
//////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------
// Name : OBJVEC3()
// Desc : コンストラクタ
//------------------------------------------------------------------------
OBJVEC3::OBJVEC3(float nx, float ny, float nz)
	: x(nx), y(ny), z(nz)
{
}

//------------------------------------------------------------------------
// operator float*
//------------------------------------------------------------------------
OBJVEC3::operator float *()
{
	return (float*)&x;
}

//------------------------------------------------------------------------
// operator const float*
//------------------------------------------------------------------------
OBJVEC3::operator const float *() const
{
	return (const float*)&x;
}

//////////////////////////////////////////////////////////////////////////
// OBJBOUNDINGBOX
/////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------
// Name : OBJBOUNDINGBOX()
// Desc : コンストラクタ
//-----------------------------------------------------------------------
OBJBOUNDINGBOX::OBJBOUNDINGBOX(OBJVEC3 value)
	: minimum(value), maximum(value)
{
}

//-----------------------------------------------------------------------
// Name : Merage()
// Desc : 
//-----------------------------------------------------------------------
void OBJBOUNDINGBOX::Merge(OBJVEC3 value)
{
	maximum = OBJVEC3Max(value, maximum);
	minimum = OBJVEC3Min(value, minimum);
	size = OBJVEC3Substract(maximum, minimum);
}

//////////////////////////////////////////////////////////////////////////
// OBJBOUNDINGSPHERE
//////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------
// Name : Create()
// Desc :
//------------------------------------------------------------------------
void OBJBOUNDINGSPHERE::Create(OBJBOUNDINGBOX box)
{
	center = OBJVEC3Add(box.maximum, box.minimum);
	center = OBJVEC3Scale(center, 0.5f);

	OBJVEC3 half;
	half = OBJVEC3Substract(box.maximum, center);
	radius = OBJVEC3Length(half);
}


/////////////////////////////////////////////////////////////////////////
// OBJMESH
/////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------
// Name : OBJMESH()
// Desc : コンストラクタ
//-----------------------------------------------------------------------
OBJMESH::OBJMESH()
{
	m_NumVertices = 0;
	m_NumMaterials = 0;
	m_NumSubsets = 0;
	m_NumIndices = 0;

	m_Vertices = 0;
	m_Materials = 0;
	m_Subsets = 0;
	m_Indices = 0;
}

//-----------------------------------------------------------------------
// Name : ~OBJMESH()
// Desc : デストラクタ
//-----------------------------------------------------------------------
OBJMESH::~OBJMESH()
{
	Release();
};

//-----------------------------------------------------------------------
// Name : Release()
// Desc : メモリを破棄
//-----------------------------------------------------------------------
void OBJMESH::Release()
{
	SAFE_DELETE_ARRAY(m_Vertices);
	SAFE_DELETE_ARRAY(m_Materials);
	SAFE_DELETE_ARRAY(m_Subsets);
	SAFE_DELETE_ARRAY(m_Indices);

	m_NumVertices = 0;
	m_NumMaterials = 0;
	m_NumSubsets = 0;
	m_NumIndices = 0;
}

//-----------------------------------------------------------------------
// Name : LoadOBJFile()
// Desc : OBJファイルの読み込み
//-----------------------------------------------------------------------
bool OBJMESH::LoadOBJFile(const char *filename)
{
	ifstream file;

	char mtlFileName[OBJ_NAME_LENGTH] = { 0 };
	char buf[OBJ_BUFFER_LENGTH] = { 0 };
	vector<OBJVEC3> positions;
	vector<OBJVEC3> normals;
	vector<OBJVEC2> texcoords;
	vector<OBJVERTEX> t_vertices;
	vector<OBJSUBSET> t_subsets;
	vector<unsigned int> t_indices;
	bool initBox = false;
	int prevSize = 0;

	unsigned long total = 0;

	OBJMATERIAL material;
	unsigned int dwFaceIndex = 0;
	unsigned int dwFaceCount = 0;
	unsigned int dwCurSubset = 0;

	//　ディレクトリを切り取り
	strcpy_s(m_directoryPath, GetDirectoryPath(filename));

	//　ファイルを開く
	file.open(filename, ios::in);

	//　チェック
	if (!file.is_open())
	{
		cerr << "Error : ファイルオープンに失敗\n";
		cerr << "File Name : " << filename << endl;
		return false;
	}

	//　ループ
	for (;; )
	{
		file >> buf;
		if (!file)
			break;

		//　コメント
		if (0 == strcmp(buf, "#"))
		{
		}

		//　頂点座標
		else if (0 == strcmp(buf, "v"))
		{
			float x, y, z;
			file >> x >> y >> z;
			OBJVEC3 v(x, y, z);
			positions.push_back(v);

			//　バウンディングボックスの初期化
			if (!initBox)
			{
				m_Box = OBJBOUNDINGBOX(v);
				initBox = true;
			}

			//　バウンディングボックスの算出
			m_Box.Merge(v);
		}

		//　テクスチャ座標
		else if (0 == strcmp(buf, "vt"))
		{
			float u, v;
			file >> u >> v;
			texcoords.push_back(OBJVEC2(u, v));
		}

		//　法線ベクトル
		else if (0 == strcmp(buf, "vn"))
		{
			float x, y, z;
			file >> x >> y >> z;
			normals.push_back(OBJVEC3(x, y, z));
		}

		//　面
		else if (0 == strcmp(buf, "f"))
		{
			unsigned int iPosition, iTexCoord, iNormal;
			unsigned int p[4] = { -1 }, t[4] = { -1 }, n[4] = { -1 };
			OBJVERTEX vertex;
			dwFaceIndex++;
			dwFaceCount++;
			int count = 0;
			unsigned int index = 0;

			//　三角形・四角形のみ対応
			for (int iFace = 0; iFace < 4; iFace++)
			{
				count++;	//　頂点数を数える
				ZeroMemory(&vertex, sizeof(OBJVERTEX));

				file >> iPosition;
				vertex.position = positions[iPosition - 1];
				p[iFace] = iPosition - 1;

				if ('/' == file.peek())
				{
					file.ignore();

					//　テクスチャ座標インデックス
					if ('/' != file.peek())
					{
						file >> iTexCoord;
						vertex.texcoord = texcoords[iTexCoord - 1];
						t[iFace] = iTexCoord - 1;
					}

					//　法線ベクトルインデックス
					if ('/' == file.peek())
					{
						file.ignore();

						file >> iNormal;
						vertex.normal = normals[iNormal - 1];
						n[iFace] = iNormal - 1;
					}
				}

				//　カウントが3未満
				if (iFace < 3)
				{
					t_vertices.push_back(vertex);
					index = t_vertices.size() - 1;
					t_indices.push_back(index);
				}

				//　次が改行だったら終了
				if ('\n' == file.peek())
				{
					break;
				}

			}

			//　四角形ポリゴンの場合，三角形を追加する
			if (count > 3)
			{
				//　カウント
				dwFaceIndex++;
				dwFaceCount++;

				//　頂点とインデックスを追加
				for (int iFace = 1; iFace < 4; iFace++)
				{
					int j = (iFace + 1) % 4;
					ZeroMemory(&vertex, sizeof(OBJVERTEX));

					if (p[j] != -1) vertex.position = positions[p[j]];
					if (t[j] != -1) vertex.texcoord = texcoords[t[j]];
					if (n[j] != -1) vertex.normal = normals[n[j]];

					t_vertices.push_back(vertex);
					index = t_vertices.size() - 1;
					t_indices.push_back(index);
				}

			}
		}

		//　マテリアルファイル
		else if (0 == strcmp(buf, "mtllib"))
		{
			file >> mtlFileName;
			//　マテリアルファイルの読み込み
			if (mtlFileName[0])
			{
				if (!LoadMTLFile(SetDirectoryPath(mtlFileName, m_directoryPath)))
				{
					cerr << "Error : マテリアルのロードに失敗\n";
					return false;
				}
			}
		}

		//　マテリアル
		else if (0 == strcmp(buf, "usemtl"))
		{
			char strName[OBJ_NAME_LENGTH] = { 0 };
			file >> strName;
			OBJSUBSET subset;
			subset.faceCount = 1;

			for (unsigned int i = 0; i < m_NumMaterials; i++)
			{
				if (0 == strcmp(m_Materials[i].name, strName))
				{
					dwCurSubset = i;
					break;
				}
			}

			subset.materialIndex = dwCurSubset;
			subset.faceStart = dwFaceIndex * 3;
			prevSize = t_subsets.size();
			t_subsets.push_back(subset);
			if (t_subsets.size() > 1)
			{
				t_subsets[prevSize - 1].faceCount = dwFaceCount * 3;
				dwFaceCount = 0;
			}
		}

		file.ignore(OBJ_BUFFER_LENGTH, '\n');
	}

	//　サブセット
	if (t_subsets.size() > 0)
	{
		int maxSize = t_subsets.size();
		t_subsets[maxSize - 1].faceCount = dwFaceCount * 3;
	}

	//　ファイルを閉じる
	file.close();

	//　頂点データをコピー
	m_NumVertices = t_vertices.size();
	m_Vertices = new OBJVERTEX[m_NumVertices];
	for (unsigned int i = 0; i < m_NumVertices; i++)
		m_Vertices[i] = t_vertices[i];

	//　サブセットデータをコピー
	m_NumSubsets = t_subsets.size();
	m_Subsets = new OBJSUBSET[m_NumSubsets];
	for (unsigned int i = 0; i < m_NumSubsets; i++)
		m_Subsets[i] = t_subsets[i];

	//　インデックスデータをコピー
	m_NumIndices = t_indices.size();
	m_Indices = new unsigned int[m_NumIndices];
	for (unsigned int i = 0; i < m_NumIndices; i++)
		m_Indices[i] = t_indices[i];

	//　バウンディングスフィアの作成
	m_Sphere.Create(m_Box);

	//　メモリ破棄
	positions.clear();
	normals.clear();
	texcoords.clear();
	t_vertices.clear();
	t_subsets.clear();
	t_indices.clear();

	//　正常終了
	return true;
}

//-----------------------------------------------------------------------
// Name : LoadMTLFile()
// Desc : MTLファイルの読み込み
//-----------------------------------------------------------------------
bool OBJMESH::LoadMTLFile(const char* filename)
{
	char buf[OBJ_BUFFER_LENGTH] = { 0 };
	int iMtlCount = -1;
	ifstream file;
	vector<OBJMATERIAL> t_materials;
	OBJMATERIAL material;
	InitMaterial(&material);

	//　ファイルを開く
	file.open(filename, ios::in);

	//　チェック
	if (!file.is_open())
	{
		cerr << "Error : ファイルオープンに失敗しました\n";
		cerr << "File Name : " << filename << endl;
		return false;
	}

	//　ループ
	for (;; )
	{
		file >> buf;
		if (!file)
			break;

		// New Material
		if (0 == strcmp(buf, "newmtl"))
		{
			iMtlCount++;
			t_materials.push_back(material);
			char strName[OBJ_NAME_LENGTH] = { 0 };
			file >> strName;
			strcpy_s(t_materials[iMtlCount].name, strName);
		}
		// Ambient Color
		else if (0 == strcmp(buf, "Ka"))
		{
			float r, g, b;
			file >> r >> g >> b;
			t_materials[iMtlCount].ambient = OBJVEC3(r, g, b);
		}
		// Diffuse Color
		else if (0 == strcmp(buf, "Kd"))
		{
			float r, g, b;
			file >> r >> g >> b;
			t_materials[iMtlCount].diffuse = OBJVEC3(r, g, b);
		}
		// Specular Color
		else if (0 == strcmp(buf, "Ks"))
		{
			float r, g, b;
			file >> r >> g >> b;
			t_materials[iMtlCount].specular = OBJVEC3(r, g, b);
		}
		// Alpha
		else if (0 == strcmp(buf, "d") ||
			0 == strcmp(buf, "Tr"))
		{
			file >> t_materials[iMtlCount].alpha;
		}
		// Shininess
		else if (0 == strcmp(buf, "Ns"))
		{
			file >> t_materials[iMtlCount].shininess;
		}
		// Ambient Map
		else if (0 == strcmp(buf, "map_Ka"))
		{
			char mapKaName[OBJ_NAME_LENGTH];
			file >> mapKaName;
			SetDirectoryPath(mapKaName, m_directoryPath);
			strcpy_s(t_materials[iMtlCount].ambientMapName, mapKaName);
		}
		// Diffuse Map
		else if (0 == strcmp(buf, "map_Kd"))
		{
			char mapKdName[OBJ_NAME_LENGTH];
			file >> mapKdName;
			SetDirectoryPath(mapKdName, m_directoryPath);
			strcpy_s(t_materials[iMtlCount].diffuseMapName, mapKdName);
		}
		// Specular Map
		else if (0 == strcmp(buf, "map_Ks"))
		{
			char mapKsName[OBJ_NAME_LENGTH];
			file >> mapKsName;
			SetDirectoryPath(mapKsName, m_directoryPath);
			strcpy_s(t_materials[iMtlCount].specularMapName, mapKsName);
		}
		// Bump Map
		else if (0 == strcmp(buf, "map_Bump"))
		{
			char mapBumpName[OBJ_NAME_LENGTH];
			file >> mapBumpName;
			SetDirectoryPath(mapBumpName, m_directoryPath);
			strcpy_s(t_materials[iMtlCount].bumpMapName, mapBumpName);
		}

		file.ignore(OBJ_BUFFER_LENGTH, '\n');
	}

	//　ファイルを閉じる
	file.close();

	//　マテリアルデータをコピー
	m_NumMaterials = t_materials.size();
	m_Materials = new OBJMATERIAL[m_NumMaterials];
	for (unsigned int i = 0; i < m_NumMaterials; i++)
		m_Materials[i] = t_materials[i];

	//　正常終了
	return true;
}

//-----------------------------------------------------------------------
// Name : LoadFile()
// Desc : メッシュファイルの読み込み
//-----------------------------------------------------------------------
bool OBJMESH::LoadFile(const char* filename)
{
	//　OBJ, MTLファイルを読み込み
	if (!LoadOBJFile(filename))
	{
		cerr << "Error : メッシュファイルの読み込みに失敗しました\n";
		return false;
	}

	//　正常終了
	return true;
}


//-----------------------------------------------------------------------
// Name : Draw()
// Desc : 描画処理
//-----------------------------------------------------------------------
void OBJMESH::Draw()
{
	for (unsigned int i = 0; i < m_NumSubsets; i++)
	{
		//　マテリアル
		OBJMATERIAL* pMat = &m_Materials[m_Subsets[i].materialIndex];
		SetMaterial(pMat);

		//　三角形描画
		glInterleavedArrays(GL_T2F_N3F_V3F, 0, m_Vertices);
		glDrawElements(GL_TRIANGLES, m_Subsets[i].faceCount, GL_UNSIGNED_INT, &m_Indices[m_Subsets[i].faceStart]);
	}

	//cout << m_NumSubsets << endl;

	/*
	if (m_NumSubsets == 0) {
		vector<OBJMATERIAL> t_materials;
		t_materials[0].ambient = OBJVEC3(0.60000, 0.51060, 0.39060);
		t_materials[0].diffuse = OBJVEC3(0.80000, 0.68080, 0.510080);
		t_materials[0].specular = OBJVEC3(0.00000, 0.00000, 0.00000);
		t_materials[0].shininess = 5.00000;

		m_Materials = new OBJMATERIAL[1];
		m_Materials[0] = t_materials[0];

		OBJMATERIAL* pMat = &m_Materials[0];
		SetMaterial(pMat);

		glInterleavedArrays(GL_T2F_N3F_V3F, 0, m_Vertices);
		glDrawElements(GL_TRIANGLES, 4908, GL_UNSIGNED_INT, &m_Indices[0]);
	}
	*/
}


//------------------------------------------------------------------------
// Name : GetNumVertices()
// Desc : 頂点数を取得
//------------------------------------------------------------------------
unsigned int OBJMESH::GetNumVertices()
{
	return m_NumVertices;
}

//-----------------------------------------------------------------------
// Name : GetNumSubsets()
// Desc : サブセット数を取得
//-----------------------------------------------------------------------
unsigned int OBJMESH::GetNumSubsets()
{
	return m_NumSubsets;
}

//-----------------------------------------------------------------------
// Name : GetNumMaterials()
// Desc : マテリアル数を取得
//-----------------------------------------------------------------------
unsigned int OBJMESH::GetNumMaterials()
{
	return m_NumMaterials;
}

//-----------------------------------------------------------------------
// Name : GetNumIndices()
// Desc : インデックス数を取得
//-----------------------------------------------------------------------
unsigned int OBJMESH::GetNumIndices()
{
	return m_NumIndices;
}

//-----------------------------------------------------------------------
// Name : GetIndexData()
// Desc : 指定されたインデックスデータを取得
//-----------------------------------------------------------------------
unsigned int OBJMESH::GetIndexData(unsigned int index)
{
	return m_Indices[index];
}

//-----------------------------------------------------------------------
// Name : GetVertex()
// Desc : 指定された頂点データを取得
//-----------------------------------------------------------------------
OBJVERTEX OBJMESH::GetVertex(unsigned int index)
{
	return m_Vertices[index];
}

//-----------------------------------------------------------------------
// Name : GetSubsets()
// Desc : 指定されたサブセットデータを取得
//-----------------------------------------------------------------------
OBJSUBSET OBJMESH::GetSubset(unsigned int index)
{
	return m_Subsets[index];
}

//-----------------------------------------------------------------------
// Name : GetMaterial()
// Desc : 指定されたマテリアルデータを取得
//-----------------------------------------------------------------------
OBJMATERIAL OBJMESH::GetMaterial(unsigned int index)
{
	return m_Materials[index];
}

//-----------------------------------------------------------------------
// Name : GetVertices()
// Desc : 頂点データを取得
//-----------------------------------------------------------------------
OBJVERTEX* OBJMESH::GetVertices()
{
	return m_Vertices;
}

//-----------------------------------------------------------------------
// Name : GetSubsets()
// Desc : サブセットデータを取得
//-----------------------------------------------------------------------
OBJSUBSET* OBJMESH::GetSubsets()
{
	return m_Subsets;
}

//-----------------------------------------------------------------------
// Name : GetMaterails()
// Desc : マテリアルデータを取得
//-----------------------------------------------------------------------
OBJMATERIAL* OBJMESH::GetMaterials()
{
	return m_Materials;
}

//-----------------------------------------------------------------------
// Name : GetIndices()
// Desc : インデックスデータを取得
//-----------------------------------------------------------------------
unsigned int* OBJMESH::GetIndices()
{
	return m_Indices;
}

//-----------------------------------------------------------------------
// Name : GetBox()
// Desc : バウンディングボックスを取得
//-----------------------------------------------------------------------
OBJBOUNDINGBOX OBJMESH::GetBox()
{
	return m_Box;
}

//-----------------------------------------------------------------------
// Name : GetSphere()
// Desc : バウンディングスフィアを取得
//-----------------------------------------------------------------------
OBJBOUNDINGSPHERE OBJMESH::GetSphere()
{
	return m_Sphere;
}



//////////////////////////////////////////////////////////////////////////
//　　BMPImage class
//////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------------------------------
//　　BMPImage
//　　Desc : コンストラクタ
//-----------------------------------------------------------------------------------------------------
BMPImage::BMPImage()
{
	imageSize = 0;
	imageData = NULL;
	format = GL_RGB;
	internalFormat = GL_RGB;
	width = 0;
	height = 0;
	bpp = 0;
	ID = 0;
}

//-----------------------------------------------------------------------------------------------------
//　　~BMPImage
//　　Desc : デストラクタ
//-----------------------------------------------------------------------------------------------------
BMPImage::~BMPImage()
{
	if (imageData)
	{
		delete[] imageData;
		imageData = NULL;
	}
}

//-----------------------------------------------------------------------------------------------------
//　　ReadBMP
//　　Desc : BMPファイルの読み込み
//-----------------------------------------------------------------------------------------------------


bool BMPImage::ReadBMP(const char *filename)
{
	FILE *fp;

	BITMAPINFOHEADER bitmapInfoHeader;
	BITMAPFILEHEADER header;
	GLubyte temp = 0;

	//　ファイルを開く
	if ((fp = fopen(filename, "rb")) == NULL)
	{
		cout << "Error : 指定されたファイルが開けません\n";
		cout << "File Name : " << filename << endl;
		return false;
	}

	// ファイルを開きます。
//FILE * file;
//= fopen(imagepath, "rb");
//errno_t error;
//error = fopen_s(&file, imagepath, "rb");

//　ヘッダー情報の読み取り
	fread(&header, 1, sizeof(BITMAPFILEHEADER), fp);

	//if (fread(header, 1, 54, file) != 54) { // 54バイト読み込めなければ、問題があります。
	//	printf("BMPファイルではありません。\n");
	//	return false;
	//}
		// ファイルから実際のデータをバッファに読み込む
	//fread(data, 1, imageSize, file);

	//　ファイルチェック
	if (header.bfType != 0x4d42)
	{
		cout << "Error : Bitmapファイルではありません\n";
		fclose(fp);
		return false;
	}

	//　ヘッダー情報の読み取り
	fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, fp);

	//　幅と高さを取得
	width = bitmapInfoHeader.biWidth;
	height = bitmapInfoHeader.biHeight;

	if (bitmapInfoHeader.biSizeImage == 0)
	{
		bitmapInfoHeader.biSizeImage = bitmapInfoHeader.biWidth * bitmapInfoHeader.biHeight * 3;
	}

	fseek(fp, header.bfOffBits, SEEK_SET);

	//　データサイズを決定し，メモリを確保
	imageSize = bitmapInfoHeader.biSizeImage;
	imageData = new GLubyte[imageSize];

	//　エラーチェック
	if (!imageData)
	{
		cout << "Error : メモリが確保できませんでした\n";
		delete[] imageData;
		fclose(fp);
		return false;
	}

	//　ピクセルデータの読み込み
	fread(imageData, 1, bitmapInfoHeader.biSizeImage, fp);

	//　BGR　→　RGBに変換
	for (int i = 0; i < (int)imageSize; i += 3)
	{
		temp = imageData[i + 0];
		imageData[i + 0] = imageData[i + 2];
		imageData[i + 2] = temp;
	}

	//　ファイルを閉じる
	fclose(fp);

	return true;
}

//-----------------------------------------------------------------------------------------------------
//　　Load
//　　Desc : BMPファイルを読み込み，テクスチャを生成
//-----------------------------------------------------------------------------------------------------
GLuint BMPImage::Load(const char *filename)
{
	if (!ReadBMP(filename))
		return 0;

	glGenTextures(1, &ID);

	glBindTexture(GL_TEXTURE_2D, ID);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	gluBuild2DMipmaps(GL_TEXTURE_2D, internalFormat, width, height, format, GL_UNSIGNED_BYTE, imageData);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	if (imageData)
	{
		delete[] imageData;
		imageData = NULL;
	}
	return ID;
}

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






//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 絶滅させた相良の努力の結晶. OpenGLマスターは君だけだ
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
GLuint loadBMP_custom(const char * imagepath) {
	// BMPファイルのヘッダから読み込まれるデータ
	unsigned char header[54]; // 各BMPファイルは54バイトのヘッダから始まります。
	unsigned int dataPos;     // 実際のデータがファイルのどの位置にあるか
	unsigned int width, height;
	unsigned int imageSize;   // = 横*高さ*3
	// 実際のRGBデータ
	unsigned char * data;

	// ファイルを開きます。
	FILE * file;
	//= fopen(imagepath, "rb");
	errno_t error;
	error = fopen_s(&file, imagepath, "rb");



	if (error) {
		printf("画像が開けませんでした。\n");
		return 0;
	}

	if (fread(header, 1, 54, file) != 54) { // 54バイト読み込めなければ、問題があります。
		printf("BMPファイルではありません。\n");
		return false;
	}

	if (header[0] != 'B' || header[1] != 'M') {
		printf("BMPファイルではありません。\n");
		return 0;
	}

	// バイト配列から整数を読み込む
	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);

	// BMPファイルの中にはミスフォーマットがあります。その情報を推測します。
	if (imageSize == 0)    imageSize = width * height * 3; // 3 : 1バイトは赤、緑、青の各色です。
	if (dataPos == 0)      dataPos = 54; // これでBMPヘッダは終わりです。

	// バッファを作る
	data = new unsigned char[imageSize];

	// ファイルから実際のデータをバッファに読み込む
	fread(data, 1, imageSize, file);

	//すべてはメモリ上にあるので、ファイルは閉じます。
	fclose(file);

	// ひとつのOpenGLテクスチャを作ります。
	GLuint textureID;
	glGenTextures(1, &textureID);

	// 新たに作られたテクスチャを"バインド"します。つまりここから後のテクスチャ関数はこのテクスチャを変更します。
	glBindTexture(GL_TEXTURE_2D, textureID);

	// OpenGLに画像を渡します。
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// 画像を拡大(MAGnifying)するときは線形(LINEAR)フィルタリングを使います。
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// 画像を縮小(MINifying)するとき、線形(LINEAR)フィルタした、二つのミップマップを線形(LINEARYLY)に混ぜたものを使います。
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	// 次のようにしてミップマップを作ります。
	glGenerateMipmap(GL_TEXTURE_2D);

	return textureID;

}

GLuint loadTGA_glfw(const char * imagepath) {

	// 一つのOpenGLテクスチャを作ります。
	GLuint textureID;
	glGenTextures(1, &textureID);

	// 新しく作ったテクスチャを"バインド"します。これ以降の関数はすべてこのテクスチャを変更します。
	glBindTexture(GL_TEXTURE_2D, textureID);

	// 正しいパラメータでglTexImage2Dを呼んで、ファイルを読み込みます。
	//glfwLoadTexture2D(imagepath, 0);

	// 3つの線形フィルタリング
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	// 作成したテクスチャのIDを返します。
	return textureID;
}

GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path) {

	// シェーダを作ります。
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// ファイルから頂点シェーダのコードを読み込みます。
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open())
	{
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}

	// ファイルからフラグメントシェーダを読み込みます。
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;


	// 頂点シェーダをコンパイルします。
	printf("Compiling shader : %sn", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// 頂点シェーダをチェックします。
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);

	//std::vector<char> VertexShaderErrorMessage(InfoLogLength);
	//glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
	//fprintf(stdout, "%sn", &VertexShaderErrorMessage[0]);

	// フラグメントシェーダをコンパイルします。
	printf("Compiling shader : %sn", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// フラグメントシェーダをチェックします。
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);

	//std::vector<char> FragmentShaderErrorMessage(InfoLogLength);
	//glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
	//fprintf(stdout, "%sn", &FragmentShaderErrorMessage[0]);


	// プログラムをリンクします。
	fprintf(stdout, "Linking programn");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// プログラムをチェックします。
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> ProgramErrorMessage(std::max(InfoLogLength, int(1)));
	glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
	fprintf(stdout, "%sn", &ProgramErrorMessage[0]);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	glUseProgram(ProgramID);


	//GLuint ProgramID = glCreateProgram();
	return ProgramID;
}

bool loadOBJ(const char * path, std::vector<glm::vec3>  & out_vertices, std::vector<glm::vec2>  & out_uvs, std::vector<glm::vec3>  & out_normals, int &ver) {

	std::vector<int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;

	//FILE * file = fopen(path, "r");
	//if (file == NULL) {
	//	printf("ファイルを開けません!n");
	//	return false;
	//}

	// ファイルを開きます。
	FILE * file;
	errno_t error;
	error = fopen_s(&file, path, "r");
	char lineHeader[2048];
	bool dbg = false;					//trueにすると, アルファベットがたくさん出てくるデバッグが開始される
	while (1) {

		// 行の最初の文字列を読み込みます。
		int res = fscanf_s(file, "%s", lineHeader, _countof(lineHeader));
		if (res == EOF) {
			if (dbg) printf("b");
			// 各三角形の各頂点
			for (unsigned int i = 0; i < vertexIndices.size(); i++) {
				unsigned int vertexIndexa = vertexIndices[i];
				glm::vec3 vertex = temp_vertices[vertexIndexa - 1];
				out_vertices.push_back(vertex);
				ver++;
				//std::cout << vertexIndex;
			}
			if (dbg) printf("v");

			for (unsigned int i = 0; i < uvIndices.size(); i++) {
				//for (unsigned int i = 0; i < 12; i++) {
				unsigned int uvIndexa = uvIndices[i];
				glm::vec2 uv;
				uv = temp_uvs[uvIndexa - 1];
				out_uvs.push_back(uv);
			}

			if (dbg) printf("u");
			for (unsigned int i = 0; i < normalIndices.size(); i++) {
				unsigned int normalIndexa = normalIndices[i];
				glm::vec3 normal = temp_normals[normalIndexa - 1];
				out_normals.push_back(normal);
			}
			if (dbg) printf("a");
			break; // EOF = End Of File. ループを終了します。
		}
		// そうでなければlineHeaderをパースします。

		if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 vertex;
			fscanf_s(file, "%f %f %fn", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
			if (dbg) printf("v");
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			glm::vec2 uv;
			fscanf_s(file, "%f %fn", &uv.x, &uv.y);
			temp_uvs.push_back(uv);
			if (dbg) printf("vt");
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec3 normal;
			fscanf_s(file, "%f %f %fn", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
			if (dbg) printf("vn");
		}
		else if (strcmp(lineHeader, "f") == 0) {
			if (dbg) printf("f");
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf_s(file, "%d/%d/%d %d/%d/%d %d/%d/%dn", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			//int matches = fscanf_s(file, "%d//%d %d//%d %d//%dn", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				printf("File can't be read by our simple parser : ( Try exporting with other optionsn");
				return false;
			}
			if (dbg) printf("o");
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
			if (dbg) printf("e");
		}


	}

	return true;
}

bool loadOBJnoUV(const char * path, std::vector<glm::vec3>  & out_vertices, std::vector<glm::vec3>  & out_normals, int &ver) {

	std::vector<int> vertexIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec3> temp_normals;

	//FILE * file = fopen(path, "r");
	//if (file == NULL) {
	//	printf("ファイルを開けません!n");
	//	return false;
	//}

	// ファイルを開きます。
	FILE * file;
	errno_t error;
	error = fopen_s(&file, path, "r");

	while (1) {

		char lineHeader[1024];
		// 行の最初の文字列を読み込みます。
		int res = fscanf_s(file, "%s", lineHeader, _countof(lineHeader));
		if (res == EOF) {
			printf("b");
			// 各三角形の各頂点
			for (unsigned int i = 0; i < vertexIndices.size(); i++) {
				unsigned int vertexIndexa = vertexIndices[i];
				glm::vec3 vertex = temp_vertices[vertexIndexa - 1];
				out_vertices.push_back(vertex);
				ver++;
				//std::cout << vertexIndex;
			}
			printf("v");

			printf("u");
			for (unsigned int i = 0; i < normalIndices.size(); i++) {
				unsigned int normalIndexa = normalIndices[i];
				glm::vec3 normal = temp_normals[normalIndexa - 1];
				out_normals.push_back(normal);
			}
			printf("a");
			break; // EOF = End Of File. ループを終了します。
		}
		// そうでなければlineHeaderをパースします。

		if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 vertex;
			fscanf_s(file, "%f %f %fn", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
			printf("v");
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec3 normal;
			fscanf_s(file, "%f %f %fn", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
			printf("vn");
		}
		else if (strcmp(lineHeader, "f") == 0) {
			printf("f");
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], normalIndex[3];
			int matches = fscanf_s(file, "%d//%d %d//%d %d//%dn", &vertexIndex[0], &normalIndex[0], &vertexIndex[1], &normalIndex[1], &vertexIndex[2], &normalIndex[2]);
			//int matches = fscanf_s(file, "%d//%d %d//%d %d//%dn", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 6) {
				printf("File can't be read by our simple parser : ( Try exporting with other optionsn");
				return false;
			}
			printf("o");
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
			printf("e");
		}


	}

	return true;
}

void prosessingOfOBJ(int *ver, GLuint *vertexbuffer, std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals, std::vector<glm::vec2>& uvs, std::string calc_OBJFile, GLuint *uvbuffer, GLuint *VertexArrayID) {
	glGenBuffers(1, vertexbuffer);									// バッファを1つ作り、vertexbufferに結果IDを入れます。
	glBindBuffer(GL_ARRAY_BUFFER, *vertexbuffer);					// 次のコマンドは'vertexbuffer'バッファについてです。
	//if (!loadOBJ(calc_OBJFile.c_str(), vertices, uvs, normals, *ver)) {	// もしobjファイルを読み込めなかったら
	//	//printf("[ERROR] No File of %s\n", calc_OBJFile.c_str());	// エラー文を返す
	//}
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, uvbuffer);										// バッファを1つ作り、uvbufferに結果IDを入れます。
	glBindBuffer(GL_ARRAY_BUFFER, *uvbuffer);						// 次のコマンドは'uvbuffer'バッファについてです。
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

	glGenVertexArrays(1, VertexArrayID);							// バッファを1つ作り、VertexArrayIDに結果IDを入れます。
	glBindVertexArray(*VertexArrayID);								// 次のコマンドは'VertexArrayID'バッファについてです。

	// 最初の属性バッファ：頂点
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, *vertexbuffer);
	glVertexAttribPointer(
		0,                  // 属性0：0に特に理由はありません。しかし、シェーダ内のlayoutとあわせないといけません。
		3,                  // サイズ
		GL_FLOAT,           // タイプ
		GL_FALSE,           // 正規化？
		0,                  // ストライド
		(void*)0            // 配列バッファオフセット
	);


	// 2nd attribute buffer : colors
	glEnableVertexAttribArray(1);
	//glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, *uvbuffer);
	glVertexAttribPointer(
		1,                                // 属性。1という数字に意味はありません。ただしシェーダのlayoutとあわせる必要があります。
		2,                                // サイズ
		GL_FLOAT,                         // タイプ
		GL_FALSE,                         // 正規化？
		0,                                // ストライド
		(void*)0                          // 配列バッファオフセット
	);

	// 頂点をOpenGLに渡します。
	//glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
}

void prosessingOfMoveOBJ(int *ver, GLuint *vertexbuffer, std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals, std::vector<glm::vec2>& uvs, std::string calc_OBJFile, GLuint *uvbuffer, GLuint *VertexArrayID) {
	//glGenBuffers(1, vertexbuffer);									// バッファを1つ作り、vertexbufferに結果IDを入れます。
	glBindBuffer(GL_ARRAY_BUFFER, *vertexbuffer);					// 次のコマンドは'vertexbuffer'バッファについてです。
	//if (!loadOBJ(calc_OBJFile.c_str(), vertices, uvs, normals, *ver)) {	// もしobjファイルを読み込めなかったら
	//	printf("[ERROR] No File of %s\n", calc_OBJFile);					// エラー文を返す
	//}

	//std::cout << calc_OBJFile.c_str() << std::endl;

	//std::cout << "w";


	float objx_max = 0, objx_min = 0, objz_max = 0, objz_min = 0;
	float tmp;
	float obji = 0; double objj = 0;


	float obj_speed = 0.001;  //objectのスピード
	float dif_x, dif_z, sq;
	float obj_xspeed, obj_zspeed;


	if (calc_OBJFile == "obj/lowpig.obj") {

		if (obji == 0) {
			tmp = vertices[obji].x;
			obji++;
		}
		while (tmp == vertices[obji].x) {
			obji++;
		}
		if (tmp > vertices[obji].x) {
			objx_max = tmp;
			objx_min = vertices[obji].x;
		}
		else {
			objx_max = vertices[obji].x;
			objx_min = tmp;
		}

		double obj_x = (double)((objx_max + objx_min) / 2.0);

		if (objj == 0) {
			tmp = vertices[objj].z;
			objj++;
		}
		while (tmp == vertices[objj].z) {
			objj++;
		}
		if (tmp > vertices[objj].z) {
			objz_max = tmp;
			objz_min = vertices[objj].z;
		}
		else {
			objz_max = vertices[objj].z;
			objz_min = tmp;
		}

		double obj_z = (double)((objz_max + objz_min) / 2.0);

		dif_x = position.x - obj_x;
		dif_z = position.z - obj_z;
		sq = sqrt(dif_x * dif_x + dif_z * dif_z);

		//std::cout << sq << "\n";
		if (sq < 10) {

			obj_xspeed = (double)(dif_x / sq * obj_speed);
			obj_zspeed = (double)(dif_z / sq * obj_speed);

			for (int i = 0; i < vertices.size(); i++) {
				vertices[i].x += obj_xspeed;
				vertices[i].z += obj_zspeed;

				//if (objhit(vertices[i].x, vertices[i].z, cube)) {
				//	vertices[i].x -= obj_xspeed;
				//	vertices[i].z -= obj_zspeed;

			}
		}
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
	}



	//else if (calc_OBJFile == "obj/lowapple.obj") {
	//	//printf("bbbb");
	//	for (int i = 0; i < vertices.size(); i++) {
	//		vertices[i].x += obj_speed;
	//	}
	//	//std::cout << "apple";
	//	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
	//}

	//else {
	//	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
	//}


	//glGenBuffers(1, uvbuffer);										// バッファを1つ作り、uvbufferに結果IDを入れます。
	glBindBuffer(GL_ARRAY_BUFFER, *uvbuffer);						// 次のコマンドは'uvbuffer'バッファについてです。
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

	//glGenVertexArrays(1, VertexArrayID);							// バッファを1つ作り、VertexArrayIDに結果IDを入れます。
	glBindVertexArray(*VertexArrayID);								// 次のコマンドは'VertexArrayID'バッファについてです。

	// 最初の属性バッファ：頂点
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, *vertexbuffer);
	glVertexAttribPointer(
		0,                  // 属性0：0に特に理由はありません。しかし、シェーダ内のlayoutとあわせないといけません。
		3,                  // サイズ
		GL_FLOAT,           // タイプ
		GL_FALSE,           // 正規化？
		0,                  // ストライド
		(void*)0            // 配列バッファオフセット
	);


	// 2nd attribute buffer : colors
	glEnableVertexAttribArray(1);
	//glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, *uvbuffer);
	glVertexAttribPointer(
		1,                                // 属性。1という数字に意味はありません。ただしシェーダのlayoutとあわせる必要があります。
		2,                                // サイズ
		GL_FLOAT,                         // タイプ
		GL_FALSE,                         // 正規化？
		0,                                // ストライド
		(void*)0                          // 配列バッファオフセット
	);

	// 頂点をOpenGLに渡します。
	//glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
}

glm::mat4 getProjectionMatrix() {

	//glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
	return ProjectionM;
}

glm::mat4 getViewMatrix() {

	//glm::mat4 View = glm::lookAt(
	//	glm::vec3(4, 3, 3), // ワールド空間でカメラは(4,3,3)にあります。
	//	glm::vec3(0, 0, 0), // 原点を見ています。
	//	glm::vec3(0, 1, 0)  // 頭が上方向(0,-1,0にセットすると上下逆転します。)
	//);
	//return View;

	return ViewM;
}

glm::mat4 getModelMatrix(int i) {
	return ModelM[i];
}

glm::mat4 ObjMove(int i) {

	float objx_max = 0, objx_min = 0, objz_max = 0, objz_min = 0;
	float tmp;
	float obji = 0; double objj = 0;

	float obj_speed = 0.001;  //objectのスピード 今使ってない
	float dif_x, dif_z, sq;
	float obj_xspeed, obj_zspeed;

	double x = movecount;

	glm::mat4 movematrix;
	glm::mat4 movemat;


	if (i == 7) {
		//std::cout << "r";
		//glRotatef(10, 1.0f, 0.0f, 0.0f);
		//Model = getModelMatrix(i);
		//平行移動の変換行列
		movemat = glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, -5 + x, -1, 0, 1);

		//movemat = glm::mat4(1.0f) * movematrix;
		//mat4 ModelMatrix = RotationMatrix ;
		//mat4 ModelMatrix = TranslationMatrix * RotationMatrix * ScaleMatrix;
		// MVP行列を作るためにモデル行列を使えます。

	}

	return movemat;

}

glm::mat4 ObjRoll(int i, double RadianAngle) {
	//glBindBuffer(GL_ARRAY_BUFFER, *vertexbuffer);					// 次のコマンドは'vertexbuffer'バッファについてです。
	//glLoadIdentity();
	double pi = 3.1415;
	double onedegree = pi / 180.0; //1°のラジアン
	glm::mat4 rollmat;

	//回転
	glm::vec3 RotationAxis;
	//πの使い方忘れてしまった
	double RotationAngle = RadianAngle;
	RotationAxis.x = 0.0f;
	RotationAxis.y = 1.0f;
	RotationAxis.z = 0.0f;

	// RotationAngleはラジアンで表します。
	//double x = RotationAxis.x * sin(RotationAngle / 2);
	//double y = RotationAxis.y * sin(RotationAngle / 2);
	//double z = RotationAxis.z * sin(RotationAngle / 2);
	//double w = cos(RotationAngle / 2);

	// 単位クォータニオンを作る（何の回転もさせない）
	quat MyQuaternion;

	// 4要素の直接的な記述
	// これを直接使うことはないでしょう。
	//MyQuaternion = quat(w, x, y, z);

	// オイラー角からラジアンのクォータニオンへの変換
	//vec3 EulerAngles(90, 0, 0);
	//MyQuaternion = quat(EulerAngles);

	//std::cout << degrees(RotationAngle)<<"\n";

	//MyQuaternion = glm::angleAxis(float(degrees(RotationAngle)), RotationAxis);
	MyQuaternion = glm::angleAxis(float(RotationAngle), RotationAxis);

	// 軸-角からの変換
	// GLMでは角度は度で表すので、変換します。
	//MyQuaternion = glmangleAxis(degrees(RotationAngle), RotationAxis);

	//vec3 EulerAngles(RotationAroundXInRadians, RotationAroundYInRadians, RotationAroundZInRadians);

	rollmat = glm::toMat4(MyQuaternion);

	//mat4 ModelMatrix = RotationMatrix ;
	//mat4 ModelMatrix = TranslationMatrix * RotationMatrix * ScaleMatrix;
	// MVP行列を作るためにモデル行列を使えます。
	if (i == 7) {
		//std::cout << "r";
		//glRotatef(10, 1.0f, 0.0f, 0.0f);
		//Model = getModelMatrix(i);

	}
	//
	//glm::mat3 M = glm::mat3(2, 0, 0, 0, 2, 0, 0, 0, 2);
	//glm::vec3 a = glm::vec3(1, 0, 0);
	//glm::vec3 b = M * a;
	//glm::rotate(2.0, a);
	//glm::vec3 c = vertices[8];
	//拡大行列
	//glm::mat3 M = glm::mat3(1.001, 0, 0, 0, 1.001, 0, 0, 0, 1.001);
	//for (int i = 0; i < vertices.size(); i++) {
		//vertices[i] = M * vertices[i];
		//std::cout << "\nver= "<<vertices[i].x<< vertices[i].y<<vertices[i].z;
	//}
	//std::cout << "\nver= " << vertices[1].x << vertices[1].y << vertices[1].z;
	//std::cout << calc_OBJFile;

	return rollmat;
}

glm::mat4 ObjMoveRollWithCamera(int i, glm::vec3 position, float hA, float vA) {

	//double x = movecount;

	double x, y, z;
	float Ax, Az;
	//glm::vec3(4, 1, 2);
	x = position.x;
	y = position.y;
	z = position.z;


	glm::mat4 movematrix;
	glm::mat4 movemat;
	glm::mat4 rollmat;

	rollmat = ObjRoll(i, hA);

	Ax = sin(hA);
	Az = cos(hA);
	//Ax = 0;
	//Az = 1;

	//std::cout << "difhA=" <<difhA << std::endl;
	//std::cout << "Ax,Az=" << Ax << "," << Az << std::endl;
	if (i == 2) {
		//std::cout << "x,y,z="<<x<<y<<z<<std::endl;
		//glRotatef(10, 1.0f, 0.0f, 0.0f);
		//Model = getModelMatrix(i);
		//平行移動の変換行列
		movemat = glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, x + Ax, y, z + Az, 1);

		movemat = movemat * rollmat;

		//movemat = glm::mat4(1.0f) * movematrix;
		//mat4 ModelMatrix = RotationMatrix ;
		//mat4 ModelMatrix = TranslationMatrix * RotationMatrix * ScaleMatrix;
		// MVP行列を作るためにモデル行列を使えます。

	}

	return movemat;

}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
