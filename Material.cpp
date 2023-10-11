#include "Material.h"

using namespace DirectX;


Material::Material(XMFLOAT4 _colorTint, std::shared_ptr<SimplePixelShader> _pixelShader, std::shared_ptr<SimpleVertexShader> _vertexShader)
{
	colorTint = _colorTint;
	pixelShader = _pixelShader;
	vertexShader = _vertexShader;
}

Material::~Material()
{
	// nothing for now!
}

XMFLOAT4 Material::GetColorTint()
{
	return colorTint;
}

std::shared_ptr<SimplePixelShader> Material::GetPixelShader()
{
	return pixelShader;
}

std::shared_ptr<SimpleVertexShader> Material::GetVertexShader()
{
	return vertexShader;
}

void Material::setColorTint(XMFLOAT4 newTint)
{
	colorTint = newTint;
}

void Material::setPixelShader(std::shared_ptr<SimplePixelShader> newPixelShader)
{
	pixelShader = newPixelShader;
}

void Material::setVertexShader(std::shared_ptr<SimpleVertexShader> newVertexShader)
{
	vertexShader = newVertexShader;
}
