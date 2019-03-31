#ifndef CAMERA_H
#define CAMERA_H

//Default values
#define YAW -90.0f
#define PITCH 0.0f
#define ROLL 0.0f
#define SPEED 2.5f
#define MOUSE_SENSITIVITY 0.3f
#define CONTROLLER_SENSITIVITY 1.5f
#define FIELD_OF_VIEW 45.0f

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SDL_rect.h>



class Window;

class Camera
{
	glm::mat4 cameraProjection = glm::mat4();

	glm::vec3 Position;
	glm::vec3 Forward;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	glm::quat Quaternion;
	glm::vec3 eulerAngle;

	SDL_Rect cameraViewport;

	//Euler angles
	//float Pitch;
	//float Yaw;
	//float Roll;

	//PROPERTIES THAT SHOULD BE MOVED SOMEWHERE ELSE
	//===============================================
	float MovementSpeed;
	float MouseSensitivity;
	float ControllerSensitivity;
	//===============================================

	float FOV;

	//Options
	bool isYawInvert = false;
	bool isPitchInvert = true;

public:
	//Generic camera with a perspective matrix
	Camera(Window* window, float near = 0.1f, float far = 100.0f);
	//Generic camera with an orthographic matrix
	Camera(float left, float right, float bottom, float top, float near, float far);
	//Camera with perspective matrix and more specific properties
	Camera(glm::vec3 pos,glm::vec3 forward,glm::vec3 up,glm::vec3 right,glm::vec3 worldup,glm::vec3 orientation,float movespeed,float sensitivity,float c_sensitivity,float fov, Window* w,float near, float far);
	Camera(glm::vec3 pos, glm::vec3 forward, glm::vec3 up, glm::vec3 right, glm::vec3 worldup, glm::quat orientation, float movespeed, float sensitivity, float c_sensitivity, float fov, Window* w, float near, float far);
	//Camera with orthographic matrix and more specific properties
	Camera(glm::vec3 pos, glm::vec3 forward, glm::vec3 up, glm::vec3 right, glm::vec3 worldup, glm::vec3 orientation, float movespeed, float sensitivity, float c_sensitivity, float fov, float l, float r, float b, float t, float near, float far);
	Camera(glm::vec3 pos, glm::vec3 forward, glm::vec3 up, glm::vec3 right, glm::vec3 worldup, glm::quat orientation, float movespeed, float sensitivity, float c_sensitivity, float fov, float l, float r, float b, float t, float near, float far);
	glm::mat4 GetViewMatrix() { return glm::lookAt(Position, Position + Forward, Up); /*position,pointing forward(-Z axis),up*/}
	glm::mat4 GetViewMatrix() const { return glm::lookAt(Position, Position + Forward, Up); /*position,pointing forward(-Z axis),up*/}

	void SetCameraViewport(SDL_Rect& rect) { cameraViewport = rect; }
	void SetCameraViewport(float x, float y, float w, float h) { cameraViewport.x = x; cameraViewport.y = y; cameraViewport.w = w; cameraViewport.h = h; }

	~Camera();
	//For keyboard input
	void Keyboard(float forward, float right, float deltaTime);
	//For controller input
	void Controller(float xvalue,float zvalue, float xoffset, float yoffset, float deltaTime, bool constrainPitch = true);
	//For rotation
	void MouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
	//For FOV mainpulation
	void MouseScroll(float offset);

	bool Init();
	void Update();
	void Render();
	bool Shutdown();

	void InvertYaw() { isYawInvert = isYawInvert ? false : true; }
	void InvertPitch() { isPitchInvert = isPitchInvert ? false : true; }

	//GETTERS

	inline glm::vec3 GetPosition() const { return Position; }
	//Get a perspective matrix based off this camera
	glm::mat4 GetPerspective(Window& w, float near, float far);
	//Get an orthographic matrix based off this camera
	glm::mat4 GetOrtho(float left, float right, float bottom, float top);
	//Get an orthographic matrix based off this camera
	glm::mat4 GetOrtho(float left, float right, float bottom, float top, float near, float far);
	inline glm::mat4 GetProjectionMatrix() const { return cameraProjection; }
	float GetFov() { return FOV; }

	//SETTERS

	void SetSensitivity(float sensitivity) { MouseSensitivity = sensitivity; }
	void SetPosition(glm::vec3 position) { Position = position; }
	void SetEulerAngle(glm::vec3 rotation) { eulerAngle = rotation; }
	void SetQuaternion(glm::quat quaternion) { Quaternion = quaternion; }
	void SetFov(float fov) { FOV = fov; }
	void Translate(glm::vec3 position) { Position += position; }
	void Rotate(glm::vec3 rotation) { eulerAngle += rotation; }
	void Rotate(float x, float y, float z) { eulerAngle.x += x; eulerAngle += y; eulerAngle += z;}
	void Rotate(glm::quat rotation) { Quaternion += rotation; }
};
#endif // !CAMERA_H