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
#include "NCube.h"
#include "Particle.h"
#include "Effect.h"
#include "NAudio.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

class NTitleScene
{
private:
	//�I�u�W�F�N�g
	NMaterial material;				//�}�e���A��
	std::unique_ptr<NObj3d> player;			//�I�u�W�F�N�g(�萔�o�b�t�@)

	std::unique_ptr<NCube> cube;	//�����̏��(���_�A�C���f�b�N�X)

	////�w�i�X�v���C�g
	//static const int maxBackSprite = 3;	//�w�i�X�v���C�g��
	//NSprite* backSprite[maxBackSprite];	//�w�i�X�v���C�g

	//�O�i�X�v���C�g
	//static const int maxForeSprite = 2;	//�O�i�X�v���C�g��
	std::unique_ptr<NSprite> titleSprite;	//�^�C�g�����S
	std::unique_ptr<NSprite> startSprite;	//�X�^�[�gUI

	//�J����
	XMMATRIX matProjection;
	XMMATRIX matView;

	//�J����
	XMFLOAT3 eye = { 0, 0, -100 };	//���_���W
	XMFLOAT3 target = { 0, 0, 0 };	//�����_���W
	XMFLOAT3 up = { 0, 1, 0 };		//������x�N�g��
	float angle = 0.0f;				//�J�����̉�]�p

	NAudio* audio = nullptr;
	static const int maxSoundData = 3;
	uint32_t soundData[maxSoundData] = {};

public:
	//�C���X�^���X�擾
	static NTitleScene* GetInstance();

	//������
	void Initialize(NDX12* dx12);
	//�X�V
	void Update();
	//�`��
	void Draw(NDX12* dx12);
	//���Z�b�g����
	void Reset();
	//�I������
	void Finalize();
};