#pragma once

#include "DXCore.h"
#include "Mesh.h"
#include "BufferStructs.h"
#include "GameEntity.h"
#include "Camera.h"
#include <DirectXMath.h>
#include <wrl/client.h> // Used for ComPtr - a smart pointer for COM objects
#include <memory>
#include <vector>

class Game 
	: public DXCore
{

public:
	Game(HINSTANCE hInstance);
	~Game();

	// Overridden setup and game loop methods, which will be called automatically
	void Init();
	void OnResize();
	void Update(float deltaTime, float totalTime);
	void Draw(float deltaTime, float totalTime);
	DirectX::XMFLOAT4 editColor;
	std::shared_ptr<Camera> activeCam;
	std::shared_ptr<Camera> cam1;
	std::shared_ptr<Camera> cam2;
	std::shared_ptr<Camera> cam3;
	std::vector<std::shared_ptr<Camera>> cameras;

private:

	// Initialization helper methods - feel free to customize, combine, remove, etc.
	void LoadShaders(); 
	void CreateGeometry();
	void ImGuiHelper(float dt, std::vector<GameEntity> _entities);
	void ImGuiSetup(float dt);

	// Note the usage of ComPtr below
	//  - This is a smart pointer for objects that abide by the
	//     Component Object Model, which DirectX objects do
	//  - More info here: https://github.com/Microsoft/DirectXTK/wiki/ComPtr

	// Buffers to hold actual geometry data
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
	
	// Shaders and shader-related constructs
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;

	// shapes
	std::shared_ptr<Mesh> triangle;
	std::shared_ptr<Mesh> shape1;
	std::shared_ptr<Mesh> shape2;

	Microsoft::WRL::ComPtr<ID3D11Buffer> vsConstantBuffer;

	// entity list
	std::vector<GameEntity> entities;

};

