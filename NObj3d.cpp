#include "NObj3d.h"
#include "NInput.h"

void NObj3d::Initialize(ComPtr<ID3D12Device> device)
{
	//定数バッファ
	SetCBHeap();
	SetCBResource();
	CreateCB(device);
	MappingCB();
}

void NObj3d::SetCBHeap()
{
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPUへの転送用
}

void NObj3d::SetCBResource()
{
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = (sizeof(constMapTransform) + 0xff) & ~0xff;	//256バイトアラインメント
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
}

void NObj3d::CreateCB(ComPtr<ID3D12Device> device)
{
	HRESULT result;

	result = device->CreateCommittedResource(
		&heapProp,	//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc,	//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffTransform)
	);
	assert(SUCCEEDED(result));
}

void NObj3d::MappingCB()
{
	HRESULT result;

	result = constBuffTransform->Map(0, nullptr, (void**)&constMapTransform);	//マッピング
	assert(SUCCEEDED(result));
}

void NObj3d::MoveKey()
{
	//いずれかのキーを押したとき
	if (NInput::IsKeyDown(DIK_W) || NInput::IsKeyDown(DIK_S) || NInput::IsKeyDown(DIK_D) || NInput::IsKeyDown(DIK_A))
	{
		if (NInput::IsKeyDown(DIK_W)) { position.y += 5.0f; }
		else if (NInput::IsKeyDown(DIK_S)) { position.y -= 5.0f; }
		if (NInput::IsKeyDown(DIK_D)) { position.x += 5.0f; }
		else if (NInput::IsKeyDown(DIK_A)) { position.x -= 5.0f; }
	}
}

void NObj3d::UpdateMatrix(XMMATRIX matView, XMMATRIX matProjection)
{
	//ワールド行列
	XMMATRIX matScale;	//スケーリング行列
	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);

	XMMATRIX matRot = XMMatrixIdentity();		//回転行列
	matRot *= XMMatrixRotationZ(rotation.z);	//Z軸周りに0度回転してから
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));	//X軸周りに15度回転してから
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));	//Y軸周りに30度回転

	XMMATRIX matTrans;	//平行移動行列
	matTrans = XMMatrixTranslation(position.x, position.y, position.z);

	matWorld = XMMatrixIdentity();	//単位行列代入
	matWorld *= matScale;	//ワールド座標にスケーリングを反映
	matWorld *= matRot;	//ワールド座標に回転を反映
	matWorld *= matTrans;	//ワールド座標に平行移動を反映

	//親オブジェクトがあれば
	if (parent != nullptr)
	{
		//親オブジェクトのワールド行列をかける
		matWorld *= parent->matWorld;
	}

	TransferMatrix(matView, matProjection);
}

void NObj3d::TransferMatrix(XMMATRIX matView, XMMATRIX matProjection)
{
	constMapTransform->mat = matWorld * matView * matProjection;
}

void NObj3d::CommonBeginDraw(ComPtr<ID3D12GraphicsCommandList> commandList, const ComPtr<ID3D12PipelineState> pipelineState, ComPtr<ID3D12RootSignature> rootSignature, ComPtr<ID3D12DescriptorHeap> srvHeap)
{
	// パイプラインステートとルートシグネチャの設定コマンド
	commandList->SetPipelineState(pipelineState.Get());
	commandList->SetGraphicsRootSignature(rootSignature.Get());

	// プリミティブ形状の設定コマンド
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト

	ID3D12DescriptorHeap* ppHeaps[] = { srvHeap.Get() };
	commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
}

void NObj3d::Draw(ComPtr<ID3D12GraphicsCommandList> commandList, NMaterial material, ComPtr<ID3D12DescriptorHeap> srvHeap,
	D3D12_VERTEX_BUFFER_VIEW vbView, D3D12_INDEX_BUFFER_VIEW ibView, UINT indexSize, UINT incrementSize)
{
	SetMaterialCBV(commandList, material);
	SetSRVHeap(srvHeap, commandList, incrementSize);
	SetVB(commandList, vbView);
	SetIB(commandList, ibView);
	SetMatCBV(commandList);
	DrawCommand(commandList, indexSize);
}

void NObj3d::SetMaterialCBV(ComPtr<ID3D12GraphicsCommandList> commandList, NMaterial material)
{
	material.SetCBV(commandList);
}

void NObj3d::SetSRVHeap(ComPtr<ID3D12DescriptorHeap> srvHeap, ComPtr<ID3D12GraphicsCommandList> commandList, UINT incrementSize)
{
	//シェーダリソースビュー(SRV)ヒープの先頭ハンドルを取得(SRVを指してるはず)
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = srvHeap->GetGPUDescriptorHandleForHeapStart();
	//ハンドルを指定のとこまで進める
	srvGpuHandle.ptr += incrementSize * texNum;
	//指定のヒープにあるSRVをルートパラメータ1番に設定
	commandList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);
}

void NObj3d::SetVB(ComPtr<ID3D12GraphicsCommandList> commandList, D3D12_VERTEX_BUFFER_VIEW vbView)
{
	commandList->IASetVertexBuffers(0, 1, &vbView);
}

void NObj3d::SetIB(ComPtr<ID3D12GraphicsCommandList> commandList, D3D12_INDEX_BUFFER_VIEW ibView)
{
	commandList->IASetIndexBuffer(&ibView);
}

void NObj3d::SetMatCBV(ComPtr<ID3D12GraphicsCommandList> commandList)
{
	//ルートパラメータ2番に3D変換行列の定数バッファを渡す
	commandList->SetGraphicsRootConstantBufferView(2, constBuffTransform->GetGPUVirtualAddress());
}

void NObj3d::DrawCommand(ComPtr<ID3D12GraphicsCommandList> commandList, UINT indexSize)
{
	commandList->DrawIndexedInstanced(indexSize, 1, 0, 0, 0); //インデックスを使って描画
}
