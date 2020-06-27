////////////////////////////////////////////////////////////////////////////////
// Filename: positionclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "positionclass.h"


PositionClass::PositionClass()
{
	m_positionX = 0.0f;
	m_positionY = 0.0f;
	m_positionZ = 0.0f;
	
	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_rotationZ = 0.0f;

	m_frameTime = 0.0f;

	//m_forwardSpeed   = 0.0f;
	//m_backwardSpeed  = 0.0f;
	//m_upwardSpeed    = 0.0f;
	//m_downwardSpeed  = 0.0f;
	//m_leftTurnSpeed  = 0.0f;
	//m_rightTurnSpeed = 0.0f;
	//m_lookUpSpeed    = 0.0f;
	//m_lookDownSpeed  = 0.0f;

	m_turningDecel = 4;
	m_moveAccel = 12.5f;
	m_moveDecel = 0.5f;
	m_turningDecel = 2.5f;
	m_turningAccel = 2.5f;
	//m_jumpPower = 0.5f;

	m_sprintBarCurrent = 10;
	m_sprintBarMax = 10;
}


PositionClass::PositionClass(const PositionClass& other)
{
}


PositionClass::~PositionClass()
{
}


void PositionClass::SetPosition(float x, float y, float z)
{
	m_positionX = x;
	m_positionY = y;
	m_positionZ = z;
	return;
}


void PositionClass::SetRotation(float x, float y, float z)
{
	m_rotationX = x;
	m_rotationY = y;
	m_rotationZ = z;
	return;
}


void PositionClass::GetPosition(float& x, float& y, float& z)
{
	x = m_positionX;
	y = m_positionY;
	z = m_positionZ;
	return;
}


void PositionClass::GetRotation(float& x, float& y, float& z)
{
	//Returns degrees
	x = m_rotationX;
	y = m_rotationY;
	z = m_rotationZ;
	return;
}


void PositionClass::SetFrameTime(float time)
{
	m_frameTime = time;
	return;
}


void PositionClass::MoveForward(bool keydown)
{

	// Update the forward speed movement based on the frame time and whether the user is holding the key down or not.
	if(keydown)
	{
		m_xMomentum += m_frameTime * m_moveAccel * sinf(toRadians * m_rotationY);
		m_zMomentum += m_frameTime * m_moveAccel * cosf(toRadians * m_rotationY);

		//m_forwardSpeed += m_frameTime * 1.0f;
		//if(m_forwardSpeed > (m_frameTime * 50.0f))
		//{
		//	m_forwardSpeed = m_frameTime * 50.0f;
		//}
	}
	//else
	//{
	//	m_forwardSpeed -= m_frameTime * 0.5f;

	//	if(m_forwardSpeed < 0.0f)
	//	{
	//		m_forwardSpeed = 0.0f;
	//		return;
	//	}
	//}

	//// Convert degrees to radians.
	//float radians = m_rotationY * 0.0174532925f;

	//// Update the position.
	//m_positionX += sinf(radians) * m_forwardSpeed;
	//m_positionZ += cosf(radians) * m_forwardSpeed;

	return;
}


void PositionClass::MoveBackward(bool keydown)
{
	// Update the backward speed movement based on the frame time and whether the user is holding the key down or not.
	if(keydown)
	{
		m_xMomentum -= m_frameTime * m_moveAccel * sinf(toRadians * m_rotationY);
		m_zMomentum -= m_frameTime * m_moveAccel * cosf(toRadians * m_rotationY);

		//m_backwardSpeed += m_frameTime * 1.0f;

		//if(m_backwardSpeed > (m_frameTime * 50.0f))
		//{
		//	m_backwardSpeed = m_frameTime * 50.0f;
		//}
	}
	//else
	//{
	//	m_backwardSpeed -= m_frameTime * 0.5f;
	//	
	//	if(m_backwardSpeed < 0.0f)
	//	{
	//		m_backwardSpeed = 0.0f;
	//		return;
	//	}
	//}

	//// Convert degrees to radians.
	//float radians = m_rotationY * 0.0174532925f;

	//// Update the position.
	//m_positionX -= sinf(radians) * m_backwardSpeed;
	//m_positionZ -= cosf(radians) * m_backwardSpeed;

	//return;
}

