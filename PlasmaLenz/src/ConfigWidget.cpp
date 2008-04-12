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

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QWidget>
#include <QGroupBox>
#include <QIcon>
#include <QLabel>

//temp
#include <QMessageBox>
//end temp

#include "../../common/2d_bgnd_w_lens/ConfigWidget.h"
#include "../../common/2d_bgnd_w_lens/PaletteEditor.h"
#include "ProfileEditDialog.h"
#include "AboutDialog.h"

/*
#include "gui/PaletteEditor.h"
#include "gui/LensEditor.h"
#include "gui/BackgroundEditor.h"
#include "gui/MasterProfileEditDialog.h"
#include "gui/BackgroundProfileEditDialog.h"
*/
//#include "../common/IndexedPaletteDialog.h"

//constructor & destructor
ConfigWidget::ConfigWidget(QWidget* parent, Qt::WindowFlags f) {
	QDialog(parent, f);

	_manager = NULL;
	_gplDialog = NULL;
	_stackedLayout = NULL;
	_mpAvailList = NULL;
	_mpSelList = NULL;

	_windowTitle = tr("PlasmaLenz Configuration");

	setWindowTitle(_windowTitle);

	QIcon icon(":/app_icon.png");
	setWindowIcon(icon);
}

ConfigWidget::~ConfigWidget() {
	_manager = NULL;

	if(_gplDialog != NULL) {
		delete _gplDialog;
		_gplDialog = NULL;
	}

	if(_stackedLayout != NULL) {
		delete _stackedLayout;
		_stackedLayout = NULL;
	}

	if(_mpAvailList != NULL) {
		delete _mpAvailList;
		_mpAvailList = NULL;
	}

	if(_mpSelList != NULL) {
		delete _mpSelList;
		_mpSelList = NULL;
	}

	if(_historyBox != NULL) {
		delete _historyBox;
		_historyBox = NULL;
	}
}

void ConfigWidget::setup(void) {

	//precaution
	if(_manager == NULL) {
		emit dialogFinished();
	}

	_gplDialog = new GPLDialog(_manager->getGPLAccepted());

	QWidget* tempWidget = NULL;
	QPushButton* tempButton = NULL;
	QLabel* tempLabel = NULL;
	QVBoxLayout* mainLayout = new QVBoxLayout();

	//Master Profile selection lists and related.
	tempWidget = genMPWidget();
	mainLayout->addWidget(tempWidget);
	tempWidget = NULL;

	// import/export/palette editor buttons
	QHBoxLayout* midButtonsLayout = new QHBoxLayout();

	tempButton = new QPushButton(tr("Palette Editor"));
	QObject::connect(tempButton, SIGNAL(clicked(bool)), this, SLOT(paletteEditorClicked(bool)));
	midButtonsLayout->addWidget(tempButton);
	tempButton = NULL;

	midButtonsLayout->addStretch(0);

	tempButton = new QPushButton(tr("Import Settings"));
	QObject::connect(tempButton, SIGNAL(clicked(bool)), this, SLOT(importClicked(bool)));
	midButtonsLayout->addWidget(tempButton);
	tempButton = NULL;

	tempButton = new QPushButton(tr("Export Settings"));
	QObject::connect(tempButton, SIGNAL(clicked(bool)), this, SLOT(exportClicked(bool)));
	midButtonsLayout->addWidget(tempButton);
	tempButton = NULL;

	tempWidget = new QWidget();
	tempWidget->setLayout(midButtonsLayout);
	mainLayout->addWidget(tempWidget);
	tempWidget = NULL;

	//About, OK, Apply, and Cancel buttons
	QHBoxLayout* botButtonsLayout = new QHBoxLayout();
	
	tempButton = new QPushButton(tr("A&bout"));
	QObject::connect(tempButton, SIGNAL(clicked(bool)), this, SLOT(aboutClicked(bool)));
	botButtonsLayout->addWidget(tempButton);
	tempButton = NULL;


	botButtonsLayout->addStretch(0);

	tempButton = new QPushButton(tr("&OK"));
	QObject::connect(tempButton, SIGNAL(clicked(bool)), this, SLOT(okClicked(bool)));
	tempButton->setDefault(true);
	botButtonsLayout->addWidget(tempButton);
	tempButton = NULL;

	tempButton = new QPushButton(tr("&Apply"));
	QObject::connect(tempButton, SIGNAL(clicked(bool)), this, SLOT(applyClicked(bool)));
	botButtonsLayout->addWidget(tempButton);
	tempButton = NULL;

	tempButton = new QPushButton(tr("&Cancel"));
	QObject::connect(tempButton, SIGNAL(clicked(bool)), this, SLOT(cancelClicked(bool)));
	botButtonsLayout->addWidget(tempButton);
	tempButton = NULL;

	tempWidget = new QWidget();
	tempWidget->setLayout(botButtonsLayout);
	mainLayout->addWidget(tempWidget);
	tempWidget = NULL;

	setLayout(mainLayout);

	populateMPLists();

}

