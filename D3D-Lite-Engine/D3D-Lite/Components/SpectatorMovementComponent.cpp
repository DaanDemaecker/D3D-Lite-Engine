#include "SpectatorMovementComponent.h"
#include "TransformComponent.h"
#include "../Includes/GLMIncludes.h"
#include "../Managers/TimeManager.h"

#include <iostream>
#include "../Includes/GLFWIncludes.h"
#include "../Includes/ImGuiIncludes.h"
#include "../Engine/Window.h"

D3D::SpectatorMovementComponent::SpectatorMovementComponent()
{
	glfwGetCursorPos(Window::GetInstance().GetWindowStruct().pWindow, &m_PrevXPos, &m_PrevYPos);
}

void D3D::SpectatorMovementComponent::Update()
{
	glm::vec3 direction{};

	auto window = Window::GetInstance().GetWindowStruct().pWindow;

	auto transform = GetTransform();
	auto deltaTime = TimeManager::GetInstance().GetDeltaTime();

	// Determine movement direction based on key presses
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		//direction += transform->GetForward() * m_Speed * deltaTime;
		direction += transform->GetForward();
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		//direction -= transform->GetForward() * m_Speed * deltaTime;
		direction -= transform->GetForward();
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		//direction -= transform->GetRight() * m_Speed * deltaTime;
		direction -= transform->GetRight();
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		//direction += transform->GetRight() * m_Speed * deltaTime;
		direction += transform->GetRight();
	}

	glm::normalize(direction);

	direction *= m_Speed * deltaTime;


	// Translate the object based on the rotated movement direction
	transform->Translate(direction);

	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) &&
		!ImGui::IsAnyItemActive() &&
		!ImGui::IsAnyItemHovered() &&
		!ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow))
	{
		double deltaX = xpos - m_PrevXPos;
		double deltaY = ypos - m_PrevYPos;


		m_TotalPitch += static_cast<float>(deltaY * deltaTime * m_AngularSpeed);
		m_TotalYaw += static_cast<float>(deltaX * deltaTime * m_AngularSpeed);

		// Rotate the camera based on mouse movement
		transform->SetWorldRotation(m_TotalPitch, m_TotalYaw, 0);
	}

	m_PrevXPos = xpos;
	m_PrevYPos = ypos;

	//std::cout << "x: " << direction.x << "y: " << direction.y << "z: " << direction.z << std::endl;
}