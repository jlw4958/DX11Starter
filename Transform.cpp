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

void Transform::MoveAbsolute(float x, float y, float z)
{
}

void Transform::MoveRelative(float x, float y, float z)
{
}

void Transform::Rotate(float p, float y, float r)
{
}

void Transform::Scale(float x, float y, float z)
{
}

void Transform::SetPosition(float x, float y, float z)
{
}

void Transform::SetRotation(float x, float y, float z)
{
}

void Transform::SetScale(float x, float y, float z)
{
}

DirectX::XMFLOAT3 Transform::GetPosition()
{
	return DirectX::XMFLOAT3();
}

DirectX::XMFLOAT3 Transform::GetRotation()
{
	return DirectX::XMFLOAT3();
}

DirectX::XMFLOAT3 Transform::GetScale()
{
	return DirectX::XMFLOAT3();
}
