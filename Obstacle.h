#pragma once
#include "Util.h"
#include "NDX12.h"

#include <vector>
#include <wrl.h>

#include "NRootParam.h"
#include "NMaterial.h"
#include "NCube.h"
#include "NGPipeline.h"
#include "NRootSignature.h"
#include "NObj3d.h"
#include "NTexture.h"
#include "NSprite.h"
#include "NPreDraw.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

class Obstacle {
	/// --メンバ変数-- ///
public:

private:
	// --オブジェクト-- //
	NObj3d object_;

	// --ブロックの種類
	int blockType_;

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
	void Update(XMMATRIX& matView, XMMATRIX& matProjection, XMFLOAT3& eye, XMFLOAT3& target, XMFLOAT3& up);

	// --描画処理-- //
	void Draw(NDX12* dx12, NMaterial material,NCube*cube);

	// --オブジェクトを参照-- //
	BoxObj GetBoxObj();

	// --ブロックの種類を参照
	int GetBlockType();

private:

	/// --メンバ関数END-- ///
};