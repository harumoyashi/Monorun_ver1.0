#include "NWindows.h"
#include "NDX12.h"
#include "NGameScene.h"

//Windows�A�v���ł̃G���g���[�|�C���g(main�֐�)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
#pragma region WindowsAPI������
	NWindows win;	//�E�B���h�E�N���X
	win.Set();
	win.CreateWindowObj();
	win.Display();
#pragma endregion
#pragma region DirectX������
	NDX12* dx12;
	dx12 = new NDX12();
	dx12->Init(win);
	//input������
	NInput::KeyInit(win.w.hInstance, win.hwnd);
#pragma endregion
#pragma region �Q�[���V�[��������
	NGameScene gameScene;
	gameScene.Initialize(dx12);
#pragma endregion
	//�Q�[�����[�v
	while (true)
	{
#pragma region �E�B���h�E���b�Z�[�W����
		if (PeekMessage(&win.msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&win.msg);	//�L�[���̓��b�Z�[�W�̏���
			DispatchMessage(&win.msg);	//�v���V�[�W���Ƀ��b�Z�[�W�𑗂�
		}

		//�A�v���P�[�V�������I���Ƃ���mwssage��WM_QUIT�ɂȂ�
		if (win.msg.message == WM_QUIT)
		{
			break;
		}
#pragma endregion
#pragma region DirectX���t���[������
		//DirectX���t���[���@��������
		NInput::KeyUpdate();
		gameScene.Update();
		gameScene.Draw(dx12);
	}
#pragma endregion
#pragma region WindowsAPI��n��
	//�����N���X�͎g��Ȃ��̂œo�^��������
	UnregisterClass(win.w.lpszClassName, win.w.hInstance);
#pragma endregion
	return 0;
}