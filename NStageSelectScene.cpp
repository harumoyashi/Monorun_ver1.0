#include "NStageSelectScene.h"
#include "NSceneManager.h"
#include "NInput.h"
#include "Util.h"
#include <chrono>

// --�C�[�W���O-- //
float EaseOutCubic(float start, float end, float t) {
	float time = 1 - pow(1 - t, 3);
	return start * (1.0f - time) + end * time;
	//return start * (1.0f - pow(1.0f - t, 3)) + end * t;
}

NStageSelectScene* NStageSelectScene::GetInstance()
{
	static NStageSelectScene instance;
	return &instance;
}

void NStageSelectScene::Reset() {
	// --���Ԍv���ɕK�v�ȃf�[�^�ϐ�-- //
	nowCount_ = 0;
	startCount_ = 0;

	// --�X�N���[�����Ă���̌o�ߎ���-- //
	nowScrollTime_ = 0.0f;

	// --���ݑI��ł���X�e�[�W-- //
	selectStage_ = 1;

	for (size_t i = 0; i < maxForeSprite; i++) {
		foreSprite[i]->position.x = 300.0f;
		foreSprite[i]->position.y = 400.0f + (i * 250.0f);
		foreSprite[i]->UpdateMatrix();

		// --�C�[�W���O�p�ϐ�������-- //
		easeStartPos_[i] = { 300.0f, 400.0f + (i * 250.0f), 0.0f };
		easeEndPos_[i] = { 300.0f, 400.0f + (i * 250.0f), 0.0f };
	}
}

void NStageSelectScene::Initialize(NDX12* dx12)
{
#pragma region �`�揉��������
	////�}�e���A��(�萔�o�b�t�@)
	//material.Initialize(dx12->GetDevice());

	////�I�u�W�F�N�g(�萔�o�b�t�@)
	//for (size_t i = 0; i < maxObj; i++)
	//{
	//	obj3d[i].Initialize(dx12->GetDevice());
	//	obj3d[i].texNum = 1;
	//}

	//obj3d[1].position.x = -20.0f;
	//obj3d[2].position.x = 20.0f;

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

	stage_ = stage_->GetInstance();
	stage_->Initialize(dx12);

	//�O�i�X�v���C�g����
	for (size_t i = 0; i < maxForeSprite; i++) {
		foreSprite[i] = new NSprite();
		foreSprite[i]->texNum = static_cast<int>(STAGESELECTIMAGE);
		foreSprite[i]->CreateClipSprite(dx12->GetDevice(), NSceneManager::GetTex()[foreSprite[i]->texNum].texBuff, {i * 200.0f, 0.0f}, {200.0f, 200.0f});
		//sprite[i]->CreateClipSprite(dx12->GetDevice(),tex[sprite[i]->texNum].texBuff,{100.0f,0},{50.0f,100.0f});	//�ꕔ�؂����Đ���
		foreSprite[i]->SetColor(1, 1, 1, 1);
		foreSprite[i]->size = { 200.0f,200.0f };
		foreSprite[i]->TransferVertex();
		foreSprite[i]->position.x = 300.0f;
		foreSprite[i]->position.y = 400.0f + (i * 250.0f);
		foreSprite[i]->UpdateMatrix();

		// --�C�[�W���O�p�ϐ�������-- //
		easeStartPos_[i] = { 300.0f, 400.0f + (i * 250.0f), 0.0f};
		easeEndPos_[i] = { 300.0f, 400.0f + (i * 250.0f), 0.0f};
	}

	// --���Ԍv���ɕK�v�ȃf�[�^�ϐ�-- //
	nowCount_ = 0;
	startCount_ = 0;

	// --�X�N���[�����Ă���̌o�ߎ���-- //
	nowScrollTime_ = 0.0f;

	// --���ݑI��ł���X�e�[�W-- //
	selectStage_ = 1;

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

void NStageSelectScene::Update()
{
	// --[SPACE]����������-- //
	if (NInput::IsKeyTrigger(DIK_SPACE))
	{
		stage_->SetCSV(selectStage_);
		if (!NSceneManager::GetPlayEffect()) {
			NSceneManager::SetScene(GAMESCENE);
		}
	}

	// --[��]����������-- //
	if (NInput::IsKeyTrigger(DIK_UP)) {
		// --�I��ł���X�e�[�W�̒l��1���傫���Ȃ�-- //
		if (selectStage_ > 1) {
			// --���݂̃J�E���g���擾-- //
			startCount_ = static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count());

			for (size_t i = 0; i < 10; i++) {
				easeStartPos_[i] = foreSprite[i]->position;
				easeEndPos_[i].y += 250.0f;
			}

			// --���ݑI��ł���X�e�[�W�̒l��1���炷-- //
			selectStage_--;
		}
	}

	// --[��]����������-- //
	if (NInput::IsKeyTrigger(DIK_DOWN)) {
		// --�I��ł���X�e�[�W�̒l��10��菬�����Ȃ�-- //
		if (selectStage_ < 10) {
			// --���݂̃J�E���g���擾-- //
			startCount_ = static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count());

			for (size_t i = 0; i < 10; i++) {
				easeStartPos_[i] = foreSprite[i]->position;
				easeEndPos_[i].y -= 250.0f;
			}

			// --���ݑI��ł���X�e�[�W�̒l��1���炷-- //
			selectStage_++;
		}
	}

	// --�o�ߎ���timeRate[s]�̌v�Z
	//nowCount_ = GetNowCount();
	nowCount_ = static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count());
	nowScrollTime_ = nowCount_ - startCount_;
	nowScrollTime_ = nowScrollTime_ / 1000.0f;

	float timeRate = Util::Clamp(nowScrollTime_ / maxScrollTime_, 1.0f, 0.0f);

	float len[10];
	for (size_t i = 0; i < 10; i++) {
		foreSprite[i]->position.y = EaseOutCubic(easeStartPos_[i].y, easeEndPos_[i].y, timeRate);
		len[i] = abs(400.0f - foreSprite[i]->position.y);
		len[i] = Util::Clamp(len[i] / 600.0f, 1.0f, 0.0f);
		foreSprite[i]->size.x = 200 * (1.0f - len[i]);
		foreSprite[i]->size.y = 200 * (1.0f - len[i]);
		foreSprite[i]->TransferVertex();
		foreSprite[i]->UpdateMatrix();
	}


