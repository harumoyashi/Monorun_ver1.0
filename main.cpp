#include "NWindows.h"
#include "NDX12.h"
#include "NGameScene.h"

//Windows�A�v���ł̃G���g���[�|�C���g(main�֐�)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
#pragma region WindowsAPI������
	NWindows* win = nullptr;	//�E�B���h�E�N���X
	win = NWindows::GetInstance();
	win->Set();
	win->CreateWindowObj();
	win->Display();
#pragma endregion
#pragma region DirectX������
	NDX12* dx12 = nullptr;
	dx12 = NDX12::GetInstance();
	dx12->Init(win);
	//input������
	NInput::KeyInit(win->GetHInstance(), win->GetHwnd());
#pragma endregion
#pragma region �Q�[���V�[��������
	NGameScene* gameScene;
	gameScene = new NGameScene();
	gameScene->Initialize(dx12);
#pragma endregion
	//�Q�[�����[�v
	while (true)
	{
#pragma region �E�B���h�E���b�Z�[�W����
		if (win->WindowMessage()) { break; }
#pragma endregion
#pragma region DirectX���t���[������
		//DirectX���t���[���@��������
		NInput::KeyUpdate();
		gameScene->Update();
		gameScene->Draw(dx12);
	}
#pragma endregion
#pragma region WindowsAPI��n��
	win->Finalize();
	gameScene->Finalize();
	delete gameScene;
#pragma endregion
	return 0;
}