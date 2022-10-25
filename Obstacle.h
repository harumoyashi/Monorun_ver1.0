#pragma once
#include "Util.h"
#include "NDX12.h"

#include <vector>
#include <wrl.h>

#include "NRootParam.h"
#include "NMaterial.h"
#include "NCube.h"
#include "NParticle.h"
#include "NGPipeline.h"
#include "NRootSignature.h"
#include "NObj3d.h"
#include "NTexture.h"
#include "NSprite.h"
#include "NPreDraw.h"
#include "NReaction.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

class Obstacle {
	/// --メンバ変数-- ///
public:

private:
	// --オブジェクト-- //
	NObj3d object_[3];

	// --ブロックの種類
	int blockType_;

	// --衝突フラグ-- //
	bool isCollision_ = false;

	//リアクション//
	NReaction* reaction;

	std::unique_ptr<NParticle> particle;	//パーティクル

	/// --メンバ変数END-- ///
	/// --------------- ///
	/// --メンバ関数-- ///
public:
	// --コンストラクタ-- //
	Obstacle(NDX12* dx12, XMFLOAT3 pos, int blockType);

	// --デストラクタ-- //
	~Obstacle();

	// --初期化処理-- //
	void Initialize();

	// --更新処理-- //
	void Update(XMMATRIX matView, XMMATRIX matProjection);

	// --描画処理-- //
	void Draw(NDX12* dx12, NMaterial material,NCube*cube);

	// --オブジェクトを参照-- //
	BoxObj GetBoxObj();

	// --ブロックの種類を参照
	int GetBlockType();

	// --衝突フラグの取得-- //
	bool GetIsCol() { return isCollision_; }

	// --衝突フラグの設定-- //
	void SetIsCol(bool isCollision) { isCollision_ = isCollision; }

private:
	
	/// --メンバ関数END-- ///
};