#pragma once
#include "NDX12.h"
#include "NVector3.h"
#include "NMatrix4.h"

#include <vector>
#include <wrl.h>

#include "NMaterial.h"
#include "NGPipeline.h"
#include "NRootParam.h"
#include "NRootSignature.h"
#include "NObj3d.h"
#include "NTexture.h"
#include "NSprite.h"
#include "NPreDraw.h"
#include "NPostDraw.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

class NGameScene
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	NRootParam rootParams;	//���[�g�p�����[�^

	//�I�u�W�F�N�g
	NMaterial material;				//�}�e���A��
	static const int maxObj = 3;	//�I�u�W�F�N�g��
	NObj3d obj3d[maxObj];			//�I�u�W�F�N�g(�萔�o�b�t�@)

	//�J����
	XMMATRIX matProjection;
	XMMATRIX matView;
	XMFLOAT3 eye = { 0, 0, -100 };	//���_���W
	XMFLOAT3 target = { 0, 0, 0 };	//�����_���W
	XMFLOAT3 up = { 0, 1, 0 };		//������x�N�g��
	float angle = 0.0f;				//�J�����̉�]�p

	//�e�N�X�`��
	static const int maxTex = 3;	//�e�N�X�`����
	NTexture tex[maxTex];			//�e�N�X�`��

	//�X�v���C�g
	static const int maxSprite = 3;	//�X�v���C�g��
	NSprite* sprite[maxSprite];		//�X�v���C�g

	//�O���t�B�b�N�X�p�C�v���C��
	NGPipeline* gPipe3d;		//3D�I�u�W�F�N�g�p�O���t�B�b�N�X�p�C�v���C��
	NGPipeline* gPipeSprite;	//�X�v���C�g�p�O���t�B�b�N�X�p�C�v���C��

	float increment = 0.0f;	//�e�X�g�p

public:

public:
	void Initialize(NDX12* dx12);
	void Update();
	void Draw(NDX12* dx12);

	void Finalize();

private:


};

