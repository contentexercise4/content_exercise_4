// TMF_glut.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。

#include"pch.h"
#include "lib.h"
#include "MusicController.h"
//#include <glm/gtc/quaternion.hpp>
//#include <glm/gtx/quaternion.hpp>
//#include <math.h>
//#include <cmath>

//glm::mat4 Movemat= glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);;


//objFlags objflag;


//time_t cuTime;
//time_t laTime;

//----------------------------------------------------
// メイン関数
//----------------------------------------------------

int main(int argc, char* argv[]) {
	srand((unsigned)time(NULL));

	//alutInit(&argc, argv);

	glutInit(&argc, argv); //環境の初期化
	glutInitWindowPosition(WindowPositionX, WindowPositionY);  //ウィンドウの位置の指定
	glutInitWindowSize(WindowWidth, WindowHeight);             //ウィンドウサイズの指定
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE); //ディスプレイモードの指定
	glutCreateWindow(WindowTitle); //ウィンドウの作成



	glutDisplayFunc(Display);			//描画時に呼び出される関数を指定する（関数名：Display）
	glutReshapeFunc(Reshape);			//ごめんわからない
	glutIdleFunc(Idle);					//プログラムアイドル状態時に呼び出される関数
	glutKeyboardFunc(Keyboard);			//キーボード入力時に呼び出される関数を指定する（関数名：Keyboard）
	glutSpecialFunc(special_key);		//特殊キーの設定
	glutKeyboardUpFunc(KeyboadUP);		//キーボードから手を離したときに発動
	glutSpecialUpFunc(special_keyUP);	//特殊キーから手を離したときに発動

	Initialize();						//初期設定の関数を呼び出す

	//initState();						//dimenco.hに関係している
	glutMainLoop();						//ループ関数. モード選択によってゲームへ面を遷移していく
	return 0;
}

static void initState(void)
{


	glDisable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0, (double)screenWidth / (double)screenHeight, 3.0f, 100.0);

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

//----------------------------------------------------
// 描画の関数
//----------------------------------------------------
void Display(void) {

	glClearColor(0.2f, 0.2f, 0.2f, 0.0f);					// おまじない
	glEnable(GL_DEPTH_TEST);								// デプステストを有効にする
	glDepthFunc(GL_LESS);									// 前のものよりもカメラに近ければ、フラグメントを受け入れる
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// スクリーンをクリアする


	view3D();

	glLoadIdentity();


	ProjectionMatrix = getProjectionMatrix();
	ViewMatrix = getViewMatrix();
	//ModelMatrix = getModelMatrix(0);
	ModelMatrix = glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	//これは機能する
	MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

	//// 現在バインドしているシェーダの"MVP" uniformに変換を送る
	//// レンダリングする各モデルごと、なぜならMVPが違うからです。(少なくともMの部分が違います。)
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);


	if (ModeSelect == 0) {
		if (sound_check == 0) {
			playMusic(ALL, PAUSE);
			playMusic(START1, PLAY_LOOP);
			sound_check = 1;
		}
		StartMode();
	}

	if (ModeSelect == 1) {
		if (sound_check == 1) {
			playMusic(START1, PAUSE);
			playMusic(MAIN2, PLAY_LOOP);
			sound_check = 2;
		}
		PlayMode();
	}


	//これの生態を探る

	glutSwapBuffers(); //glutInitDisplayMode(GLUT_DOUBLE)でダブルバッファリングを利用可
}


