#include "NGameScene.h"
#include "NSceneManager.h"

NGameScene* NGameScene::GetInstance()
{
	static NGameScene instance;
	return &instance;
}

void NGameScene::Initialize(NDX12* dx12)
{
#pragma region	�J����������
	camera = std::make_unique<NCamera>();
	camera->ProjectiveProjection();
	camera->CreateMatView();
#pragma endregion
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

	col_ = Collision::GetInstance();

}

void NGameScene::Update()
{
	// --�v���C���[�X�V����-- //
	player_->Update(camera->GetMatView(), camera->GetMatProjection());
	camera->SetScrollY(player_->GetScrollY());

	col_->Update(camera->GetMatView(), camera->GetMatProjection());
	camera->SetScrollY(player_->GetScrollY());

	stage_->Update(camera->GetMatView(), camera->GetMatProjection());

	camera->CreateMatView();

	if (NSceneManager::GetPlayEffect() == false)
	{
		if (NInput::IsKeyTrigger(DIK_Q)) {
			NSceneManager::SetScene(TITLESCENE);
		}
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
	col_->Release();
	/*for (size_t i = 0; i < maxForeSprite; i++)
	{
		delete foreSprite[i];
	}

	for (size_t i = 0; i < maxBackSprite; i++)
	{
		delete backSprite[i];
	}*/
}