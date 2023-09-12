#pragma once
#include <d3d11.h> // for referencing Direct3D stuff
#include <wrl/client.h> // when using ComPtrs for Direct3D objects
#include "Vertex.h"
class Mesh
{
public:
	/// <summary>
	/// Mesh class constructor
	/// </summary>
	/// <param name="vertices"></param>
	/// <param name="numVertices"></param>
	/// <param name="indices"></param>
	/// <param name="numIndices"></param>
	/// <param name="_device"></param>
	/// <param name="_context"></param>
	Mesh(Vertex *_vertices, int numVertices, unsigned int *_indices, int numIndices, Microsoft::WRL::ComPtr<ID3D11Device> _device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> _context);
	/// <summary>
	/// Mesh class destructor
	/// </summary>
	~Mesh();
	/// <summary>
	/// returns the pointer to the vertex buffer object
	/// </summary>
	Microsoft::WRL::ComPtr<ID3D11Buffer> GetVertexBuffer();
	/// <summary>
	/// returns the pointer to the index buffer
	/// </summary>
	Microsoft::WRL::ComPtr<ID3D11Buffer> GetIndexBuffer();
	/// <summary>
	/// returns the number of indices this mesh contains
	/// </summary>
	int GetIndexCount();
	/// <summary>
	/// sets buffers; tells DirectX to draw the correct number of indices
	/// </summary>
	void Draw();

private:
	/// <summary>
	/// points to the chunk of data holding the vertices of this mesh
	/// </summary>
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	/// <summary>
	/// points to the chunk of data holding the indices of this mesh
	/// </summary>
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
	/// <summary>
	/// used for issuing draw commands
	/// </summary>
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
	/// <summary>
	/// number of indices in this mesh's index buffer
	/// </summary>
	int indices;
};

