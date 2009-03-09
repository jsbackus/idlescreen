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

// debug
//#include <iostream>
//using namespace std;
//end debug

#include <QMessageBox>
#include <QInputDialog>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QGroupBox>
#include <QFileDialog>
#include <QDir>
#include <QImageWriter>
#include <QByteArray>
#include <QList>
#include <QStringList>

#include "IndexedPalette/IndexedPaletteProfile.h"
#include "IndexedPalette/IndexedPaletteDialog.h"
#include "utility/HelpDialog.h"
#include "utility/ImportExportDialog.h"

#include "PlasmaGeneratorDialog.h"
#include "PlasmaGeneratorPaletteEditor.h"

PlasmaGeneratorDialog::PlasmaGeneratorDialog(ConfigManager* manager, QWidget* parent, Qt::WindowFlags f) {
  _windowTitle = "Plasma Generation Utility";
  setWindowTitle(_windowTitle);
  QIcon icon(":/app_icon.png");
  setWindowIcon(icon);

  _coarsenessAdjust = 100.0;

  // nullify pointers
  _fileEdit = NULL;
  _nameBox = NULL;
  _palName = NULL;
  _xDimBox = NULL;
  _yDimBox = NULL;
  _tileVertBox = NULL;
  _tileHorizBox = NULL;
  _coarsenessBox = NULL;
  _clampColorBox = NULL;
  _alwaysBox = NULL;
  _neverBox = NULL;
  _askBox = NULL;
  _previewScrollWidget = NULL;
  _previewLabel = NULL;
  _previewPixmap = NULL;
  _prgDialog = NULL;
  _gplDialog = NULL;
  _plasma = NULL;

  // set up
  _manager = manager;
  _gplDialog = new GPLDialog(_manager->getGPLAccepted());
  if(_gplDialog == NULL) {
    return;
  }
  _prgDialog = new QProgressDialog(tr("Plasma Generation Progress"), 0, 0, 100);
  if(_prgDialog == NULL) {
    return;
  }

  // lock out while setting up.
  _bDontUpdateDisplay = true;


  QWidget* tmpWidget = NULL;
  QPushButton* tmpButton = NULL;
  QLabel* tmpLabel = NULL;
  QVBoxLayout* tmpV = NULL;
  QHBoxLayout* tmpH = NULL;
  QHBoxLayout* tmpH2 = NULL;
  QHBoxLayout* tmpH3 = NULL;
  QGroupBox* tmpGroup = NULL;
  QString tmpTip;

  QVBoxLayout* mainLayout = new QVBoxLayout();
  if(mainLayout == NULL) {
    return;
  }
  
  // set up name box region
  _nameBox = new QComboBox();
  if(_nameBox == NULL) {
    return;
  }
  _nameBox->addItems(_manager->getAvailableProfiles());
  _nameBox->setEditable(false);
  QObject::connect(_nameBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(profileChanged(QString)));

  tmpH = new QHBoxLayout();
  if(tmpH == NULL) {
    return;
  }
  tmpTip = QString(tr("Name of the currently selected profile."));
  tmpLabel = new QLabel("Profile Name: ");
  tmpLabel->setToolTip(tmpTip);
  tmpH->addWidget(tmpLabel);
  _nameBox->setToolTip(tmpTip);
  tmpH->addWidget(_nameBox);
  tmpLabel = NULL;

  tmpH->addStretch(0);

  _defaultButton = new QPushButton(tr("Set Default"));
  _defaultButton->setDefault(false);
  _defaultButton->setAutoDefault(false);
  if(_defaultButton == NULL) {
    return;
  }
  _defaultButton->setToolTip(tr("Sets this profile as the default profile."));
  QObject::connect(_defaultButton, SIGNAL(clicked(bool)), this, SLOT(setDefaultClicked(bool)));
  tmpH->addWidget(_defaultButton);
  
  tmpWidget = new QWidget();
  tmpWidget->setLayout(tmpH);
  mainLayout->addWidget(tmpWidget);
  tmpWidget = NULL;
  tmpH = NULL;

  // profile management region
  tmpH = new QHBoxLayout();
  if(tmpH == NULL) {
    return;
  }

  tmpButton = new QPushButton(tr("New"));
  if(tmpButton == NULL) {
    return;
  }
  tmpButton->setToolTip(tr("Creates a new profile."));
  QObject::connect(tmpButton, SIGNAL(clicked(bool)), this, SLOT(newProfileClicked(bool)));
  tmpH->addWidget(tmpButton);
  tmpButton = NULL;
  
  tmpButton = new QPushButton(tr("Rename"));
  if(tmpButton == NULL) {
    return;
  }
  tmpButton->setToolTip(tr("Renames this profile."));
  QObject::connect(tmpButton, SIGNAL(clicked(bool)), this, SLOT(renameProfileClicked(bool)));
  tmpH->addWidget(tmpButton);
  tmpButton = NULL;
  
  tmpButton = new QPushButton(tr("Copy"));
  if(tmpButton == NULL) {
    return;
  }
  tmpButton->setToolTip(tr("Copies the currently selected profile."));
  QObject::connect(tmpButton, SIGNAL(clicked(bool)), this, SLOT(copyProfileClicked(bool)));
  tmpH->addWidget(tmpButton);
  tmpButton = NULL;
  
  tmpButton = new QPushButton(tr("Delete"));
  if(tmpButton == NULL) {
    return;
  }
  tmpButton->setToolTip(tr("Deletes this profile."));
  QObject::connect(tmpButton, SIGNAL(clicked(bool)), this, SLOT(deleteProfileClicked(bool)));
  tmpH->addWidget(tmpButton);
  tmpButton = NULL;
  
  tmpWidget = new QWidget();
  tmpWidget->setLayout(tmpH);
  mainLayout->addWidget(tmpWidget);
  tmpWidget = NULL;
  tmpH = NULL;

  // palette selection & import/export region
  _palName = new QComboBox();
  if(_palName == NULL) {
    return;
  }
  _palName->addItems(_manager->getPaletteNames());
  _palName->setEditable(false);

  tmpH = new QHBoxLayout();
  if(tmpH == NULL) {
    return;
  }
  tmpTip = QString(tr("Name of the currently selected palette."));
  tmpLabel = new QLabel("Palette: ");
  tmpLabel->setToolTip(tmpTip);
  tmpH->addWidget(tmpLabel);
  _palName->setToolTip(tmpTip);
  tmpH->addWidget(_palName);
  tmpLabel = NULL;
  
  tmpButton = new QPushButton(tr("Manage Palettes"));
  if(tmpButton == NULL) {
    return;
  }
  tmpButton->setToolTip(tr("Click here to add, edit, or delete palettes."));
  QObject::connect(tmpButton, SIGNAL(clicked(bool)), this, SLOT(managePalettesClicked(bool)));
  tmpH->addWidget(tmpButton);
  tmpButton = NULL;

  tmpH->addStretch(0);
    
  tmpButton = new QPushButton(tr("Import"));
  if(tmpButton == NULL) {
    return;
  }
  tmpButton->setToolTip(tr("Click here to import palettes and profiles."));
  QObject::connect(tmpButton, SIGNAL(clicked(bool)), this, SLOT(importClicked(bool)));
  tmpH->addWidget(tmpButton);
  tmpButton = NULL;
    
  tmpButton = new QPushButton(tr("Export"));
  if(tmpButton == NULL) {
    return;
  }
  tmpButton->setToolTip(tr("Click here to export palettes and profiles."));
  QObject::connect(tmpButton, SIGNAL(clicked(bool)), this, SLOT(exportClicked(bool)));
  tmpH->addWidget(tmpButton);
  tmpButton = NULL;
    
  tmpWidget = new QWidget();
  tmpWidget->setLayout(tmpH);
  mainLayout->addWidget(tmpWidget);
  tmpWidget = NULL;
  tmpH = NULL;

  // dimensions and tiling controls
  tmpH = new QHBoxLayout();
  if(tmpH == NULL) {
    return;
  }

  // dimensions
  tmpGroup = new QGroupBox("Dimensions");
  if(tmpGroup == NULL) {
    return;
  }
  tmpTip = QString(tr("Plasma dimensions"));
  tmpGroup->setToolTip(tmpTip);

  tmpH3 = new QHBoxLayout();
  if(tmpH3 == NULL) {
    return;
  }
  
  tmpH2 = new QHBoxLayout();
  if(tmpH2 == NULL) {
    return;
  }
  tmpLabel = new QLabel(tr("Width"));
  if(tmpLabel == NULL) {
    return;
  }
  tmpLabel->setToolTip(tmpTip);
  tmpH2->addWidget(tmpLabel);

  _xDimBox = new QSpinBox();
  if(_xDimBox == NULL) {
    return;
  }
  _xDimBox->setToolTip(tmpTip);
  _xDimBox->setMinimum(16);
  _xDimBox->setMaximum(1024*1024);
  _xDimBox->setSingleStep(1);
  tmpH2->addWidget(_xDimBox);

  tmpWidget = new QWidget();
  tmpWidget->setLayout(tmpH2);
  tmpH3->addWidget(tmpWidget);
  tmpWidget = NULL;
  tmpH2 = NULL;

  tmpH2 = new QHBoxLayout();
  if(tmpH2 == NULL) {
    return;
  }
  tmpLabel = new QLabel(tr("Height"));
  if(tmpLabel == NULL) {
    return;
  }
  tmpLabel->setToolTip(tmpTip);
  tmpH2->addWidget(tmpLabel);

  _yDimBox = new QSpinBox();
  if(_yDimBox == NULL) {
    return;
  }
  _yDimBox->setToolTip(tmpTip);
  _yDimBox->setMinimum(16);
  _yDimBox->setMaximum(1024*1024);
  _yDimBox->setSingleStep(1);
  tmpH2->addWidget(_yDimBox);

  tmpWidget = new QWidget();
  tmpWidget->setLayout(tmpH2);
  tmpH3->addWidget(tmpWidget);
  tmpWidget = NULL;
  tmpH2 = NULL;

  tmpGroup->setLayout(tmpH3);
  tmpH3 = NULL;
  tmpH->addWidget(tmpGroup);
  tmpGroup = NULL;
  
  tmpH->addStretch(0);

  tmpGroup = new QGroupBox("Tiling");
  if(tmpGroup == NULL) {
    return;
  }
  tmpTip = QString(tr("Generate a plasma that can be tiled on the horizontal and/or vertical axes"));
  tmpGroup->setToolTip(tmpTip);

  tmpH3 = new QHBoxLayout();
  if(tmpH3 == NULL) {
    return;
  }
  
  _tileVertBox = new QCheckBox(tr("Vertical"));
  if(_tileVertBox == NULL) {
    return;
  }
  _tileVertBox->setToolTip(tmpTip);
  _tileVertBox->setTristate(false);
  tmpH3->addWidget(_tileVertBox);

  _tileHorizBox = new QCheckBox(tr("Horizontal"));
  if(_tileHorizBox == NULL) {
    return;
  }
  _tileHorizBox->setToolTip(tmpTip);
  _tileHorizBox->setTristate(false);
  tmpH3->addWidget(_tileHorizBox);

  tmpGroup->setLayout(tmpH3);
  tmpH3 = NULL;
  tmpH->addWidget(tmpGroup);
  tmpGroup = NULL;

  tmpWidget = new QWidget();
  tmpWidget->setLayout(tmpH);
  mainLayout->addWidget(tmpWidget);
  tmpWidget = NULL;
  tmpH = NULL;

  // coarseness adjust and clamp color box
  tmpGroup = new QGroupBox(tr("Field Generation"));
  if(tmpGroup == NULL)
    return;

  tmpH = new QHBoxLayout();
  if(tmpH == NULL) {
    return;
  }
    
  tmpLabel = new QLabel(tr("Less Volatile"));
  tmpTip = tr("This quantity determines how coarse or volatile the fractal is.  More volatile, the more variation.");
  tmpLabel->setToolTip(tmpTip);
  tmpH->addWidget(tmpLabel);

  _coarsenessBox = new QSlider(Qt::Horizontal);
  _coarsenessBox->setTickInterval(0.10*_coarsenessAdjust);
  //_coarsenessBox->setTickPosition(QSlider::TicksBelow);
  _coarsenessBox->setMinimum(0.01*_coarsenessAdjust);
  _coarsenessBox->setMaximum(6.00*_coarsenessAdjust);
  _coarsenessBox->setToolTip(tmpTip);
  tmpH->addWidget(_coarsenessBox);
  
  tmpLabel = new QLabel(tr("More Volatile"));
  tmpLabel->setToolTip(tmpTip);
  tmpH->addWidget(tmpLabel);
	
  tmpH->addStretch(0);

  _clampColorBox = new QCheckBox(tr("Clamp color on overflow"));
  if(_clampColorBox == NULL) {
    return;
  }
  _clampColorBox->setToolTip(tr("When the plasma algorithm generates an invalid color index, it will clamp")+"\n"
			     +tr("to the nearest valid color if checked, or wrap around if unchecked."));
  tmpH->addWidget(_clampColorBox);

  tmpGroup->setLayout(tmpH);
  mainLayout->addWidget(tmpGroup);
  tmpH = NULL;
  tmpGroup = NULL;

  // preview region
  _previewScrollWidget = new QScrollArea();
  if(_previewScrollWidget == NULL) {
    return;
  }
  _previewScrollWidget->setToolTip(tr("Plasma preview area."));
  _previewLabel = new QLabel();
  if(_previewLabel == NULL) {
    return;
  }
  _previewLabel->setPixmap(_previewPixmap);
  _previewLabel->setToolTip(_previewScrollWidget->toolTip());
  _previewScrollWidget->setWidget(_previewLabel);
  mainLayout->addWidget(_previewScrollWidget);

  // overwrite policy & generate region
  tmpH2 = new QHBoxLayout();
  if(tmpH2 == NULL) {
    return;
  }

  tmpGroup = new QGroupBox("Overwrite Policy");
  if(tmpGroup == NULL) {
    return;
  }
  tmpTip = QString(tr("File overwrite policy.  If a file with the name below already exists, should PlasmaGenerator replace the file: always, never, or ask.  Note that ask defaults to never in command-line mode."));
  tmpGroup->setToolTip(tmpTip);

  tmpH = new QHBoxLayout();
  if(tmpH == NULL) {
    return;
  }

  _alwaysBox = new QRadioButton(tr("Always"));
  if(_alwaysBox == NULL) {
    return;
  }
  tmpH->addWidget(_alwaysBox);
  
  _neverBox = new QRadioButton(tr("Never"));
  if(_neverBox == NULL) {
    return;
  }
  tmpH->addWidget(_neverBox);
  
  _askBox = new QRadioButton(tr("Ask"));
  if(_askBox == NULL) {
    return;
  }
  tmpH->addWidget(_askBox);

  tmpGroup->setLayout(tmpH);
  tmpH2->addWidget(tmpGroup);
  tmpGroup = NULL;
  tmpH = NULL;

  tmpH2->addStretch(0);

  tmpButton = new QPushButton(tr("Generate"));
  if(tmpButton == NULL) {
    return;
  }
  tmpButton->setToolTip(tr("Generates a new plasma"));
  QObject::connect(tmpButton, SIGNAL(clicked(bool)), this, SLOT(generateClicked(bool)));
  tmpH2->addWidget(tmpButton);
  tmpButton = NULL;

  tmpWidget = new QWidget();
  tmpWidget->setLayout(tmpH2);
  mainLayout->addWidget(tmpWidget);
  tmpWidget = NULL;
  tmpH2 = NULL;

  // save to file region
  tmpH = new QHBoxLayout();
  if(tmpH == NULL) {
    return;
  }

  _fileEdit = new QLineEdit();
  if(_fileEdit == NULL) {
    return;
  }
  _fileEdit->setToolTip(tr("Filename to save plasma image to."));
  _fileEdit->setReadOnly(false);
  _fileEdit->setDragEnabled(true);
  tmpH->addWidget(_fileEdit);

  tmpButton = new QPushButton(tr("Browse"));
  if(tmpButton == NULL) {
    return;
  }
  tmpButton->setToolTip(tr("Browse for file to export image to."));
  QObject::connect(tmpButton, SIGNAL(clicked(bool)), this, SLOT(browseClicked(bool)));
  tmpH->addWidget(tmpButton);
  tmpButton = NULL;

  tmpButton = new QPushButton(tr("&Save"));
  if(tmpButton == NULL) {
    return;
  }
  tmpButton->setToolTip(tr("Saves the current plasma image to disk."));
  QObject::connect(tmpButton, SIGNAL(clicked(bool)), this, SLOT(saveClicked(bool)));
  tmpH->addWidget(tmpButton);
  tmpButton = NULL;

  tmpWidget = new QWidget();
  tmpWidget->setLayout(tmpH);
  mainLayout->addWidget(tmpWidget);
  tmpWidget = NULL;
  tmpH = NULL;

  // buttons at bottom
  tmpH = new QHBoxLayout();
  if(tmpH == NULL) {
    return;
  }
  tmpButton = new QPushButton(tr("&Help"));
  if(tmpButton == NULL) {
    return;
  }
  QObject::connect(tmpButton, SIGNAL(clicked(bool)), this, SLOT(helpClicked(bool)));
  tmpH->addWidget(tmpButton);
  tmpButton = NULL;

  tmpH->addStretch(0);
  
  tmpButton = new QPushButton(tr("&Apply"));
  if(tmpButton == NULL) {
    return;
  }
  QObject::connect(tmpButton, SIGNAL(clicked(bool)), this, SLOT(applyClicked(bool)));
  tmpH->addWidget(tmpButton);
  tmpButton = NULL;
  
  tmpButton = new QPushButton(tr("&OK"));
  if(tmpButton == NULL) {
    return;
  }
  QObject::connect(tmpButton, SIGNAL(clicked(bool)), this, SLOT(okClicked(bool)));
  tmpH->addWidget(tmpButton);
  tmpButton->setDefault(true);
  tmpButton->setAutoDefault(true);
  tmpButton = NULL;
  
  tmpButton = new QPushButton(tr("&Cancel"));
  if(tmpButton == NULL) {
    return;
  }
  QObject::connect(tmpButton, SIGNAL(clicked(bool)), this, SLOT(cancelClicked(bool)));
  tmpH->addWidget(tmpButton);
  tmpButton = NULL;
  
  tmpWidget = new QWidget();
  tmpWidget->setLayout(tmpH);
  mainLayout->addWidget(tmpWidget);
  tmpWidget = NULL;
  tmpH = NULL;

  setLayout(mainLayout);

  // generate the filter list of supported file formats 
  QList<QByteArray> list = QImageWriter::supportedImageFormats();
  QStringList uniqueList;
  QString ext;
  int i;
  for(i=0; i<list.size();i++) {
    ext = list.at(i);
    if(!uniqueList.contains(QString(ext), Qt::CaseInsensitive)) {
      uniqueList.append(ext.toUpper());
    }
  }
  int listSize = uniqueList.size();
  for(i=0; i<listSize;i++) {
    ext = uniqueList.at(i);
    _fileFormatFilter.append(ext);
    _fileFormatFilter.append(" Files (*.");
    _fileFormatFilter.append(ext.toLower());
    _fileFormatFilter.append(" *.");
    _fileFormatFilter.append(ext);
    _fileFormatFilter.append(")");
    if(i < listSize-1) {
      _fileFormatFilter.append(";;");
    }
  }

  // choose default filter
  if(uniqueList.contains(QString("JPG"))) {
    _selFilter = QString("JPG Files (*.jpg *.JPG)");
  } else if(uniqueList.contains(QString("PNG"))) {
    _selFilter = QString("PNG Files (*.png *.PNG)");
  } else if(uniqueList.contains(QString("BMP"))) {
    _selFilter = QString("BMP Files (*.bmp *.BMP)");
  }

  // last thing is to update the data with the default profile.
  _bDontUpdateDisplay = false;
  _nameBox->setCurrentIndex(_nameBox->findText(_manager->getDefaultProfile()));
  updateDisplay();
}

