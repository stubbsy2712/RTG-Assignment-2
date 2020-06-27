/*
////////////////////////////////////////////////////////////////////////////////
// Filename: bubblemodelclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _BUBBLEMODELCLASS_H_
#define BUBBLEMODELCLASS_H_



//////////////
// INCLUDES //
//////////////
#include <d3d11_1.h>
#include <directXMath.h>
using namespace DirectX;

#include <fstream>
using namespace std;


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "textureclass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: BubbleModelClass
////////////////////////////////////////////////////////////////////////////////
#pragma once
class BubbleModelClass
{
private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

public:
	BubbleModelClass();
	BubbleModelClass(const BubbleModelClass&);
	~BubbleModelClass();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, char*, char*, char*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

	ID3D11ShaderResourceView* GetTexture1();
	ID3D11ShaderResourceView* GetTexture2();
	//ID3D11ShaderResourceView* GetTexture3();

	XMFLOAT3 position;

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTextures(ID3D11Device*, ID3D11DeviceContext*, char*, char*);
	void ReleaseTextures();

	bool LoadModel(char*);
	void ReleaseModel();

private:
	ID3D11Buffer* m_vertexBuffer, * m_indexBuffer;
	int m_vertexCount, m_indexCount;
	//TextureClass* m_Texture1, * m_Texture2, * m_Texture3;
	TextureClass* m_Texture1, * m_Texture2;//, * m_Texture3;
	ModelType* m_model;

};

#endif
*/