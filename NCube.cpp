#include "NCube.h"

//静的メンバ変数の実体
std::vector<Vertex> NCube::vertices;
std::vector<unsigned short>NCube::indices;


void NCube::Initialize(ComPtr<ID3D12Device> device)
{
	//頂点
	SetVert();
	SetIndex();	//ここでインデックスの設定だけ行う
	SetNormal();
	CreateVertBuff(device);
	VertMaping();
	CreateVertBuffView();

	//インデックスデータ
	CreateIndexBuff(device);
	IndexMaping();
	CreateIndexBuffView();
}

void NCube::SetVert()
{
	// 頂点データ設定
	vertices = {
		//	x		y		z	 法線	u	v
		//前
		{{ -5.0f, -5.0f, -5.0f }, {}, {0.0f,1.0f}},	// 左下
		{{ -5.0f,  5.0f, -5.0f }, {}, {0.0f,0.0f}},	// 左上
		{{  5.0f, -5.0f, -5.0f }, {}, {1.0f,1.0f}},	// 右下
		{{  5.0f,  5.0f, -5.0f }, {}, {1.0f,0.0f}},	// 右上

		//後
		{{ -5.0f, -5.0f, 5.0f }, {}, {0.0f,1.0f}},	// 左下
		{{ -5.0f,  5.0f, 5.0f }, {}, {0.0f,0.0f}},	// 左上
		{{  5.0f, -5.0f, 5.0f }, {}, {1.0f,1.0f}},	// 右下
		{{  5.0f,  5.0f, 5.0f }, {}, {1.0f,0.0f}},	// 右上

		 // 左
		{{-5.0f,-5.0f,-5.0f }, {}, {0.0f, 1.0f}},    // 左下
		{{-5.0f,-5.0f, 5.0f }, {}, {0.0f, 0.0f}},    // 左上
		{{-5.0f, 5.0f,-5.0f }, {}, {1.0f, 1.0f}},    // 右下
		{{-5.0f, 5.0f, 5.0f }, {}, {1.0f, 0.0f}},    // 右上

		// 右
		{{ 5.0f,-5.0f,-5.0f }, {}, {0.0f, 1.0f}},    // 左下
		{{ 5.0f,-5.0f, 5.0f }, {}, {0.0f, 0.0f}},    // 左上
		{{ 5.0f, 5.0f,-5.0f }, {}, {1.0f, 1.0f}},    // 右下
		{{ 5.0f, 5.0f, 5.0f }, {}, {1.0f, 0.0f}},    // 右上

		// 上
		{{-5.0f,-5.0f,-5.0f }, {}, {0.0f, 1.0f}},    // 左下
		{{ 5.0f,-5.0f,-5.0f }, {}, {0.0f, 0.0f}},    // 左上
		{{-5.0f,-5.0f, 5.0f }, {}, {1.0f, 1.0f}},    // 右下
		{{ 5.0f,-5.0f, 5.0f }, {}, {1.0f, 0.0f}},    // 右上

		// 下
		{{-5.0f, 5.0f,-5.0f }, {}, {0.0f, 1.0f}},    // 左下
		{{ 5.0f, 5.0f,-5.0f }, {}, {0.0f, 0.0f}},    // 左上
		{{-5.0f, 5.0f, 5.0f }, {}, {1.0f, 1.0f}},    // 右下
		{{ 5.0f, 5.0f, 5.0f }, {}, {1.0f, 0.0f}},    // 右上
	};

	////設定したら他でも使える変数に代入
	//std::copy(std::begin(vert), std::end(vert), vertices);

	//頂点バッファのサイズを代入
	singleSizeVB = static_cast<UINT>(sizeof(vertices[0]));
	sizeVB = static_cast<UINT>(sizeof(Vertex) * vertices.size());
}

void NCube::CreateVertBuff(ComPtr<ID3D12Device> device)
{
	HRESULT result;

	// ヒーププロパティ
	CD3DX12_HEAP_PROPERTIES heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// リソース設定
	CD3DX12_RESOURCE_DESC resDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeVB);

	// 頂点バッファの生成
	result = device->CreateCommittedResource(
		&heapProp, // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));
}

