/*
#include "bubbleshaderclass.h" 
//#include <d3dx9shader.h>
#include <d3dcompiler.h>

BubbleShaderClass::BubbleShaderClass()
{
	m_vertexShader9 = 0;
	m_pixelShader9 = 0;
	m_layout = 0;
	m_sampleState = 0;
	m_matrixBuffer = 0;
	m_cameraBuffer = 0;
	m_lightBuffer = 0;
}


BubbleShaderClass::BubbleShaderClass(const BubbleShaderClass& other)
{
}


BubbleShaderClass::~BubbleShaderClass()
{
}

bool BubbleShaderClass::Initialize(ID3D11Device* device, IDirect3DDevice9* device9, HWND hwnd)
{
	bool result;
	
	// Initialize the vertex and pixel shaders.
	result = InitializeShader(device, device9, hwnd, L"../Project/bubbleshader.vs", L"../Project/bubbleshader.ps");
	if (!result)
	{
		return false;
	}

	return true;
}

void BubbleShaderClass::Shutdown()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
	ShutdownShader();

	return;
}

bool BubbleShaderClass::Render(ID3D11DeviceContext* deviceContext, int indexCount, const XMMATRIX& worldMatrix, const XMMATRIX& viewMatrix, const XMMATRIX& projectionMatrix,
	ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* filmTexture, const XMFLOAT3 cameraPosition)
{
	bool result;

	// Set the shader parameters that it will use for rendering.
	result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, texture, cameraPosition);
	if (!result)
	{
		return false;
	}

	// Now render the prepared buffers with the shader.
	RenderShader(deviceContext, indexCount);

	return true;
}

//bool BubbleShaderClass::Render(ID3D11DeviceContext* deviceContext, int indexCount, const XMMATRIX& worldMatrix, const XMMATRIX& viewMatrix,
//	const XMMATRIX& projectionMatrix, ID3D11ShaderResourceView* texture, XMFLOAT3 lightDirection, XMFLOAT4 ambientColor,
//	XMFLOAT4 diffuseColor, XMFLOAT3 cameraPosition, XMFLOAT4 specularColor, float specularPower)
//{
//	bool result;
//
//	// Set the shader parameters that it will use for rendering.
//	result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, texture, lightDirection, ambientColor, diffuseColor,
//		cameraPosition, specularColor, specularPower);
//	if (!result)
//	{
//		return false;
//	}
//
//	// Now render the prepared buffers with the shader.
//	RenderShader(deviceContext, indexCount);
//
//	return true;
//}

bool BubbleShaderClass::InitializeShader(ID3D11Device* device,  IDirect3DDevice9* device9, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename)
{
	HRESULT result;
	//LPD3DXBUFFER* vertexShaderBuffer9;
	//LPD3DXBUFFER* pixelShaderBuffer9;
	//LPD3DXBUFFER* errorMessage9;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[3];
	unsigned int numElements;
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_BUFFER_DESC cameraBufferDesc;
	D3D11_BUFFER_DESC lightBufferDesc;
	
	// Initialize the pointers this function will use to null.
	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;
	//errorMessage9 = new LPD3DXBUFFER;
	//vertexShaderBuffer9 = new LPD3DXBUFFER;
	//pixelShaderBuffer9 = new LPD3DXBUFFER;

	// Compile the vertex shader code.
	//psrcfile, hsrcmodule, psrcresource, psrcdata, stcdatalen
	
	//HRESULT WINAPI
	//	D3DXAssembleShaderFromFileW(
	//		LPCWSTR                         pSrcFile,
	//		CONST D3DXMACRO *				pDefines,
	//		LPD3DXINCLUDE                   pInclude,
	//		DWORD                           Flags,
	//		LPD3DXBUFFER *					ppShader,
	//		LPD3DXBUFFER *					ppErrorMsgs);

	//wstring wc = L"bubbleshader.ps";
	LPCWSTR pSrcFile = L"bubbleshader.ps";
	
	result = -1;

	// Compile the pixel shader code.
	//result = D3DXAssembleShaderFromFile(pSrcFile, NULL, NULL, NULL, pixelShaderBuffer9, errorMessage9);

	if (FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, psFilename);
		}
		// If there was nothing in the error message then it simply could not find the file itself.
		else
		{
			MessageBox(hwnd, psFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	pSrcFile = L"bubbleshader.vs";

	//result = D3DXAssembleShaderFromFile(pSrcFile, NULL, NULL, NULL, vertexShaderBuffer9, errorMessage9);
	if (FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, vsFilename);
		}
		// If there was nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(hwnd, vsFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}
	// Create the vertex shader from the buffer.

	// DirectX 9!
	D3DVERTEXELEMENT9 MyDecl[] =
	{  
		// stream, offset, type, method, usage, usage index
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},  
		{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
		{ 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
		// Second stream is second mesh  
		{ 1, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 1},
		{ 1, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 1},
		D3DDECL_END()
	}; // and to validate and create the tokenized version

	IDirect3DVertexDeclaration9* pDecl;
	device9->CreateVertexDeclaration(MyDecl, &pDecl);
	// Set the declaration
	device9->SetVertexDeclaration(pDecl);

	DWORD bufferThing[] =
	{
		1, 2, 3, 4
	};

	result = device9->CreateVertexShader(bufferThing, &m_vertexShader9);

	//result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
	//if (FAILED(result))
	//{
	//	return false;
	//}

	//// Create the pixel shader from the buffer.
	//result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
	//if (FAILED(result))
	//{
	//	return false;
	//}

	

	// Create the vertex input layout description.
	// This setup needs to match the VertexType stucture in the ModelClass and in the shader.
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	polygonLayout[2].SemanticName = "NORMAL";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;

	// Get a count of the elements in the layout.
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create the vertex input layout.
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(),
		&m_layout);
	if (FAILED(result))
	{
		return false;
	}

	// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	// Create a texture sampler state description.
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	result = device->CreateSamplerState(&samplerDesc, &m_sampleState);
	if (FAILED(result))
	{
		return false;
	}

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	//matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Setup the description of the camera dynamic constant buffer that is in the vertex shader.
	cameraBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	//cameraBufferDesc.ByteWidth = sizeof(CameraBufferType);
	cameraBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cameraBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cameraBufferDesc.MiscFlags = 0;
	cameraBufferDesc.StructureByteStride = 0;

	// Create the camera constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = device->CreateBuffer(&cameraBufferDesc, NULL, &m_cameraBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Setup the description of the light dynamic constant buffer that is in the pixel shader.
	// Note that ByteWidth always needs to be a multiple of 16 if using D3D11_BIND_CONSTANT_BUFFER or CreateBuffer will fail.
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	//lightBufferDesc.ByteWidth = sizeof(LightBufferType);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = device->CreateBuffer(&lightBufferDesc, NULL, &m_lightBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void BubbleShaderClass::ShutdownShader()
{
	// Release the light constant buffer.
	if (m_lightBuffer)
	{
		m_lightBuffer->Release();
		m_lightBuffer = 0;
	}

	// Release the camera constant buffer.
	if (m_cameraBuffer)
	{
		m_cameraBuffer->Release();
		m_cameraBuffer = 0;
	}

	// Release the matrix constant buffer.
	if (m_matrixBuffer)
	{
		m_matrixBuffer->Release();
		m_matrixBuffer = 0;
	}

	// Release the sampler state.
	if (m_sampleState)
	{
		m_sampleState->Release();
		m_sampleState = 0;
	}

	// Release the layout.
	if (m_layout)
	{
		m_layout->Release();
		m_layout = 0;
	}

	// Release the pixel shader.
	if (m_pixelShader9)
	{
		m_pixelShader9->Release();
		m_pixelShader9 = 0;
	}

	// Release the vertex shader.
	if (m_vertexShader9)
	{
		m_vertexShader9->Release();
		m_vertexShader9 = 0;
	}

	return;
}

void BubbleShaderClass::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename)
{
	char* compileErrors;
	unsigned long bufferSize, i;
	ofstream fout;


	// Get a pointer to the error message text buffer.
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// Get the length of the message.
	bufferSize = errorMessage->GetBufferSize();

	// Open a file to write the error message to.
	fout.open("shader-error.txt");

	// Write out the error message.
	for (i = 0; i < bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	// Close the file.
	fout.close();

	// Release the error message.
	errorMessage->Release();
	errorMessage = 0;

	// Pop a message up on the screen to notify the user to check the text file for compile errors.
	MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);

	return;
}


bool BubbleShaderClass::SetShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX& worldMatrix, const XMMATRIX& viewMatrix,
	const XMMATRIX& projectionMatrix, ID3D11ShaderResourceView* texture, XMFLOAT3 cameraPosition)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	unsigned int bufferNumber;
	XMFLOAT4 c0 = { 0.0f,  0.5f, 1.0f, 2.0f };//helper constants
	XMFLOAT4 c1 = { 4.0f, 1.50796f, 3.14159f, 6.28219f };//multiples of pi
	XMFLOAT4 c2 = { 1.0f, -0.16667f, 0.00833, 0.00020 };//factorials for sin
	XMFLOAT4 c3 = { };//factorials for cos
	//Maybe need to transpose

	XMFLOAT4X4 compositeMatrix;
	XMStoreFloat4x4(&compositeMatrix, XMMatrixMultiply(viewMatrix, projectionMatrix));

	XMFLOAT4 c4 = { compositeMatrix._11,  compositeMatrix._12, compositeMatrix._13, compositeMatrix._14 };
	XMFLOAT4 c5 = { compositeMatrix._21,  compositeMatrix._22, compositeMatrix._23, compositeMatrix._24 };
	XMFLOAT4 c6 = { compositeMatrix._31,  compositeMatrix._32, compositeMatrix._33, compositeMatrix._34 };
	XMFLOAT4 c7 = { compositeMatrix._41,  compositeMatrix._42, compositeMatrix._43, compositeMatrix._44 };
	XMFLOAT3 c8 = cameraPosition;
	//There is no c9 :/
	XMFLOAT4 c10 = { 1.02f, 0.04f, 0.0f, 0.0f };//fixup factor for Taylor series imprecision
	XMFLOAT4 c11;//4 wave heights
	XMFLOAT4 c12;//4 wave offsets
	XMFLOAT4 c13;//4 wave speeds
	XMFLOAT4 c14;//4 wave dirXs
	XMFLOAT4 c15;//4 wave dirYs
	XMFLOAT4 c16;//time
	XMFLOAT4 c17;//texture coordinate distortion
	XMFLOAT4 c18;//worldMatrix[0]
	XMFLOAT4 c19;//worldMatrix[1]
	XMFLOAT4 c20;//worldMatrix[2]
	XMFLOAT4 c21;//worldMatrix[3]
	//MatrixBufferType* dataPtr;
	//LightBufferType* dataPtr2;
	//CameraBufferType* dataPtr3;


	// Transpose the matrices to prepare them for the shader.

	// Lock the constant buffer so it can be written to.
	result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	//dataPtr = (MatrixBufferType*)mappedResource.pData;
	// Copy the matrices into the constant buffer.
	//dataPtr->world = XMMatrixTranspose(worldMatrix);
	//dataPtr->view = XMMatrixTranspose(viewMatrix);
	//dataPtr->projection = XMMatrixTranspose(projectionMatrix);

	// Unlock the constant buffer.
	deviceContext->Unmap(m_matrixBuffer, 0);

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// Now set the constant buffer in the vertex shader with the updated values.
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);

	// Lock the camera constant buffer so it can be written to.
	result = deviceContext->Map(m_cameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	//dataPtr3 = (CameraBufferType*)mappedResource.pData;
	// Copy the camera position into the constant buffer.
	//dataPtr3->cameraPosition = cameraPosition;
	//dataPtr3->padding = 0.0f;

	// Unlock the camera constant buffer.
	deviceContext->Unmap(m_cameraBuffer, 0);

	// Set the position of the camera constant buffer in the vertex shader.
	bufferNumber = 1;

	// Now set the camera constant buffer in the vertex shader with the updated values.
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_cameraBuffer);

	// Set shader texture resource in the pixel shader.
	deviceContext->PSSetShaderResources(0, 1, &texture);

	// Lock the light constant buffer so it can be written to.
	result = deviceContext->Map(m_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the light constant buffer.
	//dataPtr2 = (LightBufferType*)mappedResource.pData;
	// Copy the lighting variables into the light constant buffer.
	//dataPtr2->ambientColor = ambientColor;
	//dataPtr2->diffuseColor = diffuseColor;
	//dataPtr2->lightDirection = lightDirection;
	//dataPtr2->specularColor = specularColor;
	//dataPtr2->specularPower = specularPower;

	// Unlock the light constant buffer.
	deviceContext->Unmap(m_lightBuffer, 0);

	// Set the position of the light constant buffer in the pixel shader.
	bufferNumber = 0;

	// Finally set the light constant buffer in the pixel shader with the updated values.
	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_lightBuffer);

	return true;
}


void BubbleShaderClass::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	// Set the vertex input layout.
	deviceContext->IASetInputLayout(m_layout);

	// Set the vertex and pixel shaders that will be used to render this triangle.
	
	//deviceContext->VSSetShader(m_vertexShader, NULL, 0);
	//deviceContext->PSSetShader(m_pixelShader, NULL, 0);

	// Set the sampler state in the pixel shader.
	deviceContext->PSSetSamplers(0, 1, &m_sampleState);

	// Render the triangle.
	deviceContext->DrawIndexed(indexCount, 0, 0);

	return;
}
*/