PlasmaGeneratorDialog::~PlasmaGeneratorDialog() {
  _manager = NULL;

  if(_gplDialog != NULL) {
    delete _gplDialog;
    _gplDialog = NULL;
  }
  // widgets should get deleted automatically when the main layout gets
  // deleted...
  /*
    if(_fileEdit != NULL) {
    delete _fileEdit;
    _fileEdit = NULL;
    }
    if(_nameBox != NULL) {
    delete _nameBox;
    _nameBox = NULL;
    }
    if(_palName != NULL) {
    delete _palName;
    _palName = NULL;
    }
    if(_xDimBox != NULL) {
    delete _xDimBox;
    _xDimBox = NULL;
    }
    if(_yDimBox != NULL) {
    delete _yDimBox;
    _yDimBox = NULL;
    }
    if(_tileVertBox != NULL) {
    delete _tileVertBox;
    _tileVertBox = NULL;
    }
    if(_tileHorizBox != NULL) {
    delete _tileHorizBox;
    _tileHorizBox = NULL;
    }
    if(_alwaysBox != NULL) {
    delete _alwaysBox;
    _alwaysBox = NULL;
    }
    if(_askBox != NULL) {
    delete _askBox;
    _askBox = NULL;
    }
    if(_neverBox != NULL) {
    delete _neverBox;
    _neverBox = NULL;
    }
    if(_previewScrollWidget != NULL) {
    delete _previewScrollWidget;
    _previewScrollWidget = NULL;
    }
    if(_previewLabel != NULL) {
    delete _previewLabel;
    _previewLabel = NULL;
    }
    if(_previewPixmap != NULL) {
    delete _previewPixmap;
    _previewPixmap = NULL;
    }
  */
  if(_prgDialog != NULL) {
    delete _prgDialog;
    _prgDialog = NULL;
  }
  if(_plasma != NULL) {
    delete _plasma;
    _plasma = NULL;
  }
}

