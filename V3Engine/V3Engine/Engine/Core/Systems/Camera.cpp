#include "Camera.h"
#include "Window.h"

Camera::Camera(Window* window, float near, float far) : Position(0.0f,0.0f,0.0f), Forward(0.0f,0.0f,-1.0f), Up(0.0f,1.0f,0.0f), Right(1.0f,0.0f,0.0f), WorldUp(0.0f,1.0f,0.0f), 
													    MovementSpeed(10.0f), ControllerSensitivity(CONTROLLER_SENSITIVITY),MouseSensitivity(MOUSE_SENSITIVITY), FOV(FIELD_OF_VIEW)
{
	cameraProjection = glm::perspective(FOV, (float)window->GetWidth() / (float)window->GetHeight(), near, far);
	eulerAngle.x = PITCH;
	eulerAngle.y = YAW;
	eulerAngle.z = ROLL;
}

Camera::Camera(float left, float right, float bottom, float top, float near, float far) : Position(0.0f, 0.0f, 0.0f), Forward(0.0f, 0.0f, -1.0f), Up(0.0f, 1.0f, 0.0f), Right(1.0f, 0.0f, 0.0f), WorldUp(0.0f, 1.0f, 0.0f),
																					      MovementSpeed(10.0f), ControllerSensitivity(CONTROLLER_SENSITIVITY), MouseSensitivity(MOUSE_SENSITIVITY), FOV(FIELD_OF_VIEW)
{
	cameraProjection = glm::ortho(left, right, bottom, top, near, far);
	eulerAngle.x = PITCH;
	eulerAngle.y = YAW;
	eulerAngle.z = ROLL;
}

Camera::Camera(glm::vec3 pos, glm::vec3 forward, glm::vec3 up, glm::vec3 right, glm::vec3 worldup, 
			   glm::vec3 orientation, float movespeed, float sensitivity, float c_sensitivity,float fov, 
			   Window* window, float near, float far) : 
	           Position(pos),Forward(forward),Up(up),Right(right),WorldUp(worldup),
			   MovementSpeed(movespeed), ControllerSensitivity(c_sensitivity),MouseSensitivity(sensitivity),FOV(fov)
{
	cameraProjection = glm::perspective(FOV, (float)window->GetWidth() / (float)window->GetHeight(), near, far);
	eulerAngle = orientation;
	Quaternion = glm::quat(eulerAngle);
}

Camera::Camera(glm::vec3 pos, glm::vec3 forward, glm::vec3 up, glm::vec3 right, glm::vec3 worldup,
			   glm::quat orientation, float movespeed, float sensitivity, float c_sensitivity, float fov,
			   Window* window, float near, float far) :
			   Position(pos), Forward(forward), Up(up), Right(right), WorldUp(worldup),
			   MovementSpeed(movespeed), ControllerSensitivity(c_sensitivity), MouseSensitivity(sensitivity), FOV(fov)
{
	cameraProjection = glm::perspective(FOV, (float)window->GetWidth() / (float)window->GetHeight(), near, far);
	Quaternion = orientation;
	eulerAngle = glm::eulerAngles(Quaternion);
}
Camera::Camera(glm::vec3 pos, glm::vec3 forward, glm::vec3 up, glm::vec3 right, glm::vec3 worldup, 
			   glm::vec3 orientation, float movespeed, float sensitivity, float c_sensitivity, float fov, 
			   float l, float r, float b, float t, float near, float far) : 
			   Position(pos), Forward(forward), Up(up), Right(right), WorldUp(worldup), 
			   MovementSpeed(movespeed), ControllerSensitivity(c_sensitivity), MouseSensitivity(sensitivity), FOV(fov)
{
	cameraProjection = glm::ortho(l, r, b, t, near, far);
	eulerAngle = orientation;
	Quaternion = glm::quat(eulerAngle);
}
Camera::Camera(glm::vec3 pos, glm::vec3 forward, glm::vec3 up, glm::vec3 right, glm::vec3 worldup,
			   glm::quat orientation, float movespeed, float sensitivity, float c_sensitivity, float fov,
			   float l, float r, float b, float t, float near, float far) :
			   Position(pos), Forward(forward), Up(up), Right(right), WorldUp(worldup),
			   MovementSpeed(movespeed), ControllerSensitivity(c_sensitivity), MouseSensitivity(sensitivity), FOV(fov)
{
	cameraProjection = glm::ortho(l, r, b, t, near, far);
	Quaternion = orientation;
	eulerAngle = glm::eulerAngles(Quaternion);
}
bool Camera::Init()
{
	return true;
}
void Camera::Update()
{
	glm::vec3 forward;

	forward.x = cos(glm::radians(eulerAngle.y)) * cos(glm::radians(eulerAngle.x));
	forward.y = sin(glm::radians(eulerAngle.x));
	forward.z = sin(glm::radians(eulerAngle.y)) * cos(glm::radians(eulerAngle.x));

	Forward = glm::normalize(forward);
	Right = glm::normalize(glm::cross(Forward, WorldUp));
	Up = glm::normalize(glm::cross(Right, Forward));
}

