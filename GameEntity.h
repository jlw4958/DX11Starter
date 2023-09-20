#pragma once
#include "Transform.h"
#include "Mesh.h"
#include <iostream>

class GameEntity
{
public:
	GameEntity(std::shared_ptr<Mesh> mesh_ptr);

	// getters
	std::shared_ptr<Mesh> GetMesh();
	std::shared_ptr<Transform> GetTransform();

	void Draw();

private:
	std::shared_ptr<Transform> myTransform;
	std::shared_ptr<Mesh> myMesh;

	// editing values
	DirectX::XMFLOAT4 editColor;
};

