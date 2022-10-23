#include "Obstacle.h"
#include "StageManager.h"
#include "NSceneManager.h"


enum struct ShapeType {
	None,		// ����
	Default,	// �����������`
	Iphone,		// �c���������`
	LongWallet,	// ���������قǂ̉����������`
};

// --�R���X�g���N�^-- //
Obstacle::Obstacle(NDX12* dx12, XMFLOAT3 pos, int blockType) {
	// --�v���C���[�I�u�W�F�N�g(�萔�o�b�t�@)-- //
	object_.Initialize(dx12->GetDevice());
	object_.texNum = BLOCK;
	object_.scale = { 32.0f, 32.0f, 32.0f };
	object_.position = pos;

	// --�u���b�N�̎��
	blockType_ = blockType;
}

// --�f�X�g���N�^
Obstacle::~Obstacle() {

}

// --����������
void Obstacle::Initialize() {

}

// --�X�V����
void Obstacle::Update(XMMATRIX& matView, XMMATRIX& matProjection, XMFLOAT3& eye, XMFLOAT3& target, XMFLOAT3& up) {
	//�r���[�ϊ��s��쐬
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
	object_.UpdateMatrix(matView, matProjection);
}

// --�`�揈��
void Obstacle::Draw(NDX12* dx12, NMaterial material,NCube*cube) {
	// --�I�u�W�F�N�g�`��-- //
	object_.CommonBeginDraw(dx12->GetCommandList(), NSceneManager::GetPipeline3d()->pipelineSet.pipelineState, NSceneManager::GetPipeline3d()->pipelineSet.rootSig.entity, dx12->GetSRVHeap());
	object_.Draw(dx12->GetCommandList(), material, dx12->GetSRVHeap(), cube->vbView, cube->ibView, cube->numIB, NSceneManager::GetTex()[0].incrementSize);
}

//// --�I�u�W�F�N�g���Q��-- //
//BoxObj Obstacle::GetBoxObj() {
//	return object_;
//}

// --�u���b�N�̎�ނ��Q��
int Obstacle::GetBlockType() {
	return blockType_;
}