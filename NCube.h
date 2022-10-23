#pragma once
#include <DirectXMath.h>
#include "NDX12.h"
#include <d3dx12.h>

#include <wrl.h>

//���_�f�[�^�\����
struct Vertex
{
	XMFLOAT3 pos;		//xyz���W
	XMFLOAT3 normal;	//�@���x�N�g��
	XMFLOAT2 uv;		//uv���W
};

enum ModelNum
{
	CUBE,		//������
	CRYSTAL,	//���ʑ�
};

class NCube
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	//���_�܂��//
	UINT singleSizeVB;					//���_�o�b�t�@1������̃T�C�Y
	UINT sizeVB;						//���_�o�b�t�@�S�̂̃T�C�Y
	static std::vector<Vertex> vertices;//���_�f�[�^�z��
	ComPtr<ID3D12Resource> vertBuff;	//���_�o�b�t�@
	Vertex* vertMap = nullptr;			//�}�b�v�p

	//���_�C���f�b�N�X//
	static std::vector<unsigned short>indices;//���_�C���f�b�N�X�z��
	UINT sizeIB;						//�C���f�b�N�X�o�b�t�@�S�̂̃T�C�Y
	ComPtr<ID3D12Resource> indexBuff;	//�C���f�b�N�X�o�b�t�@
	uint16_t* indexMap = nullptr;		//�}�b�v�p

public:
	D3D12_VERTEX_BUFFER_VIEW vbView{};	//���_�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW ibView{};	//�C���f�b�N�X�o�b�t�@�r���[
	UINT numIB;							//�C���f�b�N�X�o�b�t�@�̐�

public:
	//3D�I�u�W�F�N�g
	void Initialize(ComPtr<ID3D12Device> device, int modelNum = CUBE);

private:
#pragma region ���_�܂��
	//���_�f�[�^�ݒ�
	void SetVertCube();
	//���_�f�[�^�ݒ�
	void SetVertCrystal();
	//�@���̌v�Z
	void SetNormal();
	//�o�b�t�@�쐬
	void CreateVertBuff(ComPtr<ID3D12Device> device);
	//�}�b�s���O
	void VertMaping();
	//���_�o�b�t�@�r���[�쐬
	void CreateVertBuffView();
#pragma endregion
#pragma region ���_�C���f�b�N�X�܂��
	//���_�C���f�b�N�X�ݒ�
	void SetIndexCube();
	//���_�C���f�b�N�X�ݒ�
	void SetIndexCrystal();
	//�o�b�t�@�쐬
	void CreateIndexBuff(ComPtr<ID3D12Device> device);
	//�}�b�s���O
	void IndexMaping();
	//�C���f�b�N�X�o�b�t�@�r���[�쐬
	void CreateIndexBuffView();
#pragma endregion
};

