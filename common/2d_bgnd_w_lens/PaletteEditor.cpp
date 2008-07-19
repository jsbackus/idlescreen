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

#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QInputDialog>

#include "PaletteEditor.h"

PaletteEditor::PaletteEditor(ConfigManager* confMgr, QWidget* parent, Qt::WindowFlags f) {
	QDialog(parent, f);
	_windowTitle = tr("Palette Editor");
	setWindowTitle(_windowTitle);
	setWindowModality(Qt::WindowModal);
	QIcon icon(":/app_icon.png");
	setWindowIcon(icon);

	_listWidget = NULL;
	_confMgr = confMgr;
	_palDlg = NULL;

	//populate dialog box
	QWidget* tempWidget = NULL;
	QPushButton* tempButton = NULL;
	QHBoxLayout* tempLayout = NULL;

	QVBoxLayout* mainLayout = new QVBoxLayout();

	_listWidget = new PaletteSelectWidget(confMgr);
	mainLayout->addWidget(_listWidget);

	tempLayout = new QHBoxLayout();

	tempLayout->addStretch(0);

	tempButton = new QPushButton(tr("&Close"));
	tempButton->setDefault(true);
	QObject::connect(tempButton, SIGNAL(clicked(bool)), this, SLOT(closeClicked(bool)));
	tempLayout->addWidget(tempButton);
	tempButton = NULL;

	tempLayout->addStretch(0);

	tempWidget = new QWidget();
	tempWidget->setLayout(tempLayout);
	tempLayout = NULL;

	mainLayout->addWidget(tempWidget);
	tempWidget = NULL;

	setLayout(mainLayout);
}
PaletteEditor::~PaletteEditor() {

	//the layout should delete the object.
	_listWidget = NULL;

	/*
	if(_listWidget != NULL) {
		delete _listWidget;
		_listWidget = NULL;
	}
	*/

	if(_palDlg != NULL) {
		delete _palDlg;
		_palDlg = NULL;
	}

	_confMgr = NULL;
}

void PaletteEditor::closeClicked(bool checked) {
	accept();
}
