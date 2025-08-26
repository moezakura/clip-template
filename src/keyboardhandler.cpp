#include "keyboardhandler.h"

// X11 headers - order matters
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/XTest.h>
#undef None
#undef KeyPress
#undef KeyRelease
#include <unistd.h>

void KeyboardHandler::simulateKeyPress(Display *display, Window window, KeySym keysym)
{
    if (!display || window == 0) return;
    
    KeyCode keycode = XKeysymToKeycode(display, keysym);
    if (keycode == 0) return;
    
    // Generate key press event
    XKeyEvent event;
    event.display = display;
    event.window = window;
    event.root = DefaultRootWindow(display);
    event.subwindow = 0;
    event.time = CurrentTime;
    event.x = 1;
    event.y = 1;
    event.x_root = 1;
    event.y_root = 1;
    event.same_screen = True;
    event.keycode = keycode;
    event.state = 0;
    event.type = 2; // KeyPress constant value
    
    XSendEvent(display, window, True, KeyPressMask, (XEvent*)&event);
    XFlush(display);
    usleep(10000);
    
    // Generate key release event
    event.type = 3; // KeyRelease constant value
    XSendEvent(display, window, True, KeyReleaseMask, (XEvent*)&event);
    XFlush(display);
}

void KeyboardHandler::simulateKeyCombo(Display *display, Window window, KeySym modifier, KeySym key)
{
    if (!display || window == 0) return;
    
    KeyCode modCode = XKeysymToKeycode(display, modifier);
    KeyCode keyCode = XKeysymToKeycode(display, key);
    
    if (modCode == 0 || keyCode == 0) return;
    
    // Press modifier
    XTestFakeKeyEvent(display, modCode, True, 0);
    XFlush(display);
    usleep(10000);
    
    // Press key
    XTestFakeKeyEvent(display, keyCode, True, 0);
    XFlush(display);
    usleep(10000);
    
    // Release key
    XTestFakeKeyEvent(display, keyCode, False, 0);
    XFlush(display);
    usleep(10000);
    
    // Release modifier
    XTestFakeKeyEvent(display, modCode, False, 0);
    XFlush(display);
}

Window KeyboardHandler::getActiveWindow(Display *display)
{
    if (!display) return 0;
    
    Window root = DefaultRootWindow(display);
    Window parent, *children;
    unsigned int nchildren;
    
    Atom actualType;
    int actualFormat;
    unsigned long nItems, bytesAfter;
    unsigned char *prop = nullptr;
    
    Atom activeWindow = XInternAtom(display, "_NET_ACTIVE_WINDOW", False);
    
    if (XGetWindowProperty(display, root, activeWindow, 0L, 1L, False,
                          33, &actualType, &actualFormat, // XA_WINDOW = 33
                          &nItems, &bytesAfter, &prop) == Success) {
        if (prop) {
            Window active = *((Window*)prop);
            XFree(prop);
            return active;
        }
    }
    
    return 0;
}

Window KeyboardHandler::getFocusedWindow(Display *display)
{
    if (!display) return 0;
    
    Window focusWindow;
    int revertTo;
    XGetInputFocus(display, &focusWindow, &revertTo);
    
    if (focusWindow == 0 || focusWindow == PointerRoot) {
        return getActiveWindow(display);
    }
    
    return focusWindow;
}