#pragma once
#ifndef MUSIC_CONTROLLER_H
#define MUSIC_CONTROLLER_H

#define MSG(m) { MessageBoxA(NULL,m,NULL,MB_OK); }
#include "alut.h"

HWND hwnd;			//�E�B���h�E�n���h��
HINSTANCE hinst;	//�C���X�^���X�n���h��
#define WIDTH 500
#define HEIGHT 300

static MCI_OPEN_PARMS open1, open2, open3;
static MCI_PLAY_PARMS play1, play2, play3;
int sound_check;

enum BGM_IDX {				//BGM�̑啶���t�@�C�����𐔎��Ή�������
	MAIN1   = 0,
	START1  = 1,
	ALL     = 2,
	MAIN2   = 3,

};

enum SE_IDX {				//SE�̑啶���t�@�C�����𐔎��Ή�������
	COLLISION = 0,
	GET_ITEM  = 1,
};

enum COMMAND {				//playMusic() ��2�ڂ̈���
	OPEN = 0,
	PLAY_INSTANCE = 1,
	PLAY_LOOP = 2,
	STOP = 3,
	PAUSE = 4,
	RESUME = 5,
	CLOSE = 6,
};

//---SE---//
ALuint seCollisionWall;		//�ǂɏՓ˂���SE
ALuint seCollisionEnemy;	//�G�ɏՓ˂���SE
ALuint seGetItem;			//�A�C�e�����擾����SE

//---BGM---//
ALuint bgmStart;			//�X�^�[�g��ʂ�BGM
ALuint bgmPlay;				//�v���C��ʂ�BGM

//---souce SE---//
ALuint sseCollisionWall;	//�ǂɏՓ˂���sSE
ALuint sseCollisionEnemy;	//�G�ɏՓ˂���sSE
ALuint sseGetItem;			//�A�C�e�����擾����sSE

//---souce BGM---//
ALuint sbgmStart;			//�X�^�[�g��ʂ�sBGM
ALuint sbgmPlay;			//�v���C��ʂ�sBGM

int countCollisionWall = 0;	//�ǂɏՓˎ���SE��A�����Đ����Ȃ��悤�ɐ��䂷��
int initCountCollision = 300;	//count�����̒l�𒴂�����0�ɏ���������


//void musicController(const char* filename, const char* command);
void playMusic(int file_idx, int commnad);	//BGM, SE�̍Đ��𒼊��I�ɍs�����߂̊֐�
void InitMusic();							//SE�̏�����
void closeMusic();							//���ׂĂ�music�����
void soundPlay(ALuint buf, ALuint *source, bool loop);
void soundStop(ALuint *source);
bool chackDoubleSpecialKeyPush();


//https://bituse.info/winapi/35
//https://docs.microsoft.com/ja-jp/windows/desktop/Multimedia/mci-open
//http://wisdom.sakura.ne.jp/system/winapi/media/mm3.html


