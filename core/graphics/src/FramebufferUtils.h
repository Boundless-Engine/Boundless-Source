#pragma once

#include "pch.h"

#include "VulkanAPI.h"

namespace Boundless {
	namespace Graphics {
		namespace Framebuffer {

			static BReturn GetPixelInfo(I::IGraphicsAPI* api, int32_t x, int32_t y, uint32_t size, VulkanTexture2D texture, void* pixel) {
				auto vk = reinterpret_cast<VulkanAPI*>(api);
				VulkanBuffer buffer;

				VkDeviceSize bufferSize = (VkDeviceSize)size;

				vk->AllocateBuffer<BufferType::TransferDst>(bufferSize, buffer);

				VkBufferImageCopy region{};
				region.bufferOffset = 0;
				region.bufferRowLength = 0;
				region.bufferImageHeight = 0;
				region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				region.imageSubresource.mipLevel = 0;
				region.imageSubresource.baseArrayLayer = 0;
				region.imageSubresource.layerCount = 1;
				region.imageOffset = { x, y, 0 };
				region.imageExtent = {1, 1, 1};
			
				vk->CopyImageToBuffer(texture, region, buffer);
			
				vk->ReadFromBuffer(buffer, bufferSize, pixel);

				vk->FreeBuffer(buffer);
				return SUCCESS;
			}
		}
	}
}
