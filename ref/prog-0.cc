#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <stdio.h>

int main()
{
    Display *display;
    Window   window, rootwindow;
    XEvent   event;
    KeySym   escape;

    display = XOpenDisplay(NULL);
    rootwindow = DefaultRootWindow(display);
    window = XCreateWindow(display, rootwindow,
                           -99, -99, 1, 1, /* x, y, width, height */
                           0, 0, InputOnly, /* border, depth, class */
                           CopyFromParent, /* visual */
                           0, NULL); /* valuemask and attributes */
    XSelectInput(display, window, StructureNotifyMask | SubstructureRedirectMask | ResizeRedirectMask | KeyPressMask | KeyReleaseMask);
    XLowerWindow(display, window);
    XMapWindow(display, window);

fprintf(stderr,"DBG0");
    do {
        XNextEvent(display, &event);
    } while (event.type != MapNotify);
fprintf(stderr,"DBG1");

    XGrabKeyboard(display, window, False, GrabModeAsync, GrabModeAsync, CurrentTime);
    XLowerWindow(display, window);

    escape = XKeysymToKeycode(display, XK_Escape);
    printf("\nPress ESC to exit.\n\n");
    fflush(stdout);

    while (1) {

        XNextEvent(display, &event);

        if (event.type == KeyPress) {
            printf("KeyPress: keycode %u state %u\n", event.xkey.keycode, event.xkey.state);
            fflush(stdout);

        } else
        if (event.type == KeyRelease) {

            printf("KeyRelease: keycode %u state %u\n", event.xkey.keycode, event.xkey.state);
            fflush(stdout);

            if (event.xkey.keycode == escape)
                break;
        } else
        if (event.type == UnmapNotify) {

            XUngrabKeyboard(display, CurrentTime);
            XDestroyWindow(display, window);
            XCloseDisplay(display);

            display = XOpenDisplay(NULL);
            rootwindow = DefaultRootWindow(display);
            window = XCreateWindow(display, rootwindow,
                                   -99, -99, 1, 1, /* x, y, width, height */
                                   0, 0, InputOnly, /* border, depth, class */
                                   CopyFromParent, /* visual */
                                   0, NULL); /* valuemask and attributes */

            XSelectInput(display, window, StructureNotifyMask | SubstructureRedirectMask | ResizeRedirectMask | KeyPressMask | KeyReleaseMask);
            XLowerWindow(display, window);
            XMapWindow(display, window);

            do {
                XNextEvent(display, &event);
            } while (event.type != MapNotify);

            XGrabKeyboard(display, window, False, GrabModeAsync, GrabModeAsync, CurrentTime);
            XLowerWindow(display, window);

            escape = XKeysymToKeycode(display, XK_Escape);

        } else {

            printf("Event type %d\n", event.type);
            fflush(stdout);
        }
    }

    XUngrabKeyboard(display, CurrentTime);

    XDestroyWindow(display, window);
    XCloseDisplay(display);
    return 0;
}