void Camera::Render()
{
}

bool Camera::Shutdown()
{
	return true;
}

glm::mat4 Camera::GetPerspective(Window & w, float near, float far)
{
	return glm::perspective(GetFov(), (float)w.GetWidth() / (float)w.GetHeight(), near, far);
}

glm::mat4 Camera::GetOrtho(float left, float right, float bottom, float top)
{
	return glm::ortho(left, right, bottom, top);
}

glm::mat4 Camera::GetOrtho(float left, float right, float bottom, float top, float near, float far)
{
	return glm::ortho(left, right, bottom, top, near, far);
}

void Camera::Keyboard(float forward, float right, float deltaTime)
{
	float velocity = MovementSpeed * deltaTime;

	Position += (Forward * forward * velocity) + (Right * right * velocity);
}

void Camera::Controller(float xvalue, float zvalue, float xoffset, float yoffset, float deltaTime, bool constrainPitch)
{
	float velocity = MovementSpeed * deltaTime;

	if (xvalue > 0) {
		Position += Right * velocity;
	}
	else if (xvalue < 0) {
		Position -= Right * velocity;
	}

	if (zvalue < 0) {
		Position += Forward * velocity;
	}
	else if (zvalue > 0) {
		Position -= Forward * velocity;
	}

	xoffset *= ControllerSensitivity;
	yoffset *= ControllerSensitivity;

	eulerAngle.x += yoffset * (isPitchInvert ? -1 : 1);
	eulerAngle.y += xoffset * (isYawInvert ? -1 : 1);

	if (constrainPitch) {
		if (eulerAngle.x > 89.0f) {
			eulerAngle.x = 89.0f;
		}
		if (eulerAngle.x < -89.0f) {
			eulerAngle.x = -89.0f;
		}
	}
}


void Camera::MouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
	xoffset *= MouseSensitivity;
	yoffset *= MouseSensitivity;

	eulerAngle.x += yoffset * (isPitchInvert ? -1 : 1);
	eulerAngle.y += xoffset * (isYawInvert ? -1 : 1);

	if (constrainPitch) {
		if (eulerAngle.x > 89.0f) {
			eulerAngle.x = 89.0f;
		}
		if (eulerAngle.x < -89.0f) {
			eulerAngle.x = -89.0f;
		}
	}

	//Update();
}

void Camera::MouseScroll(float offset)
{
	//Adjust FOV
	if (FOV >= 1.0f && FOV <= 45.0f)
		FOV -= offset;
	if (FOV <= 1.0f)
		FOV = 1.0f;
	if (FOV >= 45.0f)
		FOV = 45.0f;

	//Update();
}

Camera::~Camera()
{
}
