#include "D3DEngine.h"
#include "../Vulkan/VulkanRenderer.h"
#include "../Managers/TimeManager.h"
#include "../Managers/SceneManager.h"
#include "../DataTypes/Materials/Material.h"
#include "../Managers/ConfigManager.h"
#include "Window.h"
#include <chrono>
#include <thread>

#include "Scene.h"
#include "ServiceLocator/ServiceLocator.h"
#include "ServiceLocator/ModelLoader/D3DModelLoader.h"

D3D::D3DEngine::D3DEngine()
{
	// Create the window with the given width and height
	D3D::Window::GetInstance();

	ServiceLocator::RegisterModelLoader(std::make_unique<D3DModelLoader>());

	auto& renderer{ D3D::VulkanRenderer::GetInstance() };

	renderer.AddDefaultPipeline();
}

D3D::D3DEngine::~D3DEngine()
{

}

void D3D::D3DEngine::Run(const std::function<void()>& load)
{
	load();

	auto& renderer{ VulkanRenderer::GetInstance() };
	auto& sceneManager{ SceneManager::GetInstance() };
	auto& time{ TimeManager::GetInstance() };
	auto& window{ Window::GetInstance() };


	bool doContinue = true;
	auto lastTime = std::chrono::high_resolution_clock::now();

	float lag = 0.0f;
	constexpr float fixedTimeStep = 0.02f;

	time.SetFixedTime(fixedTimeStep);

	constexpr float desiredFrameRate = 144.f;
	constexpr float desiredFrameDuration = 1000.f / desiredFrameRate;


	while (doContinue)
	{
		const auto currentTime = std::chrono::high_resolution_clock::now();
		const float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();

		time.SetDeltaTime(deltaTime);

		//std::cout << time.GetFps() << std::endl;

		lastTime = currentTime;
		lag += deltaTime;

		doContinue = !glfwWindowShouldClose(window.GetWindowStruct().pWindow);

		glfwPollEvents();

		sceneManager.StartFrame();

		sceneManager.EarlyUpdate();

		while (lag >= fixedTimeStep)
		{
			sceneManager.FixedUpdate();
			lag -= fixedTimeStep;
		}

		sceneManager.Update();

		sceneManager.LateUpdate();

		sceneManager.PostUpdate();

		sceneManager.Cleanup();

		renderer.Render();

		const auto frameDuration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - currentTime);
		const auto sleepTime = std::chrono::milliseconds(static_cast<int>(desiredFrameDuration)) - frameDuration;

		if (sleepTime > std::chrono::milliseconds(0))
		{
			std::this_thread::sleep_for(sleepTime);
		}
	}

	sceneManager.EndProgram();
}