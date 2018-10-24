#ifndef MANAGER_H
#define MANAGER_H

template <class T>
class Manager {
protected:
	Manager() {}
	virtual ~Manager() {}
public:
	virtual bool Init() = 0;
	virtual void Update() = 0;
	virtual bool Shutdown() = 0;
};

#endif // !MANAGER_H