void ConfigWidget::setManager(ConfigManager* manager) {
	_manager = manager;
}

void ConfigWidget::addListClicked(bool checked) {
	if(_mpAvailList == NULL || _mpSelList == NULL)
		return;

	QList<QListWidgetItem*> itemList = _mpAvailList->selectedItems();
	for(int i =0; i<itemList.size(); i++) {
		if(_mpSelList->findItems(itemList.at(i)->text(),Qt::MatchExactly).empty()) {
			_mpSelList->addItem(itemList.at(i)->text());
		}
	}
}

void ConfigWidget::removeListClicked(bool checked) {
	if(_mpSelList == NULL)
		return;

	QList<QListWidgetItem*> list = _mpSelList->selectedItems();
	for(int i=0;i<list.size(); i++) {
		QListWidgetItem* tmp = _mpSelList->takeItem(_mpSelList->row(list.at(i)));
		delete tmp;
		tmp = NULL;
	}
}

void ConfigWidget::upListClicked(bool checked) {
	if(_mpSelList == NULL)
		return;

	int row = _mpSelList->currentRow();
	if(row > 0) {
		_mpSelList->insertItem(row-1,_mpSelList->takeItem(row));
		_mpSelList->setCurrentRow(row-1);
	}
}

void ConfigWidget::downListClicked(bool checked) {
	if(_mpSelList == NULL)
		return;

	int row = _mpSelList->currentRow();
	if(row < _mpSelList->count()) {
		_mpSelList->insertItem(row+1,_mpSelList->takeItem(row));
		if(row+1 == _mpSelList->count()) {
			_mpSelList->setCurrentRow(_mpSelList->count()-1);
		} else {
			_mpSelList->setCurrentRow(row+1);
		}
	}
}

void ConfigWidget::okClicked(bool checked) {
	saveData();

	emit dialogFinished();
}

void ConfigWidget::applyClicked(bool checked) {
	saveData();
}

void ConfigWidget::cancelClicked(bool checked) {
	emit dialogFinished();
}

void ConfigWidget::randomClicked(bool checked) {
	_manager->setRandomProfile(checked);
}

void ConfigWidget::aboutClicked(bool checked) {
/*
	_about->exec();
*/
	//temporary!
	//QMessageBox::information(this, tr("About"), "Not implemented yet.", QMessageBox::Ok);
	AboutDialog* dlg = new AboutDialog();
	if(dlg != NULL) {
		dlg->exec();
		delete dlg;
		dlg = NULL;
	}
}

//shows the GPL dialog
void ConfigWidget::showGPLDialog(void) {
	_gplDialog->exec();
	_manager->setGPLAccepted(_gplDialog->isAccepted());
	if(_manager->getGPLAccepted()) {
		_manager->save();
	}
}

void ConfigWidget::addMPClicked(bool checked) {
	
	ProfileEditDialog* dlg = new ProfileEditDialog("", _manager);
	QObject::connect(dlg, SIGNAL(profileNameChange(QString, QString)), this, SLOT(profileNameChange(QString, QString)));
	dlg->exec();	
}

void ConfigWidget::editMPClicked(bool checked) {
//	QMessageBox::information(this, _windowTitle, "Not implemented yet.", QMessageBox::Ok);

	if(_mpAvailList == NULL)
		return;

	QList<QListWidgetItem*> list = _mpAvailList->selectedItems();
	for(int i=0;i<list.size(); i++) {
		QListWidgetItem* tmp = _mpAvailList->item(_mpAvailList->row(list.at(i)));
		ProfileEditDialog* dlg = new ProfileEditDialog(tmp->text(), _manager);
		QObject::connect(dlg, SIGNAL(profileNameChange(QString, QString)), this, SLOT(profileNameChange(QString, QString)));
		dlg->exec();

		//clean up
		tmp = NULL;
		delete dlg;
		dlg = NULL;
	}
}

