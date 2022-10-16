#pragma once
#include <string>
#include <windows.h>

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
};

