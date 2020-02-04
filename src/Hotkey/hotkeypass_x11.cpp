#include "hotkeypass.h"

#include <X11/XKBlib.h>
#include <X11/Xlib.h>

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

void HotkeyPass::triggerPassthrough() {
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

    KeySym nativKeyCode = XkbKeycodeToKeysym(display, KeyCode(currentNativeShortcut().key), 0, 0);
    // std::cout << "Native Key Code: " << nativKeyCode << std::endl;

    // Send a fake key press event to the window.
    XKeyEvent event = createKeyEvent(display, winFocus, winRoot, true, nativKeyCode, 0);
    XSendEvent(event.display, event.window, True, KeyPressMask, reinterpret_cast<XEvent*>(&event));
    // XFlush(display);

    // Send a fake key release event to the window.
    event = createKeyEvent(display, winFocus, winRoot, false, nativKeyCode, 0);
    XSendEvent(event.display, event.window, True, KeyPressMask, reinterpret_cast<XEvent*>(&event));
    XFlush(display);

    XCloseDisplay(display);
}
