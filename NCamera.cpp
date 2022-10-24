#include "NCamera.h"
#include "NWindows.h"

void NCamera::CreateMatView()
{
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
}

void NCamera::ProjectiveProjection()
{
	matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45.0f),		//�㉺��p45�x
		(float)NWindows::win_width / NWindows::win_height,	//�A�X�y�N�g��(��ʉ���/��ʏc��)
		0.1f, 1000.0f					//�O�[�A���[
	);
}
