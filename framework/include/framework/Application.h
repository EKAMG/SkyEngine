//
// Created by Zach Lee on 2021/11/10.
//


#pragma once

#include <framework/window/NativeWindow.h>
#include <framework/interface/EngineInterface.h>
#include <core/util/DynamicModule.h>
#include <memory>

namespace sky {

    class Environment;

    class ApplicationImpl {
    public:
        ApplicationImpl() = default;
        virtual ~ApplicationImpl() = default;

        static ApplicationImpl* Create();

        virtual void PumpMessages() = 0;

        virtual bool IsExit() const = 0;

        virtual void SetExit() = 0;
    };

    class Application {
    public:
        Application();
        ~Application();

        bool Init(const StartInfo&);

        void Mainloop();

        void Shutdown();

        void SetExit();

        NativeWindow* CreateNativeWindow(const NativeWindow::Descriptor&);

        IEngine* GetEngine() const;

        ApplicationImpl* GetImpl() const;

    private:
        ApplicationImpl* impl;
        IEngine* engineInstance;
        Environment* env;
        std::unique_ptr<DynamicModule> engineModule;
        std::vector<std::unique_ptr<DynamicModule>> modules;
    };

}
