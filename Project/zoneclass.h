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

private:
	void HandleMovementInput(InputClass*, float);
	bool Render(D3DClass*, ShaderManagerClass*, TextureManagerClass*);
	//bool RenderToTexture(float rotation);

private:

	FireModelClass* m_CentreSkull;
	WindowModelClass* m_WindowPane;
	RenderTextureClass* m_RenderTexture;

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