void PositionClass::MoveUpward(bool keydown)
{
	//// Update the upward speed movement based on the frame time and whether the user is holding the key down or not.
	//if(keydown)
	//{
	//	m_upwardSpeed += m_frameTime * 1.5f;

	//	if(m_upwardSpeed > (m_frameTime * 15.0f))
	//	{
	//		m_upwardSpeed = m_frameTime * 15.0f;
	//	}
	//}
	//else
	//{
	//	m_upwardSpeed -= m_frameTime * 0.5f;

	//	if(m_upwardSpeed < 0.0f)
	//	{
	//		m_upwardSpeed = 0.0f;
	//		return;
	//	}
	//}

	//// Update the height position.
	//m_positionY += m_upwardSpeed;

	//return;
}

void PositionClass::MoveDownward(bool keydown)
{
	//// Update the downward speed movement based on the frame time and whether the user is holding the key down or not.
	//if(keydown)
	//{
	//	m_downwardSpeed += m_frameTime * 1.5f;

	//	if(m_downwardSpeed > (m_frameTime * 15.0f))
	//	{
	//		m_downwardSpeed = m_frameTime * 15.0f;
	//	}
	//}
	//else
	//{
	//	m_downwardSpeed -= m_frameTime * 0.5f;

	//	if(m_downwardSpeed < 0.0f)
	//	{
	//		m_downwardSpeed = 0.0f;
	//		return;
	//	}
	//}

	//// Update the height position.
	//m_positionY -= m_downwardSpeed;

	//return;
}

void PositionClass::MoveRight(bool keydown)
{
	// Update the upward speed movement based on the frame time and whether the user is holding the key down or not.
	if (keydown)
	{
		m_xMomentum += m_frameTime * m_moveAccel * cosf(toRadians * m_rotationY);
		m_zMomentum -= m_frameTime * m_moveAccel * sinf(toRadians * m_rotationY);

		//m_rightSpeed += m_frameTime * 1.5f;

		//if (m_rightSpeed > (m_frameTime * 15.0f))
		//{
		//	m_rightSpeed = m_frameTime * 15.0f;
		//}
	}
	//else
	//{
	//	m_rightSpeed -= m_frameTime * 0.5f;

	//	if (m_rightSpeed < 0.0f)
	//	{
	//		m_rightSpeed = 0.0f;
	//		return;
	//	}
	//}

	//// Convert degrees to radians.
	//radians = m_rotationY * 0.0174532925f;

	//// Update the position.
	//m_positionX += sinf(radians) * m_forwardSpeed;
	//m_positionZ += cosf(radians) * m_forwardSpeed;

	//float radians = m_rotationY * 0.0174532925f;

	//// Update the height position.
	//m_positionX += m_rightSpeed * cosf(radians);
	//m_positionZ -= m_rightSpeed * sinf(radians);

	//return;
}

void PositionClass::MoveLeft(bool keydown)
{
	// Update the upward speed movement based on the frame time and whether the user is holding the key down or not.
	if (keydown)
	{
		m_xMomentum -= m_frameTime * m_moveAccel * cosf(toRadians * m_rotationY);
		m_zMomentum += m_frameTime * m_moveAccel * sinf(toRadians * m_rotationY);

		//if (m_leftSpeed > (m_frameTime * 15.0f))
		//{
		//	m_leftSpeed = m_frameTime * 15.0f;
		//}

	}

	//else
	//{
	//	m_leftSpeed -= m_frameTime * 0.5f;

	//	if (m_leftSpeed < 0.0f)
	//	{
	//		m_leftSpeed = 0.0f;
	//	}
	//}

	// Update the height position.

	//float radians = m_rotationY * 0.0174532925f;

	// Update the height position.

	//m_positionX -= m_leftSpeed* cosf(radians);
	//m_positionZ += m_leftSpeed * sinf(radians);

	//return;
}

void PositionClass::TurnLeft(bool keydown)
{
	// Update the left turn speed movement based on the frame time and whether the user is holding the key down or not.
	if(keydown)
	{
		m_turningSpeedY -= m_frameTime * m_turningAccel;

		//m_leftTurnSpeed += m_frameTime * 5.0f;

		//if(m_leftTurnSpeed > (m_frameTime * 150.0f))
		//{
		//	m_leftTurnSpeed = m_frameTime * 150.0f;
		//}
	}
	//else
	//{
	//	m_leftTurnSpeed -= m_frameTime* m_turningDecel;

	//	if(m_leftTurnSpeed < 0.0f)
	//	{
	//		m_leftTurnSpeed = 0.0f;
	//	}
	//}

	//// Update the rotation.
	//m_rotationY -= m_leftTurnSpeed;

	//// Keep the rotation in the 0 to 360 range.
	//if(m_rotationY < 0.0f)
	//{
	//	m_rotationY += 360.0f;
	//}

	//return;
}


