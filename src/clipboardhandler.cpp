#include "clipboardhandler.h"
#include <QApplication>
#include <QClipboard>
#include <QString>

// X11 headers must be included after Qt headers
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/XTest.h>
#include <X11/keysym.h>
#undef None
#undef KeyPress
#undef KeyRelease
#undef FocusIn
#undef FocusOut
#include <unistd.h>

ClipboardHandler::ClipboardHandler() = default;
ClipboardHandler::~ClipboardHandler() = default;

void ClipboardHandler::copyToClipboard(const std::string &text)
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(QString::fromStdString(text));
}

void ClipboardHandler::pasteToWindow(Window window)
{
    if (window == 0) return;
    
    Display *display = XOpenDisplay(nullptr);
    if (!display) return;
    
    // Set focus to the target window
    setFocusToWindow(display, window);
    
    // Wait a bit for focus to settle
    usleep(50000); // 50ms
    
    // Send Ctrl+V
    sendCtrlV(display, window);
    
    XCloseDisplay(display);
}

void ClipboardHandler::setFocusToWindow(Display *display, Window window)
{
    // Raise the window
    XRaiseWindow(display, window);
    
    // Set input focus
    XSetInputFocus(display, window, RevertToPointerRoot, CurrentTime);
    
    // Send focus event
    XEvent event;
    event.type = 9; // FocusIn constant value
    event.xfocus.type = 9; // FocusIn constant value
    event.xfocus.serial = 0;
    event.xfocus.send_event = True;
    event.xfocus.display = display;
    event.xfocus.window = window;
    event.xfocus.mode = NotifyNormal;
    event.xfocus.detail = NotifyNonlinear;
    
    XSendEvent(display, window, True, FocusChangeMask, &event);
    XFlush(display);
}

void ClipboardHandler::sendCtrlV(Display *display, Window window)
{
    // Get keycodes
    KeyCode ctrlKey = XKeysymToKeycode(display, XK_Control_L);
    KeyCode vKey = XKeysymToKeycode(display, XK_v);
    
    // Press Ctrl
    XTestFakeKeyEvent(display, ctrlKey, True, 0);
    XFlush(display);
    usleep(10000); // 10ms
    
    // Press V
    XTestFakeKeyEvent(display, vKey, True, 0);
    XFlush(display);
    usleep(10000); // 10ms
    
    // Release V
    XTestFakeKeyEvent(display, vKey, False, 0);
    XFlush(display);
    usleep(10000); // 10ms
    
    // Release Ctrl
    XTestFakeKeyEvent(display, ctrlKey, False, 0);
    XFlush(display);
}