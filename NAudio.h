#pragma once
#include <xaudio2.h>

#pragma comment(lib,"xaudio2.lib")

#include <fstream>
#include <wrl.h>

//�����f�[�^
struct SoundData
{
	//�g�`�t�H�[�}�b�g
	WAVEFORMATEX wfex;
	//�o�b�t�@�̐擪�A�h���X
	BYTE* pBuffer;
	//�o�b�t�@�̃T�C�Y
	unsigned int bufferSize;
};

class NAudio
{
private:

	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	ComPtr<IXAudio2> xAudio2;
	IXAudio2MasteringVoice* masterVoice;


	std::ifstream file;			//�t�@�C�����̓X�g���[���̃C���X�^���X
	FormatChunk format = {};	//�t�H�[�}�b�g�`�����N
	ChunkHeader data;			//�f�[�^�`�����N
	char* pBuffer;
	SoundData soundData = {};	//�����f�[�^

public:
	void Initialize();

	//XAudio�G���W���̃C���X�^���X�𐶐�
	void CreateInstance();
	//�����f�[�^�̓ǂݍ���
	SoundData SoundLoadWave(const char* filename);
	//�����f�[�^���
	void SoundUnload(SoundData* soundData);
	//�����Đ�
	void SoundPlayWave(IXAudio2* xAudio2, const SoundData& soundData);

private:
	//�t�@�C���ǂݍ���
	void FileOpen(const char* filename);
	//.wav�f�[�^�ǂݍ���
	void LoadWave();
	//�ǂݍ��񂾉����f�[�^��Ԃ����߂Ɋi�[
	void SetSoundData();
};