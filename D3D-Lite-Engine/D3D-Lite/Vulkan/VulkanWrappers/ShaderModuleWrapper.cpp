// ShaderModuleWrapper.cpp

// File includes
#include "ShaderModuleWrapper.h"
#include "../../Utils/Utils.h"

// Standard library includes
#include <stdexcept>

D3D::ShaderModuleWrapper::ShaderModuleWrapper(VkDevice device, const std::string& filePath)
{
	// Read the file into the shader code
	m_ShaderCode = Utils::readFile(filePath);

	// Create the shader module
	CreateShaderModule(device);

	// Create reflect shader module
	ReflectShader();

	// Create the shader stage create info
	CreateShaderStageInfo();
}

void D3D::ShaderModuleWrapper::Cleanup(VkDevice device)
{
	// Destroy the reflected shadermodule
	spvReflectDestroyShaderModule(&m_ReflectShaderModule);
	// Delete fragment shader module
	vkDestroyShaderModule(device, m_ShaderModule, nullptr);
}

void D3D::ShaderModuleWrapper::AddDescriptorSetLayoutBindings(std::vector<VkDescriptorSetLayoutBinding>& bindings)
{
	// Read the shader stage from the shader module
	auto stage{ static_cast<VkShaderStageFlagBits>(m_ReflectShaderModule.shader_stage) };

	// Get the amount of bindings
	auto amount{ m_ReflectShaderModule.descriptor_binding_count };

	// Get list of the bindings
	auto descriptorBindings{ m_ReflectShaderModule.descriptor_bindings };

	// Loop trough the amoun of bindings
	for (uint32_t i{}; i < amount; i++)
	{
		auto descriptorCount = descriptorBindings[i].count;
		if (descriptorBindings[i].array.dims_count > 0) {
			// Found a texture array binding
			descriptorCount = descriptorBindings[i].array.dims[0];
			// numTextures now contains the number of textures in the array
		}

		// Create ubolayoutbinding and get the information from the reflect shader module
		VkDescriptorSetLayoutBinding binding{};
		binding.binding = descriptorBindings[i].binding;
		binding.descriptorType = static_cast<VkDescriptorType>(descriptorBindings[i].descriptor_type);
		binding.descriptorCount = descriptorCount;
		binding.stageFlags = stage; 
		binding.pImmutableSamplers = nullptr;

		// Place binding in the vector of bindings
		bindings.push_back(binding);
	}
}

void D3D::ShaderModuleWrapper::AddDescriptorInfo(std::map<VkDescriptorType, int>& typeCount, std::map<int, int>& descriptorsPerBinding)
{
	// Get the amount of descriptor bindings
	auto amount{ m_ReflectShaderModule.descriptor_binding_count };

	// Get list of the descriptor bindings
	auto descriptorBindings{ m_ReflectShaderModule.descriptor_bindings };

	// Loop trough the amount of descriptors
	for (uint32_t i{}; i < amount; i++)
	{
		descriptorsPerBinding[descriptorBindings[i].binding] = descriptorBindings[i].count;
		if (descriptorBindings[i].array.dims_count > 0) {
			// Found a texture array binding
			descriptorsPerBinding[descriptorBindings[i].binding] = descriptorBindings[i].array.dims[0];
			// numTextures now contains the number of textures in the array
		}


		// Get the type of the current binding
		auto currentType{ static_cast<VkDescriptorType>(descriptorBindings[i].descriptor_type) };

		// Add 1 to the value of the current binding type
		if (typeCount.contains(currentType))
		{
			typeCount[currentType] += m_ReflectShaderModule.descriptor_binding_count;
		}
		else
		{
			typeCount[currentType] = m_ReflectShaderModule.descriptor_binding_count;
		}
	}
}

void D3D::ShaderModuleWrapper::AddPushConstantRanges(std::vector<VkPushConstantRange>& pushConstantRanges)
{
	auto index = pushConstantRanges.size();
	auto ranges = m_ReflectShaderModule.push_constant_blocks;
	int pushConstantRangeAmount = m_ReflectShaderModule.push_constant_block_count;

	pushConstantRanges.resize(index + pushConstantRangeAmount);

	for (int i{}; i < pushConstantRangeAmount; i++)
	{
		pushConstantRanges[index].offset = ranges[i].offset;
		pushConstantRanges[index].size = ranges[i].size;
		pushConstantRanges[index].stageFlags = m_ReflectShaderModule.shader_stage;

		index++;
	}
}

void D3D::ShaderModuleWrapper::CreateShaderModule(VkDevice device)
{
	// Create modlue create info
	VkShaderModuleCreateInfo createInfo{};
	// Set type to shader module craete info
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	// Set the code size to the size of the vector
	createInfo.codeSize = m_ShaderCode.size();
	// Give the data of the vector as a pointer
	createInfo.pCode = reinterpret_cast<const uint32_t*>(m_ShaderCode.data());

	// Create shadermodule
	if (vkCreateShaderModule(device, &createInfo, nullptr, &m_ShaderModule) != VK_SUCCESS)
	{
		// Unsuccessful, throw runtime error
		throw std::runtime_error("failed to create shader module!");
	}
}

void D3D::ShaderModuleWrapper::ReflectShader()
{
	// Create the reflect shader module
	SpvReflectResult result = spvReflectCreateShaderModule(m_ShaderCode.size() * sizeof(char), m_ShaderCode.data(), &m_ReflectShaderModule);

	// If unsuccessful, throw runtime error
	if (result != SPV_REFLECT_RESULT_SUCCESS)
	{
		throw std::runtime_error("failed to create shader module");
	}
}

void D3D::ShaderModuleWrapper::CreateShaderStageInfo()
{
	// Set type to pipeline shader stage create info
	m_ShaderstageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	// Set stage to fragment
	m_ShaderstageCreateInfo.stage =
		static_cast<VkShaderStageFlagBits>(m_ReflectShaderModule.shader_stage);
	// Give the correct shader module
	m_ShaderstageCreateInfo.module = m_ShaderModule;
	// Give the name of the function
	m_ShaderstageCreateInfo.pName = m_ReflectShaderModule.entry_point_name;
}