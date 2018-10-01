#include "Input.h"
#include <SDL.h>
#include <stdio.h>
//#include "imgui/imgui.h"
//#include "imgui/imgui_impl_sdl_gl3.h"

#include <iostream>

#define MAX(a,b) a > b ? a : b
#define MIN(a,b) a < b ? a : b
#define CLAMP(x,upper,lower) MIN(upper,MAX(x,lower))


Input::Input() : requestedQuit(false)
{
	
}

Input::~Input()
{
}

bool Input::Init() {
	SDL_InitSubSystem(SDL_INIT_JOYSTICK);

	//Check for joysticks
	for (int i = 0; i < SDL_NumJoysticks(); i++) {
		joy = SDL_JoystickOpen(i);

		if (joy) {
			std::cout << "Opened joystick " << i << std::endl;
			std::cout << "Name: " << SDL_JoystickNameForIndex(i) << std::endl;
			std::cout << "Id: " << SDL_JoystickInstanceID(joy) << std::endl;
			std::cout << "Number of hats: " << SDL_JoystickNumHats(joy) << std::endl;
			std::cout << "Number of axes: " << SDL_JoystickNumAxes(joy) << std::endl;
			std::cout << "Number of buttons: " << SDL_JoystickNumButtons(joy) << std::endl;
			std::cout << "Number of balls: " << SDL_JoystickNumBalls(joy) << std::endl;
		}
		else {
			std::cout << "Failed to open joystick " << i << std::endl;
			std::cout << SDL_GetError() << std::endl;
			//***************Remove later!!
			return false;
		}
	}


	return true;
}
void Input::Update() {
	oldkeys = keys;
	
	mouseMotionX = 0;
	mouseMotionY = 0;
	mouseWheelY = 0;

	joystickLeftAxisX = 0;
	joystickLeftAxisY = 0;
	joystickRightAxisX = 0;
	joystickRightAxisY = 0;

	SDL_Event e;

	while (SDL_PollEvent(&e) != 0) {
		//Check for event type
		switch (e.type) {
		//Send to UI
		//ImGui_ImplSdlGL3_ProcessEvent(&e);
		//Is key down
		case(SDL_KEYDOWN):
			//Check for key
			keys[e.key.keysym.sym] = true;
			break;
		//For mouse movement
		case(SDL_MOUSEMOTION):
			mouseMotionX = e.motion.xrel;
			mouseMotionY = e.motion.yrel;
			break;
		//For mouse wheel
		case(SDL_MOUSEWHEEL):
			mouseWheelY = e.wheel.y;
			break;
		//Is key up
		case(SDL_KEYUP):
			keys[e.key.keysym.sym] = false;
			break;
		//For controller axis
		case(SDL_JOYAXISMOTION):
			//Value for axis ranges form -32768 to 32767
			//Some joy sticks use 2 and 3 for extra buttons
			//Left x
			if (e.jaxis.axis == 0 && e.jaxis.value > JOYSTICK_DEAD_ZONE || e.jaxis.value < -JOYSTICK_DEAD_ZONE) {
				joystickLeftAxisX = CLAMP(e.jaxis.value, 10, -10);
			}
			//Left y
			else if (e.jaxis.axis == 1 && e.jaxis.value > JOYSTICK_DEAD_ZONE || e.jaxis.value < -JOYSTICK_DEAD_ZONE) {
				joystickLeftAxisY = CLAMP(e.jaxis.value, 10, -10);
			}

			//Right x
			if (e.jaxis.axis == 3 && e.jaxis.value > JOYSTICK_DEAD_ZONE || e.jaxis.value < -JOYSTICK_DEAD_ZONE) {
				joystickRightAxisX = CLAMP(e.jaxis.value, 10, -10);
			}
			//Right y
			else if (e.jaxis.axis == 4 && e.jaxis.value > JOYSTICK_DEAD_ZONE || e.jaxis.value < -JOYSTICK_DEAD_ZONE) {
				joystickRightAxisY = CLAMP(e.jaxis.value, 10, -10);
			}

			std::cout << e.jaxis.value << std::endl;
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
	if (SDL_JoystickGetAttached(joy)) {
		SDL_JoystickClose(joy);
	}
	//delete joy;

	keys.clear();
	oldkeys.clear();

	return true;
}

DirectionInput Input::getDirectionInput() {
	return dr;
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