void PlasmaGeneratorDialog::applyClicked(bool checked) {
  // update and save
  updateProfile();
  _manager->save();
}

void PlasmaGeneratorDialog::okClicked(bool checked) {
  // update profile in config manager and save
  updateProfile();
  _manager->save();
  accept();
}

void PlasmaGeneratorDialog::cancelClicked(bool checked) {
  reject();
}

void PlasmaGeneratorDialog::helpClicked(bool checked) {
  HelpDialog* dlg = new HelpDialog("qrc:/help/index.html");
  if(dlg != NULL) {
    dlg->exec();
    delete dlg;
    dlg = NULL;
  }
}

void PlasmaGeneratorDialog::managePalettesClicked(bool checked) {
  PlasmaGeneratorPaletteEditor* dlg = 
    new PlasmaGeneratorPaletteEditor(_manager, this);
  dlg->exec();

  //clean up
  delete dlg;
  dlg = NULL;

  // refresh names
  _palName->clear();
  _palName->addItems(_manager->getPaletteNames());
  updateDisplay();
}

void PlasmaGeneratorDialog::setDefaultClicked(bool checked) {
  _bDefault = true;
  _defaultButton->setEnabled(false);
}

void PlasmaGeneratorDialog::newProfileClicked(bool checked) {
  // prompt for name from user
  bool bOk;
  bool bDone = false;
  QString tmpName;
  
  while(!bDone) {

    tmpName = QInputDialog::getText(this, tr("Create New Profile"), 
				    tr("Please enter a new profile name:"), 
				    QLineEdit::Normal, tr("New Profile"),
				    &bOk);
    if(!bOk) {
      return;
    }

    if(tmpName.isEmpty()) {
      QMessageBox::information(this, tr("Profile Name"), 
			       tr("Please enter a valid profile name."), 
			       QMessageBox::Ok);      
    } else if(_manager->doesProfileExist(tmpName)) {
      QMessageBox::information(this, tr("Profile Name"), 
			       tr("There is already a profile named '")+
			       tmpName+tr("'.  Please choose a new name."), 
			       QMessageBox::Ok);
    } else {
      bDone = true;
    }
  }

  // set defaults
  PlasmaGeneratorProfile profile;
  profile.setWidth(_manager->getDefaultWidth());
  profile.setHeight(_manager->getDefaultHeight());
  profile.setPaletteName(_manager->getDefaultPalette());
  profile.setName(tmpName);
  _manager->addProfile(profile);
  _nameBox->addItem(tmpName);
  _nameBox->setCurrentIndex(_nameBox->findText(tmpName));
}

