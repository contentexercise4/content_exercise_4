
#include "pch.h"
#include <cstdio>
#include <iostream>
#include <array>
#include <cmath>
#include <limits>
#include <thread>

#include <AL/alut.h>
#include "AL/al.h"
#include "AL/alc.h"
#include "AL/alext.h"

#define PI 3.141592

void tutorial1();	//1秒分のsin波の音声を再生
void tutorial2();	//4つのsin波を同時に再生
void tutorial3();	//バッファを4つ用意して, それぞれに音声を割り当てて音階を変えながら再生
void tutorial4();	//立体音響


void tutorial1() {
	// OpenAL の 初期化
	ALCdevice* device = alcOpenDevice(nullptr);					// デバイスを開く 
	ALCcontext* context = alcCreateContext(device, nullptr);	// コンテキストを生成 
	alcMakeContextCurrent(context);								// 生成したコンテキストを操作対象にする 

	// 一秒ぶんのsin波(440 Hz)を生成
	//const size_t pcm_freq = 44100;
	//const float key_freq = 440.0;
	//std::array<ALshort, pcm_freq> pcm_data;
	//for (size_t i = 0; i < pcm_data. size(); ++ i) {
	//	pcm_data[i] = std:: sin( key_freq * PI * 2.0 * i / pcm_freq) * std:: numeric_limits < ALshort>::max();
	//}

	//ALuint buffer_id;							// バッファを割り当てるID
	//alGenBuffers( 1, &buffer_id);				// バッファの生成
	//alBufferData(								// 用意したsin波woをバッファにコピー
	//	buffer_id,								// バッファを割り当てるID
	//	AL_FORMAT_MONO16,						// 波形データの量子化ビット数 & チャンネル数
	//	&pcm_data[0],							// リニアPCM形式データが格納されている場所
	//	pcm_data.size() * sizeof(ALshort),		// サイズ(バイト数)
	//	pcm_freq								// pcm_freq
	//);
	
	// 1秒分のsin波(440Hz)をステレオで生成
	const size_t pcm_freq = 44100;
	const float key_freq = 440.0;
	std::array< ALshort, pcm_freq * 2 > pcm_data;
	for (size_t i = 0; (i < pcm_data.size() / 2); ++i) {
		pcm_data[i * 2] = std::sin(key_freq * PI * 2.0 * i / pcm_freq)
						* std::numeric_limits < ALshort >::max();		// 左チャンネルの波形
		pcm_data[i * 2 + 1] = 0;										// 右チャンネルの波形
	}

	ALuint buffer_id;							// バッファを割り当てるID
	alGenBuffers(1, &buffer_id);				// バッファの生成 
	alBufferData(								// 用意したsin波woをバッファにコピー
		buffer_id,								// バッファを割り当てるID
		AL_FORMAT_STEREO16,						// 波形データの量子化ビット数 & チャンネル数
		&pcm_data[0],							// リニアPCM形式データが格納されている場所
		pcm_data.size() * sizeof(ALshort),		// サイズ(バイト数)
		pcm_freq								// pcm_freq
	);

	ALuint source_id;							// ソースを割り当てるID
	alGenSources(1, &source_id);				// ソースの生成
	alSourcei(source_id, AL_BUFFER, buffer_id);	// ソースに再生するバッファを割り当てる
	alSourcePlay(source_id);					// ソースの再生開始

	std::this_thread::sleep_for(std::chrono::seconds(1));	// 1秒待つ

	alSourcei(source_id, AL_BUFFER, 0);			// ソースへのバッファの割り当て
	alDeleteSources(1, &source_id);				// ソースの破棄
	alDeleteBuffers(1, &buffer_id);				// バッファの破棄

	// OpenAL の 後始末 
	alcMakeContextCurrent(nullptr);				// 操作対象のコンテキストを解除
	alcDestroyContext(context);					// コンテキストを破棄 
	alcCloseDevice(device);						// デバイスを閉じる
}

