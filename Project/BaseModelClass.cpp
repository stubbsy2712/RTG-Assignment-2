#include "BaseModelClass.h"
#include "textureclass.h"
#include <fstream>
#include <iostream>

using namespace DirectX;

BaseModelClass::BaseModelClass()
{
	
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_model = 0;
	m_Scale = {1.0f, 1.0f, 1.0f};
	m_transformPosition = {};
	m_name = 0;
	boundingBoxMin = {};
	boundingBoxMax = {};
	rotationSpeed = 0;
}


BaseModelClass::~BaseModelClass()
{
}

bool BaseModelClass::Initialize(ID3D11Device * device, ID3D11DeviceContext* deviceContext, char * modelFilename, char * textureFilename)
{
	bool result;
	modelMatrix = XMMatrixIdentity();
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

	//Initialize OOBB buffer
	result = InitializeBoundingBoxBuffers(device, boundingBoxVertBuffer, &m_boundingBoxVertexBuffer, OOBB);
	if (!result)
	{
		return false;
	}

	//Initialize AABB buffer
	result = InitializeBoundingBoxBuffers(device, AABBBoundingBoxVerts, &m_AABBBoundingBoxVertexBuffer, AABB);
	if (!result)
	{
		return false;
	}

	// Load the texture for this model.
	result = LoadTexture(device, deviceContext, textureFilename);
	if (!result)
	{
		return false;
	}

	return true;
}

bool BaseModelClass::LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename)
{
	bool result;


	// Create the texture object.
	m_Texture = new TextureClass;
	if (!m_Texture)
	{
		return false;
	}

	// Initialize the texture object.
	result = m_Texture->Initialize(device, deviceContext, filename);
	if (!result)
	{
		return false;
	}

	return true;
}

bool BaseModelClass::LoadModel(char* filename)
{
	std::ifstream fin;
	char input;
	int i;


	// Open the model file.
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

	CreateBoundingVolumes(m_model);

	return true;
}

XMVECTOR BaseModelClass::getObjectCenter()
{
	return objectCenterOffset;
}

bool BaseModelClass::InitializeBuffers(ID3D11Device * device)
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
		vertices[i].normal = XMFLOAT3(m_model[i].nx, m_model[i].ny, m_model[i].nz);

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

ID3D11ShaderResourceView* BaseModelClass::GetTexture()
{
	return m_Texture->GetTexture();
}

void BaseModelClass::ReleaseTexture()
{
	// Release the texture object.

	if(m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}

	return;
}

int BaseModelClass::GetIndexCount()
{
	return m_indexCount;
}

BaseModelClass::ModelType * BaseModelClass::GetModel()
{
	return m_model;
}

XMFLOAT3 BaseModelClass::GetBoundingBoxMin()
{
	return boundingBoxMin;
}

XMFLOAT3 BaseModelClass::GetBoundingBoxMax()
{
	return boundingBoxMax;
}

void BaseModelClass::SetDirOfTravel(XMVECTOR vec)
{
	dirOfTravel = vec;
}

XMVECTOR BaseModelClass::GetDirTravel()
{
	return dirOfTravel;
}

void BaseModelClass::SetScale(float x, float y, float z)
{
	m_Scale = {x, y, z};
}

void BaseModelClass::SetTransform(XMVECTOR vec)
{
	m_transformPosition = vec;
}

void BaseModelClass::SetRotationSpeed(float speed)
{
	rotationSpeed = speed;
}

XMFLOAT3 BaseModelClass::GetScale()
{
	return m_Scale;
}

float BaseModelClass::GetRotationSpeed()
{
	return rotationSpeed;
}

XMVECTOR BaseModelClass::GetTransform()
{
	return m_transformPosition;
}

void BaseModelClass::SetName(char * name)
{
	m_name = name;
}

char * BaseModelClass::GetName()
{
	return m_name;
}

void BaseModelClass::ShutdownBuffers()
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

	if (m_boundingBoxVertexBuffer)
	{
		m_boundingBoxVertexBuffer->Release();
		m_boundingBoxVertexBuffer = 0;
	}
	if (m_boundingBoxIndexBuffer) 
	{
		m_boundingBoxIndexBuffer->Release();
		m_boundingBoxIndexBuffer = 0;
	}
	if (m_AABBBoundingBoxVertexBuffer)
	{
		m_AABBBoundingBoxVertexBuffer->Release();
		m_AABBBoundingBoxVertexBuffer = 0;
	}
	return;
}

void BaseModelClass::Render(ID3D11DeviceContext *deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return;
}

