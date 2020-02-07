#include "listitem.h"

#include "../mainapp.h"
#include "../Hotkey/hotkeypass.h"
#include <QObject>
#include <QHotkey>

ListItem::ListItem(Main *main) :
    QObject(main),
    main(main),
    _hasKey(false),
    _key(0),
    hotkey(new HotkeyPass(main))
{
    // Sets up the keybind
    QObject::connect(hotkey, &QHotkey::activated, this, &ListItem::trigger);
}

ListItem::~ListItem() {
    delete hotkey;
}


quint32 ListItem::key() {
    return _key;
}

void ListItem::unSetKey() {
    _hasKey = false;
}

void ListItem::setKey(quint32 k) {
    _key = k;
    _hasKey = true;
    hotkey->setNativeShortcut(QHotkey::NativeShortcut(_key));
}

bool ListItem::hasKey() {
    return _hasKey;
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

void ListItem::trigger() {
}
