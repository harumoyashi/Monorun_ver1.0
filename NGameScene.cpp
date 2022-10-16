#include "NGameScene.h"

void NGameScene::Initialize(NDX12* dx12)
{
#pragma region 描画初期化処理
	//ルートパラメータの設定
	rootParams.SetDescRange();
	rootParams.SetRootParam();

	//マテリアル(定数バッファ)
	material.Initialize(dx12->device);

	//オブジェクト(定数バッファ)
	for (size_t i = 0; i < maxObj; i++)
	{
		obj3d[i].Initialize(dx12->device);
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
		(float)win_width / win_height,	//アスペクト比(画面横幅/画面縦幅)
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
		tex[i].CreateTexBuff(dx12->device);
		tex[i].TexBuffMaping();
		//シェーダーリソースビュー設定
		tex[i].SetSRV();
		tex[i].CreateSRV(dx12->device, dx12->srvHeap, i);
	}

	//スプライト生成
	for (size_t i = 0; i < maxSprite; i++)
	{
		sprite[i] = new NSprite();
		sprite[i]->texNum = i;
		sprite[i]->CreateSprite(dx12->device, tex[sprite[i]->texNum].texBuff);
		//sprite[i]->CreateClipSprite(dx12->device,tex[sprite[i]->texNum].texBuff,{100.0f,0},{50.0f,100.0f});	//一部切り取って生成
		sprite[i]->position.x = i * 200.0f + 200.0f;
		sprite[i]->position.y = 300.0f;
		sprite[i]->UpdateMatrix();
		sprite[i]->TransferMatrix();
	}

#pragma region グラフィックスパイプライン
	gPipe3d = new NGPipeline();
	gPipe3d->Initialize3d(dx12->device);
	gPipe3d->pipelineSet = gPipe3d->CreatePipeline3d(dx12->device, rootParams.entity);

	gPipeSprite = new NGPipeline();
	gPipeSprite->pipelineSet = gPipeSprite->CreatePipelineSprite(dx12->device, rootParams.entity);
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

	////スプライトの変換テスト
	//increment += 1.0f;
	//for (size_t i = 0; i < maxSprite; i++)
	//{
	//	sprite[i]->rotation = increment;
	//	sprite[i]->position.x = increment;

	//	sprite[i]->UpdateMatrix();
	//	sprite[i]->TransferMatrix();
	//	sprite[i]->SetColor(1, 0, 1, 1);

	//	sprite[i]->size = { increment,i * 100.0f + 100.0f };
	//	sprite[i]->TransferVertex();

	//	if (increment > 300.0f)
	//	{
	//		sprite[i]->isInvisible = true;
	//	}
	//}
#pragma endregion
}

void NGameScene::Draw(NDX12* dx12)
{
	HRESULT result;

#pragma region 描画前処理
	NPreDraw* preDraw = nullptr;
	preDraw = new NPreDraw();

	preDraw->SetResBarrier(dx12->swapchain, dx12->backBuffers, dx12->GetCommandList());
	preDraw->SetRenderTarget(dx12->rtvHeap, dx12->device, dx12->rtvHeapDesc, dx12->dsvHeap, dx12->GetCommandList());
	preDraw->ClearScreen(dx12->GetCommandList());
#pragma endregion
#pragma region グラフィックスコマンド
	// 4.描画コマンドここから
	preDraw->SetViewport(dx12->GetCommandList());
	preDraw->SetScissorRect(dx12->GetCommandList());

	//3Dオブジェクト
	for (size_t i = 0; i < maxObj; i++)
	{
		obj3d[i].CommonBeginDraw(dx12->GetCommandList(), gPipe3d->pipelineSet.pipelineState, gPipe3d->pipelineSet.rootSig.entity, dx12->srvHeap);
		obj3d[i].Draw(dx12->GetCommandList(), material, dx12->srvHeap, gPipe3d->vbView, gPipe3d->ibView, gPipe3d->sizeIB, tex[0].incrementSize);
	}

	//スプライト
	for (size_t i = 0; i < maxSprite; i++)
	{
		sprite[i]->CommonBeginDraw(dx12->GetCommandList(), gPipeSprite->pipelineSet.pipelineState, gPipeSprite->pipelineSet.rootSig.entity, dx12->srvHeap);
		sprite[i]->Draw(dx12->srvHeap, tex[0].incrementSize, dx12->GetCommandList());
	}
	// 4.描画コマンドここまで
#pragma endregion
	/*NPostDraw* postDraw = nullptr;
	postDraw = new NPostDraw;
	preDraw->BarrierReset(preDraw->barrierDesc, dx12->GetCommandList());
	preDraw->CmdListClose(dx12->GetCommandList());
	preDraw->ExecuteCmdList(dx12->GetCommandList(), dx12->commandQueue);
	preDraw->BufferSwap(dx12->swapchain);
	preDraw->CommandWait(dx12->commandQueue, dx12->GetFence(), dx12->fenceVal);
	preDraw->ClearQueue(dx12->GetCommandAllocator());
	preDraw->CmdListReset(dx12->GetCommandList(), dx12->GetCommandAllocator());*/

	// 5.リソースバリアを戻す
	preDraw->barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET; // 描画状態から
	preDraw->barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT; // 表示状態へ
	dx12->GetCommandList()->ResourceBarrier(1, &preDraw->barrierDesc);

	// 命令のクローズ
	result = dx12->GetCommandList()->Close();
	assert(SUCCEEDED(result));

	// コマンドリストの実行
	ID3D12CommandList* commandLists[] = { dx12->GetCommandList() };
	dx12->commandQueue->ExecuteCommandLists(1, commandLists);

	// 画面に表示するバッファをフリップ(裏表の入替え)
	result = dx12->swapchain->Present(1, 0);
	assert(SUCCEEDED(result));

	// コマンドの実行完了を待つ
	dx12->commandQueue->Signal(dx12->GetFence(), ++dx12->fenceVal);
	if (dx12->GetFence()->GetCompletedValue() != dx12->fenceVal) {
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		dx12->GetFence()->SetEventOnCompletion(dx12->fenceVal, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}

	// キューをクリア
	result = dx12->GetCommandAllocator()->Reset();
	assert(SUCCEEDED(result));

	// 再びコマンドリストを貯める準備
	result = dx12->GetCommandList()->Reset(dx12->GetCommandAllocator(), nullptr);
	assert(SUCCEEDED(result));
}