////////////////////////////////////////////////////////////////////////////////
// Filename: zoneclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "zoneclass.h"


ZoneClass::ZoneClass()
{
	m_UserInterface = 0;
	m_Camera = 0;
	m_Light = 0;
	m_Position = 0;
	m_Frustum = 0;
	m_SkyDome = 0;
	m_Terrain = 0;
	
	//m_BubbleSkull = 0;
	//m_CentreSkull = 0;
	m_WindowPane = 0;
	
	m_RenderTexture = 0;
	
	m_objectiveCompleteAudio = 0;
	m_backgroundMusic = 0;
	m_footPrintAudio = 0;
	m_damageAudio = 0;
	m_batKilledAudio = 0;
	m_shootingAudio = 0;

	m_objectiveModel = 0;
	m_fireBatModel = 0;
	m_reflectionTexture = 0;

	m_lastFramePosition = 0;

	m_northBatSpawner = 0;
	m_southBatSpawner = 0;
	m_eastBatSpawner = 0;
	m_westBatSpawner = 0;
	m_batZone = 0;
}


ZoneClass::ZoneClass(const ZoneClass& other)
{
}


ZoneClass::~ZoneClass()
{
}


bool ZoneClass::Initialize(D3DClass* Direct3D, HWND hwnd, int screenWidth, int screenHeight, float screenDepth)
{
	bool result;
	m_batZone = new CuboidZone;
	m_batZone->leftBound = 256;
	m_batZone->rightBound = 768;
	m_batZone->backBound = 768;
	m_batZone->frontBound = 256;
	m_batZone->lowerBound = 75;
	m_batZone->upperBound = 100;

	m_bulletModel = new ModelClass;
	result = m_bulletModel->Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(),
		"../Project/data/models/Bullet.txt", "../Project/data/textures/BulletColour.dds", "../Project/data/textures/BulletColour.dds");
	if (!result)
	{
		return false;
	}

	m_glassCannonModel = new WindowModelClass;
	result = m_glassCannonModel->Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(),
		"../Project/data/models/NavalCannon.txt", "../Project/data/textures/glass01.dds", "../Project/data/textures/glass01.dds", "../Project/data/textures/glass01.dds");
	if (!result)
	{
		return false;
	}
	
	m_objectiveModel = new ModelClass;
	result = m_objectiveModel->Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(),
		"../Project/data/models/Sphere.txt", "../Project/data/textures/glass01.dds", "../Project/data/textures/glass01.dds");
	if (!result)
	{
		return false;
	}

	m_northBatSpawner = new BatSpawner;
	result = m_northBatSpawner->InitializeWithModel(m_glassCannonModel);
	if (!result)
	{
		return false;
	}
	m_northBatSpawner->setPosition(m_northSpawnerLocation);
	m_northBatSpawner->setRotation(0, XM_PI, 0);
	m_northBatSpawner->m_xSize = m_cannonSize.x;
	m_northBatSpawner->m_ySize = m_cannonSize.y;
	m_northBatSpawner->m_zSize = m_cannonSize.z;
	m_northBatSpawner->calculateBoundingBox();
	m_spawners.push_back(m_northBatSpawner);
	m_collidables.push_back(m_northBatSpawner);

	m_westBatSpawner = new BatSpawner;
	result = m_westBatSpawner->InitializeWithModel(m_glassCannonModel);
	if (!result)
	{
		return false;
	}
	m_westBatSpawner->setPosition(m_westSpawnerLocation);
	m_westBatSpawner->setRotation(0, XM_PIDIV2, 0);
	m_westBatSpawner->m_xSize = m_cannonSizePerp.x;
	m_westBatSpawner->m_ySize = m_cannonSizePerp.y;
	m_westBatSpawner->m_zSize = m_cannonSizePerp.z;
	m_westBatSpawner->calculateBoundingBox();
	m_spawners.push_back(m_westBatSpawner);
	m_collidables.push_back(m_westBatSpawner);

	m_eastBatSpawner = new BatSpawner;
	result = m_eastBatSpawner->InitializeWithModel(m_glassCannonModel);
	if (!result)
	{
		return false;
	}
	m_eastBatSpawner->setPosition(m_eastSpawnerLocation);
	m_eastBatSpawner->setRotation(0, XM_PIDIV2 * 3, 0);
	m_eastBatSpawner->m_xSize = m_cannonSizePerp.x;
	m_eastBatSpawner->m_ySize = m_cannonSizePerp.y;
	m_eastBatSpawner->m_zSize = m_cannonSizePerp.z;
	m_eastBatSpawner->calculateBoundingBox();
	m_spawners.push_back(m_eastBatSpawner);
	m_collidables.push_back(m_eastBatSpawner);

	m_southBatSpawner = new BatSpawner;
	result = m_southBatSpawner->InitializeWithModel(m_glassCannonModel);
	if (!result)
	{
		return false;
	}
	m_southBatSpawner->setPosition(m_southSpawnerLocation);
	m_southBatSpawner->setRotation(0, 0, 0);
	m_southBatSpawner->m_xSize = m_cannonSize.x;
	m_southBatSpawner->m_ySize = m_cannonSize.y;
	m_southBatSpawner->m_zSize = m_cannonSize.z;
	m_southBatSpawner->calculateBoundingBox();
	m_spawners.push_back(m_southBatSpawner);
	m_collidables.push_back(m_southBatSpawner);

	m_lastFramePosition = new XMFLOAT3;
	if (!m_lastFramePosition)
	{
		return false;
	}

	// Create the render to texture object.
	m_RenderTexture = new RenderTextureClass;
	if (!m_RenderTexture)
	{
		return false;
	}

	// Initialize the render to texture object.
	result = m_RenderTexture->Initialize(Direct3D->GetDevice(), screenWidth, screenHeight);
	if (!result)
	{
		return false;
	}

	// Create the user interface object.
	m_UserInterface = new UserInterfaceClass;
	if(!m_UserInterface)
	{
		return false;
	}

	// Initialize the user interface object.
	result = m_UserInterface->Initialize(Direct3D, screenHeight, screenWidth);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the user interface object.", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if(!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera and build the matrices needed for rendering.
	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);
	m_Camera->Render();
	m_Camera->RenderBaseViewMatrix();

	// Create the light object.
	m_Light = new LightClass;
	if(!m_Light)
	{
		return false;
	}

	// Initialize the light object.
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(-0.5f, -1.0f, -0.5f);
	m_Light->SetAmbientColor(0.2f, 0.2f, 0.2f, 1.0f);
	m_Light->SetPosition(500.0f, 100.0f, 500.0f);

	// Create the position object.
	m_Position = new PositionClass;
	if(!m_Position)
	{
		return false;
	}

	// Set the initial position and rotation.
	m_Position->SetPosition(512.5f, 10.0f, 10.0f);
	m_Position->SetRotation(0.0f, 0.0f, 0.0f);

	// Create the frustum object.
	m_Frustum = new FrustumClass;
	if(!m_Frustum)
	{
		return false;
	}

	// Initialize the frustum object.
	m_Frustum->Initialize(screenDepth);

	// Initialize the flaming bat model object.
	m_fireBatModel = new FireModelClass;
	if (!m_fireBatModel)
	{
		return false;
	}

	result = m_fireBatModel->Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), "../Project/data/models/bat.txt",
		"../Project/data/textures/fire01.dds", "../Project/data/textures/noise01.dds", "../Project/data/textures/alpha01.dds");

	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the flaming bat object.", L"Error", MB_OK);
		return false;
	}

	// Create the centre skull object.
	//m_CentreSkull = new FireModelClass;
	//if (!m_CentreSkull)
	//{
	//	return false;
	//}
	//m_CentreSkull->position = XMFLOAT3(800, 10, 600);

	// Initialize the flaming head object.

	//result = m_CentreSkull->Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), "../Project/data/models/new-ninjaHead.txt",
	//	"../Project/data/textures/fire01.dds", "../Project/data/textures/noise01.dds", "../Project/data/textures/alpha01.dds");

	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the flame skull object.", L"Error", MB_OK);
		return false;
	}

	m_reflectionTexture = new TextureClass;
	if (!m_reflectionTexture)
	{
		return false;
	}
	result = m_reflectionTexture->Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), "../Project/data/textures/BreithornPanoramaResizedAndStretched.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the reflection texture.", L"Error", MB_OK);
		return false;
	}

	//// Create the centre bubble skull object.
	//m_BubbleSkull = new BubbleModelClass;
	//if (!m_BubbleSkull)
	//{
	//	return false;
	//}
	//m_BubbleSkull->position = XMFLOAT3(800, 10, 600);

	//// Initialize the Bubble Skull object.
	//result = m_BubbleSkull->Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), "../Project/data/models/new-ninjaHead.txt",
	//	"../Project/data/textures/rainbowfilm_smooth.dds", "../Project/data/textures/room-dxt5.dds");

	//if (!result)
	//{
	//	MessageBox(hwnd, L"Could not initialize the bubble skull object.", L"Error", MB_OK);
	//	return false;
	//}

	// Create the sky dome object.
	m_SkyDome = new SkyDomeClass;
	if(!m_SkyDome)
	{
		return false;
	}

	// Initialize the sky dome object.
	result = m_SkyDome->Initialize(Direct3D->GetDevice());
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the sky dome object.", L"Error", MB_OK);
		return false;
	}

	// Create the terrain object.f
	m_Terrain = new TerrainClass;
	if(!m_Terrain)
	{
		return false;
	}

	// Initialize the terrain object.
	result = m_Terrain->Initialize(hwnd, Direct3D->GetDevice(), "../Project/data/setup.txt");
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the terrain object.", L"Error", MB_OK);
		return false;
	}

	// Create the window model object.
	
	m_WindowPane = new WindowModelClass;
	if (!m_WindowPane)
	{
		return false;
	}
	result = m_WindowPane->Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), "../Project/data/models/cube.txt",
		"../Project/data/textures/glass01.dds", "../Project/data/textures/bump03.dds", "../Project/data/textures/bump03.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the glass object.", L"Error", MB_OK);
		return false;
	}
	m_WindowPane->m_xSize = m_windowSize.x;
	m_WindowPane->m_ySize = m_windowSize.y;
	m_WindowPane->m_zSize = m_windowSize.z;
	m_WindowPane->calculateBoundingBox();
	m_collidables.push_back(m_WindowPane);
	m_WindowPane->setPosition(250, 5, 10);

	m_backgroundMusic = new SoundClass;
	result = m_backgroundMusic->Initialize(hwnd, "../Project/data/sounds/DoNotChillWavVersion.wav", true);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the background music object.", L"Error", MB_OK);
		return false;//lasts 175
	}
	m_backgroundMusic->repeating = true;
	m_backgroundMusic->playFor(175);

	m_footPrintAudio = new SoundClass;
	result = m_footPrintAudio->Initialize(hwnd, "../Project/data/sounds/Snowy_Footstep_2.wav", false);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the footprint sound object.", L"Error", MB_OK);
		return false;
	}
	m_Position->SetFootstepSound(m_footPrintAudio);

	m_shootingAudio = new SoundClass;
	result = m_shootingAudio->Initialize(hwnd, "../Project/data/sounds/Shooting_Sound_Effect.wav", false);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the firing sound object.", L"Error", MB_OK);
		return false;
	}

	m_batKilledAudio = new SoundClass;
	result = m_batKilledAudio->Initialize(hwnd, "../Project/data/sounds/Bat_Sound_Effect.wav", false);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bat dying sound object.", L"Error", MB_OK);
		return false;
	}

	m_damageAudio = new SoundClass;
	result = m_damageAudio->Initialize(hwnd, "../Project/data/sounds/Minecraft_Oof.wav", false);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bat dying sound object.", L"Error", MB_OK);
		return false;
	}

	m_objectiveCompleteAudio = new SoundClass;
	result = m_objectiveCompleteAudio->Initialize(hwnd, "../Project/data/sounds/Sparkle_Sound_Effect.wav", false);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the objective completion sound object.", L"Error", MB_OK);
		return false;
	}

	// Set the UI to display by default.
	m_displayUI = true;

	// Set wire frame rendering initially to disabled.
	m_wireFrame = false;

	// Set the rendering of cell lines initially to disabled.
	m_cellLines = false;

	// Set the user locked to the terrain height for movement.
	m_heightLocked = true;

	return true;
}


