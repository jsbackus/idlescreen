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

//begin debug
//#include <iostream>
//using namespace std;
//#include "2d_bgnd_w_lens/lens_engine/SphericalLensProfile.h"
//end debug

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QWidget>
#include <QGroupBox>
#include <QIcon>
#include <QLabel>
#include <QFileDialog>
#include <QDir>

//temp
#include <QMessageBox>
//end temp

#include "../../common/2d_bgnd_w_lens/ConfigWidget.h"
#include "../../common/2d_bgnd_w_lens/PaletteEditor.h"
#include "../../common/utility/ImportExportDialog.h"
#include "../../common/utility/misc_funcs.h"
#include "../../common/utility/AboutDialog.h"
#include "../../common/utility/HelpDialog.h"
#include "../../common/2d_bgnd_w_lens/project_specific_extern_defs.h"
#include "ProfileEditDialog.h"

//constructor & destructor
ConfigWidget::ConfigWidget(QWidget* parent, Qt::WindowFlags f) {
	QDialog(parent, f);

	_manager = NULL;
	_gplDialog = NULL;
	_stackedLayout = NULL;
	_mpAvailList = NULL;
	_mpSelList = NULL;

	_windowTitle = tr("Crawlies Configuration");

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
	tempButton->setToolTip(tr("Opens the palette editor."));
	QObject::connect(tempButton, SIGNAL(clicked(bool)), this, SLOT(paletteEditorClicked(bool)));
	midButtonsLayout->addWidget(tempButton);
	tempButton = NULL;

	midButtonsLayout->addStretch(0);

	tempButton = new QPushButton(tr("Import Settings"));
	tempButton->setToolTip(tr("Imports settings from a file."));
	QObject::connect(tempButton, SIGNAL(clicked(bool)), this, SLOT(importClicked(bool)));
	midButtonsLayout->addWidget(tempButton);
	tempButton = NULL;

	tempButton = new QPushButton(tr("Export Settings"));
	tempButton->setToolTip(tr("Exports settings to a file."));
	QObject::connect(tempButton, SIGNAL(clicked(bool)), this, SLOT(exportClicked(bool)));
	midButtonsLayout->addWidget(tempButton);
	tempButton = NULL;

	tempWidget = new QWidget();
	tempWidget->setLayout(midButtonsLayout);
	mainLayout->addWidget(tempWidget);
	tempWidget = NULL;

	//About, Help, OK, Apply, and Cancel buttons
	QHBoxLayout* botButtonsLayout = new QHBoxLayout();
	
	tempButton = new QPushButton(tr("A&bout"));
	QObject::connect(tempButton, SIGNAL(clicked(bool)), this, SLOT(aboutClicked(bool)));
	botButtonsLayout->addWidget(tempButton);
	tempButton = NULL;

	tempButton = new QPushButton(tr("&Help"));
	tempButton->setDefault(false);
	tempButton->setAutoDefault(false);
	QObject::connect(tempButton, SIGNAL(clicked(bool)), this, SLOT(helpClicked(bool)));
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
	AboutDialog* dlg = new AboutDialog(getAppFullName(), getAppVersion());
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
	QMessageBox::information(this, _windowTitle, "Not implemented yet.", QMessageBox::Ok);
	/*	
	ProfileEditDialog* dlg = new ProfileEditDialog("", _manager);
	QObject::connect(dlg, SIGNAL(profileNameChange(QString, QString)), this, SLOT(profileNameChange(QString, QString)));
	dlg->exec();	
	*/
}

