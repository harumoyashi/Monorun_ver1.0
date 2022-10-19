#include "NSceneManager.h"
#pragma region static�����o�ϐ�������
NGPipeline* NSceneManager::gPipe3d = nullptr;
NGPipeline* NSceneManager::gPipeSprite = nullptr;
//�V�[���̏�����
int NSceneManager::scene = TITLESCENE;
//�V�[���ύX�t���O�̏�����
bool NSceneManager::isSceneChange = false;
#pragma region
NSceneManager* NSceneManager::GetInstance()
{
	static NSceneManager instance;
	return &instance;
}

void NSceneManager::Initialize(NDX12* dx12)
{
	//���[�g�p�����[�^�̐ݒ�
	rootParams.SetDescRange();
	rootParams.SetRootParam();
#pragma region �O���t�B�b�N�X�p�C�v���C��
	gPipe3d = new NGPipeline();
	gPipe3d->Initialize3d(dx12->GetDevice());
	gPipe3d->pipelineSet = gPipe3d->CreatePipeline3d(dx12->GetDevice(), rootParams.entity);

	gPipeSprite = new NGPipeline();
	gPipeSprite->pipelineSet = gPipeSprite->CreatePipelineSprite(dx12->GetDevice(), rootParams.entity);
#pragma endregion
	titleScene->Initialize(dx12);
	stageSelectScene->Initialize(dx12);
	gameScene->Initialize(dx12);
}

void NSceneManager::Update(NDX12* dx12)
{
	// --�^�C�g���V�[���̍X�V����-- //
	if (scene == TITLESCENE) {
		titleScene->Update();
	}

	// --�X�e�[�W�Z���N�g�V�[���̍X�V����-- //
	else if (scene == STAGESELECTSCENE) {
		stageSelectScene->Update();
	}

	// --�Q�[���V�[���̍X�V����-- //
	else if (scene == GAMESCENE) {
		gameScene->Update();
	}

	// --���U���g�V�[���̍X�V����-- //
	else if (scene == RESULTSCENE) {
		//resultScene->Update();
	}

	// --�V�[���ύX�����ꂽ��-- //
	if (isSceneChange == true) {
		// --�^�C�g���V�[����������-- //
		if (scene == TITLESCENE) {
			//���Z�b�g
		}

		// --�X�e�[�W�Z���N�g�V�[���Ȃ�-- //
		else if (scene == STAGESELECTSCENE) {
			//���Z�b�g
		}

		// --�Q�[���V�[���Ȃ�-- //
		else if (scene == GAMESCENE) {
			//���Z�b�g
		}

		// --�V�[���ύX�t���OOFF�ɂ���-- //
		isSceneChange = false;
	}
}

void NSceneManager::Draw(NDX12* dx12)
{
#pragma region �`��O����
	NPreDraw* preDraw = nullptr;
	preDraw = new NPreDraw();

	preDraw->SetResBarrier(dx12->GetSwapchain(), dx12->backBuffers, dx12->GetCommandList());
	preDraw->SetRenderTarget(dx12->GetRTVHeap(), dx12->GetDevice(), dx12->GetRTVHeapDesc(), dx12->GetDSVHeap(), dx12->GetCommandList());
	preDraw->ClearScreen(dx12->GetCommandList());

	preDraw->SetViewport(dx12->GetCommandList());
	preDraw->SetScissorRect(dx12->GetCommandList());
#pragma endregion
	// --�^�C�g���V�[���̕`�揈��-- //
	if (scene == TITLESCENE) {
		titleScene->Draw(dx12);
	}

	// --�X�e�[�W�Z���N�g�V�[���̕`�揈��-- //
	else if (scene == STAGESELECTSCENE) {
		stageSelectScene->Draw(dx12);
	}

	// --�Q�[���V�[���̕`�揈��-- //
	else if (scene == GAMESCENE) {
		gameScene->Draw(dx12);
	}
	dx12->PostDraw(preDraw->barrierDesc);
}

void NSceneManager::Finalize()
{
	titleScene->Finalize();
	stageSelectScene->Finalize();
	gameScene->Finalize();

	delete gPipe3d;
	delete gPipeSprite;
}

void NSceneManager::SetScene(int selectScene)
{
	// --�V�[����ύX-- //
	scene = selectScene;

	// --�V�[���ύX�t���O��ON��-- //
	isSceneChange = true;
}

NSceneManager::NSceneManager()
{
	titleScene = titleScene->GetInstance();
	stageSelectScene = stageSelectScene->GetInstance();
	gameScene = gameScene->GetInstance();
}
