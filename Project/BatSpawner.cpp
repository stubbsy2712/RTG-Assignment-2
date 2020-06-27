#include "BatSpawner.h"

Bat* BatSpawner::spawnBatWithModel(FireModelClass* batModel, ZoneClass* zone, CuboidZone* flyZone)
{
	Bat* batPtr = new Bat();
	batPtr->setPosition(m_position->x, m_position->y + 3, m_position->z);
	batPtr->setRotation(m_rotation->x, m_rotation->y, m_rotation->z);
	batPtr->InitializeWithModel(batModel, zone, flyZone);
	batPtr->m_xSize = 2;
	batPtr->m_ySize = 1;
	batPtr->m_zSize = 2;
	return batPtr;
}

BatSpawner::BatSpawner()
{
}

bool BatSpawner::InitializeWithModel(WindowModelClass* model)
{
	m_model = model;
	return true;
}

void BatSpawner::Shutdown()
{
	m_model->Shutdown();
}

void BatSpawner::setPosition(XMFLOAT3* givenPosition)
{
	m_position = givenPosition;
}

void BatSpawner::Render(ID3D11DeviceContext* context)
{
	m_model->Render(context);
}

XMFLOAT3* BatSpawner::getPosition()
{
	return m_position;
}

WindowModelClass* BatSpawner::getModel()
{
	return m_model;
}

int BatSpawner::GetIndexCount()
{
	return m_model->GetIndexCount();
}