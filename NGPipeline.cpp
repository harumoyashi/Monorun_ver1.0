#include "NGPipeline.h"

void NGPipeline::Initialize3d(ComPtr<ID3D12Device> device)
{
	//頂点
	SetVert();
	SetVertHeap();
	SetVertResource();
	SetIndex();	//ここでインデックスの設定だけ行う
	SetNormal();
	CreateVertBuff(device);
	VertMaping();
	CreateVertBuffView();

	//インデックスデータ
	SetIndexResource();
	CreateIndexBuff(device);
	IndexMaping();
	CreateIndexBuffView();
}

void NGPipeline::SetVert()
{
	// 頂点データ設定
	Vertex vert[] = {
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

	//設定したら他でも使える変数に代入
	std::copy(std::begin(vert), std::end(vert), vertices);

	//頂点バッファのサイズを代入
	singleSizeVB = static_cast<UINT>(sizeof(vertices[0]));
	sizeVB = static_cast<UINT>(sizeof(vertices));
}

void NGPipeline::SetVertHeap()
{
	//ヒープ設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転送用
	heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
}

void NGPipeline::SetVertResource()
{
	//リソース設定
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB;			//頂点データ全体のサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.Format = DXGI_FORMAT_UNKNOWN;
	resDesc.SampleDesc.Count = 1;
	resDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
}

void NGPipeline::CreateVertBuff(ComPtr<ID3D12Device> device)
{
	HRESULT result;

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

void NGPipeline::VertMaping()
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

void NGPipeline::CreateVertBuffView()
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

void NGPipeline::SetIndex()
{
	//インデックスデータ
	unsigned short index[] =
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

	//設定したら他でも使える変数に代入
	std::copy(std::begin(index), std::end(index), indices);

	//頂点バッファのサイズを代入
	sizeIB = static_cast<UINT>(sizeof(uint16_t) * _countof(indices));
}

void NGPipeline::SetIndexResource()
{
	//リソース設定
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeIB;			//頂点データ全体のサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.Format = DXGI_FORMAT_UNKNOWN;
	resDesc.SampleDesc.Count = 1;
	resDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
}

void NGPipeline::SetNormal()
{
	for (int i = 0; i < _countof(indices) / 3; i++)
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

void NGPipeline::CreateIndexBuff(ComPtr<ID3D12Device> device)
{
	HRESULT result;

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

void NGPipeline::IndexMaping()
{
	HRESULT result;

	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	assert(SUCCEEDED(result));
	// 全頂点に対して
	for (int i = 0; i < _countof(indices); i++)
	{
		indexMap[i] = indices[i];	//インデックスをコピー
	}
	// 繋がりを解除
	indexBuff->Unmap(0, nullptr);
}

void NGPipeline::CreateIndexBuffView()
{
	// GPU仮想アドレス
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	//インデックス1個分のサイズ
	ibView.Format = DXGI_FORMAT_R16_UINT;
	// インデックスバッファのサイズ
	ibView.SizeInBytes = sizeIB;
}

void NGPipeline::LoadVertShader3d()
{
	HRESULT result;

	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/BasicVS.hlsl", // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "vs_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&vsBlob, &errorBlob);

	// エラーなら
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string error;
		error.resize(errorBlob->GetBufferSize());
		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(error.c_str());
		assert(0);
	}
}

void NGPipeline::LoadPixelShader3d()
{
	HRESULT result;

	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/BasicPS.hlsl", // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "ps_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&psBlob, &errorBlob);

	// エラーなら
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string error;
		error.resize(errorBlob->GetBufferSize());
		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(error.c_str());
		assert(0);
	}
}

void NGPipeline::LoadVertShaderSprite()
{
	HRESULT result;

	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/SpriteVS.hlsl", // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "vs_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&vsBlob, &errorBlob);

	// エラーなら
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string error;
		error.resize(errorBlob->GetBufferSize());
		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(error.c_str());
		assert(0);
	}
}

