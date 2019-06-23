#pragma once
#ifndef MUSIC_CONTROLLER_H
#define MUSIC_CONTROLLER_H

#define MSG(m) { MessageBoxA(NULL,m,NULL,MB_OK); }
#include "alut.h"

HWND hwnd;			//ウィンドウハンドル
HINSTANCE hinst;	//インスタンスハンドル
#define WIDTH 500
#define HEIGHT 300

static MCI_OPEN_PARMS open1, open2, open3;
static MCI_PLAY_PARMS play1, play2, play3;
int sound_check;

enum BGM_IDX {				//BGMの大文字ファイル名を数字対応させた
	MAIN1   = 0,
	START1  = 1,
	ALL     = 2,
	MAIN2   = 3,

};

enum SE_IDX {				//SEの大文字ファイル名を数字対応させた
	COLLISION = 0,
	GET_ITEM  = 1,
};

enum COMMAND {				//playMusic() の2つ目の引数
	OPEN = 0,
	PLAY_INSTANCE = 1,
	PLAY_LOOP = 2,
	STOP = 3,
	PAUSE = 4,
	RESUME = 5,
	CLOSE = 6,
};

//---SE---//
ALuint seCollisionWall;		//壁に衝突するSE
ALuint seCollisionEnemy;	//敵に衝突するSE
ALuint seGetItem;			//アイテムを取得するSE

//---BGM---//
ALuint bgmStart;			//スタート画面のBGM
ALuint bgmPlay;				//プレイ画面のBGM

//---souce SE---//
ALuint sseCollisionWall;	//壁に衝突するsSE
ALuint sseCollisionEnemy;	//敵に衝突するsSE
ALuint sseGetItem;			//アイテムを取得するsSE

//---souce BGM---//
ALuint sbgmStart;			//スタート画面のsBGM
ALuint sbgmPlay;			//プレイ画面のsBGM

int countCollisionWall = 0;	//壁に衝突時のSEを連座奥再生しないように制御する
int initCountCollision = 300;	//countがこの値を超えたら0に初期化する


//void musicController(const char* filename, const char* command);
void playMusic(int file_idx, int commnad);	//BGM, SEの再生を直感的に行うための関数
void InitMusic();							//SEの初期化
void closeMusic();							//すべてのmusicを閉じる
void soundPlay(ALuint buf, ALuint *source, bool loop);
void soundStop(ALuint *source);
bool chackDoubleSpecialKeyPush();


//https://bituse.info/winapi/35
//https://docs.microsoft.com/ja-jp/windows/desktop/Multimedia/mci-open
//http://wisdom.sakura.ne.jp/system/winapi/media/mm3.html


