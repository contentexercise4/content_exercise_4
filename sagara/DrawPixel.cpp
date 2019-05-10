#include "DxLib.h"
#include<math.h>
#include<stdlib.h>
#include<string>

#define SHOTCOU 20
#define ENEMYCOU 11
struct Shot {
	bool ShotFlag=false;
	int ShotX, ShotY;
	int ShotSpeed , ShotSpeedX, ShotSpeedY;
};
class Charactor {
private:
	int LifePoint;
	double SpeedX, SpeedY;
public :
	int X,Y;
	Shot shot[SHOTCOU];
	int ShotCount;

	//�R���X�g���N�^
	Charactor() {
		this->X = 200;
		this->Y = 400;
		this->LifePoint = 3;
		this->SpeedX = 4;
		this->SpeedY = 4;
		this->ShotCount = 0;
	}
	Charactor(int X,int Y,int LifePoint,int SpeedX,int SpeedY) {
		this->X = X;
		this->Y = Y;
		this->LifePoint = LifePoint;
		this->SpeedX = SpeedX;
		this->SpeedY = SpeedY;
		this->ShotCount = 0;
	}
	//���C�t�𑝌�
	void ChangeLife(int LifeChange) {
		LifePoint += LifeChange;
	}
	//���C�t��Ԃ�
	int ReturnLife() {
		return LifePoint;
	}
	//���x���Z�b�g
	void SetSpeed(int SpeedX, int SpeedY) {
		this->SpeedX = SpeedX;
		this->SpeedY = SpeedY;
	}
	//X���̑��x��Ԃ�
	int ReturnSpeedX() {
		return SpeedX;
	}
	//Y���̑��x��Ԃ�
	int ReturnSpeedY() {
		return SpeedY;
	}
	//�e�����ׂď���
	void ShotReset() {
		for (int i = 0; i < SHOTCOU; i++) {
			shot[i].ShotFlag = false;
		}
	}
};

class Player : public Charactor{
private:

public:
	//���@�̌����𐔎��ŊǗ�
	int PlayerDirX, PlayerDirY;
	int PastPlayerDirX, PastPlayerDirY;

	//�R���X�g���N�^
	Player():Charactor() {
		PlayerDirX = 0;
		PlayerDirY = -1;
		PastPlayerDirX = 0;
		PastPlayerDirY = -1;
	}
	Player(int X, int Y, int LifePoint, int SpeedX, int SpeedY) :Charactor(X, Y, LifePoint, SpeedX, SpeedY) {
		PlayerDirX = 0;
		PlayerDirY = -1;
		PastPlayerDirX = 0;
		PastPlayerDirY = -1;
	}
	Player(int X, int Y, int LifePoint, int SpeedX, int SpeedY, int PlayerDirX, int PlayerDirY) :Charactor(X, Y, LifePoint, SpeedX, SpeedY) {
		this->PlayerDirX = PlayerDirX;
		this->PlayerDirY = PlayerDirY;
		PastPlayerDirX = PlayerDirX;
		PastPlayerDirY = PlayerDirY;
	}
};

class Enemy : public Charactor {
private:

public :
	//�G�@�̌����𐔎��ŊǗ�
	int EnemyDirX, EnemyDirY;
	int PastEnemyDirX, PastEnemyDirY;
	int DamageCounter;
	bool ExistFlag, DamageFlag;

	//�R���X�g���N�^
	Enemy() :Charactor() {
		EnemyDirX = 0;
		EnemyDirY = 1;
		PastEnemyDirX = 0;
		PastEnemyDirY = 1;
		DamageCounter = 0;
		ExistFlag = false;
		DamageFlag = false;
	}
	Enemy(int X, int Y, int LifePoint, int SpeedX, int SpeedY) :Charactor(X, Y, LifePoint, SpeedX, SpeedY) {
		EnemyDirX = 0;
		EnemyDirY = 1;
		PastEnemyDirX = 0;
		PastEnemyDirY = 1;
		DamageCounter = 0;
		ExistFlag = false;
		DamageFlag = false;
	}
	Enemy(int X, int Y, int LifePoint, int SpeedX, int SpeedY, int EnemyDirX, int EnemyDirY) :Charactor(X, Y, LifePoint, SpeedX, SpeedY) {
		this->EnemyDirX = EnemyDirX;
		this->EnemyDirY = EnemyDirY;
		PastEnemyDirX = EnemyDirX;
		PastEnemyDirY = EnemyDirY;
		DamageCounter = 0;
		ExistFlag = false;
		DamageFlag = false;
	}
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	// �c�w���C�u����������
	if (DxLib_Init() == -1) return -1;

