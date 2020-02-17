#ifndef HOTKEYPASS_H
#define HOTKEYPASS_H

#include <QHotkey>

class HotkeyPass : public QHotkey
{
public:
    HotkeyPass(QObject* parent = nullptr);
protected:
    void triggerPassthrough();
};

#endif // HOTKEYPASS_H
