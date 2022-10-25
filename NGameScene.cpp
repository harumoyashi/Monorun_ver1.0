#include "NGameScene.h"
#include "NSceneManager.h"

NGameScene* NGameScene::GetInstance()
{
	static NGameScene instance;
	return &instance;
}

void NGameScene::Initialize(NDX12* dx12)
{
#pragma region	カメラ初期化
	camera = std::make_unique<NCamera>();
	camera->ProjectiveProjection();
	camera->SetEye({ -100, -300, -1500 });
	camera->SetTarget({ 0.0f, -300.0f, 0.0f });
	camera->CreateMatView();
#pragma endregion
	//マテリアル(定数バッファ)
	material_.Initialize(dx12->GetDevice());

	//立方体情報
	cube = std::make_unique<NCube>();
	cube->Initialize(dx12->GetDevice());

	// --プレイヤー初期化-- //
	player_ = Player::GetInstance();
	player_->Initialize(dx12);

	stage_ = StageManager::GetInstance();
	stage_->Initialize(dx12);

	col_ = Collision::GetInstance();

	//前景スプライト生成
	resultSprite = std::make_unique<NSprite>();
	resultSprite->texNum = RESULTTEXT;
	resultSprite->CreateSprite(dx12->GetDevice(), NSceneManager::GetTex()[resultSprite->texNum].texBuff);
	resultSprite->position = { NWindows::win_width / 2,200.0f,0 };
	resultSprite->UpdateMatrix();

	stageSelectSprite = std::make_unique<NSprite>();
	stageSelectSprite->texNum = STAGESELECTTEXT;
	stageSelectSprite->CreateSprite(dx12->GetDevice(), NSceneManager::GetTex()[stageSelectSprite->texNum].texBuff);
	stageSelectSprite->position = { NWindows::win_width / 2,600.0f,0 };
	stageSelectSprite->UpdateMatrix();

	retrySprite = std::make_unique<NSprite>();
	retrySprite->texNum = RETRYTEXT;
	retrySprite->CreateSprite(dx12->GetDevice(), NSceneManager::GetTex()[retrySprite->texNum].texBuff);
	retrySprite->position = { NWindows::win_width / 2,500.0f,0 };
	retrySprite->UpdateMatrix();

	nextSprite = std::make_unique<NSprite>();
	nextSprite->texNum = NEXTTEXT;
	nextSprite->CreateSprite(dx12->GetDevice(), NSceneManager::GetTex()[nextSprite->texNum].texBuff);
	nextSprite->position = { NWindows::win_width / 2,500.0f,0 };
	nextSprite->UpdateMatrix();
}