/*

SND_APPLICATION		: アプリケーション特有の関連付けを使ってサウンドを再生します。
SND_ALIAS           : pszSound パラメータは、レジストリまたは WIN.INI ファイルに記述されているシステムイベントの別名（エイリアス）です。SND_FILENAME や SND_RESOURCE と同時に指定することはできません。
SND_ALIAS_ID        : pszSound パラメータは、定義済みのサウンド識別子（"SystemStart"、"SystemExit" など）です。
SND_ASYNC			: サウンドを非同期再生し、サウンドが開始されると、PlaySound 関数は即座に制御を返します。非同期再生されているサウンドを停止するには、pszSound パラメータで NULL を指定して PlaySound 関数を呼び出してください。
SND_FILENAME        : pszSound パラメータは、ファイル名を表します。
SND_LOOP            : サウンドを繰り返し再生します。pszSound パラメータで NULL を指定して PlaySound 関数を呼び出すと、サウンドが停止します。サウンドイベントを非同期再生するよう指示するために、SND_ASYNC と同時に指定しなければなりません。
SND_MEMORY			: サウンドイベントのファイルは、メモリ内に既にロードされています。pszSound パラメータは、メモリ内のサウンドイメージへのポインタを表します。
SND_NODEFAULT		: 既定のサウンドイベントを使いません。指定されたサウンドが見つからなかった場合、PlaySound 関数は、既定のサウンド（一般の警告音）を再生せずに静かに制御を返します。
SND_NOSTOP			: 既にほかのサウンドが再生されている場合、指定されたサウンドを再生しません。指定されたサウンドを再生するために必要なリソースが、ほかのサウンドを再生していてビジーであり、指定されたサウンドを再生できない場合、この関数は指定されたサウンドを再生せずに、即座に FALSE を返します。
						このフラグを指定しなかった場合、PlaySound 関数は現在再生されているサウンドを停止して、指定された新しいサウンドを再生しようとします。
SND_NOWAIT			: ドライバがビジー状態の場合、指定されたサウンドを再生せずに即座に制御を返します。
SND_PURGE			: 呼び出し側タスクに関係するサウンドの再生を停止します。pszSound パラメータが NULL ではない場合、指定したサウンドのすべてのインスタンスを停止します。pszSound パラメータが NULL の場合、呼び出し側タスクに関係するすべてのサウンドを停止します。
						SND_RESOURCEパラメータで指定したサウンドイベントを停止させる場合は、hmod パラメータでインスタンスハンドルも指定しなければなりません。
SND_RESOURCE        : pszSound パラメータは、リソース識別子を表します。hmod パラメータで、リソースを保持している実行可能ファイルのインタンスハンドルを指定しなければなりません。
SND_SYNC            : サウンドイベントを同期再生します。PlaySound 関数は、サウンドの再生が完了した後で制御を返します。


MCI_OPEN       : デバイスをオープンします。
MCI_CLOSE      : デバイスをクローズします
MCI_PLAY       : 再生します
MCI_STOP       : 停止します
MCI_PAUSE      : 一時停止します
MCI_RESUME     : 一時停止解除します
MCI_SET        : MCI_FORMAT_MILLISECONDSを指定し、時間のフォーマットを指定します。
MCI_SEEK       : MCI_SEEK_TO_STARTなどを指定し、シーク位置の調整をします。
MCI_STATUS     : MCI_STATUS_PARMS.dwItemにMCI_STATUS_LENGTHを指定し、再生時間取得
MCI_STATUS     : MCI_STATUS_PARMS.dwItemにMCI_STATUS_POSITIONを指定し、現在位置を取得
MCI_STATUS     : MCI_STATUS_PARMS.dwItemにMCI_STATUS_MODEを指定すると以下のものを返す。
					MCI_MODE_PLAY  : 再生中
					MCI_MODE_STOP  : 停止中
					MCI_MODE_PAUSE : 一時停止中

MCIERROR mciSendCommand(	//wave, MP3の再生の同時再生が可能
	MCIDEVICEID IDDevice,	// デバイス識別子
	UINT uMsg,				// コマンドメッセージ
	DWORD fdwCommand,		// フラグ
	DWORD dwParam			// パラメータを保持している構造体
);

typedef struct {
	DWORD dwCallback;
	MCIDEVICEID wDeviceID;
	LPCSTR lpstrDeviceType;		//初期化必要 .wavファイルを再生するのなら MCI_DEVTYPE_WAVEFORM_AUDIO で初期化
	LPCSTR lpstrElementName;	//初期化必要 再生するファイル名
	LPCSTR lpstrAlias;
} MCI_OPEN_PARMS;

typedef struct {
	DWORD_PTR dwCallback;
	DWORD dwFrom;
	DWORD dwTo;
} MCI_PLAY_PARMS;

BOOL mciGetErrorString(
	DWORD fdwError,			// エラーコード
	LPTSTR lpszErrorText,	// バッファへのポインタ
	UINT cchErrorText		// バッファのサイズ
);

*/

#endif

//std::vector<std::string> command = { "open", "play_instance", "play_loop", "stop", "pause", "resume", "close" };
//musicController(filename1.c_str(), command[0].c_str());
//musicController(filename1.c_str(), command[1].c_str());


