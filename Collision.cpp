#include "Collision.h"

bool BoxCollision(BoxObj box1, BoxObj box2) {
	if (box1.pos.x - box1.radius >= box2.pos.x + box2.radius) return false;
	if (box1.pos.x + box1.radius <= box2.pos.x - box2.radius) return false;
	if (box1.pos.y - box1.radius >= box2.pos.y + box2.radius) return false;
	if (box1.pos.y + box1.radius <= box2.pos.y - box2.radius) return false;
	return true;
}

bool BoxCenterCol(BoxObj box1, BoxObj box2) {
	if (box1.pos.x - box1.radius > box2.pos.x + box2.radius) return false;
	if (box1.pos.x + box1.radius < box2.pos.x - box2.radius) return false;
	return true;
}

enum Dire {
	UP = 1,
	DOWN = 2,
	LEFT = 3,
	RIGHT = 4
};

int BoxXYCol(BoxObj box1, BoxObj box2) {


	float len1 = abs((box1.pos.y - box1.radius) - (box2.pos.y + box2.radius));
	float len2 = abs((box1.pos.y + box1.radius) - (box2.pos.y - box2.radius));
	float len3 = abs((box1.pos.x - box1.radius) - (box2.pos.x + box2.radius));
	float len4 = abs((box1.pos.x + box1.radius) - (box2.pos.x - box2.radius));

	if (len1 <= len2) {
		if (len1 <= len3) {
			if (len1 <= len4) return 1;
		}
	}

	if (len2 <= len1) {
		if (len2 <= len3) {
			if (len2 <= len4) return 2;
		}
	}

	if (len3 <= len1) {
		if (len3 <= len2) {
			if (len3 <= len4) return 3;
		}
	}

	return 4;
}

// --インスタンスにNULLを代入-- //
Collision* Collision::myInstance_ = nullptr;

// --インスタンス読み込み-- //
Collision* Collision::GetInstance() {
	// --インスタンスが無かったら生成する-- //
	if (myInstance_ == nullptr) myInstance_ = new Collision();

	// --インスタンスを返す-- //
	return myInstance_;
}

// --メモリ解放-- //
void Collision::Release() {
	// --メモリ解放-- //
	delete myInstance_;

	// --NULLを代入-- //
	myInstance_ = nullptr;
}

// --コンストラクタ
Collision::Collision() {
	player_ = Player::GetInstance();
	stage_ = StageManager::GetInstance();

	scrollY_ = 0.0f;
}

// --デストラクタ
Collision::~Collision() {
	
}

void Collision::Reset() {
	oldObj_ = { {-360.0f, -96.0f}, 0.0f };
	//particles.clear();
}

void Collision::Finalize() {
	delete audio;
}

// --初期化処理
void Collision::Initialize() {
	audio = audio = new NAudio();
	audio->Initialize();
	soundData[0] = audio->LoadWave("hit_bound.wav");	// バウンドブロックに当たる音
	soundData[1] = audio->LoadWave("hit_deathBlock.wav");// デスブロックにあたる音
	soundData[2] = audio->LoadWave("break.wav");		// デスブロック壊す音
	soundData[3] = audio->LoadWave("coin_get.wav");		// コイン取る音

	particles = std::make_unique<NParticle[]>(10);
}

