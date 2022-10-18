#include "NGameScene.h"

void NGameScene::Initialize(NDX12* dx12)
{
#pragma region �`�揉��������
	//���[�g�p�����[�^�̐ݒ�
	rootParams.SetDescRange();
	rootParams.SetRootParam();

	//�}�e���A��(�萔�o�b�t�@)
	material.Initialize(dx12->GetDevice());

	//�I�u�W�F�N�g(�萔�o�b�t�@)
	for (size_t i = 0; i < maxObj; i++)
	{
		obj3d[i].Initialize(dx12->GetDevice());
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
		(float)NWindows::win_width / NWindows::win_height,	//�A�X�y�N�g��(��ʉ���/��ʏc��)
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
		tex[i].CreateTexBuff(dx12->GetDevice());
		tex[i].TexBuffMaping();
		//�V�F�[�_�[���\�[�X�r���[�ݒ�
		tex[i].SetSRV();
		tex[i].CreateSRV(dx12->GetDevice(), dx12->GetSRVHeap(), i);
	}

	//�w�i�X�v���C�g����
	for (size_t i = 0; i < maxBackSprite; i++)
	{
		backSprite[i] = new NSprite();
		backSprite[i]->texNum = static_cast<int>(i);
		backSprite[i]->CreateSprite(dx12->GetDevice(), tex[backSprite[i]->texNum].texBuff);
		//sprite[i]->CreateClipSprite(dx12->GetDevice(),tex[sprite[i]->texNum].texBuff,{100.0f,0},{50.0f,100.0f});	//�ꕔ�؂����Đ���
		backSprite[i]->position.x = i * 300.0f + 400.0f;
		backSprite[i]->position.y = 400.0f;
		backSprite[i]->UpdateMatrix();
		backSprite[i]->TransferMatrix();
	}

	//�O�i�X�v���C�g����
	for (size_t i = 0; i < maxForeSprite; i++)
	{
		foreSprite[i] = new NSprite();
		foreSprite[i]->texNum = static_cast<int>(i);
		foreSprite[i]->CreateSprite(dx12->GetDevice(), tex[backSprite[i]->texNum].texBuff);
		//sprite[i]->CreateClipSprite(dx12->GetDevice(),tex[sprite[i]->texNum].texBuff,{100.0f,0},{50.0f,100.0f});	//�ꕔ�؂����Đ���
		foreSprite[i]->SetColor(1, 1, 1, 0.5f);
		foreSprite[i]->size = { 150,150 };
		foreSprite[i]->TransferVertex();
		foreSprite[i]->position.x = i * 200.0f + 200.0f;
		foreSprite[i]->position.y = 200.0f;
		foreSprite[i]->UpdateMatrix();
		foreSprite[i]->TransferMatrix();
	}

#pragma region �O���t�B�b�N�X�p�C�v���C��
	gPipe3d = new NGPipeline();
	gPipe3d->Initialize3d(dx12->GetDevice());
	gPipe3d->pipelineSet = gPipe3d->CreatePipeline3d(dx12->GetDevice(), rootParams.entity);

	gPipeSprite = new NGPipeline();
	gPipeSprite->pipelineSet = gPipeSprite->CreatePipelineSprite(dx12->GetDevice(), rootParams.entity);
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

	//�X�v���C�g�̕ϊ��e�X�g
	increment += 1.0f;
	for (size_t i = 0; i < maxForeSprite; i++)
	{
		//foreSprite[i]->rotation = increment;
		//foreSprite[i]->position.x = increment;

		//foreSprite[i]->UpdateMatrix();
		//foreSprite[i]->TransferMatrix();*/
		//foreSprite[i]->SetColor(1, 1, 1, increment*0.01f);

		//foreSprite[i]->size = { increment,i * 100.0f + 100.0f };
		//foreSprite[i]->TransferVertex();

		//if (increment > 300.0f)
		//{
		//	foreSprite[i]->isInvisible = true;
		//}
	}
#pragma endregion
}

void NGameScene::Draw(NDX12* dx12)
{
	HRESULT result;

#pragma region �`��O����
	NPreDraw* preDraw = nullptr;
	preDraw = new NPreDraw();

	preDraw->SetResBarrier(dx12->GetSwapchain(), dx12->backBuffers, dx12->GetCommandList());
	preDraw->SetRenderTarget(dx12->GetRTVHeap(), dx12->GetDevice(), dx12->GetRTVHeapDesc(), dx12->GetDSVHeap(), dx12->GetCommandList());
	preDraw->ClearScreen(dx12->GetCommandList());
#pragma endregion
#pragma region �O���t�B�b�N�X�R�}���h
	// 4.�`��R�}���h��������
	preDraw->SetViewport(dx12->GetCommandList());
	preDraw->SetScissorRect(dx12->GetCommandList());

	//�w�i�X�v���C�g
	for (size_t i = 0; i < maxBackSprite; i++)
	{
		backSprite[i]->CommonBeginDraw(dx12->GetCommandList(), gPipeSprite->pipelineSet.pipelineState, gPipeSprite->pipelineSet.rootSig.entity, dx12->GetSRVHeap());
		backSprite[i]->Draw(dx12->GetSRVHeap(), tex[0].incrementSize, dx12->GetCommandList());
	}

	//3D�I�u�W�F�N�g
	for (size_t i = 0; i < maxObj; i++)
	{
		obj3d[i].CommonBeginDraw(dx12->GetCommandList(), gPipe3d->pipelineSet.pipelineState, gPipe3d->pipelineSet.rootSig.entity, dx12->GetSRVHeap());
		obj3d[i].Draw(dx12->GetCommandList(), material, dx12->GetSRVHeap(), gPipe3d->vbView, gPipe3d->ibView, gPipe3d->numIB, tex[0].incrementSize);
	}

	//�O�i�X�v���C�g
	for (size_t i = 0; i < maxForeSprite; i++)
	{
		foreSprite[i]->CommonBeginDraw(dx12->GetCommandList(), gPipeSprite->pipelineSet.pipelineState, gPipeSprite->pipelineSet.rootSig.entity, dx12->GetSRVHeap());
		foreSprite[i]->Draw(dx12->GetSRVHeap(), tex[0].incrementSize, dx12->GetCommandList());
	}
	// 4.�`��R�}���h�����܂�
#pragma endregion
	dx12->PostDraw(preDraw->barrierDesc);
}

void NGameScene::Finalize()
{
	for (size_t i = 0; i < maxForeSprite; i++)
	{
		delete foreSprite[i];
	}

	for (size_t i = 0; i < maxBackSprite; i++)
	{
		delete backSprite[i];
	}

	delete gPipe3d;
	delete gPipeSprite;
}