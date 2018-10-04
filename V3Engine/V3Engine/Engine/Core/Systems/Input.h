#ifndef INPUT_H
#define INPUT_H

#include <SDL_joystick.h>
#include <map>

enum class DirectionInput: unsigned __int8 {
	UP,
	DOWN,
	LEFT,
	RIGHT,
	NONE,
	TOTAL
};

class Input
{
	const int JOYSTICK_DEAD_ZONE = 8000;

	SDL_Joystick* joy;
	bool requestedQuit;
	DirectionInput dr;

	const int JOYSTICK_DEAD_ZONE = 8000;
	SDL_Joystick* joy;

	//Map is a dictionary that has a key and a value
	//Current key state
	std::map<unsigned int, bool> keys;
	//Old key state
	std::map<unsigned int, bool> oldkeys;
	//Current joystick button state
	std::map<unsigned int, bool> joyButtons;
	//Old joystick button state
	std::map<unsigned int, bool> oldJoyButtons;

	//Mouse motion
	int mouseMotionX;
	int mouseMotionY;
	//Mouse wheel 
	int mouseWheelY;
	int mouseButtonPress;
	//joystick left axis
	int joystickLeftAxisX;
	int joystickLeftAxisY;
	//joystick right axis
	int joystickRightAxisX;
	int joystickRightAxisY;
	//Mouse button press
	int mouseButtonLeftPress;
	int mouseButtonRightPress;
	int mouseButtonMiddlePress;
	//Joystick left axis
	int joystickLeftAxisX;
	int joystickLeftAxisY;
	//Joystick right axis
	int joystickRightAxisX;
	int joystickRightAxisY;
	//Joystick triggers
	int joystickLeftTrigger;
	int joystickRightTrigger;

public:
	Input();
	~Input();

	bool Init();
	void Update();
	void Render();
	bool Shutdown();
	//SystemType getSystemType();
	bool QuitRequested() { return requestedQuit; }
	DirectionInput getDirectionInput();
	//Check if key is down (Key down)
	bool IsKeyDown(unsigned int key);
	//Check if key is not down
	bool IsKeyUp(unsigned int key);
	//Check if the key was pressed this frame (Key pressed)
	bool WasKeyPressed(unsigned int key);
	//Check if the the was released this frame
	bool WasKeyReleased(unsigned int key);
	//Check if button is down (Button down)
	bool IsJoyStickButtonDown(unsigned int key);
	//Check if button is not down
	bool IsJoyStickButtonUp(unsigned int key);
	//Check if key was pressed this frame (Button pressed)
	bool WasJoyStickButtonPressed(unsigned int key);
	//Check if key was released this frame
	bool WasJoyStickButtonReleased(unsigned int key);

	//Get mouse motion in X direction
	inline int GetMouseMotionX() { return mouseMotionX; }
	//Get mouse motion in Y direction
	inline int GetMouseMotionY() { return mouseMotionY; }
	//Get mouse wheel y
	inline int GetMouseWheelY() { return mouseWheelY; }

	//Get joystick axis in X direction
	inline int GetJoyStickLeftAxisX() { return joystickLeftAxisX; }
	//Get joystick axis in Y direction
	inline int GetJoyStickLeftAxisY() { return joystickLeftAxisY; }
	//Get joystick axis in X direction
	inline int GetJoyStickRightAxisX() { return joystickRightAxisX; }
	//Get joystick axis in Y direction
	inline int GetJoyStickRightAxisY() { return joystickRightAxisY; }
	//Get joystick left trigger
	inline int GetJoyStickLeftTrigger() { return joystickLeftTrigger; }
	//Get joystick right trigger
	inline int GetJoyStickRightTrigger() { return joystickRightTrigger; }
};

#endif // !INPUT_H

