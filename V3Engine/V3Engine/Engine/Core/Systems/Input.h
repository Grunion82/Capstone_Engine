#ifndef INPUT_H
#define INPUT_H

#include "EventSystem.h"
#include <SDL_joystick.h>
#include <SDL_gamecontroller.h>
#include <map>
#include <vector>

class Joystick {
public:
	//For axis
	//Joy axis value
	std::map<unsigned int, int> joyAxis;
	//For axis direciton
	std::map<unsigned int, short> joyAxisDir;
	//Current joystick button state
	std::map<unsigned int, bool> joyButtons;
	//Old joystick button state
	std::map<unsigned int, bool> oldJoyButtons;

private:
	//The actual joystick
	SDL_Joystick* joy;

protected:
	SDL_JoystickType type;
	int id;
	int hats;
	int axes;
	int buttons;
	int balls;
	int eventFlags;

	//Dead zone of the left stick
	int left_stick_dead_zone = 8000;
	//Dead zone of the right stick
	int right_stick_dead_zone = 8000;

public:

	Joystick(SDL_Joystick* j, int left_stick_dead_zone, int right_stick_dead_zone);
	~Joystick();

	virtual void Update(SDL_Event& e);
	virtual void Shutdown();

	//GETTERS

	inline int GetLeftStickDeadZone() { return left_stick_dead_zone; }
	inline int GetRightStickDeadZone() { return right_stick_dead_zone; }

	inline int GetID() { return id; }
	inline int GetHats() { return hats; }
	inline int GetAxes() { return axes; }
	inline int GetButtons() { return buttons; }
	inline int GetBalls() { return balls; }
	inline SDL_JoystickType GetType() { return type; }
	inline SDL_Joystick* GetJoyStick() { return joy; }

	inline int GetAxisValue(unsigned int axis) { return joyAxis[axis]; }
	inline int GetAxisDir(unsigned int axis) { return joyAxisDir[axis]; }

	//JOYSTICK BUTTONS===============================================
	//Check if button is down (Button down)
	bool IsJoyStickButtonDown(unsigned int key);
	//Check if button is not down
	bool IsJoyStickButtonUp(unsigned int key);
	//Check if button was pressed this frame (Button pressed)
	bool WasJoyStickButtonPressed(unsigned int key);
	//Check if button was released this frame
	bool WasJoyStickButtonReleased(unsigned int key);

	//SETTERS

	void SetLeftStickDeadZone(unsigned int value) { left_stick_dead_zone = value; }
	void SetRightStickDeadZone(unsigned int value) { right_stick_dead_zone = value; }
};

class GameController : public Joystick {
	SDL_GameController* gameController;
	char* mapping = nullptr;

public:
	GameController(SDL_Joystick* j, int left_stick_dead_zone, int right_stick_dead_zone);
	//GameController(SDL_GameController* gc,int left_stick_dead_zone, int right_stick_dead_zone);
	~GameController();

	void Update(SDL_Event& e) override;
	void Shutdown() override;

	inline SDL_GameController* GetGameController() { return gameController; }
	inline SDL_Joystick* GetGameControllerJoystick() { SDL_GameControllerGetJoystick(gameController); }

	void RebindButton();
};

class Input : public EventSystem
{	
	//Make private for singleton
	Input();
	~Input();

	std::vector<Joystick*> joysticks;

	//Map is a dictionary that has a key and a value
	//Current key state
	std::map<unsigned int, bool> keys;
	//Old key state
	std::map<unsigned int, bool> oldkeys;
	//Old mouse butt
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
	//For number of click
	unsigned int clicks;

	bool requestedQuit;

	int eventFlags;
public:

	//Get instance for singleton
	static Input* GetInstance();

	bool Init();
	void Update(SDL_Event& e) override;
	bool Shutdown();
	//SystemType getSystemType();
	bool QuitRequested() { return requestedQuit; }

	//Joystick* GetJoystick(unsigned int which);

	inline std::vector<Joystick*> GetJoysticks() { return joysticks; }

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

