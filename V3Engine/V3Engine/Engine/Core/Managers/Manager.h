#ifndef MANAGER_H
#define MANAGER_H

template <class T>
class Manager {
	static T* instance;
protected:
	Manager();
	virtual ~Manager() {
		instance = nullptr;
		delete instance;
	}
	virtual void Update() = 0;
public:

	Manager(const Manager&) = delete;
	Manager(Manager&&) = delete;

	static T* GetInstace() {
		if (instance != nullptr) {
			return instance;
		}
		else {
			instance = new T();
			return instance;
		}
	}
};
template<class T>
T* Manager<T>::instance = nullptr;

#endif
