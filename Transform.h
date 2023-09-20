#pragma once

#include <DirectXMath.h>

class Transform
{
public:
	Transform();

	//DirectX::XMFLOAT4X4 GetWorldMatrix();

	//void MoveRelative(float x, float y, float z);
	void MoveAbsolute(float x, float y, float z);
	void Rotate(float p, float y, float r);
	void Scale(float x, float y, float z);

	// transformers that take existing vectors
	//void MoveRelative(float x, float y, float z);
	void MoveAbsolute(DirectX::XMFLOAT3 offset);
	void Rotate(DirectX::XMFLOAT3 rotation);
	void Scale(DirectX::XMFLOAT3 scale);

	// Setters - Overwriting existing variables
	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);
	void SetScale(float x, float y, float z);

	// setters that take existing vectors
	void SetPosition(DirectX::XMFLOAT3 position);
	void SetRotation(DirectX::XMFLOAT3 rotation);
	void SetScale(DirectX::XMFLOAT3 scale);

	// Getters - get values
	DirectX::XMFLOAT3 GetPosition();
	DirectX::XMFLOAT3 GetRotation();
	DirectX::XMFLOAT3 GetScale();


private:
	// raw transformation data
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 pitchYawRoll;
	DirectX::XMFLOAT3 scale;

	// matrices
	DirectX::XMFLOAT4X4 world;
	DirectX::XMFLOAT4X4 worldInverseTranspose;
	bool isDirty;

	DirectX::XMFLOAT4X4 UpdateMatrices();
};

