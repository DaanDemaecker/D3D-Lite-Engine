#include "../../Vulkan/VulkanRenderer.h"
#include "../../Managers/SceneManager.h"

#include "../../DataTypes/Materials/Material.h"
#include "../../DataTypes/Materials/TexturedMaterial.h"
#include "../../DataTypes/Materials/MultiMaterial.h"

#include "../../Components/MeshRenderComponent.h"
#include "../../DataTypes/Mesh.h"
#include "../../Components/TransformComponent.h"
#include "../Scene.h"
#include "../../Components/SkyBoxComponent.h"

#include "../../Components/CameraComponent.h"
#include "../../Components/DirectionalLightComponent.h"

#include "../../Components/RotatorComponent.h"
#include "../../Components/SpectatorMovementComponent.h"
#include "../../Components/AnimatorComponent.h"

#include "../../Vulkan/VulkanRenderer.h"
#include "../../Managers/ResourceManager.h"
#include "../../Managers/ConfigManager.h"

namespace LoadTestScene
{
	void SetupPipelines();

	void SetupVehicle(D3D::Scene* scene);

	void SetupVikingRoom(D3D::Scene* scene);

	void SetupGun(D3D::Scene* scene);

	void SetupMario(D3D::Scene* scene);

	void SetupCamera(D3D::Scene* scen);

	void SetupLight(D3D::Scene* scene);

	void SetupGroundPlane(D3D::Scene* scene);

	void loadTestScene()
	{
		auto scene = D3D::SceneManager::GetInstance().CreateScene("Test");
		D3D::SceneManager::GetInstance().SetActiveScene(scene);

		SetupPipelines();

		SetupGroundPlane(scene.get());

		SetupVehicle(scene.get());

		//SetupVikingRoom(scene.get());

		//SetupGun(scene.get());

		SetupMario(scene.get());

		SetupCamera(scene.get());

		SetupLight(scene.get());
	}

	void SetupPipelines()
	{
		auto& renderer{ D3D::VulkanRenderer::GetInstance() };

		renderer.AddGraphicsPipeline("Diffuse", { "Resources/Shaders/Diffuse.Vert.spv", "Resources/Shaders/Diffuse.Frag.spv" });
		renderer.AddGraphicsPipeline("NormalMap", { "Resources/Shaders/NormalMap.Vert.spv", "Resources/Shaders/NormalMap.Frag.spv" });
		renderer.AddGraphicsPipeline("DiffNorm", { "Resources/Shaders/DiffNorm.Vert.spv", "Resources/Shaders/DiffNorm.Frag.spv" });
		renderer.AddGraphicsPipeline("Test", { "Resources/Shaders/Test.Vert.spv", "Resources/Shaders/Test.Frag.spv" });
		renderer.AddGraphicsPipeline("DiffuseUnshaded", { "Resources/Shaders/DiffuseUnshaded.Vert.spv", "Resources/Shaders/DiffuseUnshaded.Frag.spv" });
		renderer.AddGraphicsPipeline("Specular", { "Resources/Shaders/Specular.Vert.spv", "Resources/Shaders/Specular.Frag.spv" });
		renderer.AddGraphicsPipeline("DiffNormSpec", { "Resources/Shaders/DiffNormSpec.Vert.spv", "Resources/Shaders/DiffNormSpec.Frag.spv" });
		renderer.AddGraphicsPipeline("MultiShader", { "Resources/Shaders/MultiShader.Vert.spv", "Resources/Shaders/MultiShader.Frag.spv" });
		renderer.AddGraphicsPipeline("Skinned", { "Resources/Shaders/Skinned.Vert.spv", "Resources/Shaders/MultiShader.Frag.spv" });
	}

