#pragma once
#include "BaseModelClass.h"
class WindowModelClass : public BaseModelClass
{
private:
	//TextureClass* colourTexture;
	TextureClass* normalTexture;
	//TextureClass* refractionTexture;
	void RenderBuffers(ID3D11DeviceContext* deviceContext);
	bool LoadSpecificTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename, TextureClass*& texture);
public:
	virtual bool Initialize(ID3D11Device*, ID3D11DeviceContext*, char*, char*, char*, char*);
	void Render(ID3D11DeviceContext* context);
	void Shutdown();
	ID3D11ShaderResourceView* GetNormalMap();
	//ID3D11ShaderResourceView* GetRefractionMap();

	WindowModelClass();

	float m_refractionScale = 0.6f;
};
