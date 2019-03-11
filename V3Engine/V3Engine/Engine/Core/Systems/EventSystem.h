#ifndef EVENT_SYSTEM_H
#define EVENT_SYSTEM_H

#include "System.h"
#include <SDL_events.h>

class EventSystem : public System {
public:
	EventSystem() {

	}
	virtual ~EventSystem() {

	}

	virtual void Update(SDL_Event& e) {

	}
	virtual bool Shutdown() {
		return true;
	}

};

#endif // !EVENT_SYSTEM_H