void ConfigWidget::deleteMPClicked(bool checked) {
	if(_mpAvailList == NULL)
		return;

	QList<QListWidgetItem*> list = _mpAvailList->selectedItems();
	for(int i=0;i<list.size(); i++) {
		// get the selected master profile
		QListWidgetItem* tmp = _mpAvailList->takeItem(_mpAvailList->row(list.at(i)));
		QString name = tmp->text();
		delete tmp;
		tmp = NULL;

		//delete the background profile
		MasterProfile tempMp = _manager->getProfile(name);
		_manager->removeBackgroundProfile(tempMp.getBackgroundProfileName());

		//delete the master profile
		_manager->removeProfile(name);

		//now remove the master profile from selected list, if it is in there.
		int j=0;
		bool bDone = false;
		while(!bDone) {
			tmp = _mpSelList->item(j);
			if(tmp->text() == name) {
				bDone = true;
				_mpSelList->takeItem(j);
				delete tmp;
				tmp = NULL;
			}
			
			j++;
			if(j>= _mpSelList->count())
				bDone = true;
		}
	}
}

void ConfigWidget::advancedChecked(bool checked) {
// nada
}

//clears and then populates both MasterProfile lists.
void ConfigWidget::populateMPLists() {
	if(_mpAvailList == NULL || _mpSelList == NULL)
		return;

	//available list
	_mpAvailList->clear();
	_mpAvailList->addItems(_manager->getAvailableProfiles());

	//selected list
	_mpSelList->clear();
	_mpSelList->addItems(_manager->getSelectedProfiles());
}

//generates a widget that contains the available and selected
//master profile lists as well as the requisite buttons.
QWidget* ConfigWidget::genMPWidget() {
	QWidget* tempWidget = NULL;
	QPushButton* tempButton = NULL;

	QVBoxLayout* mpLayout = new QVBoxLayout();

	QHBoxLayout* listsLayout = new QHBoxLayout();

	_mpAvailList = new QListWidget();
	_mpAvailList->setSelectionMode(QAbstractItemView::ExtendedSelection);
	_mpAvailList->setToolTip(tr("This is the list of available master configuration profiles."));

	listsLayout->addWidget(_mpAvailList);

	QVBoxLayout* listButtonLayout = new QVBoxLayout();

	tempButton = new QPushButton(">>");
	tempButton->setToolTip(tr("Click to add the items selected in the list of available master configuration profiles to the list of selected screen savers."));
	QObject::connect(tempButton, SIGNAL(clicked(bool)), this, SLOT(addListClicked(bool)));
	listButtonLayout->addWidget(tempButton);
	tempButton = NULL;

	tempButton = new QPushButton("<<");
	tempButton->setToolTip(tr("Click to remove the selected items in the list of selected master configuration profiles."));
	QObject::connect(tempButton, SIGNAL(clicked(bool)), this, SLOT(removeListClicked(bool)));
	listButtonLayout->addWidget(tempButton);
	tempButton = NULL;
	
	tempButton = new QPushButton(tr("Up"));
	tempButton->setToolTip(tr("Click to move the selected item in the list of selected master configuration profiles up in the order."));
	QObject::connect(tempButton, SIGNAL(clicked(bool)), this, SLOT(upListClicked(bool)));
	listButtonLayout->addWidget(tempButton);
	tempButton = NULL;

	tempButton = new QPushButton(tr("Down"));
	tempButton->setToolTip(tr("Click to move the selected item in the list of selected master configuration profiles down in the order."));
	QObject::connect(tempButton, SIGNAL(clicked(bool)), this, SLOT(downListClicked(bool)));
	listButtonLayout->addWidget(tempButton);
	tempButton = NULL;

	tempWidget = new QWidget();
	tempWidget->setLayout(listButtonLayout);
	listsLayout->addWidget(tempWidget);
	tempWidget = NULL;

	_mpSelList = new QListWidget();
	_mpSelList->setSelectionMode(QAbstractItemView::ExtendedSelection);
	_mpSelList->setToolTip(tr("This is the list of currently selected master configuration profiles."));
	listsLayout->addWidget(_mpSelList);

	tempWidget = new QWidget();
	tempWidget->setLayout(listsLayout);
	mpLayout->addWidget(tempWidget);
	tempWidget = NULL;

	//add/edit/delete buttons, stretch, history box
	QHBoxLayout* bottomButtonLayout = new QHBoxLayout();

	tempButton = new QPushButton(tr("New"));
	tempButton->setToolTip(tr("Click to add a new master configuration profile."));
	QObject::connect(tempButton, SIGNAL(clicked(bool)), this, SLOT(addMPClicked(bool)));
	bottomButtonLayout->addWidget(tempButton);
	tempButton = NULL;

	tempButton = new QPushButton(tr("Edit"));
	tempButton->setToolTip(tr("Click to edit the currently selected master configuration profile."));
	QObject::connect(tempButton, SIGNAL(clicked(bool)), this, SLOT(editMPClicked(bool)));
	bottomButtonLayout->addWidget(tempButton);
	tempButton = NULL;

	tempButton = new QPushButton(tr("Delete"));
	tempButton->setToolTip(tr("Click to remove the currently selected master configuration profile."));
	QObject::connect(tempButton, SIGNAL(clicked(bool)), this, SLOT(deleteMPClicked(bool)));
	bottomButtonLayout->addWidget(tempButton);
	tempButton = NULL;

	bottomButtonLayout->addStretch(0);

	//history box
	QGroupBox* randomBox = new QGroupBox(tr("Choose &Randomly"));
	randomBox->setCheckable(true);
	randomBox->setChecked(_manager->isRandomProfile());
	randomBox->setToolTip(tr("Enabling this will randomize the order in which the master profiles will run."));
	QObject::connect(randomBox, SIGNAL(clicked(bool)), this, SLOT(randomClicked(bool)));

	QHBoxLayout* historyLayout = new QHBoxLayout();
	QLabel* historyLabel = new QLabel(tr("Size of History"));
	historyLabel->setToolTip(tr("The number of times before a master profile can run again."));
	historyLayout->addWidget(historyLabel);

	_historyBox = new QSpinBox();
	_historyBox->setRange(0,10);
	_historyBox->setSingleStep(1);
	_historyBox->setValue(_manager->getProfileHistorySize());
	_historyBox->setToolTip(tr("The number of times before a master profile can run again."));
	historyLayout->addWidget(_historyBox);

	randomBox->setLayout(historyLayout);
	bottomButtonLayout->addWidget(randomBox);

	tempWidget = new QWidget();
	tempWidget->setLayout(bottomButtonLayout);
	mpLayout->addWidget(tempWidget);
	tempWidget = NULL;

	tempWidget = new QWidget();
	tempWidget->setLayout(mpLayout);
	return tempWidget;
}

