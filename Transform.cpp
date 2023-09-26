#include "Transform.h"
#include <DirectXMath.h>

// constructor
Transform::Transform() : 
	scale(1, 1, 1),
	pitchYawRoll(0, 0, 0),
	position(0, 0, 0),
	forward(0, 0, 1),
	right(1, 0, 0),
	up(0, 1, 0),
	isDirty(false),
	vectorsDirty(false)
{
	// transformation values
	//position = DirectX::XMFLOAT3(1, 1, 1);
	//pitchYawRoll = DirectX::XMFLOAT3(1, 1, 1);
	SetScale(1, 1, 1);

	// matrices
	XMStoreFloat4x4(&world, DirectX::XMMatrixIdentity());
	XMStoreFloat4x4(&worldInverseTranspose, DirectX::XMMatrixIdentity());
}

// transformations: floats
void Transform::MoveAbsolute(float x, float y, float z)
{
	// making a vector with the parameters
	DirectX::XMFLOAT3 offset = DirectX::XMFLOAT3(x, y, z);

	// loading position into math vector
	DirectX::XMVECTOR posVec = XMLoadFloat3(&position);

	// loading offset into math vector
	DirectX::XMVECTOR offVec = XMLoadFloat3(&offset);

	// math time!
	posVec = DirectX::XMVectorAdd(posVec, offVec);

	// copying value back to storage type
	DirectX::XMStoreFloat3(&position, posVec);

	isDirty = true;

}

void Transform::MoveRelative(float x, float y, float z)
{
	// rotate the vector I'm moving along to be relative to me
	// create a direction vector from the input
	// and rotate to match our current orientation
	DirectX::XMVECTOR movement = DirectX::XMVectorSet(x, y, z, 0);
	DirectX::XMVECTOR rotQuat = DirectX::XMQuaternionRotationRollPitchYawFromVector(XMLoadFloat3(&pitchYawRoll));

	// the non relative direction for our movement
	DirectX::XMVECTOR relativeDir = DirectX::XMVector3Rotate(movement, rotQuat);

	// add and store the results
	DirectX::XMStoreFloat3(&position, XMLoadFloat3(&position) + relativeDir); // fix this
	isDirty = true;
}

void Transform::Rotate(float p, float y, float r)
{
	// making a vector with the parameters
	DirectX::XMFLOAT3 rotationChange = DirectX::XMFLOAT3(p, y, r);

	// loading rotation into math vector
	DirectX::XMVECTOR rotVec = XMLoadFloat3(&pitchYawRoll);

	// loading change into math vector
	DirectX::XMVECTOR rotChangeVec = XMLoadFloat3(&rotationChange);

	// math time!
	rotVec = DirectX::XMVectorAdd(rotVec, rotChangeVec);

	// copying value back to storage type
	DirectX::XMStoreFloat3(&pitchYawRoll, rotVec);

	isDirty = true;
}

void Transform::Scale(float x, float y, float z)
{
	Scale(DirectX::XMFLOAT3(x, y, z));
	isDirty = true;

}

// transformations: vectors
void Transform::MoveAbsolute(DirectX::XMFLOAT3 _offset)
{
	// loading position into math vector
	DirectX::XMVECTOR posVec = XMLoadFloat3(&position);

	// loading offset into math vector
	DirectX::XMVECTOR offVec = XMLoadFloat3(&_offset);

	// math time!
	posVec = DirectX::XMVectorAdd(posVec, offVec);

	// copying value back to storage type
	DirectX::XMStoreFloat3(&position, posVec);

	isDirty = true;
}

void Transform::MoveRelative(DirectX::XMFLOAT3 offset)
{
}

void Transform::Rotate(DirectX::XMFLOAT3 _rotation)
{
	// loading rotation into math vector
	DirectX::XMVECTOR rotVec = XMLoadFloat3(&pitchYawRoll);

	// loading change into math vector
	DirectX::XMVECTOR rotChangeVec = XMLoadFloat3(&_rotation);

	// math time!
	rotVec = DirectX::XMVectorAdd(rotVec, rotChangeVec);

	// copying value back to storage type
	DirectX::XMStoreFloat3(&pitchYawRoll, rotChangeVec);

	isDirty = true;
}

void Transform::Scale(DirectX::XMFLOAT3 _scale)
{
	// loading scale into math vector
	DirectX::XMVECTOR scaleVec = XMLoadFloat3(&scale);

	// loading scaling into math vector
	DirectX::XMVECTOR scaleChangeVec = XMLoadFloat3(&_scale);

	// math time!!
	scaleVec = DirectX::XMVectorMultiply(scaleVec, scaleChangeVec);

	// copying value back to storage type
	DirectX::XMStoreFloat3(&scale, scaleVec);

	isDirty = true;
}

// setters: floats
void Transform::SetPosition(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;
}

void Transform::SetRotation(float x, float y, float z)
{
	pitchYawRoll = DirectX::XMFLOAT3(x, y, z);
}

void Transform::SetScale(float x, float y, float z)
{
	scale = DirectX::XMFLOAT3(x, y, z);
}

// setters: vectors
void Transform::SetPosition(DirectX::XMFLOAT3 _position)
{
	position = _position;
}

void Transform::SetRotation(DirectX::XMFLOAT3 _rotation)
{
	pitchYawRoll = _rotation;
}

void Transform::SetScale(DirectX::XMFLOAT3 _scale)
{
	scale = _scale;
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

DirectX::XMFLOAT3 Transform::GetForward()
{
	// update vectors
	// then return vectors
	return DirectX::XMFLOAT3();
}

DirectX::XMFLOAT3 Transform::GetRight()
{
	return DirectX::XMFLOAT3();
}

DirectX::XMFLOAT3 Transform::GetUp()
{
	return DirectX::XMFLOAT3();
}


// helpers

DirectX::XMFLOAT4X4 Transform::GetWorldMatrix()
{
	if (isDirty) {

		UpdateMatrices();
	}

	isDirty = false;
	return world;

}

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

void Transform::UpdateVectors()
{
	if (!vectorsDirty)
		return;

	// update the vectors!
	// quaternion math :0
	DirectX::XMVECTOR rotQuat = DirectX::XMQuaternionRotationRollPitchYawFromVector(XMLoadFloat3(&pitchYawRoll)); // rotation quaternion
	DirectX::XMStoreFloat3(&up, DirectX::XMVector3Rotate(DirectX::XMVectorSet(0, 1, 0, 0), rotQuat));
	DirectX::XMStoreFloat3(&right, DirectX::XMVector3Rotate(DirectX::XMVectorSet(1, 0, 0, 0), rotQuat));
	DirectX::XMStoreFloat3(&forward, DirectX::XMVector3Rotate(DirectX::XMVectorSet(0, 0, 1, 0), rotQuat));	// starts at 0, 0, 1 instead of moving to one spot and moving from there to another; 
																											// starts from the beginning and moves to new spot each frame



	// we're clean
	vectorsDirty = false;
}
