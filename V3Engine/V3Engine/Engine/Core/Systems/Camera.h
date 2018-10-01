#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


enum class CameraMovement : unsigned __int8 {
	FORWARD = 0,
	BACKWARD = 1,
	RIGHT = 3,
	LEFT = 4,
	TOTAL
};

class Camera
{
	//Default values
	const float YAW = -90.0f;
	const float PITCH = 0.0f;
	const float ROLL = 0.0f;
	const float SPEED = 2.5f;
	const float MOUSE_SENSITIVITY = 0.3f;
	const float FIELD_OF_VIEW = 45.0f;

	glm::vec3 Position;
	glm::vec3 Forward;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	//Euler angles
	float Yaw;
	float Pitch;
	float Roll;

	float MovementSpeed;
	float MouseSensitivity;
	float FOV;

	//Options
	bool isYawInvert = false;
	bool isPitchInvert = true;

public:
	Camera();
	~Camera();
	Camera(glm::vec3 pos,glm::vec3 forward,glm::vec3 up,glm::vec3 right,glm::vec3 worldup,float yaw,float pitch,float movespeed,float sensitivity,float fov);
	glm::mat4 GetViewMatrix() { return glm::lookAt(Position, Position + Forward, Up); /*position,pointing forward(-Z axis),up*/}
	void Keyboard(CameraMovement direction, float deltaTime);
	void Controller(float xvalue,float zvalue, float deltaTime);
	void MouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
	void MouseScroll(float offset);

	bool Init();
	void Update();
	void Render();
	bool Shutdown();

	void InvertYaw() { isYawInvert = isYawInvert ? false : true; }
	void InvertPitch() { isPitchInvert = isPitchInvert ? false : true; }

	//Getters
	glm::vec3 GetPosition() { return Position; }
	float GetFov() { return FOV; }

	//Setters
	void SetSensitivity(float sensitivity) { MouseSensitivity = sensitivity; }
	void SetFov(float fov) { FOV = fov; }
	void Translate(glm::vec3 position) { Position = position; }
	void Rotate(float yaw, float pitch) { Yaw = yaw; Pitch = pitch; }

};
#endif // !CAMERA_H