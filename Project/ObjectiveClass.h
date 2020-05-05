#pragma once
#include "bubblemodelclass.h"
class ObjectiveClass
{
private:
	BubbleModelClass* m_model;
	XMFLOAT3* m_position;
public:
	void initialize(BubbleModelClass*, XMFLOAT3*);
	ObjectiveClass();
	void Shutdown();
	~ObjectiveClass();
};

