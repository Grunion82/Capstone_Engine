#ifndef MANAGER_H
#define MANAGER_H

template <class T>
class Manager {
	static T* instance;
protected:
	Manager() {}
	virtual ~Manager() {}
public:
	virtual bool Init() = 0;
	virtual void Update() = 0;
	virtual bool Shutdown() = 0;

	static T* GetInstance() {
		if (m_instance != nullptr) {
			return *instance;
		}
		else {
			instance = new T();
			return *instance;
		}
	}

};
template <class T>
T* Manager<T>::instance = nullptr;

#endif // !MANAGER_H