//void musicController(const char* filename, const char* command) {
//	std::cout << "\n[START musicContloller()]\n";
//	MCI_OPEN_PARMS open;
//	MCI_PLAY_PARMS play;
//	int result;
//	char buf[1024];
//	bool dbg = true;
//
//	//if (command == "open") {
//	open.lpstrDeviceType = (LPCWSTR)MCI_DEVTYPE_WAVEFORM_AUDIO;
//	open.lpstrElementName = (LPCWSTR)filename;
//	result = mciSendCommand(0, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_TYPE_ID | MCI_OPEN_ELEMENT, (DWORD_PTR)&open);
//	play.dwCallback = (DWORD)hwnd;
//	//}
//	if (command == "play_instance") {
//		result = mciSendCommand(open.wDeviceID, MCI_PLAY, MCI_NOTIFY, (DWORD_PTR)&play);
//	}
//	else if (command == "play_roop") {
//		result = mciSendCommand(open.wDeviceID, MCI_PLAY, MCI_NOTIFY, (DWORD_PTR)&play);
//	}
//	else if (command == "stop") {
//		result = mciSendCommand(open.wDeviceID, MCI_STOP, MCI_NOTIFY, (DWORD_PTR)&play);
//	}
//	else if (command == "pause") {
//		result = mciSendCommand(open.wDeviceID, MCI_PAUSE, MCI_NOTIFY, (DWORD_PTR)&play);
//	}
//	else if (command == "resume") {
//		result = mciSendCommand(open.wDeviceID, MCI_RESUME, MCI_NOTIFY, (DWORD_PTR)&play);
//	}
//	else if (command == "close") {
//		result = mciSendCommand(open.wDeviceID, MCI_CLOSE, 0, 0);
//	}
//
//	if (dbg) {
//		std::cout << "fileneme = " << filename << std::endl;
//		std::cout << "command  = " << command << std::endl;
//		//std::cout << "result   = " << result << std::endl;
//	}
//	if (false) {	//エラーならば0以外が返る
//		mciGetErrorString(result, (LPWSTR)buf, sizeof(buf));
//		MSG(buf);
//		PostQuitMessage(0);
//	}
//	else {
//		std::cout << "[SUCCESS] " << command << " " << filename << std::endl;
//		std::cout << std::endl;
//	}
//	std::cout << "[END musicController()]\n";
//
//}


