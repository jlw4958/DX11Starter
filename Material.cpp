#include "Material.h"

using namespace DirectX;


Material::Material(XMFLOAT4 _colorTint, std::shared_ptr<SimplePixelShader> _pixelShader, std::shared_ptr<SimpleVertexShader> _vertexShader):
	vertexShader(_vertexShader),
	pixelShader(_pixelShader),
	colorTint(_colorTint)
{
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

void Material::SetColorTint(XMFLOAT4 newTint)
{
	colorTint = newTint;
}

void Material::SetPixelShader(std::shared_ptr<SimplePixelShader> newPixelShader)
{
	pixelShader = newPixelShader;
}

void Material::SetVertexShader(std::shared_ptr<SimpleVertexShader> newVertexShader)
{
	vertexShader = newVertexShader;
}
