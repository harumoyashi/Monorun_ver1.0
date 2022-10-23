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

void StageManager::Initialize()
{
	// --障害物を削除-- //
	obstacles_.clear();

	// --最大コイン数-- //
	maxCoin_ = 0;

	// --現在のコイン数-- //
	coin_ = 0;

	// --CSVの行数-- //
	lineCounter_ = 0;
}

void StageManager::Update(XMMATRIX& matView, XMMATRIX& matProjection, XMFLOAT3& eye, XMFLOAT3& target, XMFLOAT3& up)
{
	// --障害物の描画
	for (int i = 0; i < obstacles_.size(); i++) {
		obstacles_[i].Update(matView, matProjection, eye, target, up);
	}

	/*for (size_t i = 0; i < rightWalls_.size(); i++) {
		leftWalls_[i].Update(matView, matProjection, eye, target, up);
	}

	for (size_t i = 0; i < rightWalls_.size(); i++) {
		rightWalls_[i].Update(matView, matProjection, eye, target, up);
	}*/
}

void StageManager::Draw(NDX12* dx12, NMaterial material,NCube*cube)
{
	// --障害物の描画
	for (int i = 0; i < obstacles_.size(); i++) {
		obstacles_[i].Draw(dx12, material,cube);
	}

	for (size_t i = 0; i < rightWalls_.size(); i++) {
		leftWalls_[i].Draw(dx12, material,cube);
	}

	for (size_t i = 0; i < rightWalls_.size(); i++) {
		rightWalls_[i].Draw(dx12, material,cube);
	}
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
		Obstacle lObj(dx12, {-352.0f, -static_cast<float>(i * blockSize_ + 32.0f), 0.0f}, BLOCK);
		leftWalls_.push_back(lObj);

		Obstacle rObj(dx12, { 352.0f, -static_cast<float>(i * blockSize_ + 32.0f), 0.0f }, BLOCK);
		rightWalls_.push_back(rObj);
	}

	// --障害物の初期化
	for (int i = 0; i < obstacles_.size(); i++) {
		obstacles_[i].Initialize();
	}

	lineCounter_ = loopCount;
}

int StageManager::GetLineCount() { return lineCounter_; }

// --ブロックサイズを参照-- //
int StageManager::GetBlockSize() { return blockSize_; }

// --コインの数に1追加-- //
void StageManager::AddCoin() { coin_++; }

// --読み込むファイルをセットする-- //
void StageManager::SetCSV(int num) {
	if (num == 1) {
		path_ = "csv/check_progress_gimic1.csv";
	}
	else if (num == 2) {
		path_ = "csv/check_progress_gimic2.csv";
	}
	else if (num == 3) {
		path_ = "csv/check_progress_gimic3.csv";
	}
	else if (num == 4) {
		path_ = "csv/check_progress_gimic4.csv";
	}
	else if (num == 5) {
		path_ = "csv/check_progress_gimic5.csv";
	}
	else if (num == 6) {
		path_ = "csv/check_progress_gimic6.csv";
	}
}