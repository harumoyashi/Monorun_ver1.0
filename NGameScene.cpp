#include "NGameScene.h"

void NGameScene::Initialize(NDX12* dx12)
{
#pragma region 描画初期化処理
	//ルートパラメータの設定
	rootParams.SetDescRange();
	rootParams.SetRootParam();

	//マテリアル(定数バッファ)
	material.Initialize(dx12->GetDevice());

	//オブジェクト(定数バッファ)
	for (size_t i = 0; i < maxObj; i++)
	{
		obj3d[i].Initialize(dx12->GetDevice());
	}
	obj3d[0].texNum = 0;
	obj3d[1].texNum = 1;
	obj3d[2].texNum = 2;

	obj3d[1].position.x = -20.0f;
	obj3d[2].position.x = 20.0f;
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

	//射影投影変換//
	matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45.0f),		//上下画角45度
		(float)NWindows::win_width / NWindows::win_height,	//アスペクト比(画面横幅/画面縦幅)
		0.1f, 1000.0f					//前端、奥端
	);

	//ここでビュー変換行列計算
	matView;
	eye = { 0, 0, -100 };	//視点座標
	target = { 0, 0, 0 };	//注視点座標
	up = { 0, 1, 0 };		//上方向ベクトル
	//ビュー変換行列作成
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));

	angle = 0.0f;	//カメラの回転角
#pragma endregion
	//テクスチャ生成
	tex[0].Load(L"Resources/mario.jpg");
	tex[1].Load(L"Resources/itiro_kimegao.png");
	tex[2].Load(L"Resources/hamutaro.jpg");

	for (int i = 0; i < maxTex; i++)
	{
		tex[i].CreateMipmap();
		//テクスチャバッファ設定
		tex[i].SetTBHeap();
		tex[i].SetTBResource();
		tex[i].CreateTexBuff(dx12->GetDevice());
		tex[i].TexBuffMaping();
		//シェーダーリソースビュー設定
		tex[i].SetSRV();
		tex[i].CreateSRV(dx12->GetDevice(), dx12->GetSRVHeap(), i);
	}

	//背景スプライト生成
	for (size_t i = 0; i < maxBackSprite; i++)
	{
		backSprite[i] = new NSprite();
		backSprite[i]->texNum = static_cast<int>(i);
		backSprite[i]->CreateSprite(dx12->GetDevice(), tex[backSprite[i]->texNum].texBuff);
		//sprite[i]->CreateClipSprite(dx12->GetDevice(),tex[sprite[i]->texNum].texBuff,{100.0f,0},{50.0f,100.0f});	//一部切り取って生成
		backSprite[i]->position.x = i * 300.0f + 400.0f;
		backSprite[i]->position.y = 400.0f;
		backSprite[i]->UpdateMatrix();
		backSprite[i]->TransferMatrix();
	}

	//前景スプライト生成
	for (size_t i = 0; i < maxForeSprite; i++)
	{
		foreSprite[i] = new NSprite();
		foreSprite[i]->texNum = static_cast<int>(i);
		foreSprite[i]->CreateSprite(dx12->GetDevice(), tex[backSprite[i]->texNum].texBuff);
		//sprite[i]->CreateClipSprite(dx12->GetDevice(),tex[sprite[i]->texNum].texBuff,{100.0f,0},{50.0f,100.0f});	//一部切り取って生成
		foreSprite[i]->SetColor(1, 1, 1, 0.5f);
		foreSprite[i]->size = { 150,150 };
		foreSprite[i]->TransferVertex();
		foreSprite[i]->position.x = i * 200.0f + 200.0f;
		foreSprite[i]->position.y = 200.0f;
		foreSprite[i]->UpdateMatrix();
		foreSprite[i]->TransferMatrix();
	}

