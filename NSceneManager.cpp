#include "NSceneManager.h"
#pragma region static�����o�ϐ�������
NTexture NSceneManager::tex[];
NGPipeline* NSceneManager::gPipe3d = nullptr;
NGPipeline* NSceneManager::gPipeSprite = nullptr;
//�V�[���̏�����
int NSceneManager::nextScene_ = 0;
int NSceneManager::scene = TITLESCENE;
//�V�[���ύX�t���O�̏�����
bool NSceneManager::isSceneChange = false;
bool NSceneManager::isActiveEffect_ = false;
bool NSceneManager::isPlayEffect_ = false;
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

	//�e�N�X�`������
	tex[0].Load(L"Resources/droppin_title_logo.png");
	tex[1].Load(L"Resources/droppin_start.png");
	tex[2].Load(L"Resources/player_tex.png");
	tex[3].Load(L"Resources/block_tex.png");
	tex[4].Load(L"Resources/StageSelectImage.png");
	tex[5].Load(L"Resources/white64x64.png");

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
#pragma region �O���t�B�b�N�X�p�C�v���C��
	gPipe3d = new NGPipeline();
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

	// --�V�[���ύX�����ꂽ��-- //
	if (isActiveEffect_) {
		effect_.Activate();
		isActiveEffect_ = false;
	}

	if (effect_.GetAllowChangeScene()) {
		scene = nextScene_;
		isSceneChange = true;
	}

	if (isSceneChange) {
		// --�^�C�g���V�[����������-- //
		if (scene == TITLESCENE) {
			//���Z�b�g
		}

		// --�X�e�[�W�Z���N�g�V�[���Ȃ�-- //
		else if (scene == STAGESELECTSCENE) {
			//���Z�b�g
			stageSelectScene->Reset();
		}

		// --�Q�[���V�[���Ȃ�-- //
		else if (scene == GAMESCENE) {
			//���Z�b�g
			gameScene->Reset(dx12);
		}

		// --�V�[���ύX�t���OOFF�ɂ���-- //
		//isSceneChange = false;

		isSceneChange = false;

	}

	// �G�t�F�N�gUpdate
	effect_.ExpandSquareUpdate();

	isPlayEffect_ = effect_.GetEffectPlay();
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
	effect_.ExpandSquareDraw();
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
	nextScene_ = selectScene;

	// --�V�[���ύX�t���O��ON��-- //
	//isSceneChange = true;

	isActiveEffect_ = true;
}

NSceneManager::NSceneManager()
{
	titleScene = NTitleScene::GetInstance();
	stageSelectScene = NStageSelectScene::GetInstance();
	gameScene = NGameScene::GetInstance();
}
