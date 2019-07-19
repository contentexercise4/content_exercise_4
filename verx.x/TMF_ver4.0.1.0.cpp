// TMF_glut.cpp : ���̃t�@�C���ɂ� 'main' �֐����܂܂�Ă��܂��B�v���O�������s�̊J�n�ƏI���������ōs���܂��B

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


//�@disable Warning C4996
#pragma warning(disable : 4996)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------------------------------------
// ���C���֐�
//--------------------------------------------------------------------------------------------------------
int main(int argc, char *argv[]) {
	srand((unsigned)time(NULL));

	alutInit(&argc, argv);

	glutInit(&argc, argv); //���̏�����
	glutInitWindowPosition(WindowPositionX, WindowPositionY);  //�E�B���h�E�̈ʒu�̎w��
	glutInitWindowSize(WindowWidth, WindowHeight);             //�E�B���h�E�T�C�Y�̎w��
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE); //�f�B�X�v���C���[�h�̎w��
	glutCreateWindow(WindowTitle); //�E�B���h�E�̍쐬


#ifdef USE_DIMENCO_OPENGL_INTERFACE
	glutFullScreen();
	dimencoInit();
#endif
	//glutFullScreen();

	InitMusic();
	glutDisplayFunc(Display);			//�`�掞�ɌĂяo�����֐����w�肷��i�֐����FDisplay�j
	//glutReshapeFunc(Reshape);			//���߂�킩��Ȃ�
	glutIdleFunc(Idle);					//�v���O�����A�C�h����Ԏ��ɌĂяo�����֐�
	glutKeyboardFunc(Keyboard);			//�L�[�{�[�h���͎��ɌĂяo�����֐����w�肷��i�֐����FKeyboard�j
	glutSpecialFunc(special_key);		//����L�[�̐ݒ�
	glutKeyboardUpFunc(KeyboadUP);		//�L�[�{�[�h�����𗣂����Ƃ��ɔ���
	glutSpecialUpFunc(special_keyUP);	//����L�[�����𗣂����Ƃ��ɔ���

	initState();
	Initialize();						//�����ݒ�̊֐����Ăяo��
	glutMainLoop();						//���[�v�֐�. ���[�h�I���ɂ���ăQ�[���֖ʂ�J�ڂ��Ă���
	return 0;
}

//--------------------------------------------------------------------------------------------------------
// �Q�[���̏�����
//--------------------------------------------------------------------------------------------------------
void Initialize() {

#ifdef USE_DIMENCO_OPENGL_INTERFACE
	dimencoInitState();
#endif

	//�@GLEW�̏�����
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Error : GLEW�̏������Ɏ��s���܂���\n";
		exit(0);
	}

	InitObjFlag();
	InitEndFlag();
	//------------------------------------------------
	// moveOBL()
	//------------------------------------------------
	initialFoV = 45.0f;
	mouseSpeed = 0.05f;
	FoV = 45.0f;				//�Ȃ񂩓�����
	hspeed = 1.0f;
	disOfNoticePig = 5.5;		//��l���Ɠ؂Ƃ̋�����disOfNoticePig�ȉ��ɂȂ�ƃu�^���߂Â��Ȃ���������グ��
	skycheck = true;

	timeflag = true;

	selectEnviroment = MYPC;
	//selectEnviroment = DIMENCO;



	if (selectEnviroment == MYPC) {
		Bigger = 1;			//�����̑���   
		bigger = 1;			//�J�����̑���
		Bigger_pig = 1;		//�u�^�̂͂₳
		//speed = 15.0f*Bigger;					//��l���̍��W�𓮂�������
		//changePos = 40*bigger;					//�J�����̌����𓮂�������

		speed = 0.75f*Bigger;               //��l���̍��W�𓮂�������
		changePos = 2 * bigger;				 //�J�����̌����𓮂�������
	}
	else if (selectEnviroment == DIMENCO) {
		Bigger = 7.5;			//�����̑���   
		bigger = 20;			//�J�����̑���
		Bigger_pig = 10.0;		//�u�^�̂͂₳
		//speed = 15.0f*Bigger;					//��l���̍��W�𓮂�������
		//changePos = 40*bigger;					//�J�����̌����𓮂�������

		speed = 0.75f*Bigger;               //��l���̍��W�𓮂�������
		changePos = 2 * bigger;				 //�J�����̌����𓮂�������
	}

	if (selectEnviroment == MYPC) {
		//--------------------------------------------
		// �茳��PC��
		//--------------------------------------------
		WindowPositionX = 0;         //��������E�B���h�E�ʒu��X���W
		WindowPositionY = 0;         //��������E�B���h�E�ʒu��Y���W
		WindowWidth = 1960;          //��������E�B���h�E�̕�
		WindowHeight = 1080;         //��������E�B���h�E�̍���
		//speed = 0.75f;               //��l���̍��W�𓮂�������
		//changePos = 2;				 //�J�����̌����𓮂�������
	}
	else if (selectEnviroment == DIMENCO) {
		//--------------------------------------------
		// Dimenco Display�p��
		//--------------------------------------------
		//ver1
		//int WindowWidth = 4320;
		//int WindowHeight = 11520;

		//ver2
		//���͕��ʂ����オ�₯�ɍL��
		//int WindowWidth = 8640;
		//int WindowHeight = 3240;

		//ver3
		//����2���S���f��Ȃ��Ȃ���(height��1/3=2/6���炵��)
		//int WindowWidth = 8640;
		//int WindowHeight = 2160;

		//ver4
		//height��3240����2700��(1/6���炵��)
		//int WindowWidth = 8640;
		//int WindowHeight = 2700;

		//ver5
		//width��1/6���炵��
		//int WindowWidth = 7200;
		//int WindowHeight = 3240;

		//ver5
		//�O��̌���, 6�ʃf�B�X�v���C�̍�1��1/6�f��Ȃ��Ȃ����B���̌㎆�ʏ�Ōv�Z���ďo���l��w�ɑ������
		//���󂱂ꂪ��ԗǂ��B�{�Ԋ��p
		WindowWidth = 8214;
		WindowHeight = 3240;

		//speed = 15.0f;					//��l���̍��W�𓮂�������
		//changePos = 40;					//�J�����̌����𓮂�������
		//changePos = 50;				//�J�����̌����𓮂�������
	}

	//-----------------------------------------------------------------------
	// �`��Ɏg��obj�̏�����
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
	// �`��Ɏg��bmp�̏�����
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

	//�����܂ł͂�����


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
	// �v�Z�Ɏg��obj�̏�����. �ꉞ�S����`���Ă邯��map�Ɋւ�����̂����g��Ȃ�
	//-----------------------------------------------------------------------
	calc_OBJFile[0] = "obj/Newwall.obj";
	//calc_OBJFile[0] = "obj/Map01uv.obj";
	calc_OBJFile[7] = "obj/lowpigO.obj";
	calc_OBJFile[8] = "obj/lowapple.obj";


	if (!loadOBJ(calc_OBJFile[0].c_str(), vertices[0], uvs[0], normals[0], ver[0])) {	// ����obj�t�@�C����ǂݍ��߂Ȃ�������
		std::cout << "[ERROR] No File of " << calc_OBJFile[0] << '\n';					// �G���[����Ԃ�
	}

	if (!loadOBJ(calc_OBJFile[7].c_str(), vertices[7], uvs[7], normals[7], ver[7])) {	// ����obj�t�@�C����ǂݍ��߂Ȃ�������
		std::cout << "[ERROR] No File of " << calc_OBJFile[7] << '\n';					// �G���[����Ԃ�
	}

	if (!loadOBJ(calc_OBJFile[8].c_str(), vertices[8], uvs[8], normals[8], ver[8])) {	// ����obj�t�@�C����ǂݍ��߂Ȃ�������
		std::cout << "[ERROR] No File of " << calc_OBJFile[8] << '\n';					// �G���[����Ԃ�
	}


	//for (int i = 0; i < 9; i++) {
	//	if (i == 0 || i == 7 || i == 8) {
	//		std::cout << "i=" << i << std::endl;
	//		if (!loadOBJ(calc_OBJFile[i].c_str(), vertices[i], uvs[i], normals[i], ver[i])) {	// ����obj�t�@�C����ǂݍ��߂Ȃ�������
	//			std::cout << "[ERROR] No File of " << calc_OBJFile[i] << '\n';					// �G���[����Ԃ�
	//		}
	//		//prosessingOfOBJ(&ver[i], &vertexbuffer[i], vertices[i], normals[i], uvs[i], calc_OBJFile[i], &uvbuffer[i], &VertexArrayID[i]);
	//	}
	//}


	//�����S�̑��݃t���O�̏�����
	for (int i = 0; i < 4; i++) {
		appleFlag[i] = true;
	}

	//HARD�p�̃u�^�̑��xUP�t���O�̏�����
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

	decideCube(vertices[0]);			//�����蔻��ɗp����z��cube[][]�ɒl���i�[����

	//std::cout << "\n\n\nPosition of Pig\n";
	//for (int i = 0; i < (int)vertices[7].size(); i++) {
	//	std::cout << i << "(" << vertices[7][i].x <<  ", " << vertices[7][i].z << std::endl;
	//}

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//���ꂢ��񂩂��A�������ł���
	gluLookAt(
		-5, 1, -12,								// ���[���h��ԂŃJ������(4,3,3)�ɂ���܂��B
		//enepos_x2, enepos_y2, enepos_z2,		// ���E�̒���(�ǉ� 6/12)
		1, 1, 1,
		0.0, 1.0, 0.0							// ���E�̏�����̃x�N�g��x,y,z
	);


	// ���ŃG�X�P�[�v�L�[���������̂𑨂���̂�ۏ؂��܂��B
	//glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	//position.x = -12, position.y = 0.5, position.z = -15.5;//�J�����̈ʒu
	position.x = 0, position.y = 0.5, position.z = 0;//�J�����̈ʒu
	direction.x = 0, direction.y = 0, direction.z = 0;//���ł�����
	horizontalAngle = 0.0f;//�J�����̌���
	verticalAngle = 0.0f;
	movecount = 0;
	p = 0;
	flush_cnt = 0;		//START��ʂ̓_�ł𐧌䂷��ϐ�. �C���N�������g����200�ɓ��B������0�ɖ߂�

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
// escKey�������ꂽ����s. ���ׂẴo�b�t�@�����폜������
//--------------------------------------------------------------------------------------------------------
void GameAllDelete() {
	soundAllDelete();		//���ׂẲ��y�o�b�t�@���폜
	objAllDelete();			//���ׂẴI�u�W�F�N�g�o�b�t�@���폜
	textureAllDelete();		//���ׂẴe�N�X�`���o�b�t�@���폜
}

//--------------------------------------------------------------------------------------------------------
// �I�u�W�F�N�g�̂��ׂẴo�b�t�@���폜������
//--------------------------------------------------------------------------------------------------------
void objAllDelete() {
	for (int i = 0; i < 11; i++) {
		OBJFile[i].Release();
	}
}

//--------------------------------------------------------------------------------------------------------
// �e�N�X�`���̂��ׂẴo�b�t�@���폜������
//--------------------------------------------------------------------------------------------------------
void textureAllDelete() {
	for (int i = 0; i < N; i++) {
		Texture[i].~BMPImage();
	}
}

//--------------------------------------------------------------------------------------------------------
// dimenco�Ɋ֘A���鏉����
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
// �����̓���ɉe����^����\����(Objflag)�̈����̃t���O�̏�����
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
// �Q�[���I�[�o�[�E�Q�[���N���A�E�^�C���A�b�v�̃t���O
//--------------------------------------------------------------------------------------------------------
void InitEndFlag() {
	endflag.gameoverFlag = false;
	endflag.gameclearFlag = false;
	endflag.timeupFlag = false;
}

//--------------------------------------------------------------------------------------------------------
// �`��̊֐�
//--------------------------------------------------------------------------------------------------------
void Display(void) {

	glClearColor(0.2f, 0.2f, 0.2f, 0.0f);					// ���܂��Ȃ�
	glEnable(GL_DEPTH_TEST);								// �f�v�X�e�X�g��L���ɂ���
	glDepthFunc(GL_LESS);									// �O�̂��̂����J�����ɋ߂���΁A�t���O�����g���󂯓����
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// �X�N���[�����N���A����

	glDisable(GL_LIGHTING);

	//�����ϊ��s��̐ݒ�------------------------------
	glMatrixMode(GL_PROJECTION); //�s�񃂁[�h�̐ݒ�iGL_PROJECTION : �����ϊ��s��̐ݒ�AGL_MODELVIEW�F���f���r���[�ϊ��s��j
	glLoadIdentity();            //�s��̏�����
	gluPerspective(30.0, (double)WindowWidth / (double)WindowHeight, 0.1, 1500.0); //�������e�@�̎��̐�gluPerspactive(th, w/h, near, far);

	//���f���r���[�ϊ��s��̐ݒ�--------------------------
	glMatrixMode(GL_MODELVIEW); //�s�񃂁[�h�̐ݒ�iGL_PROJECTION : �����ϊ��s��̐ݒ�AGL_MODELVIEW�F���f���r���[�ϊ��s��j
	glLoadIdentity();           //�s��̏�����
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
	//�c�����S���m�F
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


	glutSwapBuffers(); //glutInitDisplayMode(GLUT_DOUBLE)�Ń_�u���o�b�t�@�����O�𗘗p��
}

