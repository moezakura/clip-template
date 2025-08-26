#ifndef KEYBOARDHANDLER_H
#define KEYBOARDHANDLER_H

// Forward declarations for X11 types
typedef unsigned long XID;
typedef XID Window;
typedef XID KeySym;
struct _XDisplay;
typedef struct _XDisplay Display;

class KeyboardHandler {
public:
    static void simulateKeyPress(Display *display, Window window, KeySym keysym);
    static void simulateKeyCombo(Display *display, Window window, KeySym modifier, KeySym key);
    static Window getActiveWindow(Display *display);
    static Window getFocusedWindow(Display *display);
};

#endif // KEYBOARDHANDLER_H