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
	//glutFullScreen();
	dimencoInit();
#endif

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

	//-----------------------------------------------------------------------
	// 描画に使うobjの初期化
	//-----------------------------------------------------------------------
	OBJFile[0].LoadFile("obj/MapSmall.obj");
	//OBJFile[0].LoadFile("obj/Map01uv.obj");
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
	OBJFile[10].LoadFile("obj/applealert.obj");

	//-----------------------------------------------------------------------
	// 描画に使うbmpの初期化
	//-----------------------------------------------------------------------
	Texture[0].Load("bmp/stonewall.bmp");
	//Texture[1].Load("bmp/mapbottom.bmp");
	Texture[1].Load("bmp/map4.bmp");
	Texture[2].Load("bmp/sampletex.bmp");
	Texture[3].Load("bmp/start.bmp");
	Texture[4].Load("bmp/black.bmp");
	Texture[5].Load("bmp/enter.BMP");
	Texture[6].Load("bmp/result.bmp");
	//Texture[7].Load("bmp/subdermal_hand.bmp");
	//Texture[7].Load("bmp/OctoBody.bmp");
	//Texture[7].Load("bmp/pigfbx1.bmp");
	Texture[7].Load("bmp/lowpig.bmp");
	Texture[8].Load("bmp/lowapple.bmp");
	Texture[9].Load("bmp/sky.bmp");

	Texture[10].Load("bmp/apple/apple1.bmp");
	Texture[11].Load("bmp/apple/apple2.bmp");
	Texture[12].Load("bmp/apple/apple3.bmp");
	Texture[13].Load("bmp/apple/apple4.bmp");

	Texture[14].Load("bmp/alert/alert_down.bmp");
	Texture[15].Load("bmp/alert/alert_up.bmp");
	Texture[16].Load("bmp/alert/alert_left.bmp");
	Texture[17].Load("bmp/alert/alert_right.bmp");

	Texture[18].Load("bmp/time/1_00.bmp");

	//Texture[19].Load("bmp/time/1_30.bmp");
	//Texture[20].Load("bmp/time/3_00.bmp");
	//Texture[20].Load("bmp/time/2_00.bmp");
	//Texture[21].Load("bmp/time/2_30.bmp");
	//Texture[22].Load("bmp/time/3_00.bmp");
	//Texture[23].Load("bmp/time/1sec.bmp");
	//Texture[24].Load("bmp/time/2sec.bmp");
	//Texture[24].Load("bmp/time/3sec.bmp");
	//Texture[25].Load("bmp/time/3sec.bmp");

	//Texture[26].Load("bmp/time/4sec.bmp");
	//Texture[27].Load("bmp/time/5sec.bmp");
	//Texture[28].Load("bmp/time/6sec.bmp");
	//Texture[29].Load("bmp/time/7sec.bmp");
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
	calc_OBJFile[0] = "obj/MapSmall.obj";
	//calc_OBJFile[0] = "obj/Map01uv.obj";
	calc_OBJFile[7] = "obj/lowpigO.obj";
	calc_OBJFile[8] = "obj/lowapple.obj";


	for (int i = 0; i < N; i++) {
		if (i == 0 || i == 7 || i == 8) {
			std::cout << "i=" << i << std::endl;
			if (!loadOBJ(calc_OBJFile[i].c_str(), vertices[i], uvs[i], normals[i], ver[i])) {	// もしobjファイルを読み込めなかったら
				std::cout << "[ERROR] No File of " << calc_OBJFile[i] << '\n';					// エラー文を返す
			}
			prosessingOfOBJ(&ver[i], &vertexbuffer[i], vertices[i], normals[i], uvs[i], calc_OBJFile[i], &uvbuffer[i], &VertexArrayID[i]);
		}
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

	position.x = -12, position.y = 1, position.z = -15.5;//カメラの位置
	//position.x = 0, position.y = 0, position.z = 0;//カメラの位置
	direction.x = 0, direction.y = 0, direction.z = 0;//何でもいい
	horizontalAngle = 3.14f / 2.0f;//カメラの向き
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
	gluPerspective(30.0, (double)screenWidth / (double)screenHeight, 3.0f, 100.0);

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


#ifdef USE_DIMENCO_OPENGL_INTERFACE
	dimencoSetBackgroundState();
#endif
	view3D();
	//glLoadIdentity();

	if (ModeSelect == 0) {
		if (sound_check == 0) {
			soundStop(&sbgmPlay);
		}

		StartMode();

		if (sound_check == 0) {
			//playMusic(ALL, PAUSE);
			//playMusic(START1, PLAY_LOOP);
			soundPlay(bgmStart, &sbgmStart, true, volume_BGM);
			sound_check = 1;
		}
	}

	if (ModeSelect == 1) {
		if (sound_check == 1) {
			//playMusic(START1, PAUSE);
			//playMusic(MAIN2, PLAY_LOOP);
			soundStop(&sbgmStart);
		}

		PlayMode();

		if (sound_check == 1) {
			soundPlay(bgmPlay, &sbgmPlay, true, volume_BGM);
			sound_check = 2;
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

	if (key == 'a') {
		//xpos -= 48;
		objflag.xposDec = true;
	}
	if (key == 'd') {
		//xpos += 48;
		objflag.xposInc = true;
	}
	if (key == 's') {
		//ypos += 96;
		objflag.yposInc = true;
	}
	if (key == 'w') {
		//ypos -= 96;
		objflag.yposDec = true;
	}

	if (key == 'z') {
		//FoV = initialFoV - 5 * 3.0f;
		objflag.FoVInc = true;
	}
	if (key == 'x') {
		//FoV = initialFoV + 5 * 3.0f;
		objflag.FoVDec = true;
	}
	if (key == 'g') {
		ModeSelect = 1;
	}
	if (key == 't') {
		//std::cout << "cuTime=" << cuTime;
		//std::cout << "\nlaTime=" << laTime;
	}
	if (key == 'c') {
		//rot += 0.01;
		//std::cout << "rot = " << rot << std::endl;
	}
	if (key == escKey) {
		exit(0);
	}
}

//--------------------------------------------------------------------------------------------------------
// カーソルキーやF7キーを押してる間の処理
//--------------------------------------------------------------------------------------------------------
void special_key(int key, int x, int y) {

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

	//隠しコマンド Debug用
	if (key == GLUT_KEY_F7) {
		if (ModeSelect == 0) {
			ModeSelect = 1;
			sound_check = 1;
		}
		else if (ModeSelect == 1) {
			ModeSelect = 0;
			sound_check = 0;
		}
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
	speed = 0.75f;              //主人公の座標を動かす速さ
	hspeed = 1.0f;
	int changePos = 2;			//カメラの向きを動かす速さ

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
		position += direction * deltaTime * speed;

		if (hit(cube) == true) {
			if (countCollisionWall == 0) {
				soundStop(&sseCollisionWall);
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
	if (objflag.keyDOWN) {
		position -= direction * deltaTime * speed;

		if (hit(cube) == true) {
			if (countCollisionWall == 0) {
				soundStop(&sseCollisionWall);
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
	if (objflag.keyRIGHT) {
		position += right_vec3 * deltaTime * speed;

		if (hit(cube) == true) {
			if (countCollisionWall == 0) {
				soundStop(&sseCollisionWall);
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
	if (objflag.keyLEFT) {
		position -= right_vec3 * deltaTime * speed;

		if (hit(cube) == true) {
			if (countCollisionWall == 0) {
				soundStop(&sseCollisionWall);
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
	speed = 4.0f; // 3 units / second

}

//--------------------------------------------------------------------------------------------------------
// glutMainLoop() を通してディスプレイ コールバックが呼び出される
//--------------------------------------------------------------------------------------------------------
static void Idle(void)
{
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

	gluLookAt(
		0.125, 0.08, 5.5, // ワールド空間でカメラは(4,3,3)にあります。
		//enepos_x2, enepos_y2, enepos_z2,    // 視界の調整(追加 6/12)
		0.125, 0.08, 0,
		0.0, 1.0, 0.0);                     // 視界の上方向のベクトルx,y,z

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//#ifdef USE_DIMENCO_OPENGL_INTERFACE
	//	dimencoSetZBufState();
	//#endif

	// テクスチャマッピング有効化 (追加 bmp)
	glEnable(GL_TEXTURE_2D);
	//　テクスチャをバインド (追加 bmp)
	glBindTexture(GL_TEXTURE_2D, Texture[3].ID);
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


	if (flush_cnt < 100) {
		// テクスチャマッピング有効化 (追加 bmp)
		glEnable(GL_TEXTURE_2D);
		//　テクスチャをバインド (追加 bmp)
		glBindTexture(GL_TEXTURE_2D, Texture[5].ID);
		glPushMatrix();
		OBJFile[5].Draw();
		glPopMatrix();
	}
	else if (flush_cnt == 200) {
		flush_cnt = 0;
	}
	flush_cnt++;

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
	//disFromPig = 100;
	//disFromPig += 0.015625;
	//std::cout << "dis = " << disFromPig << std::endl;
	//if (disFromPig > 60) {
	//	disFromPig = 1;
	//}
	//decideVolumeOfPigCry(disFromPig);

	//soundPlay(sePig, &ssePig, false, volume_PIG);
	//volume_PIG += 0.0625f;
	//if (volume_PIG > 3.0f) {
	//	volume_PIG = 0.0f;
	//}
	//std::cout << volume_PIG << '\n';

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


	//time_t cuTime;
	//currentTime = glfwGetTime();
	//deltaTime = float(currentTime - lastTime);
	//lastTime = currentTime;
	//time(&cuTime);
	//deltaTime = float(cuTime - laTime);
	deltaTime = 0.016666;
	//laTime = cuTime;

	//動かすときに使ってる(仮)
	movecount += 0.001;
	moveOBJ();
	// シェーダを使う


	// カメラのほうを向いていない法線の三角形をカリングします。
	glEnable(GL_CULL_FACE);


	//glfwPollEvents();

	//壁
	// テクスチャマッピング有効化 (追加 bmp)
	glEnable(GL_TEXTURE_2D);
	//　テクスチャをバインド (追加 bmp)
	glBindTexture(GL_TEXTURE_2D, Texture[0].ID);
	glPushMatrix();
	OBJFile[0].Draw();
	glPopMatrix();

	//床
	// テクスチャマッピング有効化 (追加 bmp)
	glEnable(GL_TEXTURE_2D);
	//　テクスチャをバインド (追加 bmp)
	glBindTexture(GL_TEXTURE_2D, Texture[1].ID);
	glPushMatrix();
	glTranslatef(0.0, 0.0, -2.0);
	glScalef(0.78f, 1.0f, 0.85f);
	OBJFile[1].Draw();
	glPopMatrix();

	//時間
	// テクスチャマッピング有効化 (追加 bmp)
	glEnable(GL_TEXTURE_2D);
	//　テクスチャをバインド (追加 bmp)
	glBindTexture(GL_TEXTURE_2D, Texture[18].ID);
	glPushMatrix();
	ObjMoveRoll_Camera(horizontalAngle, verticalAngle, 0);
	//glTranslatef(-11.0, 1.05, -15.6);
	//glRotatef(90.0f,0.0f,1.0f,0.0f);
	OBJFile[2].Draw();
	glPopMatrix();

	//取得アイテム
	// テクスチャマッピング有効化 (追加 bmp)
	glEnable(GL_TEXTURE_2D);
	//　テクスチャをバインド (追加 bmp)
	glBindTexture(GL_TEXTURE_2D, Texture[10].ID);
	glPushMatrix();
	ObjMoveRoll_Camera(horizontalAngle, verticalAngle, 1);
	//glTranslatef(-11.0, 1.05, -15.6);
	//glRotatef(90.0f,0.0f,1.0f,0.0f);
	OBJFile[10].Draw();
	glPopMatrix();

	//アラート
	// テクスチャマッピング有効化 (追加 bmp)
	glEnable(GL_TEXTURE_2D);
	//　テクスチャをバインド (追加 bmp)
	glBindTexture(GL_TEXTURE_2D, Texture[15].ID);
	glPushMatrix();
	ObjMoveRoll_Camera(horizontalAngle, verticalAngle, 2);
	//glTranslatef(-11.0, 1.05, -15.6);
	//glRotatef(90.0f,0.0f,1.0f,0.0f);
	OBJFile[10].Draw();
	glPopMatrix();

	//pig
	// テクスチャマッピング有効化 (追加 bmp)
	glEnable(GL_TEXTURE_2D);
	//　テクスチャをバインド (追加 bmp)
	glBindTexture(GL_TEXTURE_2D, Texture[7].ID);
	glPushMatrix();
	glTranslatef(10.0, -1.0, 0.0);
	OBJFile[7].Draw();
	glPopMatrix();

	//apple1
	// テクスチャマッピング有効化 (追加 bmp)
	glEnable(GL_TEXTURE_2D);
	//　テクスチャをバインド (追加 bmp)
	glBindTexture(GL_TEXTURE_2D, Texture[8].ID);
	glPushMatrix();
	glTranslatef(28.0, 0.0, 0.0);
	OBJFile[8].Draw();
	glPopMatrix();

	//apple2
// テクスチャマッピング有効化 (追加 bmp)
	glEnable(GL_TEXTURE_2D);
	//　テクスチャをバインド (追加 bmp)
	glBindTexture(GL_TEXTURE_2D, Texture[8].ID);
	glPushMatrix();
	glTranslatef(10.0, 0.0, -27.5);
	OBJFile[8].Draw();
	glPopMatrix();

	//apple3
// テクスチャマッピング有効化 (追加 bmp)
	glEnable(GL_TEXTURE_2D);
	//　テクスチャをバインド (追加 bmp)
	glBindTexture(GL_TEXTURE_2D, Texture[8].ID);
	glPushMatrix();
	glTranslatef(5.0, 0.0, -27.5);
	OBJFile[8].Draw();
	glPopMatrix();

	//apple4
// テクスチャマッピング有効化 (追加 bmp)
	glEnable(GL_TEXTURE_2D);
	//　テクスチャをバインド (追加 bmp)
	glBindTexture(GL_TEXTURE_2D, Texture[8].ID);
	glPushMatrix();
	glTranslatef(-18.5, 0.0, 0.0);
	OBJFile[8].Draw();
	glPopMatrix();




	//sky
	// テクスチャマッピング有効化 (追加 bmp)
	glEnable(GL_TEXTURE_2D);
	//　テクスチャをバインド (追加 bmp)
	glBindTexture(GL_TEXTURE_2D, Texture[9].ID);
	glPushMatrix();
	OBJFile[9].Draw();
	glPopMatrix();

	//std::cout << "dis=" << disFromPig << std::endl;

}

//--------------------------------------------------------------------------------------------------------
// 壁に衝突しているか否かを返すbool型関数
//--------------------------------------------------------------------------------------------------------
bool hit(double cube[47][4]) {
	for (int i = 0; i < 47; i++) {
		if ((position.x < cube[i][0] && position.x > cube[i][1]) &&
			(position.z < cube[i][2] && position.z > cube[i][3])) {
			return true;
		}
	}
	return false;
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

	if (i == 0) {

		Ax = 1.2*sin(hA + pi / 36.0);
		Az = 1.2*cos(hA + pi / 36.0);

		glTranslatef(x + Ax, y + 0.15, z + Az);
		glScalef(0.5f, 0.5f, 0.5f);
		glRotatef(rolldegree, 0.0f, 1.0f, 0.0f);

	}
	else if (i == 1) {


		Ax = sin(hA + pi / 24.0);
		Az = cos(hA + pi / 24.0);

		glTranslatef(x + Ax, y - 0.2, z + Az);
		glScalef(0.5f, 0.5f, 0.5f);
		glRotatef(rolldegree, 0.0f, 1.0f, 0.0f);

	}
	else if (i == 2) {

		Ax = sin(hA + pi / 48.0);
		Az = cos(hA + pi / 48.0);

		glTranslatef(x + Ax, y - 0.2, z + Az);
		glScalef(0.5f, 0.5f, 0.5f);
		glRotatef(rolldegree, 0.0f, 1.0f, 0.0f);



	}
	//Debug
	//std::cout << "x=" << x + Ax << ",y=" << y+0.05 << ",z=" << z + Az << std::endl;
	//movemat = glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, x + Ax, y, z + Az, 1);
	//position.x = -12, position.y = 1, position.z = -15.5
	//glTranslatef(-11.0, 1.05, -15.6);
	//glRotatef(90.0f,0.0f,1.0f,0.0f);
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


	//---alutの機能の初期化(SE)---//
	seCollisionWall = alutCreateBufferFromFile("SE/CollisionWall.wav");
	seGetItem = alutCreateBufferFromFile("SE/GetItem.wav");
	sePig = alutCreateBufferFromFile("SE/Pig2.wav");

	//volume_PIG = 0.0f;
	//soundPlay(sePig, &ssePig, false, volume_PIG);
}

//----------------------------------------------------
// 音楽を再生
//----------------------------------------------------
void soundPlay(ALuint buf, ALuint *source, bool loop, float volume) {
	alGenSources(1, source);						// 音データのソースを生成
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
		ALint state;
		alGetSourcei(*source, AL_SOURCE_STATE, &state);
		//if (buf == seCollisionWall) {
		//std::cout << state << '\n';
		if (state == AL_INITIAL) {
			//std::cout << "INITIAL\n";
		}
		else if (state == AL_PLAYING) {
			//std::cout << "PLAYING\n";
		}
		else if (state == AL_STOPPED) {
			//std::cout << "STOP\n";
		}
		else if (state == AL_PAUSED) {
			//std::cout << "PAUSE\n";
		}
		else {
			//std::cout << state << '\n';
		}
		//}
		if (state == AL_INITIAL) {
			//std::cout << "PLAY1\n";
			if (buf == sePig) volume = 0;
			alSourcef(*source, AL_GAIN, volume);			// 音量
			alSourcei(*source, AL_BUFFER, buf);				// 音データのバッファを指定
			alSourcePlay(*source);							// SEを再生する
			//std::cout << "PLAY1\n";
		}
		else {
			if ((state != AL_PLAYING) && (state != AL_INITIAL)) {							// 再生されていなければ
				//std::cout << "volume = " << volume << std::endl;
				//if (getOffset(*source) == 0) {						// 再生されていなければ
					//soundStop(source);								// 一度不要なバッファを削除してから
				alDeleteBuffers(1, source);
				//volume_PIG = 0.0f;
				//std::cout << "volume=" << volume << std::endl;
				alGenSources(1, source);						// 音データのソースを生成
				alSourcef(*source, AL_GAIN, volume);			// 音量
				alSourcei(*source, AL_BUFFER, buf);				// 音データのバッファを指定
				alSourcePlay(*source);							// SEを再生する
				//std::cout << "PLAY2\n";
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
}

//----------------------------------------------------
// 音楽を停止
//----------------------------------------------------
void soundStop(ALuint *source) {
	//std::cout << "soundStop" << source << std::endl;	
	alSourceStop(*source);								// 音の停止
	alDeleteSources(1, source);							// 音データのバッファの削除
	source = 0;											// 音データの初期化
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
// 壁との当たり判定を行うための壁の端点座標を決定する関数
//--------------------------------------------------------------------------------------------------------

//void decideCube(std::vector<glm::vec3> &vertices) {
//	double x_max = 0, x_min = 0, z_max = 0, z_min = 0;
//	double tmp;												//max or minの判定を受ける座標をいったん代入するための変数
//	double alpha = 0.0;										//壁に厚みを付けることで壁の向こう側を見えないようにする
//	int ik = 0;
//	int k = 0;
//	int num_of_all_rec = ((int)vertices.size() - 6) / 36;	//直方体数. 2を引いているのは, なぜか1つだけ頂点数が36ではなく42個ある直方体があるから
//	int mistery = 14;										//MapSmall.objにおいて, 14番目のcubeの座標だけなぜかverticesの数が8個ではなく10個ある .objファイルの647～656行目参照	
//	int num_of_rec_vertex = 36;								//直方体の頂点数
//
//	//vertices[ik].xの決定
//	while (1) {
//		if (k < mistery) {
//			if (ik % 36 == 0) {
//				tmp = vertices[ik].x;
//				ik++;
//			}
//			while (tmp == vertices[ik].x) {
//				ik++;
//			}
//			if (tmp > vertices[ik].x) {
//				x_max = tmp;
//				x_min = vertices[ik].x;
//			}
//			else {
//				x_max = vertices[ik].x;
//				x_min = tmp;
//			}
//			cube[k][0] = x_max + alpha;
//			cube[k][1] = x_min - alpha;
//			ik = (k+1) * 36;
//			k++;
//		}
//		else if (k == mistery) {
//			if (ik == mistery * 36 && ik % 36 == 0) {
//				tmp = vertices[ik].x;
//				ik++;
//			}
//			while (tmp == vertices[ik].x) {
//				ik++;
//			}
//			if (tmp > vertices[ik].x) {
//				x_max = tmp;
//				x_min = vertices[ik].x;
//			}
//			else {
//				x_max = vertices[ik].x;
//				x_min = tmp;
//			}
//			cube[k][0] = x_max + alpha;
//			cube[k][1] = x_min - alpha;
//			ik = (k + 1) * 36 + 6;
//			k++;
//		}
//		else {
//			if (ik % 36 == 6) {
//				tmp = vertices[ik].x;
//				ik++;
//			}
//			while (tmp == vertices[ik].x) {
//				ik++;
//			}
//			if (tmp > vertices[ik].x) {
//				x_max = tmp;
//				x_min = vertices[ik].x;
//			}
//			else {
//				x_max = vertices[ik].x;
//				x_min = tmp;
//			}
//			cube[k][0] = x_max + alpha;
//			cube[k][1] = x_min - alpha;
//			ik = (k + 1) * 36 + 6;
//			k++;
//		}
//		if (k >= num_of_all_rec) { break; }
//	}
//
//	k = 0;
//	ik = 0;
//
//	//vertices[ik].zの決定
//	while (1) {
//		if (k < mistery) {
//			if (ik % 36 == 0) {
//				tmp = vertices[ik].z;
//				ik++;
//			}
//			while (tmp == vertices[ik].z) {
//				ik++;
//			}
//			if (tmp > vertices[ik].z) {
//				x_max = tmp;
//				x_min = vertices[ik].z;
//			}
//			else {
//				x_max = vertices[ik].z;
//				x_min = tmp;
//			}
//			cube[k][0] = x_max + alpha;
//			cube[k][1] = x_min - alpha;
//			ik = (k + 1) * 36;
//			k++;
//		}
//		else if (k == mistery) {
//			if (ik == mistery * 36 && ik % 36 == 0) {
//				tmp = vertices[ik].z;
//				ik++;
//			}
//			while (tmp == vertices[ik].z) {
//				ik++;
//			}
//			if (tmp > vertices[ik].z) {
//				x_max = tmp;
//				x_min = vertices[ik].z;
//			}
//			else {
//				x_max = vertices[ik].z;
//				x_min = tmp;
//			}
//			cube[k][0] = x_max + alpha;
//			cube[k][1] = x_min - alpha;
//			ik = (k + 1) * 36 + 6;
//			k++;
//		}
//		else {
//			if (ik % 36 == 6) {
//				tmp = vertices[ik].z;
//				ik++;
//			}
//			while (tmp == vertices[ik].z) {
//				ik++;
//			}
//			if (tmp > vertices[ik].z) {
//				x_max = tmp;
//				x_min = vertices[ik].z;
//			}
//			else {
//				x_max = vertices[ik].z;
//				x_min = tmp;
//			}
//			cube[k][0] = x_max + alpha;
//			cube[k][1] = x_min - alpha;
//			ik = (k + 1) * 36 + 6;
//			k++;
//		}
//		if (k >= num_of_all_rec) { break; }
//	}
//
//	std::cout << "\n\n[decideCube]\n";
//	std::cout << "k = (" << k << '\n';
//	std::cout << "num_of_all_rec = " << num_of_all_rec << std::endl;
//	std::cout << std::endl;
//}
void decideCube(std::vector<glm::vec3> &vertices) {
	double x_max = 0, x_min = 0, z_max = 0, z_min = 0;
	double tmp;
	int ik = 0, il = 0;
	int k = 1, l = 1;

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
		cube[k - 1][0] = x_max;
		cube[k - 1][1] = x_min;
		ik = k * 36;
		k++;
		if (k == 48) { break; }
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
		cube[l - 1][2] = z_max;
		cube[l - 1][3] = z_min;
		il = l * 36;
		l++;
		if (l == 48) { break; }
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
	if (disFromPig >= 50) {
		volume_PIG = 0.0f;
	}
	else {
		volume_PIG = (float)1.0 / (float)disFromPig;
		soundPlay(sePig, &ssePig, false, volume_PIG);
	}
	//std::cout << "dis="<<disFromPig << std::endl;
}























//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// オブジェクトの読み込みなどのブラックボックス化された関数たち
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


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
