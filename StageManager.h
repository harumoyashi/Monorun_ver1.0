#pragma once
#include <memory>
#include <string>
#include <array>
#include <vector>
#include "Obstacle.h"

enum BlockId
{
	None,   // -> なにもなし
	Coin,// -> コイン
	BoundBlock,// -> バウンドブロック
	DeathBlock// -> デスブロック
};

class StageManager {
	/// --メンバ変数-- ///
public:
	// --マップ-- //
	std::vector<int[10]> map_;

	// --オブジェクト-- //
	std::vector<Obstacle> obstacles_;

	// --壁オブジェクト-- //
	std::vector<Obstacle> leftWalls_;
	std::vector<Obstacle> rightWalls_;

	// --1マスのサイズ-- //
	const int blockSize_;

	// csv行数
	int lineCounter_;

private:
	// --インスタンス-- //
	static StageManager* myInstance_;

	// --最大コイン数-- //
	int maxCoin_ = 0;

	// --現在のコイン数-- //
	int coin_ = 0;

	// --読み込むCSVのパス-- //
	std::string path_;

	/// --メンバ変数END-- ///
	/// --------------- ///
	/// --メンバ関数-- ///
public:
	// --インスタンス取得-- //
	static StageManager* GetInstance();

	// --メモリ解放-- //
	static void Release();

	// --初期化処理-- //
	void Initialize();

	// --更新処理-- //
	void Update(XMMATRIX& matView, XMMATRIX& matProjection, XMFLOAT3& eye, XMFLOAT3& target, XMFLOAT3& up);

	// --描画処理-- //
	void Draw(NDX12* dx12, NMaterial material,NCube* cube);

	// --CSVを読み込む-- //
	void LoadCSV(NDX12* dx12);

	// --読み込んだCSVの行数を参照-- //
	int GetLineCount();

	// --ブロックサイズを参照-- //
	int GetBlockSize();

	// --コインの数に1追加-- //
	void AddCoin();

	// --読み込むファイルをセットする-- //
	void SetCSV(int num);

private:

	// --コンストラクタ-- //
	StageManager();

	// --デストラクタ-- //
	~StageManager();

	// --コピーコンストラクタ禁止-- //
	StageManager(const StageManager&) = delete;

	// --コピー代入演算子禁止-- //
	StageManager& operator=(const StageManager&) = delete;

	/// --メンバ関数END-- ///
};