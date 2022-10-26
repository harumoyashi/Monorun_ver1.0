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
	TITLE,				//�^�C�g���摜
	START,				//�X�^�[�gUI
	PLAYER,				//�v���C���[�e�N�X�`��
	BLOCK,				//�u���b�N�e�N�X�`��
	NUMBER,				//�����{�����_
	WHITEPNG,			//0xffffff��64x64(px)��png
	STAGESELECTTEXT,	//�X�e�[�W�Z���N�g�e�L�X�g
	RESULTTEXT,			//���U���g�e�L�X�g
	RETRYTEXT,			//���g���C�e�L�X�g
	NEXTTEXT,			//�l�N�X�g�e�L�X�g
	STAGESELECTFRAME,	//�X�e�[�W�Z���N�g�g
	TIMETEXT,			//�^�C���e�L�X�g
	BIGNUMBER,			//�傫������+�����_
	GOTEXT,				//[GO]�e�L�X�g
	MINUS,				//[-]�e�L�X�g
	KMTEXT,				//[/km]�e�L�X�g
	SLASHTEXT,			//[/]�e�L�X�g
	CRYSTALTEXT,		//[CRYSTAL]�e�L�X�g

	MAXTEX
};

class NSceneManager
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	NRootParam rootParams;	//���[�g�p�����[�^

	//�e�N�X�`��
	static const int maxTex = MAXTEX;	//�e�N�X�`����
	static NTexture tex[maxTex];	//�e�N�X�`��

	//�O���t�B�b�N�X�p�C�v���C��
	static NGPipeline* gPipe3d;		//3D�I�u�W�F�N�g�p�O���t�B�b�N�X�p�C�v���C��
	static NGPipeline* gPipeSprite;	//�X�v���C�g�p�O���t�B�b�N�X�p�C�v���C��

	NTitleScene* titleScene;
	NStageSelectScene* stageSelectScene;
	NGameScene* gameScene;	//�Q�[���V�[��

	static int nextScene_;	// ���̃V�[��
	static int scene;		// ���݂̃V�[��
	static bool isSceneChange;	//�V�[���̏������t���O
	static bool isActiveEffect_;	// �G�t�F�N�g�̋N���t���O
	static bool isPlayEffect_;		// �G�t�F�N�g�̍Đ��t���O ��getter�p
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
	static bool GetPlayEffect() { return isPlayEffect_; }
#pragma endregion
private:
	Effect effect_{ static_cast<int>(EffectType::CToA) };
};