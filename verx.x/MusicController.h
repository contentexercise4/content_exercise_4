#pragma once
#ifndef MUSIC_CONTROLLER_H
#define MUSIC_CONTROLLER_H

#include "alut.h"


int sound_check;			//BGM�̎�ނ����߂�
bool se_pig_check;			//�u�^�̖�����1��ڂ𐧌�
//int tmp = 0;

//---volume---//
float volume_BGM = 1.0f;		//BGM�̉���
float volume_PIG = 1.0f;		//�u�^�̖����̉���
float volume_WALL = 1.0f;		//�ǂɏՓ˂����Ƃ��̉���
float volume_GetItem = 1.0f;	//�A�C�e�����擾�����Ƃ��̉���
float volume_gameover = 1.0f;	//�u�^�ɓ������ăQ�[���I�[�o�[�ɂȂ�Ƃ��̉���
float volume_gameclear = 1.0f;	//��񂲂�4����ăQ�[���N���A�ɂȂ�Ƃ��̉���
float volume_timeup = 1.0f;		//�������Ԑ؂�ɂȂ����Ƃ��̉���

//---SE---//
ALuint seCollisionWall;		//�ǂɏՓ˂���SE
ALuint seCollisionEnemy;	//�G�ɏՓ˂���SE
ALuint seGetItem;			//�A�C�e�����擾����SE
ALuint sePig;				//�u�^�̖�����SE
ALuint seGameover;			//�u�^�ɓ������ăQ�[���I�[�o�[�ɂȂ�Ƃ���SE
ALuint seGameclear;			//��񂲂�4����ăQ�[���N���A�ɂȂ�Ƃ���SE
ALuint seTimeup;			//�������Ԑ؂�ɂȂ����Ƃ���SE

//---BGM---//
ALuint bgmStart;			//�X�^�[�g��ʂ�BGM
ALuint bgmPlay;				//�v���C��ʂ�BGM
ALuint bgmResult;			//���U���g��ʂ�BGM

//---souce SE---//
ALuint sseCollisionWall;	//�ǂɏՓ˂���sSE
ALuint sseCollisionEnemy;	//�G�ɏՓ˂���sSE
ALuint sseGetItem;			//�A�C�e�����擾����sSE
ALuint ssePig;				//�u�^�̖�����sSe
ALuint sseGameover;			//�u�^�ɓ������ăQ�[���I�[�o�[�ɂȂ�Ƃ���sSE
ALuint sseGameclear;		//��񂲂�4����ăQ�[���N���A�ɂȂ�Ƃ���sSe
ALuint sseTimeup;			//�������Ԑ؂�ɂȂ����Ƃ���sSe

//---souce BGM---//
ALuint sbgmStart;			//�X�^�[�g��ʂ�sBGM
ALuint sbgmPlay;			//�v���C��ʂ�sBGM
ALuint sbgmResult;			//���U���g��ʂ�sBGM

int countCollisionWall = 0;		//�ǂɏՓˎ���SE��A�����Đ����Ȃ��悤�ɐ��䂷��
int initCountCollision = 300;	//count�����̒l�𒴂�����0�ɏ���������
double disFromPig = 0;			//�u�^�Ǝ����Ƃ̋���


//-----------------------------
// �֐��̃v���g�^�C�v�錾
//-----------------------------
void InitMusic();
void soundPlay(ALuint buf, ALuint *source, bool loop, float volume);
void soundStop(ALuint *source);
bool chackDoubleSpecialKeyPush();
void decideVolumeOfPigCry(double disFromPig);
float getDuration(ALuint buffer);
float getOffset(ALuint source);
void allSoundStop();
void allSeStop();

#endif

//--------------------------------------------------------------------
// �ߋ��ɎQ�l�ɂ���URL. ���݂�OpenAL���g�p�\�ɂȂ����̂ŕK�v�Ȃ�
//--------------------------------------------------------------------
//https://bituse.info/winapi/35
//https://docs.microsoft.com/ja-jp/windows/desktop/Multimedia/mci-open
//http://wisdom.sakura.ne.jp/system/winapi/media/mm3.html
