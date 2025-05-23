#include "frpch.h"
#include "Camera.h"
#include "Engine/Input/Input.h"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 pos, glm::vec3 rot, float fov, float walkSpeed, float sprintSpeed, glm::vec2 mouseSensitivity)
{
	m_Position = pos;
	m_Rotation = rot;
	m_WalkSpeed = walkSpeed;
	m_SprintSpeed = sprintSpeed;
	m_MouseSensitivity = mouseSensitivity;

	//Calculate the transformation matrix
	CalculateTransform();

	//calculate the front, right, and up vectors of the resulting transform
	CalculateCameraVectors();

	//Calculate the view matrix with the Camera's transformation matrix
	CalculateViewMatrix();

	m_Fov = fov;
}

const glm::mat4& Camera::GetViewMatrix()
{
	return m_ViewMatrix;
}

const glm::mat4& Camera::GetProjMatrix(float aspectRatio)
{
	m_AspectRatio = aspectRatio;
	m_ProjMatrix = glm::perspective(glm::radians(m_Fov), aspectRatio, m_NearClip, m_FarClip);
	return m_ProjMatrix;
}


static glm::vec2 lastMousePos;
static glm::vec3 lastVel;
static float velLerp = 0.5f;

void Camera::Update(const Time& time)
{
	//Look
	glm::vec2 mousePos = InputManager::GetMousePosition();
	glm::vec2 mouseMovement = mousePos - lastMousePos;
	lastMousePos = mousePos;

	mouseMovement *= m_MouseSensitivity;

	float xRot = m_Rotation.x;
	xRot -= mouseMovement.y;
	xRot = glm::clamp(xRot, -89.5f, 89.5f);
	float yRot = m_Rotation.y;
	yRot -= mouseMovement.x;

	m_Rotation = glm::vec3(xRot, yRot, 0.0f);

	CalculateTransform();
	CalculateCameraVectors();
	CalculateViewMatrix();


	//Movement
	glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f);

	//Forward Backward
	if (InputManager::IsKeyDown(KEYCODE_W))
		velocity += m_Front; // Forward is negative, following opengl's coordinate
	else if (InputManager::IsKeyDown(KEYCODE_S))
		velocity -= m_Front; //Backward is positive, following opengl's coordinate 


	//Left Right
	if (InputManager::IsKeyDown(KEYCODE_D))
		velocity += m_Right;
	else if (InputManager::IsKeyDown(KEYCODE_A))
		velocity -= m_Right;


	//Up Down
	if (InputManager::IsKeyDown(KEYCODE_E))
		velocity += glm::vec3(0.0f, 1.0f, 0.0f); // Move Up
	else if (InputManager::IsKeyDown(KEYCODE_Q))
		velocity -= glm::vec3(0.0f, 1.0f, 0.0f); // Move Down


	//Check if we're sprinting
	float movementSpeed = m_WalkSpeed;

	if (InputManager::IsKeyDown(KEYCODE_LEFT_SHIFT))
		movementSpeed = m_SprintSpeed;

	//Move if velocity's magnitude is greater than 0
	if (glm::length(velocity) > 0.0)
	{
		//Normalize velocity so that we wont move faster if we're moving diagonals
		velocity = glm::normalize(velocity);
		velocity *= movementSpeed * time.deltaTime;

		//Move our Camera
		m_Position += glm::mix(lastVel, velocity, velLerp * time.deltaTime);
		CalculateTransform();
		CalculateViewMatrix();
		
	}

	lastVel = velocity;
}

glm::vec3 Camera::GetPosition() const
{
	return m_Position;
}

glm::vec3 Camera::GetDirection() const
{
	return m_Front;
}


void Camera::CalculateTransform()
{
	glm::mat4 temp = glm::mat4(1.0f); //Identity Matrix;
	temp = glm::translate(temp, m_Position);

	temp = glm::rotate(temp, glm::radians(m_Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	temp = glm::rotate(temp, glm::radians(m_Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	temp = glm::rotate(temp, glm::radians(m_Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));

	m_Transform = temp;
}


void Camera::CalculateViewMatrix()
{
	m_ViewMatrix = glm::inverse(m_Transform);
}


void Camera::CalculateCameraVectors()
{
	m_Front = glm::vec3(m_Transform * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f));
	m_Front = glm::normalize(m_Front);

	// Calculate camera's right vector and up vector
	m_Right = glm::cross(m_Front, glm::vec3(0.0f, 1.0f, 0.0f));
	m_Up = glm::cross(m_Right, m_Front);
}
