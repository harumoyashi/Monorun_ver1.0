//#include "Collision.h"
//
//bool BoxCollision(BoxObj box1, BoxObj box2) {
//	if (box1.pos.x - box1.radiusX >= box2.pos.x + box2.radiusX) return false;
//	if (box1.pos.x + box1.radiusX <= box2.pos.x - box2.radiusX) return false;
//	if (box1.pos.y - box1.radiusY >= box2.pos.y + box2.radiusY) return false;
//	if (box1.pos.y + box1.radiusY <= box2.pos.y - box2.radiusY) return false;
//	return true;
//}
//
//bool BoxCenterCol(BoxObj box1, BoxObj box2) {
//	if (box1.pos.x - box1.radiusX > box2.pos.x + box2.radiusX) return false;
//	if (box1.pos.x + box1.radiusX < box2.pos.x - box2.radiusX) return false;
//	return true;
//}
//
//enum Dire {
//	UP = 1,
//	DOWN = 2,
//	LEFT = 3,
//	RIGHT = 4
//};
//
//int BoxXYCol(BoxObj box1, BoxObj box2) {
//
//
//	float len1 = abs((box1.pos.y - box1.radiusY) - (box2.pos.y + box2.radiusY));
//	float len2 = abs((box1.pos.y + box1.radiusY) - (box2.pos.y - box2.radiusY));
//	float len3 = abs((box1.pos.x - box1.radiusX) - (box2.pos.x + box2.radiusX));
//	float len4 = abs((box1.pos.x + box1.radiusX) - (box2.pos.x - box2.radiusX));
//
//	if (len1 <= len2) {
//		if (len1 <= len3) {
//			if (len1 <= len4) return 1;
//		}
//	}
//
//	if (len2 <= len1) {
//		if (len2 <= len3) {
//			if (len2 <= len4) return 2;
//		}
//	}
//
//	if (len3 <= len1) {
//		if (len3 <= len2) {
//			if (len3 <= len4) return 3;
//		}
//	}
//
//	return 4;
//}
//
//// --�C���X�^���X��NULL����-- //
//Collision* Collision::myInstance_ = nullptr;
//
//// --�C���X�^���X�ǂݍ���-- //
//Collision* Collision::GetInstance() {
//	// --�C���X�^���X�����������琶������-- //
//	if (myInstance_ == nullptr) myInstance_ = new Collision();
//
//	// --�C���X�^���X��Ԃ�-- //
//	return myInstance_;
//}
//
//// --���������-- //
//void Collision::Release() {
//	// --���������-- //
//	delete myInstance_;
//
//	// --NULL����-- //
//	myInstance_ = nullptr;
//}
//
//// --�R���X�g���N�^
//Collision::Collision() {
//	player_ = player_->GetInstance();
//	stage_ = StageManager::GetInstance();
//}
//
//// --�f�X�g���N�^
//Collision::~Collision() {
//
//}
//
//// --����������
//void Collision::Initialize() {
//
//}
//
//void Collision::Update() {
//	// --�ŐV�v���C���[�����i�[-- //
//	BoxObj newObj = player_->GetBoxObj();
//	newObj.pos.y += Camera::GetScroll();
//
//	// --�Â��v���C���[����ŐV�v���C���[�܂ł̃x�N�g�����v�Z-- //
//	Vector2 old2newVec = newObj.pos - oldObj_.pos;
//
//	float scroll = Camera::GetScroll();
//
//	bool isOutLoop = false;
//
//	// --�ׂ���������Ƃ邽�߂Ɉړ��ʂ�10�Ŋ����ē����蔻����Ƃ�-- //
//	for (int i = 0; i < 10; i++) {
//		if (isOutLoop == true) break;
//
//		bool isCollision = true;
//
//		oldObj_.pos += old2newVec / 10;
//
//		while (isCollision) {
//			BoxObj colObs = {};
//			int colObsXY = 0;// ----------> �ǂ̕�������Փ˂�����
//			int closestObsIndex = 0;// ---> �Փ˂����Ȃ��ň�ԋ߂���Q���̗v�f�ԍ�
//			float length = 100.0f;// -----> �Փ˂����v���C���[�Ə�Q���̍ł��Z���������L�^
//
//			// --��Q���̐������[�v����-- //
//			for (int j = 0; j < stage_->obstacles_.size(); j++) {
//				BoxObj obstacle = stage_->obstacles_[j].GetBoxObj();
//
//				// --�Փ˂��Ă���-- //
//				if (BoxCollision(oldObj_, obstacle)) {
//					float player2ObsLen = (obstacle.pos - oldObj_.pos).length();
//					if (player2ObsLen <= length) {
//						colObsXY = BoxXYCol(oldObj_, obstacle);
//						closestObsIndex = j;
//						length = player2ObsLen;
//						colObs = stage_->obstacles_[j].GetBoxObj();
//					}
//				}
//			}
//
//			if (length == 100.0f) {
//				isCollision = false;
//			}
//
//			if (isCollision == true) {
//				if (stage_->obstacles_[closestObsIndex].GetBlockType() == DeathBlock) {
//					stage_->obstacles_.erase(stage_->obstacles_.begin() + closestObsIndex);
//
//					// --�v���C���[�̏�Ԃ��ʏ��ԂȂ�-- //
//					if (player_->GetState() == NormalAir || player_->GetState() == NormalWallHit) {
//						// --�v���C���[�̏�Ԃ�ύX-- //
//						player_->SetDeath();// -> ���S���
//					}
//
//					//// --�v���C���[����]��ԂȂ�-- //
//					//else if (player_->GetState() == RotateWallHit || player_->GetState() == RotateAir) {
//					//	// --��Q�����폜-- //
//					//	stage_->obstacles_.erase(stage_->obstacles_.begin() + colObsIndex[i]);
//					//}
//				}
//
//				else if (stage_->obstacles_[closestObsIndex].GetBlockType() == Coin) {
//					stage_->AddCoin();
//					stage_->obstacles_.erase(stage_->obstacles_.begin() + closestObsIndex);
//				}
//
//				else if (stage_->obstacles_[closestObsIndex].GetBlockType() == BoundBlock) {
//
//					if (colObsXY >= 3) {
//						// --�v���C���[�̏�Ԃ�ς���-- //
//						player_->SetRotate();// --��]��Ԃ�-- //
//
//						// --X���̈ړ�������ς���-- //
//						player_->ChangeDireX();
//
//						// --�v���C���[�̏�Ԃ��ʏ��Ԃ��󒆂ɂ�����-- //
//						if (player_->GetState() == NormalAir) {
//							// --�v���C���[�̏�Ԃ�ύX-- //
//							player_->SetState(RotateAir);// -> ��]��Ԃ���
//						}
//
//						// --�v���C���[�Ə�Q����X������-- //
//						float len = abs(oldObj_.pos.x - colObs.pos.x);
//
//						// --�v���C���[�Ə�Q����Y�����a�𑫂����l-- //
//						float radius = oldObj_.radiusX + colObs.radiusX;
//
//						// --�v���C���[����Q���̉E���ɂ�����-- //
//						if (oldObj_.pos.x >= colObs.pos.x) {
//							// --�v���C���[�ɋ����Ɣ��a���������������Z-- //
//							oldObj_.pos.x += radius - len;
//							player_->object_.pos.x = oldObj_.pos.x;
//						}
//
//						// --�v���C���[����Q���̍����ɂ�����-- //
//						else if (oldObj_.pos.x < colObs.pos.x) {
//							// --�J�����ɋ����Ɣ��a���������������Z-- //
//							oldObj_.pos.x -= radius - len;
//							player_->object_.pos.x = oldObj_.pos.x;
//						}
//					}
//
//					else if (colObsXY <= 2) {
//						// --�v���C���[�̏�Ԃ�ς���-- //
//						player_->SetRotate();// --��]��Ԃ�-- //
//
//						// --�v���C���[��Y���̈ړ�������ς���-- //
//						player_->ChangeDireY();
//
//						// --�v���C���[�Ə�Q����Y������-- //
//						float len = abs(oldObj_.pos.y - colObs.pos.y);
//
//						// --�v���C���[�Ə�Q����Y�����a�𑫂����l-- //
//						float radius = oldObj_.radiusY + colObs.radiusY;
//
//						// --�v���C���[����Q���̉����ɂ�����-- //
//						if (oldObj_.pos.y >= colObs.pos.y) {
//							// --�J�����ɋ����Ɣ��a���������������Z-- //
//							oldObj_.pos.y += radius - len;
//							Camera::AddScroll(radius - len);
//						}
//
//						// --�v���C���[����Q���̍����ɂ�����-- //
//						else if (oldObj_.pos.y < colObs.pos.y) {
//							// --�J�����ɋ����Ɣ��a���������������Z-- //
//							oldObj_.pos.y -= radius - len;
//							Camera::AddScroll(-(radius - len));
//							scroll = Camera::GetScroll();
//						}
//					}
//
//					isOutLoop = true;
//				}
//			}
//		}
//
//		if (i == 9) {
//			oldObj_ = newObj;
//		}
//	}
//
//	// --�Â��v���C���[�����i�[-- //
//	oldObj_ = player_->GetBoxObj();
//	oldObj_.pos.y += Camera::GetScroll();
//}