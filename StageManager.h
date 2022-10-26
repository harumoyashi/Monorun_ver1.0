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
	DeathBlock,// -> デスブロック
	GoalBlock,// -> ゴールブロック
	WallBlock
};

class StageManager {
	/// --メンバ変数-- ///
public:
	NMaterial whiteMaterial_;
	NMaterial blackMaterial_;
	NMaterial redMaterial_;
	NMaterial greenMaterial_;
	NMaterial blueMaterial_;
	NMaterial goalMaterial_;

	// --オブジェクト-- //
	std::vector<Obstacle> obstacles_;

	// --壁オブジェクト-- //
	std::vector<Obstacle> leftWalls_;
	std::vector<Obstacle> rightWalls_;

	std::unique_ptr<NCube> crystalCube;	//クリスタル情報(頂点、インデックス)

	// --1マスのサイズ-- //
	const int blockSize_;

	// csv行数
	int lineCounter_;

	int selectStage_;

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
	void Initialize(NDX12 * dx12);

	// --更新処理-- //
	void Update(XMMATRIX matView, XMMATRIX matProjection);

	// --描画処理-- //
	void Draw(NDX12* dx12, NMaterial material,NCube* cube);

	// --リセット処理-- //
	void Reset();

	// --CSVを読み込む-- //
	void LoadCSV(NDX12* dx12);

	// --終了処理-- //
	void Finalize();

	// --読み込んだCSVの行数を参照-- //
	int GetLineCount();

	// --ブロックサイズを参照-- //
	int GetBlockSize();

	// --コインの数に1追加-- //
	void AddCoin();

	int GetMaxCoin() { return maxCoin_; }

	int GetCoin() { return coin_; }

	// --読み込むファイルをセットする-- //
	void SetCSV(int num);

	// --現在選んでいるステージを取得-- //
	int GetSelectStage() { return selectStage_; }

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