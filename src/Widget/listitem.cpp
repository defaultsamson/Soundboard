#include "listitem.h"

#include "../mainapp.h"
#include "../Hotkey/hotkeypass.h"
#include "../Hotkey/hotkeyutil.h"
#include "../Hotkey/hotkeytrigger.h"
#include <QHotkey>
#include <functional>

ListItem::ListItem(Main* main) :
    HotkeyTrigger(main, [this]{ trigger(); })
{
}


void ListItem::load(const QJsonObject &json) {
    setText(json["name"].toString());
    if (json["haskey"].toBool())
        setKey(static_cast<quint32>(json["key"].toInt()));
}

void ListItem::save(QJsonObject &json) {
    json["name"] = text();
    json["haskey"] = hasKey();
    json["key"] = static_cast<int>(key());
}
