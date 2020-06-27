#include "Collidable.h"
#include "zoneclass.h"

void Collidable::checkCollisionPoint(XMFLOAT3* playerPosition, ZoneClass* zone)
{
	if (m_boundingBox->isInZone(playerPosition))
		collisionOccured(zone);
}

void Collidable::checkColliisonAABB(BoundingBox* otherBox, ZoneClass* zone)
{
	for each (XMFLOAT3 vert in otherBox->verts())
	{
		if (m_boundingBox->isInZone(&vert))
			collisionOccured(zone);
	}
}

bool Collidable::checkCollisionWithBullet(XMFLOAT3* bulletLocation, ZoneClass*)
{
	if (m_boundingBox->isInZone(bulletLocation))
		return true;
	return false;
}

void Collidable::forceCollision(ZoneClass* zone)
{
	collisionOccured(zone);
}

void Collidable::updatePrevPosition()
{
	m_previousPosition->x = m_position->x;
	m_previousPosition->y = m_position->y;
	m_previousPosition->z = m_position->z;
}

void Collidable::calculateBoundingBox()
{
	m_boundingBox->xMax = m_position->x + m_xSize / 2;
	m_boundingBox->xMin = m_position->x - m_xSize / 2;
	m_boundingBox->yMax = m_position->y + m_xSize / 2;
	m_boundingBox->yMin = m_position->y - m_xSize / 2;
	m_boundingBox->zMax = m_position->z + m_xSize / 2;
	m_boundingBox->zMin = m_position->z - m_xSize / 2;
}

void Collidable::setPosition(float newX, float newY, float newZ)
{
	//delete m_position;
	m_position = new XMFLOAT3(newX, newY, newZ);
}

void Collidable::setRotation(float newX, float newY, float newZ)
{
	//delete m_rotation;
	m_rotation = new XMFLOAT3(newX, newY, newZ);
}

BoundingBox* Collidable::getBoundingBox()
{
	return m_boundingBox;
}

XMFLOAT3* Collidable::getPosition()
{
	return m_position;
}

XMFLOAT3* Collidable::getRotation()
{
	return m_rotation;
}

void Collidable::collisionOccured(ZoneClass* zone)
{
	zone->pushBackPlayer();
}

void Collidable::pushToPreviousPosition()
{
	m_position->x = m_previousPosition->x;
	m_position->y = m_previousPosition->y;
	m_position->z = m_previousPosition->z;
}
