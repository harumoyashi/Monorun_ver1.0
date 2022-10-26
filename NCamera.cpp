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
		XMConvertToRadians(45.0f),		//上下画角45度
		(float)NWindows::win_width / NWindows::win_height,	//アスペクト比(画面横幅/画面縦幅)
		0.1f, 1610.0f					//前端、奥端
	);
}

void NCamera::SetScrollY(float scrollY) {
	eye.y += scrollY;
	target.y += scrollY;
}
