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
 * This class extends QWidget and generates the config dialog box.
 * The config manager must be set with a call to setManager().
 * The app connection to dialogFinished should be connected before
 * setup() is called.  setup() will emit dialogFinished if the
 * manager object is NULL.
 */

#ifndef __CONFIGWIDGET_H__
#define __CONFIGWIDGET_H__

#include <QWidget>
#include <QPushButton>
#include <QListWidget>
#include <QListWidgetItem>
#include <QAbstractItemView>
#include <QSpinBox>
#include <QLabel>
#include <QHash>
#include <QStringList>
#include <QString>
#include <QIcon>

#include "gpl_related/gpldialog.h"
#include "SearchPathDialog.h"
#include "ConfigManager.h"


class ConfigWidget : public QWidget {

	Q_OBJECT

public:
	//Sets up all widgets in the dialog box
	void setup(void);

	//sets the manager object
	void setManager(ConfigManager* manager);

	//shows the GPL dialog
	void showGPLDialog(void);

public slots:
	void aboutClicked(bool checked = false);
	void addClicked(bool checked = false);
	void removeClicked(bool checked = false);
	void upClicked(bool checked = false);
	void downClicked(bool checked = false);
	void okClicked(bool checked = false);
	void applyClicked(bool checked = false);
	void cancelClicked(bool checked = false);
	void searchPathsClicked(bool checked = false);
	void randomClicked(bool checked = false);
	void searchAccepted(void);
	void enableTimerClicked(bool checked = false);
	void previewClicked(bool checked = false);
	void settingsClicked(bool checked = false);

signals:
	void dialogFinished(void);

private:
	//run on initialization.  Convers list of selected screen savers and puts in select list.
	void populateSelectedList();
	void updateListBoxes();
	QListWidgetItem* genListEntry(QString path);
	void storeSelected();

	ConfigManager* _manager;
	QListWidget* _avail;
	QListWidget* _sel;
	QSpinBox* _historyBox;
	QSpinBox* _timerIntervalBox;
	SearchPathDialog* _search;
	QHash<QString, QString> _nameMap;
	GPLDialog* _gplDialog;
};

#endif