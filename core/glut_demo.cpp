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
 * Additionally, this code makes heavy use of the Qt libraries from
 * Trolltech and the OpenGL Utility Toolkit by Mark Kilgard.
 * The Qt libraries are released under the GNU General Public License,
 * Version 2.0.  Please contact Trolltech for more information at
 *
 *     http://trolltech.com/
 * 
 * The OpenGL Utility Toolkit(GLUT) is not open source and therefore
 * compiled versions of this code are not distributed.  For more
 * information regarding the GLUT, please refer to the GLUT homepage at
 *
 * http://www.opengl.org/resources/libraries/glut/
 *
 */

/**
 * This is an alternative version which utilizes the GLUT to
 * render the screensaver.  It is allows the user to use
 * various keyboard commands to make screen captures,
 * adjust the animation rate, etc.
 *
 * Revision History:
 * 080216	jsbackus	Initial Revision.
 */

#ifdef _M_IX86
#include <windows.h>
#else
#include <stream.h>
#endif

#include <iostream>
#include <ctime>
#include <gl/gl.h>
#include <gl/glu.h>
#include <glut.h>

#include "opengl_main.h"

using namespace std;

void init(void);
void display(void);
void timerFunc( int );
void handle_kb(unsigned char, int, int);

int	window_id;	// the window id

//Maintains the configuraiton
bool bShowFPS;
long lastTimeStamp;
int frameCount;
void main(int argc, char* argv[])
{
	//load configuration
	//confMgr = ConfigManager().getConfigManager();
	//set up any data structures
	initFunc();
	frameCount = 0;
	lastTimeStamp = time(NULL);
	bShowFPS = true;

	//initialize & set up glut
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB );
	window_id = glutCreateWindow( "Plasma Lens GLUT Demo - Jeff Backus" );
	//glutReshapeWindow(600,400);		//default window size
	glutFullScreen();

	glutDisplayFunc( display );	// Setup GLUT callbacks
	glutKeyboardFunc( handle_kb );
	//set up timer for hole animation
	glutTimerFunc( (unsigned int)getTimerMillis(), &timerFunc, 1 );

	//setup openGL
	init();

	//throw down on some open gl
	glutMainLoop();
}

void init(void)
{
	//set up viewport
	setupViewport(1280,1024);

}

void display(void)
{
	render();
	glutSwapBuffers();			// Display back buffer
	if(bShowFPS)
	{
		frameCount++;
		long curTime = time(NULL);
		if(curTime - lastTimeStamp > 0)
		{
			cout<<"FPS: "<<frameCount/(curTime - lastTimeStamp)<<endl;
			frameCount = 0;
			lastTimeStamp = curTime;
		}
	}
}

void timerFunc( int value)
{
	animate();
	glutTimerFunc( (unsigned int)getTimerMillis(), &timerFunc, 1 );
	glutPostRedisplay();
}

void handle_kb(unsigned char key, int x, int y)
{
	switch(key)
	{
	case 32:	//space bar
		//toggle fps dump
		bShowFPS = !bShowFPS;
		break;
	default:
		exitFunc();
		exit(0);
		//quit
	}
}

