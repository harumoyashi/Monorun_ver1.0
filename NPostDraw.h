#pragma once
#include "NDX12.h"

#include <wrl.h>

class NPostDraw
{
public:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
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
	void All(D3D12_RESOURCE_BARRIER barrierDesc, ComPtr<ID3D12GraphicsCommandList> commandList,ComPtr<ID3D12CommandQueue> commandQueue,
		ComPtr<IDXGISwapChain4> swapchain, ComPtr<ID3D12Fence> fence, UINT64 fenceVal,ComPtr<ID3D12CommandAllocator> commandAllocator);
};

