#include "NParticle.h"
#include "NSceneManager.h"

#include <math.h>
#include <time.h>

void NParticle::Initialize(ComPtr<ID3D12Device> device)
{
	//�}�e���A��(�萔�o�b�t�@)
	material.Initialize(device);
	material.SetColor({ 0.9f,0.9f,0.9f,0.7f });

	//�����̏��
	cube = std::make_unique<NCube>();
	cube->Initialize(device);

	//�I�u�W�F�N�g(�萔�o�b�t�@)
	for (size_t i = 0; i < maxObj; i++)
	{
		obj3d[i] = std::make_unique<NObj3d>();
		obj3d[i]->texNum = WHITEPNG;
		obj3d[i]->scale = { 0.0f,0.0f,0.0f };
		obj3d[i]->Initialize(device);
	}
}

void NParticle::WallHit(bool isParticle, int isDirectionR, XMMATRIX matView, XMMATRIX matProjection, NObj3d* player)
{
	srand(time(nullptr));

	for (size_t i = 0; i < maxObj; i++)
	{
		if (obj3d[i]->scale.x <= 0.0f)
		{
			if (isParticle)
			{
				float scale = static_cast<float>(rand() % 10);

				obj3d[i]->scale = { scale,scale,scale };
				obj3d[i]->position = player->position;
				//�E���ɐ������邩�����ɐ������邩
				obj3d[i]->position.x += player->scale.x * isDirectionR;
				obj3d[i]->position.y += player->scale.y;
			}
		}

		//�����_���ł��낢�듮���������߂�
		float rot = static_cast<float>(rand() % 10) + 10.0f;
		float speedX = static_cast<float>(rand() % 30) * 0.1f;
		float speedY = static_cast<float>(rand() % 15 + 10.0f) * 0.1f;
		float speedZ = static_cast<float>(rand() % 8 - 4.0f);
		//�k�����Ă�
		if (obj3d[i]->scale.x > 0)
		{
			obj3d[i]->scale.x -= 0.2f;
			obj3d[i]->scale.y -= 0.2f;
			obj3d[i]->scale.z -= 0.2f;
		}

		//���邮��
		obj3d[i]->rotation.x -= rot;
		obj3d[i]->rotation.y -= rot;
		//�Ԃ����ł�
		obj3d[i]->position.x -= speedX * isDirectionR;
		obj3d[i]->position.y += speedY;
		obj3d[i]->position.z += speedZ;
		obj3d[i]->UpdateMatrix(matView, matProjection);
	}
}

void NParticle::Draw(NDX12* dx12, const ComPtr<ID3D12PipelineState> pipelineState, ComPtr<ID3D12RootSignature> rootSignature, UINT incrementSize)
{
	for (size_t i = 0; i < maxObj; i++)
	{
		obj3d[i]->CommonBeginDraw(dx12->GetCommandList(), pipelineState, rootSignature, dx12->GetSRVHeap());
		obj3d[i]->Draw(dx12->GetCommandList(), material, dx12->GetSRVHeap(), cube->vbView, cube->ibView, cube->numIB, incrementSize);
	}
}
