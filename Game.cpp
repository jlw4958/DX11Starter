// clean up this file

#include "Game.h"
#include "Vertex.h"
#include "Input.h"
#include "PathHelpers.h"
#include "Mesh.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"
#include "BufferStructs.h"
#include "GameEntity.h"
#include "Transform.h"
#include "Camera.h"
#include "Material.h"
#include "SimpleShader.h"

#include "WICTextureLoader.h" // windows imaging component

// Needed for a helper function to load pre-compiled shader files
#pragma comment(lib, "d3dcompiler.lib")
#include <iostream>
#include <d3dcompiler.h>

// For the DirectX Math library
using namespace DirectX;

// --------------------------------------------------------
// Constructor
//
// DXCore (base class) constructor will set up underlying fields.
// Direct3D itself, and our window, are not ready at this point!
//
// hInstance - the application's OS-level handle (unique ID)
// --------------------------------------------------------
Game::Game(HINSTANCE hInstance)
	: DXCore(
		hInstance,			// The application's handle
		L"DirectX Game",	// Text for the window's title bar (as a wide-character string)
		1280,				// Width of the window's client area
		720,				// Height of the window's client area
		false,				// Sync the framerate to the monitor refresh? (lock framerate)
		true)				// Show extra stats (fps) in title bar?
{
#if defined(DEBUG) || defined(_DEBUG)
	// Do we want a console window?  Probably only in debug mode
	CreateConsoleWindow(500, 120, 32, 120);
	printf("Console window created successfully.  Feel free to printf() here.\n");
#endif

	editColor = XMFLOAT4(0, 0, 255, 1);
	ambientColor = XMFLOAT3(.1f, .1f, .25f);
	directionalLight1 = {}; // set all to 0, then set only necessary values
}

