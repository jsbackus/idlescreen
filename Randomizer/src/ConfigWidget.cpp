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

//#include <QMessageBox>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QWidget>
#include <QGroupBox>
#include <QDir>
#include <QIcon>
#include <QProcess>

#include <windows.h>
#include <string>
#include <iostream>
using namespace std;

#include "../../common/utility/misc_funcs.h"
#include "../../common/utility/AboutDialog.h"
#include "globaldefs.h"
#include "ConfigWidget.h"

void ConfigWidget::setup(void) {

	//precaution
	if(_manager == NULL) {
		emit dialogFinished();
	}

	_gplDialog = new GPLDialog(_manager->getGPLAccepted());

	setWindowTitle(tr("Screen Saver Randomizer Configuration"));

	QIcon icon(":/app_icon.png");
	setWindowIcon(icon);

	//dialog boxes
	_search = new SearchPathDialog(this);
	_search->setPathList(_manager->getSearchPathList());
	QObject::connect(_search, SIGNAL(accepted()), this, SLOT(searchAccepted()));

	QWidget* tempWidget = NULL;
	QPushButton* tempButton = NULL;

	QVBoxLayout* mainLayout = new QVBoxLayout();

	// above the lists section
	QHBoxLayout* aboveListsLayout = new QHBoxLayout();

	tempButton = new QPushButton(tr("&Search Paths..."));
	tempButton->setToolTip(tr("This button opens the dialog for setting the paths to search."));
	QObject::connect(tempButton, SIGNAL(clicked(bool)), this, SLOT(searchPathsClicked(bool)));
	aboveListsLayout->addWidget(tempButton);
	tempButton = NULL;

	aboveListsLayout->addStretch(0);

	QCheckBox* tmpChkBox = new QCheckBox(tr("Hide Desktop"));
	tmpChkBox->setChecked(_manager->getBlankDesktop());
	tmpChkBox->setToolTip(tr("When enabled, a black window is opened to hide the contents of the")+"\n"
							+tr("desktop between screensaver changes."));
	QObject::connect(tmpChkBox, SIGNAL(clicked(bool)), this, SLOT(hideClicked(bool)));
	aboveListsLayout->addWidget(tmpChkBox);
	tmpChkBox = NULL;

	QVBoxLayout* timerLayout = new QVBoxLayout();
	
	QGroupBox* timerBox = new QGroupBox(tr("Enable Timer"));
	timerBox->setCheckable(true);
	timerBox->setChecked(_manager->useTimer());
	timerBox->setToolTip(tr("Enabling this will change screen savers according to the timer interval.")+"\n"
							+tr("Disables history lookup when using random."));
	QObject::connect(timerBox, SIGNAL(clicked(bool)), this, SLOT(enableTimerClicked(bool)));

	QHBoxLayout* timerIntervalLayout = new QHBoxLayout();
	QLabel* timerLabel = new QLabel(tr("Timer Interval"));
	timerLabel->setToolTip(tr("The number of minutes to wait before switching screen savers."));
	timerIntervalLayout->addWidget(timerLabel);

	_timerIntervalBox = new QSpinBox();
	_timerIntervalBox->setRange(1,1000);
	_timerIntervalBox->setSingleStep(1);
	_timerIntervalBox->setValue(_manager->getTimerMinutes());
	_timerIntervalBox->setToolTip(tr("The number of minutes to wait before switching screen savers."));
	timerIntervalLayout->addWidget(_timerIntervalBox);

	timerBox->setLayout(timerIntervalLayout);
	aboveListsLayout->addWidget(timerBox);

	tempWidget = new QWidget();
	tempWidget->setLayout(aboveListsLayout);
	mainLayout->addWidget(tempWidget);
	tempWidget = NULL;

	// Scr List boxes and buttons in between
	_avail = new QListWidget();
	_avail->setSelectionMode(QAbstractItemView::ExtendedSelection);
	_avail->setToolTip(tr("This is the list of available screen savers."));

	QHBoxLayout* listsLayout = new QHBoxLayout();
	listsLayout->addWidget(_avail);

	QVBoxLayout* buttonLayout = new QVBoxLayout();

	tempButton = new QPushButton(">>");
	tempButton->setToolTip(tr("Click to add the items selected in the list of available screen savers")+"\n"
								+tr("to the list of selected screen savers."));
	QObject::connect(tempButton, SIGNAL(clicked(bool)), this, SLOT(addClicked(bool)));
	buttonLayout->addWidget(tempButton);
	tempButton = NULL;

	tempButton = new QPushButton("<<");
	tempButton->setToolTip(tr("Click to remove the selected items in the list of selected screen savers."));
	QObject::connect(tempButton, SIGNAL(clicked(bool)), this, SLOT(removeClicked(bool)));
	buttonLayout->addWidget(tempButton);
	tempButton = NULL;
	
	tempButton = new QPushButton(tr("Up"));
	tempButton->setToolTip(tr("Click to move the selected item in the list of selected screen savers up in the order."));
	QObject::connect(tempButton, SIGNAL(clicked(bool)), this, SLOT(upClicked(bool)));
	buttonLayout->addWidget(tempButton);
	tempButton = NULL;

	tempButton = new QPushButton(tr("Down"));
	tempButton->setToolTip(tr("Click to move the selected item in the list of selected screen savers down in the order."));
	QObject::connect(tempButton, SIGNAL(clicked(bool)), this, SLOT(downClicked(bool)));
	buttonLayout->addWidget(tempButton);
	tempButton = NULL;

	tempWidget = new QWidget();
	tempWidget->setLayout(buttonLayout);
	listsLayout->addWidget(tempWidget);
	tempWidget = NULL;

	_sel = new QListWidget();
	_sel->setSelectionMode(QAbstractItemView::ExtendedSelection);
	_sel->setToolTip(tr("This is the list of currently selected screen savers."));
	listsLayout->addWidget(_sel);

	tempWidget = new QWidget();
	tempWidget->setLayout(listsLayout);
	mainLayout->addWidget(tempWidget);
	tempWidget = NULL;

	// Preview and Settings buttons and random checkbox
	QHBoxLayout* belowListsLayout = new QHBoxLayout();

	tempButton = new QPushButton(tr("Preview"));
	tempButton->setToolTip(tr("This button runs the currently selected screen saver in the list on")+"\n"
							  +tr("the left.  When multiple screen savers are selected, only runs the first."));
	QObject::connect(tempButton, SIGNAL(clicked(bool)), this, SLOT(previewClicked(bool)));
	belowListsLayout->addWidget(tempButton);
	tempButton = NULL;

	tempButton = new QPushButton(tr("Settings"));
	tempButton->setToolTip(tr("This button opens the config dialog box for the currently selected screen saver")+"\n"
								+tr("on the left.  When multiple screen savers are selected, only configures the first."));
	QObject::connect(tempButton, SIGNAL(clicked(bool)), this, SLOT(settingsClicked(bool)));
	belowListsLayout->addWidget(tempButton);
	tempButton = NULL;

	belowListsLayout->addStretch(0);

	QVBoxLayout* randomLayout = new QVBoxLayout();

	QGroupBox* randomBox = new QGroupBox(tr("Choose &Randomly"));
	randomBox->setCheckable(true);
	randomBox->setChecked(_manager->isRandom());
	randomBox->setToolTip(tr("Enabling this will randomize the order in which the screen savers will run."));
	QObject::connect(randomBox, SIGNAL(clicked(bool)), this, SLOT(randomClicked(bool)));

	QHBoxLayout* historyLayout = new QHBoxLayout();
	QLabel* historyLabel = new QLabel(tr("Size of History"));
	historyLabel->setToolTip(tr("The number of times before a screen saver can run again."));
	historyLayout->addWidget(historyLabel);

	_historyBox = new QSpinBox();
	_historyBox->setRange(0,10);
	_historyBox->setSingleStep(1);
	_historyBox->setValue(_manager->getHistorySize());
	_historyBox->setToolTip(tr("The number of times before a screen saver can run again."));
	historyLayout->addWidget(_historyBox);

	randomBox->setLayout(historyLayout);
	belowListsLayout->addWidget(randomBox);

	tempWidget = new QWidget();
	tempWidget->setLayout(belowListsLayout);
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

	//populate list boxes
	populateSelectedList();
	updateListBoxes();
}

