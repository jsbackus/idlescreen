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
#include <QCheckBox>
#include <QColorDialog>
#include <QColor>
#include <QPixmap>
#include <QImage>

#include <QProgressDialog>
#include <QApplication>

#include <QString>

#include "IndexedPaletteDialog.h"

IndexedPaletteDialog::IndexedPaletteDialog(IndexedPaletteProfile* palProfile, QWidget* parent, Qt::WindowFlags f) {
	QDialog(parent, f);
	setWindowTitle(tr("Palette Editor"));
	setWindowModality(Qt::WindowModal);
	//QIcon icon(":/icon1.png");
	//setWindowIcon(icon);

	_prgDlg = NULL;
	_prgCount = 0;

	if(palProfile == NULL) {
		_palProfile.setHeight(16);
		_palProfile.setWidth(16);
		_origPalProfile = _palProfile;
	} else {
		_palProfile = *palProfile;
		_origPalProfile = *palProfile;
	}

	_prgDlg = new QProgressDialog("Please wait...", 0, 0, _palProfile.getHeight()*3+2);
	_prgCount = 0;

	_colorWidget = new IndexedPaletteEditorWidget();
	QObject::connect(_colorWidget, SIGNAL(colorChanged(int, int, QRgb)), this, SLOT(colorChanged(int, int, QRgb)));
	populateColorWidget();

	//hide();
	QWidget* tempWidget = NULL;
	QPushButton* tempButton = NULL;
	QVBoxLayout* mainLayout = new QVBoxLayout();

	//top region
	QHBoxLayout* topRegion = new QHBoxLayout();

	QVBoxLayout* topLeftPane = new QVBoxLayout();

	//name section and change default color
	QHBoxLayout* nameRegion = new QHBoxLayout();
	QLabel* tmpLabel = new QLabel(tr("Palette Name:"));
	nameRegion->addWidget(tmpLabel);
	tmpLabel = NULL;

	_nameEdit = new QLineEdit(_palProfile.getName());
	nameRegion->addWidget(_nameEdit);

	tempButton = new QPushButton(tr("Default Color"));
	QObject::connect(tempButton, SIGNAL(clicked(bool)), this, SLOT(defaultColorClicked(bool)));
	nameRegion->addWidget(tempButton);
	tempButton = NULL;

	tempWidget = new QWidget();
	tempWidget->setLayout(nameRegion);
	topLeftPane->addWidget(tempWidget);
	tempWidget = NULL;

	//dimensions
	QHBoxLayout* dimsRegion = new QHBoxLayout();

	tmpLabel = new QLabel(tr("Dimensions:"));
	//tmpLabel->setToolTip(tr("The number of minutes to wait before switching screen savers."));
	dimsRegion->addWidget(tmpLabel);

	dimsRegion->addStretch(0);

	tmpLabel = new QLabel(tr("X"));
	//tmpLabel->setToolTip(tr("The number of minutes to wait before switching screen savers."));
	dimsRegion->addWidget(tmpLabel);

	_xDimBox = new QSpinBox();
	_xDimBox->setRange(1,1024);
	_xDimBox->setSingleStep(1);
	_xDimBox->setValue(_palProfile.getWidth());
	//_xDimBox->setToolTip(tr("The number of minutes to wait before switching screen savers."));
	dimsRegion->addWidget(_xDimBox);

	dimsRegion->addStretch(0);

	tmpLabel = new QLabel(tr("Y"));
	//tmpLabel->setToolTip(tr("The number of minutes to wait before switching screen savers."));
	dimsRegion->addWidget(tmpLabel);

	_yDimBox = new QSpinBox();
	_yDimBox->setRange(1,1024);
	_yDimBox->setSingleStep(1);
	_yDimBox->setValue(_palProfile.getHeight());
	//_yDimBox->setToolTip(tr("The number of minutes to wait before switching screen savers."));
	dimsRegion->addWidget(_yDimBox);

	tempButton = new QPushButton(tr("Resize"));
	QObject::connect(tempButton, SIGNAL(clicked(bool)), this, SLOT(resizeClicked(bool)));
	dimsRegion->addWidget(tempButton);
	tempButton = NULL;

	tempWidget = new QWidget();
	tempWidget->setLayout(dimsRegion);
	topLeftPane->addWidget(tempWidget);
	tempWidget = NULL;

	tempWidget = new QWidget();
	tempWidget->setLayout(topLeftPane);
	topRegion->addWidget(tempWidget);
	tempWidget = NULL;

	//interp colors region
	QGroupBox* interpBox = new QGroupBox(tr("Interpolate Colors"));
	interpBox->setCheckable(true);
	interpBox->setChecked(_palProfile.getInterpolateColors());
	//interpBox->setToolTip(tr("Enabling this will randomize the order in which the screen savers will run."));
	QObject::connect(interpBox, SIGNAL(clicked(bool)), this, SLOT(interpClicked(bool)));

	QHBoxLayout* interpBoxLayout = new QHBoxLayout();
	QCheckBox* wrapBox = new QCheckBox(tr("Wrap Colors"));
	wrapBox->setCheckable(true);
	wrapBox->setChecked(_palProfile.getWrapColors());
	QObject::connect(wrapBox, SIGNAL(clicked(bool)), this, SLOT(wrapClicked(bool)));
	interpBoxLayout->addWidget(wrapBox);
	interpBox->setLayout(interpBoxLayout);

	topRegion->addWidget(interpBox);

	tempWidget = new QWidget();
	tempWidget->setLayout(topRegion);
	mainLayout->addWidget(tempWidget);
	tempWidget = NULL;

	//draw grid of entry buttons
	_colorScrollWidget = new QScrollArea();

	_colorScrollWidget->setWidget(_colorWidget);
	mainLayout->addWidget(_colorScrollWidget);

	//preview area
	tmpLabel = new QLabel("Preview:");
	mainLayout->addWidget(tmpLabel);
	tmpLabel = NULL;

	_previewZoom = 4;
	_previewLabel = new QLabel();
	_previewLabel->setPixmap(_previewPixmap);
	updatePreviewIcon();

	QScrollArea* previewScrollArea = new QScrollArea();
	previewScrollArea->setWidget(_previewLabel);
	mainLayout->addWidget(previewScrollArea);

	//Revert, OK, and Cancel buttons
	QHBoxLayout* botButtonsLayout = new QHBoxLayout();
	
	tempButton = new QPushButton(tr("&Revert"));
	tempButton->setEnabled(false);
	QObject::connect(tempButton, SIGNAL(clicked(bool)), this, SLOT(revertClicked(bool)));
	botButtonsLayout->addWidget(tempButton);
	tempButton = NULL;

	botButtonsLayout->addStretch(0);

	tempButton = new QPushButton(tr("&OK"));
	QObject::connect(tempButton, SIGNAL(clicked(bool)), this, SLOT(okClicked(bool)));
	tempButton->setDefault(true);
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

	//show();

	//clean up progress dialog
	if(_prgDlg != NULL) {
		delete _prgDlg;
		_prgDlg = NULL;
		_prgCount = 0;
	}

}

