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
	//soundData[0] = audio->LoadWave("preparation.wav");
	//soundData[1] = audio->LoadWave("mokugyo.wav");
	//soundData[2] = audio->LoadWave("fanfare.wav");
	////BGM鳴らす
	//audio->PlayWave(soundData[0], true);
#pragma endregion
#pragma region	カメラ初期化
	//射影投影変換//
	matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45.0f),		//上下画角45度
		(float)NWindows::win_width / NWindows::win_height,	//アスペクト比(画面横幅/画面縦幅)
		0.1f, 2000.0f					//前端、奥端
	);

	//ここでビュー変換行列計算
	matView;
	eye = { -100, 100, -1000 };	//視点座標
	target = { 0, 0, 0 };	//注視点座標
	up = { 0, 1, 0 };		//上方向ベクトル
	//ビュー変換行列作成
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));

	angle = 0.0f;	//カメラの回転角
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
	player->UpdateMatrix(matView, matProjection);
#pragma endregion
	////背景スプライト生成
	//for (size_t i = 0; i < maxBackSprite; i++)
	//{
	//	backSprite[i] = new NSprite();
	//	backSprite[i]->texNum = static_cast<int>(i);
	//	backSprite[i]->CreateSprite(dx12->GetDevice(), tex[backSprite[i]->texNum].texBuff);
	//	//sprite[i]->CreateClipSprite(dx12->GetDevice(),tex[sprite[i]->texNum].texBuff,{100.0f,0},{50.0f,100.0f});	//一部切り取って生成
	//	backSprite[i]->position.x = i * 300.0f + 400.0f;
	//	backSprite[i]->position.y = 400.0f;
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
	startSprite->position = { NWindows::win_width / 2,NWindows::win_height / 2,0 };
	startSprite->UpdateMatrix();

#pragma endregion
}

void NTitleScene::Update()
{
	if (NInput::IsKeyTrigger(DIK_SPACE))
	{
		NSceneManager::SetScene(STAGESELECTSCENE);
	}
#pragma region 行列の計算
	player->rotation.z += 0.3f;
	player->UpdateMatrix(matView, matProjection);
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

void NTitleScene::Finalize()
{
}