void ZoneClass::Shutdown()
{
	//TODO: Release all the new objects.
	// Release the terrain object.
	if(m_Terrain)
	{
		m_Terrain->Shutdown();
		delete m_Terrain;
		m_Terrain = 0;
	}

	//Release both sounds
	if (m_footPrintAudio)
	{
		m_footPrintAudio->Shutdown();
		delete m_footPrintAudio;
		m_footPrintAudio = 0;
	}
	if (m_backgroundMusic)
	{
		m_backgroundMusic->Shutdown();
		delete m_backgroundMusic;
		m_backgroundMusic = 0;
	}

	// Release the sky dome object.
	if(m_SkyDome)
	{
		m_SkyDome->Shutdown();
		delete m_SkyDome;
		m_SkyDome = 0;
	}

	// Release the frustum object.
	if(m_Frustum)
	{
		delete m_Frustum;
		m_Frustum = 0;
	}

	// Release the position object.
	if(m_Position)
	{
		delete m_Position;
		m_Position = 0;
	}

	// Release the light object.
	if(m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the camera object.
	if(m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the user interface object.
	if(m_UserInterface)
	{
		m_UserInterface->Shutdown();
		delete m_UserInterface;
		m_UserInterface = 0;
	}

	//if (m_CentreSkull)
	//{
	//	m_CentreSkull->Shutdown();
	//	delete m_CentreSkull;
	//	m_CentreSkull = 0;
	//}

	//if (m_BubbleSkull)
	//{
	//	m_BubbleSkull->Shutdown();
	//	delete m_BubbleSkull;
	//	m_BubbleSkull = 0;
	//}

	if (m_WindowPane)
	{
		m_WindowPane->Shutdown();
		delete m_WindowPane;
		m_WindowPane = 0;
	}
	// Release the render to texture object.
	if (m_RenderTexture)
	{
		m_RenderTexture->Shutdown();
		delete m_RenderTexture;
		m_RenderTexture = 0;
	}
	if (m_lastFramePosition)
	{
		delete m_lastFramePosition;
		m_lastFramePosition = 0;
	}
	return;
}

bool ZoneClass::RenderToTexture(D3DClass* Direct3D, ShaderManagerClass* ShaderManager, float rotation, TextureManagerClass* TextureManager)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;

	// Set the render target to be the render to texture.
	m_RenderTexture->SetRenderTarget(Direct3D->GetDeviceContext(), Direct3D->GetDepthStencilView());

	// Clear the render to texture.
	
	m_RenderTexture->ClearRenderTarget(Direct3D->GetDeviceContext(), Direct3D->GetDepthStencilView(), 0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	Direct3D->GetProjectionMatrix(projectionMatrix);

	// Multiply the world matrix by the rotation.

	XMMatrixRotationY(rotation);
	//D3DXMatrixRotationY(&worldMatrix, rotation);

	// Put everything on the graphics pipeline to prepare them for drawing.
	for (int i = 0; i < m_Terrain->GetCellCount(); i++)
	{
		// Render each terrain cell if it is visible only.
		result = m_Terrain->RenderCell(Direct3D->GetDeviceContext(), i, m_Frustum);
		if (result)
		{
			// Render the cell buffers using the hgih quality terrain shader.
			result = ShaderManager->RenderTerrainShader(Direct3D->GetDeviceContext(), m_Terrain->GetCellIndexCount(i), worldMatrix, viewMatrix,
				projectionMatrix, TextureManager->GetTexture(0), TextureManager->GetTexture(1),
				TextureManager->GetTexture(2), TextureManager->GetTexture(3),
				m_Light->GetDirection(), m_Light->GetDiffuseColor());
			if (!result)
			{
				return false;
			}

			// If needed then render the bounding box around this terrain cell using the color shader. 
			if (m_cellLines)
			{
				m_Terrain->RenderCellLines(Direct3D->GetDeviceContext(), i);
				ShaderManager->RenderColorShader(Direct3D->GetDeviceContext(), m_Terrain->GetCellLinesIndexCount(i), worldMatrix,
					viewMatrix, projectionMatrix);
				if (!result)
				{
					return false;
				}
			}
		}
	}

	// Render the sky dome using the sky dome shader.
	m_SkyDome->Render(Direct3D->GetDeviceContext());
	result = ShaderManager->RenderSkyDomeShader(Direct3D->GetDeviceContext(), m_SkyDome->GetIndexCount(), worldMatrix, viewMatrix,
		projectionMatrix, m_SkyDome->GetApexColor(), m_SkyDome->GetCenterColor());
	if (!result)
	{
		return false;
	}

	// Render the cube model using the texture shader.
	//result = m_TextureShader->Render(Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTexture());
	//if (!result)
	//{
	//	return false;
	//}

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	Direct3D->SetBackBufferRenderTarget();

	return true;
}

bool ZoneClass::Frame(D3DClass* Direct3D, InputClass* Input, ShaderManagerClass* ShaderManager, TextureManagerClass* TextureManager, 
					  float frameTime, int fps)
{
	if (m_health < 1)
		return false;

	bool result, foundHeight;
	float posX, posY, posZ, rotX, rotY, rotZ, height;

	// Do the frame input processing.
	HandleMovementInput(Input, frameTime, Direct3D);


	// Get the view point position/rotation.
	m_Position->GetPosition(posX, posY, posZ);
	m_Position->GetRotation(rotX, rotY, rotZ);
	XMFLOAT3 positionAsVec = { posX, posY, posZ };

	//Tick each of the SoundClass*
	m_backgroundMusic->tick(frameTime);
	m_footPrintAudio->tick(frameTime);
	m_damageAudio->tick(frameTime);
	m_batKilledAudio->tick(frameTime);
	m_shootingAudio->tick(frameTime);
	m_objectiveCompleteAudio->tick(frameTime);
	m_totalTime += frameTime;
	deleteThings();
	tickTimeOfDay(frameTime);

	for each (Bat* b in m_enemies)
	{
		b->move(frameTime);
		b->calculateBoundingBox();

		//Check collision with player.
		b->checkCollisionPoint(&positionAsVec, this);

		//Check collision with the ground.
		if (isUnderground(b->getBoundingBox()))
			b->pushToPreviousPosition();

		//Check collision with other bats
		for each (Bat* other in m_enemies)
		{
			if (b != other)
			{
				b->checkColliisonAABB(other->getBoundingBox(), this);
			}
		}

		//Check collisions with the bullets.
		for each (BulletClass* bc in m_projectiles)
		{
			b->checkColliisonWithBullet(bc->getPosition(), this);
		}

		//Now it's previous position can be updated.
		b->updatePrevPosition();
	}

	//Check collisions between objects and the floor
	for each (Collidable* i in m_collidables)
	{
		i->checkCollisionPoint(&positionAsVec, this);
		for each (Collidable * j in m_collidables)
		{
			i->checkColliisonAABB(j->getBoundingBox(), this);
		}
	}
	//Check colliisons for the bullets.
	for each (BulletClass * b in m_projectiles)
	{
		b->move(frameTime);

		for each (Collidable * j in m_collidables)
		{
			if (j->checkCollisionWithBullet(b->getPosition(), this))
				m_bulletsToDelete.push_back(b);
		}
		if (b)
		{
			if (isUnderground(b->getPosition()))
			{
				m_bulletsToDelete.push_back(b);
			}
		}
	}
	//Check colliisons for the objectives
	for each (ObjectiveClass* o in m_objectives)
	{
		o->calculateBoundingBox();
		o->checkCollisionPoint(&positionAsVec, this);
	}
	tickFiring(frameTime);
	//TODO: Set up previous frame's position.
	//TODO: Tick the bat spawners
	tickBatSpawners(Direct3D, frameTime);

	// Do the frame processing for the user interface.
	result = m_UserInterface->Frame(Direct3D->GetDeviceContext(), fps, posX, posY, posZ, rotX, rotY, rotZ);
	if(!result)
	{
		return false;
	}

	// Do the terrain frame processing.
	m_Terrain->Frame();

	// If the height is locked to the terrain then position the camera on top of it.
	if(m_heightLocked)
	{
		// Get the height of the triangle that is directly underneath the given camera position.
		foundHeight = m_Terrain->GetHeightAtPosition(posX, posZ, height);
		if(foundHeight)
		{
			if (posY < height + m_Position->getHeight() || m_Position->isGrounded())
			{
				// If there was a triangle under the camera then position the camera just above it by one meter.
				m_Position->SetPosition(posX, height + m_Position->getHeight(), posZ);
				m_Position->hitGround();
				m_Camera->SetPosition(posX, height + m_Position->getHeight(), posZ);
			}
		}
	}

	if ((outOfBounds(&positionAsVec) || getAngleMovement() > m_angleTolerance) && !m_firstFrame)
		pushBackPlayer();
	else
	{
		m_lastFramePosition->x = posX;
		m_lastFramePosition->y = posY;
		m_lastFramePosition->z = posZ;
	}
	if (m_firstFrame)
	{
		m_lastFramePosition->x = posX;
		m_lastFramePosition->y = posY;
		m_lastFramePosition->z = posZ;
		spawnObjectives(5);
		m_firstFrame = false;
	}

	// Render the graphics.
	m_Camera->RenderReflection(posY);
	result = Render(Direct3D, ShaderManager, TextureManager);
	if(!result)
	{
		return false;
	}

	return true;
}

void ZoneClass::batHit(Bat* b)
{
	m_batKilledAudio->playFor(1);
	incrementScore(1);
	m_enemiesToDelete.push_back(b);
	//__debugbreak();
	//m_enemies.remove(b);
	//__debugbreak();
}

void ZoneClass::playerHit(Bat* b)
{
	//__debugbreak();
	//m_enemies.remove(b);
	//__debugbreak();
	m_damageAudio->playFor(1);
	m_enemiesToDelete.push_back(b);
	m_health -= 1;
}

void ZoneClass::pushBackPlayer()
{
	m_Position->SetPosition(m_lastFramePosition->x, m_lastFramePosition->y, m_lastFramePosition->z);
	m_Position->resetXVelocity();
	m_Position->resetYVelocity();
	m_Position->resetZVelocity();
}

void ZoneClass::objectiveCollected(ObjectiveClass* o)
{
	m_objectiveCompleteAudio->playFor(1.5f);
	m_objectivesToDelete.push_back(o);
	incrementScore(1);
	if (m_objectives.size() < 1)
	{
		spawnObjectives(5);
	}
}

float* ZoneClass::getPlayerPositionXPtr()
{
	return m_Position->getPositionXPtr();
}

float* ZoneClass::getPlayerPositionYPtr()
{
	return m_Position->getPositionYPtr();
}

float* ZoneClass::getPlayerPositionZPtr()
{
	return m_Position->getPositionZPtr();
}

//XMFLOAT3* ZoneClass::getPlayerLocationPtr()
//{
//	return m_Position->GetPositionAsVector();
//}


int ZoneClass::getEnemyDifficulty()
{
	return m_batsDifficulty;
}

void ZoneClass::HandleMovementInput(InputClass* Input, float frameTime, D3DClass* d3d)
{
	bool keyDown;
	float posX, posY, posZ, rotX, rotY, rotZ;


	// Set the frame time for calculating the updated position.
	m_Position->SetFrameTime(frameTime);

	// Handle the input.
	keyDown = Input->IsLeftPressed();
	m_Position->TurnLeft(keyDown);

	keyDown = Input->IsRightPressed();
	m_Position->TurnRight(keyDown);

	//keyDown = Input->IsUpPressed();
	//m_Position->MoveForward(keyDown);

	//keyDown = Input->IsDownPressed();
	//m_Position->MoveBackward(keyDown);

	//keyDown = Input->IsAPressed();
	//m_Position->MoveUpward(keyDown);

	//keyDown = Input->IsZPressed();
	//m_Position->MoveDownward(keyDown);

	keyDown = Input->IsRPressed();
	m_Position->MoveUpward(keyDown);

	keyDown = Input->IsFPressed();
	m_Position->MoveDownward(keyDown);

	keyDown = Input->IsUpPressed();
	m_Position->LookUpward(keyDown);

	keyDown = Input->IsDownPressed();
	m_Position->LookDownward(keyDown);

	keyDown = Input->IsWPressed();
	m_Position->MoveForward(keyDown);

	keyDown = Input->IsAPressed();
	m_Position->MoveLeft(keyDown);

	keyDown = Input->IsSPressed();
	m_Position->MoveBackward(keyDown);

	keyDown = Input->IsDPressed();
	m_Position->MoveRight(keyDown);

	keyDown = Input->IsSpacePressed();
	m_Position->jump(keyDown);

	keyDown = (Input->isLeftMouseDown() || Input->IsEPressed());
	if (keyDown)
		fire(d3d->GetDevice(), d3d->GetDeviceContext());

	keyDown = (Input->isRightMouseDown() || Input->IsQPressed());
	if (keyDown)
		zoom();
	else
		unZoom();
	adjustZoom(frameTime);

	keyDown = Input->IsShiftPressed();
	m_Position->sprint(keyDown);

	// HandleMouse Rotations
	m_Position->MouseRotate(Input->GetMouseXDelta(), Input->GetMouseYDelta());

	m_Position->Update();

	// Get the view point position/rotation.
	m_Position->GetPosition(posX, posY, posZ);
	m_Position->GetRotation(rotX, rotY, rotZ);

	// Set the position of the camera.
	m_Camera->SetPosition(posX, posY, posZ);
	m_Camera->SetRotation(rotX, rotY, rotZ);

	// Determine if the user interface should be displayed or not.
	if(Input->IsF1Toggled())
	{
		m_displayUI = !m_displayUI;
	}

	// Determine if the terrain should be rendered in wireframe or not.
	if(Input->IsF2Toggled())
	{
		m_wireFrame = !m_wireFrame;
	}

	// Determine if we should render the lines around each terrain cell.
	if(Input->IsF3Toggled())
	{
		m_cellLines = !m_cellLines;
	}

	// Determine if we should be locked to the terrain height when we move around or not.
	if(Input->IsF4Toggled())
	{
		m_heightLocked = !m_heightLocked;
	}

	return;
}


bool ZoneClass::Render(D3DClass* Direct3D, ShaderManagerClass* ShaderManager, TextureManagerClass* TextureManager)
{
	//TODO: Render... Everything

	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, baseViewMatrix, orthoMatrix, translationMatrix;
	bool result;
	XMFLOAT3 cameraPosition;
	int i;


	//Variables for the fire 
	XMFLOAT3 scrollSpeeds, scales;
	XMFLOAT2 distortion1, distortion2, distortion3;
	float distortionScale, distortionBias;
	static float frameTime = 0.0f;

	// Increment the frame time counter.
	frameTime += 0.01f;
	if (frameTime > 1000.0f)
	{
		frameTime = 0.0f;
	}

	float fireModifer = abs(cosf(m_totalTime * m_distortionTimeMultiplier * XM_PI / 180));

	// Set the three scrolling speeds for the three different noise textures.
	scrollSpeeds = XMFLOAT3(1.3f * fireModifer, 2.1f * fireModifer, 2.3f * fireModifer);

	// Set the three scales which will be used to create the three different noise octave textures.
	scales = XMFLOAT3(1.0f, 2.0f, 3.0f);
	
	// The the scale and bias of the texture coordinate sampling perturbation.
	distortionScale = 0.8f;
	distortionBias = 0.5f;

	// Set the three different x and y distortion factors for the three different noise textures.
	distortion1 = XMFLOAT2(0.1f, 0.2f);
	distortion2 = XMFLOAT2(0.1f, 0.3f);
	distortion3 = XMFLOAT2(0.1f, 0.1f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	Direct3D->GetProjectionMatrix(projectionMatrix);
	m_Camera->GetBaseViewMatrix(baseViewMatrix);
	Direct3D->GetOrthoMatrix(orthoMatrix);
	
	// Get the position of the camera.
	cameraPosition = m_Camera->GetPosition();
	
	// Construct the frustum.

	//if (m_zooming)
	//{
	//	float yawRadians;
	//	float pitchRadians;
	//	float rollRadians;
	//	m_Position->GetRotation(pitchRadians, yawRadians, rollRadians);
	//	yawRadians *= XM_PI / 180;
	//	pitchRadians *= XM_PI / 180;
	//	rollRadians *= XM_PI / 180;

	//	m_Frustum->ConstructFrustum(projectionMatrix, viewMatrix, m_zoomingAdder);

	//	projectionMatrix.r[0].m128_f32[3] += m_zoomingAdder * cosf(yawRadians);
	//	projectionMatrix.r[1].m128_f32[3] += m_zoomingAdder * cosf(yawRadians);
	//	projectionMatrix.r[2].m128_f32[3] += m_zoomingAdder * cosf(yawRadians);
	//	projectionMatrix.r[3].m128_f32[3] += m_zoomingAdder * cosf(yawRadians);

	//}
	//else
	//	m_Frustum->ConstructFrustum(projectionMatrix, viewMatrix, 0);


	float yawRadians;
	float pitchRadians;
	float rollRadians;
	m_Position->GetRotation(pitchRadians, yawRadians, rollRadians);
	yawRadians *= XM_PI / 180;
	pitchRadians *= XM_PI / 180;
	rollRadians *= XM_PI / 180;
	
	float amountToZoom = m_zoomingAdder * m_zoomState / m_timeToZoom;
	m_Frustum->ConstructFrustum(projectionMatrix, viewMatrix, amountToZoom);
	
	XMFLOAT4X4 projectionMatrixAsVector;
	XMStoreFloat4x4(&projectionMatrixAsVector, projectionMatrix);

	//projectionMatrix.r[0].m128_f32[3] += amountToZoom * cosf(yawRadians);
	//projectionMatrix.r[1].m128_f32[3] += amountToZoom * cosf(yawRadians);
	//projectionMatrix.r[2].m128_f32[3] += amountToZoom * cosf(yawRadians);
	//projectionMatrix.r[3].m128_f32[3] += amountToZoom * cosf(yawRadians);

	projectionMatrixAsVector._11 += amountToZoom;// * cosf(yawRadians);//Stretches everything, left-right.
	//projectionMatrixAsVector._12 += amountToZoom * cosf(yawRadians);//Rolls image
	//projectionMatrixAsVector._13 += amountToZoom * cosf(yawRadians);//Blackbars the sides of the screen (like _23)
	//projectionMatrixAsVector._14 += amountToZoom * cosf(yawRadians);//Untested.

	//projectionMatrixAsVector._21 += amountToZoom * cosf(yawRadians);//Stretches everything, top-right and bottom left
	//projectionMatrixAsVector._22 += amountToZoom * cosf(yawRadians);//Stretches everything, top-bottom
	projectionMatrixAsVector._22 += amountToZoom;// *sinf(pitchRadians);//Stretches everything, top-bottom
	//projectionMatrixAsVector._23 -= amountToZoom * cosf(yawRadians);//+ flattens everything (with black bars on top and bottom of screen). - is no more useful
	//projectionMatrixAsVector._24 += amountToZoom * cosf(yawRadians);//Untested.
	
	//projectionMatrixAsVector._31 += amountToZoom * cosf(yawRadians);//stretches everything on left-hand side
	//projectionMatrixAsVector._32 += amountToZoom * cosf(yawRadians);distorts top/bottom
	//projectionMatrixAsVector._33 += amountToZoom * cosf(yawRadians);//Untested.
	//projectionMatrixAsVector._34 += amountToZoom * cosf(yawRadians);//Untested.

	//projectionMatrixAsVector._41 += amountToZoom * cosf(yawRadians);//Untested.
	//projectionMatrixAsVector._42 += amountToZoom * cosf(yawRadians);//Untested.
	//projectionMatrixAsVector._43 -= amountToZoom * cosf(yawRadians);//changes shrinks skybox, do not use
	//projectionMatrixAsVector._44 += amountToZoom * cosf(yawRadians);//Untested.

	projectionMatrix = XMLoadFloat4x4(&projectionMatrixAsVector);

	// Clear the buffers to begin the scene.
	Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Turn off back face culling and turn off the Z buffer.
	Direct3D->TurnOffCulling();
	Direct3D->TurnZBufferOff();

	// Translate the sky dome to be centered around the camera position.
	worldMatrix = XMMatrixTranslation(cameraPosition.x, cameraPosition.y, cameraPosition.z);

	// Render the sky dome using the sky dome shader.
	m_SkyDome->Render(Direct3D->GetDeviceContext());
	result = ShaderManager->RenderSkyDomeShader(Direct3D->GetDeviceContext(), m_SkyDome->GetIndexCount(), worldMatrix, viewMatrix, 
												projectionMatrix, m_SkyDome->GetApexColor(), m_SkyDome->GetCenterColor());
	if(!result)
	{
		return false;
	}

	// Turn the Z buffer back and back face culling on.
	Direct3D->TurnZBufferOn();
	Direct3D->TurnOnCulling();
	
	// Turn on wire frame rendering of the terrain if needed.
	if(m_wireFrame)
	{
		Direct3D->EnableWireframe();
	}

	// Reset the world matrix.
	Direct3D->GetWorldMatrix(worldMatrix);
	translationMatrix = XMMatrixTranslation(m_WindowPane->getPosition()->x, m_WindowPane->getPosition()->y, m_WindowPane->getPosition()->z);
	worldMatrix = XMMatrixMultiply(worldMatrix, translationMatrix);
	m_WindowPane->Render(Direct3D->GetDeviceContext());
	result = ShaderManager->RenderGlassShader(Direct3D->GetDeviceContext(), m_WindowPane->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_WindowPane->GetTexture(),
		m_WindowPane->GetNormalMap(), m_RenderTexture->GetShaderResourceView(), m_WindowPane->m_refractionScale);
	//
	//result = ShaderManager->RenderTextureShader(Direct3D->GetDeviceContext(), m_WindowPane->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_WindowPane->GetTexture());
	if (!result)
	{
		return false;
	}

	for each (BulletClass* b in m_projectiles)
	{
		Direct3D->GetWorldMatrix(worldMatrix);
		translationMatrix = XMMatrixTranslation(b->getPosition()->x, b->getPosition()->y, b->getPosition()->z);
		XMFLOAT3* rotationMatrix = b->getRotation();
		worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixRotationX((rotationMatrix->x * XM_PI / 180) + XM_PIDIV2));
		worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixRotationY(rotationMatrix->y * XM_PI / 180));
		worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixScaling(m_bulletScale, m_bulletScale, m_bulletScale));
		worldMatrix = XMMatrixMultiply(worldMatrix, translationMatrix);
		//Probably going to need to apply scaling.
		b->Render(Direct3D->GetDeviceContext());
		result = ShaderManager->RenderLightShader(Direct3D->GetDeviceContext(), b->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
			m_WindowPane->GetTexture(), m_Light->GetDirection(), m_Light->GetDiffuseColor());
		if (!result)
		{
			return false;
		}
	}

	Direct3D->GetWorldMatrix(worldMatrix);

	for each (ObjectiveClass* o in m_objectives)
	{
		worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixScaling(m_objectiveScale, m_objectiveScale, m_objectiveScale));
		translationMatrix = XMMatrixTranslation(o->getPosition()->x, o->getPosition()->y, o->getPosition()->z);
		worldMatrix = XMMatrixMultiply(worldMatrix, translationMatrix);
		o->getModel()->Render(Direct3D->GetDeviceContext());
		result = ShaderManager->RenderReflectionShader(Direct3D->GetDeviceContext(), o->getModel()->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
			o->getModel()->GetTexture(), m_reflectionTexture->GetTexture(), m_Camera->GetReflectionViewMatrix());
		Direct3D->GetWorldMatrix(worldMatrix);
	}

	for each (Bat* b in m_enemies)
	{
		worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixScaling(m_batScale, m_batScale, m_batScale));
		worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixRotationX(b->getRotation()->x));
		worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixRotationY(-(b->getRotation()->y + XM_PIDIV2)));
		worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixRotationZ(b->getRotation()->z));
		translationMatrix = XMMatrixTranslation(b->getPosition()->x, b->getPosition()->y, b->getPosition()->z);
		worldMatrix = XMMatrixMultiply(worldMatrix, translationMatrix);
		//Probably going to need to apply scaling.
		b->getModel()->Render(Direct3D->GetDeviceContext());
		result = ShaderManager->RenderFireShader(Direct3D->GetDeviceContext(), b->getModel()->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
			b->getModel()->GetTexture1(), b->getModel()->GetTexture2(), b->getModel()->GetTexture3(), frameTime, scrollSpeeds,
			scales, distortion1, distortion2, distortion3, distortionScale, distortionBias);
		
		if (!result)
		{
			return false;
		}
		Direct3D->GetWorldMatrix(worldMatrix);
	}

	// Render the terrain cells (and cell lines if needed).
	for(i=0; i<m_Terrain->GetCellCount(); i++)
	{
		// Render each terrain cell if it is visible only.
		result = m_Terrain->RenderCell(Direct3D->GetDeviceContext(), i, m_Frustum);
		if(result)
		{
			// Render the cell buffers using the hgih quality terrain shader.
			result = ShaderManager->RenderTerrainShader(Direct3D->GetDeviceContext(), m_Terrain->GetCellIndexCount(i), worldMatrix, viewMatrix,
														projectionMatrix, TextureManager->GetTexture(0), TextureManager->GetTexture(1),
														TextureManager->GetTexture(2), TextureManager->GetTexture(3),
														m_Light->GetDirection(), m_Light->GetDiffuseColor());
			if(!result)
			{
				return false;
			}

			// If needed then render the bounding box around this terrain cell using the color shader. 
			if(m_cellLines)
			{
				m_Terrain->RenderCellLines(Direct3D->GetDeviceContext(), i);
				ShaderManager->RenderColorShader(Direct3D->GetDeviceContext(), m_Terrain->GetCellLinesIndexCount(i), worldMatrix, 
												 viewMatrix, projectionMatrix);
				if(!result)
				{
					return false;
				}
			}
		}
	}
	
	// Reset the world matrix.

	//Direct3D->GetWorldMatrix(worldMatrix);
	//translationMatrix = XMMatrixTranslation(m_CentreSkull->position.x, m_CentreSkull->position.y, m_CentreSkull->position.z);
	//worldMatrix = XMMatrixMultiply(worldMatrix, translationMatrix);
	//m_CentreSkull->Render(Direct3D->GetDeviceContext());
	//result = ShaderManager->RenderFireShader(Direct3D->GetDeviceContext(), m_CentreSkull->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
	//	m_CentreSkull->GetTexture1(), m_CentreSkull->GetTexture2(), m_CentreSkull->GetTexture3(), frameTime, scrollSpeeds,
	//	scales, distortion1, distortion2, distortion3, distortionScale, distortionBias);
	//if (!result)
	//{
	//	return false;
	//}

	//// Reset the world matrix.
	//Direct3D->GetWorldMatrix(worldMatrix);
	//translationMatrix = XMMatrixTranslation(m_BubbleSkull->position.x, m_BubbleSkull->position.y, m_BubbleSkull->position.z);
	//worldMatrix = XMMatrixMultiply(worldMatrix, translationMatrix);
	//m_BubbleSkull->Render(Direct3D->GetDeviceContext());
	//result = ShaderManager->RenderBubbleShader(Direct3D->GetDeviceContext(), m_BubbleSkull->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
	//	m_BubbleSkull->GetTexture1(), m_BubbleSkull->GetTexture2(), m_Camera->GetPosition());
	//if (!result)
	//{
	//	return false;
	//}

	// Reset the world matrix.
	Direct3D->GetWorldMatrix(worldMatrix);

	// Render the scene to texture first.
	result = RenderToTexture(Direct3D, ShaderManager, yawRadians, TextureManager);
	if (!result)
	{
		return false;
	}
	for each (BatSpawner * bs in m_spawners)
	{
		worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixScaling(m_cannonScale, m_cannonScale, m_cannonScale));
		translationMatrix = XMMatrixTranslation(bs->getPosition()->x, bs->getPosition()->y, bs->getPosition()->z);
		worldMatrix = XMMatrixMultiply(worldMatrix, translationMatrix);
		//Probably going to need to apply scaling.
		bs->Render(Direct3D->GetDeviceContext());
		result = ShaderManager->RenderGlassShader(Direct3D->GetDeviceContext(), bs->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
			bs->getModel()->GetTexture(), bs->getModel()->GetNormalMap(), m_RenderTexture->GetShaderResourceView(), bs->getModel()->m_refractionScale);
		//bs->getModel()->GetTexture(), bs->getModel()->GetNormalMap(), m_reflectionTexture->GetTexture(), bs->getModel()->m_refractionScale);
		if (!result)
		{
			return false;
		}
		Direct3D->GetWorldMatrix(worldMatrix);
	}

	// Turn off wire frame rendering of the terrain if it was on.
	if(m_wireFrame)
	{
		Direct3D->DisableWireframe();  
	}

	// Update the render counts in the UI.
	result = m_UserInterface->UpdateRenderCounts(Direct3D->GetDeviceContext(), m_Terrain->GetRenderCount(), m_Terrain->GetCellsDrawn(), 
												 m_Terrain->GetCellsCulled(), m_health, m_score, m_ammo, m_ammoCap, m_enemies.size() );
	if(!result)
	{
		return false;
	}

	// Render the user interface.
	if(m_displayUI)
	{
		result = m_UserInterface->Render(Direct3D, ShaderManager, worldMatrix, baseViewMatrix, orthoMatrix);
		if(!result)
		{
			return false;
		}
	}

	// Present the rendered scene to the screen.

	Direct3D->EndScene();

	return true;
}

