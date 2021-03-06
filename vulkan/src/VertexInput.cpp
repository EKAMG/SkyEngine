//
// Created by Zach Lee on 2022/1/18.
//

#include <vulkan/VertexInput.h>
#include <core/hash/Crc32.h>
#include <core/hash/Hash.h>

namespace sky::drv {

    VertexInput::Builder& VertexInput::Builder::Begin()
    {
        if (!vertexInput) {
            vertexInput = std::make_shared<VertexInput>();
        }
        return *this;
    }

    VertexInput::Builder& VertexInput::Builder::AddAttribute(uint32_t loc, uint32_t binding, uint32_t off, VkFormat format)
    {
        vertexInput->attributes.emplace_back(VkVertexInputAttributeDescription{loc, binding, format, off});
        return *this;
    }

    VertexInput::Builder& VertexInput::Builder::AddStream(uint32_t binding, uint32_t stride, VkVertexInputRate rate)
    {
        vertexInput->bindings.emplace_back(VkVertexInputBindingDescription{binding, stride, rate});
        return *this;
    }

    std::shared_ptr<VertexInput> VertexInput::Builder::Build()
    {
        vertexInput->vInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInput->vInputInfo.flags = 0;
        vertexInput->vInputInfo.pNext = nullptr;
        vertexInput->vInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(vertexInput->attributes.size());
        vertexInput->vInputInfo.pVertexAttributeDescriptions = vertexInput->attributes.data();
        vertexInput->vInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(vertexInput->bindings.size());
        vertexInput->vInputInfo.pVertexBindingDescriptions = vertexInput->bindings.data();
        vertexInput->hash = 0;
        HashCombine32(vertexInput->hash, Crc32::Cal((uint8_t*)vertexInput->attributes.data(), static_cast<uint32_t>(vertexInput->attributes.size())));
        HashCombine32(vertexInput->hash, Crc32::Cal((uint8_t*)vertexInput->bindings.data(), static_cast<uint32_t>(vertexInput->bindings.size())));
        return vertexInput;
    }

    const VkPipelineVertexInputStateCreateInfo* VertexInput::GetInfo() const
    {
        return &vInputInfo;
    }

    uint32_t VertexInput::GetHash() const
    {
        return hash;
    }

}