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
	bool RenderToTexture(D3DClass*, ShaderManagerClass*, float, TextureManagerClass*);
	bool isUnderground(XMFLOAT3*);
	bool isUnderground(BoundingBox*);
	
	int m_health = 3;
	int m_score = 0;

	int m_ammoCap = 10;
	int m_ammo = 5;
	float m_timeBetweenAmmoDrops = 7.5f;
	float m_timeUntilNextAmmoDrop = m_timeBetweenAmmoDrops;
	float m_shotPower = 75;

	float m_shotCooldown = 0;
	float m_timeBetweenShots = 1;

	int m_maxBats = 5;
	int m_batsDifficulty = 2;//Higher = easier, this is the amount of random points the bats must hit before they can hunt the player.
	float m_timeToSpawnBat = 10;
	float m_timeBetweenSpawns = 10;
	int m_lastSpawnerUsed = 0;

	bool m_zooming;
	float m_zoomingAdder = 10;
	float m_timeToZoom = 3;
	float m_zoomState;

	float m_angleTolerance = 52.5f;
	bool m_firstFrame = true;

	double m_totalTime = 0;
	float m_distortionTimeMultiplier = 20;

	float m_cannonScale = 0.1f;
	float m_batScale = 0.1f;
	float m_bulletScale = 0.1f;
	float m_objectiveScale = 0.2f;
	float m_objectiveFloat = 2;

	float m_timeOfDay = 0;
	float m_dayCycleTime = 48;
	bool m_timeGoingUp = true;

	XMFLOAT3 m_batSize = {3,1,3};
	XMFLOAT3 m_cannonSize = {3,5,5};
	XMFLOAT3 m_cannonSizePerp = {5,5,3};
	XMFLOAT3 m_windowSize = {1,1,1};
	XMFLOAT3 m_objectiveSize = {3,3,3};

	XMFLOAT4 m_apexDayColour = { 0.0f, 0.05f, 0.6f, 1.0f };
	XMFLOAT4 m_centreDayColour = { 0.0f, 0.5f, 0.8f, 1.0f };

	XMFLOAT4 m_apexNightColour = {0.0f, 0.001f, 0.005f, 0.5f};
	XMFLOAT4 m_centreNightColour = {0.0f, 0.02f, 0.3f, 0.5f};

	XMFLOAT4 m_dayLightColour = { 1, 1, 1, 1 };
	XMFLOAT4 m_nightLightColour = {0.05f, 0.05f, 0.1f, 1};

	void zoom();
	void unZoom();
	void adjustZoom(float);
	void fire(ID3D11Device*, ID3D11DeviceContext*);
	void checkAllCollisions();
	void spawnObjectives(int);
	void incrementScore(int);
	void tickBatSpawners(D3DClass*, float);
	void spawnNewObjective();
	void tickFiring(float);
	void checkObjectiveWave();
	void deleteThings();
	void tickTimeOfDay(float);
	bool spawnBats(int);
	bool outOfBounds(XMFLOAT3*);
	float getAngleMovement();

private:

	list<Bat*> m_enemies;
	list<Collidable*> m_collidables;
	list<ObjectiveClass*> m_objectives;
	list<BulletClass*> m_projectiles;
	list<BatSpawner*> m_spawners;

	list<BulletClass*> m_bulletsToDelete;
	list<Bat*> m_enemiesToDelete;
	list<ObjectiveClass*> m_objectivesToDelete;

	XMFLOAT3* m_lastFramePosition;

	//FireModelClass* m_CentreSkull;
	WindowModelClass* m_WindowPane;
	//BubbleModelClass* m_BubbleSkull;
	
	SoundClass* m_backgroundMusic;
	SoundClass* m_footPrintAudio;
	SoundClass* m_damageAudio;
	SoundClass* m_batKilledAudio;
	SoundClass* m_shootingAudio;
	SoundClass* m_objectiveCompleteAudio;
	
	RenderTextureClass* m_RenderTexture;

	BatSpawner* m_northBatSpawner;
	BatSpawner* m_southBatSpawner;
	BatSpawner* m_eastBatSpawner;
	BatSpawner* m_westBatSpawner;
	XMFLOAT3* m_northSpawnerLocation = new XMFLOAT3(500, 58.8f, 885);
	XMFLOAT3* m_southSpawnerLocation = new XMFLOAT3(626, 27, 80);
	XMFLOAT3* m_eastSpawnerLocation = new XMFLOAT3(40, 21, 500);
	XMFLOAT3* m_westSpawnerLocation = new XMFLOAT3(999, 19, 498);
	CuboidZone* m_batZone;

	WindowModelClass* m_glassCannonModel;
	FireModelClass* m_fireBatModel;
	ModelClass* m_bulletModel;
	ModelClass* m_objectiveModel;
	TextureClass* m_reflectionTexture;

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