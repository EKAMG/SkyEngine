//
// Created by Zach Lee on 2021/12/15.
//

#include <editor/inspector/InspectorWidget.h>
#include <editor/inspector/InspectorBase.h>
#include <editor/inspector/PropertyUtil.h>
#include <QPushButton>
#include <QVBoxLayout>
#include <engine/world/GameObject.h>

#include <editor/inspector/PropertyTransform.h>

namespace sky::editor {

    InspectorWidget::InspectorWidget(QWidget* parent)
        : QDockWidget(parent)
    {
        setWindowTitle(tr("Inspector"));
        auto widget = new QWidget(this);
        setWidget(widget);
        auto rootLayout = new QVBoxLayout(widget);
        rootLayout->setContentsMargins(0, 0, 0, 0);
        rootLayout->setAlignment(Qt::AlignTop);

        auto button = new QPushButton(tr("Add Component"), widget);
        groupWidget = new QWidget(widget);

        rootLayout->setContentsMargins(0, 0, 0, 0);
        rootLayout->setSpacing(0);
        rootLayout->addWidget(groupWidget);
        rootLayout->addWidget(button);

        layout = new QVBoxLayout(groupWidget);
        layout->setAlignment(Qt::AlignTop);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);
    }

    void InspectorWidget::AddComponent(Component* comp)
    {
        const TypeInfoRT* info = comp->GetTypeInfo();
        auto node = GetTypeNode(info);
        if (node == nullptr) {
            return;
        }
        auto inspector = new InspectorBase(groupWidget);
        inspector->SetName(node->info->typeId.data());

        layout->addWidget(inspector);
        for(auto& mem : node->members) {
            if (!util::IsVisible(mem.second)) {
                continue;
            }
            auto prop = util::CreateByTypeMemberInfo(mem.second, groupWidget);
            prop->SetInstance(comp, mem.first.data(), mem.second);
            layout->addWidget(prop);
        }

        groups.emplace_back(inspector);
    }

    void InspectorWidget::Clear()
    {
        while (QLayoutItem* child = layout->takeAt(0))
        {
            delete child->widget();
            delete child;
        }
        groups.clear();
    }

    void InspectorWidget::SetWorldItem(WorldItem* item)
    {
        selectedItem = item;
        Refresh();
    }

    void InspectorWidget::Refresh()
    {
        if (selectedItem == nullptr) {
            return;
        }
        Clear();
        auto go = selectedItem->go;
        auto& comps = go->GetComponents();
        for (auto& comp : comps) {
            AddComponent(comp);
        }
    }

}