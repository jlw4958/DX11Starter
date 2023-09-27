#pragma once

#include "Transform.h"
#include <DirectXMath.h>

class Camera
{
public:
	Camera(float x, float y, float z, float moveSpeed, float mouseLookSpeed, float fov, float aspectRatio);
	~Camera();

	// update methods
	void Update(float dt);
	void UpdateViewMatrix();
	void UpdateProjectionMatrix(float _fov, float _aspectRatio);

	Transform* GetTransform();
	XMFLOAT4X4 GetView();
	XMFLOAT4X4 GetProjection();

private:
	// matrices
	DirectX::XMFLOAT4X4 viewMatrix;
	DirectX::XMFLOAT4X4 projectionMatrix;

	Transform transform;

	float moveSpeed;
	float mouseLookSpeed;

};