void BaseModelClass::RenderBoundingBox(ID3D11DeviceContext * deviceContext)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_boundingBoxVertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_boundingBoxIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

void BaseModelClass::RenderAABBBoundingBox(ID3D11DeviceContext * deviceContext)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_AABBBoundingBoxVertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_boundingBoxIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

void BaseModelClass::RenderBuffers(ID3D11DeviceContext * deviceContext)
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

int BaseModelClass::GetVertexCount()
{
	return m_vertexCount;
}

void BaseModelClass::CreateBoundingVolumes(ModelType* vertPosArray)
{
	XMFLOAT3 minVertex = XMFLOAT3(FLT_MAX, FLT_MAX, FLT_MAX);
	XMFLOAT3 maxVertex = XMFLOAT3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	for (UINT i = 0; i < GetVertexCount(); i++)
	{
		// The minVertex and maxVertex will most likely not be actual vertices in the model, but vertices
		// that use the smallest and largest x, y, and z values from the model to be sure ALL vertices are
		// covered by the bounding volume

		//Get the smallest vertex 
		if (vertPosArray[i].x < minVertex.x) {		// Find smallest x value in model
			minVertex.x = vertPosArray[i].x;
		}
		if (vertPosArray[i].y < minVertex.y) {		// Find smallest y value in model
			minVertex.y = vertPosArray[i].y;
		}
		if (vertPosArray[i].z < minVertex.z) {		// Find smallest z value in model
			minVertex.z = vertPosArray[i].z;
		}
		if (vertPosArray[i].x > maxVertex.x) {		// Find largest x value in model
			maxVertex.x = vertPosArray[i].x;
		}
		if (vertPosArray[i].y > maxVertex.y) {		// Find largest y value in model
			maxVertex.y = vertPosArray[i].y;
		}
		if (vertPosArray[i].z > maxVertex.z) {		// Find largest z value in model
			maxVertex.z = vertPosArray[i].z;
		}
		
	}

	// Compute distance between maxVertex and minVertex
	float distX = (maxVertex.x - minVertex.x) / 2.0f;
	float distY = (maxVertex.y - minVertex.y) / 2.0f;
	float distZ = (maxVertex.z - minVertex.z) / 2.0f;

	// Now store the distance between (0, 0, 0) in model space to the models real center
	objectCenterOffset = XMVectorSet(maxVertex.x - distX, maxVertex.y - distY, maxVertex.z - distZ, 0.0f);

	// Compute bounding sphere (distance between min and max bounding box vertices)
	// boundingSphere = sqrt(distX*distX + distY*distY + distZ*distZ) / 2.0f;
	//boundingSphere = XMVectorGetX(XMVector3Length(XMVectorSet(distX, distY, distZ, 0.0f)));


	// Create bounding box    
	// Front Vertices

	boundingBoxVertBuffer[0] = XMFLOAT3(minVertex.x, minVertex.y, minVertex.z);
	boundingBoxVertBuffer[1] = XMFLOAT3(minVertex.x, maxVertex.y, minVertex.z);
	boundingBoxVertBuffer[2] = XMFLOAT3(maxVertex.x, maxVertex.y, minVertex.z);
	boundingBoxVertBuffer[3] = XMFLOAT3(maxVertex.x, minVertex.y, minVertex.z);

	boundingBoxVertBuffer[4] = XMFLOAT3(minVertex.x, minVertex.y, maxVertex.z);
	boundingBoxVertBuffer[5] = XMFLOAT3(maxVertex.x, minVertex.y, maxVertex.z);
	boundingBoxVertBuffer[6] = XMFLOAT3(maxVertex.x, maxVertex.y, maxVertex.z);
	boundingBoxVertBuffer[7] = XMFLOAT3(minVertex.x, maxVertex.y, maxVertex.z);

	AABBBoundingBoxVerts;
	for (int i = 0; i < 8; i++) {
		AABBBoundingBoxVerts[i] = boundingBoxVertBuffer[i];
	}

	// Front Face
	boundingBoxIndexBuffer[0] = 0; boundingBoxIndexBuffer[1] = 1; boundingBoxIndexBuffer[2] = 2;
	boundingBoxIndexBuffer[3] = 0; boundingBoxIndexBuffer[4] = 2; boundingBoxIndexBuffer[5] = 3;

	// Back Face
	boundingBoxIndexBuffer[6] = 4; boundingBoxIndexBuffer[7] = 5; boundingBoxIndexBuffer[8] = 6;
	boundingBoxIndexBuffer[9] = 4; boundingBoxIndexBuffer[10] = 6; boundingBoxIndexBuffer[11] = 7;

	// Top Face
	boundingBoxIndexBuffer[12] = 1; boundingBoxIndexBuffer[13] = 7; boundingBoxIndexBuffer[14] = 6;
	boundingBoxIndexBuffer[15] = 1; boundingBoxIndexBuffer[16] = 6; boundingBoxIndexBuffer[17] = 2;

	// Bottom Face
	boundingBoxIndexBuffer[18] = 0; boundingBoxIndexBuffer[19] = 5; boundingBoxIndexBuffer[20] = 4;
	boundingBoxIndexBuffer[21] = 0; boundingBoxIndexBuffer[22] = 3; boundingBoxIndexBuffer[23] = 5;

	// Left Face
	boundingBoxIndexBuffer[24] = 4; boundingBoxIndexBuffer[25] = 7; boundingBoxIndexBuffer[26] = 1;
	boundingBoxIndexBuffer[27] = 4; boundingBoxIndexBuffer[28] = 1; boundingBoxIndexBuffer[29] = 0;

	// Right Face
	boundingBoxIndexBuffer[30] = 3; boundingBoxIndexBuffer[31] = 2; boundingBoxIndexBuffer[32] = 6;
	boundingBoxIndexBuffer[33] = 3; boundingBoxIndexBuffer[34] = 6; boundingBoxIndexBuffer[35] = 5;

}

