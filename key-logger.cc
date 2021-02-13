#include <X11/Xlib.h> // Every Xlib program must include this
#include <assert.h>   // I include this to test return values the lazy way
#include <unistd.h>   // So we got the profile for 10 seconds
#include <stdio.h>
#include <X11/keysym.h>

#define NIL (0)       // A name for the void pointer

main()
{
      // Open the display

      Display *dpy = XOpenDisplay(NIL);
      assert(dpy);

      // Get some colors

      int blackColor = BlackPixel(dpy, DefaultScreen(dpy));
      int whiteColor = WhitePixel(dpy, DefaultScreen(dpy));

      // Create the window

      Window w = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy), 0, 0, 
				     200, 100, 0, blackColor, blackColor);

      // We want to get MapNotify events

      XSelectInput(dpy, w, StructureNotifyMask);

      // "Map" the window (that is, make it appear on the screen)

      XMapWindow(dpy, w);

      // Create a "Graphics Context"

      GC gc = XCreateGC(dpy, w, 0, NIL);

      // Tell the GC we draw using the white color

      XSetForeground(dpy, gc, whiteColor);

      // Wait for the MapNotify event
      XEvent event;
      for(;;) {
	    XNextEvent(dpy, &event);
	    if (event.type == MapNotify)
		  break;
      }

      // Draw the line
      
      XDrawLine(dpy, w, gc, 10, 60, 180, 20);

      // Send the "DrawLine" request to the server

      XFlush(dpy);

      // Wait for 10 seconds

      sleep(10);

    XGrabKeyboard(dpy, w, False, GrabModeAsync, GrabModeAsync, CurrentTime);
    XLowerWindow(dpy, w);

    KeySym   escape;
    escape = XKeysymToKeycode(dpy, XK_Escape);
    printf("\nPress ESC to exit.\n\n");
    fflush(stdout);
    while (1) {

        XNextEvent(dpy, &event);

        if (event.type == KeyPress) {
            printf("KeyPress: keycode %u state %u\n", event.xkey.keycode, event.xkey.state);
            fflush(stdout);

        } else if (event.type == KeyRelease) {

            printf("KeyRelease: keycode %u state %u\n", event.xkey.keycode, event.xkey.state);
            fflush(stdout);

            if (event.xkey.keycode == escape){
                break;
            }

        } else if (event.type == UnmapNotify) {

            XUngrabKeyboard(dpy, CurrentTime);
            XDestroyWindow(dpy, w);
            XCloseDisplay(dpy);

            dpy = XOpenDisplay(NULL);
            w = XCreateWindow(dpy, DefaultRootWindow(dpy),
                                   -99, -99, 1, 1, /* x, y, width, height */
                                   0, 0, InputOnly, /* border, depth, class */
                                   CopyFromParent, /* visual */
                                   0, NULL); /* valuemask and attributes */

            XSelectInput(dpy, w, StructureNotifyMask | SubstructureRedirectMask | ResizeRedirectMask | KeyPressMask | KeyReleaseMask);
            XLowerWindow(dpy, w);
            XMapWindow(dpy, w);

            do {
                XNextEvent(dpy, &event);
            } while (event.type != MapNotify);

            XGrabKeyboard(dpy, w, False, GrabModeAsync, GrabModeAsync, CurrentTime);
            XLowerWindow(dpy, w);

            escape = XKeysymToKeycode(dpy, XK_Escape);

        } else {

            printf("Event type %d\n", event.type);
            fflush(stdout);
        }
    }

    XUngrabKeyboard(dpy, CurrentTime);

    XDestroyWindow(dpy, w);
    XCloseDisplay(dpy);
    return 0;
}








