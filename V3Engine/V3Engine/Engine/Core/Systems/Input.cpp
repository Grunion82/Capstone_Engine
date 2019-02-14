#include "Input.h"
#include <SDL.h>
#include <stdio.h>

#include <iostream>

Input* Input::instance;

Input::Input() : requestedQuit(false)
{
}


Input * Input::GetInstance()
{
	if (instance) {
		return instance;
	}
	else {
		instance = new Input();
		return instance;
	}
}

Input::~Input()
{
	Input::Shutdown();
}

bool Input::Init() {
	SDL_InitSubSystem(SDL_INIT_JOYSTICK);

	if (SDL_NumJoysticks() > 0) {
		joysticks.reserve(SDL_NumJoysticks());
		//Check for joysticks
		for (int i = 0; i < SDL_NumJoysticks(); i++) {
			//SDL_Joystick* j = SDL_JoystickOpen(i);

			//joysticks.push_back(Joystick(j,8000,8000));
			//joysticks.insert(joysticks.begin() + i, Joystick(j, 8000));

			std::cout << "Opened joystick: " << i << std::endl;
			std::cout << "Name: " << SDL_JoystickNameForIndex(i) << std::endl;
			std::cout << "Id: " << SDL_JoystickGetDeviceInstanceID(i) << std::endl;
			std::cout << "Type: " << SDL_JoystickGetDeviceType(i) << std::endl;
			std::cout << "Number of mapping: " << SDL_GameControllerNumMappings() << std::endl;

			int type = SDL_JoystickGetDeviceType(i);

			SDL_Joystick* j = SDL_JoystickOpen(i);

			if (j) {
				switch (type)
				{
				//Unknown - assume joystick
				case(0):
					joysticks.push_back(Joystick(j, 8000, 8000));
					break;
				//Game controller
				case(1):
					joysticks.push_back(GameController(j, 8000, 8000));
					//SDL_JoystickClose(j);

					break;
				default:
					break;
				}

				//Check if joysticks are pointing in a direction
				Sint16 state;
				for (unsigned int a = 0; a < SDL_JoystickNumAxes(j); a++) {
					//If there is a change for joystick during init
					if (SDL_JoystickGetAxisInitialState(j, a, &state)) {
						joysticks.at(i).joyAxis[a] = state;

						if (state > 0) {
							joysticks.at(i).joyAxisDir[a] = 1;
						}
						else if (state < 0) {
							joysticks.at(i).joyAxisDir[a] = -1;
						}
						else {
							joysticks.at(i).joyAxisDir[a] = 0;
						}
					}
				}
			}
			else {
				std::cout << "Failed to open " << i << std::endl;
				std::cout << SDL_GetError() << std::endl;
			}

			std::cout << std::endl;
		}
	}

	return true;
}
void Input::Update(SDL_Event& e) {
	oldkeys = keys;
	oldMouseButtons = mouseButtons;

	for (unsigned int i = 0; i < joysticks.size(); i++){
		joysticks.at(i).oldJoyButtons = joysticks.at(i).joyButtons;
	}

	mouseMotionX = 0;
	mouseMotionY = 0;
	mouseWheelY = 0;

	while (SDL_PollEvent(&e) != 0) {
		//Check for event type
		switch (e.type) {
		//KEYBOARD========================
		//Is key down
		case(SDL_KEYDOWN):
			//Check for key
			//oldkeys = keys;
			keys[e.key.keysym.sym] = true;
			break;
		//Is key up
		case(SDL_KEYUP):
			//oldkeys = keys;
			keys[e.key.keysym.sym] = false;
			break;
		//MOUSE==============================
		//For mouse movement
		case(SDL_MOUSEMOTION):
			mouseMotionX = e.motion.xrel;
			mouseMotionY = e.motion.yrel;
			break;
		//For mouse button down
		case(SDL_MOUSEBUTTONDOWN):
			//1 - Left click
			//2 - Middle click
			//3 - Right click
			//4 - Back button
			//5 - Front button
			mouseButtons[e.button.button] = true;
			clicks = e.button.clicks;
			break;
			//For mouse button up
		case(SDL_MOUSEBUTTONUP):
			mouseButtons[e.button.button] = false;
			break;
		//For mouse wheel
		case(SDL_MOUSEWHEEL):
			mouseWheelY = e.wheel.y;
			break;
		//JOYSTICK=================================
		//For controller axis
		case(SDL_JOYAXISMOTION):
			if (joysticks.at(e.jaxis.which).GetType() != SDL_JOYSTICK_TYPE_GAMECONTROLLER) {
				//Value for axis ranges form -32768 to 32767
				//Set axis values
				joysticks.at(e.jaxis.which).joyAxis[e.jaxis.axis] = e.jaxis.value;

				if (e.jaxis.value > 0) {
					joysticks.at(e.jaxis.which).joyAxisDir[e.jaxis.axis] = 1;
				}
				else if (e.jaxis.value < 0) {
					joysticks.at(e.jaxis.which).joyAxisDir[e.jaxis.axis] = -1;
				}
				else {
					joysticks.at(e.jaxis.which).joyAxisDir[e.jaxis.axis] = 0;
				}
			}
			break;
		case(SDL_JOYBUTTONDOWN):
			if (joysticks.at(e.jbutton.which).GetType() != SDL_JOYSTICK_TYPE_GAMECONTROLLER) {
				joysticks.at(e.jbutton.which).joyButtons[e.jbutton.button] = true;
			}
			break;
		case(SDL_JOYBUTTONUP):
			if (joysticks.at(e.jbutton.which).GetType() != SDL_JOYSTICK_TYPE_GAMECONTROLLER) {
				joysticks.at(e.jbutton.which).joyButtons[e.jbutton.button] = false;
			}
			break;

		//CONTROLLER=============================================
		case(SDL_CONTROLLERAXISMOTION):
			joysticks.at(e.caxis.which).joyAxis[e.caxis.axis] = e.caxis.value;

			switch (e.caxis.axis)
			{
			//Left X axis
			case(SDL_CONTROLLER_AXIS_LEFTX):
				if (e.jaxis.value > joysticks.at(e.caxis.which).GetLeftStickDeadZone()) {
					joysticks.at(e.caxis.which).joyAxisDir[e.caxis.axis] = 1;
				}
				else if (e.jaxis.value < -joysticks.at(e.caxis.which).GetLeftStickDeadZone()) {
					joysticks.at(e.caxis.which).joyAxisDir[e.caxis.axis] = -1;
				}
				else {
					joysticks.at(e.caxis.which).joyAxisDir[e.caxis.axis] = 0;
				}
				break;
			//Left Y axis
			case(SDL_CONTROLLER_AXIS_LEFTY):
				if (e.jaxis.value > joysticks.at(e.caxis.which).GetLeftStickDeadZone()) {
					joysticks.at(e.caxis.which).joyAxisDir[e.caxis.axis] = 1;
				}
				else if (e.jaxis.value < -joysticks.at(e.caxis.which).GetLeftStickDeadZone()) {
					joysticks.at(e.caxis.which).joyAxisDir[e.caxis.axis] = -1;
				}
				else {
					joysticks.at(e.caxis.which).joyAxisDir[e.caxis.axis] = 0;
				}
				break;
			//Right X axis
			case(SDL_CONTROLLER_AXIS_RIGHTX):
				if (e.jaxis.value > joysticks.at(e.caxis.which).GetRightStickDeadZone()) {
					joysticks.at(e.caxis.which).joyAxisDir[e.caxis.axis] = 1;
				}
				else if (e.jaxis.value < -joysticks.at(e.caxis.which).GetRightStickDeadZone()) {
					joysticks.at(e.caxis.which).joyAxisDir[e.caxis.axis] = -1;
				}
				else {
					joysticks.at(e.caxis.which).joyAxisDir[e.caxis.axis] = 0;
				}
				break;				 
			//Right Y axis			 
			case(SDL_CONTROLLER_AXIS_RIGHTY):
				if (e.jaxis.value > joysticks.at(e.caxis.which).GetRightStickDeadZone()) {
					joysticks.at(e.caxis.which).joyAxisDir[e.caxis.axis] = 1;
				}
				else if (e.jaxis.value < -joysticks.at(e.caxis.which).GetRightStickDeadZone()) {
					joysticks.at(e.caxis.which).joyAxisDir[e.caxis.axis] = -1;
				}
				else {
					joysticks.at(e.caxis.which).joyAxisDir[e.caxis.axis] = 0;
				}
				break;
			//Left trigger
			case(SDL_CONTROLLER_AXIS_TRIGGERLEFT):
				if (e.jaxis.value > 0) {
					joysticks.at(e.caxis.which).joyAxisDir[e.caxis.axis] = 1;
				}
				else if (e.jaxis.value < 0) {
					joysticks.at(e.caxis.which).joyAxisDir[e.caxis.axis] = -1;
				}
				else {
					joysticks.at(e.caxis.which).joyAxisDir[e.caxis.axis] = 0;
				}
				break;
			//Right trigger
			case(SDL_CONTROLLER_AXIS_TRIGGERRIGHT):
				if (e.jaxis.value > 0) {
					joysticks.at(e.caxis.which).joyAxisDir[e.caxis.axis] = 1;
				}
				else if (e.jaxis.value < 0) {
					joysticks.at(e.caxis.which).joyAxisDir[e.caxis.axis] = -1;
				}
				else {
					joysticks.at(e.caxis.which).joyAxisDir[e.caxis.axis] = 0;
				}
				break;
			}
			break;
		case(SDL_CONTROLLERBUTTONDOWN):
			joysticks.at(e.cbutton.which).joyButtons[e.cbutton.button] = true;
			break;
		case(SDL_CONTROLLERBUTTONUP):
			joysticks.at(e.cbutton.which).joyButtons[e.cbutton.button] = false;
			break;
		//For joy hat motion
		/*
		case(SDL_JOYHATMOTION):
			std::cout << e.jhat.hat << std::endl;
			switch (e.jhat.value) {
				case(SDL_HAT_CENTERED):
				break;
				case(SDL_HAT_UP):
				break;
				case(SDL_HAT_RIGHT):
				break;
				case(SDL_HAT_DOWN):
				break;
				case(SDL_HAT_LEFT):
				break;
				case(SDL_HAT_RIGHTUP):
				break;
				case(SDL_HAT_RIGHTDOWN):
				break;
				case(SDL_HAT_LEFTUP):
				break;
				case(SDL_HAT_LEFTDOWN):
				break;
			}
			break;*/
		//Is quit
		case(SDL_QUIT):
			requestedQuit = true;
			break;
		}
	}
}

