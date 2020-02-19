#include "widgetkeybind.h"
#include "../Dialog/dialogkeybind.h"

#include <QKeySequence>
#include <QPushButton>
#include <QWidget>
#include <QString>
#include "../Hotkey/hotkeyutil.h"

WidgetKeybind::WidgetKeybind(QWidget* parent) :
    QPushButton(parent),
    _key(0),
    _hasKey(false)
{
    setText("Click here to set key");
    QObject::connect(this, &QPushButton::clicked, this, &WidgetKeybind::clickReciever);
}

void WidgetKeybind::setKey(quint32 key) {
    _key = key;
    _hasKey = true;
    setText(HotkeyUtil::keycodeToString(_key));
}

void WidgetKeybind::unSetKey() {
    _key = 0;
    _hasKey = false;
    setText("Click here to set key");
}

quint32 WidgetKeybind::key() {
    return _key;
}

bool WidgetKeybind::hasKey() {
    return _hasKey;
}

void WidgetKeybind::clickReciever() {
    DialogKeybind w(parentWidget(), this);
    w.exec();
}
