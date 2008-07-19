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
#include <QGroupBox>
#include <QLabel>

#include <QMessageBox>

#include "ProfileEditDialog.h"
#include "2d_bgnd_w_lens/lens_engine/SphericalLensProfile.h"
#include "utility/misc_funcs.h"

ProfileEditDialog::ProfileEditDialog(QString targetName, ConfigManager* confMgr, QWidget* parent, Qt::WindowFlags f) {
	QDialog(parent, f);

	_coarsenessAdjust = 100.0;
	_nameBox = NULL;
	_timerBox = NULL;
	_screenXBox = NULL;
	_screenYBox = NULL;
	_coarsenessBox = NULL;
	_paletteBox = NULL;
	_paletteXSpeedBox = NULL;
	_paletteYSpeedBox = NULL;
	_clampColorBox = NULL;
	_palDlg = NULL;
	_bFinishedAddEdit = true;
	_lensSizeSlider = NULL;
	_lensVarSlider = NULL;
	_numLenses = NULL;

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
	QGroupBox* dimBox = new QGroupBox(tr("Plasma Dimensions"));
	QHBoxLayout* dimBoxLayout = new QHBoxLayout();

	_screenXBox = new QComboBox();
	_screenYBox = new QComboBox();

	_screenXBox->setEditable(false);
	_screenXBox->setEditable(false);

	//populate the combo box with powers of 2
	int val = 64; //2^6
	int xCurVal = 0;
	int yCurVal = 0;
	for(int i=0;i<13-6;i++) {
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

	tempToolTip = tr("The resolution width of the plasma in pixels.  Does not need to match actual")+"\n"
					+tr("screen resolution.");
	tempWidget = new QLabel(tr("Width:"));
	tempWidget->setToolTip(tempToolTip);
	dimBoxLayout->addWidget(tempWidget);
	tempWidget = NULL;
	_screenXBox->setToolTip(tempToolTip);
	dimBoxLayout->addWidget(_screenXBox);

	dimBoxLayout->addStretch(0);

	tempToolTip = tr("The resolution height of the plasma in pixels.  Does not need to match actual")+"\n"
					+tr("screen resolution.");
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
	tempToolTip = tr("The number of milliseconds between frames.  The higher the number, the")+"\n"
					+tr("slower the animation.");
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

	// Begin PlasmaFractalBackgroundProfile related
	
	// get the backgroundprofile in order to populate the data fields.
	PlasmaFractalBackgroundProfile* plasmaProfile = NULL;
	QString bkgndProfileName = _mp.getBackgroundProfileName();
	if(_confMgr->doesBackgroundProfileExist(bkgndProfileName)) {
		plasmaProfile = new PlasmaFractalBackgroundProfile();
		if(plasmaProfile != NULL) {
			PlasmaFractalBackgroundProfile* tempProfile = (PlasmaFractalBackgroundProfile*) (_confMgr->getBackgroundProfile(bkgndProfileName));
			*plasmaProfile = *tempProfile;
			_bAnimatePalette = plasmaProfile->getAnimatePalette();
		}
	}

	// palette name combo box + add & edit buttons
	QGroupBox* palGroupBox = new QGroupBox(tr("Palette"));
	if(palGroupBox == NULL)
		return;
	palGroupBox->setCheckable(false);

	QHBoxLayout* palBoxLayout = new QHBoxLayout();

	_paletteBox = new QComboBox();
	_paletteBox->setEditable(false);
	if(plasmaProfile != NULL) {
		populatePalList(plasmaProfile->getPaletteName());
	} else {
		populatePalList("");
	}
	tempToolTip = tr("The currently selected palette.");
	_paletteBox->setToolTip(tempToolTip);
	palBoxLayout->addWidget(_paletteBox);

	palBoxLayout->addStretch(0);

	tempButton = new QPushButton(tr("&Add"));
	tempButton->setDefault(false);
	tempButton->setAutoDefault(false);
	tempToolTip = tr("Add a new palette.");
	tempButton->setToolTip(tempToolTip);
	QObject::connect(tempButton, SIGNAL(clicked(bool)), this, SLOT(addPalClicked(bool)));
	palBoxLayout->addWidget(tempButton);
	tempButton = NULL;

	tempButton = new QPushButton(tr("&Edit"));
	tempButton->setDefault(false);
	tempButton->setAutoDefault(false);
	tempToolTip = tr("Edit the selected palette.");
	tempButton->setToolTip(tempToolTip);
	QObject::connect(tempButton, SIGNAL(clicked(bool)), this, SLOT(editPalClicked(bool)));
	palBoxLayout->addWidget(tempButton);
	tempButton = NULL;

	palGroupBox->setLayout(palBoxLayout);
	mainLayout->addWidget(palGroupBox);
	tempWidget = NULL;

	// Palette Animation enable and speeds
	QGroupBox* animPalBox = new QGroupBox(tr("Animate Palette"));
	animPalBox->setCheckable(true);
	animPalBox->setChecked(_bAnimatePalette);
	animPalBox->setToolTip(tr("This will enable rotating the palette in both the X and Y directions by the specified speed."));
	QObject::connect(animPalBox, SIGNAL(clicked(bool)), this, SLOT(animatePalClicked(bool)));

	QHBoxLayout* animPalLayout = new QHBoxLayout();
	tempWidget = new QLabel(tr("X Speed:"));
	tempToolTip = tr("This is how fast the palette will change in the X (primary) direction.");
	tempWidget->setToolTip(tempToolTip);
	animPalLayout->addWidget(tempWidget);

	_paletteXSpeedBox = new QDoubleSpinBox();
	_paletteXSpeedBox->setDecimals(1);
	_paletteXSpeedBox->setMinimum(0.0);
	_paletteXSpeedBox->setSingleStep(0.1);
	if(plasmaProfile != NULL) {
		_paletteXSpeedBox->setValue(plasmaProfile->getPaletteXSpeed());
	} else {
		_paletteXSpeedBox->setValue(1.0);
	}
	_paletteXSpeedBox->setToolTip(tempToolTip);
	animPalLayout->addWidget(_paletteXSpeedBox);

	animPalLayout->addStretch(0);

	tempWidget = new QLabel(tr("Y Speed:"));
	tempToolTip = tr("This is how fast the palette will change in the Y (secondary) direction.");
	tempWidget->setToolTip(tempToolTip);
	animPalLayout->addWidget(tempWidget);

	_paletteYSpeedBox = new QDoubleSpinBox();
	_paletteYSpeedBox->setDecimals(1);
	_paletteYSpeedBox->setMinimum(0.0);
	_paletteYSpeedBox->setSingleStep(0.1);
	if(plasmaProfile != NULL) {
		_paletteYSpeedBox->setValue(plasmaProfile->getPaletteYSpeed());
	} else {
		_paletteYSpeedBox->setValue(1.0);
	}
	_paletteYSpeedBox->setToolTip(tempToolTip);
	animPalLayout->addWidget(_paletteYSpeedBox);

	animPalBox->setLayout(animPalLayout);
	mainLayout->addWidget(animPalBox);

	QHBoxLayout* ccLayout = new QHBoxLayout();

	tempWidget = new QLabel(tr("Less Volatile"));
	tempToolTip = tr("This quantity determines how coarse or volatile the fractal is.  More volatile, the more variation.");
	tempWidget->setToolTip(tempToolTip);
	ccLayout->addWidget(tempWidget);

	_coarsenessBox = new QSlider(Qt::Horizontal);
	_coarsenessBox->setTickInterval(0.10*_coarsenessAdjust);
	//_coarsenessBox->setTickPosition(QSlider::TicksBelow);
	_coarsenessBox->setMinimum(0.01*_coarsenessAdjust);
	_coarsenessBox->setMaximum(6.00*_coarsenessAdjust);
	if(plasmaProfile != NULL) {
		_coarsenessBox->setValue(plasmaProfile->getCoarseness()*_coarsenessAdjust);
	} else {
		_coarsenessBox->setValue(4.50*_coarsenessAdjust);
	}
	_coarsenessBox->setToolTip(tempToolTip);
	ccLayout->addWidget(_coarsenessBox);

	tempWidget = new QLabel(tr("More Volatile"));
	tempWidget->setToolTip(tempToolTip);
	ccLayout->addWidget(tempWidget);
	
	ccLayout->addStretch(0);

	_clampColorBox = new QCheckBox(tr("Clamp color on overflow"));
	if(plasmaProfile != NULL) {
		_clampColorBox->setChecked(plasmaProfile->getClampColorIndex());
	} else {
		_clampColorBox->setChecked(true);
	}
	_clampColorBox->setToolTip(tr("When the plasma algorithm generates an invalid color index, it will clamp")+"\n"
								+tr("to the nearest valid color if checked, or wrap around if unchecked."));
	ccLayout->addWidget(_clampColorBox);

	tempWidget = new QWidget();
	tempWidget->setLayout(ccLayout);
	mainLayout->addWidget(tempWidget);
	tempWidget = NULL;

	// Begin SphericalLensProfile related
	_numLenses = new QSpinBox();
	if(_numLenses == NULL)
	  return;
	_numLenses->setMinimum(0);
	_numLenses->setMaximum(100);
	_lensSizeSlider = new QSlider(Qt::Horizontal);
	if(_lensSizeSlider == NULL)
	  return;
	_lensVarSlider = new QSlider(Qt::Horizontal);
	if(_lensVarSlider == NULL)
	  return;
	_lensSizeSlider->setMinimum(1);
	_lensSizeSlider->setMaximum(50);
	_lensVarSlider->setMinimum(0);
	_lensVarSlider->setMaximum(100);
	_lensSizeSlider->setTickInterval(1);
	_lensVarSlider->setTickInterval(1);

	QString lensProfileName = _mp.getLensProfileName();

	if(!lensProfileName.isEmpty()) {
	  SphericalLensProfile* lp = (SphericalLensProfile*)_confMgr->getLensProfile(lensProfileName);

	  _numLenses->setValue(lp->getMaxNumLenses());
	  float maxR = lp->getMaxRadius();
	  float avgR = (lp->getMinRadius()+maxR)/2.0;
	  _lensSizeSlider->setValue(avgR*100.0);
	  _lensVarSlider->setValue( (maxR-avgR)*100.0/avgR);
	} else {
	  _numLenses->setValue(0);
	  _lensSizeSlider->setValue(1);
	  _lensVarSlider->setValue(0);
	}

	QHBoxLayout* tmpHBox = NULL;
	QVBoxLayout* tmpVBox = NULL;
	QGroupBox* lensBox = new QGroupBox(tr("Lenses"));
	if(lensBox == NULL)
	  return;
	QHBoxLayout* lensLayout = new QHBoxLayout();
	if(lensLayout == NULL)
	  return;

	tempToolTip = tr("Determines the maximum number of lenses on screen at one time.");
	tempWidget = new QLabel(tr("Number:"));
	tempWidget->setToolTip(tempToolTip);
	lensLayout->addWidget(tempWidget);
	tempWidget = NULL;
	_numLenses->setToolTip(tempToolTip);
	lensLayout->addWidget(_numLenses);

	lensLayout->addStretch(0);
	
	tmpVBox = new QVBoxLayout();
	if(tmpVBox == NULL)
	  return;
	tmpHBox = new QHBoxLayout();
	if(tmpHBox == NULL)
	  return;

	tempToolTip = tr("Determines the average size of the lenses.");
	tempWidget = new QLabel(tr("Size"));
	if(tempWidget == NULL)
	  return;
	tempWidget->setToolTip(tempToolTip);
	tmpHBox->addWidget(tempWidget);
	tempWidget = NULL;

	tmpHBox->addStretch(0);
	
	tempWidget = new QLabel(tr("Min"));
	if(tempWidget == NULL)
	  return;
	tempWidget->setToolTip(tempToolTip);
	tmpHBox->addWidget(tempWidget);
	tempWidget = NULL;

	_lensSizeSlider->setToolTip(tempToolTip);
	tmpHBox->addWidget(_lensSizeSlider);

	tempWidget = new QLabel(tr("Max"));
	if(tempWidget == NULL)
	  return;
	tempWidget->setToolTip(tempToolTip);
	tmpHBox->addWidget(tempWidget);
	tempWidget = NULL;

	tempWidget = new QWidget();
	tempWidget->setLayout(tmpHBox);
	tmpVBox->addWidget(tempWidget);
	tempWidget = NULL;
	tmpHBox = NULL;

	tmpHBox = new QHBoxLayout();
	if(tmpHBox == NULL)
	  return;

	tempToolTip = tr("Determines the amount of size variation.");
	tempWidget = new QLabel(tr("Variation"));
	if(tempWidget == NULL)
	  return;
	tempWidget->setToolTip(tempToolTip);
	tmpHBox->addWidget(tempWidget);
	tempWidget = NULL;

	tmpHBox->addStretch(0);
	
	tempWidget = new QLabel(tr("Min"));
	if(tempWidget == NULL)
	  return;
	tempWidget->setToolTip(tempToolTip);
	tmpHBox->addWidget(tempWidget);
	tempWidget = NULL;

	_lensVarSlider->setToolTip(tempToolTip);
	tmpHBox->addWidget(_lensVarSlider);

	tempWidget = new QLabel(tr("Max"));
	if(tempWidget == NULL)
	  return;
	tempWidget->setToolTip(tempToolTip);
	tmpHBox->addWidget(tempWidget);
	tempWidget = NULL;

	tempWidget = new QWidget();
	tempWidget->setLayout(tmpHBox);
	tmpVBox->addWidget(tempWidget);
	tempWidget = NULL;
	tmpHBox = NULL;

	tempWidget = new QWidget();
	tempWidget->setLayout(tmpVBox);
	lensLayout->addWidget(tempWidget);
	tempWidget = NULL;

	lensBox->setLayout(lensLayout);
	mainLayout->addWidget(lensBox);

	//Help, OK and Cancel buttons
	QHBoxLayout* botButtonsLayout = new QHBoxLayout();
	
	tempButton = new QPushButton(tr("&Help"));
	tempButton->setDefault(false);
	tempButton->setAutoDefault(false);
	QObject::connect(tempButton, SIGNAL(clicked(bool)), this, SLOT(helpClicked(bool)));
	botButtonsLayout->addWidget(tempButton);
	tempButton = NULL;

	botButtonsLayout->addStretch(0);

	tempButton = new QPushButton(tr("&OK"));
	tempButton->setDefault(true);
	tempButton->setAutoDefault(true);
	QObject::connect(tempButton, SIGNAL(clicked(bool)), this, SLOT(okClicked(bool)));
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

	if(_coarsenessBox != NULL) {
		delete _coarsenessBox;
		_coarsenessBox = NULL;
	}

	if(_paletteBox != NULL) {
		delete _paletteBox;
		_paletteBox = NULL;
	}

	if(_paletteXSpeedBox != NULL) {
		delete _paletteXSpeedBox;
		_paletteXSpeedBox = NULL;
	}

	if(_paletteYSpeedBox != NULL) {
		delete _paletteYSpeedBox;
		_paletteYSpeedBox = NULL;
	}

	if(_clampColorBox != NULL) {
		delete _clampColorBox;
		_clampColorBox = NULL;
	}
	if(_palDlg != NULL) {
		delete _palDlg;
		_palDlg = NULL;
	}
	if(_lensSizeSlider != NULL) {
	  delete _lensSizeSlider;
	  _lensSizeSlider = NULL;
	}
	if(_lensVarSlider != NULL) {
	  delete _lensVarSlider;
	  _lensVarSlider = NULL;
	}
	if(_numLenses != NULL) {
	  delete _numLenses;
	  _numLenses = NULL;
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
	if(_paletteBox->currentIndex() < 0) {
		QMessageBox::warning(this, _windowTitle, "Please choose a palette from the drop-down box.", QMessageBox::Ok);
		return;
	}

	PlasmaFractalBackgroundProfile tmpProfile;
	tmpProfile.setAnimatePalette(_bAnimatePalette);
	tmpProfile.setClampColorIndex(_clampColorBox->isChecked());
	tmpProfile.setCoarseness(((float)_coarsenessBox->value())/_coarsenessAdjust);
	tmpProfile.setGenStepsPerTick(2048);
	tmpProfile.setPaletteXSpeed(_paletteXSpeedBox->value());
	tmpProfile.setPaletteYSpeed(_paletteYSpeedBox->value());
	tmpProfile.setPaletteName(_paletteBox->currentText());

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

	// LensProfile related
	SphericalLensProfile tmpSphere;
	tmpSphere.setMaxNumLenses(_numLenses->value());
	float avgR = _lensSizeSlider->value();
	float rangeR = _lensVarSlider->value();
	float minR = avgR * (1.00 - rangeR/100.0);
	float maxR = avgR * (1.00 + rangeR/100.0);
	tmpSphere.setRadius(minR/100.0, maxR/100.0, (rangeR > 0.00));
	tempName = _mp.getLensProfileName();

	//!!! NOTE: There is a bug that causes a core dump in this code!
	if(tempName == "" || !_confMgr->doesLensProfileExist(tempName)) {
		// generate a random LensProfile name that doesn't exist.
		while(tempName == "" || _confMgr->doesLensProfileExist(tempName)) {
			tempName = QString("LensProfile_")+randStr(20);
		}
		tmpSphere.setName(tempName);

		LensProfile* tL = tmpSphere.clone();
		_confMgr->addLensProfile(tL);
	} else {
		tmpSphere.setName(tempName);
		LensProfile* tL = tmpSphere.clone();
		_confMgr->replaceLensProfile(tempName,*tL);		
	}
	_mp.setLensProfileName(tempName);
	// !!! end note

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
	emit accepted();
	//emit finished(0);
	close();
}

void ProfileEditDialog::cancelClicked(bool checked) {
  //emit finished(1);
  emit rejected();
	close();
}

void ProfileEditDialog::addPalClicked(bool checked) {
	if(_confMgr == NULL)
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
void ProfileEditDialog::addAccepted(void) {
	if(_confMgr == NULL)
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
			//editPalette(pal, true);
		} else if(_confMgr->doesPaletteExist(pal.getName())) {
			//give the user a warning dialog box
			QString message(tr("A palette with name '"));
			message += pal.getName();
			message += "' already exists.  Please choose a new name.";
			QMessageBox::warning(this, 0, message, QMessageBox::Ok);
			QString param("");
			pal.setName(param);

			//attempt to edit
			//editPalette(pal, true);
		} else {
			//nope, not in the list.  Add it!
			_confMgr->addPalette(pal);
			_paletteBox->addItem(pal.getName());
			_bFinishedAddEdit = true;
		}
	}
}

void ProfileEditDialog::editPalClicked(bool checked) {
	if(_confMgr == NULL || _paletteBox == NULL)
		return;

	if(_paletteBox->currentIndex() >= 0) {
		QString selected = _paletteBox->currentText();

		//first, check for existence.
		if(_confMgr->doesPaletteExist(selected)) {
			editPalette(&_confMgr->getPaletteProfile(selected), false);
		}
	}
}

void ProfileEditDialog::editAccepted(void) {
 	if(_confMgr == NULL || _paletteBox == NULL)
		return;

	if(_palDlg != NULL) {
		IndexedPaletteProfile pal = _palDlg->getPalette();

		//check to make sure the name is valid.
		if(pal.getName() == "") {
			//give the user a warning dialog box
			QMessageBox::warning(this, 0, "Please enter a valid name.", QMessageBox::Ok);

			//restore old name and attempt to edit
			pal.setName(_palOldName);
			editPalette(&pal, false);
			return;
		}

		//check to see if the name has changed and if so, whether the new name is in the list already
		if( (_palOldName == "" || _palOldName != pal.getName()) && _confMgr->doesPaletteExist(pal.getName())) {
			//give the user a warning dialog box
			QString message(tr("A palette with name '"));
			message += pal.getName();
			message += "' already exists.  Please choose a new name.";
			QMessageBox::warning(this, 0, message, QMessageBox::Ok);

			//restore old name and attempt to edit
			pal.setName(_palOldName);
			editPalette(&pal, false);
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
			int count = _paletteBox->count();
			QString tmpItem = _paletteBox->itemText(0);
			while(tmpItem != _palOldName && i < count) {
				i++;
				tmpItem = _paletteBox->itemText(i);
			}
			if(!tmpItem.isEmpty() && tmpItem == _palOldName) {
				_paletteBox->removeItem(i);
				_paletteBox->addItem(pal.getName());
				_paletteBox->setCurrentIndex(_paletteBox->count()-1);
			}
		}
	}
}

//attempts to edit the specified palette.
void ProfileEditDialog::editPalette(IndexedPaletteProfile* pal, bool bWasAddRename) {
	//store the current name, in case the name has changed.  If this is 
	//a forced rename, don't store the current name.
	if(bWasAddRename && pal != NULL) {
		//_palOldName = "";
	} else {
		_palOldName = pal->getName();
	}
/*
	if(_palDlg != NULL) {
		_palDlg->done(0);
		delete _palDlg;
		_palDlg = NULL;
	}
*/
	//attempt to create the dialog
	_palDlg = new IndexedPaletteDialog(pal,this);
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
void ProfileEditDialog::animatePalClicked(bool checked) {
	_bAnimatePalette = checked;
}

// populates the palette name selection combo box
void ProfileEditDialog::populatePalList(QString selected) {
	if(_paletteBox == NULL)
		return;

	// empty list and repopulate
	_paletteBox->clear();
	_paletteBox->addItems(_confMgr->getPaletteNames());

	// select the currently selected.
	int idx = _paletteBox->findText(selected);
	_paletteBox->setCurrentIndex(idx);
}

void ProfileEditDialog::helpClicked(bool checked) {
	QMessageBox::information(this, _windowTitle, "Not implemented yet.", QMessageBox::Ok);
}