//--------------------------------------------------------------------------------------------------------
// �L�[�{�[�h�������Ă�Ԃ̏���
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
// �J�[�\���L�[��F7�L�[�������Ă�Ԃ̏���
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

	//�B���R�}���h Debug�p
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
// �L�[�{�[�h�����𗣂����Ƃ��̏���
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
// �J�[�\���L�[��F7�L�[�����𗣂����Ƃ��̏���
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
// �L�[�{�[�h����ɂ���ĕύX���ꂽ�I���W�F�N�g�t���O�ɉ����Ď����Ɋւ��铮����X�V����
//--------------------------------------------------------------------------------------------------------
void moveOBJ() {
	initialFoV = 45.0f;
	mouseSpeed = 0.05f;
	int xpos = 512, ypos = 384;	//���܂��ɈӖ��s��
	FoV = 45.0f;				//�Ȃ񂩓�����
	//speed = 0.75f;              //��l���̍��W�𓮂�������
	hspeed = 1.0f;
	//int changePos = 5;			//�J�����̌����𓮂�������

	if (!gameEnd) {
		//--�J�����̌����̐���--//
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

		//--FoV�̒l�𐧌�--//
		if (objflag.FoVInc) {
			FoV = initialFoV - 5 * 3.0f;
		}
		if (objflag.FoVDec) {
			FoV = initialFoV + 5 * 3.0f;
		}
		/*
		//--�ǂ̏Փ˂𐧌�--//
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

		//--�ǂ̏Փ�SE�𐧌�--//
		if (countCollisionWall > initCountCollision) {
			countCollisionWall = 0;
		}

		if (hit(cube)) {
			//std::cout << "HIT\n";
		}

		//--��l���̍��W�𐧌�--//
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

	// �V���ȕ������v�Z���܂��B
	horizontalAngle += mouseSpeed * deltaTime * float(1024 / 2 - xpos);
	verticalAngle += mouseSpeed * deltaTime * float(768 / 2 - ypos);

	// �����F���ʍ��W���璼�p���W�ɕϊ����܂��B
	direction.x = cos(verticalAngle) * sin(horizontalAngle);
	direction.y = sin(verticalAngle);
	direction.z = cos(verticalAngle) * cos(horizontalAngle);

	// �E�x�N�g��
	right_vec3.x = sin(horizontalAngle - 3.14f / 2.0f);
	right_vec3.y = 0;
	right_vec3.z = cos(horizontalAngle - 3.14f / 2.0f);

	gluLookAt(
		position.x, position.y, position.z, // ���_�̈ʒux,y,z;
		//enepos_x2, enepos_y2, enepos_z2,    // ���E�̒���(�ǉ� 6/12)
		position.x + direction.x, position.y + direction.y, position.z + direction.z,
		0.0, 1.0, 0.0);                     // ���E�̏�����̃x�N�g��x,y,z
		//cout << position.x + direction.x << position.y + direction.y<< position.z + direction.z<<std::endl;
	//speed = 4.0f; // 3 units / second

	//�����S�̂����蔻��
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
// glutMainLoop() ��ʂ��ăf�B�X�v���C �R�[���o�b�N���Ăяo�����
//--------------------------------------------------------------------------------------------------------
static void Idle(void) {
	fps->GetFPS();				//FPS�𓾂�
	if (fps->draw) {			//�b��60�t���[���̃^�C�~���O�ŕ`��
		glutPostRedisplay();	//glutDisplayFunc()���P����s����
	}
	glutPostRedisplay();
}

//--------------------------------------------------------------------------------------------------------
// 3D�\�������邽�߂ɕK�v�i�H�j�Ȋ֐�. �X���搶��main��������p
//--------------------------------------------------------------------------------------------------------
void view3D()
{
	//"""3d�\���ɐ؂�ւ�"""
	//#Z�o�b�t�@�A�J�����O�L����
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glMatrixMode(GL_PROJECTION);
	//#�����ϊ��s������o��
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	//�|���S���̕\�ʂ̂ݕ`��
	glDisable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_DEPTH_TEST);

}

//--------------------------------------------------------------------------------------------------------
// Reshape Window�̃T�C�Y�ύX. ��{�I�ɂ͕K�v�Ȃ�
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
// START��ʂ�\��
//--------------------------------------------------------------------------------------------------------
void StartMode() {
	//#ifdef USE_DIMENCO_OPENGL_INTERFACE
	//	dimencoSetBackgroundState();
	//#endif

	if (selectEnviroment == MYPC) {
		gluLookAt(
			0.125, 0.08, 5.5,						// ���[���h��ԂŃJ������(4,3,3)�ɂ���܂��B
			//enepos_x2, enepos_y2, enepos_z2,		// ���E�̒���(�ǉ� 6/12)
			0.125, 0.08, 0,
			0.0, 1.0, 0.0);							// ���E�̏�����̃x�N�g��x,y,z
	}
	else if (selectEnviroment == DIMENCO) {
		gluLookAt(
			0.05, -0.02, 5.5,						// ���[���h��ԂŃJ������(4,3,3)�ɂ���܂��B
			//enepos_x2, enepos_y2, enepos_z2,		// ���E�̒���(�ǉ� 6/12)
			0.05, -0.02, 0,
			0.0, 1.0, 0.0);							// ���E�̏�����̃x�N�g��x,y,z
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//#ifdef USE_DIMENCO_OPENGL_INTERFACE
	//	dimencoSetZBufState();
	//#endif

	// �e�N�X�`���}�b�s���O�L���� (�ǉ� bmp)
	glEnable(GL_TEXTURE_2D);
	//�@�e�N�X�`�����o�C���h (�ǉ� bmp)
	glBindTexture(GL_TEXTURE_2D, Texture[3].ID);
	//glBindTexture(GL_TEXTURE_2D, Texture[2].ID);
	glPushMatrix();
	OBJFile[3].Draw();
	glPopMatrix();

	// �e�N�X�`���}�b�s���O�L���� (�ǉ� bmp)
	glEnable(GL_TEXTURE_2D);
	//�@�e�N�X�`�����o�C���h (�ǉ� bmp)
	glBindTexture(GL_TEXTURE_2D, Texture[4].ID);
	glPushMatrix();
	OBJFile[4].Draw();
	glPopMatrix();


	//if (flush_cnt < 100) {
	glEnable(GL_TEXTURE_2D);						// �e�N�X�`���}�b�s���O�L���� (�ǉ� bmp)
	glBindTexture(GL_TEXTURE_2D, Texture[5].ID);	//�@�e�N�X�`�����o�C���h (�ǉ� bmp)
	glPushMatrix();
	OBJFile[5].Draw();
	glPopMatrix();
	//}
	//else if (flush_cnt == 200) {
	//	flush_cnt = 0;
	//}
	//flush_cnt++;
	//std::cout << "flush = " << flush_cnt << std::endl;

	// �e�N�X�`���}�b�s���O�L���� (�ǉ� bmp)
	//glEnable(GL_TEXTURE_2D);
	//�@�e�N�X�`�����o�C���h (�ǉ� bmp)
	//glBindTexture(GL_TEXTURE_2D, Texture[6].ID);
	//glPushMatrix();
	//OBJFile[6].Draw();
	//glPopMatrix();
}

//--------------------------------------------------------------------------------------------------------
// PLAY��ʂ�\��
//--------------------------------------------------------------------------------------------------------
void PlayMode() {
	//if (playflag == false) {
	//	std::cout << "asdsn";
	//	playflag = true;
	//	gluLookAt(
	//		-5, 1, -12, // ���[���h��ԂŃJ������(4,3,3)�ɂ���܂��B
	//		//enepos_x2, enepos_y2, enepos_z2,		// ���E�̒���(�ǉ� 6/12)
	//		1, 1, 1,
	//		0.0, 1.0, 0.0);							// ���E�̏�����̃x�N�g��x,y,z
	//}

	//gluLookAt(
	//	4, 1, 2,								// ���[���h��ԂŃJ������(4,3,3)�ɂ���܂��B
	//	//enepos_x2, enepos_y2, enepos_z2,		// ���E�̒���(�ǉ� 6/12)
	//	1, 1, 1,
	//	0.0, 1.0, 0.0
	//);											// ���E�̏�����̃x�N�g��x,y,z

	//decideVolumeOfPigCry(disFromPig);				//�u�^�Ǝ�l���Ƃ̋����ɉ����Ė����̑傫����ύX

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
	defTime = cuTime - faTime;//�v���C�X�^�[�g����o�ߎ���

	//std::cout << cuTime << " " << faTime << std::endl;


	//�������Ƃ��Ɏg���Ă�(��)
	movecount += 0.001;
	moveOBJ();




	// �J�����̂ق��������Ă��Ȃ��@���̎O�p�`���J�����O���܂��B
	glEnable(GL_CULL_FACE);

	//glfwPollEvents();



	//---�I�u�W�F�N�g�`�敔---//
	Wall();					//��
	Floor();				//��
	if (ModeSelect != 3) {	//ViewMode�łȂ��Ȃ��
		TimeLimit();		//��������
	}
	NumOfApple();			//���݂̃����S�擾��
	DirectionOfPig();		//�߂��ɂ���u�^�̕���(�x���e�N�X�`��) & 4�C�̃u�^
	Apple();				//4�̃����S
	AppleCheck();			//�����S�A���[�g
		//�����S���Ȃ��Ȃ��Ă��烊�U���g��
	if (appleFlag[0] == false && appleFlag[1] == false && appleFlag[2] == false && appleFlag[3] == false) {
		GameClear();
		//ModeSelect = 2;
		//sound_check = 2;
	}
	if (skycheck == true) {
		Sky();				//��
	}


	//�����S���Ȃ��Ȃ��Ă��烊�U���g��
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
// RESULT��ʂ�\��
//--------------------------------------------------------------------------------------------------------
void ResultMode() {

	if (selectEnviroment == MYPC) {
		gluLookAt(
			0.125, 0.08, 5.5, // ���[���h��ԂŃJ������(4,3,3)�ɂ���܂��B
			//enepos_x2, enepos_y2, enepos_z2,    // ���E�̒���(�ǉ� 6/12)
			0.125, 0.08, 0,
			0.0, 1.0, 0.0);                     // ���E�̏�����̃x�N�g��x,y,z
	}
	else if (selectEnviroment == DIMENCO) {
		//ver2.7.1.8
		//gluLookAt(
		//	0.06, 0.02, 5.5,						// ���[���h��ԂŃJ������(4,3,3)�ɂ���܂��B
		//	//enepos_x2, enepos_y2, enepos_z2,		// ���E�̒���(�ǉ� 6/12)
		//	0.06, 0.02, 0,							// ���E�̏�����̃x�N�g��x,y,z
		//	0.0, 1.0, 0.0
		//);        

		//ver2.7.1.9
		gluLookAt(
			0.07, 0.02, 5.5,						// ���[���h��ԂŃJ������(4,3,3)�ɂ���܂��B
			//enepos_x2, enepos_y2, enepos_z2,		// ���E�̒���(�ǉ� 6/12)
			0.07, 0.02, 0,							// ���E�̏�����̃x�N�g��x,y,z
			0.0, 1.0, 0.0
		);

		//ver2.7.1.10
		//gluLookAt(
		//	0.08, 0.02, 5.5,						// ���[���h��ԂŃJ������(4,3,3)�ɂ���܂��B
		//	//enepos_x2, enepos_y2, enepos_z2,		// ���E�̒���(�ǉ� 6/12)
		//	0.07, 0.02, 0,							// ���E�̏�����̃x�N�g��x,y,z
		//	0.0, 1.0, 0.0
		//);
	}

	applecount = 0;
	//�c�����S���m�F
	for (int i = 0; i < 4; i++) {
		if (appleFlag[i] == true) {
			applecount++;
		}
	}

	// �e�N�X�`���}�b�s���O�L���� (�ǉ� bmp)
	glEnable(GL_TEXTURE_2D);
	//�@�e�N�X�`�����o�C���h (�ǉ� bmp)
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

	// �e�N�X�`���}�b�s���O�L���� (�ǉ� bmp)
	glEnable(GL_TEXTURE_2D);
	//�@�e�N�X�`�����o�C���h (�ǉ� bmp)
	glBindTexture(GL_TEXTURE_2D, Texture[4].ID);
	glPushMatrix();
	OBJFile[4].Draw();
	glPopMatrix();


}

//--------------------------------------------------------------------------------------------------------
// �Q�[�������Z�b�g����
//--------------------------------------------------------------------------------------------------------
void GameReset() {
	gameEnd = false;
	InitEndFlag();
	InitObjFlag();
	position.x = 0, position.y = 0.5, position.z = 0;//�J�����̈ʒu
	direction.x = 0, direction.y = 0, direction.z = 0;//���ł�����
	horizontalAngle = 0.0f;//�J�����̌���
	verticalAngle = 0.0f;
	movecount = 0;
	p = 0;
	applecount = 4;
	//�����S�̑��݃t���O�̏�����
	for (int i = 0; i < 4; i++) {
		appleFlag[i] = true;
	}

	//HARD�p�̃u�^�̑��xUP�t���O�̏�����
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
	//// �茳��PC��
	////--------------------------------------------
	//if (selectEnviroment == MYPC) {
	//	speed = 0.75f;              //��l���̍��W�𓮂�������
	//	changePos = 2;				//�J�����̌����𓮂�������
	//}
	////--------------------------------------------
	//// Dimenco Display�p��
	////--------------------------------------------
	//else if (selectEnviroment == DIMENCO) {
	//	speed = 2.5f;               //��l���̍��W�𓮂�������
	//	changePos = 5;				//�J�����̌����𓮂�������
	//}
}

//--------------------------------------------------------------------------------------------------------
// �X�e�[�W���ォ�猩���낷
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
// �Q�[����Փx��I��. ������ enum �� DigreeOfDifficulty
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
// �u�^�̑������Z�b�g
//--------------------------------------------------------------------------------------------------------
void setPigSpeed(double pig_speed) {
	obj_speed = pig_speed;
}

//--------------------------------------------------------------------------------------------------------
// �u�^�̑����𑝌�
//--------------------------------------------------------------------------------------------------------
void changePigSpeed(double change_pig_speed) {
	obj_speed += change_pig_speed;
}

//--------------------------------------------------------------------------------------------------------
// ��l�����ǂɏՓ˂��Ă��邩�ۂ���Ԃ�bool�^�֐�
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
// �u�^���ǂɏՓ˂��Ă��邩�ۂ���Ԃ�bool�^�֐�1
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
// �u�^���ǂɏՓ˂��Ă��邩�ۂ���Ԃ�bool�^�֐�2
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
// �؂𓮂��ɍ��킹�ĉ�]������
//--------------------------------------------------------------------------------------------------------
void PigRolling(double xspeed, double zspeed) {
	float sita;

	sita = atan(xspeed / zspeed);

	//std::cout << sita << std::endl;

	float pi = 3.1415;
	float onedegree = pi / 180.0f; //1���̃��W�A��
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
// �����̓����ɘA�����ăI�u�W�F�N�g�𓮂����A��ɂ��̏�ɃI�u�W�F�N�g�����邩�̂悤�Ɍ����鏈��
//--------------------------------------------------------------------------------------------------------
void ObjMoveRoll_Camera(float hA, float vA, int i) {
	double x, y, z;
	float Ax, Az;
	//glm::vec3(4, 1, 2);
	x = position.x;
	y = position.y;
	z = position.z;

	float pi = 3.1415;
	float onedegree = pi / 180.0f; //1���̃��W�A��
	float rolldegree = hA / onedegree;

	//------------------------------------------------------
	// �茳��PC���p
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
	// Dimenco Display���p
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
// PlayMode()���̃I�u�W�F�N�g�̕\���֌W
/////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------------------------------
// ��
//--------------------------------------------------------------------------------------------------------
void Wall() {
	glEnable(GL_TEXTURE_2D);						// �e�N�X�`���}�b�s���O�L���� (�ǉ� bmp)
	glBindTexture(GL_TEXTURE_2D, Texture[0].ID);	// �e�N�X�`�����o�C���h (�ǉ� bmp)
	glPushMatrix();									// �s��X�^�b�N�̍ŏ㕔���R�s�[
	OBJFile[0].Draw();								// �I�u�W�F�N�g��`��
	glPopMatrix();									// �s��X�^�b�N��j��
}

//--------------------------------------------------------------------------------------------------------
// ��
//--------------------------------------------------------------------------------------------------------
void Floor() {
	glEnable(GL_TEXTURE_2D);						// �e�N�X�`���}�b�s���O�L���� (�ǉ� bmp)
	glBindTexture(GL_TEXTURE_2D, Texture[1].ID);	//�@�e�N�X�`�����o�C���h (�ǉ� bmp)
	glPushMatrix();
	glTranslatef(0.0, 0.0, -2.0);
	glScalef(0.78f, 1.0f, 0.85f);
	OBJFile[1].Draw();
	glPopMatrix();
}

//--------------------------------------------------------------------------------------------------------
// ��������
//--------------------------------------------------------------------------------------------------------
void TimeLimit() {
	bool debug = true;
	if (debug) {
		//std::cout << "defTime = " << defTime << std::endl;
	}
	if (defTime > 18 && !endflag.gameoverFlag && !endflag.gameclearFlag) {//�Q�[���I��
		std::cout << "TIMEUP\n";
		TimeUp();
		//ModeSelect = 2;
		//sound_check = 2;
	}
	else if (defTime > 17) {
		glEnable(GL_TEXTURE_2D);								// �e�N�X�`���}�b�s���O�L���� (�ǉ� bmp)
		glBindTexture(GL_TEXTURE_2D, Texture[24].ID);			//�@�e�N�X�`�����o�C���h (�ǉ� bmp)
		glPushMatrix();
		ObjMoveRoll_Camera(horizontalAngle, verticalAngle, 0);
		//glTranslatef(-11.0, 1.05, -15.6);
		//glRotatef(90.0f,0.0f,1.0f,0.0f);
		OBJFile[2].Draw();
		glPopMatrix();
	}
	else if (defTime > 12) {
		glEnable(GL_TEXTURE_2D);								// �e�N�X�`���}�b�s���O�L���� (�ǉ� bmp)
		glBindTexture(GL_TEXTURE_2D, Texture[18].ID);			//�@�e�N�X�`�����o�C���h (�ǉ� bmp)
		glPushMatrix();
		ObjMoveRoll_Camera(horizontalAngle, verticalAngle, 0);
		//glTranslatef(-11.0, 1.05, -15.6);
		//glRotatef(90.0f,0.0f,1.0f,0.0f);
		OBJFile[2].Draw();
		glPopMatrix();
	}
	else if (defTime > 6) {
		glEnable(GL_TEXTURE_2D);								// �e�N�X�`���}�b�s���O�L���� (�ǉ� bmp)
		glBindTexture(GL_TEXTURE_2D, Texture[26].ID);			//�@�e�N�X�`�����o�C���h (�ǉ� bmp)
		glPushMatrix();
		ObjMoveRoll_Camera(horizontalAngle, verticalAngle, 0);
		//glTranslatef(-11.0, 1.05, -15.6);
		//glRotatef(90.0f,0.0f,1.0f,0.0f);
		OBJFile[2].Draw();
		glPopMatrix();
	}
	else {
		glEnable(GL_TEXTURE_2D);								// �e�N�X�`���}�b�s���O�L���� (�ǉ� bmp)
		glBindTexture(GL_TEXTURE_2D, Texture[25].ID);			//�@�e�N�X�`�����o�C���h (�ǉ� bmp)
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
// �擾���ׂ������S�̐�
//--------------------------------------------------------------------------------------------------------
void NumOfApple() {
	glEnable(GL_TEXTURE_2D);										// �e�N�X�`���}�b�s���O�L���� (�ǉ� bmp)
	if (selectEnviroment == MYPC) {
		if (applecount == 4) {											// �擾���ׂ������S�͂���4��
			glBindTexture(GL_TEXTURE_2D, Texture[4].ID);				// �e�N�X�`�����o�C���h (�ǉ� bmp)
			setPigSpeed(0.005*Bigger_pig);											// ����
			//setPigSpeed(0.025);											// ����
		}
		else if (applecount == 3) {										// �擾���ׂ������S�͂���3��
			glBindTexture(GL_TEXTURE_2D, Texture[10].ID);				// �e�N�X�`�����o�C���h (�ǉ� bmp)
			if (!changePigSpeedFlag[3] && digreeOfDifficulty == HARD) {	// �܂�������ς��Ă��Ȃ��Ȃ��(HARD�̂�)
				changePigSpeedFlag[3] = true;							// �����ύX���Ȃ�
				setPigSpeed(0.006*Bigger_pig);										// HARD�̂Ƃ��̂�
				//setPigSpeed(0.035);											// ����
			}
		}
		else if (applecount == 2) {										// �擾���ׂ������S�͂���2��
			glBindTexture(GL_TEXTURE_2D, Texture[11].ID);				// �e�N�X�`�����o�C���h (�ǉ� bmp)
			if (!changePigSpeedFlag[2] && digreeOfDifficulty == HARD) {	// �܂�������ς��Ă��Ȃ��Ȃ��(HARD�̂�)
				changePigSpeedFlag[2] = true;							// �����ύX���Ȃ�
				setPigSpeed(0.007*Bigger_pig);										// HARD�̂Ƃ��̂�
				//setPigSpeed(0.045);											// ����
			}
		}
		else if (applecount == 1) {										// �擾���ׂ������S�͂���1��
			glBindTexture(GL_TEXTURE_2D, Texture[12].ID);				// �e�N�X�`�����o�C���h (�ǉ� bmp)
			if (!changePigSpeedFlag[1] && digreeOfDifficulty == HARD) {	// �܂�������ς��Ă��Ȃ��Ȃ��(HARD�̂�)
				changePigSpeedFlag[1] = true;							// �����ύX���Ȃ�
				setPigSpeed(0.008*Bigger_pig);										// HARD�̂Ƃ��̂�
				//setPigSpeed(0.05);											// ����
			}
		}
		else if (applecount == 0) {										// �S���擾����
			glBindTexture(GL_TEXTURE_2D, Texture[13].ID);				// �e�N�X�`�����o�C���h (�ǉ� bmp)
			if (!changePigSpeedFlag[0] && digreeOfDifficulty == HARD) {	// �܂�������ς��Ă��Ȃ��Ȃ��(HARD�̂�)
				changePigSpeedFlag[0] = true;							// �����ύX���Ȃ�
			}
		}
	}
	else if (selectEnviroment == DIMENCO) {
		if (applecount == 4) {											// �擾���ׂ������S�͂���4��
			glBindTexture(GL_TEXTURE_2D, Texture[4].ID);				// �e�N�X�`�����o�C���h (�ǉ� bmp)
			setPigSpeed(0.005*Bigger_pig);											// ����
		}
		else if (applecount == 3) {										// �擾���ׂ������S�͂���3��
			glBindTexture(GL_TEXTURE_2D, Texture[10].ID);				// �e�N�X�`�����o�C���h (�ǉ� bmp)
			if (!changePigSpeedFlag[3] && digreeOfDifficulty == HARD) {	// �܂�������ς��Ă��Ȃ��Ȃ��(HARD�̂�)
				changePigSpeedFlag[3] = true;							// �����ύX���Ȃ�
				setPigSpeed(0.006*Bigger_pig);										// HARD�̂Ƃ��̂�
			}
		}
		else if (applecount == 2) {										// �擾���ׂ������S�͂���2��
			glBindTexture(GL_TEXTURE_2D, Texture[11].ID);				// �e�N�X�`�����o�C���h (�ǉ� bmp)
			if (!changePigSpeedFlag[2] && digreeOfDifficulty == HARD) {	// �܂�������ς��Ă��Ȃ��Ȃ��(HARD�̂�)
				changePigSpeedFlag[2] = true;							// �����ύX���Ȃ�
				setPigSpeed(0.007*Bigger_pig);										// HARD�̂Ƃ��̂�
			}
		}
		else if (applecount == 1) {										// �擾���ׂ������S�͂���1��
			glBindTexture(GL_TEXTURE_2D, Texture[12].ID);				// �e�N�X�`�����o�C���h (�ǉ� bmp)
			if (!changePigSpeedFlag[1] && digreeOfDifficulty == HARD) {	// �܂�������ς��Ă��Ȃ��Ȃ��(HARD�̂�)
				changePigSpeedFlag[1] = true;							// �����ύX���Ȃ�
				setPigSpeed(0.008*Bigger_pig);										// HARD�̂Ƃ��̂�
			}
		}
		else if (applecount == 0) {										// �S���擾����
			glBindTexture(GL_TEXTURE_2D, Texture[13].ID);				// �e�N�X�`�����o�C���h (�ǉ� bmp)
			if (!changePigSpeedFlag[0] && digreeOfDifficulty == HARD) {	// �܂�������ς��Ă��Ȃ��Ȃ��(HARD�̂�)
				changePigSpeedFlag[0] = true;							// �����ύX���Ȃ�
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
// �߂��̃u�^�̕���&�u�^
// disFromPig     : �u�^�Ǝ����Ƃ̃��[�N���b�h����
// disOfNoticePig : �u�^�������ɋC�Â����� 
//--------------------------------------------------------------------------------------------------------
void DirectionOfPig() {
	for (int i = 0; i < 4; i++) {

		glEnable(GL_TEXTURE_2D);						// �e�N�X�`���}�b�s���O�L���� (�ǉ� bmp)
		glBindTexture(GL_TEXTURE_2D, Texture[7].ID);	// �e�N�X�`�����o�C���h (�ǉ� bmp)
		glPushMatrix();

		//double obj_speed = 0.001;						// object�̃X�s�[�h. setPigSpeed(double speed), changePigSpeed(double speed)�ŕύX
		double dif_x, dif_z;							// ��l���Ɠ؂Ƃ̍��W�̍�
		double obj_xspeed, obj_zspeed;					// �u�^�̑���
		int hitflag;									// �q�b�g�t���O
		double hittmp;									// �q�b�g�t���O�̈ꎞ�ۑ���?

		glTranslatef(pigx[i], pigy[i], pigz[i]);			// �u�^���ړ�

		dif_x = position.x - pigx[i];
		dif_z = position.z - pigz[i];
		disFromPig = sqrt(dif_x * dif_x + dif_z * dif_z);
		disFromPig2 = 0;
		double pointx, pointz; //point�Ɠ؂̍�
		double dispoint;


		if (!gameEnd) {

			if (pigNoticeFlag[i] == true) {				//�u�^�̐ڋ߃t���O��true(��ՓxEASY�̂�) ����, �u�^�Ǝ����̋�����disOfNoticePig�ȉ�
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

			//�u�^���G���A�������ǂ�������
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
			if (accessPig == true && areafind == true) {	//�u�^�̐ڋ߃t���O��true(��ՓxEASY�̂�) ����, �u�^�Ǝ����̋�����disOfNoticePig�ȉ�
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

		//�؂̍U������
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

		//�A���[�g�̕\��

		glEnable(GL_TEXTURE_2D);
		int houkou[2] = {};
		int cameraAngle;
		float pi = 3.1415;
		float onedegree = pi / 180.0f; //1���̃��W�A��
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
					// glBindTexture(GL_TEXTURE_2D, Texture[14].ID);//��
					if (abs(cameraAngle) % 4 == 0) {
						glBindTexture(GL_TEXTURE_2D, Texture[15].ID);//��
					}
					else if (abs(cameraAngle) % 4 == 1) {
						glBindTexture(GL_TEXTURE_2D, Texture[16].ID);//��
					}
					else if (abs(cameraAngle) % 4 == 2) {
						glBindTexture(GL_TEXTURE_2D, Texture[14].ID);//��
					}
					else if (abs(cameraAngle) % 4 == 3) {
						glBindTexture(GL_TEXTURE_2D, Texture[17].ID);//�E
					}
				}
				else if (houkou[0] == -1 && houkou[1] == -1 && abs(dif_x) >= abs(dif_z)) {
					//glBindTexture(GL_TEXTURE_2D, Texture[17].ID);//�E
					if (abs(cameraAngle) % 4 == 0) {
						glBindTexture(GL_TEXTURE_2D, Texture[16].ID);//��
					}
					else if (abs(cameraAngle) % 4 == 1) {
						glBindTexture(GL_TEXTURE_2D, Texture[14].ID);//��
					}
					else if (abs(cameraAngle) % 4 == 2) {
						glBindTexture(GL_TEXTURE_2D, Texture[17].ID);//�E
					}
					else if (abs(cameraAngle) % 4 == 3) {
						glBindTexture(GL_TEXTURE_2D, Texture[15].ID);//��
					}
				}

				if (houkou[0] == 1 && houkou[1] == -1 && abs(dif_x) < abs(dif_z)) {
					//glBindTexture(GL_TEXTURE_2D, Texture[14].ID);//��
					if (abs(cameraAngle) % 4 == 0) {
						glBindTexture(GL_TEXTURE_2D, Texture[15].ID);//��
					}
					else if (abs(cameraAngle) % 4 == 1) {
						glBindTexture(GL_TEXTURE_2D, Texture[16].ID);//��
					}
					else if (abs(cameraAngle) % 4 == 2) {
						glBindTexture(GL_TEXTURE_2D, Texture[14].ID);//��
					}
					else if (abs(cameraAngle) % 4 == 3) {
						glBindTexture(GL_TEXTURE_2D, Texture[17].ID);//�E
					}
				}
				else if (houkou[0] == 1 && houkou[1] == -1 && abs(dif_x) >= abs(dif_z)) {
					//glBindTexture(GL_TEXTURE_2D, Texture[16].ID);//��
					if (abs(cameraAngle) % 4 == 0) {
						glBindTexture(GL_TEXTURE_2D, Texture[17].ID);//�E
					}
					else if (abs(cameraAngle) % 4 == 1) {
						glBindTexture(GL_TEXTURE_2D, Texture[15].ID);//��
					}
					else if (abs(cameraAngle) % 4 == 2) {
						glBindTexture(GL_TEXTURE_2D, Texture[16].ID);//��
					}
					else if (abs(cameraAngle) % 4 == 3) {
						glBindTexture(GL_TEXTURE_2D, Texture[14].ID);//��
					}
				}

				if (houkou[0] == -1 && houkou[1] == 1 && abs(dif_x) < abs(dif_z)) {
					//glBindTexture(GL_TEXTURE_2D, Texture[15].ID);//��
					if (abs(cameraAngle) % 4 == 0) {
						glBindTexture(GL_TEXTURE_2D, Texture[14].ID);//��
					}
					else if (abs(cameraAngle) % 4 == 1) {
						glBindTexture(GL_TEXTURE_2D, Texture[17].ID);//�E
					}
					else if (abs(cameraAngle) % 4 == 2) {
						glBindTexture(GL_TEXTURE_2D, Texture[15].ID);//��
					}
					else if (abs(cameraAngle) % 4 == 3) {
						glBindTexture(GL_TEXTURE_2D, Texture[16].ID);//��
					}

				}
				else if (houkou[0] == -1 && houkou[1] == 1 && abs(dif_x) >= abs(dif_z)) {
					//glBindTexture(GL_TEXTURE_2D, Texture[17].ID);//�E
					if (abs(cameraAngle) % 4 == 0) {
						glBindTexture(GL_TEXTURE_2D, Texture[16].ID);//��
					}
					else if (abs(cameraAngle) % 4 == 1) {
						glBindTexture(GL_TEXTURE_2D, Texture[14].ID);//��
					}
					else if (abs(cameraAngle) % 4 == 2) {
						glBindTexture(GL_TEXTURE_2D, Texture[17].ID);//�E
					}
					else if (abs(cameraAngle) % 4 == 3) {
						glBindTexture(GL_TEXTURE_2D, Texture[15].ID);//��
					}
				}

				if (houkou[0] == 1 && houkou[1] == 1 && abs(dif_x) < abs(dif_z)) {
					//glBindTexture(GL_TEXTURE_2D, Texture[15].ID);//��
					if (abs(cameraAngle) % 4 == 0) {
						glBindTexture(GL_TEXTURE_2D, Texture[14].ID);//��
					}
					else if (abs(cameraAngle) % 4 == 1) {
						glBindTexture(GL_TEXTURE_2D, Texture[17].ID);//�E
					}
					else if (abs(cameraAngle) % 4 == 2) {
						glBindTexture(GL_TEXTURE_2D, Texture[15].ID);//��
					}
					else if (abs(cameraAngle) % 4 == 3) {
						glBindTexture(GL_TEXTURE_2D, Texture[16].ID);//��
					}
				}
				else if (houkou[0] == 1 && houkou[1] == 1 && abs(dif_x) >= abs(dif_z)) {
					//glBindTexture(GL_TEXTURE_2D, Texture[16].ID);//��
					if (abs(cameraAngle) % 4 == 0) {
						glBindTexture(GL_TEXTURE_2D, Texture[17].ID);//�E
					}
					else if (abs(cameraAngle) % 4 == 1) {
						glBindTexture(GL_TEXTURE_2D, Texture[15].ID);//��
					}
					else if (abs(cameraAngle) % 4 == 2) {
						glBindTexture(GL_TEXTURE_2D, Texture[16].ID);//��
					}
					else if (abs(cameraAngle) % 4 == 3) {
						glBindTexture(GL_TEXTURE_2D, Texture[14].ID);//��
					}
				}
			}
			else if (rolldegree >= 0) {
				if (houkou[0] == -1 && houkou[1] == -1 && abs(dif_x) < abs(dif_z)) {
					// glBindTexture(GL_TEXTURE_2D, Texture[14].ID);//��
					if (abs(cameraAngle) % 4 == 0) {
						glBindTexture(GL_TEXTURE_2D, Texture[14].ID);//��
					}
					else if (abs(cameraAngle) % 4 == 1) {
						glBindTexture(GL_TEXTURE_2D, Texture[16].ID);//��
					}
					else if (abs(cameraAngle) % 4 == 2) {
						glBindTexture(GL_TEXTURE_2D, Texture[15].ID);//��
					}
					else if (abs(cameraAngle) % 4 == 3) {
						glBindTexture(GL_TEXTURE_2D, Texture[17].ID);//�E
					}
				}
				else if (houkou[0] == -1 && houkou[1] == -1 && abs(dif_x) >= abs(dif_z)) {
					//glBindTexture(GL_TEXTURE_2D, Texture[17].ID);//�E
					if (abs(cameraAngle) % 4 == 0) {
						glBindTexture(GL_TEXTURE_2D, Texture[16].ID);//��
					}
					else if (abs(cameraAngle) % 4 == 1) {
						glBindTexture(GL_TEXTURE_2D, Texture[15].ID);//��
					}
					else if (abs(cameraAngle) % 4 == 2) {
						glBindTexture(GL_TEXTURE_2D, Texture[17].ID);//�E
					}
					else if (abs(cameraAngle) % 4 == 3) {
						glBindTexture(GL_TEXTURE_2D, Texture[14].ID);//��
					}
				}

				if (houkou[0] == 1 && houkou[1] == -1 && abs(dif_x) < abs(dif_z)) {
					//glBindTexture(GL_TEXTURE_2D, Texture[14].ID);//��
					if (abs(cameraAngle) % 4 == 0) {
						glBindTexture(GL_TEXTURE_2D, Texture[14].ID);//��
					}
					else if (abs(cameraAngle) % 4 == 1) {
						glBindTexture(GL_TEXTURE_2D, Texture[16].ID);//��
					}
					else if (abs(cameraAngle) % 4 == 2) {
						glBindTexture(GL_TEXTURE_2D, Texture[15].ID);//��
					}
					else if (abs(cameraAngle) % 4 == 3) {
						glBindTexture(GL_TEXTURE_2D, Texture[17].ID);//�E
					}
				}
				else if (houkou[0] == 1 && houkou[1] == -1 && abs(dif_x) >= abs(dif_z)) {
					//glBindTexture(GL_TEXTURE_2D, Texture[16].ID);//��
					if (abs(cameraAngle) % 4 == 0) {
						glBindTexture(GL_TEXTURE_2D, Texture[17].ID);//�E
					}
					else if (abs(cameraAngle) % 4 == 1) {
						glBindTexture(GL_TEXTURE_2D, Texture[14].ID);//��
					}
					else if (abs(cameraAngle) % 4 == 2) {
						glBindTexture(GL_TEXTURE_2D, Texture[16].ID);//��
					}
					else if (abs(cameraAngle) % 4 == 3) {
						glBindTexture(GL_TEXTURE_2D, Texture[15].ID);//��
					}
				}

				if (houkou[0] == -1 && houkou[1] == 1 && abs(dif_x) < abs(dif_z)) {
					//glBindTexture(GL_TEXTURE_2D, Texture[15].ID);//��
					if (abs(cameraAngle) % 4 == 0) {
						glBindTexture(GL_TEXTURE_2D, Texture[15].ID);//��
					}
					else if (abs(cameraAngle) % 4 == 1) {
						glBindTexture(GL_TEXTURE_2D, Texture[17].ID);//�E
					}
					else if (abs(cameraAngle) % 4 == 2) {
						glBindTexture(GL_TEXTURE_2D, Texture[14].ID);//��
					}
					else if (abs(cameraAngle) % 4 == 3) {
						glBindTexture(GL_TEXTURE_2D, Texture[16].ID);//��
					}

				}
				else if (houkou[0] == -1 && houkou[1] == 1 && abs(dif_x) >= abs(dif_z)) {
					//glBindTexture(GL_TEXTURE_2D, Texture[17].ID);//�E
					if (abs(cameraAngle) % 4 == 0) {
						glBindTexture(GL_TEXTURE_2D, Texture[16].ID);//��
					}
					else if (abs(cameraAngle) % 4 == 1) {
						glBindTexture(GL_TEXTURE_2D, Texture[15].ID);//��
					}
					else if (abs(cameraAngle) % 4 == 2) {
						glBindTexture(GL_TEXTURE_2D, Texture[17].ID);//�E
					}
					else if (abs(cameraAngle) % 4 == 3) {
						glBindTexture(GL_TEXTURE_2D, Texture[14].ID);//��
					}
				}

				if (houkou[0] == 1 && houkou[1] == 1 && abs(dif_x) < abs(dif_z)) {
					//glBindTexture(GL_TEXTURE_2D, Texture[15].ID);//��
					if (abs(cameraAngle) % 4 == 0) {
						glBindTexture(GL_TEXTURE_2D, Texture[15].ID);//��
					}
					else if (abs(cameraAngle) % 4 == 1) {
						glBindTexture(GL_TEXTURE_2D, Texture[17].ID);//�E
					}
					else if (abs(cameraAngle) % 4 == 2) {
						glBindTexture(GL_TEXTURE_2D, Texture[14].ID);//��
					}
					else if (abs(cameraAngle) % 4 == 3) {
						glBindTexture(GL_TEXTURE_2D, Texture[16].ID);//��
					}
				}
				else if (houkou[0] == 1 && houkou[1] == 1 && abs(dif_x) >= abs(dif_z)) {
					//glBindTexture(GL_TEXTURE_2D, Texture[16].ID);//��
					if (abs(cameraAngle) % 4 == 0) {
						glBindTexture(GL_TEXTURE_2D, Texture[17].ID);//�E
					}
					else if (abs(cameraAngle) % 4 == 1) {
						glBindTexture(GL_TEXTURE_2D, Texture[14].ID);//��
					}
					else if (abs(cameraAngle) % 4 == 2) {
						glBindTexture(GL_TEXTURE_2D, Texture[16].ID);//��
					}
					else if (abs(cameraAngle) % 4 == 3) {
						glBindTexture(GL_TEXTURE_2D, Texture[15].ID);//��
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
// ���
//--------------------------------------------------------------------------------------------------------
void Apple() {
	if (appleFlag[0] == true) {
		//apple1
		// �e�N�X�`���}�b�s���O�L���� (�ǉ� bmp)
		glEnable(GL_TEXTURE_2D);
		//�@�e�N�X�`�����o�C���h (�ǉ� bmp)
		glBindTexture(GL_TEXTURE_2D, Texture[8].ID);
		glPushMatrix();
		glTranslatef(28.0, 0.5, 26.0);
		OBJFile[8].Draw();
		glPopMatrix();
	}

	if (appleFlag[1] == true) {
		//apple2
	// �e�N�X�`���}�b�s���O�L���� (�ǉ� bmp)
		glEnable(GL_TEXTURE_2D);
		//�@�e�N�X�`�����o�C���h (�ǉ� bmp)
		glBindTexture(GL_TEXTURE_2D, Texture[8].ID);
		glPushMatrix();
		glTranslatef(14.0, 0.5, -27.5);
		OBJFile[8].Draw();
		glPopMatrix();
	}

	if (appleFlag[2] == true) {
		//apple3
	// �e�N�X�`���}�b�s���O�L���� (�ǉ� bmp)
		glEnable(GL_TEXTURE_2D);
		//�@�e�N�X�`�����o�C���h (�ǉ� bmp)
		glBindTexture(GL_TEXTURE_2D, Texture[8].ID);
		glPushMatrix();
		glTranslatef(5.0, 0.5, -27.5);
		OBJFile[8].Draw();
		glPopMatrix();
	}

	if (appleFlag[3] == true) {
		//apple4
	// �e�N�X�`���}�b�s���O�L���� (�ǉ� bmp)
		glEnable(GL_TEXTURE_2D);
		//�@�e�N�X�`�����o�C���h (�ǉ� bmp)
		glBindTexture(GL_TEXTURE_2D, Texture[8].ID);
		glPushMatrix();
		glTranslatef(-18.5, 0.5, 0.0);
		OBJFile[8].Draw();
		glPopMatrix();
	}
}

//--------------------------------------------------------------------------------------------------------
// ��
//--------------------------------------------------------------------------------------------------------
void Sky() {
	//sky
	// �e�N�X�`���}�b�s���O�L���� (�ǉ� bmp)
	glEnable(GL_TEXTURE_2D);
	//�@�e�N�X�`�����o�C���h (�ǉ� bmp)
	glBindTexture(GL_TEXTURE_2D, Texture[9].ID);
	glPushMatrix();
	OBJFile[9].Draw();
	glPopMatrix();

	//skywal
// �e�N�X�`���}�b�s���O�L���� (�ǉ� bmp)
	//glEnable(GL_TEXTURE_2D);
	//�@�e�N�X�`�����o�C���h (�ǉ� bmp)
	//glBindTexture(GL_TEXTURE_2D, Texture[9].ID);
	//glPushMatrix();
	//OBJFile[11].Draw();
	//glPopMatrix();
}

//--------------------------------------------------------------------------------------------------------
// �����S���߂��ɂ����!���ł�
//--------------------------------------------------------------------------------------------------------
void AppleCheck() {
	glEnable(GL_TEXTURE_2D);										// �e�N�X�`���}�b�s���O�L���� (�ǉ� bmp)

	bool  ACflag = false;
	//�����S�̂����蔻��
	if (abs(position.x - 28) < 11 && abs(position.z - (26)) < 11) {
		if (appleFlag[0]) {
			glBindTexture(GL_TEXTURE_2D, Texture[27].ID);				// �e�N�X�`�����o�C���h (�ǉ� bmp)
			ACflag = true;
		}

	}
	if (abs(position.x - 12.0) < 11 && abs(position.z - (-27.5)) < 11) {
		if (appleFlag[1]) {
			glBindTexture(GL_TEXTURE_2D, Texture[27].ID);				// �e�N�X�`�����o�C���h (�ǉ� bmp)
			ACflag = true;
		}
		//ACflag = true;
	}
	if (abs(position.x - 3.0) < 11 && abs(position.z - (-27.5)) < 11) {
		if (appleFlag[2]) {
			glBindTexture(GL_TEXTURE_2D, Texture[27].ID);				// �e�N�X�`�����o�C���h (�ǉ� bmp)
			ACflag = true;
		}
		//ACflag = true;
	}
	if (abs(position.x - (-18.5)) < 11 && abs(position.z - 0) < 11) {
		if (appleFlag[3]) {
			glBindTexture(GL_TEXTURE_2D, Texture[27].ID);				// �e�N�X�`�����o�C���h (�ǉ� bmp)
			ACflag = true;
		}
		//ACflag = true;
	}
	if (ACflag == false) {
		glBindTexture(GL_TEXTURE_2D, Texture[4].ID);				// �e�N�X�`�����o�C���h (�ǉ� bmp)
	}
	glPushMatrix();
	ObjMoveRoll_Camera(horizontalAngle, verticalAngle, 3);
	//glTranslatef(-11.0, 1.05, -15.6);
	//glRotatef(90.0f,0.0f,1.0f,0.0f);
	OBJFile[10].Draw();
	glPopMatrix();
}

//--------------------------------------------------------------------------------------------------------
// �Q�[���I�[�o�[. �u�^�ɕ߂܂�
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
// �Q�[���N���A. ��񂲂�4���.
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
// �^�C���A�b�v. ���Ԑ؂�
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
// �w�肵���e�N�X�`������ʂ����ς��ɕ\��������.
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
// �����֌W
/////////////////////////////////////////////////////////////////
//----------------------------------------------------
// BGM/SE�̏�����
//----------------------------------------------------
void InitMusic() {

	sound_check = 0;	//��ʑJ�ڎ��ɂǂ�BGM���Đ�����̂������肷��ϐ�
	//se_pig_check = 0;	//�u�^�̖����̏��߂̂P��𐳊m�ɍĐ����邽��.
	se_pig_check = false;

	//---alut�̋@�\�̏�����(BGM)---//
	bgmStart = alutCreateBufferFromFile("BGM/Start.wav");
	bgmPlay = alutCreateBufferFromFile("BGM/Play2.wav");
	bgmResult = alutCreateBufferFromFile("BGM/Result1.wav");
	//bgmResult = alutCreateBufferFromFile("BGM/Result2.wav");


	//---alut�̋@�\�̏�����(SE)---//
	seCollisionWall = alutCreateBufferFromFile("SE/CollisionWall.wav");
	sePig = alutCreateBufferFromFile("SE/Pig_girl.wav");
	//sePig = alutCreateBufferFromFile("SE/Pig2.wav");
	//sePig = alutCreateBufferFromFile("SE/Pig3.wav");
	seGetItem = alutCreateBufferFromFile("SE/GetItem.wav");
	seGameover = alutCreateBufferFromFile("SE/gameover.wav");
	seGameclear = alutCreateBufferFromFile("SE/gameclear.wav");
	seTimeup = alutCreateBufferFromFile("SE/timeup.wav");

	volume_BGM = 0.8f;			//BGM�̉���
	volume_PIG = 1.0f;			//�u�^�̖����̉���
	volume_WALL = 1.0f;			//�ǂɏՓ˂����Ƃ��̉���
	volume_GetItem = 1.0f;		//�A�C�e�����擾�����Ƃ��̉���
	volume_gameover = 1.0f;		//�u�^�ɓ������ăQ�[���I�[�o�[�ɂȂ�Ƃ��̉���
	volume_gameclear = 1.0f;	//��񂲂�4����ăQ�[���N���A�ɂȂ�Ƃ��̉���
	volume_timeup = 1.0f;		//�������Ԑ؂�ɂȂ����Ƃ��̉���


	//---1�񉹂Ȃ点�΃u�^�̖����ɂ�����������Ȃ����Ƃ��킩�����̂ŏ������̃^�C�~���O��volume0�łȂ炷---//
	//soundPlay(seCollisionWall, &sseCollisionWall, false, 0.0f);
	//soundPlay(seGetItem, &sseGetItem, false, 0.0f);
	//soundPlay(seGameover, &sseGameover, false, 0.0f);
	//soundPlay(seGameclear, &sseGameclear, false, 0.0f);
	//soundPlay(seTimeup, &sseTimeup, false, 0.0f);
}

//----------------------------------------------------
// ���y���Đ�
//----------------------------------------------------
void soundPlay(ALuint buf, ALuint *source, bool loop, float volume) {
	bool music_dbg = false;
	//std::cout << "\nsoundPlay : " << source << std::endl << std::endl;
	ALenum error;
	alGetError();
	alGenSources(1, source);								// ���f�[�^�̃\�[�X�𐶐�
	if (music_dbg) {
		std::cout << "[1]";
		if ((error = alGetError()) != AL_NO_ERROR) {
			//std::cout << "source = " << &source << std::endl;
			noticeError(error);
		}
		else {
			std::cout << "�ŐV�̃G���[�͑��݂��܂���B\n";
		}
	}
	ALint state;
	alGetSourcei(*source, AL_SOURCE_STATE, &state);			// �����f�[�^�̃X�e�[�^�X���m�F����
	if (loop) {												// �Đ����鉹����LOOP�Đ�����BGM�Ȃ��
		//alGenSources(1, source);							// ���f�[�^�̃\�[�X�𐶐�
		alSourcef(*source, AL_GAIN, volume);				// ����
		alSourcei(*source, AL_BUFFER, buf);					// ���f�[�^�̃o�b�t�@���w��
		alSourcei(*source, AL_LOOPING, AL_TRUE);			// ���[�v�̐ݒ�
		alSourcePlay(*source);								// ���̍Đ�
	}
	else {													// �Đ����鉹����SE�Ȃ��
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
			alSourcef(*source, AL_GAIN, volume);			// ����
			alSourcei(*source, AL_BUFFER, buf);				// ���f�[�^�̃o�b�t�@���w��
			if (music_dbg) {
				std::cout << "[2]";
				if ((error = alGetError()) != AL_NO_ERROR) {
					noticeError(error);
				}
				else {
					std::cout << "�ŐV�̃G���[�͑��݂��܂���B\n";
				}
			}
			alSourcePlay(*source);							// SE���Đ�����//std::cout << "PLAY1\n";
		}
		else {
			if ((state != AL_PLAYING) && (state != AL_INITIAL)) {		// �Đ�����Ă��Ȃ����
				//std::cout << "PLAY2" << std::endl;
				//if (getOffset(*source) == 0) {						// �Đ�����Ă��Ȃ����
					//soundStop(source);								// ��x�s�v�ȃo�b�t�@���폜���Ă���
				alDeleteBuffers(1, source);
				//volume_PIG = 0.0f;
				//std::cout << "volume=" << volume << std::endl;
				alGetError();
				alGenSources(1, source);						// ���f�[�^�̃\�[�X�𐶐�
				alSourcef(*source, AL_GAIN, volume);			// ����
				alSourcei(*source, AL_BUFFER, buf);				// ���f�[�^�̃o�b�t�@���w��
				if (music_dbg) {
					std::cout << "[4]";
					if ((error = alGetError()) != AL_NO_ERROR) {
						noticeError(error);
					}
					else {
						std::cout << "�ŐV�̃G���[�͑��݂��܂���B\n";
					}
				}
				alSourcePlay(*source);							// SE���Đ�����
				//alDeleteSources(1, source);							// ���f�[�^�̃o�b�t�@�̍폜
			}
			else {
				//std::cout << "PLAY3\n";
			}
			//else if (getDuration(buf) <= getOffset(*source)-0.1) {	// �������Đ��I�����Ă����(�Đ����łȂ����)
			//	soundStop(source);								// ��x�s�v�ȃo�b�t�@���폜���Ă���
			//	alGenSources(1, source);								// ���f�[�^�̃\�[�X�𐶐�
			//	alSourcef(*source, AL_GAIN, volume);			// ����
			//	alSourcei(*source, AL_BUFFER, buf);				// ���f�[�^�̃o�b�t�@���w��
			//	alSourcePlay(*source);							// ���߂�SE���Đ�����
			//	std::cout << "PLAY2\n";
			//}
		}
	}
	//std::cout << "soundPlay" << buf << "," << source << std::endl;
	//return state;
}

//----------------------------------------------------
// ���y���~
//----------------------------------------------------
void soundStop(ALuint *source) {
	//std::cout << "soundStop : " << source << std::endl;	
	alSourceStop(*source);								// ���̒�~
	alDeleteSources(1, source);							// ���f�[�^�̃o�b�t�@�̍폜
	source = 0;											// ���f�[�^�̏�����
}

//----------------------------------------------------
// ���ׂẲ��y���~
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
// ���ׂĂ�SE���~
//----------------------------------------------------
void allSeStop() {
	//soundStop(&ssePig);
	soundStop(&sseGetItem);
	soundStop(&sseCollisionWall);
	//std::cout << '\n';

	//---���˂ɉ��������邱�Ƃ��������邽�߂��艟��---//
	//seCollisionWall = alutCreateBufferFromFile("SE/CollisionWall.wav");
	////sePig = alutCreateBufferFromFile("SE/Pig3.wav");
	//sePig = alutCreateBufferFromFile("SE/Pig2.wav");
	//seGetItem = alutCreateBufferFromFile("SE/GetItem.wav");
	//seGameover = alutCreateBufferFromFile("SE/gameover.wav");
	//seGameclear = alutCreateBufferFromFile("SE/gameclear.wav");
	//seTimeup = alutCreateBufferFromFile("SE/timeup.wav");
}

//----------------------------------------------------
// ���ׂẲ��y�o�b�t�@���폜
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
//  �����f�[�^�̎ڂ�b�P�ʂŎ擾
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
//  �����f�[�^�̎ڂ�b�P�ʂŎ擾
//--------------------------------------------------------------------------------------------------------
float getOffset(ALuint source) {
	float offset;
	alGetSourcef(source, AL_SEC_OFFSET, &offset);

	return offset;
}

//--------------------------------------------------------------------------------------------------------
//  �G���[���e�̕\��
//--------------------------------------------------------------------------------------------------------
void noticeError(ALenum error) {
	if (error == AL_INVALID_NAME) {
		std::cout << "�s���ȃl�[���̈����ł��B\n";
	}
	else if (error == AL_INVALID_ENUM) {
		std::cout << "�s���ȗ񋓒l�̈����ł��B\n";
	}
	else if (error == AL_INVALID_VALUE) {
		std::cout << "�s���Ȉ����ł�\n";
	}
	else if (error == AL_INVALID_OPERATION) {
		std::cout << "�֎~����Ă���Ăяo���ł��B\n";
	}
	else if (error == AL_OUT_OF_MEMORY) {
		std::cout << "�����������蓖�Ă鎖���o���܂���B\n";
	}
	else {
		std::cout << "�����s���ł��B\n";
	}
}


//--------------------------------------------------------------------------------------------------------
// �ǂƂ̓����蔻����s�����߂̕ǂ̒[�_���W�����肷��֐�
//--------------------------------------------------------------------------------------------------------
void decideCube(std::vector<glm::vec3> &vertices) {
	double x_max = 0, x_min = 0, z_max = 0, z_min = 0;
	double tmp;
	int ik = 0, il = 0;
	int k = 1, l = 1;
	double size = 0.15;	//�ǂɉ��z�I�Ȍ��݂�t����
	//double size = 5;	//�ǂɉ��z�I�Ȍ��݂�t����

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
// �ǂɏՓ˂����Ƃ���SE�𐧌䂷�邽�߂̑��i�q�b�g���m
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
// �u�^�Ǝ����Ƃ̋����ɉ����Ė����̑傫�����X�V����
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
// �I�u�W�F�N�g�̓ǂݍ��݂Ȃǂ̃u���b�N�{�b�N�X�����ꂽ�֐�����
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


////////////////////////////////////////////////////////////////////////
//  Pluse class
////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------
// Name : Pulse()
// Desc : �R���X�g���N�^#1
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
// Desc : �R���X�g���N�^#2
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
// Desc : �F�̐ݒ�
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
// Desc : �X�s�[�h�̐ݒ�
//---------------------------------------------------------------------------------------------------
void Pulse::SetSpeed(float speed)
{
	m_fSpeed = speed;
}

//---------------------------------------------------------------------------------------------------
// Name : SetPulse()
// Desc : �t���O�̐ݒ�
//---------------------------------------------------------------------------------------------------
void Pulse::SetPulse(bool flag)
{
	m_bPulse = flag;
}

//---------------------------------------------------------------------------------------------------
// Name : SetPulse()
// Desc : �^�C�v�̐ݒ�
//---------------------------------------------------------------------------------------------------
void Pulse::SetType(float type_tmp)
{
	type = type_tmp;
}

//---------------------------------------------------------------------------------------------------
// Name : GetColor()
// Desc : �F�̎擾
//---------------------------------------------------------------------------------------------------
float* Pulse::GetColor()
{
	return m_fColor;
}

//---------------------------------------------------------------------------------------------------
// Name : GetSpeed()
// Desc : �X�s�[�h�̎擾
//---------------------------------------------------------------------------------------------------
float Pulse::GetSpeed()
{
	return m_fSpeed;
}

//---------------------------------------------------------------------------------------------------
// Name : GetPulse()
// Desc : �t���O�̎擾
//---------------------------------------------------------------------------------------------------
bool Pulse::GetPulse()
{
	return m_bPulse;
}

//------------------------------------------------------------------------
// Name : GetDirectionPath()
// Desc : �����񂩂�f�B���N�g�����擾
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
// Desc : �����񂩂�f�B���N�g�����폜
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
// Desc : �f�B���N�g����O�ɕt�����ĕ������Ԃ�
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
// Desc : �}�e���A���̏�����
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
// Desc : �}�e���A����ݒ肷��
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
// Desc : �X�P�[���{����
//-----------------------------------------------------------------------
OBJVEC3 OBJVEC3Scale(OBJVEC3 v, float scale)
{
	return OBJVEC3(v.x * scale, v.y * scale, v.z * scale);
}

//-----------------------------------------------------------------------
// Name : OBJVEC3Add()
// Desc : �����Z����
//-----------------------------------------------------------------------
OBJVEC3 OBJVEC3Add(OBJVEC3 v1, OBJVEC3 v2)
{
	return OBJVEC3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

//-----------------------------------------------------------------------
// Name : OBJVEC3Substract()
// Desc : �����Z����
//-----------------------------------------------------------------------
OBJVEC3 OBJVEC3Substract(OBJVEC3 v1, OBJVEC3 v2)
{
	return OBJVEC3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

//-----------------------------------------------------------------------
// Name : OBJVEC3Length()
// Desc : ���������߂�
//-----------------------------------------------------------------------
float OBJVEC3Length(OBJVEC3 v)
{
	return sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
}

//-----------------------------------------------------------------------
// Name : OBJVEC3Max()
// Desc : �ő�l�����߂�
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
// Desc : �ŏ��l�����߂�
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
// Desc : �R���X�g���N�^
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
// Desc : �R���X�g���N�^
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
// Desc : �R���X�g���N�^
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
// Desc : �R���X�g���N�^
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
// Desc : �f�X�g���N�^
//-----------------------------------------------------------------------
OBJMESH::~OBJMESH()
{
	Release();
};

//-----------------------------------------------------------------------
// Name : Release()
// Desc : ��������j��
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
// Desc : OBJ�t�@�C���̓ǂݍ���
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

	//�@�f�B���N�g����؂���
	strcpy_s(m_directoryPath, GetDirectoryPath(filename));

	//�@�t�@�C�����J��
	file.open(filename, ios::in);

	//�@�`�F�b�N
	if (!file.is_open())
	{
		cerr << "Error : �t�@�C���I�[�v���Ɏ��s\n";
		cerr << "File Name : " << filename << endl;
		return false;
	}

	//�@���[�v
	for (;; )
	{
		file >> buf;
		if (!file)
			break;

		//�@�R�����g
		if (0 == strcmp(buf, "#"))
		{
		}

		//�@���_���W
		else if (0 == strcmp(buf, "v"))
		{
			float x, y, z;
			file >> x >> y >> z;
			OBJVEC3 v(x, y, z);
			positions.push_back(v);

			//�@�o�E���f�B���O�{�b�N�X�̏�����
			if (!initBox)
			{
				m_Box = OBJBOUNDINGBOX(v);
				initBox = true;
			}

			//�@�o�E���f�B���O�{�b�N�X�̎Z�o
			m_Box.Merge(v);
		}

		//�@�e�N�X�`�����W
		else if (0 == strcmp(buf, "vt"))
		{
			float u, v;
			file >> u >> v;
			texcoords.push_back(OBJVEC2(u, v));
		}

		//�@�@���x�N�g��
		else if (0 == strcmp(buf, "vn"))
		{
			float x, y, z;
			file >> x >> y >> z;
			normals.push_back(OBJVEC3(x, y, z));
		}

		//�@��
		else if (0 == strcmp(buf, "f"))
		{
			unsigned int iPosition, iTexCoord, iNormal;
			unsigned int p[4] = { -1 }, t[4] = { -1 }, n[4] = { -1 };
			OBJVERTEX vertex;
			dwFaceIndex++;
			dwFaceCount++;
			int count = 0;
			unsigned int index = 0;

			//�@�O�p�`�E�l�p�`�̂ݑΉ�
			for (int iFace = 0; iFace < 4; iFace++)
			{
				count++;	//�@���_���𐔂���
				ZeroMemory(&vertex, sizeof(OBJVERTEX));

				file >> iPosition;
				vertex.position = positions[iPosition - 1];
				p[iFace] = iPosition - 1;

				if ('/' == file.peek())
				{
					file.ignore();

					//�@�e�N�X�`�����W�C���f�b�N�X
					if ('/' != file.peek())
					{
						file >> iTexCoord;
						vertex.texcoord = texcoords[iTexCoord - 1];
						t[iFace] = iTexCoord - 1;
					}

					//�@�@���x�N�g���C���f�b�N�X
					if ('/' == file.peek())
					{
						file.ignore();

						file >> iNormal;
						vertex.normal = normals[iNormal - 1];
						n[iFace] = iNormal - 1;
					}
				}

				//�@�J�E���g��3����
				if (iFace < 3)
				{
					t_vertices.push_back(vertex);
					index = t_vertices.size() - 1;
					t_indices.push_back(index);
				}

				//�@�������s��������I��
				if ('\n' == file.peek())
				{
					break;
				}

			}

			//�@�l�p�`�|���S���̏ꍇ�C�O�p�`��ǉ�����
			if (count > 3)
			{
				//�@�J�E���g
				dwFaceIndex++;
				dwFaceCount++;

				//�@���_�ƃC���f�b�N�X��ǉ�
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

		//�@�}�e���A���t�@�C��
		else if (0 == strcmp(buf, "mtllib"))
		{
			file >> mtlFileName;
			//�@�}�e���A���t�@�C���̓ǂݍ���
			if (mtlFileName[0])
			{
				if (!LoadMTLFile(SetDirectoryPath(mtlFileName, m_directoryPath)))
				{
					cerr << "Error : �}�e���A���̃��[�h�Ɏ��s\n";
					return false;
				}
			}
		}

		//�@�}�e���A��
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

	//�@�T�u�Z�b�g
	if (t_subsets.size() > 0)
	{
		int maxSize = t_subsets.size();
		t_subsets[maxSize - 1].faceCount = dwFaceCount * 3;
	}

	//�@�t�@�C�������
	file.close();

	//�@���_�f�[�^���R�s�[
	m_NumVertices = t_vertices.size();
	m_Vertices = new OBJVERTEX[m_NumVertices];
	for (unsigned int i = 0; i < m_NumVertices; i++)
		m_Vertices[i] = t_vertices[i];

	//�@�T�u�Z�b�g�f�[�^���R�s�[
	m_NumSubsets = t_subsets.size();
	m_Subsets = new OBJSUBSET[m_NumSubsets];
	for (unsigned int i = 0; i < m_NumSubsets; i++)
		m_Subsets[i] = t_subsets[i];

	//�@�C���f�b�N�X�f�[�^���R�s�[
	m_NumIndices = t_indices.size();
	m_Indices = new unsigned int[m_NumIndices];
	for (unsigned int i = 0; i < m_NumIndices; i++)
		m_Indices[i] = t_indices[i];

	//�@�o�E���f�B���O�X�t�B�A�̍쐬
	m_Sphere.Create(m_Box);

	//�@�������j��
	positions.clear();
	normals.clear();
	texcoords.clear();
	t_vertices.clear();
	t_subsets.clear();
	t_indices.clear();

	//�@����I��
	return true;
}

//-----------------------------------------------------------------------
// Name : LoadMTLFile()
// Desc : MTL�t�@�C���̓ǂݍ���
//-----------------------------------------------------------------------
bool OBJMESH::LoadMTLFile(const char* filename)
{
	char buf[OBJ_BUFFER_LENGTH] = { 0 };
	int iMtlCount = -1;
	ifstream file;
	vector<OBJMATERIAL> t_materials;
	OBJMATERIAL material;
	InitMaterial(&material);

	//�@�t�@�C�����J��
	file.open(filename, ios::in);

	//�@�`�F�b�N
	if (!file.is_open())
	{
		cerr << "Error : �t�@�C���I�[�v���Ɏ��s���܂���\n";
		cerr << "File Name : " << filename << endl;
		return false;
	}

	//�@���[�v
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

	//�@�t�@�C�������
	file.close();

	//�@�}�e���A���f�[�^���R�s�[
	m_NumMaterials = t_materials.size();
	m_Materials = new OBJMATERIAL[m_NumMaterials];
	for (unsigned int i = 0; i < m_NumMaterials; i++)
		m_Materials[i] = t_materials[i];

	//�@����I��
	return true;
}

//-----------------------------------------------------------------------
// Name : LoadFile()
// Desc : ���b�V���t�@�C���̓ǂݍ���
//-----------------------------------------------------------------------
bool OBJMESH::LoadFile(const char* filename)
{
	//�@OBJ, MTL�t�@�C����ǂݍ���
	if (!LoadOBJFile(filename))
	{
		cerr << "Error : ���b�V���t�@�C���̓ǂݍ��݂Ɏ��s���܂���\n";
		return false;
	}

	//�@����I��
	return true;
}


//-----------------------------------------------------------------------
// Name : Draw()
// Desc : �`�揈��
//-----------------------------------------------------------------------
void OBJMESH::Draw()
{
	for (unsigned int i = 0; i < m_NumSubsets; i++)
	{
		//�@�}�e���A��
		OBJMATERIAL* pMat = &m_Materials[m_Subsets[i].materialIndex];
		SetMaterial(pMat);

		//�@�O�p�`�`��
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
// Desc : ���_�����擾
//------------------------------------------------------------------------
unsigned int OBJMESH::GetNumVertices()
{
	return m_NumVertices;
}

//-----------------------------------------------------------------------
// Name : GetNumSubsets()
// Desc : �T�u�Z�b�g�����擾
//-----------------------------------------------------------------------
unsigned int OBJMESH::GetNumSubsets()
{
	return m_NumSubsets;
}

//-----------------------------------------------------------------------
// Name : GetNumMaterials()
// Desc : �}�e���A�������擾
//-----------------------------------------------------------------------
unsigned int OBJMESH::GetNumMaterials()
{
	return m_NumMaterials;
}

//-----------------------------------------------------------------------
// Name : GetNumIndices()
// Desc : �C���f�b�N�X�����擾
//-----------------------------------------------------------------------
unsigned int OBJMESH::GetNumIndices()
{
	return m_NumIndices;
}

//-----------------------------------------------------------------------
// Name : GetIndexData()
// Desc : �w�肳�ꂽ�C���f�b�N�X�f�[�^���擾
//-----------------------------------------------------------------------
unsigned int OBJMESH::GetIndexData(unsigned int index)
{
	return m_Indices[index];
}

//-----------------------------------------------------------------------
// Name : GetVertex()
// Desc : �w�肳�ꂽ���_�f�[�^���擾
//-----------------------------------------------------------------------
OBJVERTEX OBJMESH::GetVertex(unsigned int index)
{
	return m_Vertices[index];
}

//-----------------------------------------------------------------------
// Name : GetSubsets()
// Desc : �w�肳�ꂽ�T�u�Z�b�g�f�[�^���擾
//-----------------------------------------------------------------------
OBJSUBSET OBJMESH::GetSubset(unsigned int index)
{
	return m_Subsets[index];
}

//-----------------------------------------------------------------------
// Name : GetMaterial()
// Desc : �w�肳�ꂽ�}�e���A���f�[�^���擾
//-----------------------------------------------------------------------
OBJMATERIAL OBJMESH::GetMaterial(unsigned int index)
{
	return m_Materials[index];
}

//-----------------------------------------------------------------------
// Name : GetVertices()
// Desc : ���_�f�[�^���擾
//-----------------------------------------------------------------------
OBJVERTEX* OBJMESH::GetVertices()
{
	return m_Vertices;
}

//-----------------------------------------------------------------------
// Name : GetSubsets()
// Desc : �T�u�Z�b�g�f�[�^���擾
//-----------------------------------------------------------------------
OBJSUBSET* OBJMESH::GetSubsets()
{
	return m_Subsets;
}

//-----------------------------------------------------------------------
// Name : GetMaterails()
// Desc : �}�e���A���f�[�^���擾
//-----------------------------------------------------------------------
OBJMATERIAL* OBJMESH::GetMaterials()
{
	return m_Materials;
}

//-----------------------------------------------------------------------
// Name : GetIndices()
// Desc : �C���f�b�N�X�f�[�^���擾
//-----------------------------------------------------------------------
unsigned int* OBJMESH::GetIndices()
{
	return m_Indices;
}

//-----------------------------------------------------------------------
// Name : GetBox()
// Desc : �o�E���f�B���O�{�b�N�X���擾
//-----------------------------------------------------------------------
OBJBOUNDINGBOX OBJMESH::GetBox()
{
	return m_Box;
}

//-----------------------------------------------------------------------
// Name : GetSphere()
// Desc : �o�E���f�B���O�X�t�B�A���擾
//-----------------------------------------------------------------------
OBJBOUNDINGSPHERE OBJMESH::GetSphere()
{
	return m_Sphere;
}



//////////////////////////////////////////////////////////////////////////
//�@�@BMPImage class
//////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------------------------------
//�@�@BMPImage
//�@�@Desc : �R���X�g���N�^
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
//�@�@~BMPImage
//�@�@Desc : �f�X�g���N�^
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
//�@�@ReadBMP
//�@�@Desc : BMP�t�@�C���̓ǂݍ���
//-----------------------------------------------------------------------------------------------------


bool BMPImage::ReadBMP(const char *filename)
{
	FILE *fp;

	BITMAPINFOHEADER bitmapInfoHeader;
	BITMAPFILEHEADER header;
	GLubyte temp = 0;

	//�@�t�@�C�����J��
	if ((fp = fopen(filename, "rb")) == NULL)
	{
		cout << "Error : �w�肳�ꂽ�t�@�C�����J���܂���\n";
		cout << "File Name : " << filename << endl;
		return false;
	}

	// �t�@�C�����J���܂��B
//FILE * file;
//= fopen(imagepath, "rb");
//errno_t error;
//error = fopen_s(&file, imagepath, "rb");

//�@�w�b�_�[���̓ǂݎ��
	fread(&header, 1, sizeof(BITMAPFILEHEADER), fp);

	//if (fread(header, 1, 54, file) != 54) { // 54�o�C�g�ǂݍ��߂Ȃ���΁A��肪����܂��B
	//	printf("BMP�t�@�C���ł͂���܂���B\n");
	//	return false;
	//}
		// �t�@�C��������ۂ̃f�[�^���o�b�t�@�ɓǂݍ���
	//fread(data, 1, imageSize, file);

	//�@�t�@�C���`�F�b�N
	if (header.bfType != 0x4d42)
	{
		cout << "Error : Bitmap�t�@�C���ł͂���܂���\n";
		fclose(fp);
		return false;
	}

	//�@�w�b�_�[���̓ǂݎ��
	fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, fp);

	//�@���ƍ������擾
	width = bitmapInfoHeader.biWidth;
	height = bitmapInfoHeader.biHeight;

	if (bitmapInfoHeader.biSizeImage == 0)
	{
		bitmapInfoHeader.biSizeImage = bitmapInfoHeader.biWidth * bitmapInfoHeader.biHeight * 3;
	}

	fseek(fp, header.bfOffBits, SEEK_SET);

	//�@�f�[�^�T�C�Y�����肵�C���������m��
	imageSize = bitmapInfoHeader.biSizeImage;
	imageData = new GLubyte[imageSize];

	//�@�G���[�`�F�b�N
	if (!imageData)
	{
		cout << "Error : ���������m�ۂł��܂���ł���\n";
		delete[] imageData;
		fclose(fp);
		return false;
	}

	//�@�s�N�Z���f�[�^�̓ǂݍ���
	fread(imageData, 1, bitmapInfoHeader.biSizeImage, fp);

	//�@BGR�@���@RGB�ɕϊ�
	for (int i = 0; i < (int)imageSize; i += 3)
	{
		temp = imageData[i + 0];
		imageData[i + 0] = imageData[i + 2];
		imageData[i + 2] = temp;
	}

	//�@�t�@�C�������
	fclose(fp);

	return true;
}

//-----------------------------------------------------------------------------------------------------
//�@�@Load
//�@�@Desc : BMP�t�@�C����ǂݍ��݁C�e�N�X�`���𐶐�
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
// �F�̒�`�̒�`
//----------------------------------------------------
GLfloat red[] = { 0.8, 0.2, 0.2, 1.0 }; //�ԐF
GLfloat green[] = { 0.2, 0.8, 0.2, 1.0 };//�ΐF
GLfloat blue[] = { 0.2, 0.2, 0.8, 1.0 };//�F
GLfloat yellow[] = { 0.8, 0.8, 0.2, 1.0 };//���F
GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };//���F
GLfloat black[] = { 0.0, 0.0, 0.0, 1.0 };//���F
GLfloat shininess = 30.0;//����̋���






//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ��ł��������ǂ̓w�͂̌���. OpenGL�}�X�^�[�͌N������
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
GLuint loadBMP_custom(const char * imagepath) {
	// BMP�t�@�C���̃w�b�_����ǂݍ��܂��f�[�^
	unsigned char header[54]; // �eBMP�t�@�C����54�o�C�g�̃w�b�_����n�܂�܂��B
	unsigned int dataPos;     // ���ۂ̃f�[�^���t�@�C���̂ǂ̈ʒu�ɂ��邩
	unsigned int width, height;
	unsigned int imageSize;   // = ��*����*3
	// ���ۂ�RGB�f�[�^
	unsigned char * data;

	// �t�@�C�����J���܂��B
	FILE * file;
	//= fopen(imagepath, "rb");
	errno_t error;
	error = fopen_s(&file, imagepath, "rb");



	if (error) {
		printf("�摜���J���܂���ł����B\n");
		return 0;
	}

	if (fread(header, 1, 54, file) != 54) { // 54�o�C�g�ǂݍ��߂Ȃ���΁A��肪����܂��B
		printf("BMP�t�@�C���ł͂���܂���B\n");
		return false;
	}

	if (header[0] != 'B' || header[1] != 'M') {
		printf("BMP�t�@�C���ł͂���܂���B\n");
		return 0;
	}

	// �o�C�g�z�񂩂琮����ǂݍ���
	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);

	// BMP�t�@�C���̒��ɂ̓~�X�t�H�[�}�b�g������܂��B���̏��𐄑����܂��B
	if (imageSize == 0)    imageSize = width * height * 3; // 3 : 1�o�C�g�͐ԁA�΁A�̊e�F�ł��B
	if (dataPos == 0)      dataPos = 54; // �����BMP�w�b�_�͏I���ł��B

	// �o�b�t�@�����
	data = new unsigned char[imageSize];

	// �t�@�C��������ۂ̃f�[�^���o�b�t�@�ɓǂݍ���
	fread(data, 1, imageSize, file);

	//���ׂĂ̓�������ɂ���̂ŁA�t�@�C���͕��܂��B
	fclose(file);

	// �ЂƂ�OpenGL�e�N�X�`�������܂��B
	GLuint textureID;
	glGenTextures(1, &textureID);

	// �V���ɍ��ꂽ�e�N�X�`����"�o�C���h"���܂��B�܂肱�������̃e�N�X�`���֐��͂��̃e�N�X�`����ύX���܂��B
	glBindTexture(GL_TEXTURE_2D, textureID);

	// OpenGL�ɉ摜��n���܂��B
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// �摜���g��(MAGnifying)����Ƃ��͐��`(LINEAR)�t�B���^�����O���g���܂��B
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// �摜���k��(MINifying)����Ƃ��A���`(LINEAR)�t�B���^�����A��̃~�b�v�}�b�v����`(LINEARYLY)�ɍ��������̂��g���܂��B
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	// ���̂悤�ɂ��ă~�b�v�}�b�v�����܂��B
	glGenerateMipmap(GL_TEXTURE_2D);

	return textureID;

}

GLuint loadTGA_glfw(const char * imagepath) {

	// ���OpenGL�e�N�X�`�������܂��B
	GLuint textureID;
	glGenTextures(1, &textureID);

	// �V����������e�N�X�`����"�o�C���h"���܂��B����ȍ~�̊֐��͂��ׂĂ��̃e�N�X�`����ύX���܂��B
	glBindTexture(GL_TEXTURE_2D, textureID);

	// �������p�����[�^��glTexImage2D���Ă�ŁA�t�@�C����ǂݍ��݂܂��B
	//glfwLoadTexture2D(imagepath, 0);

	// 3�̐��`�t�B���^�����O
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	// �쐬�����e�N�X�`����ID��Ԃ��܂��B
	return textureID;
}

GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path) {

	// �V�F�[�_�����܂��B
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// �t�@�C�����璸�_�V�F�[�_�̃R�[�h��ǂݍ��݂܂��B
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open())
	{
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}

	// �t�@�C������t���O�����g�V�F�[�_��ǂݍ��݂܂��B
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


	// ���_�V�F�[�_���R���p�C�����܂��B
	printf("Compiling shader : %sn", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// ���_�V�F�[�_���`�F�b�N���܂��B
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);

	//std::vector<char> VertexShaderErrorMessage(InfoLogLength);
	//glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
	//fprintf(stdout, "%sn", &VertexShaderErrorMessage[0]);

	// �t���O�����g�V�F�[�_���R���p�C�����܂��B
	printf("Compiling shader : %sn", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// �t���O�����g�V�F�[�_���`�F�b�N���܂��B
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);

	//std::vector<char> FragmentShaderErrorMessage(InfoLogLength);
	//glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
	//fprintf(stdout, "%sn", &FragmentShaderErrorMessage[0]);


	// �v���O�����������N���܂��B
	fprintf(stdout, "Linking programn");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// �v���O�������`�F�b�N���܂��B
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
	//	printf("�t�@�C�����J���܂���!n");
	//	return false;
	//}

	// �t�@�C�����J���܂��B
	FILE * file;
	errno_t error;
	error = fopen_s(&file, path, "r");
	char lineHeader[2048];
	bool dbg = false;					//true�ɂ����, �A���t�@�x�b�g����������o�Ă���f�o�b�O���J�n�����
	while (1) {

		// �s�̍ŏ��̕������ǂݍ��݂܂��B
		int res = fscanf_s(file, "%s", lineHeader, _countof(lineHeader));
		if (res == EOF) {
			if (dbg) printf("b");
			// �e�O�p�`�̊e���_
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
			break; // EOF = End Of File. ���[�v���I�����܂��B
		}
		// �����łȂ����lineHeader���p�[�X���܂��B

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
	//	printf("�t�@�C�����J���܂���!n");
	//	return false;
	//}

	// �t�@�C�����J���܂��B
	FILE * file;
	errno_t error;
	error = fopen_s(&file, path, "r");

	while (1) {

		char lineHeader[1024];
		// �s�̍ŏ��̕������ǂݍ��݂܂��B
		int res = fscanf_s(file, "%s", lineHeader, _countof(lineHeader));
		if (res == EOF) {
			printf("b");
			// �e�O�p�`�̊e���_
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
			break; // EOF = End Of File. ���[�v���I�����܂��B
		}
		// �����łȂ����lineHeader���p�[�X���܂��B

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
	glGenBuffers(1, vertexbuffer);									// �o�b�t�@��1���Avertexbuffer�Ɍ���ID�����܂��B
	glBindBuffer(GL_ARRAY_BUFFER, *vertexbuffer);					// ���̃R�}���h��'vertexbuffer'�o�b�t�@�ɂ��Ăł��B
	//if (!loadOBJ(calc_OBJFile.c_str(), vertices, uvs, normals, *ver)) {	// ����obj�t�@�C����ǂݍ��߂Ȃ�������
	//	//printf("[ERROR] No File of %s\n", calc_OBJFile.c_str());	// �G���[����Ԃ�
	//}
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, uvbuffer);										// �o�b�t�@��1���Auvbuffer�Ɍ���ID�����܂��B
	glBindBuffer(GL_ARRAY_BUFFER, *uvbuffer);						// ���̃R�}���h��'uvbuffer'�o�b�t�@�ɂ��Ăł��B
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

	glGenVertexArrays(1, VertexArrayID);							// �o�b�t�@��1���AVertexArrayID�Ɍ���ID�����܂��B
	glBindVertexArray(*VertexArrayID);								// ���̃R�}���h��'VertexArrayID'�o�b�t�@�ɂ��Ăł��B

	// �ŏ��̑����o�b�t�@�F���_
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, *vertexbuffer);
	glVertexAttribPointer(
		0,                  // ����0�F0�ɓ��ɗ��R�͂���܂���B�������A�V�F�[�_����layout�Ƃ��킹�Ȃ��Ƃ����܂���B
		3,                  // �T�C�Y
		GL_FLOAT,           // �^�C�v
		GL_FALSE,           // ���K���H
		0,                  // �X�g���C�h
		(void*)0            // �z��o�b�t�@�I�t�Z�b�g
	);


	// 2nd attribute buffer : colors
	glEnableVertexAttribArray(1);
	//glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, *uvbuffer);
	glVertexAttribPointer(
		1,                                // �����B1�Ƃ��������ɈӖ��͂���܂���B�������V�F�[�_��layout�Ƃ��킹��K�v������܂��B
		2,                                // �T�C�Y
		GL_FLOAT,                         // �^�C�v
		GL_FALSE,                         // ���K���H
		0,                                // �X�g���C�h
		(void*)0                          // �z��o�b�t�@�I�t�Z�b�g
	);

	// ���_��OpenGL�ɓn���܂��B
	//glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
}

void prosessingOfMoveOBJ(int *ver, GLuint *vertexbuffer, std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals, std::vector<glm::vec2>& uvs, std::string calc_OBJFile, GLuint *uvbuffer, GLuint *VertexArrayID) {
	//glGenBuffers(1, vertexbuffer);									// �o�b�t�@��1���Avertexbuffer�Ɍ���ID�����܂��B
	glBindBuffer(GL_ARRAY_BUFFER, *vertexbuffer);					// ���̃R�}���h��'vertexbuffer'�o�b�t�@�ɂ��Ăł��B
	//if (!loadOBJ(calc_OBJFile.c_str(), vertices, uvs, normals, *ver)) {	// ����obj�t�@�C����ǂݍ��߂Ȃ�������
	//	printf("[ERROR] No File of %s\n", calc_OBJFile);					// �G���[����Ԃ�
	//}

	//std::cout << calc_OBJFile.c_str() << std::endl;

	//std::cout << "w";


	float objx_max = 0, objx_min = 0, objz_max = 0, objz_min = 0;
	float tmp;
	float obji = 0; double objj = 0;


	float obj_speed = 0.001;  //object�̃X�s�[�h
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


	//glGenBuffers(1, uvbuffer);										// �o�b�t�@��1���Auvbuffer�Ɍ���ID�����܂��B
	glBindBuffer(GL_ARRAY_BUFFER, *uvbuffer);						// ���̃R�}���h��'uvbuffer'�o�b�t�@�ɂ��Ăł��B
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

	//glGenVertexArrays(1, VertexArrayID);							// �o�b�t�@��1���AVertexArrayID�Ɍ���ID�����܂��B
	glBindVertexArray(*VertexArrayID);								// ���̃R�}���h��'VertexArrayID'�o�b�t�@�ɂ��Ăł��B

	// �ŏ��̑����o�b�t�@�F���_
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, *vertexbuffer);
	glVertexAttribPointer(
		0,                  // ����0�F0�ɓ��ɗ��R�͂���܂���B�������A�V�F�[�_����layout�Ƃ��킹�Ȃ��Ƃ����܂���B
		3,                  // �T�C�Y
		GL_FLOAT,           // �^�C�v
		GL_FALSE,           // ���K���H
		0,                  // �X�g���C�h
		(void*)0            // �z��o�b�t�@�I�t�Z�b�g
	);


	// 2nd attribute buffer : colors
	glEnableVertexAttribArray(1);
	//glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, *uvbuffer);
	glVertexAttribPointer(
		1,                                // �����B1�Ƃ��������ɈӖ��͂���܂���B�������V�F�[�_��layout�Ƃ��킹��K�v������܂��B
		2,                                // �T�C�Y
		GL_FLOAT,                         // �^�C�v
		GL_FALSE,                         // ���K���H
		0,                                // �X�g���C�h
		(void*)0                          // �z��o�b�t�@�I�t�Z�b�g
	);

	// ���_��OpenGL�ɓn���܂��B
	//glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
}

glm::mat4 getProjectionMatrix() {

	//glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
	return ProjectionM;
}

glm::mat4 getViewMatrix() {

	//glm::mat4 View = glm::lookAt(
	//	glm::vec3(4, 3, 3), // ���[���h��ԂŃJ������(4,3,3)�ɂ���܂��B
	//	glm::vec3(0, 0, 0), // ���_�����Ă��܂��B
	//	glm::vec3(0, 1, 0)  // ���������(0,-1,0�ɃZ�b�g����Ə㉺�t�]���܂��B)
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

	float obj_speed = 0.001;  //object�̃X�s�[�h ���g���ĂȂ�
	float dif_x, dif_z, sq;
	float obj_xspeed, obj_zspeed;

	double x = movecount;

	glm::mat4 movematrix;
	glm::mat4 movemat;


	if (i == 7) {
		//std::cout << "r";
		//glRotatef(10, 1.0f, 0.0f, 0.0f);
		//Model = getModelMatrix(i);
		//���s�ړ��̕ϊ��s��
		movemat = glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, -5 + x, -1, 0, 1);

		//movemat = glm::mat4(1.0f) * movematrix;
		//mat4 ModelMatrix = RotationMatrix ;
		//mat4 ModelMatrix = TranslationMatrix * RotationMatrix * ScaleMatrix;
		// MVP�s�����邽�߂Ƀ��f���s����g���܂��B

	}

	return movemat;

}

glm::mat4 ObjRoll(int i, double RadianAngle) {
	//glBindBuffer(GL_ARRAY_BUFFER, *vertexbuffer);					// ���̃R�}���h��'vertexbuffer'�o�b�t�@�ɂ��Ăł��B
	//glLoadIdentity();
	double pi = 3.1415;
	double onedegree = pi / 180.0; //1���̃��W�A��
	glm::mat4 rollmat;

	//��]
	glm::vec3 RotationAxis;
	//�΂̎g�����Y��Ă��܂���
	double RotationAngle = RadianAngle;
	RotationAxis.x = 0.0f;
	RotationAxis.y = 1.0f;
	RotationAxis.z = 0.0f;

	// RotationAngle�̓��W�A���ŕ\���܂��B
	//double x = RotationAxis.x * sin(RotationAngle / 2);
	//double y = RotationAxis.y * sin(RotationAngle / 2);
	//double z = RotationAxis.z * sin(RotationAngle / 2);
	//double w = cos(RotationAngle / 2);

	// �P�ʃN�H�[�^�j�I�������i���̉�]�������Ȃ��j
	quat MyQuaternion;

	// 4�v�f�̒��ړI�ȋL�q
	// ����𒼐ڎg�����Ƃ͂Ȃ��ł��傤�B
	//MyQuaternion = quat(w, x, y, z);

	// �I�C���[�p���烉�W�A���̃N�H�[�^�j�I���ւ̕ϊ�
	//vec3 EulerAngles(90, 0, 0);
	//MyQuaternion = quat(EulerAngles);

	//std::cout << degrees(RotationAngle)<<"\n";

	//MyQuaternion = glm::angleAxis(float(degrees(RotationAngle)), RotationAxis);
	MyQuaternion = glm::angleAxis(float(RotationAngle), RotationAxis);

	// ��-�p����̕ϊ�
	// GLM�ł͊p�x�͓x�ŕ\���̂ŁA�ϊ����܂��B
	//MyQuaternion = glmangleAxis(degrees(RotationAngle), RotationAxis);

	//vec3 EulerAngles(RotationAroundXInRadians, RotationAroundYInRadians, RotationAroundZInRadians);

	rollmat = glm::toMat4(MyQuaternion);

	//mat4 ModelMatrix = RotationMatrix ;
	//mat4 ModelMatrix = TranslationMatrix * RotationMatrix * ScaleMatrix;
	// MVP�s�����邽�߂Ƀ��f���s����g���܂��B
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
	//�g��s��
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
		//���s�ړ��̕ϊ��s��
		movemat = glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, x + Ax, y, z + Az, 1);

		movemat = movemat * rollmat;

		//movemat = glm::mat4(1.0f) * movematrix;
		//mat4 ModelMatrix = RotationMatrix ;
		//mat4 ModelMatrix = TranslationMatrix * RotationMatrix * ScaleMatrix;
		// MVP�s�����邽�߂Ƀ��f���s����g���܂��B

	}

	return movemat;

}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
