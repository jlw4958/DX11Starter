#include "GameEntity.h"
#include "Mesh.h"
#include "BufferStructs.h"

#include <iostream>
#include <DirectXMath.h>

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

/// <summary>
/// sets buffers, issues draw commands
/// </summary>
void GameEntity::Draw()
{
	// shader things (constant buffer)
	VertexShaderExternalData vsData;
	vsData.colorTint = editColor;
	XMStoreFloat4x4(&vsData.worldMatrix, XMMatrixIdentity());
}



