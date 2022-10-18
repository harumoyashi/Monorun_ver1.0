#pragma once
#include "NDX12.h"

#include <d3dx12.h>
#include <wrl.h>

//�X�v���C�g�p���_�f�[�^�\����
struct VertexUV
{
	XMFLOAT3 pos;		//xyz���W
	XMFLOAT2 uv;		//uv���W
};

//�萔�o�b�t�@�p�f�[�^�\���́i3D�ϊ��s��j
struct  SpriteCBDataTransform
{
	XMFLOAT4 color;	//�F(RGBA)
	XMMATRIX mat;	//3D�ϊ��s��
};

class NSprite
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	//���_�܂��//
	UINT singleSizeVB;						//���_�o�b�t�@1������̃T�C�Y
	UINT sizeVB;							//���_�o�b�t�@�S�̂̃T�C�Y
	VertexUV vertices[4]{};					//���_����p
	D3D12_HEAP_PROPERTIES heapPropVert{};	//�q�[�v
	D3D12_RESOURCE_DESC resDescVert{};		//���\�[�X
	ComPtr<ID3D12Resource> vertBuff;		//���_�o�b�t�@
	VertexUV* vertMap = nullptr;			//�}�b�v�p
	D3D12_VERTEX_BUFFER_VIEW vbView{};		//���_�o�b�t�@�r���[

	//�萔�o�b�t�@�܂��//
	D3D12_HEAP_PROPERTIES heapPropConst{};		//�q�[�v
	D3D12_RESOURCE_DESC resDescConst{};			//���\�[�X
	SpriteCBDataTransform* constMapTransform;	//3D�ϊ��s��
	ComPtr<ID3D12Resource> constBuffTransform;	//�萔�o�b�t�@��GPU���\�[�X�̃|�C���^

	//�s��//
	XMMATRIX matWorld{};		//�ϊ��s��
	XMMATRIX matProjection{};	//���s���e�ۊǗp

	//�������̐ݒ�p//
	XMFLOAT2 anchorPoint = { 0.5f,0.5f };	//�A���J�[�|�C���g(0.0f~1.0f����Ȃ��Ɖ摜����o��)
	bool isFlipX = false;	//���E���]�t���O
	bool isFlipY = false;	//�㉺���]�t���O
	XMFLOAT2 texLeftTop = { 0,0 };			//�e�N�X�`��������W
	XMFLOAT2 texSize = { 100,100 };			//�e�N�X�`���؂�o���T�C�Y
public:
	//�ϊ��p//
	XMFLOAT2 size = {};		//�X�v���C�g�̑傫��(�ύX���TransferVertex()���Ȃ��Ɣ��f����Ȃ�)
	float rotation = 0.0f;	//Z���̉�]�p
	XMFLOAT3 position = { 0.0f,0.0f,0.0f };	//���W
	bool isInvisible = false;	//��\���ɂ���t���O

	int texNum = 0;	//�e�N�X�`���w��p
#pragma region ������
public:
	//�X�v���C�g����
	void CreateSprite(ComPtr<ID3D12Device> device);
	//�e�N�X�`���T�C�Y�ɍ��킹�ăX�v���C�g����
	//�A���J�[�|�C���g���ݒ�ł���(�ݒ肵�Ȃ��ƒ��S�ɂȂ�)
	//�㉺���E�̔��]�t���O���ݒ�ł���(�ݒ肵�Ȃ��Ɣ��]���Ȃ�)
	void CreateSprite(ComPtr<ID3D12Device> device, ComPtr<ID3D12Resource> texBuff,
		XMFLOAT2 anchorPoint = { 0.5f,0.5f }, bool isFlipX = false, bool isFlipY = false);
	//�e�N�X�`����؂����ăX�v���C�g����(�A�j���[�V�����A�t�H���g�Ȃ�)
	//�A���J�[�|�C���g���ݒ�ł���(�ݒ肵�Ȃ��ƒ��S�ɂȂ�)
	//�㉺���E�̔��]�t���O���ݒ�ł���(�ݒ肵�Ȃ��Ɣ��]���Ȃ�)
	void CreateClipSprite(ComPtr<ID3D12Device> device, ComPtr<ID3D12Resource> texBuff, XMFLOAT2 texLeftTop = { 0,0 },
		XMFLOAT2 texSize = { 100,100 }, XMFLOAT2 anchorPoint = { 0.5f,0.5f }, bool isFlipX = false, bool isFlipY = false);

private:
	//���_�f�[�^�ݒ�
	void SetVert();
	//���_�p�q�[�v�ݒ�
	void SetVertHeap();
	//���_�p���\�[�X�ݒ�
	void SetVertResource();
	//�o�b�t�@�쐬
	void CreateVertBuff(ComPtr<ID3D12Device> device);
	//�e�N�X�`���T�C�Y�ɍ��킹��
	void MatchTexSize(ComPtr<ID3D12Resource> texBuff);
	//�A���J�[�|�C���g�K�p
	void SetAncor(XMFLOAT2 anchorPoint);
	//�㉺���E���]�t���O�K�p
	void SetIsFlip(bool isFlipX, bool isFlipY);
	//�؂蔲���͈͓K�p
	void SetClipRange(XMFLOAT2 texLeftTop, XMFLOAT2 texSize);
	//�؂蔲���Ȃ��ꍇ�e�N�X�`���T�C�Y�ɍ��킹��
	void SetClipRange();
	//�}�b�s���O
	void VertMaping();
	//���_�o�b�t�@�r���[�쐬
	void CreateVertBuffView();
	//�萔�p�q�[�v�ݒ�
	void SetConstHeap();
	//�萔�p���\�[�X�ݒ�
	void SetConstResource();
	//�萔�o�b�t�@�̐���
	void CreateCB(ComPtr<ID3D12Device> device);
	//�萔�o�b�t�@�̃}�b�s���O
	void MappingCB();
#pragma endregion
public:
#pragma region �X�V
	//�X�v���C�g�̐F�ύX
	void SetColor(float R = 1, float G = 1, float B = 1, float A = 1);
	//���s���e�ϊ�
	void ParallelProjection();
	//�}�b�v����
	void Unmap();
	//���[���h�s��̍���
	void UpdateMatrix();
	//�萔�o�b�t�@�֑��M
	void TransferMatrix();
	//���_�o�b�t�@�]��
	void TransferVertex();
#pragma endregion
#pragma region �`��
	//���ʃO���t�B�b�N�X�R�}���h
	void CommonBeginDraw(ComPtr<ID3D12GraphicsCommandList> commandList,
		const ComPtr<ID3D12PipelineState> pipelineState, ComPtr<ID3D12RootSignature> rootSignature, ComPtr<ID3D12DescriptorHeap> srvHeap);
	//�`��R�}���h
	void Draw(ComPtr<ID3D12DescriptorHeap> srvHeap, UINT incrementSize, ComPtr<ID3D12GraphicsCommandList> commandList);
#pragma endregion
};