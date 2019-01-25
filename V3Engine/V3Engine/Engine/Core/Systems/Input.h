#ifndef INPUT_H
#define INPUT_H

#include <SDL_joystick.h>
#include <map>
#include <vector>

class Joystick {
	//The actual joystick
	SDL_Joystick* joy;
public:
	Joystick(SDL_Joystick* j, int deadzone);
	~Joystick();
	
	//Current joystick button state
	std::map<unsigned int, bool> joyButtons;
	//Old joystick button state
	std::map<unsigned int, bool> oldJoyButtons;

	//Joystick left axis
	int joystickLeftAxisX;
	int joystickLeftAxisY;
	//Joystick right axis
	int joystickRightAxisX;
	int joystickRightAxisY;
	//Joystick triggers
	int joystickLeftTrigger;
	int joystickRightTrigger;
	//Joystick axis
	int leftXAxis;
	int leftYAxis;
	int rightXAxis;
	int rightYAxis;
	int leftTrigger;
	int rightTrigger;

	//Dead zone of the joystick
	const int JOYSTICK_DEAD_ZONE = 8000;

	//JOYSTICK AXIS===================================================

	//Get joystick axis in X direction
	inline int GetJoyStickLeftAxisX() { return joystickLeftAxisX; }
	//Get joystick axis in Y direction
	inline int GetJoyStickLeftAxisY() { return joystickLeftAxisY; }
	//Get joystick axis in X direction
	inline int GetJoyStickRightAxisX() { return joystickRightAxisX; }
	//Get joystick axis in Y direction
	inline int GetJoyStickRightAxisY() { return joystickRightAxisY; }
	//Get joystick left trigger value
	inline int GetJoyStickLeftTrigger() { return joystickLeftTrigger; }
	//Get joystick right trigger value
	inline int GetJoyStickRightTrigger() { return joystickRightTrigger; }
	//Get left X axis value
	inline int GetLeftXAxis() { return leftXAxis; }
	//Get left Y axis value
	inline int GetLeftYAxis() { return leftYAxis; }
	//Get right X axis value
	inline int GetRightXAxis() { return rightXAxis; }
	//Get right Y axis value
	inline int GetRightYAxis() { return rightYAxis; }
	//Get left trigger value 
	inline int GetLeftTrigger() { return leftTrigger; }
	//Get right trigger value
	inline int GetRightTrigger() { return rightTrigger; }

	//JOYSTICK PROPERTIES
	const int id;
	const int hats;
	const int axes;
	const int buttons;
	const int balls;

	inline SDL_Joystick* GetJoyStick() { return joy; }

	//JOYSTICK BUTTONS===============================================
	//Check if button is down (Button down)
	bool IsJoyStickButtonDown(unsigned int key);
	//Check if button is not down
	bool IsJoyStickButtonUp(unsigned int key);
	//Check if button was pressed this frame (Button pressed)
	bool WasJoyStickButtonPressed(unsigned int key);
	//Check if button was released this frame
	bool WasJoyStickButtonReleased(unsigned int key);
};

class Input
{	
	//Make private for singleton
	Input();
	~Input();

	std::vector<Joystick> joysticks;

	//Map is a dictionary that has a key and a value
	//Current key state
	std::map<unsigned int, bool> keys;
	//Old key state
	std::map<unsigned int, bool> oldkeys;
	//Old mouse button
	std::map<unsigned int, bool>oldMouseButtons;
	//Current mouse button
	std::map<unsigned int, bool>mouseButtons;

	static Input* instance;

	//Mouse motion
	int mouseMotionX;
	int mouseMotionY;
	//Mouse wheel 
	int mouseWheelY;
	int mouseButtonPress;
	//Mouse button press
	int mouseButtonLeftPress;
	int mouseButtonRightPress;
	int mouseButtonMiddlePress;

	//For number of click
	unsigned int clicks;
	bool requestedQuit;
public:
	//Get instance for singleton
	static Input* GetInstance();

	bool Init();
	void Update();
	void Render();
	bool Shutdown();
	//SystemType getSystemType();
	bool QuitRequested() { return requestedQuit; }

	Joystick* GetJoystick(unsigned int which);

	//KEYBOARD=======================================================

	//Check if key is down (Key down)
	bool IsKeyDown(unsigned int key);
	//Check if key is not down
	bool IsKeyUp(unsigned int key);
	//Check if the key was pressed this frame (Key pressed)
	bool WasKeyPressed(unsigned int key);
	//Check if the the was released this frame
	bool WasKeyReleased(unsigned int key);

	//MOUSE MOTION====================================================

	//Get mouse motion in X direction
	inline int GetMouseMotionX() { return mouseMotionX; }
	//Get mouse motion in Y direction
	inline int GetMouseMotionY() { return mouseMotionY; }
	//Get mouse wheel y
	inline int GetMouseWheelY() { return mouseWheelY; }

	//MOUSE BUTTONS====================================================

	//Check if mouse button up
	bool IsMouseButtonUp(unsigned int key);
	//Check if mouse button down
	bool IsMouseButtonDown(unsigned int key);
	//Check if mouse button pressed
	bool WasMouseButtonPressed(unsigned int key);
	//Check if mouse button released
	bool WasMouseButtonReleased(unsigned int key);
	//Get number of clicks
	inline unsigned int GetMouseClicks() { return clicks; }
};
#endif // !INPUT_H

