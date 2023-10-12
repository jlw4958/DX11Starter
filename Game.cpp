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
// are initialized but before the game loop.
// --------------------------------------------------------
void Game::Init()
{
	// Helper methods for loading shaders, creating some basic
	// geometry to draw and some simple camera matrices.
	//  - You'll be expanding and/or replacing these later
	LoadShaders();
	CreateGeometry();

	// colors
	XMFLOAT4 color1(1.0f, 0.7f, 0.0f, 1.0f);
	XMFLOAT4 color2(0.0f, 1.0f, 0.2f, 1.0f);
	XMFLOAT4 color3(0.6f, 0.7f, 0.4f, 1.0f);

	// making materials!
	material1 = std::make_shared<Material>(color1, pixelShader, vertexShader);
	material2 = std::make_shared<Material>(color2, pixelShader, vertexShader);
	material3 = std::make_shared<Material>(color3, pixelShader, vertexShader);
	
	// Set initial graphics API state
	//  - These settings persist until we change them
	//  - Some of these, like the primitive topology & input layout, probably won't change
	//  - Others, like setting shaders, will need to be moved elsewhere later
	{
		// Tell the input assembler (IA) stage of the pipeline what kind of
		// geometric primitives (points, lines or triangles) we want to draw.  
		// Essentially: "What kind of shape should the GPU draw with our vertices?"
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // can use lines too! or points! anything!; if you don't set this, it defaults to triangles with modern graphics drivers (but not all of them!!)

		// Ensure the pipeline knows how to interpret all the numbers stored in
		// the vertex buffer. For this course, all of your vertices will probably
		// have the same layout, so we can just set this once at startup.

		// Set the active vertex and pixel shaders
		//  - Once you start applying different shaders to different objects,
		//    these calls will need to happen multiple times per frame
	}

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

	//// making entities
	//entities.push_back(GameEntity(triangle, material1));
	//entities.push_back(GameEntity(triangle, material1));
	//entities.push_back(GameEntity(shape1, material2));
	//entities.push_back(GameEntity(shape2, material3));
	//entities.push_back(GameEntity(shape2, material3));

	// vectors to edit
	//XMFLOAT3 vec(0.0f, 0.0f, 0.0f);
	XMFLOAT4 color(1.0f, 0.0f, 0.5f, 1.0f);

	editColor = color;

	// create the cameras
	{
		// cam 1
		cam1 = std::make_shared<Camera>(
			0.0f, 0.0f, -1.0f,
			5.0f,
			1.0f,
			XM_PIDIV4, // pi/4
			this->windowWidth / this->windowHeight
			);

		// cam 2
		cam2 = std::make_shared<Camera>(
			0.0f, 0.0f, -2.0f,
			5.0f,
			1.0f,
			XM_PIDIV2, // pi/2
			this->windowWidth / this->windowHeight
			);

		// cam 3
		cam3 = std::make_shared<Camera>(
			0.0f, 0.0f, -5.0f,
			5.0f,
			1.0f,
			XM_PI/3, // pi
			this->windowWidth / this->windowHeight
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
	vertexShader = std::make_shared<SimpleVertexShader>(device, context,
		FixPath(L"VertexShader.cso").c_str());
	pixelShader = std::make_shared<SimplePixelShader>(device, context,
		FixPath(L"PixelShader.cso").c_str());
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


	// loading models
	entities.push_back(GameEntity(std::make_shared<Mesh>(FixPath("../../Assets/Models/sphere.obj").c_str(), device), material1)); // make sure all models are lined up next to each other (adjust x pos)

	/*
	//// Set up the vertices of the triangle we would like to draw
	//// - We're going to copy this array, exactly as it exists in CPU memory
	////    over to a Direct3D-controlled data structure on the GPU (the vertex buffer)
	//// - Note: Since we don't have a camera or really any concept of
	////    a "3d world" yet, we're simply describing positions within the
	////    bounds of how the rasterizer sees our screen: [-1 to +1] on X and Y
	//// - This means (0,0) is at the very center of the screen.
	//// - These are known as "Normalized Device Coordinates" or "Homogeneous 
	////    Screen Coords", which are ways to describe a position without
	////    knowing the exact size (in pixels) of the image/window/etc.  
	//// - Long story short: Resizing the window also resizes the triangle,
	////    since we're describing the triangle in terms of the window itself
	//Vertex triVertices[] =
	//{
	//	{ XMFLOAT3(+0.0f, +0.5f, +0.0f), XMFLOAT3(0, 0, -1), XMFLOAT2(0,0)},
	//	{ XMFLOAT3(+0.5f, -0.5f, +0.0f), XMFLOAT3(0, 0, -1), XMFLOAT2(0,0) },
	//	{ XMFLOAT3(-0.5f, -0.5f, +0.0f), XMFLOAT3(0, 0, -1), XMFLOAT2(0,0) },
	//};
	//Vertex shape1Vertices[] = {
	//	{ XMFLOAT3(-1.0f, +0.5f, +0.0f), XMFLOAT3(0, 0, -1), XMFLOAT2(0,0) },
	//	{ XMFLOAT3(-0.75f, +0.5f, +0.0f), XMFLOAT3(0, 0, -1), XMFLOAT2(0,0) },
	//	{ XMFLOAT3(-0.55f, +0.3f, +0.0f), XMFLOAT3(0, 0, -1), XMFLOAT2(0,0) },
	//	{ XMFLOAT3(-0.55f, +0.3f, +0.0f), XMFLOAT3(0, 0, -1), XMFLOAT2(0,0) },
	//	{ XMFLOAT3(-0.8f, +0.0f, +0.0f), XMFLOAT3(0, 0, -1), XMFLOAT2(0,0) },
	//	{ XMFLOAT3(-1.0f, +0.5f, +0.0f), XMFLOAT3(0, 0, -1), XMFLOAT2(0,0) },
	//};
	//Vertex shape2Vertices[] = {
	//	{ XMFLOAT3(+0.6f, +0.7f, +0.0f), XMFLOAT3(0, 0, -1), XMFLOAT2(0,0) },
	//	{ XMFLOAT3(+0.6f, +0.4f, +0.0f), XMFLOAT3(0, 0, -1), XMFLOAT2(0,0) },
	//	{ XMFLOAT3(+0.5f, +0.2f, +0.0f), XMFLOAT3(0, 0, -1), XMFLOAT2(0,0) },
	//	{ XMFLOAT3(+0.5f, +0.2f, +0.0f), XMFLOAT3(0, 0, -1), XMFLOAT2(0,0)},
	//	{ XMFLOAT3(+0.4f, +0.4f, +0.0f), XMFLOAT3(0, 0, -1), XMFLOAT2(0,0) },
	//	{ XMFLOAT3(+0.6f, +0.7f, +0.0f), XMFLOAT3(0, 0, -1), XMFLOAT2(0,0) },
	//	{ XMFLOAT3(+0.6f, +0.7f, +0.0f), XMFLOAT3(0, 0, -1), XMFLOAT2(0,0) },
	//	{ XMFLOAT3(+0.8f, +0.4f, +0.0f), XMFLOAT3(0, 0, -1), XMFLOAT2(0,0) },
	//	{ XMFLOAT3(+0.6f, +0.4f, +0.0f), XMFLOAT3(0, 0, -1), XMFLOAT2(0,0) },

	//};

	//// Set up indices, which tell us which vertices to use and in which order
	//// - This is redundant for just 3 vertices, but will be more useful later
	//// - Indices are technically not required if the vertices are in the buffer 
	////    in the correct order and each one will be used exactly once
	//// - But just to see how it's done...
	//unsigned int triIndices[] = { 0, 1, 2 };
	//unsigned int shape1Indices[] = { 0, 1, 2, 3, 4, 5};
	//unsigned int shape2Indices[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };

	//int triCount = sizeof(triVertices) / sizeof(triVertices[0]);
	//int shape1Count = sizeof(shape1Vertices) / sizeof(shape1Vertices[0]);
	//int shape2Count = sizeof(shape2Vertices) / sizeof(shape2Vertices[0]);

	//std::cout << "tricount:" << triCount << " " << "shape1count:" << shape1Count << " " << "shape2count:" << shape2Count;

	//// mesh object (triangle)
	//triangle = std::make_shared<Mesh>(triVertices, triCount, triIndices, triCount, device, context);
	//shape1 = std::make_shared<Mesh>(shape1Vertices, shape1Count, shape1Indices, shape1Count, device, context);
	//shape2 = std::make_shared<Mesh>(shape2Vertices, shape2Count, shape2Indices, shape2Count, device, context);
	*/
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
		//ImGuiHelper(deltaTime, entities, cameras);


		//// editing vectors
		//ImGui::DragFloat3("Edit a vector", &editVector.x);
		//ImGui::ColorEdit4("4 - component(RGBA) color editor", &editColor.x);
	}

	/*// transformations
	{
		// get transform
		// update transform
		// set transform
		if (entities[0].GetTransform()->GetScale().x >= 2) {
			entities[0].GetTransform()->SetScale(1.0f, 1.0f, 1.0f);
		}
		else {
			entities[0].GetTransform()->Scale(XMFLOAT3(1.001f, 1.0f, 1.0f)); // scale by deltatime eventually
		}
		
		if (entities[2].GetTransform()->GetPosition().y >= 1) {
			entities[2].GetTransform()->SetPosition(0.0f, -1.5f, 0.0f);
		}
		else {
			entities[2].GetTransform()->MoveAbsolute(0.0f, float(.1 * deltaTime), 0.0f);
		}

		entities[3].GetTransform()->Rotate(0.0f, 0.0f, float(1 * deltaTime));
	}*/

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

	VertexShaderExternalData vsData;
	vsData.colorTint = editColor;

	// set view and proj matrices = to view and proj getters, same with world matrix

	// drawing entities
	for (int i = 0; i < entities.size(); i++)
	{
		entities[i].Draw(activeCam);
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
	if (ImGui::TreeNode("Entities")) {
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
	}

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