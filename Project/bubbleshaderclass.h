////////////////////////////////////////////////////////////////////////////////
// Filename: bubbleshaderclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _BUBBLESHADERCLASS_H_
#define BUBBLESHADERCLASS_H_

//#include <d3d11.h>
#include <d3d11_1.h>
//#include <d3dcompiler.h>
#include <d3dx9shader.h>
#include <DirectXMath.h> 

using namespace DirectX;
#include <fstream>
using namespace std;

#pragma once
class BubbleShaderClass
{
private:

	//struct MatrixBufferType
	//{
	//	XMMATRIX  world;
	//	XMMATRIX  view;
	//	XMMATRIX  projection;
	//};

	//struct CameraBufferType
	//{
	//	XMFLOAT3  cameraPosition;
	//	float padding;
	//};

	//struct LightBufferType
	//{
	//	XMFLOAT4  ambientColor;
	//	XMFLOAT4  diffuseColor;
	//	XMFLOAT3  lightDirection;
	//	float specularPower;
	//	XMFLOAT4  specularColor;
	//};

public:
	BubbleShaderClass();
	BubbleShaderClass(const BubbleShaderClass&);
	~BubbleShaderClass();

	bool Initialize(ID3D11Device*, IDirect3DDevice9*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, const XMMATRIX&, const XMMATRIX&, const XMMATRIX&, ID3D11ShaderResourceView*, ID3D11ShaderResourceView* , const XMFLOAT3);
	//bool Render9(IDirect3DDeviceContext*, int, const XMMATRIX&, const XMMATRIX&, const XMMATRIX&, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, const XMFLOAT3);
	// c0   - { 0.0,  0.5, 1.0, 2.0}
	// c1   - { 4.0, .5pi, pi, 2pi}  
	// c2   - {1, -1/3!, 1/5!, -1/7!} for sin    (1.0f,      -1.0f/6.0f,  1.0f/120.0f, -1.0f/5040.0f)
	// c3   - {1/2!, -1/4!, 1/6!, -1/8!} for cos (1.0f/2.0f, -1.0f/24.0f, 1.0f/720.0f, -1.0f/40320.0f)
	// c4-7 - Composite World-View-Projection Matrix
	// c8   - Model Space Camera Position
	// c10  - {1.02, 0.04, 0, 0} fixup factor for Taylor series imprecision
	// c11  - {0.5, 0.5, 0.25, 0.25} waveHeight0, waveHeight1, waveHeight2, waveHeight3
	// c12  - {0.0, 0.0, 0.0, 0.0} waveOffset0, waveOffset1, waveOffset2, waveOffset3
	// c13  - {0.6, 0.7, 1.2, 1.4} waveSpeed0, waveSpeed1, waveSpeed2, waveSpeed3
	// c14  - {0.0, 2.0, 0.0, 4.0} waveDirX0, waveDirX1, waveDirX2, waveDirX3
	// c15  - {2.0, 0.0, 4.0, 0.0} waveDirY0, waveDirY1, waveDirY2, waveDirY3
	// c16  - { time }
	// c17  - {-0.00015, 1.0, 0.0, 0.0} base texcoord distortion x0, y0, x1, y1
	// c18  - World Martix
	//Copied from the vs file.

private:
	bool InitializeShader(ID3D11Device*, IDirect3DDevice9*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, const XMMATRIX&, const XMMATRIX&, const XMMATRIX&, ID3D11ShaderResourceView*, const XMFLOAT3);
	void RenderShader(ID3D11DeviceContext*, int);

private:
	//ID3D11VertexShader* m_vertexShader;
	//ID3D11PixelShader* m_pixelShader;
	IDirect3DVertexShader9* m_vertexShader9;
	IDirect3DPixelShader9* m_pixelShader9;
	ID3D11InputLayout* m_layout;
	ID3D11SamplerState* m_sampleState;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11Buffer* m_cameraBuffer;
	ID3D11Buffer* m_lightBuffer;

};

#endif