#pragma region �s��̌v�Z
	//�r���[�ϊ��s��č쐬
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));

	//obj3d[0].MoveKey();
	//for (size_t i = 0; i < maxObj; i++)
	//{
	//	obj3d[i].UpdateMatrix();
	//	obj3d[i].TransferMatrix(matView, matProjection);
	//}
#pragma endregion
}

void NStageSelectScene::Draw(NDX12* dx12)
{
#pragma region �O���t�B�b�N�X�R�}���h
	// 4.�`��R�}���h��������
	////�w�i�X�v���C�g
	//for (size_t i = 0; i < maxBackSprite; i++)
	//{
	//	backSprite[i]->CommonBeginDraw(dx12->GetCommandList(), NSceneManager::GetPipelineSprite()->pipelineSet.pipelineState, NSceneManager::GetPipelineSprite()->pipelineSet.rootSig.entity, dx12->GetSRVHeap());
	//	backSprite[i]->Draw(dx12->GetSRVHeap(), NSceneManager::GetTex()[0].incrementSize, dx12->GetCommandList());
	//}

	////3D�I�u�W�F�N�g
	//for (size_t i = 0; i < maxObj; i++)
	//{
	//	obj3d[i].CommonBeginDraw(dx12->GetCommandList(), NSceneManager::GetPipeline3d()->pipelineSet.pipelineState, NSceneManager::GetPipeline3d()->pipelineSet.rootSig.entity, dx12->GetSRVHeap());
	//	obj3d[i].Draw(dx12->GetCommandList(), material, dx12->GetSRVHeap(), NSceneManager::GetPipeline3d()->vbView, NSceneManager::GetPipeline3d()->ibView, NSceneManager::GetPipeline3d()->numIB, NSceneManager::GetTex()[0].incrementSize);
	//}

	//�O�i�X�v���C�g
	for (size_t i = 0; i < maxForeSprite; i++)
	{
		foreSprite[i]->CommonBeginDraw(dx12->GetCommandList(), NSceneManager::GetPipelineSprite()->pipelineSet.pipelineState, NSceneManager::GetPipelineSprite()->pipelineSet.rootSig.entity, dx12->GetSRVHeap());
		foreSprite[i]->Draw(dx12->GetSRVHeap(), NSceneManager::GetTex()[0].incrementSize, dx12->GetCommandList());
	}
	// 4.�`��R�}���h�����܂�
#pragma endregion
}

void NStageSelectScene::Finalize()
{
	for (size_t i = 0; i < maxForeSprite; i++)
	{
		delete foreSprite[i];
	}

	//for (size_t i = 0; i < maxBackSprite; i++)
	//{
	//	delete backSprite[i];
	//}
}