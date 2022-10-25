#pragma once
#include <DirectXMath.h>

using namespace DirectX;

class NReaction
{
private:
	const float maxTime = 1.0f;		//全体の時間
	float t;						//経過時間
	float timeRate = 0.08f;			//1フレームごとの増加量
	bool isReaction1 = false;		//1個目のリアクションフラグ(大きくなる)
	bool isReaction2 = false;		//2個目のリアクションフラグ(小さくなる)

	float originScale = 1.0f;	//オブジェクトの元サイズ(1辺)
	float scale = 1.0f;			//リアクションによって変更するサイズ
	XMFLOAT3 objScale = {};		//3軸分にscaleまとめるためのやつ

public:
	//ブロックがプレイヤーを弾くリアクション
	//引数のフラグが一回でも立ったら最後まで実行する
	void Bounce(bool isReaction);
	//リアクションで変更したscaleを渡す用
	XMFLOAT3 GetObjScale() { return objScale; }
	//リアクションするオブジェクトの元のscaleを取ってくる用
	void SetObjScale(XMFLOAT3 scale) { this->objScale = scale; }
	
private:
	//イージング群//
	float EaseInBack(float t);
	float EaseOutQuart(float t);
};