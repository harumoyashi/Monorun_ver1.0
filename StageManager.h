#pragma once
#include <memory>
#include <string>
#include <array>
#include <vector>
#include "Obstacle.h"

enum BlockId
{
	None,   // -> �Ȃɂ��Ȃ�
	Coin,// -> �R�C��
	BoundBlock,// -> �o�E���h�u���b�N
	DeathBlock,// -> �f�X�u���b�N
	GoalBlock,// -> �S�[���u���b�N
	WallBlock
};

class StageManager {
	/// --�����o�ϐ�-- ///
public:
	NMaterial whiteMaterial_;
	NMaterial blackMaterial_;
	NMaterial redMaterial_;
	NMaterial greenMaterial_;
	NMaterial blueMaterial_;
	NMaterial goalMaterial_;

	// --�I�u�W�F�N�g-- //
	std::vector<Obstacle> obstacles_;

	// --�ǃI�u�W�F�N�g-- //
	std::vector<Obstacle> leftWalls_;
	std::vector<Obstacle> rightWalls_;

	std::unique_ptr<NCube> crystalCube;	//�N���X�^�����(���_�A�C���f�b�N�X)

	// --1�}�X�̃T�C�Y-- //
	const int blockSize_;

	// csv�s��
	int lineCounter_;

	int selectStage_;

private:
	// --�C���X�^���X-- //
	static StageManager* myInstance_;

	// --�ő�R�C����-- //
	int maxCoin_ = 0;

	// --���݂̃R�C����-- //
	int coin_ = 0;

	// --�ǂݍ���CSV�̃p�X-- //
	std::string path_;

	/// --�����o�ϐ�END-- ///
	/// --------------- ///
	/// --�����o�֐�-- ///
public:
	// --�C���X�^���X�擾-- //
	static StageManager* GetInstance();

	// --���������-- //
	static void Release();

	// --����������-- //
	void Initialize(NDX12 * dx12);

	// --�X�V����-- //
	void Update(XMMATRIX matView, XMMATRIX matProjection);

	// --�`�揈��-- //
	void Draw(NDX12* dx12, NMaterial material,NCube* cube);

	// --���Z�b�g����-- //
	void Reset();

	// --CSV��ǂݍ���-- //
	void LoadCSV(NDX12* dx12);

	// --�I������-- //
	void Finalize();

	// --�ǂݍ���CSV�̍s�����Q��-- //
	int GetLineCount();

	// --�u���b�N�T�C�Y���Q��-- //
	int GetBlockSize();

	// --�R�C���̐���1�ǉ�-- //
	void AddCoin();

	int GetMaxCoin() { return maxCoin_; }

	int GetCoin() { return coin_; }

	// --�ǂݍ��ރt�@�C�����Z�b�g����-- //
	void SetCSV(int num);

	// --���ݑI��ł���X�e�[�W���擾-- //
	int GetSelectStage() { return selectStage_; }

private:

	// --�R���X�g���N�^-- //
	StageManager();

	// --�f�X�g���N�^-- //
	~StageManager();

	// --�R�s�[�R���X�g���N�^�֎~-- //
	StageManager(const StageManager&) = delete;

	// --�R�s�[������Z�q�֎~-- //
	StageManager& operator=(const StageManager&) = delete;

	/// --�����o�֐�END-- ///
};