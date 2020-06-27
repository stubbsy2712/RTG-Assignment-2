#pragma once
#include <DirectXMath.h>
#include "firemodelclass.h"
#include "modelclass.h"
#include "Collidable.h"

using namespace DirectX;
using namespace std;

class Bat : public Collidable
{
private:
	float GOAL_APPROXIMATION = 1.0f;
	float ANGLE_APPROXIMATION = 0.1f;//In radians, how close is close enough to facing the correct directi
	bool inZone;
	int zonesLeft;

	FireModelClass* model;
	XMFLOAT3* currentDestination;
	float* playerX;
	float* playerY;
	float* playerZ;
	CuboidZone* flyZone;
	CuboidZone goalBounds;

	float speed = 50;//units moved per second, bigger is faster.
	float turnRate = 0.75f;//time in seconds to turn 180 degrees, or PI radians, bigger is slower.
	float m_heightToClimb = 75;

	void softRotate(float turnPower, float needToFaceX, float needToFaceY);
	void findNewDestination();
	void setToChasePlayer();
	virtual void collisionOccured(ZoneClass* zone);//With player

public:
	void move(float time);
	void shutdown();
	void checkColliisonAABB(BoundingBox* otherBox, ZoneClass* zone);
	void InitializeWithModel(FireModelClass*, ZoneClass*, CuboidZone*);
	void checkColliisonWithBullet(XMFLOAT3*, ZoneClass*);
	FireModelClass* getModel();
	Bat();
	~Bat();
};