bool ZoneClass::isUnderground(XMFLOAT3* point)
{
	float yCoord;
	m_Terrain->GetHeightAtPosition(point->x, point->z, yCoord);
	if (point->y < yCoord)
		return true;
	return false;
}

bool ZoneClass::isUnderground(BoundingBox* box)
{
	for each (XMFLOAT3 vert in box->verts())
	{
		if (isUnderground(&vert))
			return true;
	}
	return false;
}

void ZoneClass::zoom()
{
	m_zooming = true;
}

void ZoneClass::unZoom()
{
	m_zooming = false;
}

void ZoneClass::adjustZoom(float frameTime)
{
	if (m_zooming && m_zoomState < m_timeToZoom)
	{
		m_zoomState += frameTime;
		if (m_timeToZoom < m_zoomState)
			m_zoomState = m_timeToZoom;
	}
	else if (m_zoomState > 0 && !m_zooming)
	{
		m_zoomState -= frameTime;
		if (0 > m_zoomState)
			m_zoomState = 0;
	}
}

void ZoneClass::fire(ID3D11Device* device, ID3D11DeviceContext* context)
{
	if (m_ammo > 0 && m_shotCooldown <= 0)
	{
		m_ammo -= 1;
		//Fire a new projectile, add it to the list of projectiles.
		BulletClass* newBullet = new BulletClass;
		XMFLOAT3 positionAsVector = m_Camera->GetPosition();
		XMFLOAT3 rotationAsVector = m_Camera->GetRotation();
		//newBullet->Initialize(device, context, "../Project/data/models/Bullet.txt", "../Project/data/models/BulletColour.dds", "../Project/data/models/BulletColour.dds", 
		//	&positionAsVector, &rotationAsVector, m_shotPower);
		newBullet->InitializeWithModel(m_bulletModel, &positionAsVector, &rotationAsVector, m_shotPower);
		m_projectiles.push_back(newBullet);
		m_shootingAudio->playFor(1);
		m_shotCooldown = m_timeBetweenShots;
	}
}

