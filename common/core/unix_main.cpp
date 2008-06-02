/**
 * Copyright (c) 2008 Jeff Backus.
 *
 * Licensed under the GNU General Public License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This code makes heavy use of the Qt libraries from
 * Trolltech.  These libraries are released under the GNU
 * General Public License, Version 2.0, as well.  Please contact
 * Trolltech for more information.
 *
 *     http://trolltech.com/
 * 
 * The majority of the code in this file is based on examples of
 * interface code for the Windows screensaver library by Rachel Grey.
 * More information can be found at
 *
 *     http://www.cityintherain.com/howtoscr.html
 *
 */

/**
 * This file is the main entry point for the screensaver code
 * on UNIX variants.  Based heavily on driver.c by
 * Tugrul Galatali <tugrul@galatali.com>.
 *
 * Revision History:
 * 080529	jsbackus	Initial Revision.
 */

#include <signal.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/Intrinsic.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include <GL/glx.h>

#include "include/vroot.h"

#include "opengl_main.h"
#include "configdlg_main.h"

typedef struct xstuff {
	char *commandLineName;

	Display *display;

	int screen_num;

	Window rootWindow;
	Window window;
	Window existingWindow;

	unsigned int windowWidth, windowHeight;	/* dimensions in pixels */

	GC gc;			/* Graphics context. */

	Colormap colourMap;

	void *hackstuff;
} xstuff_t;

// Globals
xstuff_t *XStuff;

int rootWindow = false; //False;
int frameTime = 0;
int be_nice = 0;
int signalled = 0;

/**
 * Signal Handler.
 */
void signalHandler (int sig) {
	signalled = 1;
}

/**
 * Sets up signal handler.
 */
void setupSigHandler() {
  struct sigaction sa;
  //memset ((void *)&sa, 0, sizeof (struct sigaction));
  //sigaction sa;
  sa.sa_handler = signalHandler;
  sigaction (SIGINT, &sa, 0);
  sigaction (SIGPIPE, &sa, 0);
  sigaction (SIGQUIT, &sa, 0);
  sigaction (SIGTERM, &sa, 0);
}

void createWindow (int argc, char **argv)
{
	XVisualInfo *visualInfo;
	GLXContext context;
	char scr_name[256];
	getProgName(&scr_name[0], 256);

	XStuff->screen_num = DefaultScreen (XStuff->display);
	XStuff->rootWindow = RootWindow (XStuff->display, XStuff->screen_num);

	if (rootWindow || XStuff->existingWindow) {
		XWindowAttributes gwa;
		Visual *visual;
		XVisualInfo templ;
		int outCount;

		XStuff->window = XStuff->existingWindow ? XStuff->existingWindow : XStuff->rootWindow;

		XGetWindowAttributes (XStuff->display, XStuff->window, &gwa);
		visual = gwa.visual;
		XStuff->windowWidth = gwa.width;
		XStuff->windowHeight = gwa.height;

		templ.screen = XStuff->screen_num;
		templ.visualid = XVisualIDFromVisual (visual);

		visualInfo = XGetVisualInfo (XStuff->display, VisualScreenMask | VisualIDMask, &templ, &outCount);

		if (!visualInfo) {
			fprintf (stderr, "%s: can't get GL visual for window 0x%lx.\n", XStuff->commandLineName, (unsigned long)XStuff->window);
			exit (1);
		}
	} else {
		int attributeList[] = {
			GLX_RGBA,
			GLX_RED_SIZE, 1,
			GLX_GREEN_SIZE, 1,
			GLX_BLUE_SIZE, 1,
			GLX_DEPTH_SIZE, 1,
			GLX_DOUBLEBUFFER,
			0
		};
		XSetWindowAttributes swa;
		XSizeHints hints;
		XWMHints wmHints;

		visualInfo = NULL;

		if (!(visualInfo = glXChooseVisual (XStuff->display, XStuff->screen_num, attributeList))) {
			fprintf (stderr, "%s: can't open GL visual.\n", XStuff->commandLineName);
			exit (1);
		}

		swa.colormap = XCreateColormap (XStuff->display, XStuff->rootWindow, visualInfo->visual, AllocNone);
		swa.border_pixel = swa.background_pixel = swa.backing_pixel = BlackPixel (XStuff->display, XStuff->screen_num);
		swa.event_mask = KeyPressMask | StructureNotifyMask;

		XStuff->windowWidth = DisplayWidth (XStuff->display, XStuff->screen_num) / 3;
		XStuff->windowHeight = DisplayHeight (XStuff->display, XStuff->screen_num) / 3;

		XStuff->window =
			XCreateWindow (XStuff->display, XStuff->rootWindow, 0, 0, XStuff->windowWidth, XStuff->windowHeight, 0, visualInfo->depth, InputOutput, visualInfo->visual,
				       CWBorderPixel | CWBackPixel | CWBackingPixel | CWColormap | CWEventMask, &swa);

		hints.flags = USSize;
		hints.width = XStuff->windowWidth;
		hints.height = XStuff->windowHeight;

		wmHints.flags = InputHint;
		wmHints.input = True;

		XmbSetWMProperties (XStuff->display, XStuff->window, scr_name,scr_name, argv, argc, &hints, &wmHints, NULL);
	}

	context = glXCreateContext (XStuff->display, visualInfo, 0, GL_TRUE);
	if (!context) {
		fprintf (stderr, "%s: can't open GLX context.\n", XStuff->commandLineName);
		exit (1);
	}

	if (!glXMakeCurrent (XStuff->display, XStuff->window, context)) {
		fprintf (stderr, "%s: can't set GL context.\n", XStuff->commandLineName);
		exit (1);
	}

	XFree (visualInfo);
	XMapWindow (XStuff->display, XStuff->window);
}

