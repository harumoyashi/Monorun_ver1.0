#pragma once
#include <string>
#include <windows.h>

class NWindows
{
public:
	static const int win_width = 1280;
	static const int win_height = 720;

private:
	//�E�B���h�E�N���X�̐������o�^
	WNDCLASSEX w{};
	//�E�B���h�E�n���h���̐���
	HWND hwnd;
	//�E�B���h�E�T�C�Y�p�̒����`�̐���
	RECT wrc;
	//���b�Z�[�W���\���̂̐���
	MSG msg = {};

public:
#pragma region ������
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
#pragma endregion
#pragma region �X�V
	//�v���V�[�W���Ƀ��b�Z�[�W�𑗂�
	bool WindowMessage();
#pragma endregion
#pragma region �I��
	//�I������
	//�N���X�̉���
	void Finalize();
#pragma endregion
#pragma region �Q�b�^�[
	HWND GetHwnd()const { return hwnd; }
	HINSTANCE GetHInstance()const { return w.hInstance; }
#pragma endregion
};