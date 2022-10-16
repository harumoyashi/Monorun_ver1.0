#include "NTexture.h"
#include "NTexture.h"

void NTexture::Load(const wchar_t* pictureName)
{
	HRESULT result;

	result = LoadFromWICFile(
		pictureName,
		WIC_FLAGS_NONE,
		&metadata, scratchImg);
}

void NTexture::CreateMipmap()
{
	HRESULT result;

	result = GenerateMipMaps(
		scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(),
		TEX_FILTER_DEFAULT, 0, mipChain);
	if (SUCCEEDED(result))
	{
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}

	//読み込んだディフューズテクスチャをSRGBとして扱う
	metadata.format = MakeSRGB(metadata.format);
}

void NTexture::SetTBHeap()
{
	texHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	texHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	texHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;	//転送はCPU側(L0)から直接行う
	texHeapProp.CreationNodeMask = 0;							//単一アダプターだから0
	texHeapProp.VisibleNodeMask = 0;
}

void NTexture::SetTBResource()
{
	texResDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	texResDesc.Format = metadata.format;

	texResDesc.Width = metadata.width;	//幅
	texResDesc.Height = (UINT)metadata.height;	//高さ
	texResDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	texResDesc.MipLevels = (UINT16)metadata.mipLevels;
	texResDesc.SampleDesc.Count = 1;
}

void NTexture::CreateTexBuff(ComPtr<ID3D12Device> device)
{
	HRESULT result;

	result = device->CreateCommittedResource(
		&texHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&texResDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texBuff)
	);
}

void NTexture::TexBuffMaping()
{
	HRESULT result;

	//全ミップマップについて
	for (size_t i = 0; i < metadata.mipLevels; i++)
	{
		//ミップマップレベルを指定してイメージを取得
		const Image* img = scratchImg.GetImage(i, 0, 0);
		//テクスチャバッファにデータ転送
		result = texBuff->WriteToSubresource(
			(UINT)i,
			nullptr,				//全領域へコピー
			img->pixels,			//元データアドレス
			(UINT)img->rowPitch,	//1ラインサイズ
			(UINT)img->slicePitch	//全サイズ
		);
		assert(SUCCEEDED(result));
	}
}

void NTexture::SetSRV()
{
	srvDesc.Format = texResDesc.Format;		//RGBA float
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = texResDesc.MipLevels;
}

void NTexture::CreateSRV(ComPtr<ID3D12Device> device, ComPtr<ID3D12DescriptorHeap> srvHeap,int texNum)
{
	//SRVヒープの先頭ハンドルを取得
	srvHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();
	//指定されたテクスチャ番号に応じてハンドルを進める
	incrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	srvHandle.ptr += incrementSize * texNum;

	//ハンドルの指す位置にシェーダーリソースビュー作成
	device->CreateShaderResourceView(texBuff.Get(), &srvDesc, srvHandle);
}