/*
LRESULT CALLBACK WinProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp){
	static MCI_OPEN_PARMS open, open2, open3;
	static MCI_PLAY_PARMS play, play2, play3;

	int result;

	char buf[1000];

	open1.lpstrDeviceType = (LPWSTR)MCI_DEVTYPE_WAVEFORM_AUDIO;
	open1.lpstrElementName = (LPWSTR)"music_sample1.wav";
	open3.lpstrDeviceType = (LPWSTR)MCI_DEVTYPE_WAVEFORM_AUDIO;
	open3.lpstrElementName = (LPWSTR)"music_sample3.wav";

	result = mciSendCommand(0, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_TYPE_ID | MCI_OPEN_ELEMENT, (DWORD_PTR)&open);

	result = mciSendCommand(0, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_TYPE_ID | MCI_OPEN_ELEMENT, (DWORD_PTR)&open3);

	//エラーなら０以外が返る
	if (result) {
		//エラー取得
		mciGetErrorString(result, (LPWSTR)buf, sizeof(buf));

		MSG(buf);

		PostQuitMessage(0);

		return -1;
	}

	//mp3再生の場合
	open2.lpstrDeviceType = (LPWSTR)"MPEGVideo";
	open2.lpstrElementName = (LPWSTR)"music_sample2.mp3";
	result = mciSendCommand(0, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT,
		(DWORD_PTR)&open2);

	//エラーなら０以外が返る
	if (result) {
		//エラー取得
		mciGetErrorString(result, (LPWSTR)buf, sizeof(buf));
		MSG(buf);
		PostQuitMessage(0);
		return -1;
	}

	play.dwCallback = (DWORD)hwnd;
	play2.dwCallback = (DWORD)hwnd;
	play3.dwCallback = (DWORD)hwnd;


	switch (msg) {
		case WM_DESTROY:
			mciSendCommand(open.wDeviceID, MCI_CLOSE, 0, 0);
			mciSendCommand(open2.wDeviceID, MCI_CLOSE, 0, 0);
			mciSendCommand(open3.wDeviceID, MCI_CLOSE, 0, 0);

			PostQuitMessage(0);
			return 0;

		case WM_CREATE:

			open.lpstrDeviceType = (LPWSTR)MCI_DEVTYPE_WAVEFORM_AUDIO;
			open.lpstrElementName = (LPWSTR)"music_sample1.wav";
			open3.lpstrDeviceType = (LPWSTR)MCI_DEVTYPE_WAVEFORM_AUDIO;
			open3.lpstrElementName = (LPWSTR)"music_sample3.wav";

			result = mciSendCommand(0, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_TYPE_ID | MCI_OPEN_ELEMENT, (DWORD_PTR)&open);

			result = mciSendCommand(0, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_TYPE_ID | MCI_OPEN_ELEMENT, (DWORD_PTR)&open3);

			//エラーなら０以外が返る
			if (result) {
				//エラー取得
				mciGetErrorString(result, (LPWSTR)buf, sizeof(buf));

				MSG(buf);

				PostQuitMessage(0);

				return -1;
			}

			//mp3再生の場合
			open2.lpstrDeviceType = (LPWSTR)"MPEGVideo";
			open2.lpstrElementName = (LPWSTR)"music_sample2.mp3";
			result = mciSendCommand(0, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT,
				(DWORD_PTR)&open2);

			//エラーなら０以外が返る
			if (result) {
				//エラー取得
				mciGetErrorString(result, (LPWSTR)buf, sizeof(buf));
				MSG(buf);
				PostQuitMessage(0);
				return -1;
			}

			play.dwCallback = (DWORD)hwnd;
			play2.dwCallback = (DWORD)hwnd;
			play3.dwCallback = (DWORD)hwnd;

			return 0;
		case WM_LBUTTONDOWN:
			mciSendCommand(open1.wDeviceID, MCI_PLAY, MCI_NOTIFY, (DWORD_PTR)&play1);
			return 0;

		case WM_KEYDOWN:
			mciSendCommand(open2.wDeviceID, MCI_PLAY, MCI_NOTIFY, (DWORD_PTR)&play2);
			return 0;

		case WM_RBUTTONDOWN:
			mciSendCommand(open3.wDeviceID, MCI_PLAY, MCI_NOTIFY, (DWORD_PTR)&play3);
			return 0;

		case MM_MCINOTIFY:
			if (lp == open1.wDeviceID) {
				if (wp == MCI_NOTIFY_SUCCESSFUL) {
						MSG("再生完了");
						//シークバーを先頭に戻す
						mciSendCommand(open1.wDeviceID, MCI_SEEK, MCI_SEEK_TO_START, 0);
				}
				return 0;
			}
			else if (lp == open2.wDeviceID) {
				if (wp == MCI_NOTIFY_SUCCESSFUL) {
						MSG("再生完了");
						//シークバーを先頭に戻す
						mciSendCommand(open2.wDeviceID, MCI_SEEK, MCI_SEEK_TO_START, 0);
				}
				return 0;
			}
			else if (lp == open3.wDeviceID) {
				if (wp == MCI_NOTIFY_SUCCESSFUL) {
						MSG("再生完了");
						//シークバーを先頭に戻す
						mciSendCommand(open3.wDeviceID, MCI_SEEK, MCI_SEEK_TO_START, 0);
				}
				return 0;
			}

	}
	return DefWindowProc(hwnd, msg, wp, lp);
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
		MSG msg;
		WNDCLASS wc;

		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = WinProc;
		wc.cbClsExtra = wc.cbWndExtra = 0;
		wc.hInstance = hInstance;
		wc.hCursor = wc.hIcon = NULL;
		wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
		wc.lpszClassName = (LPWSTR)"test";
		wc.lpszMenuName = NULL;

		if (!RegisterClass(&wc)) {
				MSG("クラスの登録失敗");
				return -1;
		}

		//インスタンスハンドル
		hinst = hInstance;
		hwnd = CreateWindowA("test", "テストウィンドウ", WS_VISIBLE | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, 0, 0, 400, 400, NULL, NULL, hInstance, NULL);


		if (hwnd == NULL) {
				MSG("ウィンドウ作成失敗");
				return -1;
		}

		//エラーチェック用変数
		int check;
		while (check = GetMessage(&msg, NULL, 0, 0)) {
				if (check == -1) {
						break;
				}
				DispatchMessage(&msg);
		}

		//クラス解放
		UnregisterClass((LPWSTR)"test", hinst);

		return 0;
}
*/