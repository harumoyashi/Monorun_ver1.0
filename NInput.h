#pragma once
#define DIRECTINPUT_VERSION 0x0800 // DirectInput�̃o�[�W�����w��
#include <dinput.h>

#include <wrl.h>

class NInput
{
public:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

private:
	static ComPtr<IDirectInputDevice8> keyboard;
	static ComPtr<IDirectInput8> directInput;

public:
	//input������
	static void KeyInit(HINSTANCE hInstance, HWND hwnd);
	//input�X�V
	static void KeyUpdate();

	//�L�[�{�[�h���͏����p (�Ԃ�l0,1)
	//�������ςȂ�
	static bool IsKeyDown(UINT8 key);	//UINT�̓r�b�g���w�肵����^���R�ۂ�
	//�������u��
	static bool IsKeyTrigger(UINT8 key);
	//�����Ă�Ƃ�
	static bool IsKeyReleased(UINT8 key);
	//�������u��
	static bool IsKeyReleaseTrigger(UINT8 key);
private:
	NInput() = delete;
};