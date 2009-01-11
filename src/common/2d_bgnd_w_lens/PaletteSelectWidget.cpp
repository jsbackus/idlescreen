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

// begin debug
#include <iostream>
using namespace std;
//end debug

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
	_list->setToolTip(tr("This is the list of available palette profiles."));
	mainLayout->addWidget(_list);

	QHBoxLayout* bkgndBtnLayout = new QHBoxLayout();
	tempButton = new QPushButton(tr("New"));
	tempButton->setToolTip(tr("Create a new palette from scratch."));
	QObject::connect(tempButton, SIGNAL(clicked(bool)), this, SLOT(addClicked(bool)));
	bkgndBtnLayout->addWidget(tempButton);
	tempButton = NULL;

	tempButton = new QPushButton(tr("Duplicate"));
	tempButton->setToolTip(tr("Create a new palette by copying the currently selected palette."));
	QObject::connect(tempButton, SIGNAL(clicked(bool)), this, SLOT(copyClicked(bool)));
	bkgndBtnLayout->addWidget(tempButton);
	tempButton = NULL;

	tempButton = new QPushButton(tr("Edit"));
	tempButton->setToolTip(tr("Edit the currently selected palette."));
	QObject::connect(tempButton, SIGNAL(clicked(bool)), this, SLOT(editClicked(bool)));
	bkgndBtnLayout->addWidget(tempButton);
	tempButton = NULL;

	tempButton = new QPushButton(tr("Delete"));
	tempButton->setToolTip(tr("Permanently remove the currently selected palette."));
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

	editPalette(QString(""), false);
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

		//check for existence and edit if so
		if(_confMgr->doesPaletteExist(selected)) {
		  editPalette(selected, true);
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

//attempts to edit the specified palette.
void PaletteSelectWidget::editPalette(const QString& palName, bool bReplace) {
  if(_confMgr == NULL || _list == NULL)
    return;
  
  QString oldPalName = palName;
  
  bool bDone = false;
  bool bFirstPass = true;
  
  IndexedPaletteDialog* palDlg = NULL;
  IndexedPaletteProfile tmpProfile;
  if(bReplace) {
    tmpProfile =  _confMgr->getPaletteProfile(palName);
  }

  while(!bDone) {
    // display the palette editor dialog using either a new
    // object or the one requested for edit.
    if(bFirstPass && !bReplace) {
      palDlg = new IndexedPaletteDialog(NULL,this);
    } else {
      palDlg = new IndexedPaletteDialog(&tmpProfile,this);
    }
    bFirstPass = false;
    if(palDlg != NULL) {
      // if the user clicked ok, then try to update the palette
      if(palDlg->exec() == QDialog::Accepted) {
	tmpProfile = palDlg->getPalette();
	
	// check the name to make sure it is valid and not
	// already in the list.
	QString newName = tmpProfile.getName();
	if(newName == "") {
	  //give the user a warning dialog box
	  QMessageBox::warning(this, 0, "Please enter a valid name.", QMessageBox::Ok);
	} else if(oldPalName != newName && _confMgr->doesPaletteExist(newName)) {
	  //give the user a warning dialog box
	  QString message(tr("A palette with name '"));
	  message += newName;
	  message += "' already exists.  Please choose a new name.";
	  QMessageBox::warning(this, 0, message, QMessageBox::Ok);
	} else {
	  // name is valid, so try to insert or replace based on bReplace
	  if(bReplace) {
	    _confMgr->replacePalette(oldPalName, tmpProfile);
	  } else {
	    _confMgr->addPalette(tmpProfile);
	  }

	  // if we are replacing, remove old name first, then
	  // insert the new name.
	  //if(oldPalName != "" && oldPalName != newName) {
	  if(oldPalName != newName) {
	    int i=0;
	    int count = _list->count();
	    QListWidgetItem* tmpItem = _list->item(0);
	    while(tmpItem != NULL && tmpItem->text() != oldPalName && i < count) {
	      i++;
	      tmpItem = _list->item(i);
	    }
	    if(tmpItem != NULL && tmpItem->text() == oldPalName) {
	      _list->takeItem(i);
	    }
	    _list->addItem(newName);
	  }
	  bDone = true;
	}
      } else {
	bDone = true;
      }
      //clean up
      delete _palDlg;
      _palDlg = NULL;
    } else {
      bDone = true;
    }
  }
}
