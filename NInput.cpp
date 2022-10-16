#include "NInput.h"
#include <cassert>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

// DirectInputの初期化
ComPtr<IDirectInputDevice8> NInput::keyboard;
ComPtr<IDirectInput8> NInput::directInput;

// 全キーの入力状態を取得する
static BYTE keys[256] = {};

// 全キーの1F前の入力状態を取得する
static BYTE prev[256] = {};

void NInput::KeyInit(HINSTANCE hInstance, HWND hwnd)
{
	HRESULT result;

	// DirectInputの初期化
	result = DirectInput8Create(
		hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&directInput, nullptr);
	assert(SUCCEEDED(result));
	// キーボードデバイスの生成
	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	assert(SUCCEEDED(result));
	// 入力データ形式のセット
	result = keyboard->SetDataFormat(&c_dfDIKeyboard); // 標準形式
	assert(SUCCEEDED(result));

	// 排他制御レベルのセット
	result = keyboard->SetCooperativeLevel(
		//DISCL_FOREGROUND：画面が手前にある場合のみ入力を受け付ける
		//DISCL_NONEXCLUSIVE：デバイスをこのアプリだけで専有しない
		//DISCL_NOWINKEY：Windowsキーを無効にする
		hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
}

void NInput::KeyUpdate()
{
	//1F前のキー情報に現在のキー情報を入れてから↓
	memcpy(prev, keys, sizeof(keys));

	//現在のキー情報の取得開始
	keyboard->Acquire();
	keyboard->GetDeviceState(sizeof(keys), keys);
}

//押しっぱなし
bool NInput::IsKeyDown(UINT8 key)
{
	return keys[key];
}

//押した瞬間
bool NInput::IsKeyTrigger(UINT8 key)
{
	return keys[key] && !prev[key];
}

//離してるとき
bool NInput::IsKeyReleased(UINT8 key)
{
	return !keys[key] && !prev[key];
}

//離した瞬間
bool NInput::IsKeyReleaseTrigger(UINT8 key)
{
	return !keys[key] && prev[key];
}