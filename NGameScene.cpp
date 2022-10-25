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
	player_->GetState();

	stage_ = StageManager::GetInstance();
	stage_->Initialize(dx12);

	col_ = Collision::GetInstance();
	col_->Initialize();

	//前景スプライト生成
	resultSprite = std::make_unique<NSprite>();
	resultSprite->texNum = RESULTTEXT;
	resultSprite->CreateSprite(dx12->GetDevice(), NSceneManager::GetTex()[resultSprite->texNum].texBuff);
	resultSprite->position = { NWindows::win_width / 2,100.0f,0 };
	resultSprite->UpdateMatrix();

	stageSelectSprite = std::make_unique<NSprite>();
	stageSelectSprite->texNum = STAGESELECTTEXT;
	stageSelectSprite->CreateSprite(dx12->GetDevice(), NSceneManager::GetTex()[stageSelectSprite->texNum].texBuff);
	stageSelectSprite->SetColor(whiteColor);
	stageSelectSprite->position = { NWindows::win_width / 2,700.0f,0 };
	stageSelectSprite->UpdateMatrix();

	retrySprite = std::make_unique<NSprite>();
	retrySprite->texNum = RETRYTEXT;
	retrySprite->CreateSprite(dx12->GetDevice(), NSceneManager::GetTex()[retrySprite->texNum].texBuff);
	retrySprite->SetColor(yellowColor);
	retrySprite->position = { NWindows::win_width / 2,600.0f,0 };
	retrySprite->UpdateMatrix();

	nextSprite = std::make_unique<NSprite>();
	nextSprite->texNum = NEXTTEXT;
	nextSprite->CreateSprite(dx12->GetDevice(), NSceneManager::GetTex()[nextSprite->texNum].texBuff);
	nextSprite->SetColor(yellowColor);
	nextSprite->position = { NWindows::win_width / 2,600.0f,0 };
	nextSprite->UpdateMatrix();

	decimalPointSprite = std::make_unique<NSprite>();
	decimalPointSprite->texNum = static_cast<int>(NUMBER);
	decimalPointSprite->CreateClipSprite(dx12->GetDevice(), NSceneManager::GetTex()[decimalPointSprite->texNum].texBuff, { 10 * 48.0f, 0.0f }, { 48.0f, 69.0f });
	decimalPointSprite->SetColor(1, 1, 1, 1);
	decimalPointSprite->size = { 96.0f,138.0f };
	decimalPointSprite->TransferVertex();
	decimalPointSprite->position.x = 348.0f;
	decimalPointSprite->position.y = 400.0f;
	decimalPointSprite->UpdateMatrix();

	timeSprite = std::make_unique<NSprite>();
	timeSprite->texNum = static_cast<int>(TIMETEXT);
	timeSprite->CreateSprite(dx12->GetDevice(), NSceneManager::GetTex()[timeSprite->texNum].texBuff);
	timeSprite->position = { NWindows::win_width / 2,250.0f,0 };
	timeSprite->UpdateMatrix();

	isDisplayTimeChange = true;
}