void NGameScene::Update()
{
	cosRota += 0.1;

	// --プレイヤー更新処理-- //
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
			if (selectText == NextText) {
				selectText = StageSelectText;
				nextSprite->size.x = 162.0f;
				nextSprite->size.y = 69.0f;
				nextSprite->TransferVertex();
				cosRota = 0.0f;
			}
		}

		else if (NInput::IsKeyTrigger(DIK_UP)) {
			if (selectText == StageSelectText) {
				selectText = NextText;
				stageSelectSprite->size.x = 439.0f;
				stageSelectSprite->size.y = 69.0f;
				stageSelectSprite->TransferVertex();
				cosRota = 0.0f;
			}
		}

		if (NInput::IsKeyTrigger(DIK_SPACE)) {
			if (selectText == StageSelectText) {
				if (!NSceneManager::GetPlayEffect()) {
					NSceneManager::SetScene(STAGESELECTSCENE);
				}
			}

			else if (selectText == NextText) {
				if (stage_->GetSelectStage() < 10) {
					stage_->SetCSV(stage_->GetSelectStage() + 1);
					if (!NSceneManager::GetPlayEffect()) {
						NSceneManager::SetScene(GAMESCENE);
					}
				}

				else {
					if (!NSceneManager::GetPlayEffect()) {
						NSceneManager::SetScene(STAGESELECTSCENE);
					}
				}
			}
		}

		if (selectText == NextText) {
			nextSprite->size.x = 162.0f + 16.2f * cosf(cosRota);
			nextSprite->size.y = 69.0f + 6.9f * cosf(cosRota);
			nextSprite->TransferVertex();
		}

		else if (selectText == StageSelectText) {
			stageSelectSprite->size.x = 439.0f + 43.9f * cosf(cosRota);
			stageSelectSprite->size.y = 69.0f + 6.9f * cosf(cosRota);
			stageSelectSprite->TransferVertex();
		}
	}

	else if (player_->GetState() == DeathResult) {
		if (NInput::IsKeyTrigger(DIK_DOWN)) {
			if (selectText == RetryText) {
				selectText = StageSelectText;
				retrySprite->size.x = 197.0f;
				retrySprite->size.y = 69.0f;
				retrySprite->TransferVertex();
				cosRota = 0.0f;
			}
		}

		else if (NInput::IsKeyTrigger(DIK_UP)) {
			if (selectText == StageSelectText) {
				selectText = RetryText;
				stageSelectSprite->size.x = 439.0f;
				stageSelectSprite->size.y = 69.0f;
				stageSelectSprite->TransferVertex();
				cosRota = 0.0f;
			}
		}

		if (NInput::IsKeyTrigger(DIK_SPACE)) {
			if (selectText == StageSelectText) {
				if (!NSceneManager::GetPlayEffect()) {
					NSceneManager::SetScene(STAGESELECTSCENE);
				}
			}

			else if (selectText == RetryText) {
				if (!NSceneManager::GetPlayEffect()) {
					NSceneManager::SetScene(GAMESCENE);
				}
			}
		}

		if (selectText == RetryText) {
			retrySprite->size.x = 197.0f + 19.7f * cosf(cosRota);
			retrySprite->size.y = 69.0f + 6.9f * cosf(cosRota);
			retrySprite->TransferVertex();
		}

		else if (selectText == StageSelectText) {
			stageSelectSprite->size.x = 439.0f + 43.9f * cosf(cosRota);
			stageSelectSprite->size.y = 69.0f + 6.9f * cosf(cosRota);
			stageSelectSprite->TransferVertex();
		}
	}

	if (player_->GetCamShake() || col_->GetCamShake()) {
		camera.get()->SetShakeCount(27);
		player_->SetCamShakeState(false);
		col_->SetCamShakeState(false);
	}

	// 常に実行されてよい
	Util::CameraShake(camera.get(), camera.get()->GetShakeCount());
}

void NGameScene::Draw(NDX12* dx12)
{
	// --プレイヤー描画処理-- //
	player_->Draw(dx12, cube.get());

	stage_->Draw(dx12, material_, cube.get());

	if (player_->GetState() == Goal) {
		resultSprite->CommonBeginDraw(dx12->GetCommandList(), NSceneManager::GetPipelineSprite()->pipelineSet.pipelineState,
			NSceneManager::GetPipelineSprite()->pipelineSet.rootSig.entity, dx12->GetSRVHeap());
		resultSprite->Draw(dx12->GetSRVHeap(), NSceneManager::GetTex()[0].incrementSize, dx12->GetCommandList());

		stageSelectSprite->CommonBeginDraw(dx12->GetCommandList(), NSceneManager::GetPipelineSprite()->pipelineSet.pipelineState,
			NSceneManager::GetPipelineSprite()->pipelineSet.rootSig.entity, dx12->GetSRVHeap());
		stageSelectSprite->Draw(dx12->GetSRVHeap(), NSceneManager::GetTex()[0].incrementSize, dx12->GetCommandList());

		nextSprite->CommonBeginDraw(dx12->GetCommandList(), NSceneManager::GetPipelineSprite()->pipelineSet.pipelineState,
			NSceneManager::GetPipelineSprite()->pipelineSet.rootSig.entity, dx12->GetSRVHeap());
		nextSprite->Draw(dx12->GetSRVHeap(), NSceneManager::GetTex()[0].incrementSize, dx12->GetCommandList());
	}

	if (player_->GetState() == DeathResult) {
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

// --リセット処理-- //
void NGameScene::Reset(NDX12* dx12) {
	selectText = 1;
	camera->SetEye({ -100, -300, -1500 });
	camera->SetTarget({ 0.0f, -300.0f, 0.0f });
	camera->CreateMatView();
	player_->Reset();
	col_->Reset();
	stage_->Reset();
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