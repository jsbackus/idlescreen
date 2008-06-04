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
#include <QListWidgetItem>

#include <QMessageBox>
#include <QInputDialog>

#include "PaletteSelectWidget.h"
#include "../IndexedPalette/IndexedPaletteDialog.h"

PaletteSelectWidget::PaletteSelectWidget(ConfigManager* confMgr, QWidget* parent, Qt::WindowFlags f) {
	QWidget(parent, f);

	_palDlg = NULL;

	_confMgr = confMgr;

	QWidget* tempWidget = NULL;
	QPushButton* tempButton = NULL;
	QVBoxLayout* mainLayout = new QVBoxLayout();

	_list = new QListWidget();
	_list->setSelectionMode(QAbstractItemView::SingleSelection);
	_list->setToolTip(tr("This is the list of available Palette profiles."));
	mainLayout->addWidget(_list);

	QHBoxLayout* bkgndBtnLayout = new QHBoxLayout();
	tempButton = new QPushButton(tr("New"));
	QObject::connect(tempButton, SIGNAL(clicked(bool)), this, SLOT(addClicked(bool)));
	bkgndBtnLayout->addWidget(tempButton);
	tempButton = NULL;

	tempButton = new QPushButton(tr("Duplicate"));
	QObject::connect(tempButton, SIGNAL(clicked(bool)), this, SLOT(copyClicked(bool)));
	bkgndBtnLayout->addWidget(tempButton);
	tempButton = NULL;

	tempButton = new QPushButton(tr("Edit"));
	QObject::connect(tempButton, SIGNAL(clicked(bool)), this, SLOT(editClicked(bool)));
	bkgndBtnLayout->addWidget(tempButton);
	tempButton = NULL;

	tempButton = new QPushButton(tr("Delete"));
	QObject::connect(tempButton, SIGNAL(clicked(bool)), this, SLOT(removeClicked(bool)));
	bkgndBtnLayout->addWidget(tempButton);
	tempButton = NULL;

	tempWidget = new QWidget();
	tempWidget->setLayout(bkgndBtnLayout);
	mainLayout->addWidget(tempWidget);
	tempWidget = NULL;

	setLayout(mainLayout);

	populateBox();
}

PaletteSelectWidget::~PaletteSelectWidget() {
	_confMgr = NULL;

	//should get deleted when the layout gets deleted
	_list = NULL;

	if(_palDlg != NULL) {
		delete _palDlg;
		_palDlg = NULL;
	}
}

/*
 * Retrieves the name of the currently selected Palette.
 */
QString PaletteSelectWidget::getCurrentPaletteName() {
	QListWidgetItem* tmp = _list->currentItem();
	if(tmp != NULL) {
		return tmp->text();
	}
	
	return QString("");
}

/*
 * Sets the currently selected Palette.
 */
void PaletteSelectWidget::setCurrentPalette(QString name) {
	int index = findItem(name);
	if(index >= 0) {
		_list->setCurrentRow(index);
	}
}

void PaletteSelectWidget::addClicked(bool checked) {
	if(_confMgr == NULL || _list == NULL)
		return;

	_palDlg = new IndexedPaletteDialog(NULL, this);
	if(_palDlg != NULL) {
		QObject::connect(_palDlg, SIGNAL(paletteUpdated()), this, SLOT(addAccepted()));
		_palDlg->exec();

		//clean up
		delete _palDlg;
		_palDlg = NULL;
	}
}

void PaletteSelectWidget::copyClicked(bool checked) {
	if(_confMgr == NULL || _list == NULL)
		return;

	int row = _list->currentRow();
	QListWidgetItem* tmp = _list->item(row);
	if(tmp != NULL) {
		QString selected = tmp->text();

		//check to see if the selected palette exists...
		if(_confMgr->doesPaletteExist(selected)) {

			//open up a QInputDialog object to prompt for a new name.
			bool bCopy = false;
			bool bDone = false;
			QString newName;
			while(!bDone) {
				bool bOk = false;
				newName = QInputDialog::getText(this, 0,
					tr("Please enter a name for new copy of '")+selected+tr("':"), QLineEdit::Normal,
					selected, &bOk);

				if(!bOk) {
					bDone = true;
				} else {
					if(newName == "") {
						QMessageBox::warning(this, 0, "Please enter a valid name.", QMessageBox::Ok);
					} else {
						if(_confMgr->doesPaletteExist(newName)) {
							//name already exists.  give the user a warning dialog box
							QString message(tr("A palette with name '"));
							message += newName;
							message += "' already exists.  Please choose a new name.";
							QMessageBox::warning(this, 0, message, QMessageBox::Ok);
						} else {
							bDone = true;
							bCopy = true;
						}
					}
				}
			}
			//if the user hit ok and the name is not empty, copy!
			if(bCopy) {
				IndexedPaletteProfile newPal = _confMgr->getPaletteProfile(selected);
				newPal.setName(newName);
				_confMgr->addPalette(newPal);
				_list->addItem(newName);
			}
		}
	}
	tmp = NULL;
}

