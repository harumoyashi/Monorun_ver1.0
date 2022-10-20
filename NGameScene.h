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

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

class NGameScene
{
private:
	//�I�u�W�F�N�g
	NMaterial material;				//�}�e���A��
	static const int maxObj = 3;	//�I�u�W�F�N�g��
	NObj3d obj3d[maxObj];			//�I�u�W�F�N�g(�萔�o�b�t�@)

	//�w�i�X�v���C�g
	static const int maxBackSprite = 3;	//�w�i�X�v���C�g��
	NSprite* backSprite[maxBackSprite];	//�w�i�X�v���C�g

	//�O�i�X�v���C�g
	static const int maxForeSprite = 3;	//�O�i�X�v���C�g��
	NSprite* foreSprite[maxForeSprite];	//�O�i�X�v���C�g

	float increment = 0.0f;	//�e�X�g�p

	//�J����
	XMMATRIX matProjection;
	XMMATRIX matView;

	//�J����
	XMFLOAT3 eye = { 0, 0, -100 };	//���_���W
	XMFLOAT3 target = { 0, 0, 0 };	//�����_���W
	XMFLOAT3 up = { 0, 1, 0 };		//������x�N�g��
	float angle = 0.0f;				//�J�����̉�]�p

public:


public:
	//�C���X�^���X�擾
	NGameScene* GetInstance();

	//������
	void Initialize(NDX12* dx12);
	//�X�V
	void Update();
	//�`��
	void Draw(NDX12* dx12);
	//�I������
	void Finalize();

private:


};