void ZoneClass::checkAllCollisions()
{

}

void ZoneClass::spawnObjectives(int amountToSpawn)
{
	for (int i = 0; i < amountToSpawn; i++)
		spawnNewObjective();
}

void ZoneClass::incrementScore(int score)
{
	m_score += score;
	//To change things past milestones do it here.
}

void ZoneClass::tickBatSpawners(D3DClass* Direct3D, float timePassed)
{
	if (m_timeToSpawnBat > 0 && m_enemies.size() < m_maxBats)
	{
		m_timeToSpawnBat -= timePassed;
		if (m_timeToSpawnBat < 0)
		{
			m_timeToSpawnBat = m_timeBetweenSpawns;
			spawnBats(1);
		}
	}
}

void ZoneClass::spawnNewObjective()
{
	float xCoord;
	float zCoord;
	float yCoord;

	//Select random coordinates that are inside the zone of acceptable coordinates.
	random_device rd1;
	default_random_engine generator1(rd1());
	uniform_real_distribution<float> distributionX(50, 974);
	xCoord = distributionX(generator1);
	random_device rd2;
	default_random_engine generator2(rd2());
	uniform_real_distribution<float> distributionZ(50, 974);
	zCoord = distributionZ(generator2);

	//RNG the X and Z coordinates, find the y
	m_Terrain->GetHeightAtPosition(xCoord, zCoord, yCoord);
	//m_Terrain->GetHeightAtPosition(500, 140, yCoord);

	ObjectiveClass* newObjective = new ObjectiveClass;
	
	XMFLOAT3* coords = new XMFLOAT3(xCoord, m_objectiveFloat + yCoord, zCoord);
	//XMFLOAT3* coords = new XMFLOAT3(500, yCoord + m_objectiveFloat, 140);

	newObjective->initializeWithModel(m_objectiveModel, coords);
	newObjective->m_xSize = m_objectiveSize.x;
	newObjective->m_ySize = m_objectiveSize.y;
	newObjective->m_zSize = m_objectiveSize.z;

	m_objectives.push_back(newObjective);
}

