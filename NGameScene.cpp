#include "NGameScene.h"

void NGameScene::Initialize(NDX12* dx12)
{
#pragma region �`�揉��������
	//���[�g�p�����[�^�̐ݒ�
	rootParams.SetDescRange();
	rootParams.SetRootParam();

	//�}�e���A��(�萔�o�b�t�@)
	material.Initialize(dx12->device);

	//�I�u�W�F�N�g(�萔�o�b�t�@)
	for (size_t i = 0; i < maxObj; i++)
	{
		obj3d[i].Initialize(dx12->device);
	}
	obj3d[0].texNum = 0;
	obj3d[1].texNum = 1;
	obj3d[2].texNum = 2;

	obj3d[1].position.x = -20.0f;
	obj3d[2].position.x = 20.0f;
#pragma region �s��̌v�Z
	/*XMMATRIX oldVer = XMMatrixIdentity();
	oldVer.r[0].m128_f32[0] = 2.0f / window_width;
	oldVer.r[1].m128_f32[1] = -2.0f / window_height;

	oldVer.r[3].m128_f32[0] = -1.0f;
	oldVer.r[3].m128_f32[1] = 1.0f;

	XMMATRIX newVer = XMMatrixOrthographicOffCenterLH(
		0,window_width,
		window_height,0,
		0.0f, 1.0f
	);*/

	//���s���e�ϊ�//
	//�P�ʍs�����
	//material.constMapTransform->mat = XMMatrixIdentity();

	/*constMapTransform->mat.r[0].m128_f32[0] = 2.0f / window_width;
	constMapTransform->mat.r[1].m128_f32[1] = -2.0f / window_height;

	constMapTransform->mat.r[3].m128_f32[0] = -1.0f;
	constMapTransform->mat.r[3].m128_f32[1] = 1.0f;*/

	//���s���e�ϊ�
	/*material.constMapTransform->mat = XMMatrixOrthographicOffCenterLH(
		0, win_width,
		win_height, 0,
		0.0f, 1.0f
	);*/

	//�������e�ϊ�//
	//material.constMapTransform->mat = XMMatrixPerspectiveFovLH(
	//	XMConvertToRadians(45.0f),		//�㉺��p45�x
	//	(float)win_width / win_height,	//�A�X�y�N�g��(��ʉ���/��ʏc��)
	//	0.1f, 1000.0f					//�O�[�A���[
	//);

	//�ˉe���e�ϊ�//
	matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45.0f),		//�㉺��p45�x
		(float)win_width / win_height,	//�A�X�y�N�g��(��ʉ���/��ʏc��)
		0.1f, 1000.0f					//�O�[�A���[
	);

	//�����Ńr���[�ϊ��s��v�Z
	matView;
	eye = { 0, 0, -100 };	//���_���W
	target = { 0, 0, 0 };	//�����_���W
	up = { 0, 1, 0 };		//������x�N�g��
	//�r���[�ϊ��s��쐬
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));

	angle = 0.0f;	//�J�����̉�]�p
#pragma endregion
	//�e�N�X�`������
	tex[0].Load(L"Resources/mario.jpg");
	tex[1].Load(L"Resources/itiro_kimegao.png");
	tex[2].Load(L"Resources/hamutaro.jpg");

	for (int i = 0; i < maxTex; i++)
	{
		tex[i].CreateMipmap();
		//�e�N�X�`���o�b�t�@�ݒ�
		tex[i].SetTBHeap();
		tex[i].SetTBResource();
		tex[i].CreateTexBuff(dx12->device);
		tex[i].TexBuffMaping();
		//�V�F�[�_�[���\�[�X�r���[�ݒ�
		tex[i].SetSRV();
		tex[i].CreateSRV(dx12->device, dx12->srvHeap, i);
	}

	//�X�v���C�g����
	for (size_t i = 0; i < maxSprite; i++)
	{
		sprite[i] = new NSprite();
		sprite[i]->texNum = i;
		sprite[i]->CreateSprite(dx12->device, tex[sprite[i]->texNum].texBuff);
		//sprite[i]->CreateClipSprite(dx12->device,tex[sprite[i]->texNum].texBuff,{100.0f,0},{50.0f,100.0f});	//�ꕔ�؂����Đ���
		sprite[i]->position.x = i * 200.0f + 200.0f;
		sprite[i]->position.y = 300.0f;
		sprite[i]->UpdateMatrix();
		sprite[i]->TransferMatrix();
	}

#pragma region �O���t�B�b�N�X�p�C�v���C��
	gPipe3d = new NGPipeline();
	gPipe3d->Initialize3d(dx12->device);
	gPipe3d->pipelineSet = gPipe3d->CreatePipeline3d(dx12->device, rootParams.entity);

	gPipeSprite = new NGPipeline();
	gPipeSprite->pipelineSet = gPipeSprite->CreatePipelineSprite(dx12->device, rootParams.entity);
#pragma endregion
#pragma endregion
}