IndexedPaletteDialog::~IndexedPaletteDialog() {

	//all widgets should get destroyed by the layout.
	_colorScrollWidget = NULL;
	_colorWidget = NULL;
	_nameEdit = NULL;
	_previewLabel = NULL;
	_prgDlg = NULL;
	_xDimBox = NULL;
	_yDimBox = NULL;

	/*
	if(_colorScrollWidget != NULL) {
		delete _colorScrollWidget;
		_colorScrollWidget = NULL;
	}
	
	if(_colorWidget != NULL) {
		delete _colorWidget;
		_colorWidget = NULL;
	}

	if(_nameEdit != NULL) {
		delete _nameEdit;
		_nameEdit = NULL;
	}

	if(_previewLabel != NULL) {
		delete _previewLabel;
		_previewLabel = NULL;
	}

	if(_prgDlg != NULL) {
		delete _prgDlg;
		_prgDlg = NULL;
	}

	if(_xDimBox != NULL) {
		delete _xDimBox;
		_xDimBox = NULL;
	}

	if(_yDimBox != NULL) {
		delete _yDimBox;
		_yDimBox = NULL;
	}
	*/
}

//returns the modified palette.
IndexedPaletteProfile IndexedPaletteDialog::getPalette(void) {
	return _palProfile;
}

void IndexedPaletteDialog::done(int r) {
	emit finished(r);
	close();
}

void IndexedPaletteDialog::okClicked(bool checked) {
	_palProfile.setName(_nameEdit->text());
	emit paletteUpdated();
	emit accepted();
	emit finished(0);
	close();
}

void IndexedPaletteDialog::cancelClicked(bool checked) {
	_palProfile = _origPalProfile;
	emit finished(1);
	close();
}

