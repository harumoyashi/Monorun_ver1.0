#pragma once
#include <string>
#include <windows.h>
#include <chrono>

const int win_width = 1280;
const int win_height = 720;

class NWindows
{
public:
	//ウィンドウクラスの生成＆登録
	WNDCLASSEX w{};
	//ウィンドウハンドルの生成
	HWND hwnd;
	//ウィンドウサイズ用の長方形の生成
	RECT wrc;
	//メッセージ情報構造体の生成
	MSG msg = {};

	//fps固定用
	std::chrono::system_clock::time_point preTime;
	const long long fps = 60;	//60FPS
	bool continueFlag = true;

public:
	//シングルトンインスタンス取得
	static NWindows* GetInstance();

	//ウィンドウプロシージャ
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	//Windowクラスの設定
	void Set();
	//コンソールへの文字出力
	void DebugText(LPCSTR text);
	//ウィンドウオブジェクトの生成
	void CreateWindowObj();
	//ウィンドウ表示
	void Display();

	bool WindowMessage();

	//FPS固定
	int FPSDelay();
	//FPS設定
	void SetFPS();
};

