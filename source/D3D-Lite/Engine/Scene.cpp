#include "Scene.h"
#include "../BaseClasses/GameObject.h"

#include "../Components/CameraComponent.h"
#include "../Components/DirectionalLightComponent.h"
#include "../Components/TransformComponent.h"

unsigned int D3D::Scene::m_IdCounter = 0;



D3D::Scene::Scene(const std::string& name) : m_Name(name)
{
	m_pSceneRoot = std::make_unique<GameObject>("Scene Root");
	m_pSceneRoot->Init();

	m_pDefaultCamera = std::make_unique<GameObject>("Default Camera");
	m_pDefaultCamera->Init();
	m_pDefaultCameraComponent = m_pDefaultCamera->AddComponent<CameraComponent>();

	m_pDefaultLight = std::make_unique<GameObject>("Default Light");
	m_pDefaultLight->Init();
	m_pDefaultLightComponent = m_pDefaultLight->AddComponent<DirectionalLightComponent>();
}

D3D::GameObject* D3D::Scene::CreateGameObject(const std::string& name)
{
	return m_pSceneRoot->CreateNewObject(name);
}

void D3D::Scene::OnSceneLoad()
{
	m_pSceneRoot->OnSceneLoad();
}

void D3D::Scene::OnSceneUnload()
{
	m_pSceneRoot->OnSceneUnload();
}

void D3D::Scene::StartFrame()
{
	m_pSceneRoot->StartFrame();
}

void D3D::Scene::EarlyUpdate()
{
	m_pSceneRoot->EarlyUpdate();
}

void D3D::Scene::Update()
{
	m_pSceneRoot->Update();
}

void D3D::Scene::FixedUpdate()
{
	m_pSceneRoot->FixedUpdate();
}

void D3D::Scene::LateUpdate()
{
	m_pSceneRoot->LateUpdate();
}

void D3D::Scene::RenderSkyBox()
{
	if (m_pActiveCamera != nullptr)
		m_pActiveCamera->RenderSkybox();
}

void D3D::Scene::Render() const
{
	m_pSceneRoot->Render();
}

void D3D::Scene::OngGUI() const
{
	m_pSceneRoot->OnGUI();
}

void D3D::Scene::Cleanup()
{
	m_pSceneRoot->Cleanup();
}

void D3D::Scene::SetCamera(std::shared_ptr<CameraComponent> pCamera)
{
	m_pActiveCamera = pCamera;
}

const std::shared_ptr<D3D::CameraComponent> D3D::Scene::GetCamera() const
{
	if (m_pActiveCamera != nullptr)
		return m_pActiveCamera;

	return m_pDefaultCameraComponent;
}

void D3D::Scene::SetLight(std::shared_ptr<DirectionalLightComponent> pLight)
{
	m_pActiveLight = pLight;
}

const std::shared_ptr<D3D::DirectionalLightComponent> D3D::Scene::GetLight() const
{
	if (m_pActiveLight != nullptr)
		return m_pActiveLight;

	return m_pDefaultLightComponent;
}