void PlasmaGeneratorDialog::renameProfileClicked(bool checked) {
  // prompt for name from user
  bool bOk;
  bool bDone = false;
  QString tmpName;
  QString oldName = _currentProfile;
  
  updateProfile();

  while(!bDone) {

    tmpName = QInputDialog::getText(this, tr("Rename Profile"), 
				    tr("Please enter a new profile name:"), 
				    QLineEdit::Normal, _currentProfile,
				    &bOk);
    if(!bOk) {
      return;
    }

    if(tmpName.isEmpty()) {
      QMessageBox::information(this, tr("Profile Name"), 
			       tr("Please enter a valid profile name."), 
			       QMessageBox::Ok);      
    } else if(_manager->doesProfileExist(tmpName)) {
      QMessageBox::information(this, tr("Profile Name"), 
			       tr("There is already a profile named '")+
			       tmpName+tr("'.  Please choose a new name."), 
			       QMessageBox::Ok);
    } else {
      bDone = true;
    }
  }

  if(tmpName == _currentProfile) {
    return;
  }

  PlasmaGeneratorProfile profile = _manager->getProfile(oldName);
  profile.setName(tmpName);
  _manager->replaceProfile(oldName, profile);
  _currentProfile = tmpName;
  _nameBox->removeItem(_nameBox->findText(oldName));
  _nameBox->addItem(tmpName);
  _nameBox->setCurrentIndex(_nameBox->findText(tmpName));
}

