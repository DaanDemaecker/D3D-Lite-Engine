// DescriptorObject.h
// This class will handle the buffers, memory and descriptor set updates of Uniform Buffer Objects

#ifndef DescriptorObjectIncluded
#define DescriptorObjectIncluded

// File includes
#include "DescriptorObject.h"
#include "../../Vulkan/VulkanRenderer.h"

namespace DDM3
{
	// Templated class so that the user can choose what the descriptor holds
	template <typename T>
	class UboDescriptorObject final : public DescriptorObject
	{
	public:
		UboDescriptorObject();

		virtual ~UboDescriptorObject();

		// Add the descriptor write objects to the list of descriptorWrites
		// Parameters:
		//     descriptorSet: the current descriptorset connected to this descriptor object
		//     descriptorWrites: the list of descriptorWrites this function will add to
		//     binding: the current binding in the shader files
		//     amount: the amount of descriptors for the current binding
		//     index: the current frame index of the renderer
		virtual void AddDescriptorWrite(VkDescriptorSet descriptorSet, std::vector<VkWriteDescriptorSet>& descriptorWrites, int& binding, int amount, int index) override;

		// Update the buffer of the object
		// Parameters:
		//     uboObject: a reference of the object in question
		//     frame: the index of the current frame
		void UpdateUboBuffer(T& uboObject, uint32_t frame);

	private:
		// Vector of buffers for UBOs
		std::vector<VkBuffer> m_UboBuffers{};
		// Vector for memories for UBOs
		std::vector<VkDeviceMemory> m_UbosMemory{};
		// Pointers to mapped UBOs
		std::vector<void*> m_UbosMapped{};

		// BufferInfos
		std::vector<VkDescriptorBufferInfo> m_BufferInfos{};

		// Set up the buffers
		void SetupBuffers();

		// Set up the buffer infos
		void SetupBufferInfos();

		// Clean up
		void Cleanup();
	};


	template<typename T>
	inline UboDescriptorObject<T>::UboDescriptorObject()
		:DescriptorObject(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER)
	{
		// Set up the buffers and buffer infos
		SetupBuffers();
		SetupBufferInfos();
	}

	template<typename T>
	inline UboDescriptorObject<T>::~UboDescriptorObject()
	{
		// Clean up
		Cleanup();
	}

	template<typename T>
	inline void UboDescriptorObject<T>::AddDescriptorWrite(VkDescriptorSet descriptorSet, std::vector<VkWriteDescriptorSet>& descriptorWrites, int& binding, int amount, int index)
	{
		VkWriteDescriptorSet descriptorWrite{};

		// DescriptorWrites
		// Set the type to WriteDescriptorSet
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		// Set the binding used in the shader
		descriptorWrite.dstBinding = binding;
		// Set array index
		descriptorWrite.dstArrayElement = 0;
		// Set descriptor type
		descriptorWrite.descriptorType = m_Type;
		// Set descriptor amount
		descriptorWrite.descriptorCount = 1;
		// Set the correct bufferInfo
		descriptorWrite.pBufferInfo = &m_BufferInfos[index];
		// Give the correct descriptorset
		descriptorWrite.dstSet = descriptorSet;

		descriptorWrites.push_back(descriptorWrite);

		binding++;
	}

	template<typename T>
	inline void UboDescriptorObject<T>::UpdateUboBuffer(T& uboObject, uint32_t frame)
	{
		// Copy the memory of the object to the buffer
		memcpy(m_UbosMapped[frame], &uboObject, sizeof(T));
	}

	template<typename T>
	inline void UboDescriptorObject<T>::SetupBuffers()
	{
		// Get reference to renderer
		auto& renderer = VulkanRenderer::GetInstance();
		// Get amount of frames
		auto frames = renderer.GetMaxFrames();

		// Get size of Uniform Buffer Object
		VkDeviceSize bufferSize = sizeof(T);

		// Resize ubobuffers to amount of frames
		m_UboBuffers.resize(frames);
		// Resize ubo memory to amount of frames
		m_UbosMemory.resize(frames);
		// Resize mapped memory to amount of frames
		m_UbosMapped.resize(frames);

		// Loop for the amount of frames there are
		for (size_t i = 0; i < frames; ++i)
		{
			// Create memory
			renderer.CreateBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
				m_UboBuffers[i], m_UbosMemory[i]);

			// Map memory from uboMemory to ubosmapped
			vkMapMemory(renderer.GetDevice(), m_UbosMemory[i], 0, bufferSize, 0, &m_UbosMapped[i]);
		}
	}

	template<typename T>
	inline void UboDescriptorObject<T>::SetupBufferInfos()
	{
		// Resize the buffer infos
		m_BufferInfos.resize(m_UboBuffers.size());

		for (size_t i{}; i < m_BufferInfos.size(); i++)
		{
			// BufferInfos
		// Set the correct buffer
			m_BufferInfos[i].buffer = m_UboBuffers[i];
			// Offset should be 0
			m_BufferInfos[i].offset = 0;
			// Give the correct size of the buffer object
			m_BufferInfos[i].range = sizeof(T);
		}
	}
	template<typename T>
	inline void UboDescriptorObject<T>::Cleanup()
	{
		// Get reference to renderer
		auto& renderer = DDM3::VulkanRenderer::GetInstance();
		// Get reference to device
		auto device = renderer.GetDevice();

		// Wait until device is idle
		vkDeviceWaitIdle(device);

		// Loop for the amount of frames
		for (size_t i = 0; i < renderer.GetMaxFrames(); ++i)
		{
			// Destroy uboBuffers
			vkDestroyBuffer(device, m_UboBuffers[i], nullptr);
			// Free ubo buffer memory
			vkFreeMemory(device, m_UbosMemory[i], nullptr);
		}
	}
}

#endif // !DescriptorObjectIncluded