#pragma once

#include "SimpleShader.h"
#include "Mesh.h"
#include <iostream>
#include <DirectXMath.h>

class Sky
{
public:
	Sky(std::shared_ptr<Mesh> mesh_ptr, Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler, Microsoft::WRL::ComPtr<ID3D11Device> _device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> _context, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv, std::shared_ptr<SimplePixelShader> ps, std::shared_ptr<SimpleVertexShader> vs);
	~Sky();

	// --------------------------------------------------------
	// Author: Chris Cascioli
	// Purpose: Creates a cube map on the GPU from 6 individual textures
	// 
	// - You are allowed to directly copy/paste this into your code base
	//   for assignments, given that you clearly cite that this is not
	//   code of your own design.
	//
	// - Note: This code assumes you’re putting the function in Sky.cpp, 
	//   you’ve included WICTextureLoader.h and you have an ID3D11Device 
	//   ComPtr called “device”.  Make any adjustments necessary for
	//   your own implementation.
	// --------------------------------------------------------
	// --- HEADER ---
	// Helper for creating a cubemap from 6 individual textures
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> CreateCubemap(
		const wchar_t* right,
		const wchar_t* left,
		const wchar_t* up,
		const wchar_t* down,
		const wchar_t* front,
		const wchar_t* back);

private:
	std::shared_ptr<Mesh> skyMesh;
	std::shared_ptr<SimplePixelShader> skyPixelShader;
	std::shared_ptr<SimpleVertexShader> skyVertexShader;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> skySampler;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> skySRV;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthBuffer;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> skyRasterizer;
};