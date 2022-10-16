#include "NRootSignature.h"

void NRootSignature::SetRootSignature(ComPtr<ID3DBlob> errorBlob, std::vector<D3D12_ROOT_PARAMETER> rootParams, D3D12_STATIC_SAMPLER_DESC samplerDesc)
{
	HRESULT result;

	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = &rootParams.front();		//ルートパラメータの先頭アドレス
	rootSignatureDesc.NumParameters = rootParams.size();		//ルートパラメータ数
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result));
}

void NRootSignature::CreateRootSignature(ComPtr<ID3D12Device> device)
{
	HRESULT result;

	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&entity));
	assert(SUCCEEDED(result));
}
