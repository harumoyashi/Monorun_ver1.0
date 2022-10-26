#include "NTitleScene.h"
#include "NSceneManager.h"
#include "NInput.h"

NTitleScene* NTitleScene::GetInstance()
{
	static NTitleScene instance;
	return &instance;
}

void NTitleScene::Initialize(NDX12* dx12)
{
#pragma region	オーディオ初期化
	audio = NAudio::GetInstance();
	soundData[0] = audio->LoadWave("fever.wav");
	soundData[1] = audio->LoadWave("mokugyo.wav");
	soundData[2] = audio->LoadWave("fanfare.wav");
	//BGM鳴らす
	audio->PlayWave(soundData[0], true,0.5f);
#pragma endregion
#pragma region	カメラ初期化
	camera = std::make_unique<NCamera>();
	camera->ProjectiveProjection();
	camera->SetEye({0,100.0f,-800.0f});
	camera->CreateMatView();
#pragma endregion
#pragma region 描画初期化処理
	//マテリアル(定数バッファ)
	material.Initialize(dx12->GetDevice());

	//立方体情報
	cube = std::make_unique<NCube>();
	cube->Initialize(dx12->GetDevice(), CUBE);
#pragma region オブジェクトの初期値設定
	//オブジェクト(定数バッファ)
	player = std::make_unique<NObj3d>();
	player->Initialize(dx12->GetDevice());
	player->texNum = PLAYER;
	player->scale = { 24.0f,24.0f,24.0f };
	player->position = { 0.0f,-100.0f,0.0f };
	//設定したのを適用
	player->UpdateMatrix(camera->GetMatView(), camera->GetMatProjection());
#pragma endregion
	////背景スプライト生成
	//for (size_t i = 0; i < maxBackSprite; i++)
	//{
	//	backSprite[i] = new NSprite();
	//	backSprite[i]->texNum = static_cast<int>(i);
	//	backSprite[i]->CreateSprite(dx12->GetDevice(), tex[backSprite[i]->texNum].texBuff);
	//	//sprite[i]->CreateClipSprite(dx12->GetDevice(),tex[sprite[i]->texNum].texBuff,{100.0f,0},{50.0f,100.0f});	//一部切り取って生成
	//	backSprite[i]->position.x = i * 300.0f + 375.0f;
	//	backSprite[i]->position.y = 375.0f;
	//	backSprite[i]->UpdateMatrix();
	//	backSprite[i]->TransferMatrix();
	//}

	//前景スプライト生成
	titleSprite = std::make_unique<NSprite>();
	titleSprite->texNum = TITLE;
	titleSprite->CreateSprite(dx12->GetDevice(), NSceneManager::GetTex()[titleSprite->texNum].texBuff);
	titleSprite->position = { NWindows::win_width / 2,NWindows::win_height / 2,0 };
	titleSprite->UpdateMatrix();

	startSprite = std::make_unique<NSprite>();
	startSprite->texNum = START;
	startSprite->CreateSprite(dx12->GetDevice(), NSceneManager::GetTex()[titleSprite->texNum].texBuff);
	startSprite->position = { NWindows::win_width / 2,NWindows::win_height / 2 - 50,0 };
	startSprite->UpdateMatrix();

#pragma endregion

}

void NTitleScene::Update()
{
	if (NSceneManager::GetPlayEffect() == false)
	{
		if (NInput::IsKeyTrigger(DIK_SPACE))
		{
			if (audio->IsPlaying(soundData[0]))
			{
				audio->StopWave(soundData[0]);
			}
			NSceneManager::SetScene(STAGESELECTSCENE);
		}
	}
#pragma region 行列の計算
	player->rotation.z += 18.0f;
	player->UpdateMatrix(camera->GetMatView(), camera->GetMatProjection());

	camera->CreateMatView();
#pragma endregion

}

void NTitleScene::Draw(NDX12* dx12)
{
#pragma region グラフィックスコマンド
	// 4.描画コマンドここから
	////背景スプライト
	//for (size_t i = 0; i < maxBackSprite; i++)
	//{
	//	backSprite[i]->CommonBeginDraw(dx12->GetCommandList(), NSceneManager::GetPipelineSprite()->pipelineSet.pipelineState, NSceneManager::GetPipelineSprite()->pipelineSet.rootSig.entity, dx12->GetSRVHeap());
	//	backSprite[i]->Draw(dx12->GetSRVHeap(), tex[0].incrementSize, dx12->GetCommandList());
	//}

	//3Dオブジェクト
	player->CommonBeginDraw(dx12->GetCommandList(), NSceneManager::GetPipeline3d()->pipelineSet.pipelineState,
		NSceneManager::GetPipeline3d()->pipelineSet.rootSig.entity, dx12->GetSRVHeap());
	player->Draw(dx12->GetCommandList(), material, dx12->GetSRVHeap(), cube->vbView, cube->ibView, cube->numIB, NSceneManager::GetTex()[0].incrementSize);

	//前景スプライト
	titleSprite->CommonBeginDraw(dx12->GetCommandList(), NSceneManager::GetPipelineSprite()->pipelineSet.pipelineState,
		NSceneManager::GetPipelineSprite()->pipelineSet.rootSig.entity, dx12->GetSRVHeap());
	titleSprite->Draw(dx12->GetSRVHeap(), NSceneManager::GetTex()[0].incrementSize, dx12->GetCommandList());

	startSprite->CommonBeginDraw(dx12->GetCommandList(), NSceneManager::GetPipelineSprite()->pipelineSet.pipelineState,
		NSceneManager::GetPipelineSprite()->pipelineSet.rootSig.entity, dx12->GetSRVHeap());
	startSprite->Draw(dx12->GetSRVHeap(), NSceneManager::GetTex()[0].incrementSize, dx12->GetCommandList());

	// 4.描画コマンドここまで
#pragma endregion
}

void NTitleScene::Reset()
{
	audio->StartWave(soundData[0]);
}

void NTitleScene::Finalize()
{
}
