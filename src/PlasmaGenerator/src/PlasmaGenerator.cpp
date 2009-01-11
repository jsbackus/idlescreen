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

/**
 * This is the main entry point for the PlasmaGenerator.
 */

#include <QApplication>

#include "ConfigManager.h"
#include "PlasmaGeneratorDialog.h"

int main(int argv, char** argc) {

	//start up the QApp
	QApplication app(argc, argc);

	//load configuration
	ConfigManager manager;
	manager.load();

	//set up the dialog
	PlasmaGeneratorDialog* dlg = new PlasmaGeneratorDialog();
	if(dlg == NULL) {
	  exit(1);
	}

	dlg->setManager(&manager);
	
	//so that ok/cancel buttons work.
	QObject::connect(dlg, SIGNAL(dialogFinished()), &app, SLOT(quit()));

	dlg->setup();

	//make sure that the GPL has been accepted.
	if(!manager.getGPLAccepted()) {
		dlg->showGPLDialog();
		if(!manager.getGPLAccepted()) {
			return;
		}
	}

	//make the dialog visible
	dlg->show();

	//run the dialog box
	app.exec();
}