void clearBuffers() {
	int i;
	XEvent event;

	for (i = 0; i < 4; i++) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ACCUM_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glXSwapBuffers (XStuff->display, XStuff->window);

		while (XPending (XStuff->display)) {
			XNextEvent (XStuff->display, &event);
		}
	}
}

void mainLoop (void)
{
  
	XEvent event;
	Atom XA_WM_PROTOCOLS = XInternAtom (XStuff->display, "WM_PROTOCOLS", False);
	Atom XA_WM_DELETE_WINDOW = XInternAtom (XStuff->display, "WM_DELETE_WINDOW", False);
	struct timeval then, now;
	
	// begin tmp
	int bFPS = 0;
	struct timeval fps_time;
	int fps = 0;
	// end tmp

	if (!rootWindow) {
		XSetWMProtocols (XStuff->display, XStuff->window, &XA_WM_DELETE_WINDOW, 1);
	}

	clearBuffers();

	gettimeofday (&now, NULL);
	int frameTimeSoFar = 0;
	while (!signalled) {
	  //begin added by jsb
	  animate();
	  render();
	  glFinish();
	  //end added by jsb
	  //hack_draw (XStuff, (double)now.tv_sec + now.tv_usec / 1000000.0f, frameTimeSoFar / 1000000.0f);

		glXSwapBuffers (XStuff->display, XStuff->window);
		//begin tmp
		if (bFPS) {
			if (fps != -1)
				fps++;

			gettimeofday (&now, NULL);

			if (now.tv_sec > fps_time.tv_sec) {
				if (fps != -1) {
				  printf ("%d fps, %d frameTimeSoFar\n", fps, frameTimeSoFar);
				}

				fps = 0;
				fps_time.tv_sec = now.tv_sec;
				fps_time.tv_usec = now.tv_usec;
			}
		}
	        //end tmp
		while (XPending (XStuff->display)) {
			KeySym keysym;
			char c = 0;

			XNextEvent (XStuff->display, &event);
			switch (event.type) {
			case ConfigureNotify:
				if ((int)XStuff->windowWidth != event.xconfigure.width || (int)XStuff->windowHeight != event.xconfigure.height) {
					XStuff->windowWidth = event.xconfigure.width;
					XStuff->windowHeight = event.xconfigure.height;
					clearBuffers();

					setupViewport(XStuff->windowWidth,
						      XStuff->windowHeight); // added by jsb
				}

				break;
			case KeyPress:
				XLookupString (&event.xkey, &c, 1, &keysym, 0);
				//begin tmp
				if (c == 'f') {
				  bFPS = !bFPS;

					if (bFPS) {
						fps = -1;
						gettimeofday (&fps_time, NULL);
					}
				}
				//end tmp
				if (c == 'q' || c == 'Q' || c == 3 || c == 27)
					return;

				break;
			case ClientMessage:
				if (event.xclient.message_type == XA_WM_PROTOCOLS) {
					if (event.xclient.data.l[0] == (int)XA_WM_DELETE_WINDOW) {
						return;
					}
				}
				break;
			case DestroyNotify:
				return;
			}
		}
		
		then = now;
		gettimeofday (&now, NULL);
		frameTimeSoFar = (now.tv_sec - then.tv_sec) * 1000000 + now.tv_usec - then.tv_usec;

		if (frameTime) {
			while (frameTimeSoFar < frameTime) {
				if (be_nice) {
		

/* nanosleep on Linux/i386 seems completely ineffective for idling for < 20ms */

  /*#ifdef HAVE_NANOSLEEP
					struct timespec hundreth;

					hundreth.tv_sec = 0;
					hundreth.tv_nsec = frameTime - frameTimeSoFar;

					nanosleep(&hundreth, NULL);
#endif
  */
		

					usleep(frameTime - frameTimeSoFar);

					struct timeval tv;

					tv.tv_sec = 0;
					tv.tv_usec = frameTime - frameTimeSoFar;
					select (0, 0, 0, 0, &tv);
				}

				gettimeofday (&now, NULL);
				frameTimeSoFar = (now.tv_sec - then.tv_sec) * 1000000 + now.tv_usec - then.tv_usec;
			}
		} else if (be_nice) {
			struct timeval tv;

			tv.tv_sec = 0;
			tv.tv_usec = 1000;
			select (0, 0, 0, 0, &tv);
		}
		
	}
}

