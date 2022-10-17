#pragma once
#include <string>
#include <windows.h>
#include <chrono>

const int win_width = 1280;
const int win_height = 720;

class NWindows
{
public:
	//�E�B���h�E�N���X�̐������o�^
	WNDCLASSEX w{};
	//�E�B���h�E�n���h���̐���
	HWND hwnd;
	//�E�B���h�E�T�C�Y�p�̒����`�̐���
	RECT wrc;
	//���b�Z�[�W���\���̂̐���
	MSG msg = {};

	//fps�Œ�p
	std::chrono::system_clock::time_point preTime;
	const long long fps = 60;	//60FPS
	bool continueFlag = true;

public:
	//�V���O���g���C���X�^���X�擾
	static NWindows* GetInstance();

	//�E�B���h�E�v���V�[�W��
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	//Window�N���X�̐ݒ�
	void Set();
	//�R���\�[���ւ̕����o��
	void DebugText(LPCSTR text);
	//�E�B���h�E�I�u�W�F�N�g�̐���
	void CreateWindowObj();
	//�E�B���h�E�\��
	void Display();

	bool WindowMessage();

	//FPS�Œ�
	int FPSDelay();
	//FPS�ݒ�
	void SetFPS();
};