void PositionClass::TurnRight(bool keydown)
{
	// Update the right turn speed movement based on the frame time and whether the user is holding the key down or not.
	if(keydown)
	{
		m_turningSpeedY += m_frameTime * m_turningAccel;
		//m_rightTurnSpeed += m_frameTime * 5.0f;

		//if(m_rightTurnSpeed > (m_frameTime * 150.0f))
		//{
		//	m_rightTurnSpeed = m_frameTime * 150.0f;
		//}
	}
	//else
	//{
	//	m_rightTurnSpeed -= m_frameTime* m_turningDecel;

	//	if(m_rightTurnSpeed < 0.0f)
	//	{
	//		m_rightTurnSpeed = 0.0f;
	//	}
	//}

	//// Update the rotation.
	//m_rotationY += m_rightTurnSpeed;

	//// Keep the rotation in the 0 to 360 range.
	//if(m_rotationY > 360.0f)
	//{
	//	m_rotationY -= 360.0f;
	//}

	//return;
}


void PositionClass::LookUpward(bool keydown)
{
	// Update the upward rotation speed movement based on the frame time and whether the user is holding the key down or not.
	if(keydown)
	{
		m_turningSpeedX += m_frameTime * m_turningAccel;
		
		//m_lookUpSpeed += m_frameTime * 7.5f;

		//if(m_lookUpSpeed > (m_frameTime * 75.0f))
		//{
		//	m_lookUpSpeed = m_frameTime * 75.0f;
		//}
	}
	//else
	//{
	//	m_lookUpSpeed -= m_frameTime* 2.0f;

	//	if(m_lookUpSpeed < 0.0f)
	//	{
	//		m_lookUpSpeed = 0.0f;
	//	}
	//}

	//// Update the rotation.
	//m_rotationX -= m_lookUpSpeed;

	//// Keep the rotation maximum 90 degrees.
	//if(m_rotationX > 90.0f)
	//{
	//	m_rotationX = 90.0f;
	//}

	//return;
}


void PositionClass::LookDownward(bool keydown)
{
	// Update the downward rotation speed movement based on the frame time and whether the user is holding the key down or not.
	if(keydown)
	{
		m_turningSpeedX += m_frameTime * m_turningAccel;

		//m_lookDownSpeed += m_frameTime * 7.5f;

		//if(m_lookDownSpeed > (m_frameTime * 75.0f))
		//{
		//	m_lookDownSpeed = m_frameTime * 75.0f;
		//}
	}
	//else
	//{
	//	m_lookDownSpeed -= m_frameTime* 2.0f;

	//	if(m_lookDownSpeed < 0.0f)
	//	{
	//		m_lookDownSpeed = 0.0f;
	//	}
	//}

	//// Update the rotation.
	//m_rotationX += m_lookDownSpeed;

	//// Keep the rotation maximum 90 degrees.
	//if(m_rotationX < -90.0f)
	//{
	//	m_rotationX = -90.0f;
	//}

	//return;
}

void PositionClass::jump(bool keyDown)
{
	if (keyDown && !m_inAir)
	{
		m_yMomentum += m_jumpPower;
		m_inAir = true;
	}
}

void PositionClass::sprint(bool keyDown)
{
	if (keyDown)
		m_sprinting = true;
}

void PositionClass::resetXVelocity()
{
	m_xMomentum = 0;
}

void PositionClass::resetYVelocity()
{
	m_yMomentum = 0;
}

void PositionClass::resetZVelocity()
{
	m_zMomentum = 0;
}

void PositionClass::hitGround()
{
	resetYVelocity();
	m_inAir = false;
}

float PositionClass::getHeight()
{
	return m_height;
}

bool PositionClass::isGrounded()
{
	return !m_inAir;
}

