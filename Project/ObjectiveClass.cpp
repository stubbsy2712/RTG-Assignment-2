#include "ObjectiveClass.h"
#include "zoneclass.h"

void ObjectiveClass::initializeWithModel(ModelClass* model, XMFLOAT3* position)
{
	m_model = model;
	m_position = position;
}

ModelClass* ObjectiveClass::getModel()
{
	return m_model;
}

ObjectiveClass::ObjectiveClass()
{
}

void ObjectiveClass::Shutdown()
{
}

ObjectiveClass::~ObjectiveClass()
{
}

void ObjectiveClass::collisionOccured(ZoneClass* zone)
{
	zone->objectiveCollected(this);
}