void PlasmaGeneratorDialog::copyProfileClicked(bool checked) {
  // prompt for name from user
  bool bOk;
  bool bDone = false;
  QString tmpName;
  
  updateProfile();

  while(!bDone) {

    tmpName = QInputDialog::getText(this, tr("Copy Profile"), 
				    tr("Please enter a new profile name:"), 
				    QLineEdit::Normal, _currentProfile,
				    &bOk);
    if(!bOk) {
      return;
    }

    if(tmpName.isEmpty()) {
      QMessageBox::information(this, tr("Profile Name"), 
			       tr("Please enter a valid profile name."), 
			       QMessageBox::Ok);      
    } else if(_manager->doesProfileExist(tmpName)) {
      QMessageBox::information(this, tr("Profile Name"), 
			       tr("There is already a profile named '")+
			       tmpName+tr("'.  Please choose a new name."), 
			       QMessageBox::Ok);
    } else {
      bDone = true;
    }
  }

  PlasmaGeneratorProfile profile = _manager->getProfile(_currentProfile);
  profile.setName(tmpName);
  _manager->addProfile(profile);
  _nameBox->addItem(tmpName);
  _nameBox->setCurrentIndex(_nameBox->findText(tmpName));
}

void PlasmaGeneratorDialog::deleteProfileClicked(bool checked) {
  QString oldName = _currentProfile;
  _currentProfile = QString("");
  _manager->removeProfile(oldName);
  _nameBox->removeItem(_nameBox->findText(oldName));
  _nameBox->setCurrentIndex(0);
}

