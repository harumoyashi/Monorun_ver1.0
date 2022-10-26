#pragma once
#include "NDX12.h"

#include <vector>
#include <wrl.h>

#include "NRootParam.h"
#include "NMaterial.h"
#include "NCube.h"
#include "NGPipeline.h"
#include "NRootSignature.h"
#include "NObj3d.h"
#include "NTexture.h"
#include "NSprite.h"
#include "NPreDraw.h"
#include "Player.h"
#include "StageManager.h"
#include "NStageSelectScene.h"
#include "Collision.h"
#include "NCamera.h"
#include "NAudio.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

enum SelectText {
	RetryText = 1,
	NextText = 1,
	StageSelectText = 2
};

enum Wave {
	StartScene,
	GameScene,
	DeathResultScene,
	GoalResultScene
};

class NGameScene
{
private:
	Player* player_;

	StageManager* stage_;

	Collision* col_;

	NStageSelectScene* selectScene_;

	// --�Q�[���V�[���̒i�K-- //
	int sceneWave_;

	// --���肵��-- //
	bool isDecision_;

	// --�I�u�W�F�N�g-- //
	NMaterial material_;// -> �}�e���A��
	std::unique_ptr<NCube> cube;	//�����̏��(���_�A�C���f�b�N�X)

	std::unique_ptr<NCamera> camera;	//�J����

	std::unique_ptr<NSprite> resultSprite;	//���U���g�e�L�X�g
	std::unique_ptr<NSprite> stageSelectSprite;	//�X�e�[�W�Z���N�g�e�L�X�g
	std::unique_ptr<NSprite> retrySprite;	//���g���C�e�L�X�g
	std::unique_ptr<NSprite> nextSprite;	//�l�N�X�g�e�L�X�g
	std::unique_ptr<NSprite> timeNumSprite[5];//����
	std::unique_ptr<NSprite> decimalPointSprite;//�����_
	std::unique_ptr<NSprite> timeSprite;//�^�C���e�L�X�g
	std::unique_ptr<NSprite> speedSprite[3];	//���x�\���p�X�v���C�g
	std::unique_ptr<NSprite> countSprite;//�X�^�[�g�܂ł̃J�E���g
	std::unique_ptr<NSprite> goSprite;//[GO]�e�L�X�g�X�v���C�g
	std::unique_ptr<NSprite> minusSprite;//[-]�e�L�X�g�X�v���C�g
	std::unique_ptr<NSprite> kmSprite;//[/km]�e�L�X�g�X�v���C�g

	XMFLOAT4 whiteColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	XMFLOAT4 yellowColor = { 0.9f, 0.9f, 0.3f, 1.0f };

	float goTextAlpha;

	int displayNum[5] = {0, 0, 0, 0, 0};

	int selectText = 1;

	float cosRota;

	int startCount_;

	float gameTime_;

	bool isDisplayTimeChange;

	// --�\�����鑬�x�̒l-- //
	int disPlaySpeed[3];

	// --�Q�[���J�n���̃J�E���g-- //
	float gameStartCountTime_ = 5.0f;

	NAudio* audio = nullptr;
	static const int maxSoundData = 5;
	uint32_t soundData[maxSoundData] = {};

	bool isCrear = false;

public:


public:
	//�C���X�^���X�擾
	static NGameScene* GetInstance();

	//������
	void Initialize(NDX12* dx12);
	//�X�V
	void Update(NDX12* dx12);
	//�`��
	void Draw(NDX12* dx12);
	//���Z�b�g����
	void Reset(NDX12* dx12);
	//�I������
	void Finalize();

private:


};