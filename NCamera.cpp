#include "NCamera.h"
#include "NWindows.h"

void NCamera::CreateMatView()
{
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
}

void NCamera::CreateShakeMatView(float value,float value2)
{
	totalEye = { eye.x + value, eye.y + value2, eye.z };
	totalTarget = { target.x + value, target.y + value2, target.z };
	matView = XMMatrixLookAtLH(XMLoadFloat3(&totalEye), XMLoadFloat3(&totalTarget), XMLoadFloat3(&up));
}

void NCamera::ProjectiveProjection()
{
	matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45.0f),		//�㉺��p45�x
		(float)NWindows::win_width / NWindows::win_height,	//�A�X�y�N�g��(��ʉ���/��ʏc��)
		0.1f, 1610.0f					//�O�[�A���[
	);
}

void NCamera::SetScrollY(float scrollY) {
	eye.y += scrollY;
	target.y += scrollY;
}