void Initialize() {

	//　GLEWの初期化
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Error : GLEWの初期化に失敗しました\n";
		exit(0);
	}

	InitMusic();
	InitObjFlag();

	OBJFile[0] = "obj/Map01uv.obj";
	OBJFile[1] = "obj/Map01Bottomuv.obj";
	OBJFile[2] = "obj/itemsample.obj";
	OBJFile[3] = "obj/start.obj";
	OBJFile[4] = "obj/title.obj";
	OBJFile[5] = "obj/enter.obj";
	OBJFile[6] = "obj/start.obj";
	//OBJFile[7] = "obj/hand1.obj";
	//OBJFile[7] = "obj/octo.obj";
	OBJFile[7] = "obj/lowpig.obj";
	//OBJFile[7] = "obj/lowpigO.obj";
	OBJFile[8] = "obj/lowapple.obj";

	Texture[0] = loadBMP_custom("bmp/texsample.bmp");
	Texture[1] = loadBMP_custom("bmp/mapbottom.bmp");
	Texture[2] = loadBMP_custom("bmp/sampletex.bmp");
	Texture[3] = loadBMP_custom("bmp/start.bmp");
	Texture[4] = loadBMP_custom("bmp/black.bmp");
	Texture[5] = loadBMP_custom("bmp/enter.BMP");
	Texture[6] = loadBMP_custom("bmp/result.bmp");
	//Texture[7] = loadBMP_custom("bmp/subdermal_hand.bmp");
	//Texture[7] = loadBMP_custom("bmp/OctoBody.bmp");
	Texture[7] = loadBMP_custom("bmp/lowpig.bmp");
	//Texture[7] = loadBMP_custom("bmp/pigfbx1.bmp");
	Texture[8] = loadBMP_custom("bmp/lowapple.bmp");

	for (int i = 0; i < N; i++) {
		//コメントアウトした 722行目 ～ 925行目の処理をまとめた関数
		if (!loadOBJ(OBJFile[i].c_str(), vertices[i], uvs[i], normals[i], ver[i])) {	// もしobjファイルを読み込めなかったら
			printf("[ERROR] No File of %s\n", OBJFile);					// エラー文を返す
		}
		prosessingOfOBJ(&ver[i], &vertexbuffer[i], vertices[i], normals[i], uvs[i], OBJFile[i], &uvbuffer[i], &VertexArrayID[i]);
	}

	decideCube(vertices[0]);			//当たり判定に用いる配列cube[][]に値を格納する

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	// 三角形を描きます！
	//glDrawArrays(GL_TRIANGLES, 0, 3); // 頂点0から始まります。合計3つの頂点です。&rarr;1つの三角形です。

	glDisableVertexAttribArray(0);

	// 三角形の描画！
	//glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // 12*3頂点は0から始まる -> 12枚の三角形 -> 6枚の正方形


	printf("\n");
	printf("%d\n", vertices[0].size());

	// シェーダからGLSLプログラムを作りコンパイルする。
	programID = LoadShaders("SimpleVertexShader.hlsl", "SimpleFragmentShader.hlsl");
	//GLuint programID2 = LoadShaders("SimpleVertexShader.hlsl", "SimpleFragmentShader.hlsl");


	// 射影行列：45&deg;の視界、アスペクト比4:3、表示範囲：0.1単位  100単位
	ProjectionM = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
	// カメラ行列
	ViewM = glm::lookAt(
		glm::vec3(4, 1, 2), // ワールド空間でカメラは(4,3,3)にあります。
		glm::vec3(1, 1, 1), // 原点を見ています。
		glm::vec3(0, 1, 0)  // 頭が上方向(0,-1,0にセットすると上下逆転します。)
	);
	// モデル行列：単位行列(モデルは原点にあります。)
	for (int i = 0; i < N; i++) {
		ModelM[i] = glm::mat4(1.0f);  // 各モデルを変える！
		if (i == 7) {
			ModelM[i] = glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, -5, -1, 0, 1);;
		}
	}
	//Model = glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);


	for (int i = 0; i < N; i++) {
		printf("ver[%d] = %d\n", i, ver[i]);

	}


	// 下でエスケープキーが押されるのを捉えるのを保証します。
	//glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	movecount = 0;
	p = 0;
	flush = 0;		//1000に到達したら0に戻す

//time(&cuTime);
//time(&laTime);
}

