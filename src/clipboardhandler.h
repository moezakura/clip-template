#ifndef CLIPBOARDHANDLER_H
#define CLIPBOARDHANDLER_H

#include <string>

// Forward declarations for X11 types
typedef unsigned long XID;
typedef XID Window;
struct _XDisplay;
typedef struct _XDisplay Display;

class ClipboardHandler {
public:
    ClipboardHandler();
    ~ClipboardHandler();
    
    void copyToClipboard(const std::string &text);
    void pasteToWindow(Window window);
    
private:
    void sendCtrlV(Display *display, Window window);
    void setFocusToWindow(Display *display, Window window);
};

#endif // CLIPBOARDHANDLER_H