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
 * This is the header for the main config dialog widget.  Each
 * project needs to create its own implementation.
 *
 * Revision History:
 * 080214	jsbackus	Initial Revision.
 */

#ifndef __CONFIGWIDGET_H__
#define __CONFIGWIDGET_H__

#include <QWidget>
#include <QStackedLayout>
#include <QListWidget>
#include <QSpinBox>

#include "ConfigManager.h"
#include "../gpl_related/gpldialog.h"

class ConfigWidget : public QWidget {

	Q_OBJECT

public:
	//constructor & destructor
	ConfigWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);
	~ConfigWidget();

	//Sets up all widgets in the dialog box
	void setup(void);

	//sets the manager object
	void setManager(ConfigManager* manager);

	//shows the GPL dialog
	void showGPLDialog(void);

public slots:
	//dialog related
	void okClicked(bool checked = false);
	void applyClicked(bool checked = false);
	void cancelClicked(bool checked = false);
	void aboutClicked(bool checked = false);

	//selected list related
	void addListClicked(bool checked = false);
	void removeListClicked(bool checked = false);
	void upListClicked(bool checked = false);
	void downListClicked(bool checked = false);

	//available list related
	void addMPClicked(bool checked = false);
	void editMPClicked(bool checked = false);
	void deleteMPClicked(bool checked = false);

	void randomClicked(bool checked = false);
	void advancedChecked(bool checked = false);
	void profileNameChange(QString oldName, QString newName);

	void importClicked(bool checked = false);
	void exportClicked(bool checked = false);

signals:
	void dialogFinished(void);

private:

	//generates a widget that contains the available and selected
	//master profile lists as well as the requisite buttons.
	QWidget* genMPWidget();

	//clears and then populates both MasterProfile lists.
	void populateMPLists();

	//saves all data back to the configuration manager
	void saveData();

	ConfigManager* _manager;
	GPLDialog* _gplDialog;

	//widget 0 is simple, widget 1 is advanced!
	QStackedLayout* _stackedLayout;

	//these are the lists of all and selected master profiles
	QListWidget* _mpAvailList;
	QListWidget* _mpSelList;

	QSpinBox* _historyBox;

	QString _windowTitle;
};
#endif