void ConfigWidget::setManager(ConfigManager* manager) {
	_manager = manager;
}

void ConfigWidget::aboutClicked(bool checked) {
	AboutDialog* dlg = new AboutDialog(getAppFullName(), getAppVersion());
	if(dlg != NULL) {
		dlg->exec();
		delete dlg;
		dlg = NULL;
	}
}

void ConfigWidget::addClicked(bool checked) {
	QList<QListWidgetItem*> itemList = _avail->selectedItems();
	for(int i =0; i<itemList.size(); i++) {
		if(_sel->findItems(itemList.at(i)->text(),Qt::MatchExactly).empty()) {
			_sel->addItem(itemList.at(i)->text());
		}
	}
}

void ConfigWidget::removeClicked(bool checked) {
	QList<QListWidgetItem*> list = _sel->selectedItems();
	for(int i=0;i<list.size(); i++) {
		QListWidgetItem* tmp = _sel->takeItem(_sel->row(list.at(i)));
		delete tmp;
		tmp = NULL;
	}
}

void ConfigWidget::upClicked(bool checked) {
	int row = _sel->currentRow();
	if(row > 0) {
		_sel->insertItem(row-1,_sel->takeItem(row));
		_sel->setCurrentRow(row-1);
	}
}

void ConfigWidget::downClicked(bool checked) {
	int row = _sel->currentRow();
	if(row < _sel->count()) {
		_sel->insertItem(row+1,_sel->takeItem(row));
		if(row+1 == _sel->count()) {
			_sel->setCurrentRow(_sel->count()-1);
		} else {
			_sel->setCurrentRow(row+1);
		}
	}
}