void PlasmaGeneratorDialog::profileChanged(const QString& newProfile) {
  updateDisplay();
}

void PlasmaGeneratorDialog::generateClicked(bool checked) {
  updateProfile();
  if(_plasma != NULL) {
    delete _plasma;
    _plasma = NULL;
  }
  
  // create a generator and connect it to the progress dialog
  _plasma = _manager->getGenerator(_currentProfile);
  if(_plasma == NULL) {
    return;
  }
  QObject::connect(_plasma, SIGNAL(progressUpdated(int)), _prgDialog, SLOT(setValue(int)));
  _prgDialog->show();
  if(!_plasma->genPlasma()) {
    _prgDialog->hide();
    // error!
    QMessageBox::critical(this, _windowTitle, tr("Error generating plasma!"), QMessageBox::Ok);
    delete _plasma;
    _plasma = NULL;
    return;
  }
  _prgDialog->hide();

  _previewPixmap = QPixmap::fromImage(_plasma->getPlasma());
  QString tmpTip = _previewScrollWidget->toolTip();
  //_previewPixmap.setToolTip(tmpTip);
  _previewLabel = new QLabel();
  if(_previewLabel == NULL) {
    QMessageBox::critical(this, _windowTitle, tr("Error generating plasma!"), QMessageBox::Ok);
    delete _plasma;
    _plasma = NULL;
    return;
  }
  _previewLabel->setPixmap(_previewPixmap);
  _previewScrollWidget->setWidget(_previewLabel);
}