void ConfigWidget::editMPClicked(bool checked) {
	QMessageBox::information(this, _windowTitle, "Not implemented yet.", QMessageBox::Ok);
	/*
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
	*/
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
		while(!bDone && j<_mpSelList->count()) {
			tmp = _mpSelList->item(j);
			if(tmp->text() == name) {
				bDone = true;
				_mpSelList->takeItem(j);
				delete tmp;
				tmp = NULL;
			}
			
			j++;
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
	_mpAvailList->setToolTip(tr("This is the list of available configuration profiles."));

	listsLayout->addWidget(_mpAvailList);

	QVBoxLayout* listButtonLayout = new QVBoxLayout();

	tempButton = new QPushButton(">>");
	tempButton->setToolTip(tr("Click to add the items selected in the list of available configuration profiles")+"\n"
							+tr("to the list of selected configuration profiles."));
	QObject::connect(tempButton, SIGNAL(clicked(bool)), this, SLOT(addListClicked(bool)));
	listButtonLayout->addWidget(tempButton);
	tempButton = NULL;

	tempButton = new QPushButton("<<");
	tempButton->setToolTip(tr("Click to remove the selected items in the list of selected configuration profiles."));
	QObject::connect(tempButton, SIGNAL(clicked(bool)), this, SLOT(removeListClicked(bool)));
	listButtonLayout->addWidget(tempButton);
	tempButton = NULL;
	
	tempButton = new QPushButton(tr("Up"));
	tempButton->setToolTip(tr("Click to move the selected item in the list of selected configuration profiles up in")+"\n"
							+tr("the order."));
	QObject::connect(tempButton, SIGNAL(clicked(bool)), this, SLOT(upListClicked(bool)));
	listButtonLayout->addWidget(tempButton);
	tempButton = NULL;

	tempButton = new QPushButton(tr("Down"));
	tempButton->setToolTip(tr("Click to move the selected item in the list of selected configuration profiles")+"\n"
							+tr("down in the order."));
	QObject::connect(tempButton, SIGNAL(clicked(bool)), this, SLOT(downListClicked(bool)));
	listButtonLayout->addWidget(tempButton);
	tempButton = NULL;

	tempWidget = new QWidget();
	tempWidget->setLayout(listButtonLayout);
	listsLayout->addWidget(tempWidget);
	tempWidget = NULL;

	_mpSelList = new QListWidget();
	_mpSelList->setSelectionMode(QAbstractItemView::ExtendedSelection);
	_mpSelList->setToolTip(tr("This is the list of currently selected configuration profiles."));
	listsLayout->addWidget(_mpSelList);

	tempWidget = new QWidget();
	tempWidget->setLayout(listsLayout);
	mpLayout->addWidget(tempWidget);
	tempWidget = NULL;

	//add/edit/delete buttons, stretch, history box
	QHBoxLayout* bottomButtonLayout = new QHBoxLayout();

	tempButton = new QPushButton(tr("New"));
	tempButton->setToolTip(tr("Click to add a new configuration profile."));
	QObject::connect(tempButton, SIGNAL(clicked(bool)), this, SLOT(addMPClicked(bool)));
	bottomButtonLayout->addWidget(tempButton);
	tempButton = NULL;

	tempButton = new QPushButton(tr("Edit"));
	tempButton->setToolTip(tr("Click to edit the currently selected configuration profile."));
	QObject::connect(tempButton, SIGNAL(clicked(bool)), this, SLOT(editMPClicked(bool)));
	bottomButtonLayout->addWidget(tempButton);
	tempButton = NULL;

	tempButton = new QPushButton(tr("Delete"));
	tempButton->setToolTip(tr("Click to permanently remove the currently selected configuration profile."));
	QObject::connect(tempButton, SIGNAL(clicked(bool)), this, SLOT(deleteMPClicked(bool)));
	bottomButtonLayout->addWidget(tempButton);
	tempButton = NULL;

	bottomButtonLayout->addStretch(0);

	//history box
	QGroupBox* randomBox = new QGroupBox(tr("Choose &Randomly"));
	randomBox->setCheckable(true);
	randomBox->setChecked(_manager->isRandomProfile());
	randomBox->setToolTip(tr("Enabling this will randomize the order in which the configuration profiles will run."));
	QObject::connect(randomBox, SIGNAL(clicked(bool)), this, SLOT(randomClicked(bool)));

	QHBoxLayout* historyLayout = new QHBoxLayout();
	QLabel* historyLabel = new QLabel(tr("Size of History"));
	historyLabel->setToolTip(tr("The number of times before a configuration profile can run again."));
	historyLayout->addWidget(historyLabel);

	_historyBox = new QSpinBox();
	_historyBox->setRange(0,10);
	_historyBox->setSingleStep(1);
	_historyBox->setValue(_manager->getProfileHistorySize());
	_historyBox->setToolTip(tr("The number of times before a configuration profile can run again."));
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
			while(!bDone && j < _mpAvailList->count()) { 
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
			}
			// repeat on the list of selected items
			j=0;
			bDone = false;
			while(!bDone &&j<_mpSelList->count()) {
				QListWidgetItem* tmp = _mpSelList->item(j);
				if(tmp->text() == oldName) {
					bDone = true;
					//remove
					_mpSelList->takeItem(j);
					delete tmp;
					tmp = NULL;

					//insert
					_mpSelList->insertItem(j, newName);
				}
				j++;
			}
		}
	}
}

