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
 * This is project specific code related to the config dialog.
 */

#include <QApplication>

#include "ConfigManager.h"
#include "ConfigWidget.h"

/**
 * Called by win_main to kick off the config dialog.  Do not call this 
 * and initFunc in the same session.
 */
void doConfigDlg(void) {
	//start up the QApp
	int argc = 0;
	QApplication app(argc, NULL);

	//load configuration
	ConfigManager manager;
	manager.load();

	//set up the config widget
	ConfigWidget* configWidget = new ConfigWidget();
	configWidget->setManager(&manager);

	//so that ok/cancel buttons work.
	QObject::connect(configWidget, SIGNAL(dialogFinished()), &app, SLOT(quit()));

	configWidget->setup();

	//make sure that the GPL has been accepted.
	if(!manager.getGPLAccepted()) {
		configWidget->showGPLDialog();
		if(!manager.getGPLAccepted()) {
			return;
		}
	}

	//make the dialog visible
	configWidget->show();

	//run the dialog box
	app.exec();
}
