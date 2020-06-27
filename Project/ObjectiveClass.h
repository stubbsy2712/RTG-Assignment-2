#pragma once
#include "modelclass.h"
#include <d3d11.h>
#include <DirectXMath.h>
#include "Collidable.h"
using namespace DirectX;
class ObjectiveClass : public Collidable
{
private:
	ModelClass* m_model;
public:
	void initializeWithModel(ModelClass*, XMFLOAT3*);
	ModelClass* getModel();
	ObjectiveClass();
	void Shutdown();
	~ObjectiveClass();
protected:
	void collisionOccured(ZoneClass*);
};

