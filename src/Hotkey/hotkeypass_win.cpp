#include "hotkeypass.h"

// Because the SendInput function is only supported in
// Windows 2000 and later, WINVER needs to be set as
// follows so that SendInput gets defined when windows.h
// is included below.
#define WINVER 0x0500
#include <windows.h>

void HotkeyPass::triggerPassthrough() {
    // Unregister so the program doesn't pick up on its own simulated keypress
    setRegistered(false);

    // This structure will be used to create the keyboard input event
    INPUT ip;

    // Set up a generic keyboard event
    ip.type = INPUT_KEYBOARD;
    ip.ki.wScan = 0; // hardware scan code for key
    ip.ki.time = 0;
    ip.ki.dwExtraInfo = 0;
    ip.ki.wVk = WORD(currentNativeShortcut().key); // virtual-key code
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));

    // Re-register the hotkey
    setRegistered(true);
}
