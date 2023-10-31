#include "Material.h"

using namespace DirectX;


Material::Material(XMFLOAT4 _colorTint, std::shared_ptr<SimplePixelShader> _pixelShader, std::shared_ptr<SimpleVertexShader> _vertexShader, float _roughness):
	vertexShader(_vertexShader),
	pixelShader(_pixelShader),
	colorTint(_colorTint),
	roughness(_roughness)
{
	// roughness will range from 0 to 1
}

Material::~Material()
{
	// nothing for now!
}

void Material::Setup()
{
	//pixelShader->SetFloat4("colorTint", colorTint);
	//pixelShader->SetFloat("roughness", roughness);
	//pixelShader->SetFloat3("cameraPosition", camPtr->GetTransform()->GetPosition());

	//vs->SetMatrix4x4("world", myTransform->GetWorldMatrix()); // match variable
	//vs->SetMatrix4x4("worldInvTranspose", myTransform->GetWorldInvTranspose());
	//vs->SetMatrix4x4("view", camPtr->GetView()); // names in your
	//vs->SetMatrix4x4("projection", camPtr->GetProjection()); // shader’s cbuffer!

	for (auto& t : textureSRVs) { pixelShader->SetShaderResourceView(t.first.c_str(), t.second); }
	for (auto& s : samplers) { pixelShader->SetSamplerState(s.first.c_str(), s.second); }
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

float Material::GetRoughness()
{
	return roughness;
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

void Material::AddTextureSRV(std::string shaderName, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv)
{
	textureSRVs.insert({ shaderName, srv });
}

void Material::AddSampler(std::string shaderName, Microsoft::WRL::ComPtr<ID3D11SamplerState> sample)
{
	samplers.insert({ shaderName, sample });
}
