#include "Game/Controllers/Controller.hpp"


//------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------
//Constructors
Controller::Controller()
{

}
Controller::~Controller()
{

}

//------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------
//Updates
void Controller::ControllerUpdate()
{
	if (m_LockControls)
	{
		m_LockControls = false;
		m_UpKeyIsDown = false;
		m_DownKeyIsDown = false;
		m_LeftKeyIsDown = false;
		m_RightKeyIsDown = false;
		m_PauseKeyIsDown = false;
		m_Action1KeyIsDown = false;
		m_SprintKeyIsDown = false;
		m_MovementVector = Vector2::vec2_zeros;
		return;
	}
	//Get XboxController Direction input
	Vector2 xboxLAnalog = g_input->getLeftAnalogPosition(0);
	if (xboxLAnalog.y == 0.f)
	{
		bool pushUp = g_input->isDPadUpDown(0);
		bool pushDown = g_input->isDPadDownDown(0);
		if (pushUp && pushDown == false)
		{
			xboxLAnalog.y = 1.f;
		}
		else if (pushDown && pushUp == false)
		{
			xboxLAnalog.y = -1.f;
		}
	}

	if (xboxLAnalog.x == 0.f)
	{
		bool pushRight = g_input->isDPadRightDown(0);
		bool pushLeft = g_input->isDPadLeftDown(0);
		if (pushRight && pushLeft == false)
		{
			xboxLAnalog.x = 1.f;
		}
		else if (pushLeft && pushRight == false)
		{
			xboxLAnalog.x = -1.f;
		}
	}

	UpdateBooleans(xboxLAnalog);
	UpdateMovementVector(xboxLAnalog);
}

void Controller::SetLockControls(bool lockControls)
{
	m_LockControls = lockControls;
}


void Controller::UpdateBooleans(Vector2& xboxLAnalog)
{
	//Read boolean info.
	if (g_input->IsKeyDown(m_UpKey) || xboxLAnalog.y > 0.f)
	{
		m_UpKeyIsDown = true;
	}
	else
	{
		m_UpKeyIsDown = false;
	}

	if (g_input->IsKeyDown(m_DownKey) || xboxLAnalog.y < 0.f)
	{
		m_DownKeyIsDown = true;
	}
	else
	{
		m_DownKeyIsDown = false;
	}

	if (g_input->IsKeyDown(m_LeftKey) || xboxLAnalog.x < 0.f)
	{
		m_LeftKeyIsDown = true;
	}
	else
	{
		m_LeftKeyIsDown = false;
	}

	if (g_input->IsKeyDown(m_RightKey) || xboxLAnalog.x > 0.f)
	{
		m_RightKeyIsDown = true;
	}
	else
	{
		m_RightKeyIsDown = false;
	}

	if (g_input->IsKeyDown(m_Action1Key) || g_input->isAButtonDown(0))
	{
		m_Action1KeyIsDown = true;
	}
	else
	{
		m_Action1KeyIsDown = false;
	}

	if (g_input->IsKeyDown(m_PauseKey) || g_input->isStartButtonDown(0))
	{
		m_PauseKeyIsDown = true;
	}
	else
	{
		m_PauseKeyIsDown = false;
	}

	if (g_input->IsKeyDown(m_SprintKey) || g_input->isBButtonDown(0))
	{
		m_SprintKeyIsDown = true;
	}
	else
	{
		m_SprintKeyIsDown = false;
	}
}

void Controller::UpdateMovementVector(Vector2& xboxLAnalog)
{
	//Update MovementVector
	m_MovementVector = Vector2::vec2_zeros;
	if (m_UpKeyIsDown && m_DownKeyIsDown == false)
	{
		m_MovementVector.y = 1.f;
		if (xboxLAnalog.y > 0.f)
		{
			m_MovementVector.y = xboxLAnalog.y;
		}
	}
	else if (m_UpKeyIsDown == false && m_DownKeyIsDown)
	{
		m_MovementVector.y = -1.f;
		if (xboxLAnalog.y < 0.f)
		{
			m_MovementVector.y = xboxLAnalog.y;
		}
	}

	if (m_RightKeyIsDown && m_LeftKeyIsDown == false)
	{
		m_MovementVector.x = 1.f;
		if (xboxLAnalog.x > 0.f)
		{
			m_MovementVector.x = xboxLAnalog.x;
		}
	}
	else if (m_RightKeyIsDown == false && m_LeftKeyIsDown)
	{
		m_MovementVector.x = -1.f;
		if (xboxLAnalog.x < 0.f)
		{
			m_MovementVector.x = xboxLAnalog.x;
		}
	}
}

//------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------
//Setters
void Controller::SetUpKey(unsigned char key)
{
	m_UpKey = key;
}

void Controller::SetDownKey(unsigned char key)
{
	m_DownKey = key;
}

void Controller::SetLeftKey(unsigned char key)
{
	m_LeftKey = key;
}

void Controller::SetRightKey(unsigned char key)
{
	m_RightKey = key;
}

void Controller::SetPauseKey(unsigned char key)
{
	m_PauseKey = key;
}

void Controller::SetAction1Key(unsigned char key)
{
	m_Action1Key = key;
}

void Controller::SetSprintKey(unsigned char key)
{
	m_SprintKey = key;
}

//------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------
//Getters
const unsigned char& Controller::GetUpKey() const
{
	return m_UpKey;
}

const unsigned char& Controller::GetDownKey() const
{
	return m_DownKey;
}

const unsigned char& Controller::GetLeftKey() const
{
	return m_LeftKey;
}

const unsigned char& Controller::GetRightKey() const
{
	return m_RightKey;
}

const unsigned char& Controller::GetPauseKey() const
{
	return m_PauseKey;
}

const unsigned char& Controller::GetAction1Key() const
{
	return m_Action1Key;
}

const unsigned char& Controller::GetSprintKey() const
{
	return m_SprintKey;
}

const bool& Controller::GetIsDirUpDown() const
{
	return m_UpKeyIsDown;
}

const bool& Controller::GetIsDirDownDown() const
{
	return m_DownKeyIsDown;
}

const bool& Controller::GetIsDirLeftDown() const
{
	return m_LeftKeyIsDown;
}

const bool& Controller::GetIsDirRightDown() const
{
	return m_RightKeyIsDown;
}

const bool& Controller::GetIsPauseKeyDown() const
{
	return m_PauseKeyIsDown;
}

const bool& Controller::GetIsAction1Down() const
{
	return m_Action1KeyIsDown;
}

const bool& Controller::GetIsSprintDown() const
{
	return m_SprintKeyIsDown;
}

const Vector2& Controller::GetMovementVector() const
{
	return m_MovementVector;
}