bool ZoneClass::spawnBats(int amountToSpawn)
{
	bool result = true;
	Bat* newBat = 0;
	for (int i = 0; i < amountToSpawn; i++)
	{
		//Spawn and initialize the bats
		if (m_lastSpawnerUsed >= m_spawners.size())
		{
			m_lastSpawnerUsed = 0;
			newBat = m_spawners.front()->spawnBatWithModel(m_fireBatModel, this, m_batZone);
		}
		else
		{
			list<BatSpawner*>::iterator batSpawnIter = m_spawners.begin();
			advance(batSpawnIter, m_lastSpawnerUsed);
			BatSpawner* spawnerPtr = *batSpawnIter;
			newBat = spawnerPtr->spawnBatWithModel(m_fireBatModel, this, m_batZone);
		}
		newBat->m_xSize = m_batSize.x;
		newBat->m_ySize = m_batSize.y;
		newBat->m_zSize = m_batSize.z;
		m_enemies.push_back(newBat);
		m_lastSpawnerUsed++;
	}
	if (!newBat)
		return false;
	return result;
}

bool ZoneClass::outOfBounds(XMFLOAT3* coord)
{
	if (coord->x <= 0 || coord->x >= 1000)
		return true;
	if (coord->z <= 0 || coord->z >= 1000)
		return true;
	return false;
}