void Keyboard(unsigned char key, int x, int y) {
	//initialFoV = 45.0f;
	//speed = 4.0f;
	//mouseSpeed = 0.05f;
	//int xpos = 512, ypos = 384;
	//FoV = 45.0f;
	//std::cout << "key = " << key << std::endl;


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

void special_key(int key, int x, int y) {

	//std::cout << "special key = " << key << std::endl;
	//speed = 4.0f; // 3 units / second

	if (key == GLUT_KEY_UP) {
		//position += direction * deltaTime * speed;
		if (hit(cube) == true) {
			playMusic(COLLISION, PLAY_INSTANCE);
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
			playMusic(COLLISION, PLAY_INSTANCE);
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
		//position += right * deltaTime * speed;
		if (hit(cube) == true) {
			playMusic(COLLISION, PLAY_INSTANCE);
			//position -= right * deltaTime * speed;
			objflag.hitRIGHT = true;
			objflag.keyRIGHT = false;
		}
		else {
			objflag.keyRIGHT = true;
			objflag.hitRIGHT = false;
		}
	}
	if (key == GLUT_KEY_LEFT) {
		//position -= right * deltaTime * speed;
		if (hit(cube) == true) {
			playMusic(COLLISION, PLAY_INSTANCE);
			//position += right * deltaTime * speed;
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
}

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

void special_keyUP(int key, int x, int y) {
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
		//position += right * deltaTime * speed;
		objflag.keyRIGHT = false;
		objflag.hitRIGHT = false;
		//if (hit(cube) == true) {
			//playMusic(COLLISION, PLAY_INSTANCE);
			//position -= right * deltaTime * speed;
		//}
	}
	if (key == GLUT_KEY_LEFT) {
		//position -= right * deltaTime * speed;
		objflag.keyLEFT = false;
		objflag.hitLEFT = false;
		//if (hit(cube) == true) {
			//playMusic(COLLISION, PLAY_INSTANCE);
			//position += right * deltaTime * speed;
		//}
	}
}

void moveOBJ() {
	initialFoV = 45.0f;
	mouseSpeed = 0.05f;
	int xpos = 512, ypos = 384;	//いまだに意味不明
	FoV = 45.0f;				//なんか難しいやつ
	speed = 0.3f;               //主人公の座標を動かす速さ
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
		position -= right * deltaTime * hspeed;
	}
	if (objflag.hitLEFT) {
		position += right * deltaTime * hspeed;
	}
	*/

	//--主人公の座標を制御--//
	if (objflag.keyUP) {
		position += direction * deltaTime * speed;
		if (hit(cube) == true) {
			position -= direction * deltaTime * speed;
		}
	}
	if (objflag.keyDOWN) {
		position -= direction * deltaTime * speed;
		if (hit(cube) == true) {
			position += direction * deltaTime * speed;
		}
	}
	if (objflag.keyRIGHT) {
		position += right * deltaTime * speed;
		if (hit(cube) == true) {
			position -= right * deltaTime * speed;
		}
	}
	if (objflag.keyLEFT) {
		position -= right * deltaTime * speed;
		if (hit(cube) == true) {
			position += right * deltaTime * speed;
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
	right.x = sin(horizontalAngle - 3.14f / 2.0f);
	right.y = 0;
	right.z = cos(horizontalAngle - 3.14f / 2.0f);

	// 上ベクトル：右と前ベクトルに垂直
	glm::vec3 up = glm::cross(right, direction);

	// 射影行列：視野45&deg;、4:3比、描画範囲0.1単位100単位
	ProjectionM = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);
	//ProjectionM = glm::perspective(glm::radians(initialFoV), 4.0f / 3.0f, 0.1f, 100.0f);
	// カメラ行列
	ViewM = glm::lookAt(
		position,				// カメラの位置
		position + direction,	// 目標地点
		up						// 上方向 (0,-1,0にセットすると上下逆さまになります。)
	);

	speed = 4.0f; // 3 units / second
	// 方向：球面座標から直角座標に変換します。
	direction.x = cos(verticalAngle) * sin(horizontalAngle);
	direction.y = sin(verticalAngle);
	direction.z = cos(verticalAngle) * cos(horizontalAngle);

	// 右ベクトル
	right.x = sin(horizontalAngle - 3.14f / 2.0f);
	right.y = 0;
	right.z = cos(horizontalAngle - 3.14f / 2.0f);

	// 上ベクトル：右と前ベクトルに垂直
	up = glm::cross(right, direction);

	//std::cout << "(x, y, z) = (" << position.x << ", " << position.y << ", " << position.z << ")\n";
	//std::cout << "(x, y, z) = (" << direction.x << ", " << direction.y << ", " << direction.z << ")\n";

	// 射影行列：視野45&deg;、4:3比、描画範囲0.1単位100単位
	ProjectionM = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);
	//ProjectionM = glm::perspective(glm::radians(initialFoV), 4.0f / 3.0f, 0.1f, 100.0f);
	// カメラ行列
	ViewM = glm::lookAt(
		position,				// カメラの位置
		position + direction,	// 目標地点
		up						// 上方向 (0,-1,0にセットすると上下逆さまになります。)
	);
}

static void Idle(void)
{
	glutPostRedisplay();
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

void StartMode() {
	flush++;


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//視点の設定------------------------------
	//gluLookAt(
	//	position.x, position.y, position.z,		// 視点の位置x,y,z;
	//	position.x + direction.x, position.y + direction.y, position.z + direction.z,
	//	0.0, 1.0, 0.0							// 視界の上方向のベクトルx,y,z
	//);
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	int xpos = 512, ypos = 384;
	FoV = 45.0f;

	// 新たな方向を計算します。
	horizontalAngle += mouseSpeed * deltaTime * float(1024 / 2 - xpos);
	verticalAngle += mouseSpeed * deltaTime * float(768 / 2 - ypos);

	// 方向：球面座標から直角座標に変換します。
	direction.x = cos(verticalAngle) * sin(horizontalAngle);
	direction.y = sin(verticalAngle);
	direction.z = cos(verticalAngle) * cos(horizontalAngle);

	// 右ベクトル
	right.x = sin(horizontalAngle - 3.14f / 2.0f);
	right.y = 0;
	right.z = cos(horizontalAngle - 3.14f / 2.0f);

	// 上ベクトル：右と前ベクトルに垂直
	glm::vec3 up = glm::cross(right, direction);

	// 射影行列：視野45&deg;、4:3比、描画範囲0.1単位100単位
	ProjectionM = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);
	//ProjectionM = glm::perspective(glm::radians(initialFoV), 4.0f / 3.0f, 0.1f, 100.0f);
	// カメラ行列
	ViewM = glm::lookAt(
		position,				// カメラの位置
		position + direction,	// 目標地点
		up						// 上方向 (0,-1,0にセットすると上下逆さまになります。)
	);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//スタート画面を描画
	glBindTexture(GL_TEXTURE_2D, Texture[3]);
	glBindVertexArray(VertexArrayID[3]);
	//三角形の描画！
	//glDrawArrays(GL_TRIANGLES, 0, ver4); // 12*3頂点は0から始まる -> 12枚の三角形 -> 6枚の正方形
	glDrawArrays(GL_TRIANGLES, 0, ver[3]); // 12*3頂点は0から始まる -> 12枚の三角形 -> 6枚の正方形

	//真っ黒な画面を張り付けるオブジェクトを描画
	glBindTexture(GL_TEXTURE_2D, Texture[4]);
	glBindVertexArray(VertexArrayID[4]);
	//三角形の描画！
	//glDrawArrays(GL_TRIANGLES, 0, ver5); // 12*3頂点は0から始まる -> 12枚の三角形 -> 6枚の正方形
	glDrawArrays(GL_TRIANGLES, 0, ver[4]); // 12*3頂点は0から始まる -> 12枚の三角形 -> 6枚の正方形

	if (flush < 100) {
		//Enter画面を張り付けるオブジェクトを描画
		glBindTexture(GL_TEXTURE_2D, Texture[5]);
		glBindVertexArray(VertexArrayID[5]);
		//三角形の描画！
		//glDrawArrays(GL_TRIANGLES, 0, ver4); // 12*3頂点は0から始まる -> 12枚の三角形 -> 6枚の正方形
		glDrawArrays(GL_TRIANGLES, 0, ver[5]); // 12*3頂点は0から始まる -> 12枚の三角形 -> 6枚の正方形
	}
	else if (flush == 200) {
		flush = 0;
	}


	//glutSwapBuffers(); //glutInitDisplayMode(GLUT_DOUBLE)でダブルバッファリングを利用可

}

void PlayMode() {

	if (hit(cube) == true) {
		std::cout << "HIT!!!!!!!!\n";
	}
	else {
		std::cout << "SAFE\n";
	}



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

	//glUseProgram(programID);

	//glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices, GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);


	// 三角形を描きます！
	//glDrawArrays(GL_TRIANGLES, 0, 3); // 頂点0から始まります。合計3つの頂点です。&rarr;1つの三角形です。

	// カメラのほうを向いていない法線の三角形をカリングします。
	glEnable(GL_CULL_FACE);


	//オブジェクトを動かすモジュール
		//prosessingOfMoveOBJ(&ver[2], &vertexbuffer[2], vertices[2], normals[2], uvs[2], OBJFile[2], &uvbuffer[2], &VertexArrayID[2]);
		//prosessingOfMoveOBJ(&ver[7], &vertexbuffer[7], vertices[7], normals[7], uvs[7], OBJFile[7], &uvbuffer[7], &VertexArrayID[7]);
		//prosessingOfMoveOBJ(&ver[8], &vertexbuffer[8], vertices[8], normals[8], uvs[8], OBJFile[8], &uvbuffer[8], &VertexArrayID[8]);



		//壁を描画
	glBindTexture(GL_TEXTURE_2D, Texture[0]);
	glBindVertexArray(VertexArrayID[0]);
	//三角形の描画！
	//glDrawArrays(GL_TRIANGLES, 0, ver); // 12*3頂点は0から始まる -> 12枚の三角形 -> 6枚の正方形
	glDrawArrays(GL_TRIANGLES, 0, ver[0]); // 12*3頂点は0から始まる -> 12枚の三角形 -> 6枚の正方形

	//glDrawArrays(GL_TRIANGLES, 0, 72);
	//glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	//床を描画
	glBindTexture(GL_TEXTURE_2D, Texture[1]);
	glBindVertexArray(VertexArrayID[1]);
	//三角形の描画！
	//glDrawArrays(GL_TRIANGLES, 0, ver2); // 12*3頂点は0から始まる -> 12枚の三角形 -> 6枚の正方形
	glDrawArrays(GL_TRIANGLES, 0, ver[1]); // 12*3頂点は0から始まる -> 12枚の三角形 -> 6枚の正方形

	//アイテムを描画
	glBindTexture(GL_TEXTURE_2D, Texture[2]);
	glBindVertexArray(VertexArrayID[2]);
	//三角形の描画！
	//glDrawArrays(GL_TRIANGLES, 0, ver3); // 12*3頂点は0から始まる -> 12枚の三角形 -> 6枚の正方形
	glDrawArrays(GL_TRIANGLES, 0, ver[2]); // 12*3頂点は0から始まる -> 12枚の三角形 -> 6枚の正方形

	//キャラを描画
	glBindTexture(GL_TEXTURE_2D, Texture[7]);
	glBindVertexArray(VertexArrayID[7]);
	//回転、移動させるための行列計算
	ProjectionMatrix = getProjectionMatrix();
	ViewMatrix = getViewMatrix();
	//ModelMatrix = glm::mat4(5, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	ModelM[7] = ObjMove(7);
	ModelM[7] = ObjRoll(7);
	ModelMatrix = getModelMatrix(7);
	MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

	//三角形の描画！
	//glDrawArrays(GL_TRIANGLES, 0, ver3); // 12*3頂点は0から始まる -> 12枚の三角形 -> 6枚の正方形
	glDrawArrays(GL_TRIANGLES, 0, ver[7]); // 12*3頂点は0から始まる -> 12枚の三角形 -> 6枚の正方形

	//appleを描画
	glBindTexture(GL_TEXTURE_2D, Texture[8]);
	glBindVertexArray(VertexArrayID[8]);
	//回転、移動,拡大させるための行列計算
	ProjectionMatrix = getProjectionMatrix();
	ViewMatrix = getViewMatrix();
	//ModelMatrix = getModelMatrix(8);
	ModelMatrix = glm::mat4(3, 0, 0, 0, 0, 3, 0, 0, 0, 0, 3, 0, 0, 0, 0, 1);
	//ModelMatrix=ObjRoll(8);
	MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
	//三角形の描画！
	//glDrawArrays(GL_TRIANGLES, 0, ver3); // 12*3頂点は0から始まる -> 12枚の三角形 -> 6枚の正方形
	glDrawArrays(GL_TRIANGLES, 0, ver[8]); // 12*3頂点は0から始まる -> 12枚の三角形 -> 6枚の正方形

	//glDisableVertexAttribArray(0);



//glutSwapBuffers(); //glutInitDisplayMode(GLUT_DOUBLE)でダブルバッファリングを利用可
//glfwPollEvents();
}

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

void camerawork() {

}

GLuint loadBMP_custom(const char* imagepath) {
	// BMPファイルのヘッダから読み込まれるデータ
	unsigned char header[54]; // 各BMPファイルは54バイトのヘッダから始まります。
	unsigned int dataPos;     // 実際のデータがファイルのどの位置にあるか
	unsigned int width, height;
	unsigned int imageSize;   // = 横*高さ*3
	// 実際のRGBデータ
	unsigned char* data;

	// ファイルを開きます。
	FILE* file;
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
	dataPos = *(int*) & (header[0x0A]);
	imageSize = *(int*) & (header[0x22]);
	width = *(int*) & (header[0x12]);
	height = *(int*) & (header[0x16]);

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

GLuint loadTGA_glfw(const char* imagepath) {

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

GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path) {

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
	char const* VertexSourcePointer = VertexShaderCode.c_str();
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
	char const* FragmentSourcePointer = FragmentShaderCode.c_str();
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

bool hit(double cube[47][4]) {
	for (int i = 0; i < 47; i++) {
		if ((position.x < cube[i][0] && position.x > cube[i][1]) &&
			(position.z < cube[i][2] && position.z > cube[i][3])) {
			return true;
		}
	}
	return false;
}

void decideCube(std::vector<glm::vec3>& vertices) {
	double x_max = 0, x_min = 0, z_max = 0, z_min = 0;
	double tmp;
	int ik = 0, il = 0;
	int k = 1, l = 1;
	int buf = 0.0;

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
		cube[k - 1][0] = x_max + buf;
		cube[k - 1][1] = x_min - buf;
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
		cube[l - 1][2] = z_max + buf;
		cube[l - 1][3] = z_min - buf;
		il = l * 36;
		l++;
		if (l == 48) { break; }
	}

	std::cout << "\n\n[decideCube]\n";
	std::cout << "(k, l) = (" << k << ", " << l << ")\n";
	std::cout << "vertices.size/36 = " << (int)(vertices.size()) / 36 << std::endl;
	std::cout << std::endl;
}

glm::mat4  getProjectionMatrix() {

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

bool loadOBJ(const char* path, std::vector<glm::vec3>& out_vertices, std::vector<glm::vec2>& out_uvs, std::vector<glm::vec3>& out_normals, int& ver) {

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
	FILE* file;
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

bool loadOBJnoUV(const char* path, std::vector<glm::vec3>& out_vertices, std::vector<glm::vec3>& out_normals, int& ver) {

	std::vector<int> vertexIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec3> temp_normals;

	//FILE * file = fopen(path, "r");
	//if (file == NULL) {
	//	printf("ファイルを開けません!n");
	//	return false;
	//}

	// ファイルを開きます。
	FILE* file;
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

void prosessingOfOBJ(int* ver, GLuint* vertexbuffer, std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals, std::vector<glm::vec2>& uvs, std::string OBJFile, GLuint* uvbuffer, GLuint* VertexArrayID) {
	glGenBuffers(1, vertexbuffer);									// バッファを1つ作り、vertexbufferに結果IDを入れます。
	glBindBuffer(GL_ARRAY_BUFFER, *vertexbuffer);					// 次のコマンドは'vertexbuffer'バッファについてです。
	if (!loadOBJ(OBJFile.c_str(), vertices, uvs, normals, *ver)) {	// もしobjファイルを読み込めなかったら
		printf("[ERROR] No File of %s\n", OBJFile.c_str());					// エラー文を返す
	}
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

void prosessingOfMoveOBJ(int* ver, GLuint* vertexbuffer, std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals, std::vector<glm::vec2>& uvs, std::string OBJFile, GLuint* uvbuffer, GLuint* VertexArrayID) {
	//glGenBuffers(1, vertexbuffer);									// バッファを1つ作り、vertexbufferに結果IDを入れます。
	glBindBuffer(GL_ARRAY_BUFFER, *vertexbuffer);					// 次のコマンドは'vertexbuffer'バッファについてです。
	//if (!loadOBJ(OBJFile.c_str(), vertices, uvs, normals, *ver)) {	// もしobjファイルを読み込めなかったら
	//	printf("[ERROR] No File of %s\n", OBJFile);					// エラー文を返す
	//}

	//std::cout << OBJFile.c_str() << std::endl;

	//std::cout << "w";


	float objx_max = 0, objx_min = 0, objz_max = 0, objz_min = 0;
	float tmp;
	float obji = 0; double objj = 0;


	float obj_speed = 0.001;  //objectのスピード
	float dif_x, dif_z, sq;
	float obj_xspeed, obj_zspeed;


	if (OBJFile == "obj/lowpig.obj") {

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



	//else if (OBJFile == "obj/lowapple.obj") {
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


glm::mat4 ObjMove(int i) {

	float objx_max = 0, objx_min = 0, objz_max = 0, objz_min = 0;
	float tmp;
	float obji = 0; double objj = 0;

	float obj_speed = 0.001;  //objectのスピード 今使ってない
	float dif_x, dif_z, sq;
	float obj_xspeed, obj_zspeed;

	//double x = movecount;
	double x = 0;

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

glm::mat4 ObjRoll(int i) {
	//glBindBuffer(GL_ARRAY_BUFFER, *vertexbuffer);					// 次のコマンドは'vertexbuffer'バッファについてです。
	//glLoadIdentity();
	glm::mat4 rollmat;

	//回転
	glm::vec3 RotationAxis;
	//πの使い方忘れてしまった
	//double RotationAngle = rot;
	//rot += 0.001;
	//std::cout << "rot = " << rot << '\n';
	//if (rot > 6.28) {
	//	rot = 0;
	//}
	int rot = 0;
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
	MyQuaternion = glm::angleAxis(float(rot), RotationAxis);
	// 軸-角からの変換
	// GLMでは角度は度で表すので、変換します。
	//MyQuaternion = glmangleAxis(degrees(RotationAngle), RotationAxis);

	//vec3 EulerAngles(RotationAroundXInRadians, RotationAroundYInRadians, RotationAroundZInRadians);

	rollmat = ModelM[i] * glm::toMat4(MyQuaternion);

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
	//std::cout << OBJFile;

	return rollmat;
}


void playMusic(int file_idx, int command) {
	if (file_idx == START1) {
		if (command == PLAY_LOOP) {
			if (!PlaySound(TEXT("BGM/START1.wav"), NULL, SND_FILENAME | SND_LOOP | SND_ASYNC)) {
				std::cout << "START1.wav を再生できません" << std::endl;
			}
		}
		if (command == PAUSE) {
			if (!PlaySound(TEXT("BGM/START1.wav"), NULL, SND_FILENAME | SND_PURGE | SND_ASYNC)) {
				std::cout << "START1.wav を一時停止できません" << std::endl;
			}
		}
	}
	if (file_idx == MAIN1) {
		if (command == PLAY_LOOP) {
			if (!PlaySound(TEXT("BGM/MAIN1.wav"), NULL, SND_FILENAME | SND_LOOP | SND_ASYNC)) {
				std::cout << "MAIN1.wav を再生できません" << std::endl;
			}
		}
		if (command == PAUSE) {
			if (!PlaySound(TEXT("BGM/MAIN1.wav"), NULL, SND_FILENAME | SND_PURGE | SND_ASYNC)) {
				std::cout << "MAIN1.wav を一時停止できません" << std::endl;
			}
		}
	}
	if (file_idx == MAIN2) {
		if (command == PLAY_LOOP) {
			if (!PlaySound(TEXT("BGM/MAIN2.wav"), NULL, SND_FILENAME | SND_LOOP | SND_ASYNC)) {
				std::cout << "MAIN2.wav を再生できません" << std::endl;
			}
		}
		if (command == PAUSE) {
			if (!PlaySound(TEXT("BGM/MAIN2.wav"), NULL, SND_FILENAME | SND_PURGE | SND_ASYNC)) {
				std::cout << "MAIN2.wav を一時停止できません" << std::endl;
			}
		}
	}
	if (file_idx == ALL) {
		if (command == PAUSE) {
			if (!PlaySound(NULL, NULL, SND_PURGE | SND_ASYNC)) {
				std::cout << "すべてのBGM/SEを一時停止できません" << std::endl;
			}
		}
	}
	if (file_idx == COLLISION) {
		if (command == PLAY_INSTANCE) {
			mciSendCommand(open1.wDeviceID, MCI_PLAY, MCI_NOTIFY, (DWORD_PTR)& play1);
			//mciSendCommand(open1.wDeviceID, MCI_STOP, MCI_NOTIFY, (DWORD_PTR)&play1);
		}
		//if (command == PAUSE) {
		//	mciSendCommand(open1.wDeviceID, MCI_PAUSE, MCI_NOTIFY, (DWORD_PTR)&play1);
		//}
	}
	if (file_idx == GET_ITEM) {
		if (command == PLAY_INSTANCE) {
			mciSendCommand(open2.wDeviceID, MCI_PLAY, MCI_NOTIFY, (DWORD_PTR)& play2);
			//mciSendCommand(open2.wDeviceID, MCI_STOP, MCI_NOTIFY, (DWORD_PTR)&play2);
		}
		//if (command == PAUSE) {
		//	mciSendCommand(open2.wDeviceID, MCI_PAUSE, MCI_NOTIFY, (DWORD_PTR)&play2);
		//}
	}
}

void InitMusic() {

	//char buf[1024];

	sound_check = 0;

	open1.lpstrDeviceType = (LPCWSTR)MCI_DEVTYPE_WAVEFORM_AUDIO;
	open1.lpstrElementName = TEXT("SE/collision.wav");
	open2.lpstrDeviceType = (LPCWSTR)MCI_DEVTYPE_WAVEFORM_AUDIO;
	open2.lpstrElementName = TEXT("SE/get_item.wav");

	char szCommand[256];
	mciSendString((LPTSTR)szCommand, 0, 0, 0);

	mciSendCommand(0, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_TYPE_ID | MCI_OPEN_ELEMENT, (DWORD_PTR)& open1);
	mciSendCommand(0, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_TYPE_ID | MCI_OPEN_ELEMENT, (DWORD_PTR)& open2);

	//mciSendCommand(open.wDeviceID, MCI_PLAY, MCI_NOTIFY, (DWORD_PTR)&play);

	//エラーなら0以外が返る
	//if (result) {
	//	//エラー取得
	//	mciGetErrorString(result, (LPWSTR)buf, sizeof(buf));
	//	MSG(buf);
	//	PostQuitMessage(0);
	//}

	//mp3再生の場合
	//open2.lpstrDeviceType = TEXT("MPEGVideo");
	//open2.lpstrElementName = TEXT("SE/music_sample2.mp3");
	//result = mciSendCommand(0, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT, (DWORD_PTR)&open2);

	//エラーなら0以外が返る
	//if (result) {
	//	//エラー取得
	//	mciGetErrorString(result, (LPWSTR)buf, sizeof(buf));
	//	MSG(buf);
	//	PostQuitMessage(0);
	//}

	play1.dwCallback = (DWORD)hwnd;
	play2.dwCallback = (DWORD)hwnd;
	//play2.dwCallback = (DWORD)hwnd;
	//play3.dwCallback = (DWORD)hwnd;
}

void closeMusic() {
	playMusic(ALL, PAUSE);
	mciSendCommand(open1.wDeviceID, MCI_CLOSE, 0, 0);
	mciSendCommand(open2.wDeviceID, MCI_CLOSE, 0, 0);
}