void NGPipeline::LoadPixelShaderSprite()
{
	HRESULT result;

	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/SpritePS.hlsl", // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "ps_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&psBlob, &errorBlob);

	// エラーなら
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string error;
		error.resize(errorBlob->GetBufferSize());
		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(error.c_str());
		assert(0);
	}
}

void NGPipeline::SetVertLayout3d()
{
	// 頂点レイアウト
	//座標
	vertLayout3d[0] = {
	"POSITION",										//セマンティック名
	0,												//同名のセマンティックがあるとき使うインデックス
	DXGI_FORMAT_R32G32B32_FLOAT,					//要素数とビット数を表す
	0,												//入力スロットインデックス
	D3D12_APPEND_ALIGNED_ELEMENT,					//データのオフセット地(左のは自動設定)
	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,		//入力データ種別
	0												//一度に描画するインスタンス数(0でよい)
	};// (1行で書いたほうが見やすい)
	//座標以外に色、テクスチャUVなどを渡す場合はさらに続ける
	//法線ベクトル
	vertLayout3d[1] = {
		"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
	};
	//UV
	vertLayout3d[2] = {
		"TEXCOORD",0,
		DXGI_FORMAT_R32G32_FLOAT,0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
	};
}

void NGPipeline::SetVertLayoutSprite()
{
	// 頂点レイアウト
	//座標
	vertLayoutSprite[0] = {
	"POSITION",										//セマンティック名
	0,												//同名のセマンティックがあるとき使うインデックス
	DXGI_FORMAT_R32G32B32_FLOAT,					//要素数とビット数を表す
	0,												//入力スロットインデックス
	D3D12_APPEND_ALIGNED_ELEMENT,					//データのオフセット地(左のは自動設定)
	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,		//入力データ種別
	0												//一度に描画するインスタンス数(0でよい)
	};// (1行で書いたほうが見やすい)
	//座標以外に色、テクスチャUVなどを渡す場合はさらに続ける
	//UV
	vertLayoutSprite[1] = {
		"TEXCOORD",0,
		DXGI_FORMAT_R32G32_FLOAT,0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
	};
}

void NGPipeline::SetShader()
{
	// シェーダーの設定
	pipelineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
	pipelineDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = psBlob->GetBufferSize();
}

void NGPipeline::SetRasterizer(bool isCull)
{
	// サンプルマスクの設定
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定

	// ラスタライザの設定
	if (isCull)
	{
		pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;	// 背面をカリング
	}
	else
	{
		pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;	// カリングしない
	}
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;	// ポリゴン内塗りつぶし(D3D12_FILL_MODE_WIREFRAMEにするとワイヤーフレームに)
	pipelineDesc.RasterizerState.DepthClipEnable = true; // 深度クリッピングを有効に
}

void NGPipeline::SetBlend()
{
	pipelineDesc.BlendState.AlphaToCoverageEnable = false;			//網羅率考慮してブレンドするか
	pipelineDesc.BlendState.IndependentBlendEnable = false;			//それぞれのレンダーターゲットに別々のブレンドするか

	//レンダーターゲットのブレンド設定
	blenddesc.BlendEnable = false;									//加算、乗算、αなどするか
	blenddesc.LogicOpEnable = false;								//論理演算するか
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	//マスク値：RBGA全てのチャンネルを描画
	//設定したブレンドを適用
	pipelineDesc.BlendState.RenderTarget[0] = blenddesc;

	blenddesc.BlendEnable = true;					//ブレンドを有効にする
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;	//加算
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;		//ソースの値を100%使う
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;	//デストの値を0%使う

	//加算合成
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;				//加算
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;			//ソースのアルファ値
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;	//1.0f-ソースのアルファ値
}