void Input::Render(){
}

bool Input::Shutdown() {
	//Close if open
	if (joysticks.size() > 0) {
		for (unsigned int i = 0; i < joysticks.size(); i++) {
			SDL_Joystick* joy = joysticks.at(i).GetJoyStick();
			if (SDL_JoystickGetAttached(joy)) {
				SDL_JoystickClose(joy);
			}

			if (SDL_JoystickGetAttached(joy)) {
				std::cout << "Joy: " << i << " still acitve" << std::endl;
			}

			joysticks.at(i).joyButtons.clear();

			joysticks.at(i).oldJoyButtons.clear();

			joysticks.at(i).joyAxis.clear();

			joysticks.at(i).joyAxisDir.clear();
		}
		joysticks.clear();
		joysticks.shrink_to_fit();
	}

	keys.clear();
	oldkeys.clear();

	mouseButtons.clear();
	oldMouseButtons.clear();

	return true;
}

bool Input::IsKeyDown(unsigned int key)
{
	auto it = keys.find(key);

	if (it != keys.end()) {
		//Returns true or false based on the state of the map
		return keys[key];
	}

	//The key has never been pressed
	return false;
}

//KEYBOARD===================================================

bool Input::IsKeyUp(unsigned int key)
{
	auto it = keys.find(key);

	if (it != keys.end()) {
		return !keys[key];
	}

	//Key has never been pressed
	return true;
}

