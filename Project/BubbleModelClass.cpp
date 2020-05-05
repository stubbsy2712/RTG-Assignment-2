////////////////////////////////////////////////////////////////////////////////
// Filename: bubblemodelclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "bubblemodelclass.h"


BubbleModelClass::BubbleModelClass()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_Texture1 = 0;
	m_Texture2 = 0;
	//m_Texture3 = 0;
	m_model = 0;
}


BubbleModelClass::BubbleModelClass(const BubbleModelClass& other)
{
}


BubbleModelClass::~BubbleModelClass()
{
}


bool BubbleModelClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* context, char* modelFilename, char* textureFilename1,
	char* textureFilename2)
{
	bool result;


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

	// Load the textures for this model.
	result = LoadTextures(device, context, textureFilename1, textureFilename2);
	if (!result)
	{
		return false;
	}

	return true;
}


void BubbleModelClass::Shutdown()
{
	// Release the model textures.
	ReleaseTextures();

	// Shutdown the vertex and index buffers.
	ShutdownBuffers();

	// Release the model data.
	ReleaseModel();

	return;
}


void BubbleModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return;
}


int BubbleModelClass::GetIndexCount()
{
	return m_indexCount;
}


bool BubbleModelClass::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;


	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	// Load the vertex array and index array with data.
	for (i = 0; i < m_vertexCount; i++)
	{
		vertices[i].position = XMFLOAT3(m_model[i].x, m_model[i].y, m_model[i].z);
		vertices[i].texture = XMFLOAT2(m_model[i].tu, m_model[i].tv);
		indices[i] = i;
	}

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}


void BubbleModelClass::ShutdownBuffers()
{
	// Release the index buffer.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}


void BubbleModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
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


bool BubbleModelClass::LoadTextures(ID3D11Device* device, ID3D11DeviceContext* context, char* textureFilename1, char* textureFilename2)
{
	bool result;


	// Create the texture object.
	m_Texture1 = new TextureClass;
	if (!m_Texture1)
	{
		return false;
	}

	// Initialize the texture object.
	result = m_Texture1->Initialize(device, context, textureFilename1);
	if (!result)
	{
		return false;
	}

	// Create the texture object.
	m_Texture2 = new TextureClass;
	if (!m_Texture2)
	{
		return false;
	}

	// Initialize the texture object.
	result = m_Texture2->Initialize(device, context, textureFilename2);
	if (!result)
	{
		return false;
	}

	//// Create the texture object.
	//m_Texture3 = new TextureClass;
	//if (!m_Texture3)
	//{
	//	return false;
	//}

	//// Initialize the texture object.
	//result = m_Texture3->Initialize(device, context, textureFilename3);
	//if (!result)
	//{
	//	return false;
	//}

	return true;
}


void BubbleModelClass::ReleaseTextures()
{
	// Release the texture objects.
	if (m_Texture1)
	{
		m_Texture1->Shutdown();
		delete m_Texture1;
		m_Texture1 = 0;
	}

	if (m_Texture2)
	{
		m_Texture2->Shutdown();
		delete m_Texture2;
		m_Texture2 = 0;
	}

	//if (m_Texture3)
	//{
	//	m_Texture3->Shutdown();
	//	delete m_Texture3;
	//	m_Texture3 = 0;
	//}

	return;
}


ID3D11ShaderResourceView* BubbleModelClass::GetTexture1()
{
	return m_Texture1->GetTexture();
}


ID3D11ShaderResourceView* BubbleModelClass::GetTexture2()
{
	return m_Texture2->GetTexture();
}


//ID3D11ShaderResourceView* BubbleModelClass::GetTexture3()
//{
//	return m_Texture3->GetTexture();
//}


bool BubbleModelClass::LoadModel(char* filename)
{
	ifstream fin;
	char input;
	int i;


	// Open the model file.  If it could not open the file then exit.
	fin.open(filename);

	// If it could not open the file then exit.
	if (fin.fail())
	{
		return false;
	}

	// Read up to the value of vertex count.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}

	// Read in the vertex count.
	fin >> m_vertexCount;

	// Set the number of indices to be the same as the vertex count.
	m_indexCount = m_vertexCount;

	// Create the model using the vertex count that was read in.
	m_model = new ModelType[m_vertexCount];
	if (!m_model)
	{
		return false;
	}

	// Read up to the beginning of the data.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	// Read in the vertex data.
	for (i = 0; i < m_vertexCount; i++)
	{
		fin >> m_model[i].x >> m_model[i].y >> m_model[i].z;
		fin >> m_model[i].tu >> m_model[i].tv;
		fin >> m_model[i].nx >> m_model[i].ny >> m_model[i].nz;
	}

	// Close the model file.
	fin.close();

	return true;
}


void BubbleModelClass::ReleaseModel()
{
	if (m_model)
	{
		delete[] m_model;
		m_model = 0;
	}

	return;
}