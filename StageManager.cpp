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

// --�C���X�^���X��NULL����-- //
StageManager* StageManager::myInstance_ = nullptr;

// --�C���X�^���X�ǂݍ���-- //
StageManager* StageManager::GetInstance() {
	// --�C���X�^���X�����������琶������-- //
	if (myInstance_ == nullptr) myInstance_ = new StageManager();

	// --�C���X�^���X��Ԃ�-- //
	return myInstance_;
}

// --���������-- //
void StageManager::Release() {
	// --���������-- //
	delete myInstance_;

	// --NULL����-- //
	myInstance_ = nullptr;
}

// --�R���X�g���N�^-- //
StageManager::StageManager() :
	// --1�}�X�̃T�C�Y-- //
	blockSize_(64)
{
	// --CSV�̍s��-- //
	lineCounter_ = 0;
}

// --�f�X�g���N�^-- //
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

	// --�ő�R�C����-- //
	maxCoin_ = 0;

	// --���݂̃R�C����-- //
	coin_ = 0;

	// --CSV�̍s��-- //
	lineCounter_ = 0;

	//�����̏��
	crystalCube = std::make_unique<NCube>();
	crystalCube->Initialize(dx12->GetDevice(), CRYSTAL);
}

void StageManager::Update(XMMATRIX matView, XMMATRIX matProjection)
{
	// --��Q���̕`��
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
	// --��Q���̕`��
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

// --���Z�b�g����-- //
void StageManager::Reset() {
	obstacles_.clear();
	leftWalls_.clear();
	rightWalls_.clear();

	// --���݂̃R�C����-- //
	coin_ = 0;

	maxCoin_ = 0;
}

// --�I������-- //
void StageManager::Finalize() {
	obstacles_.clear();
	leftWalls_.clear();
	rightWalls_.clear();
}

void StageManager::LoadCSV(NDX12* dx12)
{
	lineCounter_ = 0;

	// --�ǂݍ���CSV�t�@�C�����J��-- //
	ifstream ifs(path_);

	// --�ǂݍ���CSV�t�@�C����1�s���ꎞ�I�Ɋi�[����ϐ�-- //
	string line;

	// --[,]��؂�œǂݍ��񂾃f�[�^�ꎞ�I�Ɋi�[����ϐ�-- //
	string tmp;

	// --Y���̃}�X�����J�E���g
	size_t loopCount = 0;

	// --1�s���ǂݍ��ށA�ǂݍ���1�s��line�ϐ��Ɋi�[-- //
	while (getline(ifs, line)) {

		// --[,]��؂�œǂݍ��ނ��߂�istringstream�^�ɂ���-- //
		istringstream line_stream(line);

		// --X���̃}�X�����J�E���g
		int cellNumberX = 0;

		// --[,]��؂�œǂݍ��ށA�ǂݍ��񂾕������tmp�Ɋi�[����-- //
		while (getline(line_stream, tmp, ',')) {
			// --�`���ǂݎ��
			int blockType = stoi(tmp);

			// --�ǂݎ����������0�ȊO�Ȃ��Q���𐶐�����
			if (blockType != None) {
				Obstacle objTmp(dx12, { static_cast<float>(cellNumberX * blockSize_ + 32.0f - 320.0f), -static_cast<float>(loopCount * blockSize_ + 32.0f), 0.0f }, blockType);
				obstacles_.push_back(objTmp);

				// --�ǂ݂����������R�C����������-- //
				if (blockType == Coin) {
					// --�ő�R�C����1�ǉ�-- //
					maxCoin_++;
				}
			}

			// --X���̃}�X�����J�E���g
			cellNumberX++;
		}

		// --Y���̃}�X�����J�E���g
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

	// --��Q���̏�����
	for (int i = 0; i < obstacles_.size(); i++) {
		obstacles_[i].Initialize();
	}

	lineCounter_ = static_cast<int>(loopCount);
}

int StageManager::GetLineCount() { return lineCounter_; }

// --�u���b�N�T�C�Y���Q��-- //
int StageManager::GetBlockSize() { return blockSize_; }

// --�R�C���̐���1�ǉ�-- //
void StageManager::AddCoin() { coin_++; }

// --�ǂݍ��ރt�@�C�����Z�b�g����-- //
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