//
// Created by Zach Lee on 2021/11/7.
//
#pragma once
#include "vulkan/DevObject.h"
#include "vulkan/vulkan.h"
#include "vk_mem_alloc.h"

namespace sky::drv {

    class Device;

    class ImageView : public DevObject {
    public:
        ~ImageView();

        struct Descriptor {
            VkImageViewType viewType      = VK_IMAGE_VIEW_TYPE_2D;
            VkFormat format               = VK_FORMAT_UNDEFINED;
            VkComponentMapping components = {
                VK_COMPONENT_SWIZZLE_IDENTITY,
                VK_COMPONENT_SWIZZLE_IDENTITY,
                VK_COMPONENT_SWIZZLE_IDENTITY,
                VK_COMPONENT_SWIZZLE_IDENTITY
            };
            VkImageSubresourceRange subResourceRange = {
                VK_IMAGE_ASPECT_COLOR_BIT,
                0, 1, 0, 1
            };
        };

        bool Init(const Descriptor&);

        VkImageView GetNativeHandle() const;

    private:
        friend class Image;
        friend class Swapchain;
        ImageView(Device&, VkImage image);

        VkImage image;
        VkImageView view;
    };

}