void PaletteSelectWidget::editClicked(bool checked) {
	if(_confMgr == NULL || _list == NULL)
		return;

	int row = _list->currentRow();
	QListWidgetItem* tmp = _list->item(row);
	if(tmp != NULL) {
		QString selected = tmp->text();

		//first, check for existence.
		if(_confMgr->doesPaletteExist(selected)) {
			IndexedPaletteProfile param = _confMgr->getPaletteProfile(selected);
			editPalette(param, false);
		}
	}
	tmp = NULL;
}

void PaletteSelectWidget::removeClicked(bool checked) {
	if(_confMgr == NULL || _list == NULL)
		return;

	int row = _list->currentRow();
	QListWidgetItem* tmp = _list->takeItem(row);
	if(tmp != NULL) {
		QString selected = tmp->text();
		//attempt to remove the selected item.
		_confMgr->removePalette(selected);
	}
	//delete the list widget item.
	delete tmp;
	tmp = NULL;
}

//populates the list box
void PaletteSelectWidget::populateBox(void) {
	if(_confMgr == NULL ||  _list == NULL)
		return;

	//Palette list
	_list->clear();
	_list->addItems(_confMgr->getPaletteNames());
}

//searches the list for the specified string, returning its index, or -1.
int PaletteSelectWidget::findItem(QString name) {
	if(_list == NULL)
		return -1;

	int count = _list->count();
	int i = 0;
	while(i<count) {
		QListWidgetItem* tmp = _list->item(i);
		if(tmp != NULL && tmp->text() == name) {
			return i;
		}
		i++;
	}

	return -1;
}

void PaletteSelectWidget::addAccepted(void) {
	if(_confMgr == NULL || _list == NULL)
		return;

	//check to see if the name of the new palette is
	//already in the list.  If so, go into edit mode.
	if(_palDlg != NULL) {
		IndexedPaletteProfile pal = _palDlg->getPalette();

		//first make sure the name is valid.  If so, check to see if it is currently
		//in the hash.  If not, add it.
		if(pal.getName() == "") {
			//give the user a warning dialog box
			QMessageBox::warning(this, 0, "Please enter a valid name.", QMessageBox::Ok);

			//attempt to edit
			editPalette(pal, true);
		} else if(_confMgr->doesPaletteExist(pal.getName())) {
			//give the user a warning dialog box
			QString message(tr("A palette with name '"));
			message += pal.getName();
			message += "' already exists.  Please choose a new name.";
			QMessageBox::warning(this, 0, message, QMessageBox::Ok);

			//attempt to edit
			editPalette(pal, true);
		} else {
			//nope, not in the list.  Add it!
			_confMgr->addPalette(pal);
			_list->addItem(pal.getName());
		}
	}
}

void PaletteSelectWidget::editAccepted(void) {
	if(_confMgr == NULL || _list == NULL)
		return;

	if(_palDlg != NULL) {
		IndexedPaletteProfile pal = _palDlg->getPalette();

		//check to make sure the name is valid.
		if(pal.getName() == "") {
			//give the user a warning dialog box
			QMessageBox::warning(this, 0, "Please enter a valid name.", QMessageBox::Ok);

			//restore old name and attempt to edit
			pal.setName(_palOldName);
			editPalette(pal, false);
			return;
		}

		//check to see if the name has changed and if so, whether the new name is in the list already
		if( (_palOldName == "" || _palOldName != pal.getName()) && _confMgr->doesPaletteExist(pal.getName())) {
			//give the user a warning dialog box
			QString message(tr("A palette with name '"));
			message += pal.getName();
			message += "' already exists.  Please choose a new name.";
			QMessageBox::warning(this, 0, message, QMessageBox::Ok);

			//attempt to edit
			editPalette(pal, false);
			return;
		}

		//if the old name is valid, replace.  otherwise insert.
		if(_palOldName != "") {
			_confMgr->replacePalette(_palOldName,pal);
		} else {
			_confMgr->addPalette(pal);
		}

		//if the old name and new name are different, search the list
		//widget, remove the old and add the new
		if(_palOldName != "" && _palOldName != pal.getName()) {
			int i=0;
			int count = _list->count();
			QListWidgetItem* tmpItem = _list->item(0);
			while(tmpItem != NULL && tmpItem->text() != _palOldName && i < count) {
				i++;
				tmpItem = _list->item(i);
			}
			if(tmpItem != NULL && tmpItem->text() == _palOldName) {
				_list->takeItem(i);
				_list->addItem(pal.getName());
			}
		}
	}
}

//attempts to edit the specified palette.
void PaletteSelectWidget::editPalette(IndexedPaletteProfile& pal, bool bWasAddRename) {
	//store the current name, in case the name has changed.  If this is 
	//a forced rename, don't store the current name.
	if(bWasAddRename) {
		_palOldName = "";
	} else {
		_palOldName = pal.getName();
	}

	//attempt to create the dialog
	_palDlg = new IndexedPaletteDialog(&pal,this);
	if(_palDlg != NULL) {
		QObject::connect(_palDlg, SIGNAL(paletteUpdated()), this, SLOT(editAccepted()));
		_palDlg->exec();

		//clean up
		delete _palDlg;
		_palDlg = NULL;
	} else {
		//problem.  clear the current name
		_palOldName = "";
	}
}

