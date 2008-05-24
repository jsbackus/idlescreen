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
 * Trolltech.  These libraries are released under the GNU
 * General Public License, Version 2.0, as well.  Please contact
 * Trolltech for more information.
 *
 *     http://trolltech.com/
 * 
 */

#ifdef _M_IX86
#include <windows.h>
#else
#include <stream.h>
#endif

#include <stdlib.h>
#include <ctime>

#include <GL/gl.h>
#include <GL/glu.h>

#include <QApplication>

#include "globaldefs.h"
#include "ConfigManager.h"
#include "lens_engine/LensManager.h"

//"globals"
LensManager* _lensManager;
ConfigManager* _configManager;
Background* backgroundObj;
screen_struct _screenObj;
bool bDontSave; // so that the glut demo can specify a different config file to load.

/**
 * Sets up the OpenGL viewport.  Takes as an argument the height and width
 * of the viewport.
 */
void setupViewport(int width, int height)
{
	//set up the viewport
	//window resizing stuff
	glViewport(0, 0, (GLsizei) width, (GLsizei) height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective( 70.0, 1.0, 1.0, -1.0 );

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	gluLookAt(0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
		//camera xyz, the xyz to look at, and the up vector (+y is up)

	//glEnable( GL_DEPTH_TEST );
	
	//background
	glClearColor(0.0, 0.0, 0.0, 0.0); //0.0s is black

	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); 
	//glShadeModel(GL_SMOOTH);
	glShadeModel(GL_FLAT);

	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	glGenTextures(1, &_screenObj._textureIndex);
	glBindTexture(GL_TEXTURE_2D, _screenObj._textureIndex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	//send first off so that from here on out we can use glTexSubImage2D.
	GLubyte* tmp = new GLubyte[_screenObj._tSizeX * _screenObj._tSizeY * 4];
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _screenObj._tSizeX, _screenObj._tSizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, tmp);
	delete [] tmp;
	tmp = NULL;
}

/**
 * Perform any animations
 */
void animate(void)
{
	if(backgroundObj != NULL)
	{
		backgroundObj->clocktick();
	}

	if(_lensManager != NULL && backgroundObj != NULL && backgroundObj->isSetupFinished()) {
		_lensManager->clockTick();
	}
}

/**
 * Redraws the screen.
 */
void render(void)
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode( GL_MODELVIEW );		// Setup model transformations

	glPushMatrix();

	//redraw
	if(backgroundObj != NULL)
	{
		backgroundObj->drawBackground(&_screenObj);
	}

	if(_lensManager != NULL && backgroundObj != NULL && backgroundObj->isSetupFinished()) {
		_lensManager->drawLenses();
	}

	//actually draw image
	glBindTexture(GL_TEXTURE_2D, _screenObj._textureIndex);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _screenObj._sizeX, _screenObj._sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, _screenObj._pixels);
	glTexSubImage2D(GL_TEXTURE_2D, 0, _screenObj._tXOff,_screenObj._tYOff, _screenObj._sizeX, _screenObj._sizeY, GL_RGBA, GL_UNSIGNED_BYTE, _screenObj._pixels);

	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBegin(GL_QUADS);
		glTexCoord2f(_screenObj._left,_screenObj._bottom); glVertex3f(-1.0*VIEWPORT_WIDTH, -1.0*VIEWPORT_HEIGHT, 0.0);
		glTexCoord2f(_screenObj._left,_screenObj._top); glVertex3f(-1.0*VIEWPORT_WIDTH, 1.0*VIEWPORT_HEIGHT, 0.0);
		glTexCoord2f(_screenObj._right,_screenObj._top); glVertex3f(1.0*VIEWPORT_WIDTH, 1.0*VIEWPORT_HEIGHT, 0.0);
		glTexCoord2f(_screenObj._right,_screenObj._bottom); glVertex3f(1.0*VIEWPORT_WIDTH, -1.0*VIEWPORT_HEIGHT, 0.0);
/*
		glTexCoord2f(0.0,0.0); glVertex3f(-1.0*VIEWPORT_WIDTH, -1.0*VIEWPORT_HEIGHT, 0.0);
		glTexCoord2f(0.0,1.0); glVertex3f(-1.0*VIEWPORT_WIDTH, 1.0*VIEWPORT_HEIGHT, 0.0);
		glTexCoord2f(1.0,1.0); glVertex3f(1.0*VIEWPORT_WIDTH, 1.0*VIEWPORT_HEIGHT, 0.0);
		glTexCoord2f(1.0,0.0); glVertex3f(1.0*VIEWPORT_WIDTH, -1.0*VIEWPORT_HEIGHT, 0.0);
*/
	glEnd();
	glDisable(GL_TEXTURE_2D);
	
	glPopMatrix();
	glFlush();
}

/**
 * Contains any exit code that needs executed.  i.e. cleanup, etc.
 */
void exitFunc(void)
{
	if(backgroundObj != NULL)
	{
		delete backgroundObj;
		backgroundObj = NULL;
	}

	if(_configManager != NULL) {
		//save
		if(!bDontSave) {
			_configManager->save();
		}
		delete _configManager;
		_configManager = NULL;
	}

	if(_lensManager != NULL) {
		delete _lensManager;
		_lensManager = NULL;
	}
}

/**
 * Returns the number of milliseconds to set the timer to.
 */
long getTimerMillis() {
	if(_configManager != NULL) {
		return _configManager->getTimerMillis();
	}
	return 50;
}


/**
 * Initializes any global variables that aren't architecture specific.
 * Note: This is called before OpenGL is initialized.  Takes as an
 * argument the name of the config file to load.  If NULL, loads
 * from the default location.
 */
void initFunc(char* filename) {
	bDontSave = false;

	/** NOTE!!! You have to use a QApplication (somewhere) to load plugins! **/
	int argc=0;
	QApplication app(argc, NULL);

	//seed the random number generator.
	srand((unsigned)time( NULL ));

	_configManager = NULL;

	_screenObj._pixels = NULL;
	_screenObj._sizeX = 0;
	_screenObj._sizeY = 0;

	//set up the config manager
	_configManager = new ConfigManager();
	if(_configManager == NULL) {
	  exit(1);
	}
	if(filename == NULL) {
		_configManager->load();
	} else {
		bDontSave = true;
		_configManager->deleteAllSettings();
		if(!_configManager->importFromFile(QString(filename), true)) {
			_configManager->load();
		}
	}
	_configManager->chooseActiveProfile();

	//set up the screen object
	_screenObj._sizeX = _configManager->getScreenWidth();
	_screenObj._sizeY = _configManager->getScreenHeight();
	_screenObj._tSizeX = _configManager->getTextureWidth();
	_screenObj._tSizeY = _configManager->getTextureHeight();
	if(_screenObj._sizeX <= 0 || _screenObj._sizeY <= 0 || _screenObj._tSizeX <= 0 || _screenObj._tSizeY <= 0) {
		exit(1);
	}

	_screenObj._pixels = new GLubyte[_screenObj._sizeX*_screenObj._sizeY*4];
	if(_screenObj._pixels == NULL)
	{
		exit(1);
	}

	_screenObj._left = 0.0;
	_screenObj._bottom = 0.0;
	_screenObj._right = float(_screenObj._sizeX) / float(_screenObj._tSizeX);
	_screenObj._top = float(_screenObj._sizeY) / float(_screenObj._tSizeY);

	_screenObj._tXOff = 0;
	_screenObj._tYOff = 0;

	//set up lens manager
	_lensManager = new LensManager(&_screenObj, _configManager);

	//set up the background
	backgroundObj = _configManager->getNewBackground();
}
