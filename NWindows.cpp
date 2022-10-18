#include "NWindows.h"

#pragma comment(lib,"winmm.lib")

NWindows* NWindows::GetInstance()
{
	static NWindows instance;
	return &instance;
}

//�E�B���h�v���V�[�W��
//�ʓ|�����Ǐ����Ȃ��Ⴂ���Ȃ��֐�
LRESULT NWindows::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
		//�E�B���h�E���j�����ꂽ��Ă΂��
	case WM_DESTROY:
		PostQuitMessage(0);	//OS�ɑ΂��āu�������̃A�v���͏I���v�Ɠ`����
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);	//����̏������s��
}

//Window�N���X�̐ݒ�
void NWindows::Set()
{
	w.cbSize = sizeof(WNDCLASSEX);
	w.lpfnWndProc = (WNDPROC)WindowProc;		//�E�B���h�E�v���V�[�W����ݒ�
	w.lpszClassName = L"DX12Sample";			//�A�v���P�[�V�����N���X��
	w.hInstance = GetModuleHandle(nullptr);		//�n���h���̎擾
	w.hCursor = LoadCursor(NULL, IDC_ARROW);	//�J�[�\���w��

	RegisterClassEx(&w);	//�A�v���P�[�V�����N���X�i�E�B���h�E�N���X�̎w���OS�ɓ`����j

	wrc = { 0,0,win_width,win_height };	//�E�B���h�E�T�C�Y�����߂�

	//�֐����g���ăE�B���h�E�̃T�C�Y��␳����
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	//�V�X�e���^�C�}�[�̕���x���グ��
	timeBeginPeriod(1);
}

//�R���\�[���ւ̕����o��
void NWindows::DebugText(LPCSTR text)
{
	OutputDebugStringA(text);
}

//�E�B���h�E�I�u�W�F�N�g�̐���
void NWindows::CreateWindowObj()
{
	//�E�B���h�E�I�u�W�F�N�g�̐���
	hwnd = CreateWindow(
		w.lpszClassName,		//�N���X���w��
		L"DX12�e�X�g",			//�^�C�g���o�[�̕���
		WS_OVERLAPPEDWINDOW,	//�^�C�g���o�[�Ƌ��E��������E�B���h�E
		CW_USEDEFAULT,			//�\��x���W��OS�ɂ��C��
		CW_USEDEFAULT,			//�\��y���W��OS�ɂ��C��
		wrc.right - wrc.left,	//�E�B���h�E��
		wrc.bottom - wrc.top,	//�E�B���h�E��
		nullptr,				//�e�E�B���h�E�n���h��
		nullptr,				//���j���[�n���h��
		w.hInstance,			//�Ăяo���A�v���P�[�V�����n���h��
		nullptr					//�ǉ��p�����[�^�[
	);
}

//�E�B���h�E�\��
void NWindows::Display()
{
	ShowWindow(hwnd, SW_SHOW);
}

bool NWindows::WindowMessage()
{
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);	//�L�[���̓��b�Z�[�W�̏���
		DispatchMessage(&msg);	//�v���V�[�W���Ƀ��b�Z�[�W�𑗂�
	}

	//�A�v���P�[�V�������I���Ƃ���mwssage��WM_QUIT�ɂȂ�
	if (msg.message == WM_QUIT)
	{
		return true;	//�I�����b�Z�[�W�������烋�[�v�𔲂���
	}

	return false;
}

void NWindows::Finalize()
{
	//�����N���X�͎g��Ȃ��̂œo�^��������
	UnregisterClass(w.lpszClassName, w.hInstance);
}
