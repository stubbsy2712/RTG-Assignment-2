////////////////////////////////////////////////////////////////////////////////
// Filename: positionclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _POSITIONCLASS_H_
#define _POSITIONCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <math.h>
#include "Soundclass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: PositionClass
////////////////////////////////////////////////////////////////////////////////
class PositionClass
{
public:
	PositionClass();
	PositionClass(const PositionClass&);
	~PositionClass();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	void GetPosition(float&, float&, float&);
	void GetRotation(float&, float&, float&);

	void SetFrameTime(float);

	void MoveForward(bool);
	void MoveBackward(bool);
	void MoveUpward(bool);
	void MoveDownward(bool);
	void MoveRight(bool);
	void MoveLeft(bool);
	void TurnLeft(bool);
	void TurnRight(bool);
	void LookUpward(bool);
	void LookDownward(bool);
	void jump(bool);
	void sprint(bool);
	void resetXVelocity();
	void resetYVelocity();
	void resetZVelocity();
	void hitGround();
	float getHeight();
	bool isGrounded();

	void applyDecels();
	void MouseRotate(int, int);
	void SetFootstepSound(SoundClass*);
	void Update();

	float* getPositionXPtr();
	float* getPositionYPtr();
	float* getPositionZPtr();

private:
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;

	bool m_inAir;
	bool m_sprinting;
	float m_sprintMultiplier = 1.7f;
	float m_sprintBarCurrent, m_sprintBarMax;
	float m_height = 2;
	float m_jumpPower = 4.5f;
	float m_frameTime;
	float m_turningDecel, m_moveDecel;
	float m_turningAccel, m_moveAccel;
	float m_turningSpeedX, m_turningSpeedY;
	
	float m_distanceBetweenFootsteps = 0.6f;
	float m_distanceUntilFootstep = m_distanceBetweenFootsteps;
	float m_gravityValue = 9.8f;
	float m_terminalYVelocity = 0.3f;
	float m_terminalXVelocity = 0.3f;
	float m_terminalZVelocity = 0.3f;
	SoundClass* footstepSound;
	
	void updateFootstep();
	void applyGravity();
	void applyTerminals();
	void applyMovement();

	float m_omniDirectionalSpeedCap;
	float m_xMomentum, m_yMomentum, m_zMomentum;
	float toRadians = 0.0174532925f;

	//float m_forwardSpeed, m_backwardSpeed;
	//float m_rightSpeed, m_leftSpeed;
	//float m_upwardSpeed, m_downwardSpeed;
	//float m_leftTurnSpeed, m_rightTurnSpeed;
	//float m_lookUpSpeed, m_lookDownSpeed;
};

#endif