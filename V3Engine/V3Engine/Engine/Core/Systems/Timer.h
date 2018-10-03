#ifndef TIMER_H
#define TIMER_H

#include <SDL.h>

class Timer {
private:
	unsigned int prevTicks;
	unsigned int currentTicks;

public:
	Timer(const Timer&) = delete;
	Timer(Timer&&) = delete;
	Timer& operator=(const Timer&) = delete;
	Timer& operator=(Timer&&) = delete;

	Timer();
	~Timer();

	void Start();
	void UpdateFrameTicks();
	float GetDeltaTime() const;
	unsigned int GetSleepTime(const unsigned int _fps) const;
	float GetCurrentTicks() const;
};

#endif // ! TIMER_H