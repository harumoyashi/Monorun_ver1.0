#include "NRootParam.h"

NRootParam::~NRootParam()
{
	entity.clear();
}

void NRootParam::SetDescRange()
{
	descriptorRange.NumDescriptors = 3;		//��x�̕`��Ɏg���e�N�X�`����1���Ȃ̂�1
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0;	//�e�N�X�`�����W�X�^0��
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
}

void NRootParam::SetRootParam()
{
	D3D12_ROOT_PARAMETER rootParam{};

	//�萔�o�b�t�@0��
	rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//�萔�o�b�t�@�r���[
	rootParam.Descriptor.ShaderRegister = 0;					//�萔�o�b�t�@�ԍ�
	rootParam.Descriptor.RegisterSpace = 0;						//�f�t�H���g�l
	rootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//�S�ẴV�F�[�_�[���猩����
	entity.push_back(rootParam);
	//�e�N�X�`�����W�X�^0��
	rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//���
	rootParam.DescriptorTable.pDescriptorRanges = &descriptorRange;			//�f�X�N���v�^�����W
	rootParam.DescriptorTable.NumDescriptorRanges = 1;						//�f�X�N���v�^�����W��
	rootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;				//�S�ẴV�F�[�_�[���猩����
	entity.push_back(rootParam);
	//�萔�o�b�t�@1��
	rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//���
	rootParam.Descriptor.ShaderRegister = 1;					//�萔�o�b�t�@�ԍ�
	rootParam.Descriptor.RegisterSpace = 0;						//�f�t�H���g�l
	rootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//�S�ẴV�F�[�_�[���猩����
	entity.push_back(rootParam);
}