void NGameScene::Update(NDX12* dx12)
{
	cosRota += 0.1;

	// --プレイヤー更新処理-- //
	player_->Update(camera->GetMatView(), camera->GetMatProjection());
	camera->SetScrollY(player_->GetScrollY());

	col_->Update(dx12, camera->GetMatView(), camera->GetMatProjection());
	camera->SetScrollY(col_->GetScrollY());

	stage_->Update(camera->GetMatView(), camera->GetMatProjection());

	camera->CreateMatView();

	if (NSceneManager::GetPlayEffect() == false)
	{
		if (NInput::IsKeyTrigger(DIK_Q)) {
			NSceneManager::SetScene(TITLESCENE);
		}
	}

	if (player_->GetState() < Death) {
		int nowCount_ = Util::GetNowCount();
		gameTime_ = (nowCount_ - startCount_) / 1000.0f;
	}

	else if (player_->GetState() == Goal) {
		if (isDisplayTimeChange == true) {
			int saveNum = gameTime_ * 100;
			displayNum[0] = static_cast<int>(saveNum / 10000);
			saveNum = saveNum % 10000;
			displayNum[1] = static_cast<int>(saveNum / 1000);
			saveNum = saveNum % 1000;
			displayNum[2] = static_cast<int>(saveNum / 100);
			saveNum = saveNum % 100;
			displayNum[3] = static_cast<int>(saveNum / 10);
			saveNum = saveNum % 10;
			displayNum[4] = static_cast<int>(saveNum / 1);

			timeNumSprite[0] = std::make_unique<NSprite>();
			timeNumSprite[0]->texNum = static_cast<int>(NUMBER);
			timeNumSprite[0]->CreateClipSprite(dx12->GetDevice(), NSceneManager::GetTex()[timeNumSprite[0]->texNum].texBuff, { displayNum[0] * 48.0f, 0.0f }, { 48.0f, 69.0f });
			timeNumSprite[0]->SetColor(1, 1, 1, 1);
			timeNumSprite[0]->size = { 96.0f,138.0f };
			timeNumSprite[0]->TransferVertex();
			timeNumSprite[0]->position.x = 108.0f;
			timeNumSprite[0]->position.y = 400.0f;
			timeNumSprite[0]->UpdateMatrix();

			timeNumSprite[1] = std::make_unique<NSprite>();
			timeNumSprite[1]->texNum = static_cast<int>(NUMBER);
			timeNumSprite[1]->CreateClipSprite(dx12->GetDevice(), NSceneManager::GetTex()[timeNumSprite[1]->texNum].texBuff, { displayNum[1] * 48.0f, 0.0f }, { 48.0f, 69.0f });
			timeNumSprite[1]->SetColor(1, 1, 1, 1);
			timeNumSprite[1]->size = { 96.0f,138.0f };
			timeNumSprite[1]->TransferVertex();
			timeNumSprite[1]->position.x = 198.0f;
			timeNumSprite[1]->position.y = 400.0f;
			timeNumSprite[1]->UpdateMatrix();

			timeNumSprite[2] = std::make_unique<NSprite>();
			timeNumSprite[2]->texNum = static_cast<int>(NUMBER);
			timeNumSprite[2]->CreateClipSprite(dx12->GetDevice(), NSceneManager::GetTex()[timeNumSprite[2]->texNum].texBuff, { displayNum[2] * 48.0f, 0.0f }, { 48.0f, 69.0f });
			timeNumSprite[2]->SetColor(1, 1, 1, 1);
			timeNumSprite[2]->size = { 96.0f,138.0f };
			timeNumSprite[2]->TransferVertex();
			timeNumSprite[2]->position.x = 288.0f;
			timeNumSprite[2]->position.y = 400.0f;
			timeNumSprite[2]->UpdateMatrix();

			timeNumSprite[3] = std::make_unique<NSprite>();
			timeNumSprite[3]->texNum = static_cast<int>(NUMBER);
			timeNumSprite[3]->CreateClipSprite(dx12->GetDevice(), NSceneManager::GetTex()[timeNumSprite[3]->texNum].texBuff, { displayNum[3] * 48.0f, 0.0f }, { 48.0f, 69.0f });
			timeNumSprite[3]->SetColor(1, 1, 1, 1);
			timeNumSprite[3]->size = { 96.0f,138.0f };
			timeNumSprite[3]->TransferVertex();
			timeNumSprite[3]->position.x = 402.0f;
			timeNumSprite[3]->position.y = 400.0f;
			timeNumSprite[3]->UpdateMatrix();

			timeNumSprite[4] = std::make_unique<NSprite>();
			timeNumSprite[4]->texNum = static_cast<int>(NUMBER);
			timeNumSprite[4]->CreateClipSprite(dx12->GetDevice(), NSceneManager::GetTex()[timeNumSprite[4]->texNum].texBuff, { displayNum[4] * 48.0f, 0.0f }, { 48.0f, 69.0f });
			timeNumSprite[4]->SetColor(1, 1, 1, 1);
			timeNumSprite[4]->size = { 96.0f,138.0f };
			timeNumSprite[4]->TransferVertex();
			timeNumSprite[4]->position.x = 492.0f;
			timeNumSprite[4]->position.y = 400.0f;
			timeNumSprite[4]->UpdateMatrix();

			isDisplayTimeChange = false;
		}

		if (NInput::IsKeyTrigger(DIK_DOWN)) {
			if (selectText == NextText) {
				selectText = StageSelectText;
				nextSprite->size.x = 162.0f;
				nextSprite->size.y = 69.0f;
				nextSprite->TransferVertex();
				nextSprite->SetColor(whiteColor);
				stageSelectSprite->SetColor(yellowColor);
				cosRota = 0.0f;
			}
		}

		else if (NInput::IsKeyTrigger(DIK_UP)) {
			if (selectText == StageSelectText) {
				selectText = NextText;
				stageSelectSprite->size.x = 439.0f;
				stageSelectSprite->size.y = 69.0f;
				stageSelectSprite->TransferVertex();
				stageSelectSprite->SetColor(whiteColor);
				nextSprite->SetColor(yellowColor);
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
				retrySprite->SetColor(whiteColor);
				stageSelectSprite->SetColor(yellowColor);
				cosRota = 0.0f;
			}
		}

		else if (NInput::IsKeyTrigger(DIK_UP)) {
			if (selectText == StageSelectText) {
				selectText = RetryText;
				stageSelectSprite->size.x = 439.0f;
				stageSelectSprite->size.y = 69.0f;
				stageSelectSprite->TransferVertex();
				stageSelectSprite->SetColor(whiteColor);
				retrySprite->SetColor(yellowColor);
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

	col_->Draw(dx12);

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

		for (size_t i = 0; i < 5; i++) {
			timeNumSprite[i]->CommonBeginDraw(dx12->GetCommandList(), NSceneManager::GetPipelineSprite()->pipelineSet.pipelineState,
				NSceneManager::GetPipelineSprite()->pipelineSet.rootSig.entity, dx12->GetSRVHeap());
			timeNumSprite[i]->Draw(dx12->GetSRVHeap(), NSceneManager::GetTex()[0].incrementSize, dx12->GetCommandList());
		}

		timeSprite->CommonBeginDraw(dx12->GetCommandList(), NSceneManager::GetPipelineSprite()->pipelineSet.pipelineState,
			NSceneManager::GetPipelineSprite()->pipelineSet.rootSig.entity, dx12->GetSRVHeap());
		timeSprite->Draw(dx12->GetSRVHeap(), NSceneManager::GetTex()[0].incrementSize, dx12->GetCommandList());

		decimalPointSprite->CommonBeginDraw(dx12->GetCommandList(), NSceneManager::GetPipelineSprite()->pipelineSet.pipelineState,
			NSceneManager::GetPipelineSprite()->pipelineSet.rootSig.entity, dx12->GetSRVHeap());
		decimalPointSprite->Draw(dx12->GetSRVHeap(), NSceneManager::GetTex()[0].incrementSize, dx12->GetCommandList());
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
	isDisplayTimeChange = true;
	stageSelectSprite->SetColor(whiteColor);
	retrySprite->SetColor(yellowColor);
	nextSprite->SetColor(yellowColor);
	startCount_ = Util::GetNowCount();
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
	//col_->Release();
	/*for (size_t i = 0; i < maxForeSprite; i++)
	{
		delete foreSprite[i];
	}

	for (size_t i = 0; i < maxBackSprite; i++)
	{
		delete backSprite[i];
	}*/
}