#ifndef COMMAND_BUFFER_HPP_INCLUDED
#define COMMAND_BUFFER_HPP_INCLUDED
#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <vector>

#include "vulkan/vulkan.h"
#include "vulkan/vk_layer.h"
#include "vulkan/vk_layer_dispatch_table.h"
namespace vkBasalt
{
    void allocateCommandBuffer(const VkDevice& device, const VkLayerDispatchTable& dispatchTable, const VkCommandPool& commandPool, const uint32_t& count, VkCommandBuffer* commandBuffers);
    void writeCASCommandBuffers(const VkDevice& device, const VkLayerDispatchTable& dispatchTable, const VkPipeline& pipeline, const VkPipelineLayout& layout, const VkExtent2D& extent, const uint32_t& count,const VkImage* images,const VkDescriptorSet& uniformBufferDescriptorSet, const VkDescriptorSet* descriptorSets, VkCommandBuffer* commandBuffers, const VkImage imageDst);
    void createSemaphores(const VkDevice& device, const VkLayerDispatchTable& dispatchTable, uint32_t count, VkSemaphore* semaphores);
}

#endif // COMMAND_BUFFER_HPP_INCLUDED
