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
#include "Collision.h"
#include "NCamera.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

enum SelectText {
	RetryText = 1,
	NextText = 1,
	StageSelectText = 2
};

class NGameScene
{
private:
	Player* player_;

	StageManager* stage_;

	Collision* col_;

	// --�I�u�W�F�N�g-- //
	NMaterial material_;// -> �}�e���A��
	std::unique_ptr<NCube> cube;	//�����̏��(���_�A�C���f�b�N�X)

	std::unique_ptr<NCamera> camera;	//�J����

	std::unique_ptr<NSprite> resultSprite;	//���U���g�e�L�X�g
	std::unique_ptr<NSprite> stageSelectSprite;	//�X�e�[�W�Z���N�g�e�L�X�g
	std::unique_ptr<NSprite> retrySprite;	//���g���C�e�L�X�g
	std::unique_ptr<NSprite> nextSprite;	//�l�N�X�g�e�L�X�g

	int selectText = 1;

	float cosRota;

public:


public:
	//�C���X�^���X�擾
	static NGameScene* GetInstance();

	//������
	void Initialize(NDX12* dx12);
	//�X�V
	void Update();
	//�`��
	void Draw(NDX12* dx12);
	//���Z�b�g����
	void Reset(NDX12* dx12);
	//�I������
	void Finalize();

private:


};