	// ��ʃ��[�h�̐ݒ�
	SetGraphMode(640, 480, 16);

	//0...�^�C�g�����
	//1...�Q�[�����
	//2...�Q�[���I�[�o�[���
	//3...�X�e�[�W�J�ډ��


	// �O���t�B�b�N�̕`���𗠉�ʂɃZ�b�g
	SetDrawScreen(DX_SCREEN_BACK);

	unsigned int Cr, Cr2, Cr3;
	// ���F�̒l���擾
	Cr = GetColor(255, 255, 255);//��
	Cr2 = GetColor(255, 0, 255);//��
	Cr3 = GetColor(255, 0, 0);//��

	int ModeSelect = 0;

	Player player;
	Enemy enemy[ENEMYCOU];

	//int player.X = 200, player.Y = 400;
	//int EnemyX[ENEMYCOU], EnemyY[ENEMYCOU];
	//int player.PlayerDirX = 0,player.PlayerDirY = -1;
	//int Pastplayer.PlayerDirX = 0, Pastplayer.PlayerDirY = -1; //���@�̌����𐔎��ŊǗ�
	int GraphStage;//�X�e�[�W�̔��摜
	int GraphTitle;//�^�C�g����ʂ̔w�i
	int StageChangeCount=0;

	//int SpeedX, SpeedY;
	//int LifePoint = 3;
	int GraphLife;
	int GraphPlayer, GraphEnemy,GraphEnemyDamage,GraphL, GraphR, GraphUnder, GraphLUp, GraphLUnder, GraphRUp, GraphRUnder;
	int GraphShot;

	//int ShotX[SHOTCOU], ShotY[SHOTCOU], ShotFlag[SHOTCOU] = {};
	//int ShotBFlag = 0, ShotSpeed = 16, ShotSx[SHOTCOU], ShotSy[SHOTCOU];
	
	int ScoreCount = 0, StageCount = 0;
	int ShotHandle, ShotHitHandle, EShotHandle, GEHandle;

	//int EnemyDamageFlag[ENEMYCOU] = {}, EnemyDamageCounter[ENEMYCOU];
	//int EnemySpeedX[ENEMYCOU] = {}, EnemySpeedY[ENEMYCOU] = {}, EnemyFlag[ENEMYCOU] = {};

	//double EShotX[ENEMYCOU], EShotY[ENEMYCOU];
	//double EShotSx[ENEMYCOU], EShotSy[ENEMYCOU];
	//int EShotFlag[ENEMYCOU] = {};

	int EShotW, EShotH, GraphEnemyShot;
	//int EShotCounter[ENEMYCOU] = {};
	int EShotSpeed = 8;
	bool NoEnemy = false;

	int FontHandle;
	// �@�f�t�H���g�̃t�H���g�ŁA�T�C�Y60�A����3�̃t�H���g���쐬��
	// �쐬�����f�[�^�̎��ʔԍ���ϐ� FontHandle �ɕۑ�����
	FontHandle = CreateFontToHandle(NULL, 60, 3);