// --------------------------------------------------------
// Destructor - Clean up anything our game has created:
//  - Delete all objects manually created within this class
//  - Release() all Direct3D objects created within this class
// --------------------------------------------------------
Game::~Game()
{
	// Call delete or delete[] on any objects or arrays you've
	// created using new or new[] within this class
	// - Note: this is unnecessary if using smart pointers

	// Call Release() on any Direct3D objects made within this class
	// - Note: this is unnecessary for D3D objects stored in ComPtrs
	
	// ImGui clean up
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

// --------------------------------------------------------
// Called once per program, after Direct3D and the window
// are  ialized but before the game loop.
// --------------------------------------------------------
void Game::Init()
{
	LoadShaders();

	// colors
	XMFLOAT4 color1(0.4f, 0.2f, 0.7f, 1.0f);
	XMFLOAT4 color2(0.6f, 0.1f, 1.0f, 1.0f);
	XMFLOAT4 color3(1.0f, 0.7f, 0.4f, 1.0f);

	// textures loaded before making materials
	CreateWICTextureFromFile(device.Get(), FixPath(L"").c_str(), 0, textureSRV.GetAddressOf()); // choose a pic later

	// making sampler state
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP; // must do u, v, and w, even if texture is 2d or 1d
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX; // a very big number >:)
	device->CreateSamplerState(&samplerDesc, sampler.GetAddressOf());

	// making materials!
	material1 = std::make_shared<Material>(color1, pixelShader, vertexShader, 0.7f);
	material2 = std::make_shared<Material>(color2, pixelShader, vertexShader, 0.7f);
	material3 = std::make_shared<Material>(color3, pixelShader, vertexShader, 0.7f);

	CreateGeometry();

	// primitive topology
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // can use lines too! or points! anything!; if you don't set this, it defaults to triangles with modern graphics drivers (but not all of them!!)
	
	// ImGUI
	{
		// Initialize ImGui itself & platform/renderer backends
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui_ImplWin32_Init(hWnd);
		ImGui_ImplDX11_Init(device.Get(), context.Get());
		// Pick a style (uncomment one of these 3)
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsLight();
		//ImGui::StyleColorsClassic();
	}

	// Get size as the next multiple of 16 (instead of hardcoding a size here!)
	unsigned int size = sizeof(VertexShaderExternalData);
	size = (size + 15) / 16 * 16; // This will work even if the struct size changes

	// Describe the constant buffer
	D3D11_BUFFER_DESC cbDesc = {}; // Sets struct to all zeros
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.ByteWidth = size; // Must be a multiple of 16
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDesc.Usage = D3D11_USAGE_DYNAMIC;

	// vectors to edit
	XMFLOAT4 color(1.0f, 0.0f, 0.5f, 1.0f);

	editColor = color;

	// create the cameras
	{
		// cam 1
		cam1 = std::make_shared<Camera>(
			0.0f, 0.0f, -5.0f,
			5.0f,
			1.0f,
			XM_PIDIV4, // pi/4
			float(this->windowWidth / this->windowHeight)
			);

		// cam 2
		cam2 = std::make_shared<Camera>(
			0.0f, 0.0f, -7.0f,
			5.0f,
			1.0f,
			XM_PIDIV2, // pi/2
			float(this->windowWidth / this->windowHeight)
		);

		// cam 3
		cam3 = std::make_shared<Camera>(
			0.0f, 0.0f, -15.0f,
			5.0f,
			1.0f,
			XM_PI/3, // pi
			float(this->windowWidth / this->windowHeight)
		);
	}

	// push cameras to vector
	cameras.push_back(cam1);
	cameras.push_back(cam2);
	cameras.push_back(cam3);

	// choosing an active camera by default; cameras[0] will be active
	for (int i = 1; i < cameras.size(); i++)
	{
		cameras[i]->isActive = false;
	}

	activeCam = cameras[0];

	// lights

	// directional lights
	{
		// 1
		directionalLight1.Type = LIGHT_TYPE_DIRECTIONAL;
		directionalLight1.Direction = XMFLOAT3(0, 1, 1);
		directionalLight1.Intensity = 1.0f;
		directionalLight1.Color = XMFLOAT3(0.3f, 1.0f, 0.3f);

		// 2
		directionalLight2.Type = LIGHT_TYPE_DIRECTIONAL;
		directionalLight2.Direction = XMFLOAT3(1, 0, 1);
		directionalLight2.Intensity = 1.0f;
		directionalLight2.Color = XMFLOAT3(1.0f, 0.3f, 0.3f);

		// 3
		directionalLight3.Type = LIGHT_TYPE_DIRECTIONAL;
		directionalLight3.Direction = XMFLOAT3(1, 1, 0);
		directionalLight3.Intensity = 1.0f;
		directionalLight3.Color = XMFLOAT3(0.3f, 0.3f, 1.0f);
	}


}

// --------------------------------------------------------
// Loads shaders from compiled shader object (.cso) files
// and also created the Input Layout that describes our 
// vertex data to the rendering pipeline. 
// - Input Layout creation is done here because it must 
//    be verified against vertex shader byte code
// - We'll have that byte code already loaded below
// --------------------------------------------------------
void Game::LoadShaders()
{
	pixelShader = std::make_shared<SimplePixelShader>(device, context,
		FixPath(L"PixelShader.cso").c_str());	
	customPixelShader = std::make_shared<SimplePixelShader>(device, context,
		FixPath(L"CustomPixelShader.cso").c_str());
	vertexShader = std::make_shared<SimpleVertexShader>(device, context,
		FixPath(L"VertexShader.cso").c_str());
}

// --------------------------------------------------------
// Creates the geometry we're going to draw - a single triangle for now
// --------------------------------------------------------
void Game::CreateGeometry()
{
	// Create some temporary variables to represent colors
	// - Not necessary, just makes things more readable
	XMFLOAT4 red	= XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4 green	= XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	XMFLOAT4 blue	= XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	XMFLOAT4 black = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4 white = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	// the quads are weird >:(
	// 1
	entities.push_back(GameEntity(std::make_shared<Mesh>(FixPath("../../Assets/Models/cube.obj").c_str(), device, context), material1)); // make sure all models are lined up next to each other (adjust x pos)
	entities[0].GetTransform()->SetPosition(XMFLOAT3(-6.0f, 0.0f, 0.0f));

	// 2
	entities.push_back(GameEntity(std::make_shared<Mesh>(FixPath("../../Assets/Models/cylinder.obj").c_str(), device, context), material2));
	entities[1].GetTransform()->SetPosition(XMFLOAT3(-3.0f, 0.0f, 0.0f));

	// 3
	entities.push_back(GameEntity(std::make_shared<Mesh>(FixPath("../../Assets/Models/helix.obj").c_str(), device, context), material3));

	// 4
	entities.push_back(GameEntity(std::make_shared<Mesh>(FixPath("../../Assets/Models/sphere.obj").c_str(), device, context), material1));
	entities[3].GetTransform()->SetPosition(XMFLOAT3(3.0f, 0.0f, 0.0f));

	// 5
	entities.push_back(GameEntity(std::make_shared<Mesh>(FixPath("../../Assets/Models/torus.obj").c_str(), device, context), material2));
	entities[4].GetTransform()->SetPosition(XMFLOAT3(6.0f, 0.0f, 0.0f));
	
}

// --------------------------------------------------------
// Handle resizing to match the new window size.
//  - DXCore needs to resize the back buffer
//  - Eventually, we'll want to update our 3D camera
// --------------------------------------------------------
void Game::OnResize()
{
	// Handle base-level DX resize stuff
	DXCore::OnResize();
}

// --------------------------------------------------------
// Update your game here - user input, move objects, AI, etc. AND IMGUI
// --------------------------------------------------------
void Game::Update(float deltaTime, float totalTime)
{
	// camera things
	// make sure you update camera too!
	//call camera's update here
	activeCam->Update(deltaTime);

	// updating all projection matrices
	for (int i = 0; i < cameras.size(); i++)
	{
		if (!cameras[i]->isActive) {
			cameras[i]->UpdateProjectionMatrix(cameras[i]->GetFOV(), cameras[i]->GetAspectRatio());
		}
	}

	// ImGui things
	{
		// ImGUI setup
		ImGuiSetup(deltaTime);

		// display some data
		ImGui::Text("Framerate %f", ImGui::GetIO().Framerate);
		ImGui::Text("Width %lu", windowWidth);
		ImGui::Text("Height %lu", windowHeight);

		ImGui::ColorEdit4("4 - component(RGBA) color editor", &editColor.x);


		// calling ImGUI helper method
		ImGuiHelper(deltaTime, entities, cameras);
	}

	// Example input checking: Quit if the escape key is pressed
	if (Input::GetInstance().KeyDown(VK_ESCAPE))
		Quit();
}

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void Game::Draw(float deltaTime, float totalTime)
{
	// Frame START
	// - These things should happen ONCE PER FRAME
	// - At the beginning of Game::Draw() before drawing *anything*
	{
		// Clear the back buffer (erases what's on the screen)
		const float bgColor[4] = { 0.4f, 0.6f, 0.75f, 1.0f }; // Cornflower Blue
		context->ClearRenderTargetView(backBufferRTV.Get(), bgColor);

		// Clear the depth buffer (resets per-pixel occlusion information)
		context->ClearDepthStencilView(depthBufferDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

	// DRAW geometry
	// - These steps are generally repeated for EACH object you draw
	// - Other Direct3D calls will also be necessary to do more complex things
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	pixelShader->SetData(
		"directionalLight1", // The name of the (eventual) variable in the shader
		&directionalLight1, // The address of the data to set
		sizeof(Light)); // The size of the data (the whole struct!) to set

	pixelShader->SetData(
		"directionalLight1", // The name of the (eventual) variable in the shader
		&directionalLight2, // The address of the data to set
		sizeof(Light)); // The size of the data (the whole struct!) to set

	pixelShader->SetData(
		"directionalLight1", // The name of the (eventual) variable in the shader
		&directionalLight3, // The address of the data to set
		sizeof(Light)); // The size of the data (the whole struct!) to set

	// textures
	pixelShader->SetSamplerState("BasicSampler", sampler);
	pixelShader->SetShaderResourceView("SurfaceTexture", textureSRV);

	// drawing entities
	for (int i = 0; i < entities.size(); i++)
	{
		entities[i].Draw(activeCam, totalTime);
		entities[i].GetMaterial()->GetPixelShader()->SetFloat3("ambient", ambientColor);
	}

	// Frame END
	// - These should happen exactly ONCE PER FRAME
	// - At the very end of the frame (after drawing *everything*)
	{
		// Present the back buffer to the user
		//  - Puts the results of what we've drawn onto the window
		//  - Without this, the user never sees anything
		bool vsyncNecessary = vsync || !deviceSupportsTearing || isFullscreen;
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		swapChain->Present(
			vsyncNecessary ? 1 : 0,
			vsyncNecessary ? 0 : DXGI_PRESENT_ALLOW_TEARING);

		// Must re-bind buffers after presenting, as they become unbound
		context->OMSetRenderTargets(1, backBufferRTV.GetAddressOf(), depthBufferDSV.Get());
	}
}

void Game::ImGuiHelper(float dt, std::vector<GameEntity> _entities, std::vector< std::shared_ptr<Camera>> _cameras)
{	
	// looping through the entities for the tree nodes
	/*if (ImGui::TreeNode("Entities")) {
		for (int i = 0; i < _entities.size(); i++)
		{
			if (ImGui::TreeNode((void*)(intptr_t)i, "Entity %d", i)) {
				ImGui::PushID(i);

				ImGui::Text("Entity %i:", i);

				// values
				XMFLOAT3 pos = _entities[i].GetTransform()->GetPosition();
				XMFLOAT3 rot = _entities[i].GetTransform()->GetRotation();
				XMFLOAT3 scale = _entities[i].GetTransform()->GetScale();

				if (ImGui::DragFloat3("Position ", &pos.x, 0.01f))
				{
					_entities[i].GetTransform()->SetPosition(pos);
				}

				if (ImGui::DragFloat3("Rotation (Radians) ", &rot.x, 0.01f))
				{
					_entities[i].GetTransform()->SetRotation(rot);
				}

				if (ImGui::DragFloat3("Scale ", &scale.x, 0.01f))
				{
					_entities[i].GetTransform()->SetScale(scale);
				}

				ImGui::Text("Mesh Index Count: %d", _entities[i].GetMesh()->GetIndexCount());

				ImGui::PopID();
				ImGui::TreePop();

			}
		}
		ImGui::TreePop();
	}*/

	// camera things
	static int clicked = 0;
	if (ImGui::TreeNode("Cameras")) {
		for (int i = 0; i < _cameras.size(); i++)
		{
			ImGui::PushID(i);
			if (ImGui::RadioButton("Camera##1", &clicked, i)) {
				if (!_cameras[i]->isActive) {
					_cameras[i]->isActive = true;
					activeCam->isActive = false;
					activeCam = _cameras[i];
				}
				// show camera info
			}
			ImGui::SameLine();
			ImGui::PopID();
		}

		ImGui::TreePop();
	}

}

void Game::ImGuiSetup(float dt)
{
	// Feed fresh input data to ImGui
	ImGuiIO& io = ImGui::GetIO();
	io.DeltaTime = dt;
	io.DisplaySize.x = (float)this->windowWidth;
	io.DisplaySize.y = (float)this->windowHeight;
	// Reset the frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	// Determine new input capture
	Input& input = Input::GetInstance();
	input.SetKeyboardCapture(io.WantCaptureKeyboard);
	input.SetMouseCapture(io.WantCaptureMouse);
	// Show the demo window
	ImGui::ShowDemoWindow();
}