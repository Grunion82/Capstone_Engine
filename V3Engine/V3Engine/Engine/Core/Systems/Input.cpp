#include "Input.h"
#include <SDL.h>

#include "Debug.h"

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
				joysticks.push_back(new GameController((SDL_GameController*)j, 8000, 8000));
				break;
			default:
				break;
			}

			std::cout << std::endl;
		}
	}
	
	eventFlags = SDL_KEYDOWN | SDL_KEYUP | SDL_MOUSEMOTION | SDL_MOUSEBUTTONDOWN | SDL_MOUSEBUTTONUP | SDL_MOUSEWHEEL | SDL_TEXTINPUT | SDL_CONTROLLERDEVICEADDED | SDL_CONTROLLERDEVICEREMOVED;

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
		case(SDL_CONTROLLERDEVICEREMOVED):
			for (unsigned int i = 0; i < joysticks.size(); i++) {
				if (joysticks[i]->GetID() == e.cdevice.which) {
					joysticks[i]->Shutdown();
					delete joysticks[i];
					joysticks[i] = nullptr;
					break;
					//joysticks.erase(joysticks.begin() + i);
				}
			}
			break;
		case(SDL_CONTROLLERDEVICEADDED):
			if (joysticks.size() > 0) {
				bool is = false;

				//This is honestly only here for the initialize of controllers
				//Should be removed later
				for (unsigned int i = 0; i < joysticks.size(); i++) {
					if (joysticks[i]) {
						if (joysticks[i]->GetID() == SDL_JoystickGetDeviceInstanceID(e.cdevice.which)) {
							is = true;
							break;
						}
					}
				}
				if (!is) {
					for (unsigned int i = 0; i < joysticks.size(); i++) {
						if (joysticks[i] == nullptr) {
							joysticks[i] = new GameController(SDL_GameControllerOpen(e.cdevice.which), 8000, 8000);
							break;
						}
						else if ((i + 1) >= joysticks.size()) {
							joysticks.push_back(new GameController(SDL_GameControllerOpen(e.cdevice.which), 8000, 8000));
							break;
						}
					}
				}
			}
			else {
				joysticks.push_back(new GameController(SDL_GameControllerOpen(e.cdevice.which), 8000, 8000));
			}


			break;
		}

		//for (unsigned int i = 0; i < joysticks.size(); i++) {
		//	joysticks[i]->Update(e);
		//}
	}
}