float ZoneClass::getAngleMovement()
{
	if (m_Position->isGrounded())
	{
		float posX, posY, posZ;
		m_Position->GetPosition(posX, posY, posZ);
		float diffX = posX - m_lastFramePosition->x;
		float diffY = posY - m_lastFramePosition->y;
		float diffZ = posZ - m_lastFramePosition->z;
		float diffXZ = sqrt(pow(diffX, 2) + pow(diffZ, 2));//Pythagorus.
		return atan(diffY / diffXZ) * 180 / XM_PI;
	}
	return 0.0f;
}

void ZoneClass::tickFiring(float frameTime)
{
	m_shotCooldown -= frameTime;
	if (m_ammo < m_ammoCap)
	{
		m_timeUntilNextAmmoDrop -= frameTime;
		if (m_timeUntilNextAmmoDrop < 0)
		{
			m_ammo += 1;
			m_timeUntilNextAmmoDrop = m_timeBetweenAmmoDrops;
		}
	}
}

void ZoneClass::checkObjectiveWave()
{
}

void ZoneClass::deleteThings()
{
	for each (BulletClass* b in m_bulletsToDelete)
	{
		m_projectiles.remove(b);
	}
	m_bulletsToDelete.clear();
	for each (Bat* b in m_enemiesToDelete)
	{
		m_enemies.remove(b);
	}
	m_enemiesToDelete.clear();
	for each (ObjectiveClass* o in m_objectivesToDelete)
	{
		m_objectives.remove(o);
	}
	m_objectivesToDelete.clear();
}

