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

#include "ProfileEditDialog.h"
#include "utility/misc_funcs.h"
#include "utility/HelpDialog.h"

ProfileEditDialog::ProfileEditDialog(QString targetName, ConfigManager* confMgr, QWidget* parent, Qt::WindowFlags f) {
  QDialog(parent, f);

  _numCrawlies = NULL;
  _spawnSlider = NULL;
  _dirSlider = NULL;
  _styleTable = NULL;

  _styleList = NULL;
  _numStyles = 0;
  _maxNumStyles = 0;

  _confMgr = confMgr;

  if(confMgr == NULL)
    return;

  _windowTitle = tr("Profile Editor");

  setWindowTitle(_windowTitle);
  setWindowModality(Qt::WindowModal);
  QIcon icon(":/app_icon.png");
  setWindowIcon(icon);

  _oldName = targetName;
  if(targetName != "") {
    _mp = _confMgr->getProfile(targetName);
  }

  //set up dialog box
  QWidget* tempWidget = NULL;
  QPushButton* tempButton = NULL;
  QVBoxLayout* mainLayout = new QVBoxLayout();
  QHBoxLayout* tmpHBox = NULL;
  QVBoxLayout* tmpVBox = NULL;
  QString tempToolTip;

  // Begin MasterProfile related.
  // Note: MasterProfiles & BackgroundProfiles are
  // collectively called "Configuration Profiles"
  // throughout the program (i.e. that's what the
  // user sees.)

  //name field
  QHBoxLayout* nameLayout = new QHBoxLayout();

  tempToolTip = tr("The name of this configuration profile.");

  tempWidget = new QLabel(tr("Name:"));
  tempWidget->setToolTip(tempToolTip);
  nameLayout->addWidget(tempWidget);
  tempWidget = NULL;

  _nameBox = new QLineEdit(_mp.getName());
  _nameBox->setToolTip(tempToolTip);
  nameLayout->addWidget(_nameBox);

  tempWidget = new QWidget();
  tempWidget->setLayout(nameLayout);
  mainLayout->addWidget(tempWidget);
  tempWidget = NULL;

  //timer and dimensions
  QHBoxLayout* dimAndTimeLayout = new QHBoxLayout();

  //dimensions group box
  QGroupBox* dimBox = new QGroupBox(tr("Screen Dimensions"));
  QHBoxLayout* dimBoxLayout = new QHBoxLayout();

  _screenXBox = new QComboBox();
  _screenYBox = new QComboBox();

  _screenXBox->setEditable(false);
  _screenXBox->setEditable(false);

  //populate the combo box with powers of 2
  int val = 16; //2^4
  int xCurVal = 0;
  int yCurVal = 0;
  int i;
  for(i=0;i<13-4;i++) {
    _screenXBox->addItem(QString::number(val));
    _screenYBox->addItem(QString::number(val));
    if(val <= _mp.getScreenHeight()) {
      yCurVal=i;
    }
    if(val <= _mp.getScreenWidth()) {
      xCurVal=i;
    }
    val*=2;
  }

  _screenXBox->setCurrentIndex(xCurVal);
  _screenYBox->setCurrentIndex(yCurVal);

  tempToolTip = tr("The resolution width of the screen in pixels.  Does not need to match actual")+"\n"+tr("screen resolution.");
  tempWidget = new QLabel(tr("Width:"));
  tempWidget->setToolTip(tempToolTip);
  dimBoxLayout->addWidget(tempWidget);
  tempWidget = NULL;
  _screenXBox->setToolTip(tempToolTip);
  dimBoxLayout->addWidget(_screenXBox);

  dimBoxLayout->addStretch(0);

  tempToolTip = tr("The resolution height of the screen in pixels.  Does not need to match actual")+"\n"+tr("screen resolution.");
  tempWidget = new QLabel(tr("Height:"));
  tempWidget->setToolTip(tempToolTip);
  dimBoxLayout->addWidget(tempWidget);
  tempWidget = NULL;
  _screenYBox->setToolTip(tempToolTip);
  dimBoxLayout->addWidget(_screenYBox);

  dimBox->setLayout(dimBoxLayout);
  dimAndTimeLayout->addWidget(dimBox);
  tempWidget = NULL;

  dimAndTimeLayout->addStretch(0);

  tempWidget = new QLabel(tr("Milliseconds Between Frames:"));
  tempToolTip = tr("The number of milliseconds between frames.  The higher the number, the")+"\n"+tr("slower the animation.");
  tempWidget->setToolTip(tempToolTip);
  dimAndTimeLayout->addWidget(tempWidget);
  tempWidget = NULL;

  _timerBox = new QSpinBox();

  _timerBox = new QSpinBox();
  _timerBox->setRange(10,1000);
  _timerBox->setSingleStep(10);
  _timerBox->setValue(_mp.getTimerMillis());
  _timerBox->setToolTip(tempToolTip);
  dimAndTimeLayout->addWidget(_timerBox);

  tempWidget = new QWidget();
  tempWidget->setLayout(dimAndTimeLayout);
  mainLayout->addWidget(tempWidget);
  tempWidget = NULL;

  // Begin CrawliesBackgroundProfile related
	
  // get the backgroundprofile in order to populate the data fields.

  CrawliesBackgroundProfile* crawliesProfile = NULL;
  QString bkgndProfileName = _mp.getBackgroundProfileName();
  if(_confMgr->doesBackgroundProfileExist(bkgndProfileName)) {
    crawliesProfile = new CrawliesBackgroundProfile();
    if(crawliesProfile != NULL) {
      CrawliesBackgroundProfile* tempProfile = (CrawliesBackgroundProfile*)
	(_confMgr->getBackgroundProfile(bkgndProfileName));
      *crawliesProfile = *tempProfile;
    }
  }

  tmpHBox = new QHBoxLayout();
  if(tmpHBox == NULL)
    return;

  _numCrawlies = new QSpinBox();
  if(_numCrawlies == NULL)
    return;
  _numCrawlies->setMinimum(1);
  _numCrawlies->setMaximum(10000);

  _spawnSlider = new QSlider(Qt::Horizontal);
  if(_spawnSlider == NULL)
    return;
  _spawnSlider->setMinimum(1);
  _spawnSlider->setMaximum(100);
  _spawnSlider->setTickInterval(1);

  _dirSlider = new QSlider(Qt::Horizontal);
  if(_dirSlider == NULL)
    return;
  _dirSlider->setMinimum(0);
  _dirSlider->setMaximum(100);
  _dirSlider->setTickInterval(1);

  if(crawliesProfile == NULL) {
    _numCrawlies->setValue(400);
    _spawnSlider->setValue(75);
    _dirSlider->setValue(50);
  } else {
    _numCrawlies->setValue(crawliesProfile->getMaxNumberCrawlies());
    _spawnSlider->setValue(crawliesProfile->getSpawnChance());
    _dirSlider->setValue(crawliesProfile->getDirChangeChance());
  }

  tempToolTip = 
    tr("Determines the maximum number of crawlies on screen at one time.");
  tempWidget = new QLabel(tr("Max Number:"));
  tempWidget->setToolTip(tempToolTip);
  tmpHBox->addWidget(tempWidget);
  tempWidget = NULL;
  _numCrawlies->setToolTip(tempToolTip);
  tmpHBox->addWidget(_numCrawlies);

  tmpHBox->addStretch(0);

  tmpVBox = new QVBoxLayout();
  if(tmpVBox == NULL)
    return;

  QHBoxLayout* tmpHBox2 = new QHBoxLayout();
  if(tmpHBox2 == NULL)
    return;

  tempToolTip = tr("The percent chance a crawly will spawn.");
  tempWidget = new QLabel(tr("Spawn Chance:"));
  if(tempWidget == NULL)
    return;
  tempWidget->setToolTip(tempToolTip);
  tmpHBox2->addWidget(tempWidget);
  tempWidget = NULL;

  tempWidget = new QLabel(tr("Min"));
  if(tempWidget == NULL)
    return;
  tempWidget->setToolTip(tempToolTip);
  tmpHBox2->addWidget(tempWidget);
  tempWidget = NULL;

  _spawnSlider->setToolTip(tempToolTip);
  tmpHBox2->addWidget(_spawnSlider);

  tempWidget = new QLabel(tr("Max"));
  if(tempWidget == NULL)
    return;
  tempWidget->setToolTip(tempToolTip);
  tmpHBox2->addWidget(tempWidget);
  tempWidget = NULL;

  tempWidget = new QWidget();
  tempWidget->setLayout(tmpHBox2);
  tmpVBox->addWidget(tempWidget);
  tmpHBox2 = NULL;
  tempWidget = NULL;

  tmpHBox2 = new QHBoxLayout();
  if(tmpHBox2 == NULL)
    return;

  tempToolTip = tr("The percent chance a crawly will change direction.");
  tempWidget = new QLabel(tr("Turn Chance:"));
  if(tempWidget == NULL)
    return;
  tempWidget->setToolTip(tempToolTip);
  tmpHBox2->addWidget(tempWidget);
  tempWidget = NULL;

  tempWidget = new QLabel(tr("Min"));
  if(tempWidget == NULL)
    return;
  tempWidget->setToolTip(tempToolTip);
  tmpHBox2->addWidget(tempWidget);
  tempWidget = NULL;

  _dirSlider->setToolTip(tempToolTip);
  tmpHBox2->addWidget(_dirSlider);

  tempWidget = new QLabel(tr("Max"));
  if(tempWidget == NULL)
    return;
  tempWidget->setToolTip(tempToolTip);
  tmpHBox2->addWidget(tempWidget);
  tempWidget = NULL;

  tempWidget = new QWidget();
  tempWidget->setLayout(tmpHBox2);
  tmpVBox->addWidget(tempWidget);
  tmpHBox2 = NULL;
  tempWidget = NULL;

  tempWidget = new QWidget();
  tempWidget->setLayout(tmpVBox);
  tmpHBox->addWidget(tempWidget);
  tempWidget = NULL;
  tmpVBox = NULL;

  tempWidget = new QWidget();
  tempWidget->setLayout(tmpHBox);
  mainLayout->addWidget(tempWidget);
  tmpHBox = NULL;
  tempWidget = NULL;

  // style table
  setupStyleTableWidget(crawliesProfile);
  if(_styleTable == NULL)
    return;

  mainLayout->addWidget(_styleTable);

  // make all cells read-only.  add a region under the table that will
  // allow the user to edit the values of the row that is currently
  // selected.

  // buttons below table widget
  tmpHBox = new QHBoxLayout();
  tempButton = new QPushButton(tr("Add"));
  if(tempButton == NULL)
    return;
  tempButton->setDefault(false);
  tempButton->setAutoDefault(false);
  tempButton->setToolTip(tr("Add a style to the list"));
  QObject::connect(tempButton, SIGNAL(clicked()), this, SLOT(addStyleClicked()));
  tmpHBox->addWidget(tempButton);
  tempButton = NULL;

  tempButton = new QPushButton(tr("Duplicate"));
  if(tempButton == NULL)
    return;
  tempButton->setDefault(false);
  tempButton->setAutoDefault(false);
  tempButton->setToolTip(tr("Duplicate the currently selected style."));
  QObject::connect(tempButton, SIGNAL(clicked()), this, SLOT(copyStyleClicked()));
  tmpHBox->addWidget(tempButton);
  tempButton = NULL;

  tempButton = new QPushButton(tr("Edit"));
  if(tempButton == NULL)
    return;
  tempButton->setDefault(false);
  tempButton->setAutoDefault(false);
  tempButton->setToolTip(tr("Edit the currently selected style."));
  QObject::connect(tempButton, SIGNAL(clicked()), this, SLOT(editStyleClicked()));
  tmpHBox->addWidget(tempButton);
  tempButton = NULL;

  tempButton = new QPushButton(tr("Delete"));
  if(tempButton == NULL)
    return;
  tempButton->setDefault(false);
  tempButton->setAutoDefault(false);
  tempButton->setToolTip(tr("Permanently remove the currently selected style."));
  QObject::connect(tempButton, SIGNAL(clicked()), this, SLOT(deleteStyleClicked()));
  tmpHBox->addWidget(tempButton);
  tempButton = NULL;

  tempWidget = new QWidget();
  if(tempWidget == NULL)
    return;
  tempWidget->setLayout(tmpHBox);
  mainLayout->addWidget(tempWidget);
  tempWidget = NULL;
  tmpHBox = NULL;

  //Help, OK and Cancel buttons
  QHBoxLayout* botButtonsLayout = new QHBoxLayout();
  if(botButtonsLayout == NULL)
    return;
	
  tempButton = new QPushButton(tr("&Help"));
  if(tempButton == NULL)
    return;
  tempButton->setDefault(false);
  tempButton->setAutoDefault(false);
  QObject::connect(tempButton, SIGNAL(clicked(bool)), this, SLOT(helpClicked(bool)));
  botButtonsLayout->addWidget(tempButton);
  tempButton = NULL;

  botButtonsLayout->addStretch(0);

  tempButton = new QPushButton(tr("&OK"));
  if(tempButton == NULL)
    return;
  tempButton->setDefault(true);
  tempButton->setAutoDefault(true);
  QObject::connect(tempButton, SIGNAL(clicked(bool)), this, SLOT(okClicked(bool)));
  botButtonsLayout->addWidget(tempButton);
  tempButton = NULL;

  tempButton = new QPushButton(tr("&Cancel"));
  if(tempButton == NULL)
    return;
  tempButton->setDefault(false);
  tempButton->setAutoDefault(false);
  QObject::connect(tempButton, SIGNAL(clicked(bool)), this, SLOT(cancelClicked(bool)));
  botButtonsLayout->addWidget(tempButton);
  tempButton = NULL;

  tempWidget = new QWidget();
  if(tempWidget == NULL)
    return;
  tempWidget->setLayout(botButtonsLayout);
  mainLayout->addWidget(tempWidget);
  tempWidget = NULL;

  setLayout(mainLayout);
}

