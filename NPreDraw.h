#pragma once
#include "NDX12.h"

#include <wrl.h>

class NPreDraw
{
public:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	UINT bbIndex;
	D3D12_RESOURCE_BARRIER barrierDesc{};		//���\�[�X�o���A
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle{};	//�����_�[�^�[�Q�b�g�r���[�n���h��
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle{};	//�f�X�N���v�^�n���h��

	D3D12_VIEWPORT viewport{};	//�r���[�|�[�g
	D3D12_RECT scissorRect{};	//�V�U�[��`

public:
	//���\�[�X�o���A�ŏ������݉\�ɕύX
	void SetResBarrier(ComPtr<IDXGISwapChain4> swapchain,
		std::vector<ComPtr<ID3D12Resource>> backBuffers, ComPtr<ID3D12GraphicsCommandList> commandList);
	//�`���̕ύX
	void SetRenderTarget(ComPtr<ID3D12DescriptorHeap> rtvHeap, ComPtr<ID3D12Device> device,
		D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc, ComPtr<ID3D12DescriptorHeap> dsvHeap, ComPtr<ID3D12GraphicsCommandList> commandList);
	//��ʃN���ARGBA
	void ClearScreen(ComPtr<ID3D12GraphicsCommandList> commandList);
	//�r���[�|�[�g�ݒ�
	void SetViewport(ComPtr<ID3D12GraphicsCommandList> commandList);
	//�V�U�[��`�ݒ�
	void SetScissorRect(ComPtr<ID3D12GraphicsCommandList> commandList);

	//�o���A����
	void BarrierReset(D3D12_RESOURCE_BARRIER barrierDesc, ComPtr<ID3D12GraphicsCommandList> commandList);
	//���߂̃N���[�Y
	//�����R�}���h���X�g�ɐςނ̂����܂�
	void CmdListClose(ComPtr<ID3D12GraphicsCommandList> commandList);
	// �R�}���h���X�g�̎��s
	void ExecuteCmdList(ComPtr<ID3D12GraphicsCommandList> commandList, ComPtr<ID3D12CommandQueue> commandQueue);
	// ��ʂɕ\������o�b�t�@���t���b�v(���\�̓��ւ�)
	void BufferSwap(ComPtr<IDXGISwapChain4> swapchain);
	// �R�}���h�̎��s������҂�
	void CommandWait(ComPtr<ID3D12CommandQueue> commandQueue, ComPtr<ID3D12Fence> fence, UINT64 fenceVal);
	// �L���[���N���A
	void ClearQueue(ComPtr<ID3D12CommandAllocator> commandAllocator);
	// �ĂуR�}���h���X�g�𒙂߂鏀��
	void CmdListReset(ComPtr<ID3D12GraphicsCommandList> commandList, ComPtr<ID3D12CommandAllocator> commandAllocator);
};

