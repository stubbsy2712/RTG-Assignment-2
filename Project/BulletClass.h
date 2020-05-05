#pragma once
#include <DirectXMath.h>
#include "modelclass.h"

using namespace DirectX;

class BulletClass
{
private:
	XMFLOAT3 position;
	XMFLOAT3 rotation;
	XMFLOAT3 velocity;
	ModelClass* model;
	float lifeSpan = 3;
public:
	BulletClass();
	~BulletClass();
	//bool Initialize(ID3D11Device*, ID3D11DeviceContext*, char*, char*, char*, XMFLOAT3*, XMFLOAT3*, float);
	bool InitializeWithModel(ModelClass*, XMFLOAT3*, XMFLOAT3*, float);
	void Shutdown();
	XMFLOAT3* getPosition();
	XMFLOAT3* getRotation();
	void move(float);
	void Render(ID3D11DeviceContext*);
	int GetIndexCount();
};

