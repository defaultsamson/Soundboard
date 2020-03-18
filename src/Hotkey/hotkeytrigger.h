#ifndef HOTKEYTRIGGER_H
#define HOTKEYTRIGGER_H

#include "../Hotkey/hotkeypass.h"

#include <functional>
#include <QObject>

class Main;

class HotkeyTrigger : public QObject
{
    Q_OBJECT

public:
    HotkeyTrigger(Main* _main, std::function<void()> trigger);
    ~HotkeyTrigger();
    bool hasKey() { return _hasKey; }
    quint32 key() { return _key; }
    void unSetKey() { _hasKey = false; _hotkey->resetShortcut(); }
    void setKey(quint32 key);
    void reg() { if (_hasKey) _hotkey->setRegistered(true); }
    void unreg() { _hotkey->setRegistered(false); }

protected:
    Main* _main;
    bool _hasKey;
    quint32 _key;
    HotkeyPass *_hotkey;
private:
    std::function<void()> _trigger;
};

#endif // HOTKEYTRIGGER_H
