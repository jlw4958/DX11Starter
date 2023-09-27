#include "GameEntity.h"
#include "Mesh.h"
#include "BufferStructs.h"
#include "Camera.h"

#include <iostream>
#include <DirectXMath.h>

GameEntity::GameEntity(std::shared_ptr<Mesh> mesh_ptr)
{
	myMesh = mesh_ptr;
	myTransform = std::make_shared<Transform>();
	editColor = DirectX::XMFLOAT4(1.0f, 0.0f, 0.5f, 1.0f);
}

std::shared_ptr<Mesh> GameEntity::GetMesh()
{
	return myMesh;
}

std::shared_ptr<Transform> GameEntity::GetTransform()
{
	return myTransform;
}

/// <summary>
/// sets buffers, issues draw commands
/// </summary>
void GameEntity::Draw(Microsoft::WRL::ComPtr<ID3D11Buffer> vsConstantBuffer, std::shared_ptr<Camera> camPtr)
{
	// cBuffer
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> ctx = myMesh->GetContext();

	// shader things (constant buffer)
	VertexShaderExternalData vsData;
	vsData.colorTint = editColor;
	vsData.worldMatrix = myTransform->GetWorldMatrix();
	vsData.projMatrix = camPtr->GetProjection();
	vsData.viewMatrix = camPtr->GetView();

	// mapping
	D3D11_MAPPED_SUBRESOURCE mappedBuffer = {};
	ctx->Map(vsConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedBuffer);
	memcpy(mappedBuffer.pData, &vsData, sizeof(vsData));
	ctx->Unmap(vsConstantBuffer.Get(), 0);

	// setting up vertex + index buffers
	myMesh->Draw();
}



