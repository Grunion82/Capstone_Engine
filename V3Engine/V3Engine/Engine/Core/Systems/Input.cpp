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
	//SDL_InitSubSystem(SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER);

	if (SDL_NumJoysticks() > 0) {
		std::cout << "Number of mapping: " << SDL_GameControllerNumMappings() << std::endl;
		std::wcout << std::endl;
		joysticks.reserve(SDL_NumJoysticks());
		//Check for joysticks
		for (int i = 0; i < SDL_NumJoysticks(); i++) {
			//SDL_Joystick* j = SDL_JoystickOpen(i);

			//joysticks.push_back(Joystick(j,8000,8000));
			//joysticks.insert(joysticks.begin() + i, Joystick(j, 8000));

			std::cout << "Joystick #: " << i << std::endl;
			std::cout << "Name: " << SDL_JoystickNameForIndex(i) << std::endl;
			std::cout << "Id: " << SDL_JoystickGetDeviceInstanceID(i) << std::endl;
			std::cout << "Type: " << (SDL_JoystickType)SDL_JoystickGetDeviceType(i) << std::endl;

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
				SDL_GameControllerEventState(SDL_ENABLE);
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
								       buttons(SDL_JoystickNumButtons(j)),balls(SDL_JoystickNumBalls(j)), type(SDL_JoystickGetType(j)),
									   joystickName(SDL_JoystickName(j))
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
		if (e.type == SDL_JOYAXISMOTION) {
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
		}

		if (e.type == SDL_JOYBUTTONDOWN || e.type == SDL_JOYBUTTONDOWN) {
			for (unsigned int i = 0; i < buttons; i++) {
				joyButtons[i] = SDL_JoystickGetButton(joy, i);
			}
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

bool Joystick::Shutdown()
{
	bool result = true;

	SDL_JoystickClose(joy);

	if(SDL_JoystickGetAttached(joy)){
		std::cout << "Joy " << id << " still active!" << std::endl;
		result = false;
	}
	else {
		joy = nullptr;
	}

	joyButtons.clear();
	oldJoyButtons.clear();

	joyAxis.clear();
	joyAxisDir.clear();

	return result;
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

	if (mapping == nullptr) {
		std::cout << "Could not find mapping for: " << SDL_GameControllerName(gameController) << "ID: " << id << std::endl;
	}

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
		switch (e.type) {
		case(SDL_JOYAXISMOTION):
		case(SDL_CONTROLLERAXISMOTION):
				for (unsigned int i = 0; i < SDL_CONTROLLER_AXIS_MAX; i++) {
					joyAxis[i] = SDL_GameControllerGetAxis(gameController, (SDL_GameControllerAxis)i);

					switch (i) {
					case(SDL_CONTROLLER_AXIS_LEFTX):
						if (joyAxis[i] > 0 && joyAxis[i] > left_stick_dead_zone) {
							joyAxisDir[i] = 1;
						}
						else if (joyAxis[i] < 0 && joyAxis[i] < -left_stick_dead_zone) {
							joyAxisDir[i] = -1;
						}
						else {
							joyAxisDir[i] = 0;
						}
						break;
					case(SDL_CONTROLLER_AXIS_LEFTY):
						if (joyAxis[i] > 0 && joyAxis[i] > left_stick_dead_zone) {
							joyAxisDir[i] = 1;
						}
						else if (joyAxis[i] < 0 && joyAxis[i] < -left_stick_dead_zone) {
							joyAxisDir[i] = -1;
						}
						else {
							joyAxisDir[i] = 0;
						}
						break;
					case(SDL_CONTROLLER_AXIS_RIGHTX):
						if (joyAxis[i] > 0 && joyAxis[i] > right_stick_dead_zone) {
							joyAxisDir[i] = 1;
						}
						else if (joyAxis[i] < 0 && joyAxis[i] < -right_stick_dead_zone) {
							joyAxisDir[i] = -1;
						}
						else {
							joyAxisDir[i] = 0;
						}
						break;
					case(SDL_CONTROLLER_AXIS_RIGHTY):
						if (joyAxis[i] > 0 && joyAxis[i] > right_stick_dead_zone) {
							joyAxisDir[i] = 1;
						}
						else if (joyAxis[i] < 0 && joyAxis[i] < -right_stick_dead_zone) {
							joyAxisDir[i] = -1;
						}
						else {
							joyAxisDir[i] = 0;
						}
						break;
					case(SDL_CONTROLLER_AXIS_TRIGGERLEFT):
						if (joyAxis[i] > 0) {
							joyAxisDir[i] = 1;
						}
						else if (joyAxis[i] < 0) {
							joyAxisDir[i] = -1;
						}
						else {
							joyAxisDir[i] = 0;
						}
						break;
					case(SDL_CONTROLLER_AXIS_TRIGGERRIGHT):
						if (joyAxis[i] > 0) {
							joyAxisDir[i] = 1;
						}
						else if (joyAxis[i] < 0) {
							joyAxisDir[i] = -1;
						}
						else {
							joyAxisDir[i] = 0;
						}
						break;
					}
				}
				break;
		case(SDL_JOYBUTTONDOWN):
		case(SDL_JOYBUTTONUP):
		case(SDL_CONTROLLERBUTTONDOWN):
		case(SDL_CONTROLLERBUTTONUP):
			std::cout << SDL_GameControllerGetStringForButton((SDL_GameControllerButton)SDL_GameControllerGetBindForButton(gameController, (SDL_GameControllerButton)e.cbutton.button).value.button) << "(" << (int)e.cbutton.button << ")" << std::endl;
				for (unsigned int i = 0; i < SDL_CONTROLLER_BUTTON_MAX; i++) {
					joyButtons[i] = SDL_JoystickGetButton(joy, i);
				}
				break;
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
			joyAxis[i] = e.caxis.value;

			switch (e.caxis.axis)
			{
				//Left X axis
			case(SDL_CONTROLLER_AXIS_LEFTX):
				if (e.caxis.value > left_stick_dead_zone) {
					joyAxisDir[i] = 1;
				}
				else if (e.caxis.value < -left_stick_dead_zone) {
					joyAxisDir[i] = -1;
				}
				else {
					joyAxisDir[i] = 0;
				}
				break;
				//Left Y axis
			case(SDL_CONTROLLER_AXIS_LEFTY):
				if (e.caxis.value > left_stick_dead_zone) {
					joyAxisDir[i] = 1;
				}
				else if (e.caxis.value < -left_stick_dead_zone) {
					joyAxisDir[i] = -1;
				}
				else {
					joyAxisDir[i] = 0;
				}
				break;
				//Right X axis
			case(SDL_CONTROLLER_AXIS_RIGHTX):
				if (e.caxis.value > right_stick_dead_zone) {
					joyAxisDir[i] = 1;
				}
				else if (e.caxis.value < -right_stick_dead_zone) {
					joyAxisDir[i] = -1;
				}
				else {
					joyAxisDir[i] = 0;
				}
				break;
				//Right Y axis			 
			case(SDL_CONTROLLER_AXIS_RIGHTY):
				if (e.caxis.value > right_stick_dead_zone) {
					joyAxisDir[i] = 1;
				}
				else if (e.caxis.value < -right_stick_dead_zone) {
					joyAxisDir[i] = -1;
				}
				else {
					joyAxisDir[i] = 0;
				}
				break;
				//Left trigger
			case(SDL_CONTROLLER_AXIS_TRIGGERLEFT):
				if (e.caxis.value > 0) {
					joyAxisDir[i] = 1;
				}
				else if (e.caxis.value < 0) {
					joyAxisDir[i] = -1;
				}
				else {
					joyAxisDir[i] = 0;
				}
				break;
				//Right trigger
			case(SDL_CONTROLLER_AXIS_TRIGGERRIGHT):
				if (e.caxis.value > 0) {
					joyAxisDir[i] = 1;
				}
				else if (e.caxis.value < 0) {
					joyAxisDir[i] = -1;
				}
				else {
					joyAxisDir[i] = 0;
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

bool GameController::Shutdown() 
{
	bool result = true;

	SDL_GameControllerClose(gameController);

	if (SDL_GameControllerGetAttached(gameController)) {
		std::cout << "Game controller " << id << " still active!" << std::endl;
		result = false;
	}
	else {
		gameController = nullptr;
	}

	joyButtons.clear();
	oldJoyButtons.clear();

	joyAxis.clear();
	joyAxisDir.clear();

	return result;
}

bool GameController::IsButtonDown(unsigned int button)
{
	auto it = joyButtons.find(button);

	if (it != joyButtons.end()) {
		//Returns true or false based on the state of the map
		return joyButtons[button];
	}

	//The button has never been pressed
	return false;
}

bool GameController::IsButtonUp(unsigned int button)
{
	auto it = joyButtons.find(button);

	if (it != joyButtons.end()) {
		return !joyButtons[button];
	}

	//Button has never been pressed
	return true;
}

bool GameController::WasButtonPressed(unsigned int button)
{
	auto it = oldJoyButtons.find(button);

	//If button was in old array
	if (it != oldJoyButtons.end()) {
		//If false and if true
		return !oldJoyButtons[button] && joyButtons[button];
	}

	return joyButtons[button];
}

bool GameController::WasButtonReleased(unsigned int button)
{
	auto it = oldJoyButtons.find(button);

	//If button was in old array
	if (it != oldJoyButtons.end()) {
		//If true and if false
		return oldJoyButtons[button] && !joyButtons[button];
	}

	return false;
}


void GameController::RebindButton()
{
	SDL_Event e;
	__int8 button = -1;
	bool isButtonSet = false;

	std::cout << "Rebind button for controller " << id << std::endl;
	std::cout << "Enter button to rebind" << std::endl;

	SDL_JoystickGUID joyGUID = SDL_JoystickGetGUID(SDL_GameControllerGetJoystick(gameController));
	
	char pszGUID[33];

	SDL_JoystickGetGUIDString(joyGUID, pszGUID, sizeof(pszGUID));

	char* thing = SDL_GameControllerMappingForGUID(joyGUID);

	while (!isButtonSet) {
			SDL_PollEvent(&e);
			if (e.type == SDL_CONTROLLERBUTTONDOWN && button == -1 && e.cdevice.which == id) {
				button = e.cbutton.button;
				std::cout << "Button set to remap: " << SDL_GameControllerGetStringForButton((SDL_GameControllerButton)SDL_GameControllerGetBindForButton(gameController, (SDL_GameControllerButton)button).value.button) << "(" << (int)button << ")" << std::endl;
			}
			else if (e.type == SDL_CONTROLLERBUTTONDOWN && button != -1 && e.cdevice.which == id) {
				//The button to rebind
				SDL_GameControllerButtonBind buttonBind = SDL_GameControllerGetBindForButton(gameController, (SDL_GameControllerButton)button);
				//The button to rebind to
				SDL_GameControllerButtonBind newbuttonBind = SDL_GameControllerGetBindForButton(gameController, (SDL_GameControllerButton)e.cbutton.button);
				
				std::string newMapping = mapping;
				std::string bs = ",";
				bs += SDL_GameControllerGetStringForButton((SDL_GameControllerButton)buttonBind.value.button);

				int b1 = newMapping.find(bs);
				bs = ",";
				bs += SDL_GameControllerGetStringForButton((SDL_GameControllerButton)newbuttonBind.value.button);
				int b2 = newMapping.find(bs);

				newMapping.replace(b1 + 4,1, std::to_string((int)newbuttonBind.value.button));
				newMapping.replace(b2 + 4,1, std::to_string((int)buttonBind.value.button));
				
				std::cout << newMapping << std::endl;

				int thing = SDL_GameControllerAddMapping(newMapping.c_str());
				std::cout << thing << std::endl;

				std::cout << "Bind button: " << SDL_GameControllerGetStringForButton((SDL_GameControllerButton)buttonBind.value.button) << "(" << buttonBind.value.button << ")" << std::endl;
				
				isButtonSet = true;
			}
	}
}