#pragma region グラフィックスパイプライン
	gPipe3d = new NGPipeline();
	gPipe3d->Initialize3d(dx12->GetDevice());
	gPipe3d->pipelineSet = gPipe3d->CreatePipeline3d(dx12->GetDevice(), rootParams.entity);

	gPipeSprite = new NGPipeline();
	gPipeSprite->pipelineSet = gPipeSprite->CreatePipelineSprite(dx12->GetDevice(), rootParams.entity);
#pragma endregion
#pragma endregion
}

void NGameScene::Update()
{
#pragma region 行列の計算
	//ビュー変換行列再作成
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));

	obj3d[0].MoveKey();
	for (size_t i = 0; i < maxObj; i++)
	{
		obj3d[i].UpdateMatrix();
		obj3d[i].TransferMatrix(matView, matProjection);
	}

	//スプライトの変換テスト
	increment += 1.0f;
	for (size_t i = 0; i < maxForeSprite; i++)
	{
		//foreSprite[i]->rotation = increment;
		//foreSprite[i]->position.x = increment;

		//foreSprite[i]->UpdateMatrix();
		//foreSprite[i]->TransferMatrix();*/
		//foreSprite[i]->SetColor(1, 1, 1, increment*0.01f);

		//foreSprite[i]->size = { increment,i * 100.0f + 100.0f };
		//foreSprite[i]->TransferVertex();

		//if (increment > 300.0f)
		//{
		//	foreSprite[i]->isInvisible = true;
		//}
	}
#pragma endregion
}

void NGameScene::Draw(NDX12* dx12)
{
	HRESULT result;

#pragma region 描画前処理
	NPreDraw* preDraw = nullptr;
	preDraw = new NPreDraw();

	preDraw->SetResBarrier(dx12->GetSwapchain(), dx12->backBuffers, dx12->GetCommandList());
	preDraw->SetRenderTarget(dx12->GetRTVHeap(), dx12->GetDevice(), dx12->GetRTVHeapDesc(), dx12->GetDSVHeap(), dx12->GetCommandList());
	preDraw->ClearScreen(dx12->GetCommandList());
#pragma endregion
#pragma region グラフィックスコマンド
	// 4.描画コマンドここから
	preDraw->SetViewport(dx12->GetCommandList());
	preDraw->SetScissorRect(dx12->GetCommandList());

	//背景スプライト
	for (size_t i = 0; i < maxBackSprite; i++)
	{
		backSprite[i]->CommonBeginDraw(dx12->GetCommandList(), gPipeSprite->pipelineSet.pipelineState, gPipeSprite->pipelineSet.rootSig.entity, dx12->GetSRVHeap());
		backSprite[i]->Draw(dx12->GetSRVHeap(), tex[0].incrementSize, dx12->GetCommandList());
	}

	//3Dオブジェクト
	for (size_t i = 0; i < maxObj; i++)
	{
		obj3d[i].CommonBeginDraw(dx12->GetCommandList(), gPipe3d->pipelineSet.pipelineState, gPipe3d->pipelineSet.rootSig.entity, dx12->GetSRVHeap());
		obj3d[i].Draw(dx12->GetCommandList(), material, dx12->GetSRVHeap(), gPipe3d->vbView, gPipe3d->ibView, gPipe3d->numIB, tex[0].incrementSize);
	}

	//前景スプライト
	for (size_t i = 0; i < maxForeSprite; i++)
	{
		foreSprite[i]->CommonBeginDraw(dx12->GetCommandList(), gPipeSprite->pipelineSet.pipelineState, gPipeSprite->pipelineSet.rootSig.entity, dx12->GetSRVHeap());
		foreSprite[i]->Draw(dx12->GetSRVHeap(), tex[0].incrementSize, dx12->GetCommandList());
	}
	// 4.描画コマンドここまで
#pragma endregion
	dx12->PostDraw(preDraw->barrierDesc);
}

void NGameScene::Finalize()
{
	for (size_t i = 0; i < maxForeSprite; i++)
	{
		delete foreSprite[i];
	}

	for (size_t i = 0; i < maxBackSprite; i++)
	{
		delete backSprite[i];
	}

	delete gPipe3d;
	delete gPipeSprite;
}