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
	camera->SetEye({ 0, -300, -1500 });
	camera->SetTarget({ 0.0f, -300.0f, 0.0f });
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

	//�O�i�X�v���C�g����
	resultSprite = std::make_unique<NSprite>();
	resultSprite->texNum = RESULTTEXT;
	resultSprite->CreateSprite(dx12->GetDevice(), NSceneManager::GetTex()[resultSprite->texNum].texBuff);
	resultSprite->position = { NWindows::win_width / 2,200.0f,0 };
	resultSprite->UpdateMatrix();

	stageSelectSprite = std::make_unique<NSprite>();
	stageSelectSprite->texNum = STAGESELECTTEXT;
	stageSelectSprite->CreateSprite(dx12->GetDevice(), NSceneManager::GetTex()[stageSelectSprite->texNum].texBuff);
	stageSelectSprite->position = { NWindows::win_width / 2,500.0f,0 };
	stageSelectSprite->UpdateMatrix();

	retrySprite = std::make_unique<NSprite>();
	retrySprite->texNum = RETRYTEXT;
	retrySprite->CreateSprite(dx12->GetDevice(), NSceneManager::GetTex()[retrySprite->texNum].texBuff);
	retrySprite->position = { NWindows::win_width / 2,600.0f,0 };
	retrySprite->UpdateMatrix();
}

void NGameScene::Update()
{
	// --�v���C���[�X�V����-- //
	player_->Update(camera->GetMatView(), camera->GetMatProjection());
	camera->SetScrollY(player_->GetScrollY());

	col_->Update(camera->GetMatView(), camera->GetMatProjection());
	camera->SetScrollY(col_->GetScrollY());

	stage_->Update(camera->GetMatView(), camera->GetMatProjection());

	camera->CreateMatView();

	if (NSceneManager::GetPlayEffect() == false)
	{
		if (NInput::IsKeyTrigger(DIK_Q)) {
			NSceneManager::SetScene(TITLESCENE);
		}
	}

	if (player_->GetState() == Goal) {
		if (NInput::IsKeyTrigger(DIK_DOWN)) {
			if (selectText == StageSelectText) {
				selectText = RetryText;
			}

			else if (selectText == RetryText) {
				selectText = StageSelectText;
			}

			else {
				selectText = StageSelectText;
			}
		}

		else if (NInput::IsKeyTrigger(DIK_UP)) {
			if (selectText == StageSelectText) {
				selectText = RetryText;
			}

			else if (selectText == RetryText) {
				selectText = StageSelectText;
			}

			else {
				selectText = StageSelectText;
			}
		}

		if (NInput::IsKeyTrigger(DIK_SPACE)) {
			if (selectText == StageSelectText) {
				NSceneManager::SetScene(STAGESELECTSCENE);
			}

			else if (selectText == RetryText) {
				NSceneManager::SetScene(GAMESCENE);
			}
		}
	}

	//if (player_->GetCamShake()) {
	//	eye = Util::CameraShake(eye, shakeCount);
	//	if (0 < shakeCount) {
	//		shakeCount--;
	//	}
	//	else {
	//		player_->SetCamShakeState(false);
	//	}
	//}
}

void NGameScene::Draw(NDX12* dx12)
{
	// --�v���C���[�`�揈��-- //
	player_->Draw(dx12,cube.get());

	stage_->Draw(dx12, material_,cube.get());

	if (player_->GetState() == Goal) {
		resultSprite->CommonBeginDraw(dx12->GetCommandList(), NSceneManager::GetPipelineSprite()->pipelineSet.pipelineState,
			NSceneManager::GetPipelineSprite()->pipelineSet.rootSig.entity, dx12->GetSRVHeap());
		resultSprite->Draw(dx12->GetSRVHeap(), NSceneManager::GetTex()[0].incrementSize, dx12->GetCommandList());

		stageSelectSprite->CommonBeginDraw(dx12->GetCommandList(), NSceneManager::GetPipelineSprite()->pipelineSet.pipelineState,
			NSceneManager::GetPipelineSprite()->pipelineSet.rootSig.entity, dx12->GetSRVHeap());
		stageSelectSprite->Draw(dx12->GetSRVHeap(), NSceneManager::GetTex()[0].incrementSize, dx12->GetCommandList());

		retrySprite->CommonBeginDraw(dx12->GetCommandList(), NSceneManager::GetPipelineSprite()->pipelineSet.pipelineState,
			NSceneManager::GetPipelineSprite()->pipelineSet.rootSig.entity, dx12->GetSRVHeap());
		retrySprite->Draw(dx12->GetSRVHeap(), NSceneManager::GetTex()[0].incrementSize, dx12->GetCommandList());
	}
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