ProfileEditDialog::~ProfileEditDialog() {
  _confMgr = NULL;

  if(_nameBox != NULL) {
    delete _nameBox;
    _nameBox = NULL;
  }
	
  if(_timerBox != NULL) {
    delete _timerBox;
    _timerBox = NULL;
  }
	
  if(_screenXBox != NULL) {
    delete _screenXBox;
    _screenXBox = NULL;
  }

  if(_screenYBox != NULL) {
    delete _screenYBox;
    _screenYBox = NULL;
  }
  if(_numCrawlies != NULL) {
    delete _numCrawlies;
    _numCrawlies = NULL;
  }
  if(_spawnSlider != NULL) {
    delete _spawnSlider;
    _spawnSlider = NULL;
  }
  if(_styleTable != NULL) {
    delete _styleTable;
    _styleTable = NULL;
  }
  if(_styleList != NULL) {
    delete [] _styleList;
    _styleList = NULL;
  }
}

void ProfileEditDialog::okClicked(bool checked) {

  /** !!!!!!!!!!!! NOTE !!!!!!!!!
   * This code needs modified to handle the BackgroundProfile and LensProfile
   * modifications/additions.
   */

  if(_confMgr == NULL)
    return;

  //update data in local copy

  // BackgroundProfile related
  // populate data fields
  CrawliesBackgroundProfile tmpProfile;
  tmpProfile.setMaxNumberCrawlies(_numCrawlies->value());
  tmpProfile.setSpawnChance(_spawnSlider->value());

  // copy styles
  tmpProfile.clearStyleList();
  for(int i=0; i<_numStyles;i++) {
    tmpProfile.addStyle();
    tmpProfile.setStyle(i, _styleList[i]);
  }

  // check for an existing BackgroundProfile.  If none
  // create a new one, otherwise update the current one.
  QString tempName = _mp.getBackgroundProfileName();
  if(tempName == "" || !_confMgr->doesBackgroundProfileExist(tempName)) {
    // generate a random BackgroundProfile name that doesn't exist.
    while(tempName == "" || _confMgr->doesBackgroundProfileExist(tempName)) {
      tempName = QString("BackgroundProfile_")+randStr(20);
    }
    tmpProfile.setName(tempName);
    _confMgr->addBackgroundProfile(tmpProfile);
  } else {
    tmpProfile.setName(tempName);
    _confMgr->replaceBackgroundProfile(tempName,tmpProfile);		
  }
  _mp.setBackgroundProfileName(tempName);

  // master profile related
  _mp.setTimerMillis(_timerBox->value());
  _mp.setScreenWidth(_screenXBox->currentText().toInt());
  _mp.setScreenHeight(_screenYBox->currentText().toInt());

  QString newName = _nameBox->text();
  _mp.setName(newName);

  //send local copy back to config manager.
  //need to check to see if this was an addition,
  //if the name has changed
  //if the new name conflicts
  //etc.
  if(newName == "") {
    QString message(tr("Please enter a valid name."));
    QMessageBox::warning(this, _windowTitle, message, QMessageBox::Ok);
    return;		
  }

  //Check to see if the name is already in use.
  if(_oldName != newName &&_confMgr->doesProfileExist(newName)) {
    QString message(tr("A profile with name '"));
    message += newName;
    message += tr("' already exists.  Please choose a new name.");
    QMessageBox::warning(this, _windowTitle, message, QMessageBox::Ok);
    return;
  }
	
  if(_oldName == "") {
    //add it!
    _confMgr->addProfile(_mp);
  } else {
    //we've editted a preexisting palette.
    _confMgr->replaceProfile(_oldName, _mp);
  }

  if(_oldName != newName) {
    emit profileNameChange(_oldName, newName);
  }

  accept();
}

