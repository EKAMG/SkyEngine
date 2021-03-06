//
// Created by Zach Lee on 2021/11/12.
//


#pragma once

#include <string>
#include <list>
#include <engine/world/Component.h>
#include <core/util/Rtti.h>

namespace sky {
    class World;

    class GameObject {
    public:
        GameObject(const GameObject&) = delete;
        GameObject& operator=(const GameObject&) = delete;

        template <typename T>
        inline T* AddComponent()
        {
            auto info = TypeInfoObj<T>::Get()->RtInfo();
            auto iter = std::find_if(components.begin(), components.end(), [info](Component* comp) {
                return comp->GetTypeInfo() == info;
            });
            if (iter == components.end()) {
                auto comp = ComponentFactory<T>::CreateComponent();
                comp->OnInit();
                comp->object = this;
                ComponentFactory<T>::Get()->template ForEach<&IComponentListener::OnAddComponent>(this, comp);
                components.emplace_back(comp);
                return comp;
            }
            return static_cast<T*>(*iter);
        }

        template <typename T>
        inline void RemoveComponent()
        {
            auto info = TypeInfoObj<T>::Get()->RtInfo();
            auto iter = std::find_if(components.begin(), components.end(), [info](Component* comp) {
                return comp->GetTypeInfo() == info;
            });
            if (iter != components.end()) {
                (*iter)->OnDestroy();
                ComponentFactory<T>::Get()->template ForEach<&IComponentListener::OnRemoveComponent>(this, *iter);
                delete *iter;
                components.erase(iter);
            }
        }

        template <typename T>
        inline T* GetComponent()
        {
            auto info = TypeInfoObj<T>::Get()->RtInfo();
            auto iter = std::find_if(components.begin(), components.end(), [info](Component* comp) {
                auto ci = comp->GetTypeInfo();
                return comp->GetTypeInfo() == info;
            });
            if (iter != components.end()) {
                return static_cast<T*>(*iter);
            }
            return nullptr;
        }

        template <typename T>
        inline const T* GetComponent() const
        {
            auto info = TypeInfoObj<T>::Get()->RtInfo();
            auto iter = std::find_if(components.begin(), components.end(), [info](Component* comp) {
                return comp->GetTypeInfo() == info;
            });
            if (iter != components.end()) {
                return static_cast<T*>(*iter);
            }
            return nullptr;
        }

        uint32_t GetId() const;

        const std::string& GetName() const;

        void SetParent(GameObject* gameObject);

        void Tick(float time);

        using ComponentList = std::list<Component*>;
        ComponentList& GetComponents();

    private:
        friend class World;
        ~GameObject();

        GameObject(const std::string& str) : name(str) {}
        World* world = nullptr;
        uint32_t objId = 0;
        std::string name;
        ComponentList components;
    };

}