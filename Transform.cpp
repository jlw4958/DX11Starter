#include "Transform.h"
#include <DirectXMath.h>

Transform::Transform() : 
	position(0, 0, 0),
	pitchYawRoll(0, 0, 0),
	scale(0, 0, 0)
{

}

DirectX::XMFLOAT4X4 Transform::GetWorldMatrix()
{
	DirectX::XMMATRIX t = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	DirectX::XMMATRIX r = DirectX::XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&pitchYawRoll));
	DirectX::XMMATRIX s = DirectX::XMMatrixScalingFromVector(XMLoadFloat3(&scale));
	
}

void Transform::Scale(float x, float y, float z)
{
}

DirectX::XMFLOAT3 Transform::GetScale()
{
	return DirectX::XMFLOAT3();
}