void PlasmaGeneratorDialog::saveClicked(bool checked) {
  updateProfile();

  // make sure there is something in the filename field.
  QString filename = _fileEdit->text();
  if(filename.isEmpty()) {
    QMessageBox::warning(this, tr("Save Plasma"), tr("Please specify a target filename."), QMessageBox::Ok);  
    
    return;
  }

  // check to see if there is a current plasma object.  If not, generate one.
  if(_plasma == NULL) {
    generateClicked(false);
  }

  bool bOverwrite = false;
  // if the overwrite policy is ask, check to see if the file exists first.
  // otherwise, set bOverwrite to the appropriate value.
  switch(_manager->getProfile(_currentProfile).getOverwritePolicy()) {
  case ALWAYS:
    bOverwrite = true;
    break;
  case NEVER:
    bOverwrite = false;
    break;
  default:
    // check for file
    if(QFile(filename).exists()) {
      int ret = QMessageBox::question(this, tr("Save Plasma"), filename+
				      tr(" already exists.  Overwrite?"),
				      QMessageBox::Yes | QMessageBox::No |
				      QMessageBox::Cancel);
      switch(ret) {
      case QMessageBox::Yes:
	bOverwrite = true;
	break;
      case QMessageBox::No:
	bOverwrite = false;
	break;
      default:
	return;
      }
    } else {
      bOverwrite = false;
    }
  }

  // now attempt to save to the specified filename
  _plasma->savePlasma(filename, bOverwrite);
}

void PlasmaGeneratorDialog::browseClicked(bool checked) {
  // no need to confirm overwrite.  This will happen with the save button.
  QString path = QFileDialog::getSaveFileName(this, tr("Save Plasma to File"),
					      QDir::homePath(), 
					      _fileFormatFilter, &_selFilter,
					      QFileDialog::DontConfirmOverwrite
					      );
  if(!path.isEmpty()) {
    _fileEdit->setText(path);
  }
}

void PlasmaGeneratorDialog::importClicked(bool checked) {
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
  QStringList profiles = tmpMgr->getAvailableProfiles();
  dlg.addSubItems("Configuration Profiles", profiles, true);

  QStringList palettes = tmpMgr->getPaletteNames();
  dlg.addSubItems("Palettes", palettes, true);

  // show dialog and get lists of checked items
  if(dlg.exec() == QDialog::Rejected) {
    delete tmpMgr;
    tmpMgr = NULL;
    return;
  }
  profiles = dlg.getSubItems("Configuration Profiles", true);
  palettes = dlg.getSubItems("Palettes", true);

  // first, add in the palettes, since they don't
  // have any dependencies
  for(int i = 0; i<palettes.size(); i++) {
    IndexedPaletteProfile tmpPal = tmpMgr->getPaletteProfile(palettes.at(i));
    _manager->addPalette(tmpPal);
  }

  // now go through the PlasmaGeneratorProfiles, adding palettes it requires
  for(int i = 0; i<profiles.size(); i++) {
    PlasmaGeneratorProfile tmpProfile = tmpMgr->getProfile(profiles.at(i));
    // make sure the  profile doesn't exist, so that we don't
    // unnecessarily copy the palettes.  No need to check the
    // palettes, because the add routine should take care of it.
    if(!_manager->doesProfileExist(tmpProfile.getName())) {
      IndexedPaletteProfile tmpPal = tmpMgr->getPaletteProfile(tmpProfile.getPaletteName());
      _manager->addPalette(tmpPal);
      _manager->addProfile(tmpProfile);
    }
  }
  
  _bDontUpdateDisplay = true;
  _palName->clear();
  _palName->addItems(_manager->getPaletteNames());
  _nameBox->clear();
  _nameBox->addItems(_manager->getAvailableProfiles());
  _nameBox->setCurrentIndex(_nameBox->findText(_currentProfile));
  _bDontUpdateDisplay = false;
  updateDisplay();
}

void PlasmaGeneratorDialog::exportClicked(bool checked) {
  ImportExportDialog dlg(false);

  // populate dialog box
  QStringList profiles = _manager->getAvailableProfiles();
  dlg.addSubItems("Configuration Profiles", profiles, true);

  QStringList palettes = _manager->getPaletteNames();
  dlg.addSubItems("Palettes", palettes, true);

  // show dialog and get lists of checked items
  if(dlg.exec() == 0)
    return;
  profiles = dlg.getSubItems("Configuration Profiles", true);
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
  for(int i = 0; i<profiles.size(); i++) {
    PlasmaGeneratorProfile tmpProfile = _manager->getProfile(profiles.at(i));
    IndexedPaletteProfile tmpPal = _manager->getPaletteProfile(tmpProfile.getPaletteName());
    tmpMgr->addPalette(tmpPal);
    tmpMgr->addProfile(tmpProfile);
  }

  // save!
  tmpMgr->exportToFile(filename, false);
  delete tmpMgr;
  tmpMgr = NULL;
}