void IndexedPaletteDialog::revertClicked(bool checked) {
	_palProfile = _origPalProfile;
	_palProfile.setHeight(_palProfile.getHeight()*2);
	_palProfile.setWidth(_palProfile.getWidth()*2);
	_nameEdit->setText(_palProfile.getName());

	if(_colorWidget != NULL)
		delete _colorWidget;
	_colorWidget = new IndexedPaletteEditorWidget();
	if(_colorScrollWidget != NULL)
		_colorScrollWidget->setWidget(_colorWidget);
	QObject::connect(_colorWidget, SIGNAL(colorChanged(int, int, QRgb)), this, SLOT(colorChanged(int, int, QRgb)));
	populateColorWidget();

	updatePreviewIcon();
}

void IndexedPaletteDialog::colorChanged(int x, int y, QRgb color) {
	_palProfile.setColor(x, y, color);
	updatePreviewIcon();
}

void IndexedPaletteDialog::populateColorWidget(void) {
	int height = _palProfile.getHeight();
	int width = _palProfile.getWidth();

	_colorWidget->setProgressDialog(_prgDlg);
	_colorWidget->changeSize(width, height);
	_colorWidget->setProgressDialog(NULL);

	for(int y=0; y<height; y++) {
		for(int x=0; x<width;x++) {
			if(_palProfile.isColorDefined(x,y)) {
				QRgb tmpColor = _palProfile.getColor(x, y);
				_colorWidget->setColor(x, y, tmpColor);
			}
		}
		stepProgress();
	}
}

void IndexedPaletteDialog::defaultColorClicked(bool checked) {
	bool bOk = false;

	QRgb newColor = QColorDialog::getRgba(_palProfile.getDefaultColor(), &bOk, this);
	if(bOk) {		
		_palProfile.setDefaultColor(newColor);
		updatePreviewIcon();
	}
}

void IndexedPaletteDialog::interpClicked(bool checked) {
	_palProfile.setInterpolateColors(checked);
	updatePreviewIcon();
}

void IndexedPaletteDialog::wrapClicked(bool checked) {
	_palProfile.setWrapColors(checked);
	updatePreviewIcon();
}

//updates the preview icon
void IndexedPaletteDialog::updatePreviewIcon(void) {
	//create the palette.
	IndexedPalette* tmpPal = _palProfile.createPalette();

	int width = _palProfile.getWidth();
	int height = _palProfile.getHeight();

	//check to see if the progress dialog box pointer is null.  If so,
	//create a new one.
	bool bISetPrgDlg = false;
	if(_prgDlg == NULL) {
		_prgDlg = new QProgressDialog("Populating...", 0, 0, height);
		_prgCount = 0;
		bISetPrgDlg = true;
	}

	//go through the palette and for each color add a pixel in the
	//image
	QImage tmpImage(width*_previewZoom, height*_previewZoom, QImage::Format_ARGB32);

	for(int y=0; y<height; y++) {
		for(int x=0; x<width; x++) {
			int buff[4];
			tmpPal->getColor(x, y, &buff[0], 4);
			QRgb tmpColor = qRgba(buff[0], buff[1], buff[2], 255);
			for(int i=0;i<_previewZoom;i++) {
				for(int j=0;j<_previewZoom;j++) {
					tmpImage.setPixel(x*_previewZoom+i,y*_previewZoom+j,tmpColor);
				}
			}
		}
		stepProgress();
	}
	_previewPixmap = QPixmap::fromImage(tmpImage);
	_previewLabel->clear();
	_previewLabel->resize(width*_previewZoom, height*_previewZoom);
	_previewLabel->setPixmap(_previewPixmap);

	//_previewLabel->setPixmap(QPixmap::fromImage(tmpImage));

	delete tmpPal;
	tmpPal = NULL;

	//if we created the progress dialog, clean up
	if(bISetPrgDlg && _prgDlg != NULL) {
		delete _prgDlg;
		_prgDlg = NULL;
		_prgCount = 0;
	}
}

//steps the progress dialog
void IndexedPaletteDialog::stepProgress() {
	if(_prgDlg != NULL) {
		_prgDlg->setValue(++_prgCount);
		QApplication::processEvents();		
	}
}

void IndexedPaletteDialog::resizeClicked(bool checked) {

	int width = _xDimBox->value();
	int height = _yDimBox->value();

	if(width > 0 && height > 0) {
		_prgDlg = new QProgressDialog("Please wait...", 0, 0, height*3+2);
		_prgCount = 0;

		_palProfile.setHeight(height);
		_palProfile.setWidth(width);

		populateColorWidget();
		updatePreviewIcon();
		//_colorScrollWidget->resize(width, height);

		delete _prgDlg;
		_prgDlg = NULL;
	}
}
