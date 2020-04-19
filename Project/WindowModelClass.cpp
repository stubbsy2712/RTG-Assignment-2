#include "WindowModelClass.h"
#include "textureclass.h"

bool WindowModelClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* modelFilename, char* textureFilenameColour, char* textureFilenameNormal, char* textureFilenameRefraction)
{
	bool result;

	//modelMatrix = XMMatrixIdentity();
	
	// Load in the model data,
	result = LoadModel(modelFilename);
	if (!result)
	{
		return false;
	}

	// Initialize the vertex and index buffers.
	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	////Initialize OOBB buffer
	//result = InitializeBoundingBoxBuffers(device, boundingBoxVertBuffer, &m_boundingBoxVertexBuffer, OOBB);
	//if (!result)
	//{
	//	return false;
	//}

	////Initialize AABB buffer
	//result = InitializeBoundingBoxBuffers(device, AABBBoundingBoxVerts, &m_AABBBoundingBoxVertexBuffer, AABB);
	//if (!result)
	//{
	//	return false;
	//}

	// Load the textures for this model.
	result = LoadTexture(device, deviceContext, textureFilenameColour);
	if (!result)
	{
		return false;
	}
	result = LoadSpecificTexture(device, deviceContext, textureFilenameNormal, normalTexture);
	if (!result)
	{
		return false;
	}
	//result = LoadTexture(device, deviceContext, textureFilenameRefraction);
	//if (!result)
	//{
	//	return false;
	//}

	return true;
}

void WindowModelClass::Render(ID3D11DeviceContext* context)
{
	RenderBuffers(context);
}

bool WindowModelClass::LoadSpecificTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename, TextureClass*& texture)
{
	bool result;

	// Create the texture object.
	texture = new TextureClass;
	if (!texture)
	{
		return false;
	}

	// Initialize the texture object.
	result = texture->Initialize(device, deviceContext, filename);
	if (!result)
	{
		return false;
	}

	return true;
}

void WindowModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

void WindowModelClass::Shutdown()
{
	//TODO: actually implement the shutdown method for this
}

ID3D11ShaderResourceView* WindowModelClass::GetNormalMap()
{
	return normalTexture->GetTexture();
}

//ID3D11ShaderResourceView* WindowModelClass::GetRefractionMap()
//{
//	return nullptr;
//}

WindowModelClass::WindowModelClass()
{

}
