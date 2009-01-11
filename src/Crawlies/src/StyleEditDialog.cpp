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
#include <QMessageBox>
// end debug

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QGroupBox>
#include <QLabel>

#include "StyleEditDialog.h"
#include "utility/misc_funcs.h"
#include "utility/HelpDialog.h"

StyleEditDialog::StyleEditDialog(crawly_profile_style* style, ConfigManager* confMgr, QWidget* parent, Qt::WindowFlags f) {
  QDialog(parent, f);

  if(style != NULL) {
    _dataVals = *style;
  } else {
    // set defaults
    _dataVals.minLength = 3;
    _dataVals.maxLength = -1;
    _dataVals.thickness = 1;
    _dataVals.minSpriteSpeed = 1.0;
    _dataVals.maxSpriteSpeed = 3.0;
    _dataVals.palSpeed = 1.0;
    _dataVals.bHeadConstantColor = true;
    _dataVals.bHeadRandomColor = true;
  }

  _paletteBox = NULL;
  _palDlg = NULL;
  _minSizeBox = NULL;
  _maxSizeBox = NULL;
  _thicknessBox = NULL;
  _minSpeedBox = NULL;
  _maxSpeedBox = NULL;
  _palSpeedBox = NULL;

  _confMgr = confMgr;

  if(confMgr == NULL)
    return;

  _windowTitle = tr("Style Editor");

  setWindowTitle(_windowTitle);
  setWindowModality(Qt::WindowModal);
  QIcon icon(":/app_icon.png");
  setWindowIcon(icon);

  //set up dialog box
  QWidget* tempWidget = NULL;
  QPushButton* tempButton = NULL;
  QVBoxLayout* mainLayout = new QVBoxLayout();
  QString tempToolTip;
  QGroupBox* tmpGroup = NULL;
  QVBoxLayout* tmpVBox = NULL;
  QHBoxLayout* tmpHBox = NULL;
  QCheckBox* tmpCheckBox = NULL;

  // palette name combo box + add & edit buttons
  QGroupBox* palGroupBox = new QGroupBox(tr("Palette"));
  if(palGroupBox == NULL)
    return;
  palGroupBox->setCheckable(false);
  palGroupBox->setFlat(false);

  QHBoxLayout* palBoxLayout = new QHBoxLayout();

  _paletteBox = new QComboBox();
  _paletteBox->setEditable(false);
  populatePalList(_dataVals.pal);
  tempToolTip = tr("The currently selected palette.");
  _paletteBox->setToolTip(tempToolTip);
  palBoxLayout->addWidget(_paletteBox);

  palBoxLayout->addStretch(0);

  tempButton = new QPushButton(tr("&Add"));
  tempButton->setDefault(false);
  tempButton->setAutoDefault(false);
  tempToolTip = tr("Add a new palette.");
  tempButton->setToolTip(tempToolTip);
  QObject::connect(tempButton, SIGNAL(clicked()), this,
		   SLOT(addPalClicked()));
  palBoxLayout->addWidget(tempButton);
  tempButton = NULL;

  tempButton = new QPushButton(tr("&Edit"));
  tempButton->setDefault(false);
  tempButton->setAutoDefault(false);
  tempToolTip = tr("Edit the selected palette.");
  tempButton->setToolTip(tempToolTip);
  QObject::connect(tempButton, SIGNAL(clicked()), this,
		   SLOT(editPalClicked()));
  palBoxLayout->addWidget(tempButton);
  tempButton = NULL;

  palGroupBox->setLayout(palBoxLayout);
  mainLayout->addWidget(palGroupBox);
  tempWidget = NULL;

  // length box
  tmpGroup = new QGroupBox(tr("Crawly Length"));
  if(tmpGroup == NULL)
    return;
  tmpGroup->setFlat(false);
  tmpGroup->setCheckable(false);

  QHBoxLayout* lengthLayout = new QHBoxLayout();
  if(lengthLayout == NULL)
    return;

  // min
  tmpVBox = new QVBoxLayout();
  if(tmpVBox == NULL)
    return;

  tmpHBox = new QHBoxLayout();
  if(tmpHBox == NULL)
    return;

  tempToolTip = tr("Minimum Crawly length");
  _bMinUsePalSize = (_dataVals.minLength == -1);
  tempWidget = new QLabel(tr("Min:"));
  if(tempWidget == NULL)
    return;
  tempWidget->setToolTip(tempToolTip);
  tmpHBox->addWidget(tempWidget);
  tempWidget = NULL;

  _minSizeBox = new QSpinBox();
  if(_minSizeBox == NULL)
    return;
  _minSizeBox->setEnabled(!_bMinUsePalSize);
  _minSizeBox->setMinimum(1);
  _minSizeBox->setMaximum(10000);
  _minSizeBox->setSingleStep(1);
  _minSizeBox->setValue(_dataVals.minLength);
  _minSizeBox->setToolTip(tempToolTip);
  tmpHBox->addWidget(_minSizeBox);
  
  tempWidget = new QWidget();
  if(tempWidget == NULL)
    return;
  tempWidget->setLayout(tmpHBox);
  tmpVBox->addWidget(tempWidget);
  tempWidget = NULL;
  tmpHBox = NULL;

  tmpCheckBox = new QCheckBox(tr("Use Palette Length For Min"));
  if(tmpCheckBox == NULL)
    return;
  tmpCheckBox->setToolTip(tr("Will use palette length for Crawly minimum length."));
  tmpCheckBox->setTristate(false);
  tmpCheckBox->setCheckState(boolToCheckState(_bMinUsePalSize));
  QObject::connect(tmpCheckBox, SIGNAL(clicked(bool)), this, SLOT(minUsePalSizeClicked(bool)));
  tmpVBox->addWidget(tmpCheckBox);
  tmpCheckBox = NULL;

  tempWidget = new QWidget();
  if(tempWidget == NULL)
    return;
  tempWidget->setLayout(tmpVBox);
  lengthLayout->addWidget(tempWidget);
  tempWidget = NULL;
  tmpVBox = NULL;

  lengthLayout->addStretch(0);

  // max
  tmpVBox = new QVBoxLayout();
  if(tmpVBox == NULL)
    return;

  tmpHBox = new QHBoxLayout();
  if(tmpHBox == NULL)
    return;

  tempToolTip = tr("Maximum Crawly length");
  _bMaxUsePalSize = (_dataVals.maxLength == -1);
  tempWidget = new QLabel(tr("Max:"));
  if(tempWidget == NULL)
    return;
  tempWidget->setToolTip(tempToolTip);
  tmpHBox->addWidget(tempWidget);
  tempWidget = NULL;

  _maxSizeBox = new QSpinBox();
  if(_maxSizeBox == NULL)
    return;
  _maxSizeBox->setEnabled(!_bMaxUsePalSize);
  _maxSizeBox->setMinimum(1);
  _maxSizeBox->setMaximum(10000);
  _maxSizeBox->setSingleStep(1);
  _maxSizeBox->setValue(_dataVals.maxLength);
  _maxSizeBox->setToolTip(tempToolTip);
  tmpHBox->addWidget(_maxSizeBox);
  
  tempWidget = new QWidget();
  if(tempWidget == NULL)
    return;
  tempWidget->setLayout(tmpHBox);
  tmpVBox->addWidget(tempWidget);
  tempWidget = NULL;
  tmpHBox = NULL;

  tmpCheckBox = new QCheckBox(tr("Use Palette Length For Max"));
  if(tmpCheckBox == NULL)
    return;
  tmpCheckBox->setToolTip(tr("Will use palette length for Crawly maximum length."));
  tmpCheckBox->setTristate(false);
  tmpCheckBox->setCheckState(boolToCheckState(_bMaxUsePalSize));
  QObject::connect(tmpCheckBox, SIGNAL(clicked(bool)), this, SLOT(maxUsePalSizeClicked(bool)));
  tmpVBox->addWidget(tmpCheckBox);
  tmpCheckBox = NULL;

  tempWidget = new QWidget();
  if(tempWidget == NULL)
    return;
  tempWidget->setLayout(tmpVBox);
  lengthLayout->addWidget(tempWidget);
  tempWidget = NULL;
  tmpVBox = NULL;
  
  tmpGroup->setLayout(lengthLayout);
  mainLayout->addWidget(tmpGroup);
  tmpGroup = NULL;

  // speed
  tmpGroup = new QGroupBox(tr("Crawly Speed"));
  if(tmpGroup == NULL)
    return;
  tmpGroup->setFlat(false);
  tmpGroup->setCheckable(false);

  tmpHBox = new QHBoxLayout();
  if(tmpHBox == NULL)
    return;

  // min
  tempToolTip = tr("Minimum Crawly speed");
  tempWidget = new QLabel(tr("Min:"));
  if(tempWidget == NULL)
    return;
  tempWidget->setToolTip(tempToolTip);
  tmpHBox->addWidget(tempWidget);
  tempWidget = NULL;

  _minSpeedBox = new QDoubleSpinBox();
  if(_minSpeedBox == NULL)
    return;
  _minSpeedBox->setSingleStep(0.1);
  _minSpeedBox->setMinimum(0.01);
  _minSpeedBox->setMaximum(100.0);
  _minSpeedBox->setSingleStep(0.1);
  _minSpeedBox->setValue(_dataVals.minSpriteSpeed);
  _minSpeedBox->setToolTip(tempToolTip);
  tmpHBox->addWidget(_minSpeedBox);

  tmpHBox->addStretch(0);

  // max
  tempToolTip = tr("Maximum Crawly speed");
  tempWidget = new QLabel(tr("Max:"));
  if(tempWidget == NULL)
    return;
  tempWidget->setToolTip(tempToolTip);
  tmpHBox->addWidget(tempWidget);
  tempWidget = NULL;

  _maxSpeedBox = new QDoubleSpinBox();
  if(_maxSpeedBox == NULL)
    return;
  _maxSpeedBox->setSingleStep(0.1);
  _maxSpeedBox->setMinimum(0.01);
  _maxSpeedBox->setMaximum(100.0);
  _maxSpeedBox->setSingleStep(0.1);
  _maxSpeedBox->setValue(_dataVals.maxSpriteSpeed);
  _maxSpeedBox->setToolTip(tempToolTip);
  tmpHBox->addWidget(_maxSpeedBox);
  
  tmpGroup->setLayout(tmpHBox);
  mainLayout->addWidget(tmpGroup);
  tmpGroup = NULL;
  tmpHBox = NULL;


  // thickness and palette speed
  tmpHBox = new QHBoxLayout();
  if(tmpHBox == NULL)
    return;

  tempToolTip = tr("Crawly Thickness");
  tempWidget = new QLabel(tr("Thickness:"));
  if(tempWidget == NULL)
    return;
  tempWidget->setToolTip(tempToolTip);
  tmpHBox->addWidget(tempWidget);
  tempWidget = NULL;

  _thicknessBox = new QSpinBox();
  if(_thicknessBox == NULL)
    return;
  _thicknessBox->setMinimum(1);
  _thicknessBox->setMaximum(100);
  _thicknessBox->setSingleStep(1);
  _thicknessBox->setValue(_dataVals.thickness);
  _thicknessBox->setToolTip(tempToolTip);
  tmpHBox->addWidget(_thicknessBox);

  tmpHBox->addStretch(0);

  // palette speed
  tempToolTip = tr("Palette (secondary) rotation speed");
  tempWidget = new QLabel(tr("Palette Speed:"));
  if(tempWidget == NULL)
    return;
  tempWidget->setToolTip(tempToolTip);
  tmpHBox->addWidget(tempWidget);
  tempWidget = NULL;

  _palSpeedBox = new QDoubleSpinBox();
  if(_palSpeedBox == NULL)
    return;
  _palSpeedBox->setSingleStep(0.1);
  _palSpeedBox->setMinimum(0.01);
  _palSpeedBox->setMaximum(100.0);
  _palSpeedBox->setSingleStep(0.1);
  _palSpeedBox->setValue(_dataVals.palSpeed);
  _palSpeedBox->setToolTip(tempToolTip);
  tmpHBox->addWidget(_palSpeedBox);
  
  tempWidget = new QWidget();
  if(tempWidget == NULL)
    return;
  tempWidget->setLayout(tmpHBox);
  mainLayout->addWidget(tempWidget);
  tempWidget = NULL;
  tmpHBox = NULL;

  // head color check boxes
  tmpHBox = new QHBoxLayout();
  if(tmpHBox == NULL)
    return;
  
  // constant color
  _bHeadConstant = _dataVals.bHeadConstantColor;
  tmpCheckBox = new QCheckBox(tr("Head Keeps Constant Color"));
  if(tmpCheckBox == NULL)
    return;
  tmpCheckBox->setToolTip(tr("Crawly head keeps the same palette index (in the primary direction) as it moves."));
  tmpCheckBox->setTristate(false);
  tmpCheckBox->setCheckState(boolToCheckState(_bHeadConstant));
  QObject::connect(tmpCheckBox, SIGNAL(clicked(bool)), this, 
		   SLOT(headConstantColorClicked(bool)));
  tmpHBox->addWidget(tmpCheckBox);
  tmpCheckBox = NULL;

  tmpHBox->addStretch(0);

  // random color
  _bHeadRandom = _dataVals.bHeadRandomColor;
  tmpCheckBox = new QCheckBox(tr("Random Head Color On Spawn"));
  if(tmpCheckBox == NULL)
    return;
  tmpCheckBox->setToolTip(tr("Crawly head color index (primary direction) is randomly chosen on spawn when checked, otherwise it is 0."));
  tmpCheckBox->setTristate(false);
  tmpCheckBox->setCheckState(boolToCheckState(_bHeadRandom));
  QObject::connect(tmpCheckBox, SIGNAL(clicked(bool)), this, 
		   SLOT(headRandomColorClicked(bool)));
  tmpHBox->addWidget(tmpCheckBox);
  tmpCheckBox = NULL;

  tempWidget = new QWidget();
  if(tempWidget == NULL)
    return;
  tempWidget->setLayout(tmpHBox);
  mainLayout->addWidget(tempWidget);
  tempWidget = NULL;
  tmpHBox = NULL;

  //Help, OK and Cancel buttons
  QHBoxLayout* botButtonsLayout = new QHBoxLayout();
	
  tempButton = new QPushButton(tr("&Help"));
  tempButton->setDefault(false);
  tempButton->setAutoDefault(false);
  QObject::connect(tempButton, SIGNAL(clicked()), this, SLOT(helpClicked()));
  botButtonsLayout->addWidget(tempButton);
  tempButton = NULL;

  botButtonsLayout->addStretch(0);

  tempButton = new QPushButton(tr("&OK"));
  tempButton->setDefault(true);
  tempButton->setAutoDefault(true);
  QObject::connect(tempButton, SIGNAL(clicked()), this, SLOT(okClicked()));
  botButtonsLayout->addWidget(tempButton);
  tempButton = NULL;

  tempButton = new QPushButton(tr("&Cancel"));
  QObject::connect(tempButton, SIGNAL(clicked()), this, SLOT(cancelClicked()));
  botButtonsLayout->addWidget(tempButton);
  tempButton = NULL;

  tempWidget = new QWidget();
  tempWidget->setLayout(botButtonsLayout);
  mainLayout->addWidget(tempWidget);
  tempWidget = NULL;

  setLayout(mainLayout);
}