	// �O���t�B�b�N���������Ƀ��[�h
	GraphPlayer = LoadGraph("Ball.png");
	GraphEnemy = LoadGraph("Sikaku.png");
	GraphShot = LoadGraph("Shot.png");
	GraphEnemyDamage = LoadGraph("SikakuDam.png");
	GraphEnemyShot = LoadGraph("EShot.png");
	GraphLife = LoadGraph("heart2.png");
	GraphL = LoadGraph("Ball-Left.jpg");
	GraphR = LoadGraph("Ball-Right.jpg");
	GraphUnder = LoadGraph("Ball-Under.jpg");
	GraphLUp = LoadGraph("Ball-L-Up.jpg");
	GraphLUnder = LoadGraph("Ball-L-Under.jpg");
	GraphRUp = LoadGraph("Ball-R-Up.jpg");
	GraphRUnder = LoadGraph("Ball-R-Under.jpg");
	GraphStage = LoadGraph("Stage.jpg");
	GraphTitle = LoadGraph("Title.jpg");

	// �����������Ƀ��[�h
	ShotHandle = LoadSoundMem("shot1.mp3");
	ShotHitHandle = LoadSoundMem("shot-struck1.mp3");
	EShotHandle = LoadSoundMem("damage.mp3");
	GEHandle = LoadSoundMem("gameover.mp3");

	int EnemyW, EnemyH, ShotW, ShotH;
	GetGraphSize(GraphEnemy, &EnemyW, &EnemyH);
	// �G�̒e�̃O���t�B�b�N�̃T�C�Y�𓾂�
	GetGraphSize(GraphEnemyShot, &EShotW, &EShotH);
	int PlayerW, PlayerH;
	// �{�[���N�ƒe�̉摜�̃T�C�Y�𓾂�
	GetGraphSize(GraphPlayer, &PlayerW, &PlayerH);
	GetGraphSize(GraphShot, &ShotW, &ShotH);
	int StageW, StageH;
	//�X�e�[�W�̑傫���擾
	GetGraphSize(GraphStage, &StageW, &StageH);
	//SpeedX = 3;
	//SpeedY = 0;



