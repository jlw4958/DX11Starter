#pragma once
#include "Transform.h"
#include "Mesh.h"
#include "Camera.h"
#include "Material.h"
#include <iostream>

class GameEntity
{
public:
	GameEntity(std::shared_ptr<Mesh> mesh_ptr, std::shared_ptr<Material> material_ptr);

	// getters
	std::shared_ptr<Mesh> GetMesh();
	std::shared_ptr<Transform> GetTransform();
	std::shared_ptr<Material> GetMaterial();

	void Draw(std::shared_ptr<Camera> camPtr);
	
	// setters
	void SetMaterial(std::shared_ptr<Material> newMaterial);

private:
	std::shared_ptr<Transform> myTransform;
	std::shared_ptr<Mesh> myMesh;
	std::shared_ptr<Material> myMaterial;

	// editing values
	DirectX::XMFLOAT4 editColor;
};

