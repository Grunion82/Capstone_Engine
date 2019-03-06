#include "Input.h"
#include <SDL.h>
#include <stdio.h>

#include <iostream>

#include <string>

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
		std::cout << "Number of mapping: " << SDL_GameControllerNumMappings() << std::endl;
		std::wcout << std::endl;
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

			int type = SDL_JoystickGetDeviceType(i);

			void* j;

			switch (type)
			{
			//Unknown - assume joystick
			case(0):
				j = SDL_JoystickOpen(i);
				joysticks.push_back(new Joystick((SDL_Joystick*)j));
				break;
			//Game controller
			case(1):
				j = SDL_GameControllerOpen(i);
				joysticks.push_back(new GameController((SDL_GameController*)j, 8000, 8000));
				break;
			default:
				break;
			}

			std::cout << std::endl;
		}
	}
	
	eventFlags = SDL_KEYDOWN | SDL_KEYUP | SDL_MOUSEMOTION | SDL_MOUSEBUTTONDOWN | SDL_MOUSEBUTTONUP | SDL_MOUSEWHEEL | SDL_TEXTINPUT;

	return true;
}
void Input::Update()
{
	oldkeys = keys;
	oldMouseButtons = mouseButtons;

	mouseMotionX = 0;
	mouseMotionY = 0;
	mouseWheelY = 0;

	clicks = 0;
}
void Input::Update(SDL_Event& e) {
	if ((e.type & eventFlags) == e.type) {
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
		}

		for (unsigned int i = 0; i < joysticks.size(); i++) {
			joysticks[i]->Update(e);
		}
	}
}

