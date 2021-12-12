//
// Created by Zach Lee on 2021/12/12.
//

#pragma once
#include <QWidget>
#include <engine/world/Viewport.h>

namespace sky::editor {

    class ViewportWidget : public QWidget {
        Q_OBJECT
    public:
        ViewportWidget(QWidget* parent);
        ~ViewportWidget();

        void Init();

    private:
        Viewport* viewport;
    };

}
