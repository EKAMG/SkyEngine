//
// Created by Zach Lee on 2021/11/12.
//


#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <core/math/Rect.h>
#include <engine/ServiceManager.h>

namespace sky {

    class Viewport;
    class World;
    class GameObject;

    class World {
    public:
        World();
        ~World();

        GameObject* CreateGameObject(const std::string& name);

        void DestroyGameObject(GameObject*);

        void Tick(float);

        const std::vector<GameObject*>& GetGameObjects() const;

        GameObject* GetRoot();

        ServiceManager* GetServiceManager() const;

        static void Reflect();

    private:
        GameObject* root;
        std::unique_ptr<ServiceManager> serviceManager;
        std::vector<GameObject*> gameObjects;
    };

}