/*

SND_APPLICATION		: �A�v���P�[�V�������L�̊֘A�t�����g���ăT�E���h���Đ����܂��B
SND_ALIAS           : pszSound �p�����[�^�́A���W�X�g���܂��� WIN.INI �t�@�C���ɋL�q����Ă���V�X�e���C�x���g�̕ʖ��i�G�C���A�X�j�ł��BSND_FILENAME �� SND_RESOURCE �Ɠ����Ɏw�肷�邱�Ƃ͂ł��܂���B
SND_ALIAS_ID        : pszSound �p�����[�^�́A��`�ς݂̃T�E���h���ʎq�i"SystemStart"�A"SystemExit" �Ȃǁj�ł��B
SND_ASYNC			: �T�E���h��񓯊��Đ����A�T�E���h���J�n�����ƁAPlaySound �֐��͑����ɐ����Ԃ��܂��B�񓯊��Đ�����Ă���T�E���h���~����ɂ́ApszSound �p�����[�^�� NULL ���w�肵�� PlaySound �֐����Ăяo���Ă��������B
SND_FILENAME        : pszSound �p�����[�^�́A�t�@�C������\���܂��B
SND_LOOP            : �T�E���h���J��Ԃ��Đ����܂��BpszSound �p�����[�^�� NULL ���w�肵�� PlaySound �֐����Ăяo���ƁA�T�E���h����~���܂��B�T�E���h�C�x���g��񓯊��Đ�����悤�w�����邽�߂ɁASND_ASYNC �Ɠ����Ɏw�肵�Ȃ���΂Ȃ�܂���B
SND_MEMORY			: �T�E���h�C�x���g�̃t�@�C���́A���������Ɋ��Ƀ��[�h����Ă��܂��BpszSound �p�����[�^�́A���������̃T�E���h�C���[�W�ւ̃|�C���^��\���܂��B
SND_NODEFAULT		: ����̃T�E���h�C�x���g���g���܂���B�w�肳�ꂽ�T�E���h��������Ȃ������ꍇ�APlaySound �֐��́A����̃T�E���h�i��ʂ̌x�����j���Đ������ɐÂ��ɐ����Ԃ��܂��B
SND_NOSTOP			: ���ɂق��̃T�E���h���Đ�����Ă���ꍇ�A�w�肳�ꂽ�T�E���h���Đ����܂���B�w�肳�ꂽ�T�E���h���Đ����邽�߂ɕK�v�ȃ��\�[�X���A�ق��̃T�E���h���Đ����Ă��ăr�W�[�ł���A�w�肳�ꂽ�T�E���h���Đ��ł��Ȃ��ꍇ�A���̊֐��͎w�肳�ꂽ�T�E���h���Đ������ɁA������ FALSE ��Ԃ��܂��B
						���̃t���O���w�肵�Ȃ������ꍇ�APlaySound �֐��͌��ݍĐ�����Ă���T�E���h���~���āA�w�肳�ꂽ�V�����T�E���h���Đ����悤�Ƃ��܂��B
SND_NOWAIT			: �h���C�o���r�W�[��Ԃ̏ꍇ�A�w�肳�ꂽ�T�E���h���Đ������ɑ����ɐ����Ԃ��܂��B
SND_PURGE			: �Ăяo�����^�X�N�Ɋ֌W����T�E���h�̍Đ����~���܂��BpszSound �p�����[�^�� NULL �ł͂Ȃ��ꍇ�A�w�肵���T�E���h�̂��ׂẴC���X�^���X���~���܂��BpszSound �p�����[�^�� NULL �̏ꍇ�A�Ăяo�����^�X�N�Ɋ֌W���邷�ׂẴT�E���h���~���܂��B
						SND_RESOURCE�p�����[�^�Ŏw�肵���T�E���h�C�x���g���~������ꍇ�́Ahmod �p�����[�^�ŃC���X�^���X�n���h�����w�肵�Ȃ���΂Ȃ�܂���B
SND_RESOURCE        : pszSound �p�����[�^�́A���\�[�X���ʎq��\���܂��Bhmod �p�����[�^�ŁA���\�[�X��ێ����Ă�����s�\�t�@�C���̃C���^���X�n���h�����w�肵�Ȃ���΂Ȃ�܂���B
SND_SYNC            : �T�E���h�C�x���g�𓯊��Đ����܂��BPlaySound �֐��́A�T�E���h�̍Đ�������������Ő����Ԃ��܂��B


MCI_OPEN       : �f�o�C�X���I�[�v�����܂��B
MCI_CLOSE      : �f�o�C�X���N���[�Y���܂�
MCI_PLAY       : �Đ����܂�
MCI_STOP       : ��~���܂�
MCI_PAUSE      : �ꎞ��~���܂�
MCI_RESUME     : �ꎞ��~�������܂�
MCI_SET        : MCI_FORMAT_MILLISECONDS���w�肵�A���Ԃ̃t�H�[�}�b�g���w�肵�܂��B
MCI_SEEK       : MCI_SEEK_TO_START�Ȃǂ��w�肵�A�V�[�N�ʒu�̒��������܂��B
MCI_STATUS     : MCI_STATUS_PARMS.dwItem��MCI_STATUS_LENGTH���w�肵�A�Đ����Ԏ擾
MCI_STATUS     : MCI_STATUS_PARMS.dwItem��MCI_STATUS_POSITION���w�肵�A���݈ʒu���擾
MCI_STATUS     : MCI_STATUS_PARMS.dwItem��MCI_STATUS_MODE���w�肷��ƈȉ��̂��̂�Ԃ��B
					MCI_MODE_PLAY  : �Đ���
					MCI_MODE_STOP  : ��~��
					MCI_MODE_PAUSE : �ꎞ��~��

MCIERROR mciSendCommand(	//wave, MP3�̍Đ��̓����Đ����\
	MCIDEVICEID IDDevice,	// �f�o�C�X���ʎq
	UINT uMsg,				// �R�}���h���b�Z�[�W
	DWORD fdwCommand,		// �t���O
	DWORD dwParam			// �p�����[�^��ێ����Ă���\����
);

typedef struct {
	DWORD dwCallback;
	MCIDEVICEID wDeviceID;
	LPCSTR lpstrDeviceType;		//�������K�v .wav�t�@�C�����Đ�����̂Ȃ� MCI_DEVTYPE_WAVEFORM_AUDIO �ŏ�����
	LPCSTR lpstrElementName;	//�������K�v �Đ�����t�@�C����
	LPCSTR lpstrAlias;
} MCI_OPEN_PARMS;

typedef struct {
	DWORD_PTR dwCallback;
	DWORD dwFrom;
	DWORD dwTo;
} MCI_PLAY_PARMS;

BOOL mciGetErrorString(
	DWORD fdwError,			// �G���[�R�[�h
	LPTSTR lpszErrorText,	// �o�b�t�@�ւ̃|�C���^
	UINT cchErrorText		// �o�b�t�@�̃T�C�Y
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
//	if (false) {	//�G���[�Ȃ��0�ȊO���Ԃ�
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

	//�G���[�Ȃ�O�ȊO���Ԃ�
	if (result) {
		//�G���[�擾
		mciGetErrorString(result, (LPWSTR)buf, sizeof(buf));

		MSG(buf);

		PostQuitMessage(0);

		return -1;
	}

	//mp3�Đ��̏ꍇ
	open2.lpstrDeviceType = (LPWSTR)"MPEGVideo";
	open2.lpstrElementName = (LPWSTR)"music_sample2.mp3";
	result = mciSendCommand(0, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT,
		(DWORD_PTR)&open2);

	//�G���[�Ȃ�O�ȊO���Ԃ�
	if (result) {
		//�G���[�擾
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

			//�G���[�Ȃ�O�ȊO���Ԃ�
			if (result) {
				//�G���[�擾
				mciGetErrorString(result, (LPWSTR)buf, sizeof(buf));

				MSG(buf);

				PostQuitMessage(0);

				return -1;
			}

			//mp3�Đ��̏ꍇ
			open2.lpstrDeviceType = (LPWSTR)"MPEGVideo";
			open2.lpstrElementName = (LPWSTR)"music_sample2.mp3";
			result = mciSendCommand(0, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT,
				(DWORD_PTR)&open2);

			//�G���[�Ȃ�O�ȊO���Ԃ�
			if (result) {
				//�G���[�擾
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
						MSG("�Đ�����");
						//�V�[�N�o�[��擪�ɖ߂�
						mciSendCommand(open1.wDeviceID, MCI_SEEK, MCI_SEEK_TO_START, 0);
				}
				return 0;
			}
			else if (lp == open2.wDeviceID) {
				if (wp == MCI_NOTIFY_SUCCESSFUL) {
						MSG("�Đ�����");
						//�V�[�N�o�[��擪�ɖ߂�
						mciSendCommand(open2.wDeviceID, MCI_SEEK, MCI_SEEK_TO_START, 0);
				}
				return 0;
			}
			else if (lp == open3.wDeviceID) {
				if (wp == MCI_NOTIFY_SUCCESSFUL) {
						MSG("�Đ�����");
						//�V�[�N�o�[��擪�ɖ߂�
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
				MSG("�N���X�̓o�^���s");
				return -1;
		}

		//�C���X�^���X�n���h��
		hinst = hInstance;
		hwnd = CreateWindowA("test", "�e�X�g�E�B���h�E", WS_VISIBLE | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, 0, 0, 400, 400, NULL, NULL, hInstance, NULL);


		if (hwnd == NULL) {
				MSG("�E�B���h�E�쐬���s");
				return -1;
		}

		//�G���[�`�F�b�N�p�ϐ�
		int check;
		while (check = GetMessage(&msg, NULL, 0, 0)) {
				if (check == -1) {
						break;
				}
				DispatchMessage(&msg);
		}

		//�N���X���
		UnregisterClass((LPWSTR)"test", hinst);

		return 0;
}
*/