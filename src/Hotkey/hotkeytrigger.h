#ifndef HOTKEYTRIGGER_H
#define HOTKEYTRIGGER_H

#include <functional>
#include <QObject>
#include <QHotkey>
#include "../Hotkey/hotkeypass.h"

class Main;

class HotkeyTrigger : public QObject
{
    Q_OBJECT

public:
    HotkeyTrigger(Main* main, std::function<void()> trigger);
    ~HotkeyTrigger();
    bool hasKey() { return _hasKey; }
    quint32 key() { return _key; }
    void unSetKey() { _hasKey = false; }
    void setKey(quint32 key);
    void reg() { if (_hasKey) _hotkey->setRegistered(true); }
    void unreg() { _hotkey->setRegistered(false); }

protected:
    Main* main;
    bool _hasKey;
    quint32 _key;
    HotkeyPass *_hotkey;
private:
    std::function<void()> _trigger;
};

#endif // HOTKEYTRIGGER_H
