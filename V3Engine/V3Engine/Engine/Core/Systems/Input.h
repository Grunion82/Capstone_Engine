#ifndef INPUT_H
#define INPUT_H

#include "EventSystem.h"
#include <SDL_joystick.h>
#include <SDL_gamecontroller.h>
#include <map>
#include <vector>

class Joystick : public EventSystem {
public:
	//For axis
	//Joy axis value
	std::map<unsigned int, int> joyAxis;
	//For axis direciton
	std::map<unsigned int, __int8> joyAxisDir;
	//Current joystick button state
	std::map<unsigned int, bool> joyButtons;
	//Old joystick button state
	std::map<unsigned int, bool> oldJoyButtons;

protected:
	//The actual joystick
	SDL_Joystick* joy;

	//Properties of the joystick

	//Type of joystick (ex.Game controller,Arcade pad)
	SDL_JoystickType type;

	//The name of the joystick
	const char* joystickName;
	//ID used to identify a joystick
	int id;
	//Number of hats
	int hats;
	//Numbder of axes
	int axes;
	//Number of buttons
	int buttons;
	//Number of balls
	int balls;
	//Used to flag for events regrading the controller
	int eventFlags;

public:
	Joystick();
	Joystick(SDL_Joystick* j);
	~Joystick();

	//virtual void Update();
	virtual void Update(SDL_Event& e) override;
	virtual bool Shutdown() override;

	inline int GetID() { return id; }
	inline int GetHats() { return hats; }
	inline int GetAxes() { return axes; }
	inline int GetButtons() { return buttons; }
	inline int GetBalls() { return balls; }
	inline SDL_JoystickType GetType() { return type; }
	inline SDL_Joystick* GetJoyStick() { return joy; }

	inline virtual int GetAxisValue(unsigned int axis) { return joyAxis[axis]; }
	inline virtual int GetAxisDir(unsigned int axis) { return joyAxisDir[axis]; }

	//JOYSTICK BUTTONS===============================================

	//Check if button is down (Button down)
	virtual bool IsButtonDown(unsigned int button);
	//Check if button is not down
	virtual bool IsButtonUp(unsigned int button);
	//Check if button was pressed this frame (Button pressed)
	virtual bool WasButtonPressed(unsigned int button);
	//Check if button was released this frame
	virtual bool WasButtonReleased(unsigned int button);

};

class GameController : public Joystick {
	//The actual game controller - has an inner SDL_Joystick
	SDL_GameController* gameController;

	//Mapping of the controller
	const char* mapping = nullptr;

	//Dead zone of the left stick
	int left_stick_dead_zone = 8000;
	//Dead zone of the right stick
	int right_stick_dead_zone = 8000;

public:
	GameController(SDL_GameController* gc, int left_stick_dead_zone, int right_stick_dead_zone);
	//GameController(SDL_GameController* gc,int left_stick_dead_zone, int right_stick_dead_zone);
	~GameController();

	//void Update() override;
	void Update(SDL_Event& e) override;
	bool Shutdown() override;

	inline SDL_GameController* GetGameController() { return gameController; }
	inline SDL_Joystick* GetGameControllerJoystick() { SDL_GameControllerGetJoystick(gameController); }

	inline int GetLeftStickDeadZone() { return left_stick_dead_zone; }
	inline int GetRightStickDeadZone() { return right_stick_dead_zone; }

	void SetLeftStickDeadZone(unsigned int value) { left_stick_dead_zone = value; }
	void SetRightStickDeadZone(unsigned int value) { right_stick_dead_zone = value; }

	//inline int GetAxisValue(unsigned int axis) override { return joyAxis[SDL_GameControllerGetBindForAxis(gameController,(SDL_GameControllerAxis)axis).value.axis]; }
	//inline int GetAxisDir(unsigned int axis) override { return joyAxisDir[SDL_GameControllerGetBindForAxis(gameController,(SDL_GameControllerAxis)axis).value.axis]; }

	//Check if button is down (Button down)
	bool IsButtonDown(unsigned int button) override;
	//Check if button is not down
	bool IsButtonUp(unsigned int button) override;
	//Check if button was pressed this frame (Button pressed)
	bool WasButtonPressed(unsigned int button) override;
	//Check if button was released this frame
	bool WasButtonReleased(unsigned int button) override;

	//Rebind a gamecontroller button/axis to another value
	void Rebind();
	void Rebind(SDL_GameControllerAxis axis, SDL_GameControllerAxis axis1);
	void Rebind(SDL_GameControllerAxis axis, SDL_GameControllerButton button);
	void Rebind(SDL_GameControllerButton button, SDL_GameControllerButton button1);
	void Rebind(SDL_GameControllerButton button, SDL_GameControllerAxis axis);
};

class Input : public EventSystem
{	
	//Make private for singleton
	Input();
	~Input();

	//Array of joysticks 
	std::vector<Joystick*> joysticks;

	//Current key state
	std::map<unsigned int, bool> keys;
	//Old key state
	std::map<unsigned int, bool> oldkeys;
	//Old mouse butt
	std::map<unsigned int, bool>oldMouseButtons;
	//Current mouse button
	std::map<unsigned int, bool>mouseButtons;

	static Input* instance;


	//Mouse motion in X direction(left and right)
	int mouseMotionX;
	//Mouse motion in Y direction(up and down)
	int mouseMotionY;
	//Mouse wheel 
	int mouseWheelY;
	//Mouse button that was pressed
	int mouseButtonPress;
	//Used to check for events regarding inputs such as keyboard and mouse
	int eventFlags;
	//For number of click
	unsigned int clicks;

	//Currently used to quit the main game loop
	bool requestedQuit;

public:

	//Get instance for singleton
	static Input* GetInstance();

	bool Init();
	//"Reset" values
	void Update();
	void Update(SDL_Event& e) override;
	void UpdateJoysticks(SDL_Event &e);
	bool Shutdown();
	//SystemType getSystemType();
	bool QuitRequested() { return requestedQuit; }

	//Joystick* GetJoystick(unsigned int which);

	inline std::vector<Joystick*> GetJoysticks() { return joysticks; }
	inline Joystick* GetJoystick(unsigned int index) { return joysticks[index]; }
	int GetJoystickAxis(unsigned int index, unsigned int axis);
	int GetJoystickAxisDir(unsigned int index, unsigned int axis);
	int GetJoystickButton(unsigned int index, unsigned int button);

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

