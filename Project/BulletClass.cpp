#include "BulletClass.h"

BulletClass::BulletClass()
{
	model = new ModelClass;
}

BulletClass::~BulletClass()
{
}

//bool BulletClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* context, char* model, char* tex1, char* tex2, XMFLOAT3* playPos, XMFLOAT3* playRot, float power)
//{
//	position = *playPos;
//	rotation = *playRot;
//
//	float radians = rotation.y * XM_PI / 180;
//	float radiansX = rotation.x * XM_PI / 180;
//	XMVECTOR vec = XMVector3Normalize(XMVectorSet(sinf(radians), cosf(radiansX), cosf(radians), 0));
//	
//	velocity.x = power * XMVectorGetX(vec);//?
//	velocity.y = power * XMVectorGetY(vec);//?
//	velocity.z = power * XMVectorGetZ(vec);//?
//	
//	//The old moving forward method in semester 1
//
//	//// Convert degrees to radians.
//	//radians = m_rotationY * DEGREE_TO_RADIAN_CONVERSION_RATE;
//	//radiansX = m_rotationX * DEGREE_TO_RADIAN_CONVERSION_RATE;
//
//	//// Update the position.
//	//m_positionX += sinf(radians) * m_forwardSpeed * cosf(radiansX);
//	//m_positionZ += cosf(radians) * m_forwardSpeed * cosf(radiansX);
//	//m_positionY -= sinf(radiansX) * m_forwardSpeed;
//
//	return this->model->Initialize(device, context, model, tex1, tex2);
//
//}

bool BulletClass::InitializeWithModel(ModelClass* model, XMFLOAT3* playPos, XMFLOAT3* playRot, float power)
{
	position = *playPos;
	rotation = *playRot;

	rotation.x = -rotation.x;

	velocity.x = power * sinf(rotation.y * XM_PI / 180);
	velocity.y = power * sinf(rotation.x * XM_PI / 180);
	velocity.z = power * cosf(rotation.y * XM_PI / 180);

	this->model = model;
	return true;
}

void BulletClass::Shutdown()
{
	model->Shutdown();
}

XMFLOAT3* BulletClass::getPosition()
{
	return &position;
}

XMFLOAT3* BulletClass::getRotation()
{
	return &rotation;
}

void BulletClass::move(float frameTime)
{
	position.x += velocity.x * frameTime;
	position.y += velocity.y * frameTime;
	position.z += velocity.z * frameTime;
	lifeSpan -= frameTime;
}

void BulletClass::Render(ID3D11DeviceContext* context)
{
	model->Render(context);
}

int BulletClass::GetIndexCount()
{
	return model->GetIndexCount();
}
