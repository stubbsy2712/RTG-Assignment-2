#include "Bat.h"
#include "zoneclass.h"
#include <random>

void Bat::shutdown()
{
	if (model)
	{
		model->Shutdown();
		delete model;
		model = 0;
	}
	if (flyZone)
	{
		model->Shutdown();
		delete model;
		model = 0;
	}
	if (currentDestination)
	{
		delete currentDestination;
		currentDestination = 0;
	}
}

FireModelClass* Bat::getModel()
{
	return model;
}

void Bat::InitializeWithModel(FireModelClass* model, ZoneClass* zone, CuboidZone* flyBounds)
{
	this->model = model;
	this->flyZone = flyBounds;
	playerX = zone->getPlayerPositionXPtr();
	playerY = zone->getPlayerPositionYPtr();
	playerZ = zone->getPlayerPositionZPtr();
	zonesLeft = zone->getEnemyDifficulty();
	findNewDestination();
}

void Bat::checkColliisonWithBullet(XMFLOAT3* bulletPosition, ZoneClass* zone)
{
	if (m_boundingBox->isInZone(bulletPosition))
		zone->batHit(this);
}

Bat::Bat()
{
	model = new FireModelClass;
	currentDestination = new XMFLOAT3;
	m_rotation = new XMFLOAT3;//Stores in terms of radians.
	//PlayerPosition = new XMFLOAT3;
	playerX = 0;
	playerY = 0;
	playerZ = 0;
	flyZone = new CuboidZone;
}

Bat::~Bat()
{
	model->Shutdown();
	delete model;
	delete currentDestination;
	//delete playerPosition;
	delete flyZone;
}

void Bat::softRotate(float turnPower, float needToFaceX, float needToFaceY)
{
	bool currentUL = (m_rotation->y > XM_PIDIV2);
	bool currentUR = (m_rotation->y > 0 && m_rotation->y < XM_PIDIV2);
	bool currentLL = (m_rotation->y < -XM_PIDIV2);
	bool currentLR = (m_rotation->y < 0 && m_rotation->y > -XM_PIDIV2);
	bool needUL = (needToFaceY > XM_PIDIV2);
	bool needUR = (needToFaceY > 0 && needToFaceY < XM_PIDIV2);
	bool needLL = (needToFaceY < -XM_PIDIV2);
	bool needLR = (needToFaceY < 0 && needToFaceY > -XM_PIDIV2);

	bool bothLeft = ((currentLL || currentUL) && (needLL || needLR));
	bool bothRight = ((currentLR || currentUR) && (needLR || needUR));
	bool bothUpper = ((currentUL || currentUR) && (needUL || needUR));
	bool bothLower = ((currentLL || currentLR) && (needLL || needLR));
	bool sameLeft = ((currentLL && needLL) || (currentUL && needUL));
	bool firstFrame = (!(currentLL || currentUL || currentLR || currentUR));

	bool simpleRotation = (bothRight || bothUpper || bothLower || sameLeft || firstFrame);
	//figure out rotations for opposite corners, and do some other magic for both left.

	if (simpleRotation)
	{
		if (needToFaceY > m_rotation->y)
			m_rotation->y += turnPower;
		else if (needToFaceY < m_rotation->y)
			m_rotation->y -= turnPower;
	}

	else if (bothLeft)//If they're both in left quadrants, but not the same quadrant.
	{
		if (needToFaceY > m_rotation->y)
			m_rotation->y -= turnPower;//clockwise.
		else if (needToFaceY < m_rotation->y)
			m_rotation->y += turnPower;//anti-clockwise.
	}

	else
	{//Opposite quadrants
		if (currentLL || currentLR)//current lower
		{
			float oppositeAngle = m_rotation->y + XM_PI;
			if (needToFaceY < oppositeAngle)
				m_rotation->y -= turnPower;//Turn clockwise
			else
				m_rotation->y += turnPower;//Anti-clockwise
		}
		else if (currentUL || currentUR)
		{
			float oppositeAngle = -(XM_PI - m_rotation->y);
			if (needToFaceY < oppositeAngle)
				m_rotation->y += turnPower;//Anti-clockwise
			else
				m_rotation->y -= turnPower;//Turn clockwise
		}
		else
		{
			m_rotation->y += turnPower;
		}
	}

	//Handle Rotation on the other axis
	if (m_rotation->x > needToFaceX)
		m_rotation->x -= turnPower;
	if (m_rotation->x < needToFaceX)
		m_rotation->x += turnPower;

	if (m_rotation->x - needToFaceX < ANGLE_APPROXIMATION)
		m_rotation->x = needToFaceX;
	if (-(m_rotation->x - needToFaceX) > -ANGLE_APPROXIMATION)
		m_rotation->x = needToFaceX;

	if (m_rotation->x > XM_PI)
		m_rotation->x = -(XM_PI - (m_rotation->x - XM_PI));
	if (m_rotation->x < -XM_PI)
		m_rotation->x += XM_2PI;
	if (m_rotation->y < -XM_PI)
		m_rotation->y += XM_2PI;
	if (m_rotation->y > XM_PI)
		m_rotation->y = -(XM_PI - (m_rotation->y - XM_PI));

}

