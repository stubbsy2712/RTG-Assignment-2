////////////////////////////////////////////////////////////////////////////////
// Filename: zoneclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _ZONECLASS_H_
#define _ZONECLASS_H_
//This class serves much the same purpose as GraphicsClass in other projects.

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "d3dclass.h"
#include "inputclass.h"
#include "shadermanagerclass.h"
#include "texturemanagerclass.h"
#include "timerclass.h"
#include "userinterfaceclass.h"
#include "cameraclass.h"
#include "lightclass.h"
#include "positionclass.h"
#include "frustumclass.h"
#include "skydomeclass.h"
#include "terrainclass.h"
#include "firemodelclass.h"
#include "WindowModelClass.h"
#include "rendertextureclass.h"
#include "modelclass.h"
#include "Bat.h"
#include "BatSpawner.h"
#include "Soundclass.h"
#include "ObjectiveClass.h"
#include "BulletClass.h"
#include <list>
////////////////////////////////////////////////////////////////////////////////
// Class name: ZoneClass
////////////////////////////////////////////////////////////////////////////////

class ZoneClass
{
public:
	ZoneClass();
	ZoneClass(const ZoneClass&);
	~ZoneClass();

	bool Initialize(D3DClass*, HWND, int, int, float);
	void Shutdown();
	bool Frame(D3DClass*, InputClass*, ShaderManagerClass*, TextureManagerClass*, float, int);

	void batHit(Bat*);
	void playerHit(Bat*);
	void pushBackPlayer();
	void objectiveCollected(ObjectiveClass*);
	float* getPlayerPositionXPtr();
	float* getPlayerPositionYPtr();
	float* getPlayerPositionZPtr();
	int getEnemyDifficulty();

private:
	void HandleMovementInput(InputClass*, float, D3DClass*);
	bool Render(D3DClass*, ShaderManagerClass*, TextureManagerClass*);
	//bool RenderToTexture(float rotation);
	bool isUnderground(XMFLOAT3*);
	
	int m_health = 3;
	int m_score = 0;

	int m_ammoCap = 10;
	int m_ammo = 5;
	float m_timeBetweenAmmoDrops = 5;
	float m_timeUntilNextAmmoDrop = m_timeBetweenAmmoDrops;
	float m_shotPower = 5;
	float m_timeSinceLastShot = 0;
	float m_timeBetweenShots = 3;
	float m_bulletSizeCorrection = 0.1f;

	int m_maxBats = 5;
	int m_batsDifficulty = 5;//Higher = easier, this is the amount of random points the bats must hit before they can hunt the player.
	float m_timeToSpawnBat = 10;
	float m_timeBetweenSpawns = 10;
	int m_lastSpawnerUsed = 0;

	bool m_zooming;
	float m_zoomingAdder = 10;
	float m_timeToZoom = 3;
	float m_zoomState;

	float m_angleTolerance = 15;

	double m_totalTime = 0;
	float m_distortionTimeMultiplier = 20;

	void zoom();
	void unZoom();
	void adjustZoom(float);
	void fire(ID3D11Device*, ID3D11DeviceContext*);
	void checkAllCollisions();
	void spawnObjectives(int);
	void incrementScore(int);
	void tickBatSpawners(D3DClass*, float);
	void spawnNewObjectives(int);
	void tickFiring(float);
	void checkObjectiveWave();
	bool spawnBats(int);
	float getAngleMovement();

private:

	list<Bat*> m_enemies;
	list<Collidable*> m_collidables;
	list<ObjectiveClass*> m_objectives;
	list<BulletClass*> m_projectiles;
	list<BatSpawner*> m_spawners;

	XMFLOAT3* m_lastFramePosition;

	FireModelClass* m_CentreSkull;
	WindowModelClass* m_WindowPane;
	BubbleModelClass* m_BubbleSkull;
	
	SoundClass* m_backgroundMusic;
	SoundClass* m_footPrintAudio;
	SoundClass* m_damageAudio;
	SoundClass* m_batKilledAudio;
	SoundClass* m_shootingAudio;
	
	RenderTextureClass* m_RenderTexture;

	BatSpawner* m_northBatSpawner;
	BatSpawner* m_southBatSpawner;
	BatSpawner* m_eastBatSpawner;
	BatSpawner* m_westBatSpawner;
	XMFLOAT3* m_northSpawnerLocation = new XMFLOAT3(250, 100, 0);
	XMFLOAT3* m_southSpawnerLocation = new XMFLOAT3(250, 100, 500);
	XMFLOAT3* m_eastSpawnerLocation = new XMFLOAT3(0, 100, 250);
	XMFLOAT3* m_westSpawnerLocation = new XMFLOAT3(500, 100, 250);
	CuboidZone* m_batZone;

	WindowModelClass* m_glassCannonModel;
	FireModelClass* m_fireBatModel;
	ModelClass* m_bulletModel;
	BubbleModelClass* m_objectiveModel;

	UserInterfaceClass* m_UserInterface;
	CameraClass* m_Camera;
	LightClass* m_Light;
	PositionClass* m_Position;
	FrustumClass* m_Frustum;
	SkyDomeClass* m_SkyDome;
	TerrainClass* m_Terrain;
	bool m_displayUI, m_wireFrame, m_cellLines, m_heightLocked;
};

#endif