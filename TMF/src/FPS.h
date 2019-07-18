#pragma once
#pragma comment(lib, "winmm.lib")

#include <Windows.h>
#include <string>
#include <mmsystem.h>

using namespace std;

#define CPUMax 30000
#define CPUMin 10000

//FPS管理クラス
class FPS {
public:
	//ループカウンタ,現在のフレーム,スリープ値,現在のフレームスキップ数,一秒おきのフレームスキップ数,一秒おきのループカウンタ,現在のFPS値
	int LoopCount, Frame, sleep, Skip, FrameSkip, Fps, LoopMax, CurrentFps;
	bool draw;
	DWORD Time, Oldtime;//現在の時間,前回の時間
	float FrameTime, FrameTimeO;
	FPS(int);
	void SetFPS(int);
	void GetFPS();
private:
	string str;
	char buf[255];
};
//コンストラクタ
FPS::FPS(int fps) {
	draw = false;
	LoopCount = 0;
	Frame = 0;
	sleep = 15;
	Skip = 0;
	Fps = fps;
	FrameSkip = 0;
	LoopMax = 0;
	CurrentFps = 0;
	FrameTime = 1000.0f / fps - 0.000001f;//1フレームの時間
	FrameTimeO = 1000.0f / fps + 0.000001f;
}
void FPS::SetFPS(int fps) {
	Fps = fps;
	FrameTime = 1000.0f / fps - 0.000001f;//1フレームの時間
	FrameTimeO = 1000.0f / fps + 0.000001f;
}
void FPS::GetFPS()
{
	draw = false;
	LoopCount++;
	Time = timeGetTime();
	if (Time - Oldtime>Frame*FrameTime) {
		Frame++;
		if (Time - Oldtime<Frame*FrameTimeO) {//描画タイミングに間に合った場合

			//str = "LOOP_MAX:["; str += _ltoa(LoopMax, buf, 10); str += "] FPS:["; str += _ltoa(CurrentFps - FrameSkip, buf, 10);
			//str += "] FRAME:["; str += _ltoa(Frame, buf, 10); str += "] FRAMESKIP:["; str += _ltoa(FrameSkip, buf, 10);
			//str += "] SLEEP:["; str += _ltoa(sleep, buf, 10); str += "]";

			str = "LOOP_MAX:["; str += _ltoa_s(LoopMax, buf, 10); str += "] FPS:["; str += _ltoa_s(CurrentFps - FrameSkip, buf, 10);
			str += "] FRAME:["; str += _ltoa_s(Frame, buf, 10); str += "] FRAMESKIP:["; str += _ltoa_s(FrameSkip, buf, 10);
			str += "] SLEEP:["; str += _ltoa_s(sleep, buf, 10); str += "]";

			glutSetWindowTitle(str.c_str());
			draw = true;
			Sleep(sleep);
		}
		else {//描画タイミングに間に合わなかった場合：フレームスキップ
			if (sleep>1)sleep--;//ウェイトを減らす
			Skip++;
		}
	}
	if (Time - Oldtime>1000 - FrameTime) {//一秒おきの処理
		LoopMax = LoopCount;
		if (LoopMax>CPUMax) { if (sleep<15)sleep++; }//十分に余力がある場合ウェイトを増やす
		if (LoopMax<CPUMin) { if (sleep>1)sleep--; }//余裕が無い場合ウェイトを減らす
		CurrentFps = Frame;
		FrameSkip = Skip;
		Skip = 0;
		LoopCount = 0;
		Frame = 0;
		Oldtime = Time;
	}
}