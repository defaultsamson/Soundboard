#include "listitem.h"

#include "../mainwindow.h"
#include <QHotkey>
#include <QObject>
#include <iostream>

#include <X11/XKBlib.h>
#include <X11/Xlib.h>

ListItem::ListItem(Main *main) :
    QObject(main),
    main(main),
    _key(-1),
    hotkey(new QHotkey(main))
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

// The key code to be sent.
// A full list of available codes can be found in /usr/include/X11/keysymdef.h
//#define KEYCODE1 XK_a

// Function to create a keyboard event
XKeyEvent createKeyEvent(Display *display, Window &win, Window &winRoot, bool press, KeySym keycode, int modifiers) {
   XKeyEvent event;

   event.display     = display;
   event.window      = win;
   event.root        = winRoot;
   event.subwindow   = None;
   event.time        = CurrentTime;
   event.x           = 1;
   event.y           = 1;
   event.x_root      = 1;
   event.y_root      = 1;
   event.same_screen = True;
   event.keycode     = XKeysymToKeycode(display, keycode);
   event.state       = static_cast<unsigned int>(modifiers);

   if(press)
      event.type = KeyPress;
   else
      event.type = KeyRelease;

   return event;
}

void ListItem::trigger() {
    // Obtain the X11 display.
    Display *display = XOpenDisplay(nullptr);
    if(display == nullptr)
       return;

    // Get the root window for the current display.
    Window winRoot = XDefaultRootWindow(display);

    // Find the window which has the current keyboard focus.
    Window winFocus;
    int    revert;
    XGetInputFocus(display, &winFocus, &revert);

    //std::cout << "noice: " << XK_1 << std::endl;
    KeySym nativKeyCode = XkbKeycodeToKeysym(display, KeyCode(hotkey->currentNativeShortcut().key), 0, 0);
    std::cout << "noice: " << nativKeyCode << std::endl;

    // Send a fake key press event to the window.
    XKeyEvent event = createKeyEvent(display, winFocus, winRoot, true, nativKeyCode, 0);
    XSendEvent(event.display, event.window, True, KeyPressMask, reinterpret_cast<XEvent*>(&event));
    // XFlush(display);

    // Send a fake key release event to the window.
    event = createKeyEvent(display, winFocus, winRoot, false, nativKeyCode, 0);
    XSendEvent(event.display, event.window, True, KeyPressMask, reinterpret_cast<XEvent*>(&event));
    XFlush(display);

    XCloseDisplay(display);
    // std::cout << "NOICE MEME" << std::endl;
}
