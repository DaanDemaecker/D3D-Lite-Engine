#ifndef TexturedMaterialIncluded
#define TexturedMaterialIncluded

#include "Material.h"

#include <initializer_list>
#include <iostream>

namespace D3D
{
	class TexturedMaterial final : public Material
	{
	public:
		TexturedMaterial(std::initializer_list<const std::string> filePaths, const std::string& pipelineName = "Default");
		~TexturedMaterial();

		PipelinePair& GetPipeline() { return m_PipelinePair; }

		virtual void CreateDescriptorSets(ModelComponent* pModel, std::vector<VkDescriptorSet>& descriptorSets) override;

		virtual void UpdateDescriptorSets(std::vector<VkBuffer>& uboBuffers, std::vector<VkDescriptorSet>& descriptorSets) override;

		virtual VkDescriptorSetLayout* GetDescriptorLayout() override;

		virtual DescriptorPoolWrapper* GetDescriptorPool() override;

	private:
		int m_TextureAmount{};

		std::vector<VkImage> m_TextureImages{};
		std::vector<VkDeviceMemory> m_TextureImageMemories{};

		std::vector<VkImageView> m_TextureImageViews{};
		VkSampler m_TextureSampler{};

		uint32_t m_MipLevels{};

		//Initialization functions
		void CreateTextureImage(const std::string& filePath, int index);
		void CreateTextureImageView(int index);
		void CreateTextureSampler();
	};
}
#endif // !TexturedMaterialIncluded