/**
 * Updates the profile info in the config manager.
 */
void PlasmaGeneratorDialog::updateProfile() {
  // sanity check
  if(_manager == NULL) {
    return;
  }
  if(_nameBox == NULL) {
    return;
  }
  if(_fileEdit == NULL) {
    return;
  }
  if(_palName == NULL) {
    return;
  }
  if(_xDimBox == NULL) {
    return;
  }
  if(_yDimBox == NULL) {
    return;
  }
  if(_tileVertBox == NULL) {
    return;
  }
  if(_tileHorizBox == NULL) {
    return;
  }
  if(_coarsenessBox == NULL) {
    return;
  }
  if(_clampColorBox == NULL) {
    return;
  }
  if(_alwaysBox == NULL) {
    return;
  }
  if(_askBox == NULL) {
    return;
  }
  if(_neverBox == NULL) {
    return;
  }

  // stuff data into a temporary profile;
  PlasmaGeneratorProfile profile;
  
  profile.setName(_currentProfile);
  profile.setPaletteName(_palName->currentText());
  profile.setHeight(_yDimBox->value());
  profile.setWidth(_xDimBox->value());
  profile.setCoarseness(((float)_coarsenessBox->value())/_coarsenessAdjust);
  profile.setClampColorIndex(_clampColorBox->isChecked());
  profile.setFileTarget(_fileEdit->text());
  if(_alwaysBox->isChecked()) {
    profile.setOverwritePolicy(ALWAYS);
  } else if(_neverBox->isChecked()) {
    profile.setOverwritePolicy(NEVER);
  } else {
    profile.setOverwritePolicy(ASK);
  }
  profile.setTileVertical(_tileVertBox->isChecked());
  profile.setTileHorizontal(_tileHorizBox->isChecked());

  _manager->replaceProfile(_currentProfile, profile);
  if(_bDefault) {
    _manager->setDefaultProfile(profile.getName());
  }
}

/**
 * Updates all widgets with the currently selected profile.
 */
void PlasmaGeneratorDialog::updateDisplay() {
  // we're locked out...
  if(_bDontUpdateDisplay) {
    return;
  }
  // sanity check
  if(_manager == NULL) {
    return;
  }
  if(_nameBox == NULL) {
    return;
  }
  if(_fileEdit == NULL) {
    return;
  }
  if(_palName == NULL) {
    return;
  }
  if(_xDimBox == NULL) {
    return;
  }
  if(_yDimBox == NULL) {
    return;
  }
  if(_tileVertBox == NULL) {
    return;
  }
  if(_tileHorizBox == NULL) {
    return;
  }
  if(_coarsenessBox == NULL) {
    return;
  }
  if(_clampColorBox == NULL) {
    return;
  }
  if(_alwaysBox == NULL) {
    return;
  }
  if(_askBox == NULL) {
    return;
  }
  if(_neverBox == NULL) {
    return;
  }
  if(_defaultButton == NULL) {
    return;
  }
  if(_previewScrollWidget == NULL) {
    return;
  }
  // check to see if profile has changed
  if( _currentProfile != _nameBox->currentText()) {
    updateProfile();
    _currentProfile = _nameBox->currentText();
  }

  _bDefault = (_manager->getDefaultProfile() == _currentProfile);
  
  // get a copy of the profile
  PlasmaGeneratorProfile profile;
  profile = _manager->getProfile(_currentProfile);

  // update fields
  _fileEdit->setText(profile.getFileTarget());
  _palName->setCurrentIndex(_palName->findText(profile.getPaletteName()));
  _xDimBox->setValue(profile.getWidth());
  _yDimBox->setValue(profile.getHeight());
  _tileVertBox->setChecked(profile.getTileVertical());
  _tileHorizBox->setChecked(profile.getTileHorizontal());
  _coarsenessBox->setValue(profile.getCoarseness()*_coarsenessAdjust);
  _clampColorBox->setChecked(profile.getClampColorIndex());
  switch(profile.getOverwritePolicy()) {
  case ALWAYS:
    _alwaysBox->setChecked(true);
    break;
  case NEVER:
    _neverBox->setChecked(true);
    break;
  default:
    _askBox->setChecked(true);
  }
  QLabel* tmpLabel = 
    new QLabel(tr("Press 'Generate' to generate a new plasma."));
  if(tmpLabel == NULL) {
    return;
  }
  _previewScrollWidget->setWidget(tmpLabel);
  tmpLabel = NULL;

  _defaultButton->setEnabled(!_bDefault);
}

/**
 * Displays the GPL license dialog.
 */
void PlasmaGeneratorDialog::showGPLDialog(void) {
  _gplDialog->exec();
  _manager->setGPLAccepted(_gplDialog->isAccepted());
  if(_manager->getGPLAccepted()) {
    _manager->save();
  }
}