void NGPipeline::SetInputLayout(bool is3d)
{
	if (is3d)
	{
		pipelineDesc.InputLayout.pInputElementDescs = vertLayout3d;
		pipelineDesc.InputLayout.NumElements = _countof(vertLayout3d);
	}
	else
	{
		pipelineDesc.InputLayout.pInputElementDescs = vertLayoutSprite;
		pipelineDesc.InputLayout.NumElements = _countof(vertLayoutSprite);
	}
}

void NGPipeline::SetTopology()
{
	//トライアングルストリップを切り離すかどうか
	pipelineDesc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;	//カットなし
	//トポロジー指定
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
}

void NGPipeline::SetDepth(bool isDepth)
{
	if (isDepth)
	{
		pipelineDesc.DepthStencilState.DepthEnable = true;						//深度テストするか
	}
	else
	{
		pipelineDesc.DepthStencilState.DepthEnable = false;						//深度テストするか
	}
	pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;	//書き込み許可
	pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;		//小さければ合格
	pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;								//深度値フォーマット
}

void NGPipeline::SetRenderTarget()
{
	pipelineDesc.NumRenderTargets = 1;								//描画対象は1つ
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;	//0~255指定のRGBA
}

void NGPipeline::SetAntiAliasing()
{
	pipelineDesc.SampleDesc.Count = 1;		//1ピクセルにつき1回サンプリング
	pipelineDesc.SampleDesc.Quality = 0;	//最低クオリティ
}

void NGPipeline::SetRootSignature(ComPtr<ID3D12Device> device, std::vector<D3D12_ROOT_PARAMETER> rootParams)
{
	pipelineSet.rootSig.SetRootSignature(errorBlob, rootParams, samplerDesc);
	pipelineSet.rootSig.CreateRootSignature(device);

	pipelineDesc.pRootSignature = pipelineSet.rootSig.entity.Get();
}

void NGPipeline::CreatePS(ComPtr<ID3D12Device> device)
{
	HRESULT result;

	result = device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineSet.pipelineState));
	assert(SUCCEEDED(result));
}

void NGPipeline::SetTexSampler()
{
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//横繰り返し（タイリング）
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//縦繰り返し（タイリング）
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//奥行繰り返し（タイリング）
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;	//ボーダーの時は黒
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;					//全てリニア補間
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;									//ミップマップ最大値
	samplerDesc.MinLOD = 0.0f;												//ミップマップ最小値
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;			//ピクセルシェーダからのみ使用可能
}

NGPipeline::PipelineSet NGPipeline::CreatePipeline3d(ComPtr<ID3D12Device> device, std::vector<D3D12_ROOT_PARAMETER> rootParams)
{
	//シェーダー
	LoadVertShader3d();
	LoadPixelShader3d();

	//頂点レイアウト設定
	SetVertLayout3d();

	//パイプラインステート
	SetShader();
	SetRasterizer(true);
	SetBlend();
	SetInputLayout(true);
	SetTopology();
	SetRenderTarget();
	SetAntiAliasing();
	SetDepth(true);

	//テクスチャサンプラーの設定
	SetTexSampler();

	//ルートシグネチャ
	SetRootSignature(device, rootParams);

	//パイプラインステート生成
	CreatePS(device);

	return pipelineSet;
}

NGPipeline::PipelineSet NGPipeline::CreatePipelineSprite(ComPtr<ID3D12Device> device, std::vector<D3D12_ROOT_PARAMETER> rootParams)
{
	//シェーダー
	LoadVertShaderSprite();
	LoadPixelShaderSprite();

	//頂点レイアウト設定
	SetVertLayoutSprite();

	//パイプラインステート
	SetShader();
	SetRasterizer(false);
	SetBlend();
	SetInputLayout(false);
	SetTopology();
	SetRenderTarget();
	SetAntiAliasing();
	SetDepth(false);

	//テクスチャサンプラーの設定
	SetTexSampler();

	//ルートシグネチャ
	SetRootSignature(device, rootParams);

	//パイプラインステート生成
	CreatePS(device);

	return pipelineSet;
}