void ConfigWidget::importClicked(bool checked) {
	QMessageBox::information(this, _windowTitle, "Not implemented yet.", QMessageBox::Ok);
	/*
	ImportExportDialog dlg(true);

	// get file name to import from
	QString filename = QFileDialog::getOpenFileName(this, tr("Import Settings From File"), QDir::homePath(), 
			tr("Settings Files (*.xml)"));

	if(filename.isEmpty())
		return;
	
	// create a temporary config manager and load
	// it with data from the import file.
	ConfigManager* tmpMgr = new ConfigManager();
	if(tmpMgr == NULL)
		return;
	tmpMgr->importFromFile(filename, false);

	// populate dialog box
	QStringList bkgnds = tmpMgr->getAvailableProfiles();
	dlg.addSubItems("Configuration Profiles", bkgnds, true);

	QStringList palettes = tmpMgr->getPaletteNames();
	dlg.addSubItems("Palettes", palettes, true);

	// show dialog and get lists of checked items
	if(dlg.exec() == QDialog::Rejected) {
		delete tmpMgr;
		tmpMgr = NULL;
		return;
	}
	bkgnds = dlg.getSubItems("Configuration Profiles", true);
	palettes = dlg.getSubItems("Palettes", true);

	// first, add in the palettes, since they don't
	// have any dependencies
	for(int i = 0; i<palettes.size(); i++) {
		IndexedPaletteProfile tmpPal = tmpMgr->getPaletteProfile(palettes.at(i));
		_manager->addPalette(tmpPal);
	}

	// now go through the MasterProfiles, adding sub profiles
	// it requires (including IndexedPaletteProfiles).
	for(int i = 0; i<bkgnds.size(); i++) {
		MasterProfile tmpMp = tmpMgr->getProfile(bkgnds.at(i));
		// make sure the master profile doesn't exist, so that we don't
		// unnecessarily the other things.  No need to check everything else
		// since the respective add routines will check before appending
		// to the lists.
		if(!_manager->doesProfileExist(tmpMp.getName())) {
		  LensProfile* tmpLens = tmpMgr->getLensProfile(tmpMp.getLensProfileName());
			_manager->addLensProfile(tmpLens);
			//debug?
			tmpLens = NULL;
			//end debug
			PlasmaFractalBackgroundProfile* tmpBkgnd = (PlasmaFractalBackgroundProfile*)tmpMgr->getBackgroundProfile(tmpMp.getBackgroundProfileName());
			IndexedPaletteProfile tmpPal = tmpMgr->getPaletteProfile(tmpBkgnd->getPaletteName());
			_manager->addPalette(tmpPal);
			_manager->addBackgroundProfile(*tmpBkgnd);
			_manager->addProfile(tmpMp);
			_mpAvailList->addItem(tmpMp.getName());
		}
	}
	*/
}

void ConfigWidget::exportClicked(bool checked) {
	QMessageBox::information(this, _windowTitle, "Not implemented yet.", QMessageBox::Ok);

	/*
	ImportExportDialog dlg(false);

	// populate dialog box
	QStringList bkgnds = _manager->getAvailableProfiles();
	dlg.addSubItems("Configuration Profiles", bkgnds, true);

	QStringList palettes = _manager->getPaletteNames();
	dlg.addSubItems("Palettes", palettes, true);

	// show dialog and get lists of checked items
	if(dlg.exec() == 0)
		return;
	bkgnds = dlg.getSubItems("Configuration Profiles", true);
	palettes = dlg.getSubItems("Palettes", true);

	// get file name to export to
	QString filename = QFileDialog::getSaveFileName(this, tr("Export Settings to File"), QDir::homePath(), 
			tr("Settings Files (*.xml)"));

	if(filename.isEmpty())
		return;
	
	// now create a temporary config manager that
	// contains only the profiles that we want.  Be sure to grab
	// all miscellaneous profiles that the MasterProfile
	// (including the IndexedPaletteProfiles).
	ConfigManager* tmpMgr = new ConfigManager();
	if(tmpMgr == NULL)
		return;

	// first, add in the palettes, since they don't
	// have any dependencies
	for(int i = 0; i<palettes.size(); i++) {
		IndexedPaletteProfile tmpPal = _manager->getPaletteProfile(palettes.at(i));
		tmpMgr->addPalette(tmpPal);
	}

	// now go through the MasterProfiles, adding sub profiles
	// it requires (including IndexedPaletteProfiles).
	for(int i = 0; i<bkgnds.size(); i++) {
		MasterProfile tmpMp = _manager->getProfile(bkgnds.at(i));
		LensProfile* tmpLens = _manager->getLensProfile(tmpMp.getLensProfileName());
		tmpMgr->addLensProfile(tmpLens);
		//debug?
		tmpLens = NULL;
		//debug?
		PlasmaFractalBackgroundProfile* tmpBkgnd = (PlasmaFractalBackgroundProfile*)_manager->getBackgroundProfile(tmpMp.getBackgroundProfileName());
		IndexedPaletteProfile tmpPal = _manager->getPaletteProfile(tmpBkgnd->getPaletteName());
		tmpMgr->addPalette(tmpPal);
		tmpMgr->addBackgroundProfile(*tmpBkgnd);
		tmpMgr->addProfile(tmpMp);
	}

	// save!
	tmpMgr->exportToFile(filename, false);
	delete tmpMgr;
	tmpMgr = NULL;
	*/
}

void ConfigWidget::paletteEditorClicked(bool checked) {
	PaletteEditor* dlg = new PaletteEditor(_manager, this);
	dlg->exec();

	//clean up
	delete dlg;
	dlg = NULL;
}
void ConfigWidget::helpClicked(bool checked) {
	HelpDialog* dlg = new HelpDialog("qrc:/help/index.html");
	if(dlg != NULL) {
		dlg->exec();
		delete dlg;
		dlg = NULL;
	}
}
