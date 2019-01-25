#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Window;

enum class CameraMovement : unsigned __int8 {
	FORWARD = 0,
	BACKWARD = 1,
	RIGHT = 3,
	LEFT = 4,
	TOTAL
};

class Camera
{
	glm::mat4 cameraProjection = glm::mat4();

	//Default values
	const float YAW = -90.0f;
	const float PITCH = 0.0f;
	const float ROLL = 0.0f;
	const float SPEED = 2.5f;
	const float MOUSE_SENSITIVITY = 0.3f;
	const float CONTROLLER_SENSITIVITY = 1.5f;
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
	float ControllerSensitivity;
	float FOV;

	//Options
	bool isYawInvert = false;
	bool isPitchInvert = true;

public:
	Camera(Window* w, float near = 0.1f, float far = 100.0f);
	Camera(float l, float r, float b, float t, float near, float far);
	~Camera();
	Camera(glm::vec3 pos,glm::vec3 forward,glm::vec3 up,glm::vec3 right,glm::vec3 worldup,float yaw,float pitch,float movespeed,float sensitivity,float c_sensitivity,float fov, Window* w,float near, float far);
	Camera(glm::vec3 pos, glm::vec3 forward, glm::vec3 up, glm::vec3 right, glm::vec3 worldup, float yaw, float pitch, float movespeed, float sensitivity, float c_sensitivity, float fov, float l, float r, float b, float t, float near, float far);
	glm::mat4 GetViewMatrix() { return glm::lookAt(Position, Position + Forward, Up); /*position,pointing forward(-Z axis),up*/}
	void Keyboard(CameraMovement direction, float deltaTime);
	void Controller(float xvalue,float zvalue, float xoffset, float yoffset, float deltaTime, bool constrainPitch = true);
	void MouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
	void MouseScroll(float offset);

	bool Init();
	void Update();
	void Render();
	bool Shutdown();

	void InvertYaw() { isYawInvert = isYawInvert ? false : true; }
	void InvertPitch() { isPitchInvert = isPitchInvert ? false : true; }

	//Getters
	inline glm::vec3 GetPosition() { return Position; }
	glm::mat4 GetPerspective(Window& w, float near, float far);
	glm::mat4 GetOrtho(float left, float right, float bottom, float top);
	glm::mat4 GetOrtho(float left, float right, float bottom, float top, float near, float far);
	inline glm::mat4 GetProjectionMatrix() { return cameraProjection; }
	float GetFov() { return FOV; }

	//Setters
	void SetSensitivity(float sensitivity) { MouseSensitivity = sensitivity; }
	void SetFov(float fov) { FOV = fov; }
	void Translate(glm::vec3 position) { Position = position; }
	void Rotate(float yaw, float pitch) { Yaw = yaw; Pitch = pitch; }

};
#endif // !CAMERA_H