void Input::UpdateJoysticks(SDL_Event & e)
{
	for (unsigned int i = 0; i < joysticks.size(); i++) {
		if (joysticks[i]) {
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
		std::string message = "Joy" + std::to_string(id) + " still active";
		Debug::SetSeverity(MessageType::TYPE_WARNING);
		Debug::Warning(message, __FILE__, __LINE__);
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

GameController::GameController(SDL_GameController* gc, int left_stick_dead_zone, int right_stick_dead_zone) : Joystick(SDL_GameControllerGetJoystick(gc)), gameController(gc), left_stick_dead_zone(left_stick_dead_zone), right_stick_dead_zone(right_stick_dead_zone)//, mapping(SDL_GameControllerMapping(gc))
{
	eventFlags = SDL_CONTROLLERAXISMOTION | SDL_CONTROLLERBUTTONDOWN | SDL_CONTROLLERBUTTONUP | SDL_CONTROLLER_BUTTON_DPAD_DOWN | SDL_CONTROLLER_BUTTON_DPAD_LEFT | SDL_CONTROLLER_BUTTON_DPAD_RIGHT | SDL_CONTROLLER_BUTTON_DPAD_UP;

	SDL_JoystickGUID joyGUID = SDL_JoystickGetGUID(joy);
	
	char pszGUID[33];
	
	SDL_JoystickGetGUIDString(joyGUID, pszGUID, sizeof(pszGUID));
	
	std::string thing = SDL_GameControllerMapping(gameController);
	
	
	thing.replace(0, sizeof(pszGUID), pszGUID);
	thing.insert(sizeof(pszGUID)-1,",");
	
	std::string name = SDL_GameControllerName(gameController);
	
	thing.replace(sizeof(pszGUID),name.size(), "");
	thing.insert(sizeof(pszGUID), joystickName);
	
	int result = SDL_GameControllerAddMapping(thing.c_str());
	
	mapping = SDL_GameControllerMappingForGUID(joyGUID);
	
	if (mapping == nullptr) {
		std::cout << "Could not find mapping for: " << joystickName << " ID: " << id << std::endl;
		std::string message = "Could not find mapping for: ";
		message += joystickName;
		message += " ID: ";
		message += id;
	
		Debug::SetSeverity(MessageType::TYPE_FATAL_ERROR);
		Debug::FatalError(message, __FILE__, __LINE__);
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
	oldJoyButtons = joyButtons;
	if ((e.type & eventFlags) == e.type && e.cdevice.which == id) {
		switch (e.type) {
		case(SDL_CONTROLLERAXISMOTION):
		{
			for (unsigned int i = 0; i < SDL_CONTROLLER_AXIS_MAX; i++) {
				joyAxis[i] = SDL_GameControllerGetAxis(gameController, (SDL_GameControllerAxis)i);

				switch (i) {
				case(SDL_CONTROLLER_AXIS_LEFTX):
					if (joyAxis[i] > left_stick_dead_zone) {
						joyAxisDir[i] = 1;
					}
					else if (joyAxis[i] < -left_stick_dead_zone) {
						joyAxisDir[i] = -1;
					}
					else {
						joyAxisDir[i] = 0;
					}
					break;
				case(SDL_CONTROLLER_AXIS_LEFTY):
					if (joyAxis[i] > left_stick_dead_zone) {
						joyAxisDir[i] = 1;
					}
					else if (joyAxis[i] < -left_stick_dead_zone) {
						joyAxisDir[i] = -1;
					}
					else {
						joyAxisDir[i] = 0;
					}
					break;
				case(SDL_CONTROLLER_AXIS_RIGHTX):
					if (joyAxis[i] > right_stick_dead_zone) {
						joyAxisDir[i] = 1;
					}
					else if (joyAxis[i] < -right_stick_dead_zone) {
						joyAxisDir[i] = -1;
					}
					else {
						joyAxisDir[i] = 0;
					}
					break;
				case(SDL_CONTROLLER_AXIS_RIGHTY):
					if (joyAxis[i] > right_stick_dead_zone) {
						joyAxisDir[i] = 1;
					}
					else if (joyAxis[i] < -right_stick_dead_zone) {
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
		}
		break;
		case(SDL_CONTROLLER_BUTTON_DPAD_DOWN):
		case(SDL_CONTROLLER_BUTTON_DPAD_LEFT):
		case(SDL_CONTROLLER_BUTTON_DPAD_RIGHT):
		case(SDL_CONTROLLER_BUTTON_DPAD_UP):
			break;
		case(SDL_CONTROLLERBUTTONDOWN):
		case(SDL_CONTROLLERBUTTONUP):
				for (unsigned int i = 0; i < SDL_CONTROLLER_BUTTON_MAX; i++) {
					joyButtons[i] = SDL_GameControllerGetButton(gameController, (SDL_GameControllerButton)i);
					if (joyButtons[i] == 1) {
						SDL_GameControllerButtonBind b = SDL_GameControllerGetBindForButton(gameController, (SDL_GameControllerButton)i);
						if (b.bindType == SDL_CONTROLLER_BINDTYPE_BUTTON) {
							std::cout << SDL_GameControllerGetStringForButton((SDL_GameControllerButton)i) << "(" << (int)b.value.button << ")" << std::endl;
						}
						else if (b.bindType == SDL_CONTROLLER_BINDTYPE_HAT){
							std::cout << SDL_GameControllerGetStringForButton((SDL_GameControllerButton)i) << "(" << (int)b.value.hat.hat << "." << (int)b.value.hat.hat_mask << ")" << std::endl;
						}
					}
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

	SDL_free(const_cast<char*>(mapping));
	SDL_GameControllerClose(gameController);

	if (SDL_GameControllerGetAttached(gameController)) {
		std::cout << "Game controller " << id << " still active!" << std::endl;
		std::string message = "Game controller " + std::to_string(id) + " still acitve";
		Debug::SetSeverity(MessageType::TYPE_WARNING);
		Debug::Warning(message, __FILE__, __LINE__);
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


void GameController::Rebind()
{
	SDL_Event e;
	__int8 toRebind = -1;
	std::string bs1;
	std::string bs2;
	SDL_GameControllerButtonBind toRebindBind;

	std::cout << "Rebind for controller " << id << std::endl;
	std::cout << "Enter axis/button to rebind" << std::endl;

	while (true) {
			SDL_PollEvent(&e);

			//Set to rebind
			if (e.type == SDL_CONTROLLERBUTTONDOWN && toRebind == -1 && e.cdevice.which == id) {
				toRebind = e.cbutton.button;
				toRebindBind = SDL_GameControllerGetBindForButton(gameController, (SDL_GameControllerButton)toRebind);
				if (toRebindBind.bindType == SDL_CONTROLLER_BINDTYPE_BUTTON) {
					std::cout << "Button set to remap: " << SDL_GameControllerGetStringForButton((SDL_GameControllerButton)toRebind) << "(" << (int)toRebindBind.value.button << ")" << std::endl;
				}
				else {
					std::cout << "Button set to remap: " << SDL_GameControllerGetStringForButton((SDL_GameControllerButton)toRebind) << "(" << (int)toRebindBind.value.hat.hat << "." << (int)toRebindBind.value.hat.hat_mask << ")" << std::endl;
				}
			}
			//Set to rebind
			else if (e.type == SDL_CONTROLLERAXISMOTION && toRebind == -1 && e.cdevice.which == id) {
				if (e.caxis.axis == SDL_CONTROLLER_AXIS_LEFTX && e.caxis.value > left_stick_dead_zone || e.caxis.value < -left_stick_dead_zone || e.caxis.axis == SDL_CONTROLLER_AXIS_LEFTY && e.caxis.value > left_stick_dead_zone || e.caxis.value < -left_stick_dead_zone ||
					e.caxis.axis == SDL_CONTROLLER_AXIS_RIGHTX && e.caxis.value > right_stick_dead_zone || e.caxis.value < -right_stick_dead_zone || e.caxis.axis == SDL_CONTROLLER_AXIS_RIGHTY && e.caxis.value > right_stick_dead_zone || e.caxis.value < -right_stick_dead_zone ||
					e.caxis.axis == SDL_CONTROLLER_AXIS_TRIGGERLEFT || e.caxis.axis == SDL_CONTROLLER_AXIS_TRIGGERRIGHT && e.caxis.value > 1000 ) {
					toRebind = e.caxis.axis;

					toRebindBind = SDL_GameControllerGetBindForAxis(gameController, (SDL_GameControllerAxis)toRebind);
					std::cout << "Axis set to remap: " << SDL_GameControllerGetStringForAxis((SDL_GameControllerAxis)toRebind) << "(" << (int)toRebindBind.value.axis << ")" << std::endl;
					
				}
			}
			//Rebind for button
			else if (e.type == SDL_CONTROLLERBUTTONDOWN && toRebind != -1 && e.cdevice.which == id) {
				SDL_GameControllerButtonBind newbuttonBind = SDL_GameControllerGetBindForButton(gameController, (SDL_GameControllerButton)e.cbutton.button);

				if (newbuttonBind.bindType == SDL_CONTROLLER_BINDTYPE_BUTTON) {
					std::cout << "Remapping with button " << SDL_GameControllerGetStringForButton((SDL_GameControllerButton)e.cbutton.button) << "(" << (int)newbuttonBind.value.button << ")" << std::endl;
				}
				else {
					std::cout << "Remapping with button " << SDL_GameControllerGetStringForButton((SDL_GameControllerButton)e.cbutton.button) << "(" << (int)newbuttonBind.value.hat.hat << "." << (int)newbuttonBind.value.hat.hat_mask << ")" << std::endl;
				}

				std::string newMapping = mapping;

				bs1 = ",";
				if (toRebindBind.bindType == SDL_CONTROLLER_BINDTYPE_AXIS) {
					bs1 += SDL_GameControllerGetStringForAxis((SDL_GameControllerAxis)toRebind);
				}
				else if(toRebindBind.bindType == SDL_CONTROLLER_BINDTYPE_BUTTON || toRebindBind.bindType == SDL_CONTROLLER_BINDTYPE_HAT){
					bs1 += SDL_GameControllerGetStringForButton((SDL_GameControllerButton)toRebind);
				}
				bs1 += ":";
				int b1 = newMapping.find(bs1);

				bs2 = ",";
				bs2 += SDL_GameControllerGetStringForButton((SDL_GameControllerButton)e.cbutton.button);
				bs2 += ":";
				int b2 = newMapping.find(bs2);

				//Replace to rebind
				if (newbuttonBind.bindType == SDL_CONTROLLER_BINDTYPE_BUTTON) {
					if (toRebindBind.bindType != SDL_CONTROLLER_BINDTYPE_HAT) {
						newMapping.replace(b1 + bs1.size(), 2, "b" + std::to_string((int)newbuttonBind.value.button));
					}
					else {
						newMapping.replace(b1 + bs1.size(), 4, "b" + std::to_string((int)newbuttonBind.value.button));
					}
				}
				else if (newbuttonBind.bindType == SDL_CONTROLLER_BINDTYPE_HAT) {
					newMapping.replace(b1 + bs1.size(), 2, "h" + std::to_string((int)newbuttonBind.value.hat.hat));
					if (toRebindBind.bindType != SDL_CONTROLLER_BINDTYPE_HAT) {
						newMapping.replace(b1 + bs1.size() + 2, 1, "." + std::to_string((int)newbuttonBind.value.hat.hat_mask) + ",");
					}
					else {
						newMapping.replace(b1 + bs1.size() + 2, 2, "." + std::to_string((int)newbuttonBind.value.hat.hat_mask));
					}
				}

				b1 = newMapping.find(bs1);
				b2 = newMapping.find(bs2);

				//Replace new bind
				if (toRebindBind.bindType == SDL_CONTROLLER_BINDTYPE_AXIS) {
					if (newbuttonBind.bindType != SDL_CONTROLLER_BINDTYPE_HAT) {
						newMapping.replace(b2 + bs2.size(), 2, "a" + std::to_string((int)toRebindBind.value.axis) + ",");
					}
					else {
						newMapping.replace(b2 + bs2.size(), 4, "a" + std::to_string((int)toRebindBind.value.axis));
					}
				}
				else if(toRebindBind.bindType == SDL_CONTROLLER_BINDTYPE_BUTTON){
					if (newbuttonBind.bindType != SDL_CONTROLLER_BINDTYPE_HAT) {
						newMapping.replace(b2 + bs2.size(), 2, "b" + std::to_string((int)toRebindBind.value.button) + ",");
					}
					else {
						newMapping.replace(b2 + bs2.size(), 4, "b" + std::to_string((int)toRebindBind.value.button));
					}
				}
				else {
					newMapping.replace(b2 + bs2.size(), 2, "h" + std::to_string((int)toRebindBind.value.hat.hat));
					if (newbuttonBind.bindType != SDL_CONTROLLER_BINDTYPE_HAT) {
						newMapping.replace(b2 + bs2.size() + 2, 1, "." + std::to_string((int)toRebindBind.value.hat.hat_mask) + ",");
					}
					else {
						newMapping.replace(b2 + bs2.size() + 2, 2, "." + std::to_string((int)toRebindBind.value.hat.hat_mask));
					}
				}

				int thing = SDL_GameControllerAddMapping(newMapping.c_str());
				if (thing == 0) {
					std::cout << "Controller " << id << " updated" << std::endl;
				}
				else if (thing == 1) {
					std::cout << "Controller " << id << " has added a new mapping" << std::endl;
				}
				else {
					std::cout << "An error occured updating controller " << id << std::endl;
					SDL_GetError();
				}

				mapping = SDL_GameControllerMappingForGUID(SDL_JoystickGetGUID(joy));

				break;
			}
			//Rebind for axis
			else if (e.type == SDL_CONTROLLERAXISMOTION && toRebind != -1 && e.cdevice.which == id && e.caxis.axis != toRebind) {
				if ((SDL_GameControllerAxis)e.caxis.axis == SDL_CONTROLLER_AXIS_TRIGGERLEFT && e.caxis.value > 0 || (SDL_GameControllerAxis)e.caxis.axis == SDL_CONTROLLER_AXIS_TRIGGERRIGHT && e.caxis.value > 0 ||
					(SDL_GameControllerAxis)e.caxis.axis == SDL_CONTROLLER_AXIS_LEFTX && e.caxis.value > left_stick_dead_zone || e.caxis.value < -left_stick_dead_zone || (SDL_GameControllerAxis)e.caxis.axis == SDL_CONTROLLER_AXIS_LEFTY && e.caxis.value > left_stick_dead_zone || e.caxis.value < -left_stick_dead_zone ||
					(SDL_GameControllerAxis)e.caxis.axis == SDL_CONTROLLER_AXIS_RIGHTX && e.caxis.value > right_stick_dead_zone || e.caxis.value < -right_stick_dead_zone || (SDL_GameControllerAxis)e.caxis.axis == SDL_CONTROLLER_AXIS_RIGHTY && e.caxis.value > right_stick_dead_zone || e.caxis.value < -right_stick_dead_zone) {

					//The button to rebind to
					SDL_GameControllerButtonBind newbuttonBind = SDL_GameControllerGetBindForAxis(gameController, (SDL_GameControllerAxis)e.caxis.axis);

					std::cout << "Remapping with axis " << SDL_GameControllerGetStringForAxis((SDL_GameControllerAxis)e.caxis.axis) << "(" << (int)newbuttonBind.value.axis<< ")" << std::endl;

					std::string newMapping = mapping;

					bs1 = ",";
					if (toRebindBind.bindType == SDL_CONTROLLER_BINDTYPE_AXIS){
						bs1 += SDL_GameControllerGetStringForAxis((SDL_GameControllerAxis)toRebind);
					}
					else {
						bs1 += SDL_GameControllerGetStringForButton((SDL_GameControllerButton)toRebind);
					}

					bs1 += ":";
					int b1 = newMapping.find(bs1);

					bs2 = ",";
					bs2 += SDL_GameControllerGetStringForAxis((SDL_GameControllerAxis)e.caxis.axis);
					bs2 += ":";
					int b2 = newMapping.find(bs2);

					//Replace to rebind
					if (toRebindBind.bindType != SDL_CONTROLLER_BINDTYPE_HAT) {
						newMapping.replace(b1 + bs1.size(), 2, "a" + std::to_string((int)newbuttonBind.value.axis));
					}
					else {
						newMapping.replace(b1 + bs1.size(), 4, "a" + std::to_string((int)newbuttonBind.value.axis));
					}

					b1 = newMapping.find(bs1);
					b2 = newMapping.find(bs2);

					//Replace new bind
					if (toRebindBind.bindType == SDL_CONTROLLER_BINDTYPE_AXIS) {
						newMapping.replace(b2 + bs2.size(), 2, "a" + std::to_string((int)toRebindBind.value.axis));
					}
					else if(toRebindBind.bindType == SDL_CONTROLLER_BINDTYPE_BUTTON) {
						newMapping.replace(b2 + bs2.size(), 2, "b" + std::to_string((int)toRebindBind.value.button));
					}
					else {
						newMapping.replace(b2 + bs2.size(), 2, "h" + std::to_string((int)toRebindBind.value.hat.hat) + "." + std::to_string((int)toRebindBind.value.hat.hat_mask));
					}

					int thing = SDL_GameControllerAddMapping(newMapping.c_str());
					if (thing == 0) {
						std::cout << "Controller " << id << " updated" << std::endl;
					}
					else if (thing == 1) {
						std::cout << "Controller " << id << " has added a new mapping" << std::endl;
					}
					else {
						std::cout << "An error occured updating controller " << id << std::endl;
						SDL_GetError();
					}

					mapping = SDL_GameControllerMappingForGUID(SDL_JoystickGetGUID(joy));

					break;
				}
			}
	}
}

void GameController::Rebind(SDL_GameControllerAxis axis, SDL_GameControllerAxis axis1)
{
	SDL_GameControllerButtonBind a = SDL_GameControllerGetBindForAxis(gameController, axis);
	SDL_GameControllerButtonBind a1 = SDL_GameControllerGetBindForAxis(gameController, axis1);


	std::string newMapping = SDL_GameControllerMapping(gameController);


	std::string sa = ",";
	sa += SDL_GameControllerGetStringForAxis(axis);
	sa += ":";
	int loc = newMapping.find(sa);

	std::string sa1 = ",";
	sa1 += SDL_GameControllerGetStringForAxis(axis1);
	sa1 += ":";
	int loc1 = newMapping.find(sa1);


	newMapping.replace(loc + sa.size(), 2, "a" + std::to_string(a1.value.axis));
	newMapping.replace(loc1 + sa1.size(), 2, "a" + std::to_string(a.value.axis));

	int thing = SDL_GameControllerAddMapping(newMapping.c_str());
	if (thing == 0) {
		std::cout << "Controller " << id << " updated" << std::endl;
	}
	else if (thing == 1) {
		std::cout << "Controller " << id << " has added a new mapping" << std::endl;
	}
	else {
		std::cout << "An error occured updating controller " << id << std::endl;
		SDL_GetError();
	}

	mapping = SDL_GameControllerMappingForGUID(SDL_JoystickGetGUID(joy));
}

void GameController::Rebind(SDL_GameControllerAxis axis, SDL_GameControllerButton button)
{
	SDL_GameControllerButtonBind a = SDL_GameControllerGetBindForAxis(gameController, axis);
	SDL_GameControllerButtonBind b = SDL_GameControllerGetBindForButton(gameController, button);


	std::string newMapping = SDL_GameControllerMapping(gameController);


	std::string sa = ",";
	sa += SDL_GameControllerGetStringForAxis(axis);
	sa += ":";
	int loca = newMapping.find(sa);

	std::string sb = ",";
	sb += SDL_GameControllerGetStringForButton(button);
	sb += ":";
	int locb = newMapping.find(sb);


	newMapping.replace(loca + sa.size(), 2, "b" + std::to_string(b.value.button));
	newMapping.replace(locb + sb.size(), 2, "a" + std::to_string(a.value.axis));

	int thing = SDL_GameControllerAddMapping(newMapping.c_str());
	if (thing == 0) {
		std::cout << "Controller " << id << " updated" << std::endl;
	}
	else if (thing == 1) {
		std::cout << "Controller " << id << " has added a new mapping" << std::endl;
	}
	else {
		std::cout << "An error occured updating controller " << id << std::endl;
		SDL_GetError();
	}

	mapping = SDL_GameControllerMappingForGUID(SDL_JoystickGetGUID(joy));
}

void GameController::Rebind(SDL_GameControllerButton button, SDL_GameControllerButton button1)
{
	SDL_GameControllerButtonBind b = SDL_GameControllerGetBindForButton(gameController, button);
	SDL_GameControllerButtonBind b1 = SDL_GameControllerGetBindForButton(gameController, button1);


	std::string newMapping = SDL_GameControllerMapping(gameController);


	std::string sb = ",";
	sb += SDL_GameControllerGetStringForButton(button);
	sb += ":";
	int locb = newMapping.find(sb);

	std::string sb1 = ",";
	sb1 += SDL_GameControllerGetStringForButton(button);
	sb1 += ":";
	int locb1 = newMapping.find(sb1);


	newMapping.replace(locb + sb.size(), 2, "b" + std::to_string(b1.value.button));
	newMapping.replace(locb1 + sb1.size(), 2, "b" + std::to_string(b.value.axis));

	int thing = SDL_GameControllerAddMapping(newMapping.c_str());
	if (thing == 0) {
		std::cout << "Controller " << id << " updated" << std::endl;
	}
	else if (thing == 1) {
		std::cout << "Controller " << id << " has added a new mapping" << std::endl;
	}
	else {
		std::cout << "An error occured updating controller " << id << std::endl;
		SDL_GetError();
	}

	mapping = SDL_GameControllerMappingForGUID(SDL_JoystickGetGUID(joy));
}

void GameController::Rebind(SDL_GameControllerButton button, SDL_GameControllerAxis axis)
{
	SDL_GameControllerButtonBind b = SDL_GameControllerGetBindForButton(gameController, button);
	SDL_GameControllerButtonBind a = SDL_GameControllerGetBindForAxis(gameController, axis);


	std::string newMapping = SDL_GameControllerMapping(gameController);

	std::string sb = ",";
	sb += SDL_GameControllerGetStringForButton(button);
	sb += ":";
	int locb = newMapping.find(sb);

	std::string sa = ",";
	sa += SDL_GameControllerGetStringForAxis(axis);
	sa += ":";
	int loca = newMapping.find(sa);

	newMapping.replace(locb + sb.size(), 2, "a" + std::to_string(a.value.axis));
	newMapping.replace(loca + sa.size(), 2, "b" + std::to_string(b.value.button));

	int thing = SDL_GameControllerAddMapping(newMapping.c_str());
	if (thing == 0) {
		std::cout << "Controller " << id << " updated" << std::endl;
	}
	else if (thing == 1) {
		std::cout << "Controller " << id << " has added a new mapping" << std::endl;
	}
	else {
		std::cout << "An error occured updating controller " << id << std::endl;
		SDL_GetError();
	}

	mapping = SDL_GameControllerMappingForGUID(SDL_JoystickGetGUID(joy));
}