void ConfigWidget::okClicked(bool checked) {
	storeSelected();
	_manager->setHistorySize(_historyBox->value());
	_manager->save();

	emit dialogFinished();
}

void ConfigWidget::applyClicked(bool checked) {
	storeSelected();
	_manager->setHistorySize(_historyBox->value());
	_manager->setTimerMinutes(_timerIntervalBox->value());
	_manager->save();
}

void ConfigWidget::cancelClicked(bool checked) {
	emit dialogFinished();
}

void ConfigWidget::searchPathsClicked(bool checked) {
	_search->exec();
}

void ConfigWidget::searchAccepted(void) {
	_manager->setSearchPathList(_search->retrievePathList());
	updateListBoxes();
}

void ConfigWidget::randomClicked(bool checked) {
	_manager->enableRandom(checked);
}

void ConfigWidget::previewClicked(bool checked) {
	QList<QListWidgetItem*> itemList = _avail->selectedItems();
	if(itemList.size()>0) {
		QString path = _nameMap.value(itemList.at(0)->text());

		if(!path.isNull()) {
			QStringList args;
			args.append("/s");

			QProcess* process = new QProcess();
			process->start(path, args);
			if(process->waitForStarted()) {
				process->waitForFinished(-1);
			}
			delete process;
			process = NULL;
		}
	}
}

void ConfigWidget::settingsClicked(bool checked) {
	QList<QListWidgetItem*> itemList = _avail->selectedItems();
	if(itemList.size()>0) {
		QString path = _nameMap.value(itemList.at(0)->text());

		if(!path.isNull()) {
			QStringList args;
			args.append("/c");

			QProcess* process = new QProcess();
			process->start(path, args);
			if(process->waitForStarted()) {
				process->waitForFinished(-1);
			}
			delete process;
			process = NULL;
		}
	}
}

void ConfigWidget::updateListBoxes() {

	//empty list of available savers and name map
	_avail->clear();
	_nameMap.clear();

	QListWidgetItem* p;


	//for each directory in the list, get a list of .scr files and add them
	//to the list.
	QStringList filters;
	filters << "*.scr";
	QStringList list = _manager->getSearchPathList();
	for(int i = 0; i < list.size(); i++) {
		QString searchDir = list.at(i); 
		QDir curdir(searchDir);
		curdir.setNameFilters(filters);

		//make sure that WinRndScr isn't among the list of available SCRs.
		QStringList pathList = curdir.entryList();
		if(pathList.contains(_manager->getProgramName(), Qt::CaseInsensitive)) {
			pathList.removeAt(pathList.indexOf(_manager->getProgramName()));
		}

		for(int j = 0; j < pathList.size(); j++) {
			p = genListEntry(searchDir + "\\" + pathList.at(j));

			//check to see if this item already exists.  If so, change it's text to the full path.
			if(_nameMap.contains(p->text())) {
				p->setText(searchDir + "\\" + pathList.at(j));
			}
			_avail->addItem(p);
			_nameMap[p->text()] = searchDir + "\\" + pathList.at(j);
			p = NULL;
		}
	}

	//now go through the list of selected screen savers, removing any that aren't
	//in the avail list
	for(i = 0; i < _sel->count(); i++) {
		p = _sel->item(i);
		if(!_nameMap.contains(p->text())) {
			p = _sel->takeItem(i--);
			delete p;
		}
		p = NULL;
	} 
}

