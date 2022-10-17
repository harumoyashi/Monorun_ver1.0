#include "NWindows.h"
#include "NDX12.h"
#include "NGameScene.h"

//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
#pragma region WindowsAPI初期化
	NWindows* win = nullptr;	//ウィンドウクラス
	win = NWindows::GetInstance();
	win->Set();
	win->CreateWindowObj();
	win->Display();
#pragma endregion
#pragma region DirectX初期化
	NDX12* dx12 = nullptr;
	dx12 = NDX12::GetInstance();
	dx12->Init(win);
	//input初期化
	NInput::KeyInit(win->w.hInstance, win->hwnd);
#pragma endregion
#pragma region ゲームシーン初期化
	NGameScene* gameScene;
	gameScene = new NGameScene();
	gameScene->Initialize(dx12);
#pragma endregion
	//ゲームループ
	while (true)
	{
#pragma region ウィンドウメッセージ処理
		if (win->WindowMessage()) { win->continueFlag = false; }
		if (win->continueFlag == false) { break; }
		//fps固定用
		if (win->FPSDelay() && win->continueFlag) { continue; }
#pragma endregion
#pragma region DirectX毎フレーム処理
		//DirectX毎フレーム　ここから
		NInput::KeyUpdate();
		gameScene->Update();
		gameScene->Draw(dx12);
	}
#pragma endregion
#pragma region WindowsAPI後始末
	//もうクラスは使わないので登録解除する
	UnregisterClass(win->w.lpszClassName, win->w.hInstance);
	gameScene->Finalize();
	delete gameScene;
#pragma endregion
	return 0;
}