void Collision::Update(NDX12* dx12, XMMATRIX matView, XMMATRIX matProjection) {
	scrollY_ = 0.0f;

	if (player_->GetIsColActive() == true) {
		// --最新プレイヤー情報を格納-- //
		BoxObj newObj = player_->GetBoxObj();

		// --古いプレイヤーから最新プレイヤーまでのベクトルを計算-- //
		NVector2 old2newVec = newObj.pos - oldObj_.pos;

		bool isOutLoop = false;
		// --障害物の衝突フラグをOFFにする-- //
		for (size_t i = 0; i < stage_->obstacles_.size(); i++)
		{
			stage_->obstacles_[i].SetIsCol(false);
		}

		// --細かく判定をとるために移動量を10で割って当たり判定をとる-- //
		for (int i = 0; i < 10; i++) {
			if (isOutLoop == true) break;

			bool isCollision = true;

			oldObj_.pos += old2newVec / 10;

			while (isCollision) {
				BoxObj colObs = {};
				int colObsXY = 0;// ----------> どの方向から衝突したら
				int closestObsIndex = 0;// ---> 衝突したなかで一番近い障害物の要素番号
				float length = 100.0f;// -----> 衝突したプレイヤーと障害物の最も短い距離を記録

				// --障害物の数分ループする-- //
				for (int j = 0; j < stage_->obstacles_.size(); j++) {
					BoxObj obstacle = stage_->obstacles_[j].GetBoxObj();

					// --衝突している-- //
					if (BoxCollision(oldObj_, obstacle)) {
						float player2ObsLen = (obstacle.pos - oldObj_.pos).Length();
						if (player2ObsLen <= length) {
							colObsXY = BoxXYCol(oldObj_, obstacle);
							closestObsIndex = j;
							length = player2ObsLen;
							colObs = stage_->obstacles_[j].GetBoxObj();
						}
					}
				}

				if (length == 100.0f) {
					isCollision = false;
				}

				if (isCollision == true) {
					if (stage_->obstacles_[closestObsIndex].GetBlockType() == DeathBlock) {
						audio->PlayWave(soundData[2]);

						// --プレイヤーの状態が通常状態なら-- //
						if (player_->GetState() == NormalAir || player_->GetState() == NormalWallHit) {
							// --プレイヤーの状態を変更-- //
							player_->SetDeath();// -> 死亡状態
							audio->PlayWave(soundData[1]);
						}

						for (size_t j = 0; j < 10; j++) {
							if (particles[j].GetIsActive() == false) {
								BoxObj obsObj = stage_->obstacles_[closestObsIndex].GetBoxObj();
								particles[j].BlockBreakInitialize(dx12->GetDevice(), 1, { obsObj.pos.x, obsObj.pos.y });
								break;
							}
						}

						stage_->obstacles_.erase(stage_->obstacles_.begin() + closestObsIndex);

						//// --プレイヤーが回転状態なら-- //
						//else if (player_->GetState() == RotateWallHit || player_->GetState() == RotateAir) {
						//	// --障害物を削除-- //
						//	stage_->obstacles_.erase(stage_->obstacles_.begin() + colObsIndex[i]);
						//}
					}

					else if (stage_->obstacles_[closestObsIndex].GetBlockType() == Coin) {
						audio->PlayWave(soundData[3], false, 2.0f);
						stage_->AddCoin();
						stage_->obstacles_.erase(stage_->obstacles_.begin() + closestObsIndex);
					}

					else if (stage_->obstacles_[closestObsIndex].GetBlockType() == BoundBlock) {

						audio->PlayWave(soundData[0]);

						// --障害物の衝突フラグをONにする-- //
						stage_->obstacles_[closestObsIndex].SetIsCol(true);

						if (colObsXY >= 3) {
							// --プレイヤーの状態を変える-- //
							player_->SetRotate();// --回転状態に-- //

							// --X軸の移動方向を変える-- //
							player_->ChangeDireX();

							// --プレイヤーと障害物のX軸距離-- //
							float len = abs(oldObj_.pos.x - colObs.pos.x);

							// --プレイヤーと障害物のY軸半径を足した値-- //
							float radius = oldObj_.radius + colObs.radius;

							// --プレイヤーが障害物の右側にいたら-- //
							if (oldObj_.pos.x >= colObs.pos.x) {
								// --プレイヤーに距離と半径を引いた数を加算-- //
								oldObj_.pos.x += radius - len;
								player_->object_->position.x = oldObj_.pos.x;
							}

							// --プレイヤーが障害物の左側にいたら-- //
							else if (oldObj_.pos.x < colObs.pos.x) {
								// --カメラに距離と半径を引いた数を加算-- //
								oldObj_.pos.x -= radius - len;
								player_->object_->position.x = oldObj_.pos.x;
							}
						}

						else if (colObsXY <= 2) {
							// --プレイヤーの状態を変える-- //
							player_->SetRotate();// --回転状態に-- //

							// --プレイヤーのY軸の移動方向を変える-- //
							player_->ChangeDireY();

							// --プレイヤーと障害物のY軸距離-- //
							float len = abs(oldObj_.pos.y - colObs.pos.y);

							// --プレイヤーと障害物のY軸半径を足した値-- //
							float radius = oldObj_.radius + colObs.radius;

							// --プレイヤーが障害物の下側にいたら-- //
							if (oldObj_.pos.y >= colObs.pos.y) {
								// --カメラに距離と半径を引いた数を加算-- //
								oldObj_.pos.y += radius - len;
								player_->object_->position.y += radius - len;
								scrollY_ += radius - len;
							}

							// --プレイヤーが障害物の左側にいたら-- //
							else if (oldObj_.pos.y < colObs.pos.y) {
								// --カメラに距離と半径を引いた数を加算-- //
								oldObj_.pos.y -= radius - len;
								player_->object_->position.y -= radius - len;
								scrollY_ -= radius - len;
							}
						}

						// 
						if (!isCameraShake_) {
							SetCamShakeState(true);
						}

						isOutLoop = true;
					}

					else if (stage_->obstacles_[closestObsIndex].GetBlockType() == GoalBlock) {
						player_->SetGoal();
						return;
					}
				}
			}

			if (i == 9) {
				oldObj_ = newObj;
			}
		}

		player_->object_->UpdateMatrix(matView, matProjection);

		// --古いプレイヤー情報を格納-- //
		oldObj_ = player_->GetBoxObj();
	}

	for (size_t i = 0; i < 10; i++) {
		if (particles[i].GetIsActive() == true) {
			particles[i].BlockBreak(25, matView, matProjection);
		}
	}
}

// --描画処理-- //
void Collision::Draw(NDX12* dx12) {
	for (size_t i = 0; i < 10; i++) {
		if (particles[i].GetIsActive() == true) {
			particles[i].Draw(dx12);
		}
	}
}