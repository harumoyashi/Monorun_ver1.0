#pragma once
#include "Util.h"
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

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

class Obstacle {
	/// --�����o�ϐ�-- ///
public:

private:
	// --�I�u�W�F�N�g-- //
	NObj3d object_;

	// --�u���b�N�̎��
	int blockType_;

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
	void Update(XMMATRIX& matView, XMMATRIX& matProjection, XMFLOAT3& eye, XMFLOAT3& target, XMFLOAT3& up);

	// --�`�揈��-- //
	void Draw(NDX12* dx12, NMaterial material,NCube*cube);

	// --�I�u�W�F�N�g���Q��-- //
	BoxObj GetBoxObj();

	// --�u���b�N�̎�ނ��Q��
	int GetBlockType();

private:

	/// --�����o�֐�END-- ///
};