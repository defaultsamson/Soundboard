#include "listitem.h"

#include "mainwindow.h"
#include <QHotkey>
#include <QObject>

ListItem::ListItem(MainWindow *main):
    _key(-1),
    hotkey(new QHotkey(main))
{
    // Sets up the keybind (must pass back to the
    // main window before the ListItem can touch it)
    QObject::connect(hotkey, &QHotkey::activated, main, [&](){
        main->trigger(this);
    });
}

ListItem::~ListItem() {
    delete hotkey;
}

int ListItem::key() {
    return _key;
}

void ListItem::setKey(int k) {
    _key = k;
    if (k < 0) return;
    hotkey->setShortcut(QKeySequence(k), false);
}

void ListItem::load(const QJsonObject &json) {
    setText(json["name"].toString());
    setKey(json["key"].toInt());
}

void ListItem::save(QJsonObject &json) {
    json["name"] = text();
    json["key"] = key();
}
