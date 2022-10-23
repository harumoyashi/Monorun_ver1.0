#include "NTitleScene.h"
#include "NSceneManager.h"
#include "NInput.h"

NTitleScene* NTitleScene::GetInstance()
{
	static NTitleScene instance;
	return &instance;
}

void NTitleScene::Initialize(NDX12* dx12)
{
#pragma region �`�揉��������
	//�}�e���A��(�萔�o�b�t�@)
	material.Initialize(dx12->GetDevice());

	//�I�u�W�F�N�g(�萔�o�b�t�@)
	for (size_t i = 0; i < maxObj; i++)
	{
		obj3d[i].Initialize(dx12->GetDevice());
		obj3d[i].texNum = PLAYER;
	}

	obj3d[0].scale = { 24.0f,24.0f,24.0f };
	obj3d[0].position = { 0.0f,-100.0f,0.0f };

	////�w�i�X�v���C�g����
	//for (size_t i = 0; i < maxBackSprite; i++)
	//{
	//	backSprite[i] = new NSprite();
	//	backSprite[i]->texNum = static_cast<int>(i);
	//	backSprite[i]->CreateSprite(dx12->GetDevice(), tex[backSprite[i]->texNum].texBuff);
	//	//sprite[i]->CreateClipSprite(dx12->GetDevice(),tex[sprite[i]->texNum].texBuff,{100.0f,0},{50.0f,100.0f});	//�ꕔ�؂����Đ���
	//	backSprite[i]->position.x = i * 300.0f + 400.0f;
	//	backSprite[i]->position.y = 400.0f;
	//	backSprite[i]->UpdateMatrix();
	//	backSprite[i]->TransferMatrix();
	//}

	//�O�i�X�v���C�g����
	titleSprite = new NSprite();
	titleSprite->texNum = TITLE;
	titleSprite->CreateSprite(dx12->GetDevice(), NSceneManager::GetTex()[titleSprite->texNum].texBuff);
	titleSprite->position = { NWindows::win_width / 2,NWindows::win_height / 2,0 };
	titleSprite->UpdateMatrix();

	startSprite = new NSprite();
	startSprite->texNum = START;
	startSprite->CreateSprite(dx12->GetDevice(), NSceneManager::GetTex()[titleSprite->texNum].texBuff);
	startSprite->position = { NWindows::win_width / 2,NWindows::win_height / 2,0 };
	startSprite->UpdateMatrix();

#pragma endregion
#pragma region	�J����������
	//�ˉe���e�ϊ�//
	matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45.0f),		//�㉺��p45�x
		(float)NWindows::win_width / NWindows::win_height,	//�A�X�y�N�g��(��ʉ���/��ʏc��)
		0.1f, 2000.0f					//�O�[�A���[
	);

	//�����Ńr���[�ϊ��s��v�Z
	matView;
	eye = { -100, 100, -1000 };	//���_���W
	target = { 0, 0, 0 };	//�����_���W
	up = { 0, 1, 0 };		//������x�N�g��
	//�r���[�ϊ��s��쐬
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));

	angle = 0.0f;	//�J�����̉�]�p
#pragma endregion

}

void NTitleScene::Update()
{
	if (NInput::IsKeyTrigger(DIK_SPACE))
	{
		NSceneManager::SetScene(STAGESELECTSCENE);
	}
#pragma region �s��̌v�Z
	obj3d[0].rotation.z += 0.3f;
	for (size_t i = 0; i < maxObj; i++)
	{
		obj3d[i].UpdateMatrix(matView, matProjection);
	}
#pragma endregion
}

void NTitleScene::Draw(NDX12* dx12)
{
#pragma region �O���t�B�b�N�X�R�}���h
	// 4.�`��R�}���h��������
	////�w�i�X�v���C�g
	//for (size_t i = 0; i < maxBackSprite; i++)
	//{
	//	backSprite[i]->CommonBeginDraw(dx12->GetCommandList(), NSceneManager::GetPipelineSprite()->pipelineSet.pipelineState, NSceneManager::GetPipelineSprite()->pipelineSet.rootSig.entity, dx12->GetSRVHeap());
	//	backSprite[i]->Draw(dx12->GetSRVHeap(), tex[0].incrementSize, dx12->GetCommandList());
	//}

	//3D�I�u�W�F�N�g
	for (size_t i = 0; i < maxObj; i++)
	{
		obj3d[i].CommonBeginDraw(dx12->GetCommandList(), NSceneManager::GetPipeline3d()->pipelineSet.pipelineState,
			NSceneManager::GetPipeline3d()->pipelineSet.rootSig.entity, dx12->GetSRVHeap());
		obj3d[i].Draw(dx12->GetCommandList(), material, dx12->GetSRVHeap(), NSceneManager::GetPipeline3d()->vbView, NSceneManager::GetPipeline3d()->ibView, NSceneManager::GetPipeline3d()->numIB, NSceneManager::GetTex()[0].incrementSize);
	}

	//�O�i�X�v���C�g
	titleSprite->CommonBeginDraw(dx12->GetCommandList(), NSceneManager::GetPipelineSprite()->pipelineSet.pipelineState,
		NSceneManager::GetPipelineSprite()->pipelineSet.rootSig.entity, dx12->GetSRVHeap());
	titleSprite->Draw(dx12->GetSRVHeap(), NSceneManager::GetTex()[0].incrementSize, dx12->GetCommandList());

	startSprite->CommonBeginDraw(dx12->GetCommandList(), NSceneManager::GetPipelineSprite()->pipelineSet.pipelineState,
		NSceneManager::GetPipelineSprite()->pipelineSet.rootSig.entity, dx12->GetSRVHeap());
	startSprite->Draw(dx12->GetSRVHeap(), NSceneManager::GetTex()[0].incrementSize, dx12->GetCommandList());

	// 4.�`��R�}���h�����܂�
#pragma endregion
}

void NTitleScene::Finalize()
{
	delete titleSprite;

	/*for (size_t i = 0; i < maxBackSprite; i++)
	{
		delete backSprite[i];
	}*/
}
