#include "Input.h"
#include <SDL.h>
#include <stdio.h>
//#include "imgui/imgui.h"
//#include "imgui/imgui_impl_sdl_gl3.h"

#include <iostream>

#define MAX(a,b) a > b ? a : b
#define MIN(a,b) a < b ? a : b
#define CLAMP(x,upper,lower) MIN(upper,MAX(x,lower))

Input* Input::instance;

Input::Input() : requestedQuit(false)
{
}

Joystick::Joystick(SDL_Joystick * j, int deadzone) : joy(j), JOYSTICK_DEAD_ZONE(deadzone), id(SDL_JoystickInstanceID(j)), hats(SDL_JoystickNumHats(j)), axes(SDL_JoystickNumAxes(j)), buttons(SDL_JoystickNumButtons(j)), balls(SDL_JoystickNumBalls(j))
{
}

Input * Input::GetInstance()
{
	if (instance != nullptr) {
		return instance;
	}
	else {
		instance = new Input();
		return instance;
	}
}

Input::~Input()
{
}

Joystick::~Joystick() {

}

bool Input::Init() {
	SDL_InitSubSystem(SDL_INIT_JOYSTICK);

	if (SDL_NumJoysticks() > 0) {
		joysticks.reserve(SDL_NumJoysticks());
		//Check for joysticks
		for (int i = 0; i < SDL_NumJoysticks(); i++) {
			SDL_Joystick* j = SDL_JoystickOpen(i);
			if (j) {
				joysticks.push_back(Joystick(j, 8000));
				//joysticks.insert(joysticks.begin() + i, Joystick(j,8000));
				std::cout << "Opened joystick: " << i << std::endl;
				std::cout << "Name: " << SDL_JoystickNameForIndex(i) << std::endl;
				std::cout << "Id: " << SDL_JoystickInstanceID(j) << std::endl;
				std::cout << "Number of hats: " << SDL_JoystickNumHats(j) << std::endl;
				std::cout << "Number of axes: " << SDL_JoystickNumAxes(j) << std::endl;
				std::cout << "Number of buttons: " << SDL_JoystickNumButtons(j) << std::endl;
				std::cout << "Number of balls: " << SDL_JoystickNumBalls(j) << std::endl;
				std::cout << std::endl;

				//Check if joysticks are pointing in a direction
				Sint16 state;
				for (unsigned int a = 0; a < SDL_JoystickNumAxes(j); i++) {
					//If there is a change for joystick during init
					if (SDL_JoystickGetAxisInitialState(j, a, &state)) {
						switch (a)
						{
						case(0):
							joysticks.at(i).leftXAxis = state;
							if (state > joysticks.at(i).JOYSTICK_DEAD_ZONE) {
								joysticks.at(i).joystickLeftAxisX = 1;
							}
							else if (state < -joysticks.at(i).JOYSTICK_DEAD_ZONE) {
								joysticks.at(i).joystickLeftAxisX = -1;
							}
							else {
								joysticks.at(i).joystickLeftAxisX = 0;
							}
							break;
						case(1):
							joysticks.at(i).leftYAxis = state;
							if (state > joysticks.at(i).JOYSTICK_DEAD_ZONE) {
								joysticks.at(i).joystickLeftAxisY = 1;
							}
							else if (state < -joysticks.at(i).JOYSTICK_DEAD_ZONE) {
								joysticks.at(i).joystickLeftAxisY = -1;
							}
							else {
								joysticks.at(i).joystickLeftAxisY = 0;
							}
							break;
						case(3):
							joysticks.at(i).rightXAxis = state;
							if (state > joysticks.at(i).JOYSTICK_DEAD_ZONE) {
								joysticks.at(i).joystickRightAxisX = 1;
							}
							else if (state < -joysticks.at(i).JOYSTICK_DEAD_ZONE) {
								joysticks.at(i).joystickRightAxisX = -1;
							}
							else {
								joysticks.at(i).joystickRightAxisX = 0;
							}
							break;
						case(4):
							joysticks.at(i).rightYAxis = state;
							if (state > joysticks.at(i).JOYSTICK_DEAD_ZONE) {
								joysticks.at(i).joystickRightAxisY = 1;
							}
							else if (state < -joysticks.at(i).JOYSTICK_DEAD_ZONE) {
								joysticks.at(i).joystickRightAxisY = -1;
							}
							else {
								joysticks.at(i).joystickRightAxisY = 0;
							}
							break;
						default:
							break;
						}
					}
				}
			}
			else {
				std::cout << "Failed to open joystick " << i << std::endl;
				std::cout << SDL_GetError() << std::endl;
				//***************Remove later!!
				return false;
			}
		}
	}

	return true;
}
void Input::Update() {
	oldkeys = keys;
	oldMouseButtons = mouseButtons;

	for (unsigned int i = 0; i < joysticks.size(); i++){
		joysticks.at(i).oldJoyButtons = joysticks.at(i).joyButtons;
	}

	mouseMotionX = 0;
	mouseMotionY = 0;
	mouseWheelY = 0;


	SDL_Event e;

	while (SDL_PollEvent(&e) != 0) {
		//Check for event type
		switch (e.type) {
		//Send to UI
		//ImGui_ImplSdlGL3_ProcessEvent(&e);
		//Is key down
		case(SDL_KEYDOWN):
			//Check for key
			//oldkeys = keys;
			keys[e.key.keysym.sym] = true;
			break;
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
		//Is key up
		case(SDL_KEYUP):
			//oldkeys = keys;
			keys[e.key.keysym.sym] = false;
			break;
		//For controller axis
		case(SDL_JOYAXISMOTION):
			//Value for axis ranges form -32768 to 32767
			//Some joy sticks use 2 and 3 for extra buttons
			//Set axis values
			if (e.jaxis.axis == 0) {
				joysticks.at(e.jaxis.which).leftXAxis = e.jaxis.value;
			}
			if (e.jaxis.axis == 1) {
				joysticks.at(e.jaxis.which).leftYAxis = e.jaxis.value;
			}

			//Left x
			if (e.jaxis.axis == 0 && e.jaxis.value > joysticks.at(e.jaxis.which).JOYSTICK_DEAD_ZONE) {
				joysticks.at(e.jaxis.which).joystickLeftAxisX = 1;
			}
			else if (e.jaxis.axis == 0 && e.jaxis.value < -joysticks.at(e.jaxis.which).JOYSTICK_DEAD_ZONE) {
				joysticks.at(e.jaxis.which).joystickLeftAxisX = -1;
			}
			else if (joysticks.at(e.jaxis.which).leftXAxis < joysticks.at(e.jaxis.which).JOYSTICK_DEAD_ZONE && joysticks.at(e.jaxis.which).leftXAxis > -joysticks.at(e.jaxis.which).JOYSTICK_DEAD_ZONE) {
				joysticks.at(e.jaxis.which).joystickLeftAxisX = 0;
			}

			//Left y
			if (e.jaxis.axis == 1 && e.jaxis.value > joysticks.at(e.jaxis.which).JOYSTICK_DEAD_ZONE) {
				joysticks.at(e.jaxis.which).joystickLeftAxisY = 1;
			}
			else if (e.jaxis.axis == 1 && e.jaxis.value < -joysticks.at(e.jaxis.which).JOYSTICK_DEAD_ZONE) {
				joysticks.at(e.jaxis.which).joystickLeftAxisY = -1;
			}
			else if (joysticks.at(e.jaxis.which).leftYAxis < joysticks.at(e.jaxis.which).JOYSTICK_DEAD_ZONE && joysticks.at(e.jaxis.which).leftYAxis > -joysticks.at(e.jaxis.which).JOYSTICK_DEAD_ZONE) {
				joysticks.at(e.jaxis.which).joystickLeftAxisY = 0;
			}

			//Set axis values
			if (e.jaxis.axis == 3) {
				joysticks.at(e.jaxis.which).rightXAxis = e.jaxis.value;
			}
			if (e.jaxis.axis == 4) {
				joysticks.at(e.jaxis.which).rightYAxis = e.jaxis.value;
			}

			//Right x
			if (e.jaxis.axis == 3 && e.jaxis.value > joysticks.at(e.jaxis.which).JOYSTICK_DEAD_ZONE) {
				joysticks.at(e.jaxis.which).joystickRightAxisX = 1;
			}
			else if (e.jaxis.axis == 3 && e.jaxis.value < -joysticks.at(e.jaxis.which).JOYSTICK_DEAD_ZONE) {
				joysticks.at(e.jaxis.which).joystickRightAxisX = -1;
			}
			else if (joysticks.at(e.jaxis.which).rightXAxis < joysticks.at(e.jaxis.which).JOYSTICK_DEAD_ZONE && joysticks.at(e.jaxis.which).rightXAxis > -joysticks.at(e.jaxis.which).JOYSTICK_DEAD_ZONE) {
				joysticks.at(e.jaxis.which).joystickRightAxisX = 0;
			}

			//Right y
			if (e.jaxis.axis == 4 && e.jaxis.value > joysticks.at(e.jaxis.which).JOYSTICK_DEAD_ZONE) {
				joysticks.at(e.jaxis.which).joystickRightAxisY = 1;
			}
			else if (e.jaxis.axis == 4 && e.jaxis.value < -joysticks.at(e.jaxis.which).JOYSTICK_DEAD_ZONE) {
				joysticks.at(e.jaxis.which).joystickRightAxisY = -1;
			}
			else if (joysticks.at(e.jaxis.which).rightYAxis < joysticks.at(e.jaxis.which).JOYSTICK_DEAD_ZONE && joysticks.at(e.jaxis.which).rightYAxis > -joysticks.at(e.jaxis.which).JOYSTICK_DEAD_ZONE) {
				joysticks.at(e.jaxis.which).joystickRightAxisY = 0;
			}

			//Triggers start at -32768; a half pull is around the negatives and anything beyond that is positive up to 32767
			//Left trigger
			if (e.jaxis.axis == 2) {
				joysticks.at(e.jaxis.which).leftTrigger = e.jaxis.value;
			}

			if (e.jaxis.axis == 2 && e.jaxis.value > 0) {
				joysticks.at(e.jaxis.which).joystickLeftTrigger = 1;
			}
			else if (e.jaxis.axis == 2 && e.jaxis.value < 0 && e.jaxis.value > -32768) {
				joysticks.at(e.jaxis.which).joystickLeftTrigger = -1;
			}
			else if (e.jaxis.axis == 2 && e.jaxis.value == -32768) {
				joysticks.at(e.jaxis.which).joystickLeftTrigger = 0;
			}

			//Right trigger
			if (e.jaxis.axis == 5) {
				joysticks.at(e.jaxis.which).rightTrigger = e.jaxis.value;
			}

			if (e.jaxis.axis == 5 && e.jaxis.value > 0) {
				joysticks.at(e.jaxis.which).joystickRightTrigger = 1;
			}
			else if (e.jaxis.axis == 5 && e.jaxis.value < 0 && e.jaxis.value > -32768) {
				joysticks.at(e.jaxis.which).joystickRightTrigger = -1;
			}
			else if (e.jaxis.axis == 5 && e.jaxis.value == -32768) {
				joysticks.at(e.jaxis.which).joystickRightTrigger = 0;
			}
			break;
		case(SDL_JOYBUTTONDOWN):
			joysticks.at(e.jbutton.which).joyButtons[e.jbutton.button] = true;
			break;
		case(SDL_JOYBUTTONUP):
			joysticks.at(e.jbutton.which).joyButtons[e.jbutton.button] = false;
			break;
		//For joy hat motion(NOT IMPLEMENTED)
		/*case(SDL_JOYHATMOTION):
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
			}*/
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

			joysticks.at(i).joyButtons.clear();
			joysticks.at(i).oldJoyButtons.clear();
		}
		joysticks.clear();
	}

	keys.clear();
	oldkeys.clear();

	return true;
}

Joystick* Input::GetJoystick(unsigned int which)
{
	if (&joysticks[which]) {
		return &joysticks[which];
	}
	return nullptr;
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

//JOYSTICK=========================================================
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

