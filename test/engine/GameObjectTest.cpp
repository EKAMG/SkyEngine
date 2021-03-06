//
// Created by Zach Lee on 2021/12/3.
//

#include <gtest/gtest.h>
#include <engine/world/World.h>
#include <engine/world/GameObject.h>
#include <engine/world/TransformComponent.h>

class TestComponent : public sky::Component {
public:
    TestComponent() = default;
    ~TestComponent() = default;

    TYPE_RTTI_WITH_VT(TestComponent)
};

using namespace sky;
TEST(EngineTest, GameObjectTest)
{
    World world;

    auto go = world.CreateGameObject("test");
    ASSERT_NE(go, nullptr);

    auto test1 = go->AddComponent<TestComponent>();
    auto test2 = go->GetComponent<TestComponent>();
    ASSERT_EQ(test1, test2);
}

TEST(EngineTest, TransformComponentTest)
{
    World world;

    auto go1 = world.CreateGameObject("t1");
    auto t1 = go1->GetComponent<TransformComponent>();

    auto go2 = world.CreateGameObject("t2");
    auto t2 = go2->GetComponent<TransformComponent>();
    t2->SetParent(t1);
    ASSERT_EQ(t2->GetParent(), t1);

    auto go3 = world.CreateGameObject("t3");
    auto t3 = go3->GetComponent<TransformComponent>();
    t3->SetParent(t1);
    ASSERT_EQ(t3->GetParent(), t1);

    auto go4 = world.CreateGameObject("t4");
    auto t4 = go4->GetComponent<TransformComponent>();
    t4->SetParent(t2);
    ASSERT_EQ(t4->GetParent(), t2);
    ASSERT_EQ(t2->GetChildren().size(), 1);

    t4->SetParent(t1);
    ASSERT_EQ(t4->GetParent(), t1);
    ASSERT_EQ(t2->GetChildren().size(), 0);

    t3->SetParent(t2);
    t4->SetParent(t3);
    ASSERT_EQ(t4->GetParent(), t3);
    ASSERT_EQ(t3->GetParent(), t2);
    ASSERT_EQ(t2->GetParent(), t1);

    ASSERT_EQ(world.GetGameObjects().size(), size_t(5));
}

struct TestSystem : public IComponentListener {
    TestSystem(int& val) : p(val)
    {
        ComponentFactory<TestComponent>::Get()->RegisterListener(this);
    }

    ~TestSystem()
    {
        ComponentFactory<TestComponent>::Get()->UnRegisterListener(this);
    }
    void OnAddComponent(GameObject* go, Component*)
    {
        p += 10;
    }
    void OnRemoveComponent(GameObject* go, Component*)
    {
        p += 20;
    }
    int& p;
};

TEST(EngineTest, ListenerTest)
{
    int val = 0;
    TestSystem system(val);

    World world;
    auto go = world.CreateGameObject("test");

    go->AddComponent<TestComponent>();
    ASSERT_EQ(val, 10);

    go->AddComponent<TestComponent>();
    ASSERT_EQ(val, 10);

    go->RemoveComponent<TestComponent>();
    ASSERT_EQ(val, 30);

    go->RemoveComponent<TestComponent>();
    ASSERT_EQ(val, 30);
}