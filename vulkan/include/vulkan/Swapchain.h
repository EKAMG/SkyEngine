//
// Created by Zach Lee on 2021/12/9.
//

#pragma once

#include "vulkan/DevObject.h"
#include "vulkan/vulkan.h"
#include <vector>

namespace sky::drv {

    class Device;
    class Queue;
    class ImageView;

    class Swapchain : public DevObject {
    public:
        ~Swapchain();

        struct Descriptor {
            void* window = nullptr;
            VkFormat preferredFormat = VK_FORMAT_R8G8B8A8_UNORM;
            VkPresentModeKHR preferredMode = VK_PRESENT_MODE_MAILBOX_KHR;
            VkSurfaceTransformFlagBitsKHR preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
            VkCompositeAlphaFlagBitsKHR compositeAlpha = VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR;
        };

        bool Init(const Descriptor&);

        VkSwapchainKHR GetNativeHandle() const;

    private:
        friend class Device;
        Swapchain(Device&);

        bool CreateSurface(const Descriptor&);
        void DestroySurface();

        bool CreateSwapchain(const Descriptor&);
        void DestroySwapchain();

        VkSurfaceKHR surface;
        VkSwapchainKHR swapchain;
        Queue* queue;
        uint32_t imageCount;
        uint32_t currentImage;
        VkExtent2D extent;
        VkSurfaceCapabilitiesKHR capabilities;
        VkSurfaceFormatKHR format;
        VkPresentModeKHR mode;
        std::vector<ImageView*> views;
    };

}