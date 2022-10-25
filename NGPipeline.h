#pragma once
#include <DirectXMath.h>
#include "NDX12.h"
#include "NTexture.h"
#include "NRootSignature.h"

#include <wrl.h>

//�V�F�[�_�[�g���̂ɕK�v�ȕ��X
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

class NGPipeline
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	struct PipelineSet
	{
		ComPtr<ID3D12PipelineState> pipelineState;
		NRootSignature rootSig;
	};

	//�V�F�[�_�[���\�[�X�r���[//
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};	//�ݒ�\����

	//�V�F�[�_�[�܂��(���_���܂�)//
	ComPtr<ID3DBlob> vsBlob;	// ���_�V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> psBlob;	// �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> errorBlob; // �G���[�I�u�W�F�N�g

	//���_���C�A�E�g//
	D3D12_INPUT_ELEMENT_DESC vertLayout3d[3]{};		//�K�v�ȕ������p�ӂ���
	D3D12_INPUT_ELEMENT_DESC vertLayoutSprite[2]{};	//�K�v�ȕ������p�ӂ���

	//�p�C�v���C���X�e�[�g//
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc = {};

	//�e�N�X�`���T���v���[//
	D3D12_STATIC_SAMPLER_DESC samplerDesc{};

public:
	//�Q�Ƃ�����̂���
	PipelineSet pipelineSet;			//�p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���܂Ƃ߂����

	D3D12_VERTEX_BUFFER_VIEW vbView{};	//���_�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW ibView{};	//�C���f�b�N�X�o�b�t�@�r���[
	UINT sizeIB;						//�C���f�b�N�X�o�b�t�@�S�̂̃T�C�Y
	UINT numIB;							//�C���f�b�N�X�o�b�t�@�̐�

public:
	//�p�C�v���C���̏�����(����)
#pragma region �V�F�[�_�[�܂��
	//3D�I�u�W�F�N�g�p���_�V�F�[�_�[�̓ǂݍ��݂ƃR���p�C��
	void LoadVertShader3d();
	//3D�I�u�W�F�N�g�p�s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	void LoadPixelShader3d();
	//�X�v���C�g�p���_�V�F�[�_�[�̓ǂݍ��݂ƃR���p�C��
	void LoadVertShaderSprite();
	//�X�v���C�g�p�s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	void LoadPixelShaderSprite();
#pragma endregion
#pragma region ���_���C�A�E�g�܂��
	//3D�I�u�W�F�N�g�p���_���C�A�E�g�ݒ�
	void SetVertLayout3d();
	//�X�v���C�g�p���_���C�A�E�g�ݒ�
	void SetVertLayoutSprite();
#pragma endregion
#pragma region �p�C�v���C���X�e�[�g�܂��
	//�V�F�[�_�[�̐ݒ�(�K�p)
	void SetShader();
	//���X�^���C�U�[�ݒ�
	void SetRasterizer(bool isCull);
	//�u�����h�ݒ�
	void SetBlend(bool AlphaToCoverageEnable);
	//���̓��C�A�E�g�̐ݒ�
	void SetInputLayout(bool is3d);
	//�}�`�̌`��ݒ�
	void SetTopology();
	//�f�v�X�X�e���V��(�[�x)�ݒ�
	void SetDepth(bool isDepth);
	//�����_�[�^�[�Q�b�g�ݒ�
	void SetRenderTarget();
	//�A���`�G�C���A�V���O�̂��߂̃T���v�����ݒ�
	void SetAntiAliasing();
	//�p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	void SetRootSignature(ComPtr<ID3D12Device> device, std::vector<D3D12_ROOT_PARAMETER> rootParams);
	//�O���t�B�b�N�X�p�C�v���C���X�e�[�g�I�u�W�F�N�g�̐���
	void CreatePS(ComPtr<ID3D12Device> device);
#pragma endregion
#pragma region �e�N�X�`���T���v���[
	//�e�N�X�`���T���v���[�ݒ�
	void SetTexSampler();
#pragma endregion
#pragma region �p�C�v���C������
	//3D�I�u�W�F�N�g�p�p�C�v���C������
	PipelineSet CreatePipeline3d(ComPtr<ID3D12Device> device, std::vector<D3D12_ROOT_PARAMETER> rootParams);
	//�X�v���C�g�p�p�C�v���C������
	PipelineSet CreatePipelineSprite(ComPtr<ID3D12Device> device, std::vector<D3D12_ROOT_PARAMETER> rootParams);
#pragma endregion

private:

};