void ProfileEditDialog::cancelClicked(bool checked) {
  reject();
}

void ProfileEditDialog::helpClicked(bool checked) {
  //QMessageBox::information(this, _windowTitle, "Not implemented yet.", QMessageBox::Ok);
  HelpDialog* dlg = new HelpDialog("qrc:/help/ProfileEditDialog.html");
  if(dlg != NULL) {
    dlg->exec();
    delete dlg;
    dlg = NULL;
  }
}

void ProfileEditDialog::addStyleClicked() {
  //QMessageBox::information(this, _windowTitle, "Not implemented yet.", QMessageBox::Ok);
  if(_confMgr == NULL)
    return;
  if(_styleTable == NULL)
    return;

  StyleEditDialog* dlg = new StyleEditDialog(NULL, _confMgr, this);
  if(dlg != NULL) {
    if(dlg->exec() == QDialog::Accepted) {
      // put the new style into the list
      crawly_profile_style tmpStyle = dlg->getStyleData();

      if(_numStyles >= _maxNumStyles)
	growStyleList();

      int styleIdx = _numStyles;
      _numStyles++;
      _styleList[styleIdx] = tmpStyle;
      updateStyleRow(styleIdx);
      _styleTable->resizeColumnsToContents();
    }
    delete dlg;
    dlg = NULL;
  }
}
void ProfileEditDialog::editStyleClicked() {
  //QMessageBox::information(this, _windowTitle, "Not implemented yet.", QMessageBox::Ok);
  if(_confMgr == NULL)
    return;
  if(_styleTable == NULL)
    return;
  if(_styleList == NULL)
    return;

  int styleIdx = _styleTable->currentRow();
  if(styleIdx < 0 || styleIdx >= _numStyles)
    return;

  StyleEditDialog* dlg = new StyleEditDialog(&_styleList[styleIdx], _confMgr);
  if(dlg != NULL) {
    if(dlg->exec() == QDialog::Accepted) {
      // put the edited style back into the list
      crawly_profile_style tmpStyle = dlg->getStyleData();

      _styleList[styleIdx] = tmpStyle;
      updateStyleRow(styleIdx);
      _styleTable->resizeColumnsToContents();
    }
    delete dlg;
    dlg = NULL;
  }
}
void ProfileEditDialog::copyStyleClicked() {
  //QMessageBox::information(this, _windowTitle, "Not implemented yet.", QMessageBox::Ok);
  if(_confMgr == NULL)
    return;
  if(_styleTable == NULL)
    return;
  if(_styleList == NULL)
    return;

  int styleIdx = _styleTable->currentRow();
  if(styleIdx < 0 || styleIdx >= _numStyles)
    return;

  // copy the selected style and append to the list.
  if(_numStyles == _maxNumStyles)
    growStyleList();
  
  _styleList[_numStyles] = _styleList[styleIdx];
  updateStyleRow(_numStyles);
  _numStyles++;
}
void ProfileEditDialog::deleteStyleClicked() {
  //QMessageBox::information(this, _windowTitle, "Not implemented yet.", QMessageBox::Ok);
  if(_confMgr == NULL)
    return;
  if(_styleTable == NULL)
    return;
  if(_styleList == NULL)
    return;

  int styleIdx = _styleTable->currentRow();
  if(styleIdx < 0 || styleIdx >= _numStyles)
    return;

  // remove the selected style and move the last style in the list
  // in its place
  _styleList[styleIdx] = _styleList[_numStyles-1];
  _styleTable->removeRow(styleIdx);
  //updateStyleRow(styleIdx);
  //updateStyleRow(_numStyles-1);
  _numStyles--;
}