void NCube::VertMaping()
{
	HRESULT result;

	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	// 全頂点に対して
	std::copy(std::begin(vertices), std::end(vertices), vertMap);	//座標をコピー
	// 繋がりを解除
	vertBuff->Unmap(0, nullptr);
}

void NCube::CreateVertBuffView()
{
	// 頂点バッファビューの作成
	// GPU仮想アドレス
	//これ渡すことで、GPU側がどのデータ見ればいいかわかるようになる
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	// 頂点バッファのサイズ
	vbView.SizeInBytes = sizeVB;
	// 頂点1つ分のデータサイズ
	vbView.StrideInBytes = singleSizeVB;
}

void NCube::SetIndex()
{
	//インデックスデータ
	indices =
	{
		//前
		0,1,2,	//三角形1つ目
		2,1,3,	//三角形2つ目
		//後
		5,4,6,	//三角形3つ目
		5,6,7,	//三角形4つ目
		//左
		8,9,10,	//三角形5つ目
		10,9,11,//三角形6つ目
		//右
		13,12,14,	//三角形7つ目
		13,14,15,	//三角形8つ目
		//下
		16,17,18,	//三角形9つ目
		18,17,19,	//三角形10つ目
		//上
		21,20,22,	//三角形11つ目
		21,22,23,	//三角形12つ目
	};

	////設定したら他でも使える変数に代入
	//std::copy(std::begin(index), std::end(index), indices);

	//頂点バッファのサイズを代入
	sizeIB = static_cast<UINT>(sizeof(unsigned short) * indices.size());
	numIB = static_cast<UINT>(sizeof(unsigned short) * indices.size() / sizeof(indices[0]));
}

void NCube::SetNormal()
{
	for (int i = 0; i < numIB / 3; i++)
	{//三角形1つごとに計算していく
		//三角形のインデックスを取り出して、一時的な変数にいれる
		unsigned short index0 = indices[i * 3 + 0];
		unsigned short index1 = indices[i * 3 + 1];
		unsigned short index2 = indices[i * 3 + 2];

		//三角形を構成する頂点座標をベクトルに代入
		XMVECTOR p0 = XMLoadFloat3(&vertices[index0].pos);
		XMVECTOR p1 = XMLoadFloat3(&vertices[index1].pos);
		XMVECTOR p2 = XMLoadFloat3(&vertices[index2].pos);

		//p0→p1ベクトル、p0→p2ベクトルを計算(ベクトルの減算)
		XMVECTOR v1 = XMVectorSubtract(p1, p0);
		XMVECTOR v2 = XMVectorSubtract(p2, p0);

		//外積は両方から垂直なベクトル
		XMVECTOR normal = XMVector3Cross(v1, v2);

		//正規化
		normal = XMVector3Normalize(normal);

		//求めた法線を頂点データに代入
		XMStoreFloat3(&vertices[index0].normal, normal);
		XMStoreFloat3(&vertices[index1].normal, normal);
		XMStoreFloat3(&vertices[index2].normal, normal);
	}
}

void NCube::CreateIndexBuff(ComPtr<ID3D12Device> device)
{
	HRESULT result;

	// ヒーププロパティ
	CD3DX12_HEAP_PROPERTIES heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// リソース設定
	CD3DX12_RESOURCE_DESC resDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeIB);

	// 頂点バッファの生成
	result = device->CreateCommittedResource(
		&heapProp, // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff));
	assert(SUCCEEDED(result));
}

void NCube::IndexMaping()
{
	HRESULT result;

	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	assert(SUCCEEDED(result));
	// 全頂点に対して
	for (int i = 0; i < numIB; i++)
	{
		indexMap[i] = indices[i];	//インデックスをコピー
	}
	// 繋がりを解除
	indexBuff->Unmap(0, nullptr);
}

void NCube::CreateIndexBuffView()
{
	// GPU仮想アドレス
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	//インデックス1個分のサイズ
	ibView.Format = DXGI_FORMAT_R16_UINT;
	// インデックスバッファのサイズ
	ibView.SizeInBytes = sizeIB;
}