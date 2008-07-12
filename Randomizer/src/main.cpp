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

#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMainWindow>
#include <QStringList>
#include <QString>

#include <process.h>
using namespace std;

#include "resource.h"

#include "ConfigManager.h"
#include "ConfigWidget.h"
#include "SaverRunner.h"

int openConfigDialog(ConfigManager* manager);
int runScr(ConfigManager* manager, QStringList);

int main(int argc, char** argv) {

	ConfigManager manager;
	manager.load();

	QStringList args;
	QString temp;

	for(int i=0;i<argc;i++) {
		temp = argv[i];
		args << temp;
	}

	//set the program name to be the first item in the arglist
	manager.setProgramName(args.at(0));

	//remove the program name from the list of arguments
	args.removeFirst();

	//look at argv[1] to see what we should do

	//fullscreen?
	temp = QString("/s");
	if(args.count() > 0 && args.at(0).compare(temp, Qt::CaseInsensitive) == 0) {
		return runScr(&manager, args);
	} 

	//thumbnail?
	temp = QString("/p");
	if(args.count() > 1 && args.at(0).compare(temp, Qt::CaseInsensitive) == 0) {
		return runScr(&manager, args);
	}

	//do config
	return openConfigDialog(&manager);
}

int openConfigDialog(ConfigManager* manager) {
	int argc = 0;

	QApplication app(argc, NULL);

	//load configuration
	ConfigWidget* configWidget = new ConfigWidget();
	configWidget->setManager(manager);

	//so that ok/cancel buttons work.
	QObject::connect(configWidget, SIGNAL(dialogFinished()), &app, SLOT(quit()));

	configWidget->setup();

	//make sure that the GPL has been accepted.
	if(!manager->getGPLAccepted()) {
		configWidget->showGPLDialog();
		if(!manager->getGPLAccepted()) {
			return 1;
		}
	}
	configWidget->show();

	return app.exec();
}

int runScr(ConfigManager* manager, QStringList args) {
	//make sure that the GPL has been accepted.
	if(!manager->getGPLAccepted()) {
		return 0;
	}

	int argc = 0;

	QApplication app(argc, NULL);

	/*
	//blank screen behind.  Does not work for dual-screen.
	QGraphicsScene* scene = new QGraphicsScene();;
	scene->setBackgroundBrush(Qt::black);
	QGraphicsView* view = new QGraphicsView(scene);
	QMainWindow* mainWindow = new QMainWindow(NULL, Qt::FramelessWindowHint);
	mainWindow->setCentralWidget(view);
	mainWindow->showMaximized();
	*/

	//start screensaver
	SaverRunner* runner = new SaverRunner();
	runner->startSCR(manager, args);
	QObject::connect(runner, SIGNAL(done()), &app, SLOT(quit()));

	//start thread so that we can use timers & QProcess
	runner->start();

	//start the app!
	return app.exec();
}
