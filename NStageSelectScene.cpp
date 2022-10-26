#include "NStageSelectScene.h"
#include "NSceneManager.h"
#include "NInput.h"
#include "Util.h"
#include <chrono>

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
	//selectStage_ = 1;

	//for (size_t i = 0; i < maxNumSprite; i++) {
	//	numSprite[i]->position.x = 300.0f;
	//	numSprite[i]->position.y = 375.0f + (i * 250.0f);
	//	numSprite[i]->UpdateMatrix();

	//	// --�C�[�W���O�p�ϐ�������-- //
	//	easeStartPos_[i] = { 300.0f, 375.0f + (i * 250.0f), 0.0f };
	//	easeEndPos_[i] = { 300.0f, 375.0f + (i * 250.0f), 0.0f };
	//}
}

void NStageSelectScene::Initialize(NDX12* dx12)
{
#pragma region	�J����������
	camera = std::make_unique<NCamera>();
	camera->ProjectiveProjection();
	camera->CreateMatView();
#pragma endregion
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
	//	backSprite[i]->position.x = i * 300.0f + 375.0f;
	//	backSprite[i]->position.y = 375.0f;
	//	backSprite[i]->UpdateMatrix();
	//	backSprite[i]->TransferMatrix();
	//}

	stage_ = stage_->GetInstance();
	stage_->Initialize(dx12);

	//�O�i�X�v���C�g����
	for (size_t i = 0; i < maxNumSprite; i++) {
		numSprite[i] = std::make_unique<NSprite>();
		numSprite[i]->texNum = static_cast<int>(NUMBER);
		numSprite[i]->CreateClipSprite(dx12->GetDevice(), NSceneManager::GetTex()[numSprite[i]->texNum].texBuff, {i * 48.0f, 0.0f}, {48.0f, 69.0f});
		numSprite[i]->SetColor(1, 1, 1, 1);
		numSprite[i]->size = { 120.0f,180.0f };
		numSprite[i]->TransferVertex();
		numSprite[i]->position.x = 300.0f;
		numSprite[i]->position.y = 375.0f + (i * 250.0f);
		numSprite[i]->UpdateMatrix();

		// --�C�[�W���O�p�ϐ�������-- //
		easeStartPos_[i] = { 300.0f, 375.0f + (i * 250.0f), 0.0f};
		easeEndPos_[i] = { 300.0f, 375.0f + (i * 250.0f), 0.0f};

		frameSprite[i] = std::make_unique<NSprite>();
		frameSprite[i]->texNum = STAGESELECTFRAME;
		frameSprite[i]->CreateSprite(dx12->GetDevice(), NSceneManager::GetTex()[frameSprite[i]->texNum].texBuff);
		frameSprite[i]->SetColor(1, 1, 1, 1);
		frameSprite[i]->size = { 250.0f,250.0f };
		frameSprite[i]->TransferVertex();
		frameSprite[i]->position.x = 300.0f;
		frameSprite[i]->position.y = 375.0f + (i * 250.0f);
		frameSprite[i]->UpdateMatrix();
	}

	// --���Ԍv���ɕK�v�ȃf�[�^�ϐ�-- //
	nowCount_ = 0;
	startCount_ = 0;

	// --�X�N���[�����Ă���̌o�ߎ���-- //
	nowScrollTime_ = 0.0f;

	// --���ݑI��ł���X�e�[�W-- //
	selectStage_ = 1;

#pragma endregion
}

