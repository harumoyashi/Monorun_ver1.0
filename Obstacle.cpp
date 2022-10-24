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
	// --�u���b�N�̎��
	blockType_ = blockType;

	if (blockType == DeathBlock) {
		// --�v���C���[�I�u�W�F�N�g(�萔�o�b�t�@)-- //
		object_[0].Initialize(dx12->GetDevice());
		object_[0].texNum = BLOCK;
		object_[0].scale = { 24.0f, 24.0f, 24.0f };
		object_[0].position = pos;

		// --�v���C���[�I�u�W�F�N�g(�萔�o�b�t�@)-- //
		object_[1].Initialize(dx12->GetDevice());
		object_[1].texNum = BLOCK;
		object_[1].scale = { 24.0f, 24.0f, 24.0f };
		object_[1].position = pos;
		object_[1].rotation.z = 45.0f;

		// --�v���C���[�I�u�W�F�N�g(�萔�o�b�t�@)-- //
		object_[2].Initialize(dx12->GetDevice());
		object_[2].texNum = BLOCK;
		object_[2].scale = { 24.0f, 24.0f, 24.0f };
		object_[2].position = pos;
		object_[2].rotation.z = -45.0f;
	}
	else if (blockType_ == Coin) {
		// --�v���C���[�I�u�W�F�N�g(�萔�o�b�t�@)-- //
		object_[0].Initialize(dx12->GetDevice());
		object_[0].texNum = BLOCK;
		object_[0].scale = { 24.0f, 24.0f, 24.0f };
		object_[0].position = pos;
	}
	else {
		// --�v���C���[�I�u�W�F�N�g(�萔�o�b�t�@)-- //
		object_[0].Initialize(dx12->GetDevice());
		object_[0].texNum = BLOCK;
		object_[0].scale = { 32.0f, 32.0f, 32.0f };
		object_[0].position = pos;
	}
}

// --�f�X�g���N�^
Obstacle::~Obstacle() {

}

// --����������
void Obstacle::Initialize() {
	if (blockType_ == DeathBlock) {
		object_[0].UpdateMatrix();
		object_[1].UpdateMatrix();
		object_[2].UpdateMatrix();
	}
	else {
		object_[0].UpdateMatrix();
	}
}

// --�X�V����
void Obstacle::Update(XMMATRIX matView, XMMATRIX matProjection) {
	if (blockType_ == DeathBlock) {
		object_[0].TransferMatrix(matView, matProjection);
		object_[1].TransferMatrix(matView, matProjection);
		object_[2].TransferMatrix(matView, matProjection);
	}
	else {
		object_[0].TransferMatrix(matView, matProjection);
	}
}

// --�`�揈��
void Obstacle::Draw(NDX12* dx12, NMaterial material,NCube*cube) {
	// --�I�u�W�F�N�g�`��-- //
	if (blockType_ == DeathBlock) {
		object_[0].CommonBeginDraw(dx12->GetCommandList(), NSceneManager::GetPipeline3d()->pipelineSet.pipelineState, NSceneManager::GetPipeline3d()->pipelineSet.rootSig.entity, dx12->GetSRVHeap());
		object_[0].Draw(dx12->GetCommandList(), material, dx12->GetSRVHeap(), cube->vbView, cube->ibView, cube->numIB, NSceneManager::GetTex()[0].incrementSize);
		object_[1].CommonBeginDraw(dx12->GetCommandList(), NSceneManager::GetPipeline3d()->pipelineSet.pipelineState, NSceneManager::GetPipeline3d()->pipelineSet.rootSig.entity, dx12->GetSRVHeap());
		object_[1].Draw(dx12->GetCommandList(), material, dx12->GetSRVHeap(), cube->vbView, cube->ibView, cube->numIB, NSceneManager::GetTex()[0].incrementSize);
		object_[2].CommonBeginDraw(dx12->GetCommandList(), NSceneManager::GetPipeline3d()->pipelineSet.pipelineState, NSceneManager::GetPipeline3d()->pipelineSet.rootSig.entity, dx12->GetSRVHeap());
		object_[2].Draw(dx12->GetCommandList(), material, dx12->GetSRVHeap(), cube->vbView, cube->ibView, cube->numIB, NSceneManager::GetTex()[0].incrementSize);
	}
	else {
		object_[0].CommonBeginDraw(dx12->GetCommandList(), NSceneManager::GetPipeline3d()->pipelineSet.pipelineState, NSceneManager::GetPipeline3d()->pipelineSet.rootSig.entity, dx12->GetSRVHeap());
		object_[0].Draw(dx12->GetCommandList(), material, dx12->GetSRVHeap(), cube->vbView, cube->ibView, cube->numIB, NSceneManager::GetTex()[0].incrementSize);
	}
}

// --�I�u�W�F�N�g���Q��-- //
BoxObj Obstacle::GetBoxObj() {
	return { {object_[0].position.x, object_[0].position.y}, 32.0f};
}

// --�u���b�N�̎�ނ��Q��
int Obstacle::GetBlockType() {
	return blockType_;
}