#pragma once

#include "SimpleShader.h"
#include <iostream>
#include <DirectXMath.h>

class Material
{
public:
	Material(DirectX::XMFLOAT4 _colorTint, std::shared_ptr<SimplePixelShader> _pixelShader, std::shared_ptr<SimpleVertexShader> _vertexShader, float _roughness);
	~Material();

	// getters
	DirectX::XMFLOAT4 GetColorTint();
	std::shared_ptr<SimplePixelShader> GetPixelShader();
	std::shared_ptr<SimpleVertexShader> GetVertexShader();
	float GetRoughness();

	// setters
	void SetColorTint(DirectX::XMFLOAT4 newTint);
	void SetPixelShader(std::shared_ptr<SimplePixelShader> newPixelShader);
	void SetVertexShader(std::shared_ptr<SimpleVertexShader> newVertexShader);

private:
	DirectX::XMFLOAT4 colorTint;
	std::shared_ptr<SimplePixelShader> pixelShader;
	std::shared_ptr<SimpleVertexShader> vertexShader;
	float roughness;
};