void ProfileEditDialog::cellDoubleClicked(int row, int column) {
  if(_styleTable == NULL)
    return;
  // select the whole row
  _styleTable->selectRow(row);

  // on double click, edit
  editStyleClicked();
}
void ProfileEditDialog::cellSelected(int row, int column) {
  if(_styleTable == NULL)
    return;
  // select the whole row
  _styleTable->selectRow(row);
}

/**
 * Updates the style table data for the specified style index.
 */
void ProfileEditDialog::updateStyleRow(int styleIdx) {
  if(_styleTable == NULL)
    return;
  if(_styleList == NULL)
    return;
  if(styleIdx >= _numStyles)
    return;

  int rowCount = _styleTable->rowCount();
  while(rowCount <= styleIdx) {
    _styleTable->insertRow(rowCount);
    rowCount++;
  }

  QTableWidgetItem* tmpItem = NULL;
  int col = 0;

  // NOTE: Order must match the order in setupStyleTableWidget()

  // palette
  tmpItem = new QTableWidgetItem(_styleList[styleIdx].pal);
  if(tmpItem != NULL) {
    tmpItem->setToolTip(_tableToolTips[col]);
    tmpItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    _styleTable->setItem(styleIdx, col, tmpItem);
  } else {
    QMessageBox::information(this, _windowTitle, "NULL!", QMessageBox::Ok);    
  }
  tmpItem = NULL;
  col++;

  // min length
  tmpItem = 
    new QTableWidgetItem(QString::number(_styleList[styleIdx].minLength));
  if(tmpItem != NULL) {
    tmpItem->setToolTip(_tableToolTips[col]);
    tmpItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    _styleTable->setItem(styleIdx, col, tmpItem);
  }
  tmpItem = NULL;
  col++;

  // max length
  tmpItem = 
    new QTableWidgetItem(QString::number(_styleList[styleIdx].maxLength));
  if(tmpItem != NULL) {
    tmpItem->setToolTip(_tableToolTips[col]);
    tmpItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    _styleTable->setItem(styleIdx, col, tmpItem);
  }
  tmpItem = NULL;
  col++;

  // thickness
  tmpItem = 
    new QTableWidgetItem(QString::number(_styleList[styleIdx].thickness));
  if(tmpItem != NULL) {
    tmpItem->setToolTip(_tableToolTips[col]);
    tmpItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    _styleTable->setItem(styleIdx, col, tmpItem);
  }
  tmpItem = NULL;
  col++;

  // min speed
  tmpItem = 
    new QTableWidgetItem(QString::number(_styleList[styleIdx].minSpriteSpeed));
  if(tmpItem != NULL) {
    tmpItem->setToolTip(_tableToolTips[col]);
    tmpItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    _styleTable->setItem(styleIdx, col, tmpItem);
  }
  tmpItem = NULL;
  col++;

  // max speed
  tmpItem = 
    new QTableWidgetItem(QString::number(_styleList[styleIdx].maxSpriteSpeed));
  if(tmpItem != NULL) {
    tmpItem->setToolTip(_tableToolTips[col]);
    tmpItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    _styleTable->setItem(styleIdx, col, tmpItem);
  }
  tmpItem = NULL;
  col++;

  // palette speed
  tmpItem = 
    new QTableWidgetItem(QString::number(_styleList[styleIdx].palSpeed));
  if(tmpItem != NULL) {
    tmpItem->setToolTip(_tableToolTips[col]);
    tmpItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    _styleTable->setItem(styleIdx, col, tmpItem);
  }
  tmpItem = NULL;
  col++;

  // constant color
  tmpItem = new QTableWidgetItem();  
  if(tmpItem != NULL) {
    tmpItem->setCheckState(boolToCheckState(_styleList[styleIdx].bHeadConstantColor));
    tmpItem->setToolTip(_tableToolTips[col]);
    tmpItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    _styleTable->setItem(styleIdx, col, tmpItem);
  }
  tmpItem = NULL;
  col++;

  // random color
  tmpItem = new QTableWidgetItem();  
  if(tmpItem != NULL) {
    tmpItem->setCheckState(boolToCheckState(_styleList[styleIdx].bHeadRandomColor));
    tmpItem->setToolTip(_tableToolTips[col]);
    tmpItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    _styleTable->setItem(styleIdx, col, tmpItem);
  }
  tmpItem = NULL;
  col++;
}