void ZoneClass::tickTimeOfDay(float frameTime)
{
	if (m_timeGoingUp)
		m_timeOfDay += frameTime;
	else
		m_timeOfDay -= frameTime;

	if (m_timeOfDay > m_dayCycleTime)
	{
		m_timeOfDay = m_dayCycleTime;
		m_timeGoingUp = false;
	}
	if (0 > m_timeOfDay)
	{
		m_timeOfDay = 0;
		m_timeGoingUp = true;
	}

	float portionThroughDay = m_timeOfDay / m_dayCycleTime;

	float r = portionThroughDay * (m_apexDayColour.x - m_apexNightColour.x) + m_apexNightColour.x;
	float g = portionThroughDay * (m_apexDayColour.y - m_apexNightColour.y) + m_apexNightColour.y;
	float b = portionThroughDay * (m_apexDayColour.z - m_apexNightColour.z) + m_apexNightColour.z;
	//float a = portionThroughDay * (m_apexDayColour.w - m_apexNightColour.w) + m_apexNightColour.w;
	XMFLOAT4 colour = { r, g, b, 1 };

	m_SkyDome->SetApexColour(colour);

	r = portionThroughDay * (m_centreDayColour.x - m_centreNightColour.x) + m_centreNightColour.x;
	g = portionThroughDay * (m_centreDayColour.y - m_centreNightColour.y) + m_centreNightColour.y;
	b = portionThroughDay * (m_centreDayColour.z - m_centreNightColour.z) + m_centreNightColour.z;
	//a = portionThroughDay * (m_centreDayColour.w - m_centreNightColour.w) + m_centreNightColour.w;
	colour = { r, g, b, 1 };

	m_SkyDome->SetApexColour(colour);

	r = portionThroughDay * (m_dayLightColour.x - m_nightLightColour.x) + m_nightLightColour.x;
	g = portionThroughDay * (m_dayLightColour.y - m_nightLightColour.y) + m_nightLightColour.y;
	b = portionThroughDay * (m_dayLightColour.z - m_nightLightColour.z) + m_nightLightColour.z;

	m_Light->SetDiffuseColor(r, g, b, 1);
}
