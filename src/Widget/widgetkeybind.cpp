#include "widgetkeybind.h"
#include "../Dialog/dialogkeybind.h"

#include <QWidget>
#include <QSizePolicy>
#include <QHBoxLayout>
#include <QPushButton>
#include <QString>
#include "../Hotkey/hotkeyutil.h"

WidgetKeybind::WidgetKeybind(QWidget* parent) :
    QWidget(parent),
    _key(0),
    _hasKey(false)
{
    QHBoxLayout *layout = new QHBoxLayout();

    layout->setContentsMargins(QMargins(0, 0, 0, 0));

    int fixedHeight = 30;

    buttonKey = new QPushButton("Click here to set key");
    buttonKey->setFixedHeight(fixedHeight);
    layout->addWidget(buttonKey);
    QObject::connect(buttonKey, &QPushButton::clicked, this, &WidgetKeybind::onButtonKeyClicked);

    buttonDelete = new QPushButton(QIcon(":/icons/res/delete.png"), "");
    buttonDelete->setIconSize(QSize(fixedHeight, fixedHeight));
    buttonDelete->setFixedSize(QSize(fixedHeight, fixedHeight));
    layout->addWidget(buttonDelete);
    QObject::connect(buttonDelete, &QPushButton::clicked, this, &WidgetKeybind::onButtonDeleteClicked);

    setLayout(layout);
}

void WidgetKeybind::setKey(quint32 key) {
    _key = key;
    _hasKey = true;
    buttonKey->setText(HotkeyUtil::keycodeToString(_key));
    buttonDelete->setEnabled(_hasKey);
}

void WidgetKeybind::unSetKey() {
    _key = 0;
    _hasKey = false;
    buttonKey->setText("Click here to set key");
    buttonDelete->setEnabled(_hasKey);
}

quint32 WidgetKeybind::key() {
    return _key;
}

bool WidgetKeybind::hasKey() {
    return _hasKey;
}

void WidgetKeybind::onButtonKeyClicked() {
    DialogKeybind w(parentWidget(), this);
    w.exec();
}

void WidgetKeybind::onButtonDeleteClicked() {
    unSetKey();
}