//saves all data back to the configuration manager
void ConfigWidget::saveData() {
	//update list of selected profiles
	if(_mpSelList != NULL) {
		QStringList retVal;
		for(int i=0;i<_mpSelList->count();i++) {
			QListWidgetItem* tmp = _mpSelList->item(i);
			if(tmp != NULL) {
				retVal.append(tmp->text());
			}
		}
		_manager->setSelectedProfiles(retVal);
	}

	//update history size
	_manager->setProfileHistorySize(_historyBox->value());

	_manager->save();
}
void ConfigWidget::profileNameChange(QString oldName, QString newName) {
	if(_mpAvailList == NULL)
		return;

	if(oldName != newName) {
		//it is an addition.  Append to the available list.
		if(oldName == "") {
			_mpAvailList->addItem(newName);
		} else {
			//otherwise, we're replacing.

			//search the list for the old name, remove it and
			//insert the new one in the same position.
			int j=0;
			bool bDone = false;
			while(!bDone) {
				QListWidgetItem* tmp = _mpAvailList->item(j);
				if(tmp->text() == oldName) {
					bDone = true;
					//remove
					_mpAvailList->takeItem(j);
					delete tmp;
					tmp = NULL;

					//insert
					_mpAvailList->insertItem(j, newName);
				}
			
				j++;
				if(j>= _mpSelList->count())
					bDone = true;
			}
		}
	}
}

void ConfigWidget::importClicked(bool checked) {
	QMessageBox::information(this, _windowTitle, "Not implemented yet.", QMessageBox::Ok);
}

void ConfigWidget::exportClicked(bool checked) {
	QMessageBox::information(this, _windowTitle, "Not implemented yet.", QMessageBox::Ok);
}

void ConfigWidget::paletteEditorClicked(bool checked) {
	PaletteEditor* dlg = new PaletteEditor(_manager, this);
	dlg->exec();

	//clean up
	delete dlg;
	dlg = NULL;
}