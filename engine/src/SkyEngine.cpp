//
// Created by Zach Lee on 2021/11/11.
//

#include <SkyEngine.h>
#include <world/World.h>
#include <render/Render.h>

namespace sky {

    bool SkyEngine::Init(const StartInfo& startInfo)
    {
        render = new Render();
        render->Init(startInfo);
        RegisterEngineListener(render);
        return true;
    }

    void SkyEngine::Tick(float time)
    {
        for (auto& module : modules) {
            module->Tick(time);
        }

        for (auto& world : worlds) {
            world->Tick(time);
        }
    }

    void SkyEngine::DeInit()
    {
        for (auto& module : modules) {
            module->Stop();
        }

        if (render != nullptr) {
            delete render;
            render = nullptr;
        }

        eventListeners.clear();
    }

    void SkyEngine::AddWorld(World& world)
    {
        auto iter = std::find(worlds.begin(), worlds.end(), &world);
        if (iter != worlds.end()) {
            return;
        }
        worlds.emplace_back(&world);
        EachListener([&world](IEngineEvent* event) {
            event->OnAddWorld(world);
        });
    }

    void SkyEngine::RemoveWorld(World& world)
    {
        auto iter = std::find(worlds.begin(), worlds.end(), &world);
        if (iter != worlds.end()) {
            worlds.erase(iter);
        }
        EachListener([&world](IEngineEvent* event) {
            event->OnRemoveWorld(world);
        });
    }

    void SkyEngine::AddViewport(Viewport& vp)
    {
        auto iter = std::find(viewports.begin(), viewports.end(), &vp);
        if (iter != viewports.end()) {
            return;
        }
        viewports.emplace_back(&vp);
        EachListener([&vp](IEngineEvent* event) {
            event->OnAddViewport(vp);
        });
    }

    void SkyEngine::RemoveViewport(Viewport& vp)
    {
        auto iter = std::find(viewports.begin(), viewports.end(), &vp);
        if (iter != viewports.end()) {
            viewports.erase(iter);
        }
        EachListener([&vp](IEngineEvent* event) {
            event->OnRemoveViewport(vp);
        });
    }

    void SkyEngine::RegisterEngineListener(IEngineEvent* event)
    {
        if (event != nullptr) {
            eventListeners.emplace_back(event);
        }
    }

    void SkyEngine::UnRegisterEngineListener(IEngineEvent* event)
    {
        auto iter = std::find(eventListeners.begin(), eventListeners.end(), event);
        if (iter != eventListeners.end()) {
            eventListeners.erase(iter);
        }
    }

    void SkyEngine::RegisterModule(IModule* module)
    {
        if (module != nullptr) {
            module->Start();
            modules.emplace_back(module);
        }
    }

    void SkyEngine::UnRegisterModule(IModule* module)
    {
        auto iter = std::find(modules.begin(), modules.end(), module);
        if (iter != modules.end()) {
            (*iter)->Stop();
            modules.erase(iter);
        }
    }

}