bool Input::WasKeyPressed(unsigned int key)
{
	auto it = oldkeys.find(key);

	//If key was in old array
	if (it != oldkeys.end()) {
			 //If false and if true
		return !oldkeys[key] && keys[key];
	}

	return keys[key];
}

bool Input::WasKeyReleased(unsigned int key)
{
	auto it = oldkeys.find(key);

	//If key was in old array
	if (it != oldkeys.end()) {
			 //If true and if false
		return oldkeys[key] && !keys[key];
	}

	return false;
}

//MOUSE=====================================================================

bool Input::IsMouseButtonDown(unsigned int key)
{
	auto it = mouseButtons.find(key);

	if (it != mouseButtons.end()) {
		//Return true or false based on the state of the map
		return mouseButtons[key];
	}

	//The button was never pressed
	return false;
}
bool Input::IsMouseButtonUp(unsigned int key)
{
	auto it = mouseButtons.find(key);

	if (it != mouseButtons.end()) {
		//Return true or false based on the state of the map
		return !mouseButtons[key];
	}

	//The button was never pressed
	return true;
}
bool Input::WasMouseButtonPressed(unsigned int key)
{
	auto it = oldMouseButtons.find(key);

	//If button in old array
	if (it != oldMouseButtons.end()) {
		//If false and true
		return !oldMouseButtons[key] && mouseButtons[key];
	}

	//Return from current array
	return mouseButtons[key];
}
bool Input::WasMouseButtonReleased(unsigned int key)
{
	auto it = oldMouseButtons.find(key);

	//If button in old array
	if (it != oldMouseButtons.end()) {
		//If true and false
		return oldMouseButtons[key] && !mouseButtons[key];
	}

	//Return from current array
	return mouseButtons[key];
}

