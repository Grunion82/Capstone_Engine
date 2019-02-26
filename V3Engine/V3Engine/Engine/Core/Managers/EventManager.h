#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include "Manager.h"
#include <vector>

class EventSystem;

class EventManager : public Manager<EventManager>
{
	std::vector<EventSystem*> eventSystems;

public:
	EventManager();
	~EventManager();

	void Update() override;
	void Shutdown();

	template <class ES>
	void AddEventSystem(ES* es) {
		if (dynamic_cast<EventSystem*>(es)) {
			eventSystems.push_back(es);
		}
	}

	void AddEventSystem(EventSystem* es);

	inline std::vector<EventSystem*> GetEventSystems() { return eventSystems; }
};

#endif // !EVENT_MANAGER_H
