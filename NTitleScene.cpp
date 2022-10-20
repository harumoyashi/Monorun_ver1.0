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
#pragma region 描画初期化処理
	//マテリアル(定数バッファ)
	material.Initialize(dx12->GetDevice());

	//オブジェクト(定数バッファ)
	for (size_t i = 0; i < maxObj; i++)
	{
		obj3d[i].Initialize(dx12->GetDevice());
		obj3d[i].texNum = 1;
	}

	obj3d[0].scale = { 24.0f,24.0f,24.0f };
	obj3d[0].position = { 0.0f,-100.0f,0.0f };
#pragma region 行列の計算
	/*XMMATRIX oldVer = XMMatrixIdentity();
	oldVer.r[0].m128_f32[0] = 2.0f / window_width;
	oldVer.r[1].m128_f32[1] = -2.0f / window_height;

	oldVer.r[3].m128_f32[0] = -1.0f;
	oldVer.r[3].m128_f32[1] = 1.0f;

	XMMATRIX newVer = XMMatrixOrthographicOffCenterLH(
		0,window_width,
		window_height,0,
		0.0f, 1.0f
	);*/

	//平行投影変換//
	//単位行列を代入
	//material.constMapTransform->mat = XMMatrixIdentity();

	/*constMapTransform->mat.r[0].m128_f32[0] = 2.0f / window_width;
	constMapTransform->mat.r[1].m128_f32[1] = -2.0f / window_height;

	constMapTransform->mat.r[3].m128_f32[0] = -1.0f;
	constMapTransform->mat.r[3].m128_f32[1] = 1.0f;*/

	//平行投影変換
	/*material.constMapTransform->mat = XMMatrixOrthographicOffCenterLH(
		0, win_width,
		win_height, 0,
		0.0f, 1.0f
	);*/

	//透視投影変換//
	//material.constMapTransform->mat = XMMatrixPerspectiveFovLH(
	//	XMConvertToRadians(45.0f),		//上下画角45度
	//	(float)win_width / win_height,	//アスペクト比(画面横幅/画面縦幅)
	//	0.1f, 1000.0f					//前端、奥端
	//);


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
	for (size_t i = 0; i < maxForeSprite; i++)
	{
		foreSprite[i] = new NSprite();
		foreSprite[i]->texNum = static_cast<int>(i);
		foreSprite[i]->CreateSprite(dx12->GetDevice(), NSceneManager::GetTex()[foreSprite[i]->texNum].texBuff);
		//sprite[i]->CreateClipSprite(dx12->GetDevice(),tex[sprite[i]->texNum].texBuff,{100.0f,0},{50.0f,100.0f});	//一部切り取って生成
		foreSprite[i]->TransferVertex();
		foreSprite[i]->position.x = NWindows::win_width / 2;
		foreSprite[i]->position.y = NWindows::win_height / 2;
		foreSprite[i]->UpdateMatrix();
		foreSprite[i]->TransferMatrix();
	}
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
	eye = { -100, 0, -1000 };	//視点座標
	target = { 0, 0, 0 };	//注視点座標
	up = { 0, 1, 0 };		//上方向ベクトル
	//ビュー変換行列作成
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));

	angle = 0.0f;	//カメラの回転角
#pragma endregion
}

void NTitleScene::Update()
{
	if (NInput::IsKeyTrigger(DIK_SPACE))
	{
		NSceneManager::SetScene(STAGESELECTSCENE);
	}
#pragma region 行列の計算
	obj3d[0].rotation.z+=0.3f;
	for (size_t i = 0; i < maxObj; i++)
	{
		obj3d[i].UpdateMatrix();
		obj3d[i].TransferMatrix(matView, matProjection);
	}
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
	for (size_t i = 0; i < maxObj; i++)
	{
		obj3d[i].CommonBeginDraw(dx12->GetCommandList(), NSceneManager::GetPipeline3d()->pipelineSet.pipelineState, NSceneManager::GetPipeline3d()->pipelineSet.rootSig.entity, dx12->GetSRVHeap());
		obj3d[i].Draw(dx12->GetCommandList(), material, dx12->GetSRVHeap(), NSceneManager::GetPipeline3d()->vbView, NSceneManager::GetPipeline3d()->ibView, NSceneManager::GetPipeline3d()->numIB, NSceneManager::GetTex()[0].incrementSize);
	}

	//前景スプライト
	for (size_t i = 0; i < maxForeSprite; i++)
	{
		foreSprite[i]->CommonBeginDraw(dx12->GetCommandList(), NSceneManager::GetPipelineSprite()->pipelineSet.pipelineState, NSceneManager::GetPipelineSprite()->pipelineSet.rootSig.entity, dx12->GetSRVHeap());
		foreSprite[i]->Draw(dx12->GetSRVHeap(), NSceneManager::GetTex()[0].incrementSize, dx12->GetCommandList());
	}
	// 4.描画コマンドここまで
#pragma endregion
}

void NTitleScene::Finalize()
{
	for (size_t i = 0; i < maxForeSprite; i++)
	{
		delete foreSprite[i];
	}

	/*for (size_t i = 0; i < maxBackSprite; i++)
	{
		delete backSprite[i];
	}*/
}
