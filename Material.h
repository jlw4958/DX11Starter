#pragma once

#include "SimpleShader.h"
#include <iostream>
#include <DirectXMath.h>

class Material
{
public:
	Material(DirectX::XMFLOAT4 _colorTint, std::shared_ptr<SimplePixelShader> _pixelShader, std::shared_ptr<SimpleVertexShader> _vertexShader);
	~Material();

	// getters
	DirectX::XMFLOAT4 GetColorTint();
	std::shared_ptr<SimplePixelShader> GetPixelShader();
	std::shared_ptr<SimpleVertexShader> GetVertexShader();

	// setters
	void setColorTint(XMFLOAT4 newTint);
	void setPixelShader(std::shared_ptr<SimplePixelShader> newPixelShader);
	void setVertexShader(std::shared_ptr<SimpleVertexShader> newVertexShader);

private:
	DirectX::XMFLOAT4 colorTint;
	std::shared_ptr<SimplePixelShader> pixelShader;
	std::shared_ptr<SimpleVertexShader> vertexShader;
};

