#pragma once

#include "DXCore.h"
#include "Mesh.h"
#include "BufferStructs.h"
#include "GameEntity.h"
#include "Camera.h"
#include "SimpleShader.h"
#include "PathHelpers.h"
#include "Lights.h"
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
	std::shared_ptr<Material> material1;
	std::shared_ptr<Material> material2;
	std::shared_ptr<Material> material3;

	DirectX::XMFLOAT3 ambientColor;

private:

	// Initialization helper methods - feel free to customize, combine, remove, etc.
	void LoadShaders(); 
	void CreateGeometry();
	void ImGuiHelper(float dt, std::vector<GameEntity> _entities, std::vector< std::shared_ptr<Camera>> _cameras);
	void ImGuiSetup(float dt);

	// Note the usage of ComPtr below
	//  - This is a smart pointer for objects that abide by the
	//     Component Object Model, which DirectX objects do
	//  - More info here: https://github.com/Microsoft/DirectXTK/wiki/ComPtr

	// Buffers to hold actual geometry data
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
	
	// Shaders and shader-related constructs
	std::shared_ptr<SimplePixelShader> pixelShader;
	std::shared_ptr<SimplePixelShader> customPixelShader;
	std::shared_ptr<SimpleVertexShader> vertexShader;

	// shapes
	std::shared_ptr<Mesh> triangle;
	//std::shared_ptr<Mesh> shape1;
	std::shared_ptr<Mesh> shape2;

	// entity list
	std::vector<GameEntity> entities;

	// lights
	Light directionalLight1;
	Light directionalLight2;
	Light directionalLight3;

	std::vector<Light> lights;

	// texture stuff
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> textureSRV; // one per image loaded!

	Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler; // for now, just one >:)

};

