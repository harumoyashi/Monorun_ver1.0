#pragma once
#include <xaudio2.h>

#pragma comment(lib,"xaudio2.lib")

#include <fstream>
#include <wrl.h>

//音声データ
struct SoundData
{
	//波形フォーマット
	WAVEFORMATEX wfex;
	//バッファの先頭アドレス
	BYTE* pBuffer;
	//バッファのサイズ
	unsigned int bufferSize;
};

class NAudio
{
private:
	//チャンクヘッダー
	struct ChunkHeader
	{
		char id[4];		//チャンク毎のID
		int32_t size;	//チャンクサイズ
	};

	//RIFFヘッダーチャンク
	struct RiffHeader
	{
		ChunkHeader chunk;	//"RIFF"
		char type[4];		//"WAVE"
	};

	//FMTチャンク
	struct FormatChunk
	{
		ChunkHeader chunk;	//"fmt"
		WAVEFORMATEX fmt;	//波形フォーマット
	};

	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	ComPtr<IXAudio2> xAudio2;
	IXAudio2MasteringVoice* masterVoice;


	std::ifstream file;			//ファイル入力ストリームのインスタンス
	FormatChunk format = {};	//フォーマットチャンク
	ChunkHeader data;			//データチャンク
	char* pBuffer;
	SoundData soundData = {};	//音声データ

public:
	void Initialize();

	//XAudioエンジンのインスタンスを生成
	void CreateInstance();
	//音声データの読み込み
	SoundData SoundLoadWave(const char* filename);
	//音声データ解放
	void SoundUnload(SoundData* soundData);
	//音声再生
	void SoundPlayWave(IXAudio2* xAudio2, const SoundData& soundData);

private:
	//ファイル読み込み
	void FileOpen(const char* filename);
	//.wavデータ読み込み
	void LoadWave();
	//読み込んだ音声データを返すために格納
	void SetSoundData();
};