bool BaseModelClass::InitializeBoundingBoxBuffers(ID3D11Device * device, XMFLOAT3* vertsIn, ID3D11Buffer** bufferIn, int bufferOption)
{
	VertexType* vertices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;

	int vertexCount = 8;
	int indexCount = 36;


	if (bufferOption == OOBB) {
		// Create the vertex array.
		vertices = new VertexType[vertexCount];
		if (!vertices)
		{
			return false;
		}

		// Load the vertex array and index array with data.
		for (i = 0; i < vertexCount; i++)
		{
			vertices[i].position = XMFLOAT3(vertsIn[i].x, vertsIn[i].y, vertsIn[i].z);

		}

		// Set up the description of the static vertex buffer.
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.ByteWidth = sizeof(VertexType) * vertexCount;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;

		// Give the subresource structure a pointer to the vertex data.
		vertexData.pSysMem = vertices;
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;

		// Now create the vertex buffer.
		result = device->CreateBuffer(&vertexBufferDesc, &vertexData, bufferIn);
		if (FAILED(result))
		{
			return false;
		}
		// Release the arrays now that the vertex and index buffers have been created and loaded.

		delete[] vertices;
		vertices = 0;
	}

	//When creating an AABB buffer
	if (bufferOption == AABB) {
		// Create the vertex array.
		AABBVertices = new VertexType[vertexCount];
		if (!AABBVertices)
		{
			return false;
		}

		// Load the vertex array and index array with data.
		for (i = 0; i < vertexCount; i++)
		{
			AABBVertices[i].position = XMFLOAT3(vertsIn[i].x, vertsIn[i].y, vertsIn[i].z);
		}

		// Set up the description of the dynamic vertex buffer.
		vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		vertexBufferDesc.ByteWidth = sizeof(VertexType) * vertexCount;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;

		// Give the subresource structure a pointer to the vertex data.
		vertexData.pSysMem = AABBVertices;
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;
		

		// Now create the vertex buffer.
		result = device->CreateBuffer(&vertexBufferDesc, &vertexData, bufferIn);
		if (FAILED(result))
		{
			return false;
		}
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(DWORD) * indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = boundingBoxIndexBuffer;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_boundingBoxIndexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

bool BaseModelClass::CalculateAABBLite(ID3D11DeviceContext* deviceContext, XMMATRIX & worldSpace)
{
	XMFLOAT3 minVertex = XMFLOAT3(FLT_MAX, FLT_MAX, FLT_MAX);
	XMFLOAT3 maxVertex = XMFLOAT3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	//Loop through the 8 vertices describing the bounding box
	for (UINT i = 0; i < 8; i++)
	{
		//Transform the bounding boxes vertices to the objects world space
		XMVECTOR Vert = XMVectorSet(boundingBoxVertBuffer[i].x, boundingBoxVertBuffer[i].y, boundingBoxVertBuffer[i].z, 0.0f);
		Vert = XMVector3TransformCoord(Vert, worldSpace);

		//Get the smallest vertex 
		minVertex.x = min(minVertex.x, XMVectorGetX(Vert));    // Find smallest x value in model
		minVertex.y = min(minVertex.y, XMVectorGetY(Vert));    // Find smallest y value in model
		minVertex.z = min(minVertex.z, XMVectorGetZ(Vert));    // Find smallest z value in model

															   //Get the largest vertex 
		maxVertex.x = max(maxVertex.x, XMVectorGetX(Vert));    // Find largest x value in model
		maxVertex.y = max(maxVertex.y, XMVectorGetY(Vert));    // Find largest y value in model
		maxVertex.z = max(maxVertex.z, XMVectorGetZ(Vert));    // Find largest z value in model
	}

	//Store Bounding Box's min and max vertices
	boundingBoxMin = { minVertex.x, minVertex.y, minVertex.z };
	boundingBoxMax = { maxVertex.x, maxVertex.y, maxVertex.z };

	return true;
}
bool BaseModelClass::CalculateAABB(ID3D11DeviceContext* deviceContext, XMMATRIX & worldSpace)
{

		XMFLOAT3 minVertex = XMFLOAT3(FLT_MAX, FLT_MAX, FLT_MAX);
		XMFLOAT3 maxVertex = XMFLOAT3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

		//Loop through the 8 vertices describing the bounding box
		for (UINT i = 0; i < 8; i++)
		{
			//Transform the bounding boxes vertices to the objects world space
			XMVECTOR Vert = XMVectorSet(boundingBoxVertBuffer[i].x, boundingBoxVertBuffer[i].y, boundingBoxVertBuffer[i].z, 0.0f);
			Vert = XMVector3TransformCoord(Vert, worldSpace);

			//Get the smallest vertex 
			minVertex.x = min(minVertex.x, XMVectorGetX(Vert));    // Find smallest x value in model
			minVertex.y = min(minVertex.y, XMVectorGetY(Vert));    // Find smallest y value in model
			minVertex.z = min(minVertex.z, XMVectorGetZ(Vert));    // Find smallest z value in model

																   //Get the largest vertex 
			maxVertex.x = max(maxVertex.x, XMVectorGetX(Vert));    // Find largest x value in model
			maxVertex.y = max(maxVertex.y, XMVectorGetY(Vert));    // Find largest y value in model
			maxVertex.z = max(maxVertex.z, XMVectorGetZ(Vert));    // Find largest z value in model
		}

		//Store Bounding Box's min and max vertices
		boundingBoxMin = { minVertex.x, minVertex.y, minVertex.z};
		boundingBoxMax = { maxVertex.x, maxVertex.y, maxVertex.z };

		//Create the AABB bounding box for rendering
		
		AABBBoundingBoxVerts[0] = { minVertex.x, minVertex.y, minVertex.z };
		AABBBoundingBoxVerts[1] = { minVertex.x, maxVertex.y, minVertex.z };
		AABBBoundingBoxVerts[2] = { maxVertex.x, maxVertex.y, minVertex.z };
		AABBBoundingBoxVerts[3] = { maxVertex.x, minVertex.y, minVertex.z };

		AABBBoundingBoxVerts[4] = { minVertex.x, minVertex.y, maxVertex.z };
		AABBBoundingBoxVerts[5] = { maxVertex.x, minVertex.y, maxVertex.z };
		AABBBoundingBoxVerts[6] = { maxVertex.x, maxVertex.y, maxVertex.z };
		AABBBoundingBoxVerts[7] = { minVertex.x, maxVertex.y, maxVertex.z };

		D3D11_MAPPED_SUBRESOURCE mappedResource;
		ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

		// Load the vertex array and index array with data.
		for (int i = 0; i < 8; i++)
		{
			AABBVertices[i].position = XMFLOAT3(AABBBoundingBoxVerts[i].x, AABBBoundingBoxVerts[i].y, AABBBoundingBoxVerts[i].z);

		}

		deviceContext->Map(m_AABBBoundingBoxVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		memcpy(mappedResource.pData, AABBVertices, sizeof( VertexType) * 8);
		deviceContext->Unmap(m_AABBBoundingBoxVertexBuffer, 0);

	return true;
}

XMMATRIX BaseModelClass::GetModelMatrix() const
{
	return modelMatrix;
}

void BaseModelClass::ReleaseModel()
{
	if(m_model)
	{
	delete [] m_model;
	m_model = 0;
	}

	return;
}

int BaseModelClass::CollisionDetected(BaseModelClass* other)
{
	if (this->m_name == "Football" && other->m_name == "GoalPosts") 
	{
		return 1;
	}

	if (this->m_name == "Football" && other->m_name == "FlameNinja")
	{
		return 10;
	}

	m_Collision = !m_Collision;
	return 0;
}

bool BaseModelClass::GetCollision()
{
	return m_Collision;
}