	void SetupVehicle(D3D::Scene* scene)
	{
		std::shared_ptr<D3D::MultiMaterial> pVehicleMaterial{ std::make_shared<D3D::MultiMaterial>("MultiShader") };

		pVehicleMaterial->AddDiffuseTextures(std::initializer_list<const std::string>{"resources/images/vehicle_diffuse.png"});

		pVehicleMaterial->AddNormalMap(std::initializer_list<const std::string>{"resources/images/vehicle_normal.png"});

		pVehicleMaterial->AddGlossTextures(std::initializer_list<const std::string>{"resources/images/vehicle_gloss.png"});

		pVehicleMaterial->AddSpecularTextures(std::initializer_list<const std::string>{"resources/images/vehicle_specular.png"});

		auto pVehicle{ scene->CreateGameObject("Vehicle") };
		pVehicle->SetShowImGui(true);
		//pVehicle->AddComponent<D3D::RotatorComponent>();

		auto pVehicleMesh{ D3D::ResourceManager::GetInstance().LoadMesh("Resources/Models/vehicle.obj") };

		auto pVehicleModel{ pVehicle->AddComponent<D3D::MeshRenderComponent>() };
		pVehicleModel->SetMesh(pVehicleMesh);
		pVehicleModel->SetMaterial(pVehicleMaterial);


		auto pVehicleTransform{ pVehicle->GetTransform() };
		pVehicleTransform->SetShowImGui(true);
		pVehicleTransform->SetLocalPosition(0, 3, 0);
		pVehicleTransform->SetLocalRotation(0.f, glm::radians(75.0f), 0.f);
		pVehicleTransform->SetLocalScale(0.05f, 0.05f, 0.05f);
	}

	void SetupVikingRoom(D3D::Scene* scene)
	{
		std::shared_ptr<D3D::TexturedMaterial> pVikingMaterial{ std::make_shared<D3D::TexturedMaterial>(std::initializer_list<const std::string>{"resources/images/viking_room.png"}, "Diffuse") };

		auto pvikingRoom{ scene->CreateGameObject("Viking Room") };

		auto pVikingRoomMesh{ D3D::ResourceManager::GetInstance().LoadMesh("Resources/Models/viking_room.obj") };

		auto pVikingRoomModel{ pvikingRoom->AddComponent<D3D::MeshRenderComponent>() };
		pVikingRoomModel->SetMesh(pVikingRoomMesh);
		pVikingRoomModel->SetMaterial(pVikingMaterial);

		auto pVikingTransform{ pvikingRoom->GetTransform() };
		pVikingTransform->SetLocalPosition(1.f, -0.2f, 3.f);
		pVikingTransform->SetLocalRotation(glm::radians(-90.0f), glm::radians(45.0f), 0.f);
		pVikingTransform->SetLocalScale(0.75f, 0.75f, 0.75f);
	}

	void SetupGun(D3D::Scene* scene)
	{
		std::shared_ptr<D3D::MultiMaterial> pGunMaterial{ std::make_shared<D3D::MultiMaterial>("MultiShader") };

		pGunMaterial->AddDiffuseTextures(std::initializer_list<const std::string>{"resources/images/gun_BaseColor.png"});

		pGunMaterial->AddNormalMap(std::initializer_list<const std::string>{"resources/images/gun_Normal.png"});

		auto pGun{ scene->CreateGameObject("Gun") };

		auto pGunMesh{ D3D::ResourceManager::GetInstance().LoadMesh("Resources/Models/gun.fbx") };

		auto pGunModel{ pGun->AddComponent<D3D::MeshRenderComponent>() };
		pGunModel->SetMesh(pGunMesh);
		pGunModel->SetMaterial(pGunMaterial);

		auto pGunTransform{ pGun->GetTransform() };
		pGunTransform->SetLocalPosition(0.f, -.5f, 6.f);
		pGunTransform->SetLocalRotation(0.f, glm::radians(-90.f), 0.f);
		pGunTransform->SetLocalScale(0.5f, 0.5f, 0.5f);
	}

	void SetupMario(D3D::Scene* scene)
	{
		auto pMario{ scene->CreateGameObject("Mario") };

		auto pMarioMesh{ D3D::ResourceManager::GetInstance().LoadMesh("Resources/Models/MarioDancing.fbx") };

		auto pMarioModel{ pMario->AddComponent<D3D::MeshRenderComponent>() };
		pMarioModel->SetMesh(pMarioMesh);

		auto pMarioAnimationComponent{ pMario->AddComponent<D3D::AnimatorComponent>() };
		pMarioAnimationComponent->AddAnimations("Resources/Models/MarioDancing.fbx");

		auto pMarioTransform{ pMario->GetTransform() };
		pMarioTransform->SetLocalPosition(0.f, 0.f, 2);
		pMarioTransform->SetLocalRotation(0.f, glm::radians(180.f), 0.f);
		//pMarioTransform->SetLocalScale(0.5f, 0.5f, 0.5f);
	}

