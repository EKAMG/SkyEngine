//
// Created by Zach Lee on 2021/11/12.
//


#pragma once
#include <engine/SkyEngine.h>
#include <framework/environment/Singleton.h>
#include <unordered_map>

namespace sky {

    namespace drv {
        class Driver;
        class Device;
        class SwapChain;
    }

    class RenderScene;

    class Render : public Singleton<Render>, public IEngineEvent {
    public:
        bool Init(const StartInfo&);

        void OnAddWorld(World&) override;
        void OnRemoveWorld(World&) override;

        void OnAddViewport(Viewport&) override;
        void OnRemoveViewport(Viewport&) override;

        void OnWorldTargetChange(World& world, Viewport& vp) override;

        void OnWindowResize(void* hwnd, uint32_t, uint32_t) override;

        void OnTick(float time) override;

        void Test();

        static void Reflect();

    private:
        friend class Singleton<Render>;
        Render() = default;
        ~Render();
        std::unordered_map<void*, drv::SwapChain*> swapChains;
        std::unordered_map<World*, RenderScene*> scenes;
    };

}