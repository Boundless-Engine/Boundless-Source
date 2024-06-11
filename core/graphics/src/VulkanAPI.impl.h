#pragma once

#include <shaderc/shaderc.hpp>

namespace Boundless {
	namespace Graphics {

		template<BufferType T>
		VkFlags DetermineBufferUsage() {
			VkFlags flags = 0;
				
			if (T == BufferType::Vertex)
				// vertex buffers must contain: VK_BUFFER_USAGE_VERTEX_BUFFER_BIT 
				// vertex buffers also must have the abillity to be copied to.
				flags |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
			else if (T == BufferType::Index)
				// index buffers must contain: VK_BUFFER_USAGE_INDEX_BUFFER_BIT 
				// index buffers also must have the abillity to be copied to.
				flags |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
			else if (T == BufferType::Staging)
				// staging buffers must have the abillity to be able to be copied to and from
				flags |= VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
			else if (T == BufferType::Uniform)
				// uniform buffers must have the abillity to be able to be copied to and from
				flags |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
			else if (T == BufferType::Constant)
				// constant buffers must have the abillity to be able to be copied to and from
				flags |= VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
			else if (T == BufferType::TransferSrc)
				flags |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
			else if (T == BufferType::TransferDst)
				flags |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
			else 
				throw std::runtime_error("BufferType is Unrecognized");

			return flags;
		}


		template<ShaderStage T>
		const char* GetEntryPointName() {
			if (T == ShaderStage::Vertex)
				return "vert";
			else if (T == ShaderStage::Fragment)
				return "frag";
			else if (T == ShaderStage::Geometry)
				return "geo";
			else if (T == ShaderStage::Tesselation)
				return "tess";
			else
				throw std::runtime_error("ShaderType is Unrecognized");
		};

	
		template<BufferType T>
		BReturn VulkanAPI::AllocateBuffer(VkDeviceSize bufferSize, VulkanBuffer& buffer)
		{
			VkBufferCreateInfo bufferInfo{};
			bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			bufferInfo.size = bufferSize;

			VkFlags usage = DetermineBufferUsage<T>();

			bufferInfo.usage = usage;

			bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

			if (vkCreateBuffer(device, &bufferInfo, allocator, &buffer.buffer) != VK_SUCCESS) {
				return FAILURE;
			}

			VkMemoryRequirements memRequirements;
			vkGetBufferMemoryRequirements(device, buffer.buffer, &memRequirements);

			VkMemoryAllocateInfo allocInfo{};
			allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			allocInfo.allocationSize = memRequirements.size;
			allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

			if (vkAllocateMemory(device, &allocInfo, allocator, &buffer.memory) != VK_SUCCESS) {
				return FAILURE;
			}

			if (vkBindBufferMemory(device, buffer.buffer, buffer.memory, 0) != VK_SUCCESS) {
				return FAILURE;
			}

			buffer.size = bufferSize;

			return SUCCESS;
		}
	}
}