/**
 * Displays the commandline help.
 */
void dispHelp() {
  printf("Usage:\n");
  printf("%s <options>\n", XStuff->commandLineName);
  printf("-c\t\tShow configuration dialog.\n");
  printf("-h\t\tDisplay this help.\n");
  printf("-n\t\tBe nice when it comes to resource use.\n");
  printf("-root\t\tUse (virtual) root window.\n");
  printf("-window-id [id]\tUse the specified window.\n");
  printf("\n");
}

/**
 * Parses the commandline for options not related
 * to window creation.  Recognized options are
 * removed from the list.
 */
void processArgs(int argc, char** argv) {
  if(XStuff != NULL) {
    XStuff->commandLineName = argv[0];
    for (int i = 0; i < argc; i++) {
      if (!strcmp (argv[i], "-window-id")) {
	if ((argv[i + 1][0] == '0') && ((argv[i + 1][1] == 'x') || (argv[i + 1][1] == 'X'))) {
	  XStuff->existingWindow = strtol ((char *)(argv[i + 1] + 2), (char **)NULL, 16);
	} else {
	  XStuff->existingWindow = strtol ((char *)(argv[i + 1]), (char **)NULL, 10);
	}
	
	for (int j = i + 2; j < argc; j++) {
	  argv[j - 2] = argv[j];
	}
	
	argc -= 2;
      } else if(!strcmp(argv[i], "-c")) {
	doConfigDlg();
	exit(0);
      } else if((!strcmp(argv[i], "-root")) || (!strcmp(argv[i], "-r"))) {
	rootWindow = true;
      } else if(!strcmp(argv[i], "-n")) {
	be_nice = true;
      } else if(!strcmp(argv[i], "-h")) {
	dispHelp();
	exit(0);
      }
    }
  }
}

/** 
 * Main entry point.
 */
int main(int argc, char* argv[]) {

    char *display_name = NULL;	/* Server to connect to */
    //int i, j;

	//XStuff = (xstuff_t *) malloc (sizeof (xstuff_t));
	XStuff = new xstuff_t;

	XStuff->existingWindow = 0;
	XStuff->display = NULL;
	XStuff->window = 0;

	processArgs(argc, argv);

	initFunc(); // added by jsb
	// set up the timer
	frameTime = (int)getTimerMillis() * 1000;

	setupSigHandler();

	/*
	 * Connect to the X server. 
	 */
	if (NULL == (XStuff->display = XOpenDisplay (display_name))) {
		fprintf (stderr, "%s: can't connect to X server %s\n", XStuff->commandLineName, XDisplayName (display_name));
		exit (1);
	}

	createWindow (argc, argv);

	setupViewport(XStuff->windowWidth,
		      XStuff->windowHeight); // added by jsb

	//hack_init (XStuff);

	mainLoop ();
	/*
	 * Clean up. 
	 */
	exitFunc();  // added by jsb
	if (XStuff->display) {
		if (XStuff->window) {
			//hack_cleanup (XStuff);

			if (!((rootWindow) || (XStuff->existingWindow)))
				XDestroyWindow (XStuff->display, XStuff->window);
		}

		XCloseDisplay (XStuff->display);
	}
	  
  return 0;
}
