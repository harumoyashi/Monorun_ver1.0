#include "Obstacle.h"
#include "StageManager.h"
#include "NSceneManager.h"

enum struct ShapeType {
	None,		// 無効
	Default,	// 横向き長方形
	Iphone,		// 縦向き長方形
	LongWallet,	// 覆いつくすほどの横向き長方形
};

// --コンストラクタ-- //
Obstacle::Obstacle(NDX12* dx12, XMFLOAT3 pos, int blockType) {
	// --プレイヤーオブジェクト(定数バッファ)-- //
	object_.Initialize(dx12->GetDevice());
	object_.texNum = BLOCK;
	object_.scale = { 32.0f, 32.0f, 32.0f };
	object_.position = pos;

	// --ブロックの種類
	blockType_ = blockType;
}

// --デストラクタ
Obstacle::~Obstacle() {

}

// --初期化処理
void Obstacle::Initialize() {
	object_.UpdateMatrix();
}

// --更新処理
void Obstacle::Update(XMMATRIX& matView, XMMATRIX& matProjection, XMFLOAT3& eye, XMFLOAT3& target, XMFLOAT3& up) {
	//ビュー変換行列作成
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
	object_.TransferMatrix(matView, matProjection);
}

// --描画処理
void Obstacle::Draw(NDX12* dx12, NMaterial material) {
	// --オブジェクト描画-- //
	object_.CommonBeginDraw(dx12->GetCommandList(), NSceneManager::GetPipeline3d()->pipelineSet.pipelineState, NSceneManager::GetPipeline3d()->pipelineSet.rootSig.entity, dx12->GetSRVHeap());
	object_.Draw(dx12->GetCommandList(), material, dx12->GetSRVHeap(), NSceneManager::GetPipeline3d()->vbView, NSceneManager::GetPipeline3d()->ibView, NSceneManager::GetPipeline3d()->numIB, NSceneManager::GetTex()[0].incrementSize);
}

//// --オブジェクトを参照-- //
//BoxObj Obstacle::GetBoxObj() {
//	return object_;
//}

// --ブロックの種類を参照
int Obstacle::GetBlockType() {
	return blockType_;
}