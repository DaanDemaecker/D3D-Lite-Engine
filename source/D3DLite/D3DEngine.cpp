#include "stdafx.h"
#include "D3DEngine.h"
#include "VulkanRenderer.h"
#include "TimeManager.h"
#include "Model.h"
#include "Material.h"
#include <chrono>

D3D::Window g_pWindow{};

D3D::D3DEngine::D3DEngine(int width, int height)
{
	g_pWindow.Width = width;
	g_pWindow.Height = height;
	InitWindow();
}

D3D::D3DEngine::~D3DEngine()
{
	glfwDestroyWindow(g_pWindow.pWindow);
	glfwTerminate();
}

void D3D::D3DEngine::Run(const std::function<void()>& load)
{
	load();

	auto& renderer{ VulkanRenderer::GetInstance() };

	std::shared_ptr<D3D::Material> pVikingMaterial{ std::make_shared<D3D::Material>("../resources/images/viking_room.png") };
	std::shared_ptr<D3D::Material> pVehicleMaterial{std::make_shared<D3D::Material>("../resources/images/vehicle_diffuse.png")};

	std::vector<std::unique_ptr<Model>> pModels{};
	pModels.push_back(std::make_unique<Model>());
	pModels[0]->LoadModel("../Resources/Models/viking_room.obj");
	pModels[0]->SetMaterial(pVikingMaterial);
	pModels[0]->SetPosition(1.f, -0.2f, 5.f);
	pModels[0]->SetRotation(glm::radians(-90.0f), glm::radians(45.0f), 0.f);
	pModels[0]->SetScale(0.75f, 0.75f, 0.75f);

	pModels.push_back(std::make_unique<Model>());
	pModels[1]->LoadModel("../Resources/Models/vehicle.obj");
	pModels[1]->SetMaterial(pVehicleMaterial);
	pModels[1]->SetPosition(-1.f, 0, 5.f);
	pModels[1]->SetRotation(0.f, glm::radians(75.0f), 0.f);
	pModels[1]->SetScale(0.05f, 0.05f, 0.05f);

	auto& time{ TimeManager::GetInstance() };


	bool doContinue = true;
	auto lastTime = std::chrono::high_resolution_clock::now();
	float lag = 0.0f;
	constexpr float fixedTimeStep = 0.02f;

	time.SetFixedTime(fixedTimeStep);

	constexpr float desiredFrameRate = 144;
	constexpr float desiredFrameDuration = 1000.f / desiredFrameRate;

	while (doContinue)
	{
		const auto currentTime = std::chrono::high_resolution_clock::now();
		const std::chrono::duration<float> deltaTime = currentTime - lastTime;
		const float deltaTimeSeconds = deltaTime.count();

		time.SetDeltaTime(deltaTimeSeconds);

		//std::cout << time.GetFps() << std::endl;

		lastTime = currentTime;
		lag += deltaTimeSeconds;

		doContinue = !glfwWindowShouldClose(g_pWindow.pWindow);

		glfwPollEvents();

		while (lag >= fixedTimeStep)
		{
			// Perform fixed time-step updates here

			lag -= fixedTimeStep;
		}

		renderer.Render(pModels);

		vkDeviceWaitIdle(renderer.GetDevice());

		const auto frameDuration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - currentTime);
		const auto sleepTime = std::chrono::milliseconds(static_cast<int>(desiredFrameDuration)) - frameDuration;

		if (sleepTime > std::chrono::milliseconds(0))
		{
			std::this_thread::sleep_for(sleepTime);
		}
	}
}

void D3D::D3DEngine::InitWindow()
{
	//Initialize glfw
	glfwInit();

	//Tell GLFW not to create an OpenGL context
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	//Initialize the window
	g_pWindow.pWindow = glfwCreateWindow(g_pWindow.Width, g_pWindow.Height, "D3D Lite Engine", nullptr, nullptr);
	glfwSetWindowUserPointer(g_pWindow.pWindow, this);
	glfwSetFramebufferSizeCallback(g_pWindow.pWindow, FramebufferResizeCallback);
}

void D3D::D3DEngine::FramebufferResizeCallback(GLFWwindow* /*pWindow*/, int /*width*/, int /*height*/)
{
	g_pWindow.FrameBufferResized = true;
}