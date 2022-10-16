#pragma once
#include "NVector3.h"

struct NMatrix4
{
	float m[4][4];
};

//�P�ʍs��
NMatrix4 Identity();
//�g�k�s��̐ݒ�
NMatrix4 Scale(const NVector3& s);
//��]�s��̐ݒ�
NMatrix4 RotateX(float angle);
NMatrix4 RotateY(float angle);
NMatrix4 RotateZ(float angle);
//���s�ړ��s��̍쐬
NMatrix4 Translation(const NVector3& t);
//���W�ϊ��i�x�N�g���ƍs��̊|���Z�j
NVector3 Transform(const NVector3& v, const NMatrix4& m);
//w���Z���g�������W�ϊ�
NVector3 WTransform(const NVector3& v, const NMatrix4& m);

//������Z�q�I�[�o�[���[�h
NMatrix4& operator*=(NMatrix4& m1, const NMatrix4& m2);

//2�����Z�q�I�[�o�[���[�h
const NMatrix4 operator*(const NMatrix4& m1, const NMatrix4& m2);
const NVector3 operator*(const NVector3& v, const NMatrix4& m);