StyleEditDialog::~StyleEditDialog() {
  _confMgr = NULL;
  if(_paletteBox != NULL) {
    delete _paletteBox;
    _paletteBox = NULL;
  }
  if(_palDlg != NULL) {
    delete _palDlg;
    _palDlg = NULL;
  }
  if(_minSizeBox != NULL) {
    delete _minSizeBox;
    _minSizeBox = NULL;
  }
  if(_maxSizeBox != NULL) {
    delete _maxSizeBox;
    _maxSizeBox = NULL;
  }
  if(_thicknessBox != NULL) {
    delete _thicknessBox;
    _thicknessBox = NULL;
  }
  if(_minSpeedBox != NULL) {
    delete _minSpeedBox;
    _minSpeedBox = NULL;
  }
  if(_maxSpeedBox != NULL) {
    delete _maxSpeedBox;
    _maxSpeedBox = NULL;
  }
  if(_palSpeedBox != NULL) {
    delete _palSpeedBox;
    _palSpeedBox = NULL;
  }
}

void StyleEditDialog::okClicked() {
  if(_confMgr == NULL)
    return;
 
  // update data in local copy

  if(_paletteBox->currentIndex() < 0) {
    QMessageBox::warning(this, _windowTitle, "Please choose a palette from the drop-down box.", QMessageBox::Ok);
    return;
  }

  _dataVals.pal = _paletteBox->currentText();
  if(_bMinUsePalSize) {
    _dataVals.minLength = -1;
  } else {
    _dataVals.minLength = _minSizeBox->value();
  }
  if(_bMaxUsePalSize) {
    _dataVals.maxLength = -1;
  } else {
    _dataVals.maxLength = _maxSizeBox->value();
  }
  _dataVals.thickness = _thicknessBox->value();
  _dataVals.minSpriteSpeed = _minSpeedBox->value();
  _dataVals.maxSpriteSpeed = _maxSpeedBox->value();
  _dataVals.palSpeed = _palSpeedBox->value();
  _dataVals.bHeadConstantColor = _bHeadConstant;
  _dataVals.bHeadRandomColor = _bHeadRandom;
  accept();
}

