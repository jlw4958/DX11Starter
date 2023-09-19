#pragma once

#include <DirectXMath.h>

class Transform
{
public:
	Transform();

	DirectX::XMFLOAT4X4 GetWorldMatrix();
	void MoveAbsolute(float x, float y, float z);
	void MoveRelative(float x, float y, float z);
	void Rotate(float p, float y, float r);
	void Scale(float x, float y, float z);

	// Setters - Overwriting existing variables
	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);
	void SetScale(float x, float y, float z);

	// Getters - get values
	DirectX::XMFLOAT3 GetPosition();
	DirectX::XMFLOAT3 GetRotation();
	DirectX::XMFLOAT3 GetScale();


private:
	// raw transformation data
	DirectX::XMFLOAT3 position;
	
	//// rotation option 1
	//float pitch; x
	//float yaw; y
	//float roll; z

	// rotation
	DirectX::XMFLOAT3 pitchYawRoll;

	DirectX::XMFLOAT3 scale;

};