//JOYSTICK=========================================================

Joystick::Joystick(SDL_Joystick * j,int left_stick_sensitivity,int right_stick_sensitivity) : joy(j),id(SDL_JoystickInstanceID(j)), hats(SDL_JoystickNumHats(j)), axes(SDL_JoystickNumAxes(j)),
																							  left_stick_dead_zone(left_stick_sensitivity),right_stick_dead_zone(right_stick_sensitivity),
																							  buttons(SDL_JoystickNumButtons(j)),balls(SDL_JoystickNumBalls(j)), type(SDL_JoystickGetType(j))
{
	std::cout << "Number of hats: " << SDL_JoystickNumHats(j) << std::endl;
	std::cout << "Number of axes: " << SDL_JoystickNumAxes(j) << std::endl;
	std::cout << "Number of buttons: " << SDL_JoystickNumButtons(j) << std::endl;
	std::cout << "Number of balls: " << SDL_JoystickNumBalls(j) << std::endl;
	std::cout << "Vendor: " << SDL_JoystickGetVendor(j) << std::endl;
	std::cout << "Product: " << SDL_JoystickGetProduct(j) << std::endl;
}

Joystick::~Joystick() {
}

void Joystick::Update(SDL_Event & e)
{
	switch (e.type) {
	case(SDL_JOYAXISMOTION):
		//Value for axis ranges form -32768 to 32767
		//Set axis values
		joyAxis[e.jaxis.axis] = e.jaxis.value;

		if (e.jaxis.value > 0) {
			joyAxisDir[e.jaxis.axis] = 1;
		}
		else if (e.jaxis.value < 0) {
			joyAxisDir[e.jaxis.axis] = -1;
		}
		else {
			joyAxisDir[e.jaxis.axis] = 0;
		}
		break;
	case(SDL_JOYBUTTONDOWN):
		joyButtons[e.jbutton.button] = true;
		break;
	case(SDL_JOYBUTTONUP):
		joyButtons[e.jbutton.button] = false;
		break;
	}
}

Joystick* Input::GetJoystick(unsigned int which)
{
	if (&joysticks[which]) {
		return &joysticks[which];
	}
	return nullptr;
}

bool Joystick::IsJoyStickButtonDown(unsigned int key)
{
	auto it = joyButtons.find(key);

	if (it != joyButtons.end()) {
		//Returns true or false based on the state of the map
		return joyButtons[key];
	}

	//The button has never been pressed
	return false;
}

bool Joystick::IsJoyStickButtonUp(unsigned int key)
{
	auto it = joyButtons.find(key);

	if (it != joyButtons.end()) {
		return !joyButtons[key];
	}

	//Button has never been pressed
	return true;
}