	//�Q�[�����[�v
	while (1) {

		ClearDrawScreen();
		if (ModeSelect == 0) {
			//DrawString(270, 220, "Sample1", Cr2);
			//SetFontSize(60);
			DrawGraph(0, 0, GraphTitle, TRUE);
			DrawStringToHandle(200, 140, "Sample1", Cr2, FontHandle);
			DrawString(240, 240, "ENTER to Start", Cr);
			DrawString(255, 260, "ESC to End", Cr);
			//DrawGraph(0, 0, GraphTitle, FALSE);
			if (CheckHitKey(KEY_INPUT_RETURN) == 1) {
				ModeSelect = 3;
			}
			//EnemyX[0] = 400, EnemyY[0] = 50, EnemyFlag[0] = 1,EnemySpeedX[0]=3, EnemySpeedY[0] = 0;
		}
		else if (ModeSelect == 1) {
			DrawGraph(120, 0, GraphStage, FALSE);

			player.PastPlayerDirX = player.PlayerDirX;
			player.PastPlayerDirY = player.PlayerDirY;
			player.PlayerDirX = 0;
			player.PlayerDirY = 0;

			// �㉺���E�̃L�[���͂ɑΉ����� Player �� x, y �̍��W�l�ƌ�����ύX����
			if (CheckHitKey(KEY_INPUT_LEFT) == 1) { player.X -= 4; player.PlayerDirX -= 1; }
			if (CheckHitKey(KEY_INPUT_RIGHT) == 1) { player.X += 4; player.PlayerDirX += 1; }
			if (CheckHitKey(KEY_INPUT_UP) == 1) { player.Y -= 4; player.PlayerDirY -= 1; }
			if (CheckHitKey(KEY_INPUT_DOWN) == 1) { player.Y += 4; player.PlayerDirY += 1; }

			if (player.PlayerDirX == 0 && player.PlayerDirY == 0) {
				player.PlayerDirX = player.PastPlayerDirX;
				player.PlayerDirY = player.PastPlayerDirY;
			}

			//���@�̈ړ�����
			if (player.X < 120) player.X = 120;
			if (player.Y < 0) player.Y = 0;
			if (player.X > 120 + StageW - 64) player.X = 120 + StageW - 64;
			if (player.Y > 480 - 64) player.Y = 480 - 64;

			// ��ʂɎ��@�̊G��\��
			if (player.PlayerDirX == 0 && player.PlayerDirY == -1) {
				DrawGraph(player.X, player.Y, GraphPlayer, FALSE);
			}
			else if (player.PlayerDirX == -1 && player.PlayerDirY == -1) {
				DrawGraph(player.X, player.Y, GraphLUp, FALSE);
			}
			else if (player.PlayerDirX == 1 && player.PlayerDirY == -1) {
				DrawGraph(player.X, player.Y, GraphRUp, FALSE);
			}
			else if (player.PlayerDirX == -1 && player.PlayerDirY == 0) {
				DrawGraph(player.X, player.Y, GraphL, FALSE);
			}
			else if (player.PlayerDirX == 1 && player.PlayerDirY == 0) {
				DrawGraph(player.X, player.Y, GraphR, FALSE);
			}
			else if (player.PlayerDirX == -1 && player.PlayerDirY == 1) {
				DrawGraph(player.X, player.Y, GraphLUnder, FALSE);
			}
			else if (player.PlayerDirX == 0 && player.PlayerDirY == 1) {
				DrawGraph(player.X, player.Y, GraphUnder, FALSE);
			}
			else if (player.PlayerDirX == 1 && player.PlayerDirY == 1) {
				DrawGraph(player.X, player.Y, GraphRUnder, FALSE);
			}

			player.ShotCount++;

			//�e���ˏ���
			if (CheckHitKey(KEY_INPUT_SPACE) == 1) {
				if (player.ShotCount >= 10) {

					for (int i = 0; i < SHOTCOU; i++) {

						if (player.shot[i].ShotFlag == false) {

							// �e�̈ʒu���Z�b�g�A�ʒu�̓{�[���N�̒��S�ɂ���
							player.shot[i].ShotSpeed = 16;
							player.shot[i].ShotX = (PlayerW - ShotW) / 2 + player.X;
							player.shot[i].ShotY = (PlayerH - ShotH) / 2 + player.Y;

							// �e�̈ړ����x��ݒ肷��
							double sb, sbx, sby, bx, by, sx, sy;
							sx = double(player.shot[i].ShotX + ShotW) / 2.0;
							sy = double(player.shot[i].ShotY + ShotH) / 2.0;
							bx = sx + player.PlayerDirX;
							by = sy + player.PlayerDirY;
							sbx = bx - sx;
							sby = by - sy;
							sb = sqrt(sbx*sbx + sby * sby);
							player.shot[i].ShotSpeedX = (int)(sbx / sb * player.shot[i].ShotSpeed);
							player.shot[i].ShotSpeedY = (int)(sby / sb * player.shot[i].ShotSpeed);

							player.shot[i].ShotFlag = true;

							// �ǂ݂��񂾉����o�b�N�O���E���h�Đ����܂�
							PlaySoundMem(ShotHandle, DX_PLAYTYPE_BACK);
							break;

						}
					}
					//�A�Ŗh�~�J�E���g�߂�
					player.ShotCount = 0;
				}

			}
			/*
			else {
				//�A�Ŗh�~�t���OOFF
				ShotBFlag = 0;
			}
			*/

			// ���@�̒e�̈ړ����[�`��( ���ݏ�Ԃ�ێ����Ă���ϐ��̓��e���P(���݂���)�̏ꍇ�̂ݍs�� )
			for (int i = 0; i < SHOTCOU; i++) {
				if (player.shot[i].ShotFlag == true)
				{

					// �e���ړ�������
					player.shot[i].ShotX += player.shot[i].ShotSpeedX;
					player.shot[i].ShotY += player.shot[i].ShotSpeedY;


					// ��ʊO�ɏo�Ă��܂����ꍇ�͑��ݏ�Ԃ�ێ����Ă���ϐ��ɂO(���݂��Ȃ�)��������
					if (player.shot[i].ShotY > 480 || player.shot[i].ShotY < 0 || player.shot[i].ShotX > 120+StageW || player.shot[i].ShotX < 120)
					{
						player.shot[i].ShotFlag = false;
					}

					// ��ʂɒe��`�悷��
					DrawGraph(player.shot[i].ShotX, player.shot[i].ShotY, GraphShot, FALSE);
				}

			}
		

			/*
			if (CheckHitKey(KEY_INPUT_A) == 1) {
				for (int i = 0; i < ENEMYCOU; i++) {
					if (enemy[i].ExistFlag == 0) {
						enemy[i].ExistFlag = 1;
						enemy[i].X = rand() % 576;
						enemy[i].Y = 50;
						enemy[i].SetSpeed(rand() % 10 + 2, 0);
						break;
					}
				}
			}*/

			// �G�̈ړ����[�`��
			for (int i = 0; i < ENEMYCOU; i++) {
				if (enemy[i].ExistFlag == 1) {
					// ���c�߂Ă��邩�ǂ����ŏ����𕪊�
					if (enemy[i].DamageFlag == 1) {
						DrawGraph(enemy[i].X, enemy[i].Y, GraphEnemyDamage, FALSE);
						// ���c�߂Ă��鎞�Ԃ𑪂�J�E���^�[�ɂP�����Z����
						enemy[i].DamageCounter++;
						// �������c�ߏ��߂� 5 �t���[���o�߂��Ă������̘c�񂾏�Ԃ���
						// ����
						if (enemy[i].DamageCounter >= 5) {
							enemy[i].DamageFlag = 0;
							//EnemySpeed[i] = rand() % 10 + ScoreCount;
							//EnemyX[i] = rand() % 576;
							//SpeedX = EnemySpeed[i];
							//EShotSpeed = rand() % 4 + 8;
							enemy[i].ExistFlag = 0;
						}
					}
					else {
						if (enemy[i].X < 120 || enemy[i].X > 120 + StageW - 64) enemy[i].SetSpeed(enemy[i].ReturnSpeedX()*(-1), enemy[i].ReturnSpeedY());
						if (enemy[i].Y < 0 || enemy[i].Y > 480 - 64) enemy[i].SetSpeed(enemy[i].ReturnSpeedX(), enemy[i].ReturnSpeedY()*(-1));

						enemy[i].X += enemy[i].ReturnSpeedX();
						enemy[i].Y += enemy[i].ReturnSpeedY();
						DrawGraph(enemy[i].X, enemy[i].Y, GraphEnemy, FALSE);

						enemy[i].ShotCount++;
						if (enemy[i].ShotCount >= 60) {
							for (int j = 0; j < SHOTCOU; j++) {
								if (enemy[i].shot[j].ShotFlag == false) {
									// �e�̈ʒu���Z�b�g�A�ʒu�̓{�[���N�̒��S�ɂ���
									enemy[i].shot[j].ShotX = (EnemyW - EShotW) / 2 + enemy[i].X;
									enemy[i].shot[j].ShotY = (EnemyH - EShotH) / 2 + enemy[i].Y;
									enemy[i].shot[j].ShotFlag = 1;
									// �e�̈ړ����x��ݒ肷��
									double sb, sbx, sby, bx, by, sx, sy;
									sx = (enemy[i].shot[j].ShotX + EShotW) / 2.0;
									sy = (enemy[i].shot[j].ShotY + EShotH) / 2.0;
									bx = (player.X + PlayerW) / 2.0;
									by = (player.Y + PlayerH) / 2.0;
									sbx = bx - sx;
									sby = by - sy;
									sb = sqrt(sbx*sbx + sby * sby);
									enemy[i].shot[j].ShotSpeedX = int(sbx / sb * EShotSpeed);
									enemy[i].shot[j].ShotSpeedY = int(sby / sb * EShotSpeed);
									break;
								}
								
							}
							enemy[i].ShotCount = 0;
						}

					}
				}
			}

			// �G�̒e�̈ړ����[�`��( ���ݏ�Ԃ�ێ����Ă���ϐ��̓��e���P(���݂���)�̏ꍇ�̂ݍs�� )
			for (int i = 0; i < ENEMYCOU; i++) {
				for (int j = 0; j < SHOTCOU; j++) {
					if (enemy[i].shot[j].ShotFlag == 1)
					{
						enemy[i].shot[j].ShotX += enemy[i].shot[j].ShotSpeedX;
						enemy[i].shot[j].ShotY += enemy[i].shot[j].ShotSpeedY;

						// ��ʊO�ɏo�Ă��܂����ꍇ�͑��ݏ�Ԃ�ێ����Ă���ϐ��ɂO(���݂��Ȃ�)��������
						if (enemy[i].shot[j].ShotY > 480 || enemy[i].shot[j].ShotY < 0 || enemy[i].shot[j].ShotX > 120+StageW || enemy[i].shot[j].ShotX < 120)
						{
							enemy[i].shot[j].ShotFlag = false;
						}

						// ��ʂɒe��`�悷��
						DrawGraph(enemy[i].shot[j].ShotX, enemy[i].shot[j].ShotY, GraphEnemyShot, FALSE);
					}
				}
			}


			// �e�ƓG�̓����蔻��A�e�A�G�̐������J��Ԃ�
			for (int i = 0; i < SHOTCOU; i++) {
				if (player.shot[i].ShotFlag == true){
					for (int j = 0; j < ENEMYCOU; j++) {
						if (enemy[j].ExistFlag == true) {
							if (((player.shot[i].ShotX > enemy[j].X && player.shot[i].ShotX < enemy[j].X + EnemyW) ||
								(player.shot[i].ShotX + ShotW > enemy[j].X && player.shot[i].ShotX + ShotW < enemy[j].X + EnemyW)) &&
								((player.shot[i].ShotY > enemy[j].Y && player.shot[i].ShotY < enemy[j].Y + EnemyH) ||
								(player.shot[i].ShotY + ShotH > enemy[j].Y && player.shot[i].ShotY + ShotH < enemy[j].Y + EnemyH))) {
								// �ڐG���Ă���ꍇ�͓��������e�̑��݂�����
								player.shot[i].ShotFlag = 0;
								enemy[j].DamageCounter = 0;
								enemy[j].DamageFlag = true;
								ScoreCount++;
								// �ǂ݂��񂾉����o�b�N�O���E���h�Đ����܂�
								PlaySoundMem(ShotHitHandle, DX_PLAYTYPE_BACK);
								break;
							}
						}
					}
				}
			}
			// �G�̒e�̓����蔻��
			for (int i = 0; i < ENEMYCOU; i++) {
				for (int j = 0; j < SHOTCOU; j++) {
					if (enemy[i].shot[j].ShotFlag == true)
					{
						if (((enemy[i].shot[j].ShotX > player.X && enemy[i].shot[j].ShotX < player.X + PlayerW) ||
							(enemy[i].shot[j].ShotX + EShotW > player.X && enemy[i].shot[j].ShotX + EShotW < player.X + PlayerW)) &&
							((enemy[i].shot[j].ShotY > player.Y && enemy[i].shot[j].ShotY < player.Y + PlayerH) ||
							(enemy[i].shot[j].ShotY + EShotH > player.Y && enemy[i].shot[j].ShotY + EShotH < player.Y + PlayerH))) {
							// �ڐG���Ă���ꍇ�͓��������e�̑��݂�����
							enemy[i].shot[j].ShotFlag = false;
							player.ChangeLife(-1);
							// �ǂ݂��񂾉����o�b�N�O���E���h�Đ����܂�
							PlaySoundMem(EShotHandle, DX_PLAYTYPE_BACK);
						}
					}
				}
			}

			//�G�̑��ݗL������A���Ȃ���΃X�e�[�W�J�ڂ�
			NoEnemy = true;
			for (int i = 0; i < ENEMYCOU; i++) {
				if (enemy[i].ExistFlag == true) {
					NoEnemy = false;
					break;
				}
			}
			if (NoEnemy == true) {
				ModeSelect = 3;
			}

			SetFontSize(20);
			//�X�R�A�\��
			std::string str;
			//str = std::to_string(ScoreCount);
			//str = "Score " + str;
			str = std::to_string(StageCount);
			str = "Stage " + str;
			DrawString(500, 0, str.c_str(), Cr2);
			//���C�t�\��
			DrawString(100, 2, "Life", Cr2);


			for (int i = 0; i < player.ReturnLife(); i++) {
				// ��ʂɃ��C�t�̊G��`�悷��
				DrawGraph(150 + i * 30, 0, GraphLife, FALSE);
			}
			if (player.ReturnLife() <= 0) {
				ModeSelect = 2;
				WaitTimer(50);
				PlaySoundMem(GEHandle, DX_PLAYTYPE_BACK);
			}

		}
		else if (ModeSelect == 2) {

			//�Q�[���I�[�o�[�\��
			SetFontSize(20);
			std::string str;
			//str = std::to_string(ScoreCount);
			//str = "Final Score " + str;
			//DrawString(295, 220, str.c_str(), Cr);
			str = std::to_string(StageCount);
			str = "Final Stage... " + str;
			DrawString(235, 220, str.c_str(), Cr);
			DrawStringToHandle(200, 140, "GameOver", Cr3, FontHandle);
			//DrawString(290, 200, "GameOver", Cr3);
			DrawString(230, 240, "ENTER to Continue", Cr);
			DrawString(210, 260, "R to back TitleScene", Cr);
			//�R���e�B�j���[����
			if (CheckHitKey(KEY_INPUT_RETURN) == 1) {
				player.ChangeLife(3);
				ScoreCount = 0;
				ModeSelect = 3;
				StageCount = 0;
				player.X = 200;
				//EnemyX[0] = 400;
				player.Y = 400;
				//EnemyY[0] = 50;
				//EnemyFlag[0] = 1;
			}
			if (CheckHitKey(KEY_INPUT_R) == 1) {
				player.ChangeLife(3);
				ScoreCount = 0;
				ModeSelect = 0;
				StageCount = 0;
			}

		}
		else if (ModeSelect == 3) {

			//�X�e�[�W�J�ډ��
			if (StageCount < 2) {
				std::string str;
				str = std::to_string(StageCount + 1);
				str = "Stage " + str + " START!!";
				SetFontSize(50);
				DrawString(120, 200, str.c_str(), Cr);
				StageChangeCount++;
				if (StageChangeCount > 60) {
					ModeSelect = 1;
					StageChangeCount = 0;
					if (player.ReturnLife() < 3) player.ChangeLife(1);
					StageCount++;
					player.X = 200;
					player.Y = 400;
					player.ShotReset();
					for (int i = 0; i < StageCount; i++) {
						enemy[i].ExistFlag = true;
						enemy[i].X = rand() % 300 + 120;
						enemy[i].Y = 50;
						enemy[i].SetSpeed(rand() % 3 + 2, 0);
						enemy[i].ShotReset();
					}

				}
			}
			else {
				//SetFontSize(50);
				DrawStringToHandle(175, 200, "GameCrear!!", Cr2, FontHandle);
				DrawString(230, 260, "ENTER to Continue", Cr);
				DrawString(210, 280, "R to back TitleScene", Cr);
				if (CheckHitKey(KEY_INPUT_RETURN) == 1) {
					ModeSelect = 0;
				}
				if (CheckHitKey(KEY_INPUT_R) == 1) {
					player.ChangeLife(3);
					ScoreCount = 0;
					ModeSelect = 0;
					StageCount = 0;
				}
			}

		}

		ScreenFlip();

		WaitTimer(30);



		// Windows �V�X�e�����炭�������������
		if (ProcessMessage() == -1) break;

		// �d�r�b�L�[�������ꂽ�烋�[�v���甲����
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) break;

	}

	// �L�[�̓��͑҂�
	//WaitKey();


	// �c�w���C�u�����g�p�̏I��
	DxLib_End();
	// �\�t�g�̏I��
	return 0;
}

