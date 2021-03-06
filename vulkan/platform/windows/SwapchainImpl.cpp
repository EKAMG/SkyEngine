//
// Created by Zach Lee on 2021/11/28.
//

#include "vulkan/Swapchain.h"
#include <windows.h>
#include <vulkan/Device.h>
#include <vulkan/vulkan_win32.h>
#include <vulkan/Basic.h>
#include <core/logger/Logger.h>

static const char* TAG = "Driver";

namespace sky::drv {

    bool SwapChain::CreateSurface()
    {
        VkWin32SurfaceCreateInfoKHR surfaceInfo = {};
        surfaceInfo.sType     = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
        surfaceInfo.hwnd      = (HWND)descriptor.window;
        surfaceInfo.hinstance = GetModuleHandle(0);
        VkResult rst = vkCreateWin32SurfaceKHR(device.GetInstance(), &surfaceInfo, VKL_ALLOC, &surface);
        if (rst != VK_SUCCESS) {
            LOG_E(TAG, "create win32 surface failed, %d", rst);
            return false;
        }
        return true;
    }

}