bool Joystick::WasJoyStickButtonPressed(unsigned int key)
{
	auto it = oldJoyButtons.find(key);

	//If button was in old array
	if (it != oldJoyButtons.end()) {
		//If false and if true
		return !oldJoyButtons[key] && joyButtons[key];
	}

	return joyButtons[key];
}

bool Joystick::WasJoyStickButtonReleased(unsigned int key)
{
	auto it = oldJoyButtons.find(key);

	//If button was in old array
	if (it != oldJoyButtons.end()) {
		//If true and if false
		return oldJoyButtons[key] && !joyButtons[key];
	}

	return false;
}

//GAMECONTROLLER==========================================================================

GameController::GameController(SDL_Joystick * j, int left_stick_dead_zone, int right_stick_dead_zone) : Joystick(j,left_stick_dead_zone,right_stick_dead_zone), gameController(SDL_GameControllerOpen(SDL_JoystickInstanceID(j)))
{
	if (mapping == nullptr) {
		mapping = SDL_GameControllerMapping(gameController);
	}
	SDL_JoystickClose(j);
}

GameController::~GameController()
{
}

void GameController::Update(SDL_Event & e)
{
	switch(e.type){
		case(SDL_CONTROLLERAXISMOTION):
			joyAxis[e.caxis.axis] = e.caxis.value;

			switch (e.caxis.axis)
			{
			//Left X axis
			case(SDL_CONTROLLER_AXIS_LEFTX):
				if (e.jaxis.value > GetLeftStickDeadZone()) {
					joyAxisDir[e.caxis.axis] = 1;
				}
				else if (e.jaxis.value < -GetLeftStickDeadZone()) {
					joyAxisDir[e.caxis.axis] = -1;
				}
				else {
					joyAxisDir[e.caxis.axis] = 0;
				}
				break;
			//Left Y axis
			case(SDL_CONTROLLER_AXIS_LEFTY):
				if (e.jaxis.value > GetLeftStickDeadZone()) {
					joyAxisDir[e.caxis.axis] = 1;
				}
				else if (e.jaxis.value < -GetLeftStickDeadZone()) {
					joyAxisDir[e.caxis.axis] = -1;
				}
				else {
					joyAxisDir[e.caxis.axis] = 0;
				}
				break;
			//Right X axis
			case(SDL_CONTROLLER_AXIS_RIGHTX):
				if (e.jaxis.value > GetRightStickDeadZone()) {
					joyAxisDir[e.caxis.axis] = 1;
				}
				else if (e.jaxis.value < -GetRightStickDeadZone()) {
					joyAxisDir[e.caxis.axis] = -1;
				}
				else {
					joyAxisDir[e.caxis.axis] = 0;
				}
				break;
			//Right Y axis			 
			case(SDL_CONTROLLER_AXIS_RIGHTY):
				if (e.jaxis.value > GetRightStickDeadZone()) {
					joyAxisDir[e.caxis.axis] = 1;
				}
				else if (e.jaxis.value < -GetRightStickDeadZone()) {
					joyAxisDir[e.caxis.axis] = -1;
				}
				else {
					joyAxisDir[e.caxis.axis] = 0;
				}
				break;
			//Left trigger
			case(SDL_CONTROLLER_AXIS_TRIGGERLEFT):
				if (e.jaxis.value > 0) {
					joyAxisDir[e.caxis.axis] = 1;
				}
				else if (e.jaxis.value < 0) {
					joyAxisDir[e.caxis.axis] = -1;
				}
				else {
					joyAxisDir[e.caxis.axis] = 0;
				}
				break;
			//Right trigger
			case(SDL_CONTROLLER_AXIS_TRIGGERRIGHT):
				if (e.jaxis.value > 0) {
					joyAxisDir[e.caxis.axis] = 1;
				}
				else if (e.jaxis.value < 0) {
					joyAxisDir[e.caxis.axis] = -1;
				}
				else {
					joyAxisDir[e.caxis.axis] = 0;
				}
				break;
			}
			break;
		case(SDL_CONTROLLERBUTTONDOWN):
			joyButtons[e.cbutton.button] = true;
			break;
		case(SDL_CONTROLLERBUTTONUP):
			joyButtons[e.cbutton.button] = false;
			break;
	}
}

void GameController::RebindButton()
{

}
