//
// Created by Zach Lee on 2021/11/7.
//

#pragma once
#include <core/template/ReferenceObject.h>
#include <memory>

namespace sky::drv {

    class Device;

    class DevObject {
    public:
        DevObject(Device& dev) : device(dev) {}
        virtual ~DevObject() = default;

    protected:
        Device& device;
    };

}
