#pragma once
#include "Engine/Time.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
	Camera(glm::vec3 pos, glm::vec3 rot = glm::vec3(0.0f), float fov = 45.0f, float walkSpeed = 2.5f, float sprintSpeed = 4.0f, glm::vec2 mouseSensitivity = glm::vec2(0.1f));

	const glm::mat4& GetViewMatrix();
	const glm::mat4& GetProjMatrix(float aspectRatio);

	void Update(const Time& time);

private:
	void CalculateTransform();
	void CalculateCameraVectors();
	void CalculateViewMatrix();
	
private:
	glm::vec3 m_Position;
	glm::vec3 m_Rotation;

	glm::mat4 m_Transform;
	glm::mat4 m_ProjMatrix;
	glm::mat4 m_ViewMatrix;

	glm::vec3 m_Front;
	glm::vec3 m_Right;
	glm::vec3 m_Up;

	// camera options
	float m_Fov;
	float m_AspectRatio;
	float m_NearClip = 0.01f;
	float m_FarClip = 100.0f;

	float m_WalkSpeed;
	float m_SprintSpeed;
	glm::vec2 m_MouseSensitivity;
};

