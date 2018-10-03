#include "Timer.h"



Timer::Timer() : prevTicks(0), currentTicks(0) {

}


Timer::~Timer() {

}

void Timer::Start() {

	prevTicks = SDL_GetTicks();
	currentTicks = SDL_GetTicks();
}

void Timer::UpdateFrameTicks() {

	prevTicks = currentTicks;
	currentTicks = SDL_GetTicks();
}

float Timer::GetDeltaTime() const {

	return (float)((currentTicks - prevTicks) / 1000.0f);
}

unsigned int Timer::GetSleepTime(const unsigned int _fps) const {

	unsigned int milliSecPerFrame = 1000 / _fps;
	if (milliSecPerFrame == 0)
		return 0;
	unsigned int sleepTime = milliSecPerFrame - SDL_GetTicks();
	if (sleepTime > milliSecPerFrame)
		return milliSecPerFrame;

	return sleepTime;
}

float Timer::GetCurrentTicks() const {

	return (float)(currentTicks / 1000.0f);
}