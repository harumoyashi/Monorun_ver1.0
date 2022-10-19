#include "NStageSelectScene.h"
#include "NSceneManager.h"
#include "NInput.h"

NStageSelectScene* NStageSelectScene::GetInstance()
{
	static NStageSelectScene instance;
	return &instance;
}

void NStageSelectScene::Initialize()
{
}

void NStageSelectScene::Update()
{
	if (NInput::IsKeyTrigger(DIK_SPACE))
	{
		NSceneManager::SetScene(GAMESCENE);
	}
}

void NStageSelectScene::Draw()
{
}