QListWidgetItem* ConfigWidget::genListEntry(QString path) {
	QPixmap* tempIcon = NULL;
	QListWidgetItem* retVal = NULL;
	wchar_t pathText[255];
	path.toWCharArray(&pathText[0]);
	pathText[path.size()] = 0x00;

	wchar_t resourceName[255];
	QString("ID_APP").toWCharArray(&resourceName[0]);
	resourceName[QString("ID_APP").size()] = 0x00;

	HINSTANCE hMod = LoadLibraryEx(&pathText[0],NULL,LOAD_LIBRARY_AS_DATAFILE);
	if(hMod != NULL) {
		wchar_t buff[255];
		int strLen = LoadString(hMod, 1, &buff[0], 255);
		wstring bufStr(&buff[0]);
		QString desc;
		desc = QString::fromStdWString(bufStr);
	/*
		//Can get icon, but can't get to copy into a QPixmap object.
		//HICON largeIcon = (HICON)LoadImage(hMod,&resourceName[0], IMAGE_ICON, 32,32,LR_DEFAULTCOLOR);
		//HICON largeIcon = (HICON)LoadImage(hMod,MAKEINTRESOURCE(100), IMAGE_ICON, 32,32,LR_DEFAULTCOLOR);
		//HICON largeIcon = (HICON)LoadIcon(hMod,&resourceName[0]);
		HICON largeIcon = (HICON)LoadIcon(hMod,MAKEINTRESOURCE(100));
		if(largeIcon != NULL) {
			tempIcon = new QPixmap(32,32);
			HBITMAP dc = tempIcon->toWinHBITMAP();
			//HDC dc = tempIcon->getDC();
			DrawIcon((HDC)dc, 0, 0, largeIcon);
			tempIcon->fromWinHBITMAP(dc);
			//DeleteObject(dc);

			QIcon* pIcon = new QIcon(*tempIcon);
			//appIcon = QIcon(*tempIcon);
			retVal =  new QListWidgetItem(*pIcon, desc);
			//retVal = new QListWidgetItem(appIcon, desc);
			//DestroyIcon(largeIcon);
		}
		else {*/
			retVal = new QListWidgetItem(desc);
		//}
		FreeLibrary(hMod);
	}
	else {
		retVal = new QListWidgetItem(path.mid(path.lastIndexOf('\\')+1,path.lastIndexOf('.') - path.lastIndexOf('\\') - 1));
		//retVal = new QListWidgetItem(path);
	}
	
	return retVal;	
}

void ConfigWidget::populateSelectedList() {
	QListWidgetItem* p;

	QStringList scrList = _manager->getScrList();

	for(int j = 0; j < scrList.size(); j++) {
		p = genListEntry(scrList.at(j));

		//check to see if this item already exists.  If so, change it's text to the full path.
		if(_nameMap.contains(p->text())) {
			p->setText(scrList.at(j));
		}
		_sel->addItem(p);
		_nameMap[p->text()] = scrList.at(j);
		p = NULL;
	}
}


void ConfigWidget::storeSelected(){
	QStringList list;
	for(int i=0; i<_sel->count(); i++) {
		list<< _nameMap.value(_sel->item(i)->text());
	}
	_manager->setScrList(list);
}

//shows the GPL dialog
void ConfigWidget::showGPLDialog(void) {
	_gplDialog->exec();
	_manager->setGPLAccepted(_gplDialog->isAccepted());
	if(_manager->getGPLAccepted()) {
		_manager->save();
	}
}

void ConfigWidget::enableTimerClicked(bool checked) {
	_manager->enableTimer(checked);
}

void ConfigWidget::hideClicked(bool checked) {
	_manager->setBlankDesktop(checked);
}
