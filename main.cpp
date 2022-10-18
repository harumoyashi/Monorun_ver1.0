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
	NInput::KeyInit(win->GetHInstance(), win->GetHwnd());
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
		if (win->WindowMessage()) { break; }
#pragma endregion
#pragma region DirectX毎フレーム処理
		//DirectX毎フレーム　ここから
		NInput::KeyUpdate();
		gameScene->Update();
		gameScene->Draw(dx12);
	}
#pragma endregion
#pragma region WindowsAPI後始末
	win->Finalize();
	gameScene->Finalize();
	delete gameScene;
#pragma endregion
	return 0;
}