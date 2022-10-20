#include "NGameScene.h"
#include "NSceneManager.h"

NGameScene* NGameScene::GetInstance()
{
	static NGameScene instance;
	return &instance;
}

void NGameScene::Initialize(NDX12* dx12)
{
#pragma region �`�揉��������
	//�}�e���A��(�萔�o�b�t�@)
	material.Initialize(dx12->GetDevice());

	//�I�u�W�F�N�g(�萔�o�b�t�@)
	for (size_t i = 0; i < maxBlock; i++)
	{
		block[i].Initialize(dx12->GetDevice());
		block[i].texNum = BLOCK;
		block[i].scale = { 32.0f,32.0f,32.0f };
	}

	block[0].position.y = 200.0f;
	block[1].position.x = -200.0f;
	block[2].position.x = 200.0f;

	player.Initialize(dx12->GetDevice());
	player.texNum = PLAYER;
	player.scale = { 24.0f,24.0f ,24.0f };

	////�w�i�X�v���C�g����
	//for (size_t i = 0; i < maxBackSprite; i++)
	//{
	//	backSprite[i] = new NSprite();
	//	backSprite[i]->texNum = static_cast<int>(i);
	//	backSprite[i]->CreateSprite(dx12->GetDevice(), NSceneManager::GetTex()[backSprite[i]->texNum].texBuff);
	//	//sprite[i]->CreateClipSprite(dx12->GetDevice(),tex[sprite[i]->texNum].texBuff,{100.0f,0},{50.0f,100.0f});	//�ꕔ�؂����Đ���
	//	backSprite[i]->position.x = i * 300.0f + 400.0f;
	//	backSprite[i]->position.y = 400.0f;
	//	backSprite[i]->UpdateMatrix();
	//	backSprite[i]->TransferMatrix();
	//}

	////�O�i�X�v���C�g����
	//for (size_t i = 0; i < maxForeSprite; i++)
	//{
	//	foreSprite[i] = new NSprite();
	//	foreSprite[i]->texNum = static_cast<int>(i);
	//	foreSprite[i]->CreateSprite(dx12->GetDevice(), NSceneManager::GetTex()[foreSprite[i]->texNum].texBuff);
	//	//sprite[i]->CreateClipSprite(dx12->GetDevice(),tex[sprite[i]->texNum].texBuff,{100.0f,0},{50.0f,100.0f});	//�ꕔ�؂����Đ���
	//	foreSprite[i]->SetColor(1, 1, 1, 0.5f);
	//	foreSprite[i]->size = { 150,150 };
	//	foreSprite[i]->TransferVertex();
	//	foreSprite[i]->position.x = i * 200.0f + 200.0f;
	//	foreSprite[i]->position.y = 200.0f;
	//	foreSprite[i]->UpdateMatrix();
	//	foreSprite[i]->TransferMatrix();
	//}
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

void NGameScene::Update()
{
	if (NInput::IsKeyTrigger(DIK_SPACE))
	{
		NSceneManager::SetScene(TITLESCENE);
	}
#pragma region �s��̌v�Z
	//�r���[�ϊ��s��č쐬
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));

	for (size_t i = 0; i < maxBlock; i++)
	{
		block[i].UpdateMatrix(matView, matProjection);
	}

	player.MoveKey();
	player.UpdateMatrix(matView, matProjection);
#pragma endregion
}

void NGameScene::Draw(NDX12* dx12)
{
#pragma region �O���t�B�b�N�X�R�}���h
	// 4.�`��R�}���h��������
	////�w�i�X�v���C�g
	//for (size_t i = 0; i < maxBackSprite; i++)
	//{
	//	backSprite[i]->CommonBeginDraw(dx12->GetCommandList(), NSceneManager::GetPipelineSprite()->pipelineSet.pipelineState, NSceneManager::GetPipelineSprite()->pipelineSet.rootSig.entity, dx12->GetSRVHeap());
	//	backSprite[i]->Draw(dx12->GetSRVHeap(), NSceneManager::GetTex()[0].incrementSize, dx12->GetCommandList());
	//}

	//3D�I�u�W�F�N�g
	for (size_t i = 0; i < maxBlock; i++)
	{
		block[i].CommonBeginDraw(dx12->GetCommandList(), NSceneManager::GetPipeline3d()->pipelineSet.pipelineState, NSceneManager::GetPipeline3d()->pipelineSet.rootSig.entity, dx12->GetSRVHeap());
		block[i].Draw(dx12->GetCommandList(), material, dx12->GetSRVHeap(), NSceneManager::GetPipeline3d()->vbView, NSceneManager::GetPipeline3d()->ibView, NSceneManager::GetPipeline3d()->numIB, NSceneManager::GetTex()[0].incrementSize);
	}
	player.CommonBeginDraw(dx12->GetCommandList(), NSceneManager::GetPipeline3d()->pipelineSet.pipelineState, NSceneManager::GetPipeline3d()->pipelineSet.rootSig.entity, dx12->GetSRVHeap());
	player.Draw(dx12->GetCommandList(), material, dx12->GetSRVHeap(), NSceneManager::GetPipeline3d()->vbView, NSceneManager::GetPipeline3d()->ibView, NSceneManager::GetPipeline3d()->numIB, NSceneManager::GetTex()[0].incrementSize);

	////�O�i�X�v���C�g
	//for (size_t i = 0; i < maxForeSprite; i++)
	//{
	//	foreSprite[i]->CommonBeginDraw(dx12->GetCommandList(), NSceneManager::GetPipelineSprite()->pipelineSet.pipelineState, NSceneManager::GetPipelineSprite()->pipelineSet.rootSig.entity, dx12->GetSRVHeap());
	//	foreSprite[i]->Draw(dx12->GetSRVHeap(), NSceneManager::GetTex()[0].incrementSize, dx12->GetCommandList());
	//}
	// 4.�`��R�}���h�����܂�
#pragma endregion
}

void NGameScene::Finalize()
{
	/*for (size_t i = 0; i < maxForeSprite; i++)
	{
		delete foreSprite[i];
	}

	for (size_t i = 0; i < maxBackSprite; i++)
	{
		delete backSprite[i];
	}*/
}