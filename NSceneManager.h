#pragma once
#include "NTitleScene.h"
#include "NStageSelectScene.h"
#include "NGameScene.h"

enum Scene
{
	TITLESCENE,
	STAGESELECTSCENE,
	GAMESCENE,
	RESULTSCENE
};

enum Texture
{
	TITLE,
	START,
	PLAYER,
	MARIO,

	MAXTEX
};

class NSceneManager
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	NRootParam rootParams;	//���[�g�p�����[�^

	//�e�N�X�`��
	static const int maxTex = 4;	//�e�N�X�`����
	static NTexture tex[maxTex];			//�e�N�X�`��

	//�O���t�B�b�N�X�p�C�v���C��
	static NGPipeline* gPipe3d;		//3D�I�u�W�F�N�g�p�O���t�B�b�N�X�p�C�v���C��
	static NGPipeline* gPipeSprite;	//�X�v���C�g�p�O���t�B�b�N�X�p�C�v���C��

	NTitleScene* titleScene;
	NStageSelectScene* stageSelectScene;
	NGameScene* gameScene;	//�Q�[���V�[��

	static int scene;		//���݂̃V�[��
	static bool isSceneChange;	//�V�[���̏������t���O

public:
	

public:
	//�C���X�^���X�擾
	NSceneManager* GetInstance();

	//������
	void Initialize(NDX12* dx12);
	//�X�V
	void Update(NDX12* dx12);
	//�`��
	void Draw(NDX12* dx12);
	//�I������
	void Finalize();
	//�V�[���̕ύX
	static void SetScene(int selectScene);

	NSceneManager();

#pragma region �Q�b�^�[
	static NGPipeline* GetPipeline3d() { return gPipe3d; }
	static NGPipeline* GetPipelineSprite() { return gPipeSprite; }
	static NTexture* GetTex() { return tex; }
#pragma endregion
private:


};