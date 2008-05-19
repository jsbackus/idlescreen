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
#include <GL/gl.h>
#include <GL/glu.h>

#ifdef _M_IX86
#include <glut.h>
#else
#include <GL/glut.h>
#endif

#include <QString>
#include <QStringList>
#include <QDateTime>
#include <QImage>
#include <QFile>

#include "opengl_main.h"

using namespace std;

void init(void);
void display(void);
void timerFunc( int );
void handle_kb(unsigned char, int, int);
void takeSnapshot(void);
void handleArgs(int, char**);

int	window_id;	// the window id

//Maintains the configuraiton
bool bShowFPS;
QString capturePath;
QString captureExt;
int captureNextNum;
int viewportWidth;
int viewportHeight;
char* configFile;

// used to calculate FPS
long lastTimeStamp;
int frameCount;

int main(int argc, char* argv[])
{
	frameCount = 0;
	lastTimeStamp = time(NULL);
	bShowFPS = true;
	viewportWidth = 1024;
	viewportHeight = 768;
	captureNextNum = 0;
	capturePath = QString("./");
	captureExt = ".BMP";
	configFile = NULL;

	// process command-line arguments and set up path for capture
	handleArgs(argc, argv);

	//set up any data structures
	initFunc(configFile);

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

	return 0;
}

void init(void)
{
	//set up viewport
	setupViewport(viewportWidth,viewportHeight);

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
	switch(key) {
	case 32:	//space bar
		//toggle fps dump
		bShowFPS = !bShowFPS;
		break;
	case 's':
	case 'S':
		takeSnapshot();
		break;
	default:
		exitFunc();
		exit(0);
		//quit
	}
}

/**
 * Attempts to dump the framebuffer into a file.  Automatically
 * generates the filename.
 */
void takeSnapshot(void) {
	if(viewportHeight < 1 || viewportWidth <1)
		return;

	// create data space
	unsigned char* buff = new unsigned char[viewportWidth*viewportHeight*3];
	if(buff == NULL)
		return;

	// grab the data from the framebuffer
	glReadBuffer(GL_FRONT);
	glReadPixels(0, 0, viewportWidth-1, viewportHeight-1, GL_RGB, GL_UNSIGNED_BYTE, buff);

	// stuff into a QImage object
	QImage imageObj(viewportWidth, viewportHeight, QImage::Format_RGB32);
	for(int y=0;y<viewportHeight;y++) {
		int yOffset = (viewportHeight-1-y)*viewportWidth;
		for(int x=0;x<viewportWidth;x++) {
			// pack RGB values into an unsigned int
			// NOTE: this code is architecture dependent!
			int idx = (x+yOffset)*3;
			unsigned int color = 0xFF;
			for(int k=0;k<3;k++) {
				color = color << 8 | (buff[idx+k] & 0xFF);
			}
			// set the pixel in the QImage object
			imageObj.setPixel(x, y, color);
		}
	}

	// save to file
	if(!imageObj.isNull()) {
		QString filename = capturePath+QString::number(captureNextNum++)+captureExt;
		imageObj.save(filename);
	}
}

// process arguments and set globals
void handleArgs(int argc, char** argv) {
	if(argc < 1)
		return;

	for(int i=1; i<argc+1;i++) {
		if(argv[i] != NULL && (argv[i][0] == '-' || argv[i][0] == '/')) {
			switch(argv[i][1]) {
				case 'p':
				case 'P':
					// set snapshot path
					if(i < argc) {
						// if not the last argument, get path and remove all quotes
						capturePath = QString(argv[i+1]).remove('\"').remove('\'');
					}
					break;
				case 'e':
				case 'E':
					// set snapshot extension
					if(i < argc) {
						// if not the last argument, get extension
						captureExt = QString(".") + QString(argv[i+1]);
					}
					break;
				case 'd':
				case 'D':
					// set viewport dimensions
					if(i < argc) {
						// looking for string WWWWxHHHH.  If
						// x not present, dimensions are equal
						QString tmpStr(argv[i+1]);
						QStringList dimList = tmpStr.split("x");
						bool bOk;
						if(dimList.size() > 1) {
							viewportWidth = dimList.at(0).toInt(&bOk);
							if(!bOk)
								viewportWidth = 1024;
							viewportHeight = dimList.at(1).toInt(&bOk);
							if(!bOk)
								viewportHeight = 768;
						} else {
							viewportWidth = dimList.at(0).toInt(&bOk);
							viewportHeight = viewportWidth;
							if(!bOk) {
								viewportWidth = 1024;
								viewportHeight = 768;
							}
						}
					}
					break;
				case 'c':
				case 'C':
					// set config file
					if(i < argc) {
						// if not the last argument, get config file name
						int len = strlen(argv[i+1]);
						configFile = new char[len];
						if(configFile == NULL)
							break;
						strcpy(configFile, argv[i+1]);
					}
					break;
				case 'f':
				case 'F':
					// log FPS
					bShowFPS = true;
					break;
			}
		}
	}
	capturePath = capturePath+QString("/")+QDateTime::currentDateTime().toString("yyyyMMddhhmmss")+QString("_");
}
