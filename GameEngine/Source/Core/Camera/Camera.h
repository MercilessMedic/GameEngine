#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Default values for the camera
const float SPEED		= 3.0f;
const float SENSITIVITY = 0.1f;
const float ZOOM		= 45.0f;
const float YAW			= -90.0f;
const float PITCH		= 0.0f;
const float FOV			= 45.0f;
const float NEARPLANE	= 0.1f;
const float FARPLANE	= 100.0f;

enum CameraMovement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};


class Camera
{
public:
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
		float yaw = YAW, float pitch = PITCH);

	//Process keyboard input and change postition of the camera
	void processKeyboard(CameraMovement movement, float dt);
	//Process mouse movement and update camera vectors
	void processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
	
	glm::mat4 getViewMatrix() const { return glm::lookAt( Position, Position + Front, Up); }
	glm::mat4 getProjectionMatrix ( const int width, const int height ) const 
								    { return glm::perspective( glm::radians(Fov), float(width)/float(height), NearPlane, FarPlane); }
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	float Yaw = YAW;
	float Pitch = PITCH;
	float MovementSpeed = SPEED;
	float MouseSensitivity = SENSITIVITY;
	float Zoom = ZOOM;

	float Fov = FOV;
	float NearPlane = NEARPLANE;
	float FarPlane = FARPLANE;
	

private:
	void updateCameraVectors();
};