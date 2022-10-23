#include "NGameScene.h"
#include "NSceneManager.h"

NGameScene* NGameScene::GetInstance()
{
	static NGameScene instance;
	return &instance;
}

void NGameScene::Initialize(NDX12* dx12)
{
	//�}�e���A��(�萔�o�b�t�@)
	material_.Initialize(dx12->GetDevice());

	//�����̏��
	cube = std::make_unique<NCube>();
	cube->Initialize(dx12->GetDevice());

	// --�v���C���[������-- //
	player_ = Player::GetInstance();
	player_->Initialize(dx12);

	stage_ = StageManager::GetInstance();
	stage_->Initialize(dx12);

	//�ˉe���e�ϊ�//
	matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45.0f),		//�㉺��p45�x
		(float)NWindows::win_width / NWindows::win_height,	//�A�X�y�N�g��(��ʉ���/��ʏc��)
		0.1f, 2000.0f					//�O�[�A���[
	);

	eye = { 0, 100, -1500 };	//���_���W
	target = { 0, 0, 0 };	//�����_���W
	up = { 0, 1, 0 };		//������x�N�g��

	//�r���[�ϊ��s��쐬
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
}

void NGameScene::Update()
{
	// --�v���C���[�X�V����-- //
	player_->Update(matView, matProjection, eye, target, up);

	stage_->Update(matView, matProjection, eye, target, up);

	if (NInput::IsKeyTrigger(DIK_Q)) {
		NSceneManager::SetScene(TITLESCENE);
	}
}

void NGameScene::Draw(NDX12* dx12)
{
	// --�v���C���[�`�揈��-- //
	player_->Draw(dx12,cube.get());

	stage_->Draw(dx12, material_,cube.get());
}

// --���Z�b�g����-- //
void NGameScene::Reset(NDX12* dx12) {
	stage_->LoadCSV(dx12);
}

void NGameScene::Finalize()
{
	stage_->Finalize();
	stage_->Release();
	player_->Finalize();
	/*for (size_t i = 0; i < maxForeSprite; i++)
	{
		delete foreSprite[i];
	}

	for (size_t i = 0; i < maxBackSprite; i++)
	{
		delete backSprite[i];
	}*/
}