#pragma once
#include "Bat.h"
#include "WindowModelClass.h"
#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

class BatSpawner : public Collidable
{
public:
	Bat* spawnBatWithModel(FireModelClass*, ZoneClass*, CuboidZone*);
	BatSpawner();
	//bool Initialize(ID3D11Device*, ID3D11DeviceContext*, char*, char*, char*);
	bool InitializeWithModel(WindowModelClass*);
	void Shutdown();
	void setPosition(XMFLOAT3*);
	void Render(ID3D11DeviceContext*);
	XMFLOAT3* getPosition();
	WindowModelClass* getModel();
	int GetIndexCount();
private:
	WindowModelClass* m_model;
};