/**
 * Grows the list of crawly styles.
 */
void ProfileEditDialog::growStyleList(int size) {
  if(_styleList == NULL) {
    _maxNumStyles = size;
    _styleList = new crawly_profile_style[_maxNumStyles];
    if(_styleList == NULL) {
      _maxNumStyles = 0;
      return;
    }
    _numStyles = 0;
  } else {
    // create a temp list and copy
    crawly_profile_style* tmpList=new crawly_profile_style[_maxNumStyles+size];
    if(tmpList == NULL) {
      return;
    }
    for(int i=0;i<_numStyles;i++) {
      tmpList[i] = _styleList[i];
    }
    delete [] _styleList;
    _styleList = tmpList;
    tmpList = NULL;
    _maxNumStyles += size;
  }

}

/**
 * Sets up the style table widget.
 */
void ProfileEditDialog::setupStyleTableWidget(CrawliesBackgroundProfile* profile) {

  _styleTable = new QTableWidget(this);
  if(_styleTable == NULL)
    return;

  _styleTable->setSortingEnabled(false);
  _styleTable->setColumnCount(9);

  int col = 0; //!< This'll make it easier to modify the item order.
  QTableWidgetItem* tmpItem = NULL;

  // palette
  tmpItem = new QTableWidgetItem("Palette");
  _tableToolTips[col] = 
    tr("The name of the palette for this style to use.");
  if(tmpItem != NULL) {
    tmpItem->setToolTip(_tableToolTips[col]);
    tmpItem->setFlags(Qt::ItemIsEnabled);
    _styleTable->setHorizontalHeaderItem(col, tmpItem);
  }
  col++;

  // min length
  tmpItem = new QTableWidgetItem("Min Length");
  _tableToolTips[col] = 
    tr("Crawly min length or -1 to use the palette length.");
  if(tmpItem != NULL) {
    tmpItem->setToolTip(_tableToolTips[col]);
    tmpItem->setFlags(Qt::ItemIsEnabled);
    _styleTable->setHorizontalHeaderItem(col, tmpItem);
  }
  col++;

  // max length
  tmpItem = new QTableWidgetItem("Max Length");
  _tableToolTips[col] = 
    tr("Crawly max length or -1 to use the palette length.");
  if(tmpItem != NULL) {
    tmpItem->setToolTip(_tableToolTips[col]);
    tmpItem->setFlags(Qt::ItemIsEnabled);
    _styleTable->setHorizontalHeaderItem(col, tmpItem);
  }
  col++;

  // thickness
  tmpItem = new QTableWidgetItem("Thickness");
  _tableToolTips[col] = 
    tr("Crawly thickness.");
  if(tmpItem != NULL) {
    tmpItem->setToolTip(_tableToolTips[col]);
    tmpItem->setFlags(Qt::ItemIsEnabled);
    _styleTable->setHorizontalHeaderItem(col, tmpItem);
  }
  col++;

  // min speed
  tmpItem = new QTableWidgetItem("Min Speed");
  _tableToolTips[col] = 
    tr("Crawly min movement speed.");
  if(tmpItem != NULL) {
    tmpItem->setToolTip(_tableToolTips[col]);
    tmpItem->setFlags(Qt::ItemIsEnabled);
    _styleTable->setHorizontalHeaderItem(col, tmpItem);
  }
  col++;

  // max speed
  tmpItem = new QTableWidgetItem("Max Speed");
  _tableToolTips[col] = 
    tr("Crawly max movement speed.");
  if(tmpItem != NULL) {
    tmpItem->setToolTip(_tableToolTips[col]);
    tmpItem->setFlags(Qt::ItemIsEnabled);
    _styleTable->setHorizontalHeaderItem(col, tmpItem);
  }
  col++;

  // palette speed
  tmpItem = new QTableWidgetItem("Palette Speed");
  _tableToolTips[col] = 
    tr("Palette rotational speed.");
  if(tmpItem != NULL) {
    tmpItem->setToolTip(_tableToolTips[col]);
    tmpItem->setFlags(Qt::ItemIsEnabled);
    _styleTable->setHorizontalHeaderItem(col, tmpItem);
  }
  col++;

  // constant color
  tmpItem = new QTableWidgetItem("Constant Color");
  _tableToolTips[col] = 
    tr("Whether the Crawly head keeps the same color or not.");
  if(tmpItem != NULL) {
    tmpItem->setToolTip(_tableToolTips[col]);
    tmpItem->setFlags(Qt::ItemIsEnabled);
    _styleTable->setHorizontalHeaderItem(col, tmpItem);
  }
  col++;

  // random color
  tmpItem = new QTableWidgetItem("Random Color");
  _tableToolTips[col] = 
    tr("Whether the Crawly head starting color is random or not.");
  if(tmpItem != NULL) {
    tmpItem->setToolTip(_tableToolTips[col]);
    tmpItem->setFlags(Qt::ItemIsEnabled);
    _styleTable->setHorizontalHeaderItem(col, tmpItem);
  }
  col++;

  _styleTable->resizeColumnsToContents();

  // set up slots/signals so that whenever a cell is clicked, it
  // selects the whole row.
  QObject::connect(_styleTable, SIGNAL(cellClicked(int,int)), this,
		   SLOT(cellSelected(int,int)));
  QObject::connect(_styleTable, SIGNAL(cellDoubleClicked(int,int)), this,
		   SLOT(cellDoubleClicked(int,int)));

  // populate style list
  if(profile == NULL)
    return;

  // grow the list
  int numStyles = profile->getNumStyles();
  growStyleList(numStyles);
  _numStyles = numStyles;

  // now load values
  int i;
  for(i=0; i<_numStyles;i++) {
    _styleList[i] = profile->getStyle(i);
  }

  // fill in table
  for(i=0; i<_numStyles;i++) {
    updateStyleRow(i);
  }
  _styleTable->resizeColumnsToContents();
}