void PositionClass::applyDecels()
{
	if (m_xMomentum > 0)
		m_xMomentum -= m_moveDecel;
	if (m_yMomentum > 0)
		m_yMomentum -= m_moveDecel;
	if (m_zMomentum > 0)
		m_zMomentum -= m_moveDecel;
	if (m_xMomentum < 0)
		m_xMomentum += m_moveDecel;
	if (m_yMomentum < 0)
		m_yMomentum += m_moveDecel;
	if (m_zMomentum < 0)
		m_zMomentum += m_moveDecel;
	if (m_xMomentum < m_moveDecel && m_xMomentum > -m_moveDecel)
		m_xMomentum = 0;
	if (m_yMomentum < m_moveDecel && m_yMomentum > -m_moveDecel)
		m_yMomentum = 0;
	if (m_zMomentum < m_moveDecel && m_zMomentum > -m_moveDecel)
		m_zMomentum = 0;
}

void PositionClass::MouseRotate(int mouseX, int mouseY)
{
	float MOUSE_SENSITIVITY = 0.08f;

	if ((m_rotationX + (mouseY * MOUSE_SENSITIVITY)) > 90) m_rotationX = 90;
	else if ((m_rotationX + (mouseY * MOUSE_SENSITIVITY)) < -90) m_rotationX = -90;
	else m_rotationX += (mouseY * MOUSE_SENSITIVITY);

	m_rotationY += (mouseX * MOUSE_SENSITIVITY);
	if (m_rotationX > 360) m_rotationX -= 360;
	else if (m_rotationX < -360) m_rotationX += 360;

	return;
}

void PositionClass::Update()
{
	updateFootstep();
	applyGravity();
	//Adjust the positions.
	applyMovement();
	m_rotationY += m_turningSpeedY;
	m_rotationX += m_turningSpeedX;
	applyDecels();
	applyTerminals();
}

float* PositionClass::getPositionXPtr()
{
	return &m_positionX;
}

float* PositionClass::getPositionYPtr()
{
	return &m_positionY;
}

float* PositionClass::getPositionZPtr()
{
	return &m_positionZ;
}

void PositionClass::SetFootstepSound(SoundClass* sound)
{
	footstepSound = sound;
}

void PositionClass::updateFootstep()
{
	if (m_xMomentum > 0)
		m_distanceUntilFootstep -= m_frameTime * m_xMomentum;
	else
		m_distanceUntilFootstep += m_frameTime * m_xMomentum;

	if (m_zMomentum > 0)
		m_distanceUntilFootstep -= m_frameTime * m_zMomentum;
	else
		m_distanceUntilFootstep += m_frameTime * m_zMomentum;

	if (m_distanceUntilFootstep < 0)
	{
		m_distanceUntilFootstep = m_distanceBetweenFootsteps;
		footstepSound->playFor(1);
		//footstepSound->PlayWaveFile();
	}
}

void PositionClass::applyGravity()
{
	if (m_inAir)
		m_yMomentum -= m_gravityValue * m_frameTime;
}

void PositionClass::applyTerminals()
{
	if (m_yMomentum < -m_terminalYVelocity)
		m_yMomentum = -m_terminalYVelocity;
	if (m_yMomentum > m_terminalYVelocity)
		m_yMomentum = m_terminalYVelocity;
	if (m_xMomentum < -m_terminalXVelocity)
		m_xMomentum = -m_terminalXVelocity;
	if (m_xMomentum > m_terminalXVelocity)
		m_xMomentum = m_terminalXVelocity;
	if (m_zMomentum < -m_terminalZVelocity)
		m_zMomentum = -m_terminalZVelocity;
	if (m_zMomentum > m_terminalZVelocity)
		m_zMomentum = m_terminalZVelocity;
}

void PositionClass::applyMovement()
{
	if (m_sprinting)
	{
		m_positionX += m_xMomentum * m_sprintMultiplier;
		m_positionY += m_yMomentum * m_sprintMultiplier;
		m_positionZ += m_zMomentum * m_sprintMultiplier;

		if (m_xMomentum > 0)
			m_distanceUntilFootstep -= (m_sprintMultiplier - 1) * m_xMomentum;
		else
			m_distanceUntilFootstep += (m_sprintMultiplier - 1) * m_xMomentum;

		if (m_zMomentum > 0)
			m_distanceUntilFootstep -= (m_sprintMultiplier - 1) * m_zMomentum;
		else
			m_distanceUntilFootstep += (m_sprintMultiplier - 1) * m_zMomentum;
	}
	else
	{
		m_positionX += m_xMomentum;
		m_positionY += m_yMomentum;
		m_positionZ += m_zMomentum;
	}
	m_sprinting = false;
}
