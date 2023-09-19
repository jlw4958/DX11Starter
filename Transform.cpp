#include "Transform.h"
#include <DirectXMath.h>

Transform::Transform() : 
	position(0, 0, 0),
	pitchYawRoll(0, 0, 0),
	scale(0, 0, 0)
{
	// transformation values
	position = DirectX::XMFLOAT3(1, 1, 1);
	pitchYawRoll = DirectX::XMFLOAT3(1, 1, 1);
	scale = DirectX::XMFLOAT3(1, 1, 1);

	// matrices
	XMStoreFloat4x4(&world, DirectX::XMMatrixIdentity());
	XMStoreFloat4x4(&worldInverseTranspose, DirectX::XMMatrixIdentity());

}

DirectX::XMFLOAT4X4 Transform::GetWorldMatrix()
{
	DirectX::XMMATRIX t = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	DirectX::XMMATRIX r = DirectX::XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&pitchYawRoll));
	DirectX::XMMATRIX s = DirectX::XMMatrixScalingFromVector(XMLoadFloat3(&scale));
	
}

// transformations: floats
void Transform::MoveAbsolute(float x, float y, float z)
{

}

void Transform::Rotate(float p, float y, float r)
{

}

void Transform::Scale(float x, float y, float z)
{

}

// transformations: vectors
void Transform::MoveAbsolute(DirectX::XMFLOAT3 offset)
{

}

void Transform::Rotate(DirectX::XMFLOAT3 rotation)
{

}

void Transform::Scale(DirectX::XMFLOAT3 scale)
{

}

// setters: floats
void Transform::SetPosition(float x, float y, float z)
{

}

void Transform::SetRotation(float x, float y, float z)
{

}

void Transform::SetScale(float x, float y, float z)
{

}

// setters: vectors
void Transform::SetPosition(DirectX::XMFLOAT3 position)
{

}

void Transform::SetRotation(DirectX::XMFLOAT3 rotation)
{

}

void Transform::SetScale(DirectX::XMFLOAT3 scale)
{

}

// getters
DirectX::XMFLOAT3 Transform::GetPosition()
{
	return position;
}

DirectX::XMFLOAT3 Transform::GetRotation()
{
	return pitchYawRoll;
}

DirectX::XMFLOAT3 Transform::GetScale()
{
	return scale;
}

// helpers
void Transform::UpdateMatrices()
{
	// translation matrix
	DirectX::XMMATRIX t = DirectX::XMMatrixTranslation(position.x, position.y, position.z);

	// rotation matrix
	DirectX::XMMATRIX r = DirectX::XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&pitchYawRoll));

	// scale matrix
	DirectX::XMMATRIX s = DirectX::XMMatrixScalingFromVector(XMLoadFloat3(&scale));	

	// multiplying matricies
	DirectX::XMMATRIX _world = s * r * t;

	// storing the matrix
	DirectX::XMStoreFloat4x4(&world, _world);
	DirectX::XMStoreFloat4x4(&worldInverseTranspose, DirectX::XMMatrixInverse(0, XMMatrixTranspose(_world)));
}