#include "StageManager.h"
#include <fstream>
#include <array>
#include <sstream>
#include <algorithm>
#include "Util.h"
#include "NSceneManager.h"

namespace {
	using namespace std;
}

// --インスタンスにNULLを代入-- //
StageManager* StageManager::myInstance_ = nullptr;

// --インスタンス読み込み-- //
StageManager* StageManager::GetInstance() {
	// --インスタンスが無かったら生成する-- //
	if (myInstance_ == nullptr) myInstance_ = new StageManager();

	// --インスタンスを返す-- //
	return myInstance_;
}

// --メモリ解放-- //
void StageManager::Release() {
	// --メモリ解放-- //
	delete myInstance_;

	// --NULLを代入-- //
	myInstance_ = nullptr;
}

// --コンストラクタ-- //
StageManager::StageManager() :
	// --1マスのサイズ-- //
	blockSize_(64)
{
	// --CSVの行数-- //
	lineCounter_ = 0;
}

// --デストラクタ-- //
StageManager::~StageManager()
{

}

void StageManager::Initialize(NDX12* dx12)
{
	whiteMaterial_.Initialize(dx12->GetDevice());
	whiteMaterial_.SetColor({0.3f, 0.3f, 0.3f, 1.0f });

	blackMaterial_.Initialize(dx12->GetDevice());
	blackMaterial_.SetColor({ 0.0f, 0.0f, 0.0f, 1.0f });

	redMaterial_.Initialize(dx12->GetDevice());
	redMaterial_.SetColor({ 0.95f, 0.05f, 0.05f, 1.0f });

	greenMaterial_.Initialize(dx12->GetDevice());
	greenMaterial_.SetColor({ 0.1f, 0.9f, 0.1f, 1.0f });

	blueMaterial_.Initialize(dx12->GetDevice());
	blueMaterial_.SetColor({ 0.1f, 0.6f, 0.6f, 1.0f });

	goalMaterial_.Initialize(dx12->GetDevice());
	goalMaterial_.SetColor({ 0.9f, 0.9f, 0.0f, 0.3f});

	// --最大コイン数-- //
	maxCoin_ = 0;

	// --現在のコイン数-- //
	coin_ = 0;

	// --CSVの行数-- //
	lineCounter_ = 0;

	//立方体情報
	crystalCube = std::make_unique<NCube>();
	crystalCube->Initialize(dx12->GetDevice(), CRYSTAL);
}

void StageManager::Update(XMMATRIX matView, XMMATRIX matProjection)
{
	// --障害物の描画
	for (int i = 0; i < obstacles_.size(); i++) {
		obstacles_[i].Update(matView, matProjection);
	}

	for (size_t i = 0; i < rightWalls_.size(); i++) {
		leftWalls_[i].Update(matView, matProjection);
	}

	for (size_t i = 0; i < rightWalls_.size(); i++) {
		rightWalls_[i].Update(matView, matProjection);
	}
}

void StageManager::Draw(NDX12* dx12, NMaterial material,NCube*cube)
{
	// --障害物の描画
	for (int i = 0; i < obstacles_.size(); i++) {
		if (obstacles_[i].GetBlockType() == BoundBlock) {
			obstacles_[i].Draw(dx12, blueMaterial_, cube);
		}
		else if (obstacles_[i].GetBlockType() == DeathBlock) {
			obstacles_[i].Draw(dx12, redMaterial_, crystalCube.get());
		}
		else if (obstacles_[i].GetBlockType() == Coin) {
			obstacles_[i].Draw(dx12, greenMaterial_, crystalCube.get());
		}
		else if (obstacles_[i].GetBlockType() == GoalBlock) {
			obstacles_[i].Draw(dx12, goalMaterial_, cube);
		}
	}

	for (size_t i = 0; i < leftWalls_.size(); i++) {
		leftWalls_[i].Draw(dx12, whiteMaterial_, cube);
	}

	for (size_t i = 0; i < rightWalls_.size(); i++) {
		rightWalls_[i].Draw(dx12, whiteMaterial_, cube);
	}
}

