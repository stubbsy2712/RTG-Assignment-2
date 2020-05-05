#pragma once
#include <d3d11_1.h>
#include <DirectXMath.h>
#include <vector>
#include "Collidable.h"
using namespace DirectX;


#define OOBB 0
#define AABB 1

class TextureClass;

class BaseModelClass : public Collidable
{
public:
	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

	struct VertexType
	{
		XMFLOAT3  position;
		XMFLOAT2  texture;
		XMFLOAT3  normal;
	};
	BaseModelClass();
	~BaseModelClass();
public:
	XMVECTOR getObjectCenter();
	virtual bool InitializeBuffers(ID3D11Device* device);
	virtual ID3D11ShaderResourceView* GetTexture();
	virtual void ReleaseTexture();
	virtual void ShutdownBuffers();
	void Render(ID3D11DeviceContext *);
	virtual void RenderBoundingBox(ID3D11DeviceContext* device);
	virtual void RenderAABBBoundingBox(ID3D11DeviceContext* device);
	virtual bool LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename);
	virtual bool LoadModel(char* filename);
	virtual bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* modelFilename, char* textureFilename);
	virtual void CreateBoundingVolumes(ModelType* vertPosArray);
	virtual void Shutdown() = 0;
	virtual bool InitializeBoundingBoxBuffers(ID3D11Device* device, XMFLOAT3* vertsIn, ID3D11Buffer** bufferIn, int bufferOption);
	virtual bool CalculateAABB(ID3D11DeviceContext* device, XMMATRIX &worldSpace);
	virtual bool CalculateAABBLite(ID3D11DeviceContext* device, XMMATRIX &worldSpace);
	XMFLOAT3 boundingBoxMin, boundingBoxMax;
	XMMATRIX GetModelMatrix() const;
	virtual void ReleaseModel();
	int CollisionDetected(BaseModelClass* other);
	bool GetCollision();

	XMVECTOR GetTransform();
	XMFLOAT3 GetScale();
	float GetRotationSpeed();
	void SetScale(float x, float y, float z);
	void SetTransform(XMVECTOR vec);
	void SetRotationSpeed(float speed);


	XMVECTOR m_transformPosition;
	XMFLOAT3 m_Scale;
	float rotationSpeed = 0.0f;
private:
	virtual void RenderBuffers(ID3D11DeviceContext* device);
	
	XMVECTOR objectCenterOffset;
	bool m_Collision = false;

public:
	int GetVertexCount();
	int GetIndexCount();
	ModelType* GetModel();

	XMFLOAT3 GetBoundingBoxMin();
	XMFLOAT3 GetBoundingBoxMax();

	void SetDirOfTravel(XMVECTOR vec);
	XMVECTOR GetDirTravel();
	void SetName(char* name);
	char* GetName();


	
	ModelType* m_model, *m_boundingBoxModel;
private:
	XMMATRIX modelMatrix;
	

	XMVECTOR dirOfTravel;
public:
	char *m_name;
	int m_vertexCount, m_indexCount;
	VertexType *AABBVertices;
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	TextureClass* m_Texture;
	XMFLOAT3 boundingBoxVertBuffer[8];
	DWORD boundingBoxIndexBuffer[36];
	XMFLOAT3 AABBBoundingBoxVerts[8];

	ID3D11Buffer *m_boundingBoxVertexBuffer, *m_boundingBoxIndexBuffer, *m_AABBBoundingBoxVertexBuffer;


};