bool Input::Shutdown() {
	//Close if open
	if (joysticks.size() > 0) {
		for (unsigned int i = 0; i < joysticks.size(); i++) {
			joysticks[i]->Shutdown();
			joysticks[i] = nullptr;
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

Joystick::Joystick()
{
}

Joystick::Joystick(SDL_Joystick * j) : joy(j),id(SDL_JoystickInstanceID(j)), hats(SDL_JoystickNumHats(j)), axes(SDL_JoystickNumAxes(j)),
								       buttons(SDL_JoystickNumButtons(j)),balls(SDL_JoystickNumBalls(j)), type(SDL_JoystickGetType(j))
{
	std::cout << "Number of hats: " << hats << std::endl;
	std::cout << "Number of axes: " << axes << std::endl;
	std::cout << "Number of buttons: " << buttons << std::endl;
	std::cout << "Number of balls: " << balls << std::endl;
	std::cout << "Vendor: " << SDL_JoystickGetVendor(j) << std::endl;
	std::cout << "Product: " << SDL_JoystickGetProduct(j) << std::endl;
	
	if (type == SDL_JOYSTICK_POWER_UNKNOWN) {

		eventFlags = SDL_JOYAXISMOTION | SDL_JOYBUTTONDOWN | SDL_JOYBUTTONUP | SDL_JOYDEVICEADDED | SDL_JOYDEVICEREMOVED;

		//Check if joysticks are pointing in a direction
		Sint16 state;
		for (int a = 0; a < SDL_JoystickNumAxes(j); a++) {
			//If there is a change for joystick during init
			if (SDL_JoystickGetAxisInitialState(j, a, &state)) {
				joyAxis[a] = state;

				if (state > 0) {
					joyAxisDir[a] = 1;
				}
				else if (state < 0) {
					joyAxisDir[a] = -1;
				}
				else {
					joyAxisDir[a] = 0;
				}
			}
		}
	}
}

Joystick::~Joystick() {
}

void Joystick::Update(SDL_Event& e)
{
	//SDL_JoystickUpdate();
	oldJoyButtons = joyButtons;
	if ((e.type & eventFlags) == e.type && e.jdevice.which == id) {
		for (unsigned int i = 0; i < axes; i++) {
			joyAxis[i] = SDL_JoystickGetAxis(joy, i);

			if (joyAxis[i] > 0) {
				joyAxisDir[i] = 1;
			}
			else if (joyAxis[i] < 0) {
				joyAxisDir[i] = -1;
			}
			else {
				joyAxisDir[i] = 0;
			}
		}

		for (unsigned int i = 0; i < buttons; i++) {
			joyButtons[i] = SDL_JoystickGetButton(joy, i);
		}
	}

}

//Event based update
/*
void Joystick::Update(SDL_Event & e)
{
	oldJoyButtons = joyButtons;

	if ((e.type & eventFlags) == e.type && e.jdevice.which == id) {

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
}*/

void Joystick::Shutdown()
{
	SDL_JoystickClose(joy);

	if(SDL_JoystickGetAttached(joy)){
		std::cout << "Joy " << id << " still active!" << std::endl;
	}
	else {
		joy = nullptr;
	}

	joyButtons.clear();
	oldJoyButtons.clear();

	joyAxis.clear();
	joyAxisDir.clear();
}

//Joystick* Input::GetJoystick(unsigned int which)
//{
//	if (&joysticks[which]) {
//		return &joysticks[which];
//	}
//	return nullptr;
//}

bool Joystick::IsButtonDown(unsigned int button)
{
	auto it = joyButtons.find(button);

	if (it != joyButtons.end()) {
		//Returns true or false based on the state of the map
		return joyButtons[button];
	}

	//The button has never been pressed
	return false;
}

bool Joystick::IsButtonUp(unsigned int button)
{
	auto it = joyButtons.find(button);

	if (it != joyButtons.end()) {
		return !joyButtons[button];
	}

	//Button has never been pressed
	return true;
}

bool Joystick::WasButtonPressed(unsigned int button)
{
	auto it = oldJoyButtons.find(button);

	//If button was in old array
	if (it != oldJoyButtons.end()) {
		//If false and if true
		return !oldJoyButtons[button] && joyButtons[button];
	}

	return joyButtons[button];
}

bool Joystick::WasButtonReleased(unsigned int button)
{
	auto it = oldJoyButtons.find(button);

	//If button was in old array
	if (it != oldJoyButtons.end()) {
		//If true and if false
		return oldJoyButtons[button] && !joyButtons[button];
	}

	return false;
}

//GAMECONTROLLER==========================================================================

GameController::GameController(SDL_GameController* gc, int left_stick_dead_zone, int right_stick_dead_zone) : Joystick(SDL_GameControllerGetJoystick(gc)), gameController(gc), left_stick_dead_zone(left_stick_dead_zone), right_stick_dead_zone(right_stick_dead_zone), mapping(SDL_GameControllerMapping(gc))
{
	eventFlags = SDL_CONTROLLERAXISMOTION | SDL_CONTROLLERBUTTONDOWN | SDL_CONTROLLERBUTTONUP | SDL_CONTROLLERDEVICEADDED | SDL_CONTROLLERDEVICEREMOVED;

	std::cout << mapping << std::endl;

	//Check if joysticks are pointing in a direction
	Sint16 state;
	for (int a = 0; a < SDL_JoystickNumAxes(joy); a++) {
		if (SDL_JoystickGetAxisInitialState(joy, a, &state)) {
			//If there is a change for joystick during init
			joyAxis[a] = SDL_GameControllerGetAxis(gameController, (SDL_GameControllerAxis)a);

			if (a == SDL_CONTROLLER_AXIS_LEFTX || a == SDL_CONTROLLER_AXIS_LEFTY) {
				if (joyAxis[a] > 0 && joyAxis[a] > left_stick_dead_zone) {
					joyAxisDir[a] = 1;
				}
				else if (joyAxis[a] < 0 && joyAxis[a] < -left_stick_dead_zone) {
					joyAxisDir[a] = -1;
				}
				else {
					joyAxisDir[a] = 0;
				}
			}
			else if (a == SDL_CONTROLLER_AXIS_RIGHTX || a == SDL_CONTROLLER_AXIS_RIGHTY) {
				if (joyAxis[a] > 0 && joyAxis[a] > right_stick_dead_zone) {
					joyAxisDir[a] = 1;
				}
				else if (joyAxis[a] < 0 && joyAxis[a] < -right_stick_dead_zone) {
					joyAxisDir[a] = -1;
				}
				else {
					joyAxisDir[a] = 0;
				}
			}
			else {
				if (joyAxis[a] > 0) {
					joyAxisDir[a] = 1;
				}
				else if (joyAxis[a] < 0) {
					joyAxisDir[a] = -1;
				}
				else {
					joyAxisDir[a] = 0;
				}
			}
		}
	}
}

GameController::~GameController()
{
}

void GameController::Update(SDL_Event& e)
{
	//SDL_GameControllerUpdate();
	oldJoyButtons = joyButtons;
	if ((e.type & eventFlags) == e.type && e.cdevice.which == id) {
		for (unsigned int i = 0; i < SDL_CONTROLLER_AXIS_MAX; i++) {
			joyAxis[i] = SDL_GameControllerGetAxis(gameController, (SDL_GameControllerAxis)i);

			if (i == SDL_CONTROLLER_AXIS_LEFTX || i == SDL_CONTROLLER_AXIS_LEFTY) {
				if (joyAxis[i] > 0 && joyAxis[i] > left_stick_dead_zone) {
					joyAxisDir[i] = 1;
				}
				else if (joyAxis[i] < 0 && joyAxis[i] < -left_stick_dead_zone) {
					joyAxisDir[i] = -1;
				}
				else {
					joyAxisDir[i] = 0;
				}
			}
			else if (i == SDL_CONTROLLER_AXIS_RIGHTX || i == SDL_CONTROLLER_AXIS_RIGHTY) {
				if (joyAxis[i] > 0 && joyAxis[i] > right_stick_dead_zone) {
					joyAxisDir[i] = 1;
				}
				else if (joyAxis[i] < 0 && joyAxis[i] < -right_stick_dead_zone) {
					joyAxisDir[i] = -1;
				}
				else {
					joyAxisDir[i] = 0;
				}
			}
			else {
				if (joyAxis[i] > 0) {
					joyAxisDir[i] = 1;
				}
				else if (joyAxis[i] < 0) {
					joyAxisDir[i] = -1;
				}
				else {
					joyAxisDir[i] = 0;
				}
			}
		}

		for (unsigned int i = 0; i < SDL_CONTROLLER_BUTTON_MAX; i++) {
			joyButtons[i] = SDL_JoystickGetButton(joy, i);
		}
	}
}

//Event based update method
/*
void GameController::Update(SDL_Event & e)
{
	oldJoyButtons = joyButtons;

	if ((e.type & eventFlags) == e.type && e.cdevice.which == id) {
		switch (e.type) {
		case(SDL_JOYAXISMOTION):
		case(SDL_CONTROLLERAXISMOTION):
			joyAxis[e.caxis.axis] = e.caxis.value;

			switch (e.caxis.axis)
			{
				//Left X axis
			case(SDL_CONTROLLER_AXIS_LEFTX):
				if (e.caxis.value > left_stick_dead_zone) {
					joyAxisDir[e.caxis.axis] = 1;
				}
				else if (e.caxis.value < -left_stick_dead_zone) {
					joyAxisDir[e.caxis.axis] = -1;
				}
				else {
					joyAxisDir[e.caxis.axis] = 0;
				}
				break;
				//Left Y axis
			case(SDL_CONTROLLER_AXIS_LEFTY):
				if (e.caxis.value > left_stick_dead_zone) {
					joyAxisDir[e.caxis.axis] = 1;
				}
				else if (e.caxis.value < -left_stick_dead_zone) {
					joyAxisDir[e.caxis.axis] = -1;
				}
				else {
					joyAxisDir[e.caxis.axis] = 0;
				}
				break;
				//Right X axis
			case(SDL_CONTROLLER_AXIS_RIGHTX):
				if (e.caxis.value > right_stick_dead_zone) {
					joyAxisDir[e.caxis.axis] = 1;
				}
				else if (e.caxis.value < -right_stick_dead_zone) {
					joyAxisDir[e.caxis.axis] = -1;
				}
				else {
					joyAxisDir[e.caxis.axis] = 0;
				}
				break;
				//Right Y axis			 
			case(SDL_CONTROLLER_AXIS_RIGHTY):
				if (e.caxis.value > right_stick_dead_zone) {
					joyAxisDir[e.caxis.axis] = 1;
				}
				else if (e.caxis.value < -right_stick_dead_zone) {
					joyAxisDir[e.caxis.axis] = -1;
				}
				else {
					joyAxisDir[e.caxis.axis] = 0;
				}
				break;
				//Left trigger
			case(SDL_CONTROLLER_AXIS_TRIGGERLEFT):
				if (e.caxis.value > 0) {
					joyAxisDir[e.caxis.axis] = 1;
				}
				else if (e.caxis.value < 0) {
					joyAxisDir[e.caxis.axis] = -1;
				}
				else {
					joyAxisDir[e.caxis.axis] = 0;
				}
				break;
				//Right trigger
			case(SDL_CONTROLLER_AXIS_TRIGGERRIGHT):
				if (e.caxis.value > 0) {
					joyAxisDir[e.caxis.axis] = 1;
				}
				else if (e.caxis.value < 0) {
					joyAxisDir[e.caxis.axis] = -1;
				}
				else {
					joyAxisDir[e.caxis.axis] = 0;
				}
				break;
			}
			break;
		case(SDL_JOYBUTTONDOWN):
		case(SDL_CONTROLLERBUTTONDOWN):
			joyButtons[e.cbutton.button] = true;
			break;
		case(SDL_JOYBUTTONUP):
		case(SDL_CONTROLLERBUTTONUP):
			joyButtons[e.cbutton.button] = false;
			break;
		}
	}
}*/

void GameController::Shutdown() 
{
	SDL_GameControllerClose(gameController);

	if (SDL_GameControllerGetAttached(gameController)) {
		std::cout << "Game controller " << id << " still active!" << std::endl;
	}
	else {
		gameController = nullptr;
	}

	joyButtons.clear();
	oldJoyButtons.clear();

	joyAxis.clear();
	joyAxisDir.clear();
}

bool GameController::IsButtonDown(unsigned int button)
{
	int b = SDL_GameControllerGetBindForButton(gameController, (SDL_GameControllerButton)button).value.button;

	auto it = joyButtons.find(b);

	if (it != joyButtons.end()) {
		//Returns true or false based on the state of the map
		return joyButtons[b];
	}

	//The button has never been pressed
	return false;
}

bool GameController::IsButtonUp(unsigned int button)
{
	int b = SDL_GameControllerGetBindForButton(gameController, (SDL_GameControllerButton)button).value.button;

	auto it = joyButtons.find(b);

	if (it != joyButtons.end()) {
		return !joyButtons[b];
	}

	//Button has never been pressed
	return true;
}

bool GameController::WasButtonPressed(unsigned int button)
{
	int b = SDL_GameControllerGetBindForButton(gameController, (SDL_GameControllerButton)button).value.button;

	auto it = oldJoyButtons.find(b);

	//If button was in old array
	if (it != oldJoyButtons.end()) {
		//If false and if true
		return !oldJoyButtons[b] && joyButtons[b];
	}

	return joyButtons[b];
}

bool GameController::WasButtonReleased(unsigned int button)
{
	int b = SDL_GameControllerGetBindForButton(gameController, (SDL_GameControllerButton)button).value.button;

	auto it = oldJoyButtons.find(b);

	//If button was in old array
	if (it != oldJoyButtons.end()) {
		//If true and if false
		return oldJoyButtons[b] && !joyButtons[b];
	}

	return false;
}

void GameController::RebindButton()
{
	
}