void tutorial2() {
	// OpenAL の 初期化 
	ALCdevice* device = alcOpenDevice( nullptr); 
	ALCcontext* context = alcCreateContext( device, nullptr); 
	alcMakeContextCurrent( context); 

	// sin波(440 Hz)を1秒ぶん生成 
	const size_t pcm_freq = 44100; 
	const size_t pcm_length_sec = 1; 
	const float key_freq = 440.0; 
	std::array < ALshort, pcm_freq * pcm_length_sec > pcm_data; 
	for (size_t i = 0; i < pcm_data. size(); ++ i) {
		pcm_data[i] = std::sin(key_freq * PI * 2.0 * i / pcm_freq) * std::numeric_limits < ALshort >::max();
	} 
	
	ALuint buffer_id; 
	alGenBuffers(1, &buffer_id);					// バッファ の 生成 
	alBufferData(									// 用意したsin波をバッファにコピー
		buffer_id,									// バッファを割り当てるID
		AL_FORMAT_MONO16,							// 波形データの量子化ビット数 & チャンネル数
		&pcm_data[0],								// リニアPCM形式データが格納されている場所
		pcm_data.size() * sizeof(ALshort),			// サイズ(バイト数)
		pcm_freq									// pcm_freq
	);
	const int source_num = 4;						// ソースを４つ生成 
	std::array< ALuint, source_num > source_id;		//ALuint型のsource_num=4つのidをarrayにて確保
	alGenSources(source_id.size(), &source_id[0]);	//すべてのソースを生成
	for (const auto& id : source_id) {
		alSourcei(id, AL_BUFFER, buffer_id);		// 全ソースに再生するバッファを割り当てる 
		alSourcei(id, AL_LOOPING, AL_TRUE);			// ついでにループON 
	} 
	
	for (const auto& id : source_id) { 
		alSourcePlay(id);											// １秒間隔で全ソースの再生 
		std:: this_thread:: sleep_for( std:: chrono:: seconds(1));	// １秒待つ
	}
	
	std::this_thread::sleep_for(std:: chrono:: seconds(2));			// ２秒待つ 

	for (const auto& id : source_id) { 
		alSourcei(id, AL_BUFFER, 0);								// 全ソースへのバッファの割り当て解除 
	} 

	alDeleteSources(source_id. size(), &source_id[0]);				// 全ソースの破棄 
	alDeleteBuffers(1, &buffer_id);									// バッファの破棄 

	// OpenAL の 後始末 
	alcMakeContextCurrent(nullptr); 
	alcDestroyContext(context); 
	alcCloseDevice( device); 
}

void tutorial3() {
	// OpenAL の 初期化 
	ALCdevice* device = alcOpenDevice(nullptr);
	ALCcontext* context = alcCreateContext(device, nullptr);
	alcMakeContextCurrent(context);

	//バッファの生成
	const int buffer_num = 4;
	std::array<ALuint, buffer_num> buffer_id;
	alGenBuffers(buffer_num, &buffer_id[0]);

	// バッファごとにsin波を1秒生成
	float key_freq = 440.0;
	for (const auto& id : buffer_id) {
		const size_t pcm_freq = 44100;
		const size_t pcm_length_sec = 1;

		std::array < ALshort, pcm_freq * pcm_length_sec > pcm_data;
		for (size_t i = 0; i < pcm_data.size(); ++i) {
			pcm_data[i] = std::sin(key_freq * PI * 2.0 * i / pcm_freq) * std::numeric_limits < ALshort >::max();
		}


		alBufferData(									// 用意したsin波をバッファにコピー
			id,											// バッファを割り当てるID
			AL_FORMAT_MONO16,							// 波形データの量子化ビット数 & チャンネル数
			&pcm_data[0],								// リニアPCM形式データが格納されている場所
			pcm_data.size() * sizeof(ALshort),			// サイズ(バイト数)
			pcm_freq									// pcm_freq
		);

		key_freq += 100.0;										//音程を変える
	}
	const int source_num = 4;									// ソースを４つ生成 
	std::array< ALuint, source_num > source_id;					//ALuint型のsource_num=4つのidをarrayにて確保
	alGenSources(source_id.size(), &source_id[0]);				//すべてのソースを生成
	for (size_t i = 0; i < source_id.size(); i++){
		alSourcei(source_id[i], AL_BUFFER, buffer_id[i]);		// 全ソースに再生するバッファを割り当てる 
		alSourcei(source_id[i], AL_LOOPING, AL_TRUE);			// ついでにループON 
	}

	for (const auto& id : source_id) {
		alSourcePlay(id);										// １秒間隔で全ソースの再生 
		std::this_thread::sleep_for(std::chrono::seconds(1));	// １秒待つ
	}

	std::this_thread::sleep_for(std::chrono::seconds(2));		// ２秒待つ 

	for (const auto& id : source_id) {
		alSourcei(id, AL_BUFFER, 0);							// 全ソースへのバッファの割り当て解除 
	}

	alDeleteSources(source_id.size(), &source_id[0]);			// 全ソースの破棄 
	alDeleteBuffers(buffer_id.size(), &buffer_id[0]);			// バッファの破棄 

	// OpenAL の 後始末 
	alcMakeContextCurrent(nullptr);
	alcDestroyContext(context);
	alcCloseDevice(device);
}

void tutorial4() {

}

int main(int argc, char **argv) {
	//tutorial1();
	//tutorial2();
	tutorial3();
	return 0;
}


