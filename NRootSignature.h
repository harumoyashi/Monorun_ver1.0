#pragma once
#include "NDX12.h"

#include <wrl.h>

class NRootSignature
{
public:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	ComPtr<ID3D12RootSignature> entity;
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	ComPtr<ID3DBlob> rootSigBlob;

public:
	//���[�g�V�O�l�`���̃V���A���C�Y(�o�C�i���R�[�h�쐬)
	void SetRootSignature(ComPtr<ID3DBlob> errorBlob, std::vector<D3D12_ROOT_PARAMETER> rootParams, D3D12_STATIC_SAMPLER_DESC samplerDesc);
	//���[�g�V�O�l�`������
	void CreateRootSignature(ComPtr<ID3D12Device> device);
};

