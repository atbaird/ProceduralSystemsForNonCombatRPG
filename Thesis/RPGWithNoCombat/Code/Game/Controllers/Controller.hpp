#pragma once

#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP
#include "Engine/Core/Input.hpp"

class Controller
{
private:
protected:
	//Keys
	unsigned char m_UpKey = 'W';
	unsigned char m_DownKey = 'S';
	unsigned char m_LeftKey = 'A';
	unsigned char m_RightKey = 'D';
	unsigned char m_PauseKey = VK_ESCAPE;
	unsigned char m_Action1Key = VK_SPACE;
	unsigned char m_SprintKey = VK_SHIFT;

	//Booleans
	bool m_LockControls = false;
	bool m_UpKeyIsDown = false;
	bool m_DownKeyIsDown = false;
	bool m_LeftKeyIsDown = false;
	bool m_RightKeyIsDown = false;
	bool m_PauseKeyIsDown = false;
	bool m_Action1KeyIsDown = false;
	bool m_SprintKeyIsDown = false;

	//Values
	Vector2 m_MovementVector = Vector2::vec2_zeros;

public:
	//Constructors
	Controller();
	virtual ~Controller();

	//Updates
	void ControllerUpdate();
	void SetLockControls(bool lockControls);
protected:
	void UpdateBooleans(Vector2& xboxLAnalog);
	void UpdateMovementVector(Vector2& xboxLAnalog);

public:
	//Setters
	void SetUpKey(unsigned char key);
	void SetDownKey(unsigned char key);
	void SetLeftKey(unsigned char key);
	void SetRightKey(unsigned char key);
	void SetPauseKey(unsigned char key);
	void SetAction1Key(unsigned char key);
	void SetSprintKey(unsigned char key);

	//Getters
	const unsigned char& GetUpKey() const;
	const unsigned char& GetDownKey() const;
	const unsigned char& GetLeftKey() const;
	const unsigned char& GetRightKey() const;
	const unsigned char& GetPauseKey() const;
	const unsigned char& GetAction1Key() const;
	const unsigned char& GetSprintKey() const;
	const bool& GetIsDirUpDown() const;
	const bool& GetIsDirDownDown() const;
	const bool& GetIsDirLeftDown() const;
	const bool& GetIsDirRightDown() const;
	const bool& GetIsPauseKeyDown() const;
	const bool& GetIsAction1Down() const;
	const bool& GetIsSprintDown() const;
	const Vector2& GetMovementVector() const;
};
#endif