void NGameScene::Update()
{
#pragma region �s��̌v�Z
	//�r���[�ϊ��s��č쐬
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));

	obj3d[0].MoveKey();
	for (size_t i = 0; i < maxObj; i++)
	{
		obj3d[i].UpdateMatrix();
		obj3d[i].TransferMatrix(matView, matProjection);
	}

	////�X�v���C�g�̕ϊ��e�X�g
	//increment += 1.0f;
	//for (size_t i = 0; i < maxSprite; i++)
	//{
	//	sprite[i]->rotation = increment;
	//	sprite[i]->position.x = increment;

	//	sprite[i]->UpdateMatrix();
	//	sprite[i]->TransferMatrix();
	//	sprite[i]->SetColor(1, 0, 1, 1);

	//	sprite[i]->size = { increment,i * 100.0f + 100.0f };
	//	sprite[i]->TransferVertex();

	//	if (increment > 300.0f)
	//	{
	//		sprite[i]->isInvisible = true;
	//	}
	//}
#pragma endregion
}

void NGameScene::Draw(NDX12* dx12)
{
	HRESULT result;

#pragma region �`��O����
	NPreDraw* preDraw = nullptr;
	preDraw = new NPreDraw();

	preDraw->SetResBarrier(dx12->swapchain, dx12->backBuffers, dx12->GetCommandList());
	preDraw->SetRenderTarget(dx12->rtvHeap, dx12->device, dx12->rtvHeapDesc, dx12->dsvHeap, dx12->GetCommandList());
	preDraw->ClearScreen(dx12->GetCommandList());
#pragma endregion
#pragma region �O���t�B�b�N�X�R�}���h
	// 4.�`��R�}���h��������
	preDraw->SetViewport(dx12->GetCommandList());
	preDraw->SetScissorRect(dx12->GetCommandList());

	//3D�I�u�W�F�N�g
	for (size_t i = 0; i < maxObj; i++)
	{
		obj3d[i].CommonBeginDraw(dx12->GetCommandList(), gPipe3d->pipelineSet.pipelineState, gPipe3d->pipelineSet.rootSig.entity, dx12->srvHeap);
		obj3d[i].Draw(dx12->GetCommandList(), material, dx12->srvHeap, gPipe3d->vbView, gPipe3d->ibView, gPipe3d->sizeIB, tex[0].incrementSize);
	}

	//�X�v���C�g
	for (size_t i = 0; i < maxSprite; i++)
	{
		sprite[i]->CommonBeginDraw(dx12->GetCommandList(), gPipeSprite->pipelineSet.pipelineState, gPipeSprite->pipelineSet.rootSig.entity, dx12->srvHeap);
		sprite[i]->Draw(dx12->srvHeap, tex[0].incrementSize, dx12->GetCommandList());
	}
	// 4.�`��R�}���h�����܂�
#pragma endregion
	/*NPostDraw* postDraw = nullptr;
	postDraw = new NPostDraw;
	preDraw->BarrierReset(preDraw->barrierDesc, dx12->GetCommandList());
	preDraw->CmdListClose(dx12->GetCommandList());
	preDraw->ExecuteCmdList(dx12->GetCommandList(), dx12->commandQueue);
	preDraw->BufferSwap(dx12->swapchain);
	preDraw->CommandWait(dx12->commandQueue, dx12->GetFence(), dx12->fenceVal);
	preDraw->ClearQueue(dx12->GetCommandAllocator());
	preDraw->CmdListReset(dx12->GetCommandList(), dx12->GetCommandAllocator());*/

	// 5.���\�[�X�o���A��߂�
	preDraw->barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET; // �`���Ԃ���
	preDraw->barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT; // �\����Ԃ�
	dx12->GetCommandList()->ResourceBarrier(1, &preDraw->barrierDesc);

	// ���߂̃N���[�Y
	result = dx12->GetCommandList()->Close();
	assert(SUCCEEDED(result));

	// �R�}���h���X�g�̎��s
	ID3D12CommandList* commandLists[] = { dx12->GetCommandList() };
	dx12->commandQueue->ExecuteCommandLists(1, commandLists);

	// ��ʂɕ\������o�b�t�@���t���b�v(���\�̓��ւ�)
	result = dx12->swapchain->Present(1, 0);
	assert(SUCCEEDED(result));

	// �R�}���h�̎��s������҂�
	dx12->commandQueue->Signal(dx12->GetFence(), ++dx12->fenceVal);
	if (dx12->GetFence()->GetCompletedValue() != dx12->fenceVal) {
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		dx12->GetFence()->SetEventOnCompletion(dx12->fenceVal, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}

	// �L���[���N���A
	result = dx12->GetCommandAllocator()->Reset();
	assert(SUCCEEDED(result));

	// �ĂуR�}���h���X�g�𒙂߂鏀��
	result = dx12->GetCommandList()->Reset(dx12->GetCommandAllocator(), nullptr);
	assert(SUCCEEDED(result));
}