void NStageSelectScene::Update()
{
	// --[SPACE]����������-- //
	if (NSceneManager::GetPlayEffect() == false)
	{
		if (NInput::IsKeyTrigger(DIK_SPACE))
		{
			stage_->SetCSV(selectStage_);
			if (!NSceneManager::GetPlayEffect()) {
				NSceneManager::SetScene(GAMESCENE);
			}
		}
	}

	// --[��]����������-- //
	if (NInput::IsKeyTrigger(DIK_UP)) {
		// --�I��ł���X�e�[�W�̒l��1���傫���Ȃ�-- //
		if (selectStage_ > 1) {
			// --���݂̃J�E���g���擾-- //
			startCount_ = static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count());

			for (size_t i = 0; i < 10; i++) {
				easeStartPos_[i] = numSprite[i]->position;
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
				easeStartPos_[i] = numSprite[i]->position;
				easeEndPos_[i].y -= 250.0f;
			}

			// --���ݑI��ł���X�e�[�W�̒l��1���炷-- //
			selectStage_++;
		}
	}

	// --�o�ߎ���timeRate[s]�̌v�Z
	//nowCount_ = GetNowCount();
	nowCount_ = static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count());
	nowScrollTime_ = static_cast<float>(nowCount_ - startCount_);
	nowScrollTime_ = nowScrollTime_ / 1000.0f;

	float timeRate = Util::Clamp(nowScrollTime_ / maxScrollTime_, 1.0f, 0.0f);

	float len[10];
	for (size_t i = 0; i < 10; i++) {
		numSprite[i]->position.y = Util::EaseOutCubic(easeStartPos_[i].y, easeEndPos_[i].y, timeRate);
		len[i] = abs(375.0f - numSprite[i]->position.y);
		len[i] = Util::Clamp(len[i] / 600.0f, 1.0f, 0.0f);
		numSprite[i]->size.x = 120 * (1.0f - len[i]);
		numSprite[i]->size.y = 180 * (1.0f - len[i]);
		numSprite[i]->TransferVertex();
		numSprite[i]->UpdateMatrix();

		frameSprite[i]->position.y = numSprite[i]->position.y;
		len[i] = abs(375.0f - frameSprite[i]->position.y);
		len[i] = Util::Clamp(len[i] / 600.0f, 1.0f, 0.0f);
		frameSprite[i]->size.x = 250.0f * (1.0f - len[i]);
		frameSprite[i]->size.y = 250.0f * (1.0f - len[i]);
		frameSprite[i]->TransferVertex();
		frameSprite[i]->UpdateMatrix();
	}


#pragma region �s��̌v�Z
	//�r���[�ϊ��s��č쐬
	camera->CreateMatView();

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
	for (size_t i = 0; i < maxNumSprite; i++)
	{
		numSprite[i]->CommonBeginDraw(dx12->GetCommandList(), NSceneManager::GetPipelineSprite()->pipelineSet.pipelineState, NSceneManager::GetPipelineSprite()->pipelineSet.rootSig.entity, dx12->GetSRVHeap());
		numSprite[i]->Draw(dx12->GetSRVHeap(), NSceneManager::GetTex()[0].incrementSize, dx12->GetCommandList());

		frameSprite[i]->CommonBeginDraw(dx12->GetCommandList(), NSceneManager::GetPipelineSprite()->pipelineSet.pipelineState, NSceneManager::GetPipelineSprite()->pipelineSet.rootSig.entity, dx12->GetSRVHeap());
		frameSprite[i]->Draw(dx12->GetSRVHeap(), NSceneManager::GetTex()[0].incrementSize, dx12->GetCommandList());
	}
	// 4.�`��R�}���h�����܂�
#pragma endregion
}

void NStageSelectScene::Finalize()
{
}

//void NStageSelectScene::SetStageSelect(int stage) {
//	selectStage_ = stage;
//	for (size_t i = 0; i < maxNumSprite; i++) {
//		numSprite[i]->position.x = 300.0f;
//		numSprite[i]->position.y = 375.0f + (i * 250.0f) - (250 * (stage -2));
//		numSprite[i]->UpdateMatrix();
//
//		// --�C�[�W���O�p�ϐ�������-- //
//		easeStartPos_[i] = { 300.0f, 375.0f + (i * 250.0f) - (250 * (stage - 2)), 0.0f };
//		easeEndPos_[i] = { 300.0f, 375.0f + (i * 250.0f) - (250 * (stage - 2)), 0.0f };
//	}
//}