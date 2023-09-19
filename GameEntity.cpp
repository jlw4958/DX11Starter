#include "GameEntity.h"
#include "Mesh.h"
#include <iostream>

GameEntity::GameEntity(std::shared_ptr<Mesh> mesh_ptr)
{
	myMesh = mesh_ptr;
}

std::shared_ptr<Mesh> GameEntity::GetMesh()
{
	return std::shared_ptr<Mesh>();
}

std::shared_ptr<Transform> GameEntity::GetTransform()
{
	return std::shared_ptr<Transform>();
}