	void SetupCamera(D3D::Scene* scene)
	{
		auto pCamera{ scene->CreateGameObject("Camera") };

		pCamera->AddComponent<D3D::SpectatorMovementComponent>();

		auto pCameraComponent{ pCamera->AddComponent<D3D::CameraComponent>() };

		auto pCameraTransform{ pCamera->GetTransform() };
		pCameraTransform->SetLocalPosition(0, 5, -5);
		//pCameraTransform->SetLocalRotation(glm::vec3(0.0f, glm::radians(180.f), 0.0f));

		//pCamera->AddComponent<D3D::RotatorComponent>();

		scene->SetCamera(pCameraComponent);

		auto& configManager{ D3D::ConfigManager::GetInstance() };

		// Set the vertex shader name
		const std::string vertShaderName{configManager.GetString("SkyboxVertName")};
		// Set the fragment shader name
		const std::string fragShaderName{ configManager.GetString("SkyboxFragName") };

		// Create the graphics pipeline for the skybox
		D3D::VulkanRenderer::GetInstance().AddGraphicsPipeline(configManager.GetString("SkyboxPipelineName"), {vertShaderName, fragShaderName}, false);

		auto pSkyBox{ pCamera->AddComponent<D3D::SkyBoxComponent>() };

		pSkyBox->LoadSkybox(std::initializer_list<const std::string>{"resources/images/CubeMap/Sky_Right.png",
			"resources/images/CubeMap/Sky_Left.png",
			"resources/images/CubeMap/Sky_Up.png",
			"resources/images/CubeMap/Sky_Down.png",
			"resources/images/CubeMap/Sky_Front.png",
			"resources/images/CubeMap/Sky_Back.png"});
	}
	                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
	void SetupLight(D3D::Scene* scene)
	{
		auto pLight{ scene->CreateGameObject("Light") };
		pLight->SetShowImGui(true);

		auto pLightComponent{ pLight->AddComponent<D3D::DirectionalLightComponent>() };
		pLightComponent->SetShowImGui(true);

		//pLightComponent->SetColor(glm::vec3{ 0, 0, 0 });

		auto pLightTransform{ pLight->GetTransform() };
		pLightTransform->SetShowImGui(true);
		pLightTransform->SetLocalRotation(glm::vec3(glm::radians(45.f), glm::radians(45.f), 0.0f));


		//pLight->AddComponent<D3D::RotatorComponent>();

		scene->SetLight(pLightComponent);
	}

	void SetupGroundPlane(D3D::Scene* scene)
	{
		std::shared_ptr<D3D::MultiMaterial> pFloorMaterial{ std::make_shared<D3D::MultiMaterial>("MultiShader") };


		//std::shared_ptr<D3D::TexturedMaterial> pFloorMaterial{
		//	std::make_shared<D3D::TexturedMaterial>(std::initializer_list<const std::string>{"resources/images/GroundPlane.png"}, "DiffuseUnshaded") };

		pFloorMaterial->AddDiffuseTextures(std::initializer_list<const std::string>{"resources/images/GroundPlane.png"});

		auto pGroundPlane{ scene->CreateGameObject("Ground Plane") };
		pGroundPlane->SetShowImGui(true);

		auto pGroundplaneMesh{ D3D::ResourceManager::GetInstance().LoadMesh("Resources/Models/Plane.obj") };

		auto pGroundPlaneModel{ pGroundPlane->AddComponent<D3D::MeshRenderComponent>() };
		pGroundPlaneModel->SetShowImGui(true);
		pGroundPlaneModel->SetMesh(pGroundplaneMesh);
		pGroundPlaneModel->SetMaterial(pFloorMaterial);
	}
}