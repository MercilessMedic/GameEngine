
#include "Camera.h"

Camera::Camera( glm::vec3 position, glm::vec3 up, float yaw, float pitch):
	Front( glm::vec3( 0.0f, 0.0f, -1.0f)),
	MovementSpeed( SPEED ),
	MouseSensitivity( SENSITIVITY ),
	Zoom( ZOOM )
{
	Position = position;
	WorldUp = up;
	Yaw = yaw;
	Pitch = pitch;
	updateCameraVectors();
}

void Camera::processKeyboard( CameraMovement movement, float dt)
{
	float velocity = MovementSpeed * dt;

	switch( movement )
	{
	case FORWARD:
		Position += Front * velocity;
		break;
	case BACKWARD:
		Position -= Front * velocity;
		break;
	case LEFT:
		Position -= Right * velocity;
		break;
	case RIGHT:
		Position += Right * velocity;
		break;
	}
}

void Camera::processMouseMovement(float xoffset, float yoffset, bool constrainPitch )
{
	xoffset *= MouseSensitivity;
	yoffset *= MouseSensitivity;

	Yaw += xoffset;
	Pitch += yoffset;

	if( constrainPitch )
	{
		if (Pitch > 89.0f) { Pitch = 89.0f; }
		if (Pitch < -89.0f) { Pitch = -89.0f; }
	}
	updateCameraVectors();
}

void Camera::updateCameraVectors()
{
	glm::vec3 newFront;
	newFront.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	newFront.y = sin(glm::radians(Pitch));
	newFront.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));

	Front = glm::normalize(newFront);
	Right = glm::normalize(glm::cross(Front, WorldUp));
	Up = glm::normalize(glm::cross(Right, Front));
}