#pragma once
#include "NDX12.h"

#include <vector>
#include <wrl.h>

#include "NRootParam.h"
#include "NMaterial.h"
#include "NGPipeline.h"
#include "NRootSignature.h"
#include "NObj3d.h"
#include "NTexture.h"
#include "NSprite.h"
#include "NPreDraw.h"
#include "StageManager.h"
#include "NCamera.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

class NStageSelectScene
{
private:
	////�I�u�W�F�N�g
	//NMaterial material;				//�}�e���A��
	//static const int maxObj = 3;	//�I�u�W�F�N�g��
	//NObj3d obj3d[maxObj];			//�I�u�W�F�N�g(�萔�o�b�t�@)

	////�w�i�X�v���C�g
	//static const int maxBackSprite = 3;	//�w�i�X�v���C�g��
	//NSprite* backSprite[maxBackSprite];	//�w�i�X�v���C�g

	StageManager* stage_;

	//�O�i�X�v���C�g
	static const int maxNumSprite = 10;		//�X�e�[�W��
	std::unique_ptr<NSprite> numSprite[maxNumSprite];		//�X�e�[�W�ԍ��X�v���C�g
	std::unique_ptr<NSprite> frameSprite[maxNumSprite];		//�X�e�[�W�g�X�v���C�g

	// --�C�[�W���O�p-- //
	XMFLOAT3 easeStartPos_[10];
	XMFLOAT3 easeEndPos_[10];

	// --���Ԍv���Ɏ��X�ȃf�[�^�ϐ�-- //
	int nowCount_;
	int startCount_;

	// --�X�N���[������̂ɂ����鎞��[s]-- //
	const float maxScrollTime_ = 0.5f;

	// --�X�N���[���J�n����̌o�ߎ���-- //
	float nowScrollTime_;

	// --���ݑI��ł���X�e�[�W-- //
	int selectStage_;

	std::unique_ptr<NCamera> camera;	//�J����

public:
	//�C���X�^���X�擾
	static NStageSelectScene* GetInstance();
	void Reset();

	void Initialize(NDX12* dx12);
	void Update();
	void Draw(NDX12* dx12);
	void Finalize();
};