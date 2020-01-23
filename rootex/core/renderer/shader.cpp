#include "shader.h"

#include "utils.h"
#include "texture.h"

Shader::Shader(const LPCWSTR& vertexPath, const LPCWSTR& pixelPath, const BufferFormat& vertexBufferFormat)
    : m_VertexPath(vertexPath)
    , m_PixelPath(pixelPath)
{
	Microsoft::WRL::ComPtr<ID3DBlob> vertexShaderBlob = RenderingDevice::GetSingleton()->createBlob(vertexPath);
	if (!vertexShaderBlob)
	{
		ERR("Vertex Shader not found");
	}
	m_VertexShader = RenderingDevice::GetSingleton()->initVertexShader(vertexShaderBlob.Get());

	Microsoft::WRL::ComPtr<ID3DBlob> pixelShaderBlob = RenderingDevice::GetSingleton()->createBlob(pixelPath);
	if (!pixelShaderBlob)
	{
		ERR("Pixel Shader not found");
	}
	m_PixelShader = RenderingDevice::GetSingleton()->initPixelShader(pixelShaderBlob.Get());

	const Vector<VertexBufferElement>& elements = vertexBufferFormat.getElements();

	Vector<D3D11_INPUT_ELEMENT_DESC> vertexDescArray;
	unsigned int offset = 0;
	for (auto& element : elements)
	{
		D3D11_INPUT_ELEMENT_DESC desc;
		desc = { element.m_Name, 0, (DXGI_FORMAT)element.m_Type, 0, offset, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		offset += VertexBufferElement::GetSize(element.m_Type);

		vertexDescArray.push_back(desc);
	}

	RenderingDevice::GetSingleton()->initVertexLayout(
	    vertexShaderBlob.Get(),
	    vertexDescArray.data(),
	    vertexDescArray.size());
}

Shader::~Shader()
{
}

void Shader::bind() const
{
	RenderingDevice::GetSingleton()->bind(m_VertexShader.Get());
	RenderingDevice::GetSingleton()->bind(m_PixelShader.Get());
}

void Shader::unbind() const
{
	RenderingDevice::GetSingleton()->unbindShaderResources();
}

void Shader::set(const ConstantBufferType& type, const Matrix& constantBuffer)
{
	D3D11_BUFFER_DESC cbd = { 0 };
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbd.MiscFlags = 0u;
	cbd.ByteWidth = sizeof(constantBuffer);
	cbd.StructureByteStride = 0u;
	D3D11_SUBRESOURCE_DATA csd = { 0 };
	csd.pSysMem = &constantBuffer;

	switch (type)
	{
	case Shader::ConstantBufferType::Model:
		RenderingDevice::GetSingleton()->initVSModelConstantBuffer(&cbd, &csd);
		break;
	case Shader::ConstantBufferType::View:
		RenderingDevice::GetSingleton()->initVSViewConstantBuffer(&cbd, &csd);
		break;
	case Shader::ConstantBufferType::Projection:
		RenderingDevice::GetSingleton()->initVSProjectionConstantBuffer(&cbd, &csd);
		break;
	default:
		break;
	}
}

void Shader::set(const PSConstantBuffer& constantBuffer)
{
	D3D11_BUFFER_DESC cbd = { 0 };
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbd.MiscFlags = 0u;
	cbd.ByteWidth = sizeof(constantBuffer);
	cbd.StructureByteStride = 0u;
	D3D11_SUBRESOURCE_DATA csd = { 0 };
	csd.pSysMem = &constantBuffer;

	RenderingDevice::GetSingleton()->initPSConstantBuffer(&cbd, &csd);
}

DiffuseShader::DiffuseShader(const LPCWSTR& vertexPath, const LPCWSTR& pixelPath, const BufferFormat& vertexBufferFormat)
    : Shader(vertexPath, pixelPath, vertexBufferFormat)
{
	m_SamplerState = RenderingDevice::GetSingleton()->createSamplerState();
}

void DiffuseShader::set(const Texture* texture)
{
	RenderingDevice::GetSingleton()->setInPixelShader(0, 1, texture->getTextureResourceView());
}

void DiffuseShader::bind() const
{
	Shader::bind();
	RenderingDevice::GetSingleton()->setInPixelShader(m_SamplerState.Get());
}