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
    class Semaphore;

    class SwapChain : public DevObject {
    public:
        ~SwapChain();

        struct Descriptor {
            void* window = nullptr;
            uint32_t width = 1;
            uint32_t height = 1;
            VkFormat preferredFormat = VK_FORMAT_R8G8B8A8_UNORM;
            VkPresentModeKHR preferredMode = VK_PRESENT_MODE_MAILBOX_KHR;
            VkSurfaceTransformFlagBitsKHR preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
            VkCompositeAlphaFlagBitsKHR compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        };

        struct PresentInfo {
            std::vector<Semaphore*> signals;
        };

        bool Init(const Descriptor&);

        VkSwapchainKHR GetNativeHandle() const;

        VkFormat GetFormat() const;

        VkExtent2D GetExtent() const;

        const std::vector<ImageView*>& GetViews() const;

        void Present(const PresentInfo&) const;

        VkResult AcquireNext() const;

        Semaphore* GetAvailableSemaphore() const;

        ImageView* GetCurrentImageView() const;

        void Resize(uint32_t width, uint32_t height);

    private:
        friend class Device;
        SwapChain(Device&);

        bool CreateSurface();
        void DestroySurface();

        bool CreateSwapChain();
        void DestroySwapChain();

        VkSurfaceKHR surface;
        VkSwapchainKHR swapChain;
        Queue* queue;
        uint32_t imageCount;
        mutable uint32_t currentImage;
        VkExtent2D extent;
        VkSurfaceCapabilitiesKHR capabilities;
        VkSurfaceFormatKHR format;
        VkPresentModeKHR mode;
        std::vector<ImageView*> views;
        Semaphore* imageAvailable;
        Descriptor descriptor;
    };

}
