#include "listitem.h"

#include "../mainapp.h"
#include "../Hotkey/hotkeypass.h"
#include <QObject>
#include <QHotkey>

ListItem::ListItem(Main *main) :
    QObject(main),
    main(main),
    _key(-1),
    hotkey(new HotkeyPass(main))
{
    // Sets up the keybind
    QObject::connect(hotkey, &QHotkey::activated, this, &ListItem::trigger);
}

ListItem::~ListItem() {
    delete hotkey;
}

int ListItem::key() {
    return _key;
}

int ListItem::nativeKey() {
    return _native;
}

void ListItem::setKey(int k) {
    _key = k;
    if (k < 0) return;
    hotkey->setShortcut(QKeySequence(k), false);
}

void ListItem::setNativeKey(int k) {
    _native = k;
    if (k < 0) return;
    hotkey->setNativeShortcut(QHotkey::NativeShortcut(_native));
}

void ListItem::load(const QJsonObject &json) {
    setText(json["name"].toString());
    setKey(json["key"].toInt());
    setNativeKey(json["native"].toInt());
}

void ListItem::save(QJsonObject &json) {
    json["name"] = text();
    json["key"] = key();
    json["native"] = nativeKey();
}

void ListItem::trigger() {
}
