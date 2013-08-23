#include "os_x360controller.hpp"

#include "h_config.h"

#ifdef __MINGW32__
// Useless MS defines (needed to compile under MinGW)
#define __in
#define __out
#define __reserved
#endif


// This define makes your program compile faster by excluding things we are not using
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <XInput.h>


namespace xb {

	// Returns true if the joystick is connected (and is an XBox 360 Controller)
	bool Joystick::isConnected (t_joyNum joyNum)
	{

		XINPUT_STATE state;
		ZeroMemory (&state, sizeof (XINPUT_STATE));

		auto result = XInputGetState (joyNum, &state);
		return  (result == ERROR_SUCCESS);

	}

	// Convenience function: Returns true if there is at least one X360 controller connected
	bool Joystick::isAnyXBox360ControllerConnected()
	{

		return  (isConnected(0) || isConnected(1) || isConnected(2) || isConnected(3));

	}

	/*
	// Returns true if the device supports audio capabilities (headset)
	// Uncomment if you need this function
	bool Joystick::voiceSupported (t_joyNum joyNum)
	{

	XINPUT_CAPABILITIES caps;
	ZeroMemory (&caps, sizeof (XINPUT_CAPABILITIES));

	auto result = XInputGetCapabilities (joyNum, XINPUT_FLAG_GAMEPAD, &caps);

	if (result != ERROR_SUCCESS)
	return false;

	return  (caps.Flags & XINPUT_CAPS_VOICE_SUPPORTED);

	}
	*/

	// Returns true if the specified button is pressed
	// Note that the triggers are NOT recognized as buttons.. You must use
	// the getTriggers function for reading the triggers state
	bool Joystick::isButtonPressed (t_joyNum joyNum, t_buttonNum buttonNum)
	{

		XINPUT_STATE state;
		ZeroMemory (&state, sizeof (XINPUT_STATE));

		XInputGetState (joyNum, &state);
		return  (state.Gamepad.wButtons & buttonNum);

	}

	// This function returns nothing
	// It fills the variables left and right with the current state of the triggers (LT and RT)
	// The values will always be in the range 0..1
	// TODO: TAKE CARE OF THE DEAD ZONE ??????????????????????????????????
	void Joystick::getTriggers (t_joyNum joyNum, float &left, float &right)
	{

		XINPUT_STATE state;
		ZeroMemory (&state, sizeof (XINPUT_STATE));

		XInputGetState (joyNum, &state);

		// Normalize and take care of the Dead Zone
		left  = static_cast <float> (state.Gamepad.bLeftTrigger)  / 255;
		right = static_cast <float> (state.Gamepad.bRightTrigger) / 255;

	}

int Joystick::getAxisDir()
	{
		sf::Vector2f ls, rs;
		xb::Joystick::getSticksPosition(0, ls, rs);

		int jDir = DIR_IDLE;

		float deltaX =  0-ls.x;
		float deltaY = 0-ls.y;		

		float angle = atan(deltaY / deltaX) * 180 / PI;
		angle = atan2(deltaY, deltaX) * 180 / PI;

		if (angle <= -45 && angle >= -140)
		{
			jDir = DIR_UP;
		}
		if (angle >= 45 && angle <= 140)
		{
			jDir = DIR_DOWN;
		}
		if ((angle > 140 && angle <= 180) || (angle >= -180 && angle <= -140) && angle != 0)
		{
			jDir = DIR_RIGHT;
		}
		if ((angle >= -40 && angle < 0) || (angle > 0 && angle <= 40) && angle != 0)
		{
			jDir = DIR_LEFT;
	}
	return jDir;
	
}

sf::Vector2f Joystick::sfJoystick_normaliseDirectionProportional(const sf::Vector2f& source)
{
	float length = sqrt((source.x * source.x) + (source.y * source.y));
	if (length != 0)
		return sf::Vector2f(source.x / length, source.y / length);
	else
		return source;
}

// This function returns nothing
// It fills the vectors left and right with the stick positions,
// wich are in the range -100..100, similar to the SFML function
// getAxisPosition
void Joystick::getSticksPosition (t_joyNum joyNum, sf::Vector2f &left, sf::Vector2f &right)
{

	XINPUT_STATE state;
	ZeroMemory (&state, sizeof (XINPUT_STATE));

	XInputGetState (joyNum, &state);

	// Check for the "DEAD ZONE"
	// Left Stick
	if ( (state.Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		state.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
		(state.Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		state.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) ) {

			state.Gamepad.sThumbLX = 0;
			state.Gamepad.sThumbLY = 0;

	}

	// Right Stick
	if ( (state.Gamepad.sThumbRX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		state.Gamepad.sThumbRX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
		(state.Gamepad.sThumbRY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		state.Gamepad.sThumbRY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) ) {

			state.Gamepad.sThumbRX = 0;
			state.Gamepad.sThumbRY = 0;

	}

	// Convert values to SFML style (-100..100)
	left.x  = static_cast <float> (state.Gamepad.sThumbLX / 327);
	left.y  = static_cast <float> (state.Gamepad.sThumbLY / 327);
	right.x = static_cast <float> (state.Gamepad.sThumbRX / 327);
	right.y = static_cast <float> (state.Gamepad.sThumbRY / 327);

}

// Set vibration (0.0 to 1.0)
// 0 stops the vibration
void Joystick::setVibration (t_joyNum joyNum, float leftMotor, float rightMotor)
{

	XINPUT_VIBRATION vib;
	ZeroMemory (&vib, sizeof (XINPUT_VIBRATION));

	vib.wLeftMotorSpeed  = static_cast <WORD> (leftMotor  * 65535.0f);
	vib.wRightMotorSpeed = static_cast <WORD> (rightMotor * 65535.0f);

	XInputSetState (joyNum, &vib);

}

} // ns