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

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

class NGameScene
{
private:
	Player* player_;

	StageManager* stage_;

	Collision* col_;

	// --�I�u�W�F�N�g-- //
	NMaterial material_;// -> �}�e���A��
	std::unique_ptr<NCube> cube;	//�����̏��(���_�A�C���f�b�N�X)

#pragma region �J�����֌W
	//�J����
	XMMATRIX matProjection;
	XMMATRIX matView;

	//�J����
	XMFLOAT3 eye;		//���_���W
	XMFLOAT3 target;	//�����_���W
	XMFLOAT3 up;		//������x�N�g��
	float angle = 0.0f;	//�J�����̉�]�p
#pragma endregion

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