#include "descriptor_set.hpp"

#ifndef ASSERT_VULKAN
#define ASSERT_VULKAN(val)\
        if(val!=VK_SUCCESS)\
        {\
            throw std::runtime_error("ASSERT_VULKAN failed " + std::to_string(val));\
        }
#endif

typedef struct {
    float sharpness;
} CasBufferObject;

namespace vkBasalt
{
    void createStorageImageDescriptorSetLayout(const VkDevice& device, const VkLayerDispatchTable& dispatchTable, VkDescriptorSetLayout& descriptorSetLayout)
    {
        VkDescriptorSetLayoutBinding descriptorSetLayoutBinding[2];
        descriptorSetLayoutBinding[0].binding = 0;
        descriptorSetLayoutBinding[0].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
        descriptorSetLayoutBinding[0].descriptorCount = 1;
        descriptorSetLayoutBinding[0].stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
        descriptorSetLayoutBinding[0].pImmutableSamplers = nullptr;

        descriptorSetLayoutBinding[1].binding = 1;
        descriptorSetLayoutBinding[1].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
        descriptorSetLayoutBinding[1].descriptorCount = 1;
        descriptorSetLayoutBinding[1].stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
        descriptorSetLayoutBinding[1].pImmutableSamplers = nullptr;

        VkDescriptorSetLayoutCreateInfo descriptorSetCreateInfo;
        descriptorSetCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        descriptorSetCreateInfo.pNext = nullptr;
        descriptorSetCreateInfo.flags = 0;
        descriptorSetCreateInfo.bindingCount = 2;
        descriptorSetCreateInfo.pBindings = descriptorSetLayoutBinding;

        
        VkResult result = dispatchTable.CreateDescriptorSetLayout(device,&descriptorSetCreateInfo,nullptr,&descriptorSetLayout);
        ASSERT_VULKAN(result)
        
    }
    void createStorageImageDescriptorPool(const VkDevice& device, const VkLayerDispatchTable& dispatchTable, const uint32_t& setCount, VkDescriptorPool& descriptorPool)
    {
        VkDescriptorPoolSize poolSize;
        poolSize.type = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
        poolSize.descriptorCount = setCount * 2;
        
        std::cout << "set count " << setCount << std::endl;

        VkDescriptorPoolCreateInfo descriptorPoolCreateInfo;
        descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        descriptorPoolCreateInfo.pNext = nullptr;
        descriptorPoolCreateInfo.flags = 0;
        descriptorPoolCreateInfo.maxSets = setCount;
        descriptorPoolCreateInfo.poolSizeCount = 1;
        descriptorPoolCreateInfo.pPoolSizes = &poolSize;

        VkResult result =  dispatchTable.CreateDescriptorPool(device,&descriptorPoolCreateInfo,nullptr,&descriptorPool);
        ASSERT_VULKAN(result);
    }
    void allocateAndWriteStorageDescriptorSets(const VkDevice& device, const VkLayerDispatchTable& dispatchTable, const VkDescriptorPool& descriptorPool, const uint32_t& setCount, const VkDescriptorSetLayout& descriptorSetLayout,const  VkImageView* imageViews, const VkImageView imageDstView, VkDescriptorSet* descriptorSets)
    {
        std::vector<VkDescriptorSetLayout> layouts(setCount,descriptorSetLayout);
        VkDescriptorSetAllocateInfo descriptorSetAllocateInfo;
        descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        descriptorSetAllocateInfo.pNext = nullptr;
        descriptorSetAllocateInfo.descriptorPool = descriptorPool;
        descriptorSetAllocateInfo.descriptorSetCount = setCount;
        descriptorSetAllocateInfo.pSetLayouts = layouts.data();
        
        std::cout << "before allocating descriptor Sets " << 1 << std::endl;
        VkResult result =  dispatchTable.AllocateDescriptorSets(device,&descriptorSetAllocateInfo,descriptorSets);
        ASSERT_VULKAN(result);

        VkDescriptorImageInfo imageInfo;
        imageInfo.sampler = VK_NULL_HANDLE;
        imageInfo.imageView = VK_NULL_HANDLE;
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_GENERAL;
        VkDescriptorImageInfo imageDstInfo;
        imageDstInfo.sampler = VK_NULL_HANDLE;
        imageDstInfo.imageView = imageDstView;
        imageDstInfo.imageLayout = VK_IMAGE_LAYOUT_GENERAL;

        VkWriteDescriptorSet writeDescriptorSet[2];
        writeDescriptorSet[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writeDescriptorSet[0].pNext = nullptr;
        writeDescriptorSet[0].dstSet = VK_NULL_HANDLE;
        writeDescriptorSet[0].dstBinding = 0;
        writeDescriptorSet[0].dstArrayElement = 0;
        writeDescriptorSet[0].descriptorCount = 1;
        writeDescriptorSet[0].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
        writeDescriptorSet[0].pImageInfo = &imageInfo;
        writeDescriptorSet[0].pBufferInfo = nullptr;
        writeDescriptorSet[0].pTexelBufferView = nullptr;

        writeDescriptorSet[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writeDescriptorSet[1].pNext = nullptr;
        writeDescriptorSet[1].dstSet = VK_NULL_HANDLE;
        writeDescriptorSet[1].dstBinding = 1;
        writeDescriptorSet[1].dstArrayElement = 0;
        writeDescriptorSet[1].descriptorCount = 1;
        writeDescriptorSet[1].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
        writeDescriptorSet[1].pImageInfo = &imageDstInfo;
        writeDescriptorSet[1].pBufferInfo = nullptr;
        writeDescriptorSet[1].pTexelBufferView = nullptr;
        
        for(unsigned int i=0;i<setCount;i++)
        {
            imageInfo.imageView = imageViews[i];
            writeDescriptorSet[0].dstSet = descriptorSets[i];
            writeDescriptorSet[1].dstSet = descriptorSets[i];
            std::cout << "before writing descriptor Sets " << std::endl;
            dispatchTable.UpdateDescriptorSets(device,2,writeDescriptorSet,0,nullptr);
            
        }
 
    }
    
    void createUniformBufferDescriptorSetLayout(const VkDevice& device, const VkLayerDispatchTable& dispatchTable, VkDescriptorSetLayout& descriptorSetLayout)
    {
        VkDescriptorSetLayoutBinding descriptorSetLayoutBinding;
        descriptorSetLayoutBinding.binding = 0;
        descriptorSetLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorSetLayoutBinding.descriptorCount = 1;
        descriptorSetLayoutBinding.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
        descriptorSetLayoutBinding.pImmutableSamplers = nullptr;

        VkDescriptorSetLayoutCreateInfo descriptorSetCreateInfo;
        descriptorSetCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        descriptorSetCreateInfo.pNext = nullptr;
        descriptorSetCreateInfo.flags = 0;
        descriptorSetCreateInfo.bindingCount = 1;
        descriptorSetCreateInfo.pBindings = &descriptorSetLayoutBinding;

        
        VkResult result = dispatchTable.CreateDescriptorSetLayout(device,&descriptorSetCreateInfo,nullptr,&descriptorSetLayout);
        ASSERT_VULKAN(result)
    }
    void createUniformBufferDescriptorPool(const VkDevice& device, const VkLayerDispatchTable& dispatchTable, const uint32_t& setCount, VkDescriptorPool& descriptorPool)
    {
        VkDescriptorPoolSize poolSize;
        poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        poolSize.descriptorCount = setCount;
        
        std::cout << "set count " << setCount << std::endl;

        VkDescriptorPoolCreateInfo descriptorPoolCreateInfo;
        descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        descriptorPoolCreateInfo.pNext = nullptr;
        descriptorPoolCreateInfo.flags = 0;
        descriptorPoolCreateInfo.maxSets = setCount;
        descriptorPoolCreateInfo.poolSizeCount = 1;
        descriptorPoolCreateInfo.pPoolSizes = &poolSize;

        VkResult result =  dispatchTable.CreateDescriptorPool(device,&descriptorPoolCreateInfo,nullptr,&descriptorPool);
        ASSERT_VULKAN(result);
    }
    void writeCasBufferDescriptorSet(const VkDevice& device, const VkLayerDispatchTable& dispatchTable, const VkDescriptorPool& descriptorPool, const VkDescriptorSetLayout& descriptorSetLayout, const VkBuffer& buffer, VkDescriptorSet& descriptorSet)
    {
        VkDescriptorSetAllocateInfo descriptorSetAllocateInfo;
        descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        descriptorSetAllocateInfo.pNext = nullptr;
        descriptorSetAllocateInfo.descriptorPool = descriptorPool;
        descriptorSetAllocateInfo.descriptorSetCount = 1;
        descriptorSetAllocateInfo.pSetLayouts = &descriptorSetLayout;
        
        VkResult result =  dispatchTable.AllocateDescriptorSets(device,&descriptorSetAllocateInfo,&descriptorSet);
        ASSERT_VULKAN(result);
        
        VkDescriptorBufferInfo bufferInfo;
        bufferInfo.buffer = buffer;
        bufferInfo.offset = 0;
        bufferInfo.range = VK_WHOLE_SIZE;
        
        VkWriteDescriptorSet writeDescriptorSet = {};
        writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writeDescriptorSet.pNext = nullptr;
        writeDescriptorSet.dstSet = descriptorSet;
        writeDescriptorSet.dstBinding = 0;
        writeDescriptorSet.dstArrayElement = 0;
        writeDescriptorSet.descriptorCount = 1;
        writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        writeDescriptorSet.pImageInfo =  nullptr;
        writeDescriptorSet.pBufferInfo = &bufferInfo;
        writeDescriptorSet.pTexelBufferView = nullptr;
        
        std::cout << "before writing buffer descriptor Sets " << std::endl;
        dispatchTable.UpdateDescriptorSets(device,1,&writeDescriptorSet,0,nullptr);
    }
}
