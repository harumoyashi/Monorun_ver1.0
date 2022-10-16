#include "NInput.h"
#include <cassert>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

// DirectInput�̏�����
ComPtr<IDirectInputDevice8> NInput::keyboard;
ComPtr<IDirectInput8> NInput::directInput;

// �S�L�[�̓��͏�Ԃ��擾����
static BYTE keys[256] = {};

// �S�L�[��1F�O�̓��͏�Ԃ��擾����
static BYTE prev[256] = {};

void NInput::KeyInit(HINSTANCE hInstance, HWND hwnd)
{
	HRESULT result;

	// DirectInput�̏�����
	result = DirectInput8Create(
		hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&directInput, nullptr);
	assert(SUCCEEDED(result));
	// �L�[�{�[�h�f�o�C�X�̐���
	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	assert(SUCCEEDED(result));
	// ���̓f�[�^�`���̃Z�b�g
	result = keyboard->SetDataFormat(&c_dfDIKeyboard); // �W���`��
	assert(SUCCEEDED(result));

	// �r�����䃌�x���̃Z�b�g
	result = keyboard->SetCooperativeLevel(
		//DISCL_FOREGROUND�F��ʂ���O�ɂ���ꍇ�̂ݓ��͂��󂯕t����
		//DISCL_NONEXCLUSIVE�F�f�o�C�X�����̃A�v�������Ő�L���Ȃ�
		//DISCL_NOWINKEY�FWindows�L�[�𖳌��ɂ���
		hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
}

void NInput::KeyUpdate()
{
	//1F�O�̃L�[���Ɍ��݂̃L�[�������Ă��火
	memcpy(prev, keys, sizeof(keys));

	//���݂̃L�[���̎擾�J�n
	keyboard->Acquire();
	keyboard->GetDeviceState(sizeof(keys), keys);
}

//�������ςȂ�
bool NInput::IsKeyDown(UINT8 key)
{
	return keys[key];
}

//�������u��
bool NInput::IsKeyTrigger(UINT8 key)
{
	return keys[key] && !prev[key];
}

//�����Ă�Ƃ�
bool NInput::IsKeyReleased(UINT8 key)
{
	return !keys[key] && !prev[key];
}

//�������u��
bool NInput::IsKeyReleaseTrigger(UINT8 key)
{
	return !keys[key] && prev[key];
}