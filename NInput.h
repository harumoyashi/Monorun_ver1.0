#pragma once
#define DIRECTINPUT_VERSION 0x0800 // DirectInputのバージョン指定
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
	//input初期化
	static void KeyInit(HINSTANCE hInstance, HWND hwnd);
	//input更新
	static void KeyUpdate();

	//キーボード入力処理用 (返り値0,1)
	//押しっぱなし
	static bool IsKeyDown(UINT8 key);	//UINTはビット数指定したら型自由ぽい
	//押した瞬間
	static bool IsKeyTrigger(UINT8 key);
	//離してるとき
	static bool IsKeyReleased(UINT8 key);
	//離した瞬間
	static bool IsKeyReleaseTrigger(UINT8 key);
private:
	NInput() = delete;
};