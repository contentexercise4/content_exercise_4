#pragma once
#ifndef MUSIC_CONTROLLER_H
#define MUSIC_CONTROLLER_H

#include "alut.h"


int sound_check;			//BGMの種類を決める
bool se_pig_check;			//ブタの鳴き声の1回目を制御
//int tmp = 0;

//---volume---//
float volume_BGM = 1.0f;		//BGMの音量
float volume_PIG = 1.0f;		//ブタの鳴き声の音量
float volume_WALL = 1.0f;		//壁に衝突したときの音量
float volume_GetItem = 1.0f;	//アイテムを取得したときの音量
float volume_gameover = 1.0f;	//ブタに当たってゲームオーバーになるときの音量
float volume_gameclear = 1.0f;	//りんごを4つ取ってゲームクリアになるときの音量
float volume_timeup = 1.0f;		//制限時間切れになったときの音量

//---SE---//
ALuint seCollisionWall;		//壁に衝突するSE
ALuint seCollisionEnemy;	//敵に衝突するSE
ALuint seGetItem;			//アイテムを取得するSE
ALuint sePig;				//ブタの鳴き声のSE
ALuint seGameover;			//ブタに当たってゲームオーバーになるときのSE
ALuint seGameclear;			//りんごを4つ取ってゲームクリアになるときのSE
ALuint seTimeup;			//制限時間切れになったときのSE

//---BGM---//
ALuint bgmStart;			//スタート画面のBGM
ALuint bgmPlay;				//プレイ画面のBGM
ALuint bgmResult;			//リザルト画面のBGM

//---souce SE---//
ALuint sseCollisionWall;	//壁に衝突するsSE
ALuint sseCollisionEnemy;	//敵に衝突するsSE
ALuint sseGetItem;			//アイテムを取得するsSE
ALuint ssePig;				//ブタの鳴き声のsSe
ALuint sseGameover;			//ブタに当たってゲームオーバーになるときのsSE
ALuint sseGameclear;		//りんごを4つ取ってゲームクリアになるときのsSe
ALuint sseTimeup;			//制限時間切れになったときのsSe

//---souce BGM---//
ALuint sbgmStart;			//スタート画面のsBGM
ALuint sbgmPlay;			//プレイ画面のsBGM
ALuint sbgmResult;			//リザルト画面のsBGM

int countCollisionWall = 0;		//壁に衝突時のSEを連座奥再生しないように制御する
int initCountCollision = 300;	//countがこの値を超えたら0に初期化する
double disFromPig = 0;			//ブタと自分との距離


//-----------------------------
// 関数のプロトタイプ宣言
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
// 過去に参考にしたURL. 現在はOpenALが使用可能になったので必要ない
//--------------------------------------------------------------------
//https://bituse.info/winapi/35
//https://docs.microsoft.com/ja-jp/windows/desktop/Multimedia/mci-open
//http://wisdom.sakura.ne.jp/system/winapi/media/mm3.html