// --リセット処理-- //
void StageManager::Reset() {
	obstacles_.clear();
	leftWalls_.clear();
	rightWalls_.clear();

	// --現在のコイン数-- //
	coin_ = 0;

	maxCoin_ = 0;
}

// --終了処理-- //
void StageManager::Finalize() {
	obstacles_.clear();
	leftWalls_.clear();
	rightWalls_.clear();
}

void StageManager::LoadCSV(NDX12* dx12)
{
	lineCounter_ = 0;

	// --読み込むCSVファイルを開く-- //
	ifstream ifs(path_);

	// --読み込んだCSVファイルの1行を一時的に格納する変数-- //
	string line;

	// --[,]区切りで読み込んだデータ一時的に格納する変数-- //
	string tmp;

	// --Y軸のマス数をカウント
	size_t loopCount = 0;

	// --1行ずつ読み込む、読み込んだ1行をline変数に格納-- //
	while (getline(ifs, line)) {

		// --[,]区切りで読み込むためにistringstream型にする-- //
		istringstream line_stream(line);

		// --X軸のマス数をカウント
		int cellNumberX = 0;

		// --[,]区切りで読み込む、読み込んだ文字列をtmpに格納する-- //
		while (getline(line_stream, tmp, ',')) {
			// --形状を読み取る
			int blockType = stoi(tmp);

			// --読み取った数字が0以外なら障害物を生成する
			if (blockType != None) {
				Obstacle objTmp(dx12, { static_cast<float>(cellNumberX * blockSize_ + 32.0f - 320.0f), -static_cast<float>(loopCount * blockSize_ + 32.0f), 0.0f }, blockType);
				obstacles_.push_back(objTmp);

				// --読みった数字がコインだったら-- //
				if (blockType == Coin) {
					// --最大コインに1追加-- //
					maxCoin_++;
				}
			}

			// --X軸のマス数をカウント
			cellNumberX++;
		}

		// --Y軸のマス数をカウント
		loopCount++;
	}

	for (size_t i = 0; i < loopCount; i++) {
		Obstacle lObj(dx12, {-352.0f, -static_cast<float>(i * blockSize_ + 32.0f), 0.0f}, WallBlock);
		leftWalls_.push_back(lObj);
		leftWalls_[i].Initialize();

		Obstacle rObj(dx12, { 352.0f, -static_cast<float>(i * blockSize_ + 32.0f), 0.0f }, WallBlock);
		rightWalls_.push_back(rObj);
		rightWalls_[i].Initialize();
	}

	// --障害物の初期化
	for (int i = 0; i < obstacles_.size(); i++) {
		obstacles_[i].Initialize();
	}

	lineCounter_ = static_cast<int>(loopCount);
}

int StageManager::GetLineCount() { return lineCounter_; }

// --ブロックサイズを参照-- //
int StageManager::GetBlockSize() { return blockSize_; }

// --コインの数に1追加-- //
void StageManager::AddCoin() { coin_++; }

// --読み込むファイルをセットする-- //
void StageManager::SetCSV(int num) {
	if (num == 1) {
		path_ = "csv/a - check_stage_design1.csv";
	}
	else if (num == 2) {
		path_ = "csv/a - check_stage_design2.csv";
	}
	else if (num == 3) {
		path_ = "csv/a - check_stage_design3.csv";
	}
	else if (num == 4) {
		path_ = "csv/a - check_stage_design4.csv";
	}
	else if (num == 5) {
		path_ = "csv/a - check_stage_design5.csv";
	}
	else if (num == 6) {
		path_ = "csv/check_progress_gimic6.csv";
	}
	else if (num == 7) {
		path_ = "csv/check_progress_gimic7.csv";
	}
	selectStage_ = num;
}