void Bat::findNewDestination()
{
	if (zonesLeft < 1)
	{
		//currentDestination = playerPosition;
	}
	//Select random coordinates that are inside the zone of acceptable coordinates.
	random_device rd1;
	default_random_engine generator1(rd1());
	uniform_real_distribution<float> distributionX(flyZone->leftBound, flyZone->rightBound);
	currentDestination->x = distributionX(generator1);
	random_device rd2;
	default_random_engine generator2(rd2());
	uniform_real_distribution<float> distributionY(flyZone->lowerBound, flyZone->upperBound);
	currentDestination->y = distributionY(generator2);
	random_device rd3;
	default_random_engine generator3(rd3());
	uniform_real_distribution<float> distributionZ(flyZone->frontBound, flyZone->backBound);
	currentDestination->z = distributionZ(generator3);
	//Set the goalBounds so that the model does not have to be on the exact point.
	goalBounds.upperBound = currentDestination->y + GOAL_APPROXIMATION;
	goalBounds.lowerBound = currentDestination->y - GOAL_APPROXIMATION;
	goalBounds.leftBound = currentDestination->x - GOAL_APPROXIMATION;
	goalBounds.rightBound = currentDestination->x + GOAL_APPROXIMATION;
	goalBounds.backBound = currentDestination->z + GOAL_APPROXIMATION;
	goalBounds.frontBound = currentDestination->z - GOAL_APPROXIMATION;
	zonesLeft -= 1;
}

void Bat::move(float time)
{
	//Handle rotation.
	//work out where it needs to face
	//Only allow them to move 1 axis at a time.
	float diffX = currentDestination->x - m_position->x;
	float diffY = currentDestination->y - m_position->y;
	float diffZ = currentDestination->z - m_position->z;
	float diffXZ = sqrt(pow(diffX, 2) + pow(diffZ, 2));//Pythagorous.

	float needToFaceY = atanf(diffZ / diffX);
	if (diffZ < 0 && diffX < 0)
		needToFaceY -= XM_PI;
	else if (diffX < 0)
		needToFaceY += XM_PI;

	float needToFaceX = atan(diffY / diffXZ);
	float turnPower = (time / turnRate) * XM_PI;
	
	//softRotate(turnPower, needToFaceX, needToFaceY);

	m_rotation->x = needToFaceX;
	m_rotation->y = needToFaceY;

	if (time > 4)//Didn't want to do this, had no choice
		time = 4;
	float moveBy = time * speed;

	float radians = m_rotation->y;
	float radiansX = m_rotation->x;

	float moveX = cosf(radians) * cosf(radiansX) * moveBy;
	float moveZ = sinf(radians) * cosf(radiansX) * moveBy;

	m_position->x += moveX;
	m_position->z += moveZ;

	m_position->y += sinf(radiansX) * moveBy;
	//Would like to use trig to limit this, so that they move at a constant speed,
	//currently they will essentially have a larger speed if they need to move on the y axis.

	//Check if they have come close enough to reaching the destination.
	if (goalBounds.isInZone(m_position))
	{
		moveBy++;
		findNewDestination();
	}
}

void Bat::collisionOccured(ZoneClass* zone)
{
	zone->playerHit(this);
}