void StyleEditDialog::cancelClicked() {
  reject();
}

void StyleEditDialog::addPalClicked() {
  if(_confMgr == NULL || _paletteBox == NULL)
    return;

  editPalette(QString(""), false);
}

void StyleEditDialog::editPalClicked() {
  if(_confMgr == NULL || _paletteBox == NULL)
    return;

  if(_paletteBox->currentIndex() >= 0) {
    QString selected = _paletteBox->currentText();

    //check for existence and edit if so
    if(_confMgr->doesPaletteExist(selected)) {
      editPalette(selected, true);
    }
  }
}

//attempts to edit the specified palette.
void StyleEditDialog::editPalette(const QString& palName, bool bReplace) {
  if(_confMgr == NULL || _paletteBox == NULL)
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
	  if(oldPalName != "" && oldPalName != newName) {
	    int i=0;
	    int count = _paletteBox->count();
	    QString tmpItem = _paletteBox->itemText(0);
	    while(tmpItem != oldPalName && i < count) {
	      i++;
	      tmpItem = _paletteBox->itemText(i);
	    }
	    if(tmpItem == oldPalName) {
	      _paletteBox->removeItem(i);
	    }
	  }
	  _paletteBox->addItem(newName);
	  _paletteBox->setCurrentIndex(_paletteBox->count()-1);
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

void StyleEditDialog::minUsePalSizeClicked(bool checked) {
  _bMinUsePalSize = checked;
  _minSizeBox->setEnabled(!checked);
}

void StyleEditDialog::maxUsePalSizeClicked(bool checked) {
  _bMaxUsePalSize = checked;
  _maxSizeBox->setEnabled(!checked);
}

void StyleEditDialog::headConstantColorClicked(bool checked) {
  _bHeadConstant = checked;
}

void StyleEditDialog::headRandomColorClicked(bool checked) {
  _bHeadRandom = checked;
}

// populates the palette name selection combo box
void StyleEditDialog::populatePalList(QString selected) {
  if(_paletteBox == NULL)
    return;

  // empty list and repopulate
  _paletteBox->clear();
  _paletteBox->addItems(_confMgr->getPaletteNames());

  // select the currently selected.
  int idx = _paletteBox->findText(selected);
  _paletteBox->setCurrentIndex(idx);
}

void StyleEditDialog::helpClicked() {
  HelpDialog* dlg = new HelpDialog("qrc:/help/StyleEditDialog.html");
  if(dlg != NULL) {
    dlg->exec();
    delete dlg;
    dlg = NULL;
  }
}

/**
 * Returns the settings.  If cancel was clicked,
 * this routine will replace data with initial values.
 */
crawly_profile_style StyleEditDialog::getStyleData() {
  return _dataVals;
}
