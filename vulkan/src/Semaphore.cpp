//
// Created by Zach Lee on 2021/11/7.
//

#include "vulkan/Semaphore.h"
#include "vulkan/Device.h"
#include "vulkan/Basic.h"
#include "core/logger/Logger.h"
static const char* TAG = "Driver";

namespace sky::drv {

    Semaphore::Semaphore(Device& dev) : DevObject(dev), semaphore(VK_NULL_HANDLE)
    {
    }

    Semaphore::~Semaphore()
    {
        if (semaphore != VK_NULL_HANDLE) {
            vkDestroySemaphore(device.GetNativeHandle(), semaphore, VKL_ALLOC);
        }
    }

    bool Semaphore::Init(const Descriptor& des)
    {
        VkSemaphoreCreateInfo semaphoreInfo = {};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        auto rst = vkCreateSemaphore(device.GetNativeHandle(), &semaphoreInfo, VKL_ALLOC, &semaphore);
        if (rst != VK_SUCCESS) {
            LOG_E(TAG, "create semaphore failed, %d", rst);
            return false;
        }
        return true;
    }

    VkSemaphore Semaphore::GetNativeHandle() const
    {
        return semaphore;
    }
}