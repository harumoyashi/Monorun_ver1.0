#include "NWindows.h"
#include "NDX12.h"
#include "NGameScene.h"

//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
#pragma region WindowsAPI初期化
	NWindows win;	//ウィンドウクラス
	win.Set();
	win.CreateWindowObj();
	win.Display();
#pragma endregion
#pragma region DirectX初期化
	NDX12* dx12;
	dx12 = new NDX12();
	dx12->Init(win);
	//input初期化
	NInput::KeyInit(win.w.hInstance, win.hwnd);
#pragma endregion
#pragma region ゲームシーン初期化
	NGameScene gameScene;
	gameScene.Initialize(dx12);
#pragma endregion
	//ゲームループ
	while (true)
	{
#pragma region ウィンドウメッセージ処理
		if (PeekMessage(&win.msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&win.msg);	//キー入力メッセージの処理
			DispatchMessage(&win.msg);	//プロシージャにメッセージを送る
		}

		//アプリケーションが終わるときにmwssageがWM_QUITになる
		if (win.msg.message == WM_QUIT)
		{
			break;
		}
#pragma endregion
#pragma region DirectX毎フレーム処理
		//DirectX毎フレーム　ここから
		NInput::KeyUpdate();
		gameScene.Update();
		gameScene.Draw(dx12);
	}
#pragma endregion
#pragma region WindowsAPI後始末
	//もうクラスは使わないので登録解除する
	UnregisterClass(win.w.lpszClassName, win.w.hInstance);
#pragma endregion
	return 0;
}