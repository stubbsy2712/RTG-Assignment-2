#pragma once
#include <DirectXMath.h>
#include <list>
using namespace DirectX;
using namespace std;

class ZoneClass;
//Struct for bounding box
struct BoundingBox
{
	float xMax;
	float yMax;
	float zMax;
	float xMin;
	float yMin;
	float zMin;
	bool isInZone(XMFLOAT3* position)
	{
		//This method uses a left hand system.
		bool xCorrect = (position->x > xMin&& position->x < xMax);
		bool yCorrect = (position->y > yMin&& position->y < yMax);
		bool zCorrect = (position->z > zMin&& position->z < zMax);
		return (xCorrect && yCorrect && zCorrect);
	}
	list<XMFLOAT3> verts()
	{
		list<XMFLOAT3> verts;
		verts.push_back(XMFLOAT3(xMax, yMax, zMax));
		verts.push_back(XMFLOAT3(xMax, yMax, zMin));
		verts.push_back(XMFLOAT3(xMax, yMin, zMax));
		verts.push_back(XMFLOAT3(xMin, yMax, zMax));
		verts.push_back(XMFLOAT3(xMax, yMin, zMin));
		verts.push_back(XMFLOAT3(xMin, yMax, zMin));
		verts.push_back(XMFLOAT3(xMin, yMin, zMax));
		verts.push_back(XMFLOAT3(xMin, yMin, zMin));
		return verts;
	}
};

class Collidable
{
public:
	virtual void checkCollisionPoint(XMFLOAT3*, ZoneClass*);
	virtual void checkColliisonAABB(BoundingBox*, ZoneClass*);
	virtual bool checkCollisionWithBullet(XMFLOAT3*, ZoneClass*);
	virtual void forceCollision(ZoneClass*);
	virtual void pushToPreviousPosition();
	virtual void updatePrevPosition();
	
	void calculateBoundingBox();
	void setPosition(float, float, float);
	void setRotation(float, float, float);
	BoundingBox* getBoundingBox();

	float m_xSize;
	float m_ySize;
	float m_zSize;

	XMFLOAT3* getPosition();
	XMFLOAT3* getRotation();
protected:
	BoundingBox* m_boundingBox = new BoundingBox();
	XMFLOAT3* m_previousPosition = new XMFLOAT3();
	virtual void collisionOccured(ZoneClass*);
	XMFLOAT3* m_position = new XMFLOAT3(0, 0, 0);
	XMFLOAT3* m_rotation = new XMFLOAT3(0, 0, 0);//Stores in terms of radians.
};

