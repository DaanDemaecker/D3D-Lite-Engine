// CubeMapMaterial.cpp

// File includes
#include "CubeMapMaterial.h"
#include "../../Vulkan/VulkanRenderer.h"
#include "../../Vulkan/VulkanWrappers/DescriptorPoolWrapper.h"
#include "../../DataTypes/DescriptorObjects/TextureDescriptorObject.h"

// Standard library includes
#include <stdexcept>

D3D::CubeMapMaterial::CubeMapMaterial(const std::initializer_list<const std::string>&& filePaths)
	:CubeMapMaterial(filePaths)
{

}

D3D::CubeMapMaterial::CubeMapMaterial(const std::initializer_list<const std::string>& filePaths)
	:Material("Skybox")
{
	Texture cubeTexture{};

	// Create the cube texture
	VulkanRenderer::GetInstance().CreateCubeTexture(cubeTexture, filePaths);

	// Create the descriptor object and give the cube texture by value
	m_pDescriptorObject = std::make_unique<TextureDescriptorObject>(cubeTexture);
}

void D3D::CubeMapMaterial::CreateDescriptorSets(MeshRenderComponent* pModel, std::vector<VkDescriptorSet>& descriptorSets)
{
	// Get descriptorpool associated with this material
	auto descriptorPool = GetDescriptorPool();
	// Add model to descriptorpool
	descriptorPool->AddModel(pModel);
	// Create descriptor sets
	descriptorPool->CreateDescriptorSets(GetDescriptorLayout(), descriptorSets);
}

void D3D::CubeMapMaterial::UpdateDescriptorSets(std::vector<VkDescriptorSet>& descriptorSets, std::vector<DescriptorObject*>& descriptorObjects)
{
	// Get pointer to the descriptorpool
	auto descriptorPool = GetDescriptorPool();

	// Create a vector for the descriptor objects
	std::vector<DescriptorObject*> descriptorObjectList{};

	// Add all the descriptor objects of the model to the list
	for (auto& descriptorObject : descriptorObjects)
	{
		descriptorObjectList.push_back(descriptorObject);
	}

	// Add the descriptor object holding the texture
	descriptorObjectList.push_back(m_pDescriptorObject.get());

	// Update descriptorsets
	descriptorPool->UpdateDescriptorSets(descriptorSets, descriptorObjectList);
}