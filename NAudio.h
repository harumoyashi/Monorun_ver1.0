#pragma once
#include <xaudio2.h>

#pragma comment(lib,"xaudio2.lib")

#include <array>
#include <cstdint>
#include <set>
#include <string>
#include <unordered_map>
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
	//���O
	std::string name_;
};

// �Đ��f�[�^
struct Voice {
	uint32_t handle = 0u;
	IXAudio2SourceVoice* sourceVoice = nullptr;
};

class NAudio
{
private:
	// �T�E���h�f�[�^�̍ő吔
	static const int kMaxSoundData = 256;

	//�`�����N�w�b�_�[
	struct ChunkHeader
	{
		char id[4];		//�`�����N����ID
		int32_t size;	//�`�����N�T�C�Y
	};

	//RIFF�w�b�_�[�`�����N
	struct RiffHeader
	{
		ChunkHeader chunk;	//"RIFF"
		char type[4];		//"WAVE"
	};

	//FMT�`�����N
	struct FormatChunk
	{
		ChunkHeader chunk;	//"fmt"
		WAVEFORMATEX fmt;	//�g�`�t�H�[�}�b�g
	};

	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	ComPtr<IXAudio2> xAudio2;
	IXAudio2MasteringVoice* masterVoice;

	std::ifstream file;			//�t�@�C�����̓X�g���[���̃C���X�^���X
	FormatChunk format = {};	//�t�H�[�}�b�g�`�����N
	ChunkHeader data;			//�f�[�^�`�����N
	char* pBuffer;
	SoundData soundData = {};	//�����f�[�^

	//�I�[�f�B�I�R�[���o�b�N
	class XAudio2VoiceCallback : public IXAudio2VoiceCallback {
	public:
		// �{�C�X�����p�X�̊J�n��
		STDMETHOD_(void, OnVoiceProcessingPassStart)(THIS_ UINT32 BytesRequired) {};
		// �{�C�X�����p�X�̏I����
		STDMETHOD_(void, OnVoiceProcessingPassEnd)(THIS) {};
		// �o�b�t�@�X�g���[���̍Đ����I��������
		STDMETHOD_(void, OnStreamEnd)(THIS) {};
		// �o�b�t�@�̎g�p�J�n��
		STDMETHOD_(void, OnBufferStart)(THIS_ void* pBufferContext) {};
		// �o�b�t�@�̖����ɒB������
		STDMETHOD_(void, OnBufferEnd)(THIS_ void* pBufferContext);
		// �Đ������[�v�ʒu�ɒB������
		STDMETHOD_(void, OnLoopEnd)(THIS_ void* pBufferContext) {};
		// �{�C�X�̎��s�G���[��
		STDMETHOD_(void, OnVoiceError)(THIS_ void* pBufferContext, HRESULT Error) {};
	};

public:
	static NAudio* GetInstance();

	//������
	//�f�B���N�g���p�X���w�肵�Ȃ���"Resources/sound/"�ɂȂ�
	void Initialize(const std::string& directoryPath = "Resources/sound/");

	//�I������
	void Finalize();
	//WAV�����ǂݍ���
	//"filename" = WAV�t�@�C����
	//return �T�E���h�f�[�^�n���h��
	uint32_t LoadWave(const std::string& filename);

	//�T�E���h�f�[�^�̉��
	void Unload(SoundData* soundData);


	//�����Đ�
	//"soundDataHandle" �T�E���h�f�[�^�n���h��
	//"loopFlag" ���[�v�Đ��t���O
	//"volume" �{�����[��
	//0�Ŗ����A1���f�t�H���g���ʁB���܂�傫����������Ɖ����ꂷ��
	//return �Đ��n���h��
	uint32_t PlayWave(uint32_t soundDataHandle, bool loopFlag = false, float volume = 1.0f);

	//�����ꎞ��~
	//"voiceHandle" �Đ��n���h��
	void StopWave(uint32_t voiceHandle);

	//�ꎞ��~�����������Đ�
	//"voiceHandle" �Đ��n���h��
	void StartWave(uint32_t voiceHandle);

	//�����Đ������ǂ���
	//"voiceHandle" �Đ��n���h��
	bool IsPlaying(uint32_t voiceHandle);

	//���ʐݒ�
	//"voiceHandle" �Đ��n���h��
	//"volume" �{�����[��
	//0�Ŗ����A1���f�t�H���g���ʁB���܂�傫����������Ɖ����ꂷ��
	void SetVolume(uint32_t voiceHandle, float volume);

private:
	// XAudio2�̃C���X�^���X
	Microsoft::WRL::ComPtr<IXAudio2> xAudio2_;
	// �T�E���h�f�[�^�R���e�i
	std::array<SoundData, kMaxSoundData> soundDatas_;
	// �Đ����f�[�^�R���e�i
	// std::unordered_map<uint32_t, IXAudio2SourceVoice*> voices_;
	std::set<Voice*> voices_;
	// �T�E���h�i�[�f�B���N�g��
	std::string directoryPath_;
	// ���Ɏg���T�E���h�f�[�^�̔ԍ�
	uint32_t indexSoundData_ = 0u;
	// ���Ɏg���Đ����f�[�^�̔ԍ�
	uint32_t indexVoice_ = 0u;
	// �I�[�f�B�I�R�[���o�b�N
	XAudio2VoiceCallback voiceCallback_;

	IXAudio2* GetXAudio2() { return xAudio2.Get(); }
};