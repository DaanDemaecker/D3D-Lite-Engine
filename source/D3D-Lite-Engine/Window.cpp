// Window.cpp

// File includes
#include "Window.h"
#include "ConfigManager.h"

// Static library includes
#include <functional>

D3D::Window::Window()
{
	// Set the width of the window
	m_Window.Width = ConfigManager::GetInstance().GetInt("WindowWidth");
	// Set the height of the window
	m_Window.Height = ConfigManager::GetInstance().GetInt("WindowHeight");

	// Initialize the window
	InitWindow();
}

D3D::Window::~Window()
{
	// Destroy the window
	glfwDestroyWindow(m_Window.pWindow);

	// Terminate glfw
	glfwTerminate();
}

void D3D::Window::SetFrameBufferResized(bool value)
{
	// Set the value of frameBufferResized to the requested value
	m_Window.FrameBufferResized = value;
}

void D3D::Window::InitWindow()
{
	// Initialize glfw
	glfwInit();

	//Tell GLFW not to create an OpenGL context
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	// Get a reference to the configmanager for later use
	auto& configManager{ ConfigManager::GetInstance() };

	// Get info ffrom config manager
	bool fullScreen{ configManager.GetBool("FullScreen") };
	int monitorIndex{ configManager.GetInt("Monitor") };

	// Get a list of all the available monitors
	int count{};
	GLFWmonitor** monitors{ glfwGetMonitors(&count) };

	// Create monitor variable and initialize with nullptr
	// If fullscreen is enabled, this will hold the correct monitor
	// If fullscreen is not enabled, this will stay nullptr
	GLFWmonitor* monitor{ nullptr };

	if (fullScreen)
	{
		// If the monitor index is available, set monitor, if not, get primary monitor
		if (monitorIndex < count)
		{
			monitor = monitors[monitorIndex];
		}
		else
		{
			monitor = glfwGetPrimaryMonitor();
		}
	}

	//Initialize the window
	m_Window.pWindow = glfwCreateWindow(m_Window.Width, m_Window.Height, configManager.GetString("WindowName").c_str(),
		monitor, nullptr);


	// If fullscreen is disabled, the monitor index is available and isn't 0, calculate position for the window on the requested monitor
	if (!fullScreen && monitorIndex != 0 && monitorIndex < count)
	{
		int x, y, w, h;

		// Get the current monitor
		auto currentMonitor{ monitors[monitorIndex] };

		// Get all the measurements of the monitor
		glfwGetMonitorWorkarea(currentMonitor, &x, &y, &w, &h);

		// Calculate new position for the screen
		int newX{ x + (w / 2) - (m_Window.Width / 2) };
		int newY{ y + (h / 2) - (m_Window.Width / 2) };

		// Get refreshrate
		auto refreshRate{ glfwGetVideoMode(currentMonitor)->refreshRate };

		// Set the new window position
		glfwSetWindowMonitor(m_Window.pWindow, nullptr, newX, newY, m_Window.Width, m_Window.Height, refreshRate);
	}

	// Set window user pointer and callbacks
	glfwSetWindowUserPointer(m_Window.pWindow, this);
	glfwSetFramebufferSizeCallback(m_Window.pWindow, FramebufferResizeCallback);
}

void D3D::Window::FramebufferResizeCallback(GLFWwindow* pWindow, int width, int height)
{
	// Get a reference to the window struct
	auto& window = GetInstance().GetWindowStruct();

	// Update values of global window variable after resizing
	window.FrameBufferResized = true;
	window.pWindow = pWindow;
	window.Width = width;
	window.Height = height;
}