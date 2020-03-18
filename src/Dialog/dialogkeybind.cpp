#include "dialogkeybind.h"
#include "ui_dialogkeybind.h"

#include <QThread>

#include "../Hotkey/hotkeyutil.h"

/** Sometimes when pressing a key on an external numpad,
 * they may function by simulating a numlock press, then
 * allowing the numpad keypress to go through, then
 * simulating a numlock unpress. The NumlockThread code
 * gives the keyPressEvent TIME_FOR_NUMPAD_HACK milliseconds
 * to pick up on the numpad keypress before assuming that
 * just the numlock key has been pressed.
 */
const unsigned long TIME_FOR_NUMPAD_HACK = 100;

class NumlockThread : public QThread
{
public:
    NumlockThread(DialogKeybind* dialog, std::shared_ptr<QMutex> _keyEventLock);
    void setNumlock(quint32 nativeKeyCode);
protected:
    void run();
private:
    DialogKeybind* dialog;
    std::shared_ptr<QMutex> _keyEventLock;
    quint32 _nativeKeyCode;
};

NumlockThread::NumlockThread(DialogKeybind* dialog, std::shared_ptr<QMutex> _keyEventLock) :
    dialog(dialog),
    _keyEventLock(_keyEventLock) {
}

void NumlockThread::run()
{
    this->msleep(TIME_FOR_NUMPAD_HACK);
    if (_keyEventLock->tryLock()) {
        dialog->setKey(_nativeKeyCode);
    }
}

void NumlockThread::setNumlock(quint32 nativeKeyCode) {
    _nativeKeyCode = nativeKeyCode;
}

DialogKeybind::DialogKeybind(QWidget* parent, WidgetKeybind* keybindWidget) :
    QDialog(parent),
    ui(new Ui::DialogKeybind),
    widget(keybindWidget)
{
    ui->setupUi(this);
    _keyEventLock = std::shared_ptr<QMutex>(new QMutex());
    numlockThread = std::shared_ptr<NumlockThread>(new NumlockThread(this, _keyEventLock));
}

DialogKeybind::~DialogKeybind()
{
    delete ui;
}

void DialogKeybind::setKey(quint32 nativeKeycode) {
    if (stopThreadFromSettingKey) return;
    widget->setKey(nativeKeycode);
    close();
}

void DialogKeybind::keyPressEvent(QKeyEvent* e) {
    // When a key is presed, update the widget value and close the dialog

    // quint32 key = e->nativeScanCode();
    // qDebug() << "[" << key << ", " << HotkeyUtil::keycodeToString(key) << "]";

    if (threadStarted) {
        // If the thread is alread started, then capture the key being pressed
        _keyEventLock->lock();
        setKey(e->nativeScanCode());
        stopThreadFromSettingKey = true;
        _keyEventLock->unlock();
    } else if (e->key() == Qt::Key_NumLock) {
        // If the key pressed is numlock, start the thread
        threadStarted = true;
        numlockThread->setNumlock(e->nativeScanCode());
        numlockThread->start();
    } else {
        // If there's no thread, and the key isn't numlock
        setKey(e->nativeScanCode());
    }
}
