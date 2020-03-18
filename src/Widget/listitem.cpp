#include "listitem.h"

#include "../mainapp.h"

ListItem::ListItem(Main* _main) :
    HotkeyTrigger(_main, [this]{ trigger(); })
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
