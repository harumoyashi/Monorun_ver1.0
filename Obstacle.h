#pragma once
#include "Util.h"
#include "NDX12.h"

#include <vector>
#include <wrl.h>

#include "NRootParam.h"
#include "NMaterial.h"
#include "NCube.h"
#include "NParticle.h"
#include "NGPipeline.h"
#include "NRootSignature.h"
#include "NObj3d.h"
#include "NTexture.h"
#include "NSprite.h"
#include "NPreDraw.h"
#include "NReaction.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

class Obstacle {
	/// --�����o�ϐ�-- ///
public:

private:
	// --�I�u�W�F�N�g-- //
	NObj3d object_[3];

	// --�u���b�N�̎��
	int blockType_;

	// --�Փ˃t���O-- //
	bool isCollision_ = false;

	//���A�N�V����//
	NReaction* reaction;

	std::unique_ptr<NParticle> particle;	//�p�[�e�B�N��

	/// --�����o�ϐ�END-- ///
	/// --------------- ///
	/// --�����o�֐�-- ///
public:
	// --�R���X�g���N�^-- //
	Obstacle(NDX12* dx12, XMFLOAT3 pos, int blockType);

	// --�f�X�g���N�^-- //
	~Obstacle();

	// --����������-- //
	void Initialize();

	// --�X�V����-- //
	void Update(XMMATRIX matView, XMMATRIX matProjection);

	// --�`�揈��-- //
	void Draw(NDX12* dx12, NMaterial material,NCube*cube);

	// --�I�u�W�F�N�g���Q��-- //
	BoxObj GetBoxObj();

	// --�u���b�N�̎�ނ��Q��
	int GetBlockType();

	// --�Փ˃t���O�̎擾-- //
	bool GetIsCol() { return isCollision_; }

	// --�Փ˃t���O�̐ݒ�-- //
	void SetIsCol(bool isCollision) { isCollision_ = isCollision; }

private:
	
	/// --�����o�֐�END-- ///
};