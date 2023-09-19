#pragma once

#include <DirectXMath.h>

class Transform
{
public:
	Transform();

	DirectX::XMFLOAT4X4 GetWorldMatrix();

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

