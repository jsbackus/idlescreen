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
 * This file is the main entry point for the screensaver code.
 *
 * Revision History:
 * 080216	jsbackus	Initial Revision.
 */

#include <windows.h>
#include <scrnsave.h>
#include <commctrl.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include "resource.h"
#include "opengl_main.h"
#include "configdlg_main.h"

//define a Windows timer 
#define TIMER 1 
static void InitGL(HWND hWnd, HDC & hDC, HGLRC & hRC);
static void CloseGL(HWND hWnd, HDC hDC, HGLRC hRC);
BOOL WINAPI ScreenSaverConfigureDialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
BOOL WINAPI RegisterDialogClasses(HANDLE hInst);
void timerFunc(HDC & hDC);

// Screen Saver Procedure
LRESULT WINAPI ScreenSaverProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  static HDC hDC;
  static HGLRC hRC;
  static RECT rect;

  switch ( message ) {

  case WM_CREATE: 
    // get window dimensions
    GetClientRect( hWnd, &rect );

	//initialize all necessary data structures
	initFunc();

    //set up OpenGL
    InitGL( hWnd, hDC, hRC );

    //Initialize perspective, viewpoint, and
    //any objects you wish to animate
	setupViewport(rect.right, rect.bottom);

    //create a timer that ticks every so many milliseconds
    SetTimer( hWnd, TIMER, getTimerMillis(), NULL ); 
	//SetTimer( hWnd, TIMER, 50, NULL ); 
    return 0;
 
  case WM_DESTROY:
    KillTimer( hWnd, TIMER );
    
    //delete any objects created during animation
    //and close down OpenGL nicely
	exitFunc();

    CloseGL( hWnd, hDC, hRC );
    return 0;

  case WM_TIMER:
    //call some function to advance your animation		
	timerFunc(hDC);
    return 0;				

  }

  //let the screensaver library take care of any
  //other messages

  return DefScreenSaverProc(hWnd, message, wParam, lParam );
}

/**
 * Begin OpenGL specific functions.
 */
static void InitGL(HWND hWnd, HDC & hDC, HGLRC & hRC)
{
      
	PIXELFORMATDESCRIPTOR pfd;
    ZeroMemory( &pfd, sizeof pfd );
    pfd.nSize = sizeof pfd;
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
      
    hDC = GetDC( hWnd );
      
    int i = ChoosePixelFormat( hDC, &pfd );  
    SetPixelFormat( hDC, i, &pfd );

    hRC = wglCreateContext( hDC );
    wglMakeCurrent( hDC, hRC );

}
  
static void CloseGL(HWND hWnd, HDC hDC, HGLRC hRC)
{
	wglMakeCurrent( NULL, NULL );
    wglDeleteContext( hRC );
    ReleaseDC( hWnd, hDC );
}
/**
 * End OpenGL specific functions.
 */

/**
 * Configuration Dialog Entry Point
 */
BOOL WINAPI ScreenSaverConfigureDialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{	
  switch ( message ) 
  {

	case WM_INITDIALOG:

		doConfigDlg();

		 //necessary to give control back to the calling window.
		EndDialog(hDlg, 0);

		return TRUE;

  }	//end command switch

  return FALSE; 
}

// needed for SCRNSAVE.LIB
BOOL WINAPI RegisterDialogClasses(HANDLE hInst)
{
  return TRUE;
}

/**
 * Called whenever the timer goes off.
 */
void timerFunc(HDC & hDC)
{
	animate();
	render();
	glFinish();
	SwapBuffers(hDC);		//swap buffers
}

