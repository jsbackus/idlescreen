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
#include <QLabel>

#include "IndexedPaletteEditorTableWidgetSplitDlg.h"

IndexedPaletteEditorTableWidgetSplitDlg::IndexedPaletteEditorTableWidgetSplitDlg(int startX, int stopX, int startY, int stopY, QWidget* parent, Qt::WindowFlags f) {
	QDialog(parent, f);
	_x = -1;

	_y = -1;
	_startX = startX;
	_stopX = stopX;
	_startY = startY;
	_stopY = stopY;
	_xBox = NULL;
	_yBox = NULL;


	setWindowTitle(tr("Insert Color"));
	QIcon icon(":/app_icon.png");
	setWindowIcon(icon);

	QPushButton* tempButton = NULL;
	QWidget* tempWidget = NULL;
	QLabel* tempLabel = NULL;
	QHBoxLayout* tmpHLyt = NULL;
	QVBoxLayout* mainLayout = new QVBoxLayout();
	QString tempToolTip;

	tempLabel = new QLabel("Insert Cell At:");
	tempToolTip = tr("Split the group of color cells up, such that there is a single undefined")+"\n"
					+tr("color at the specified location, and the remaining colors remain")+"\n"
					+tr("in groups around the specified location.");
	tempWidget->setToolTip(tempToolTip);
	mainLayout->addWidget(tempLabel);

	// X
	tmpHLyt = new QHBoxLayout();
	tempToolTip = tr("The X coordinate of the single color cell.");
	tempLabel = new QLabel("X:");
	tempLabel->setToolTip(tempToolTip);
	tmpHLyt->addWidget(tempLabel);
	tmpHLyt->addStretch(0);
	_xBox = new QSpinBox();
	if(_xBox == NULL)
		return;
	_xBox->setSingleStep(1);
	_xBox->setRange(startX, stopX);
	_xBox->setEnabled(startX < stopX);
	_xBox->setToolTip(tempToolTip);
	tmpHLyt->addWidget(_xBox);

	tempWidget = new QWidget();
	tempWidget->setLayout(tmpHLyt);
	mainLayout->addWidget(tempWidget);
	tempWidget = NULL;

	// Y
	tempToolTip = tr("The Y coordinate of the single color cell.");
	tmpHLyt = new QHBoxLayout();
	tempLabel = new QLabel("Y:");
	tempLabel->setToolTip(tempToolTip);
	tmpHLyt->addWidget(tempLabel);
	tmpHLyt->addStretch(0);
	_yBox = new QSpinBox();
	if(_yBox == NULL)
		return;
	_yBox->setSingleStep(1);
	_yBox->setRange(startY, stopY);
	_yBox->setEnabled(startY < stopY);
	_yBox->setToolTip(tempToolTip);
	tmpHLyt->addWidget(_yBox);

	tempWidget = new QWidget();
	tempWidget->setLayout(tmpHLyt);
	mainLayout->addWidget(tempWidget);
	tempWidget = NULL;

	// buttons
	tmpHLyt = new QHBoxLayout();
	tmpHLyt->addStretch(0);

	tempButton = new QPushButton(tr("&OK"));
	tempButton->setDefault(true);
	QObject::connect(tempButton, SIGNAL(clicked(bool)), this, SLOT(okClicked(bool)));
	tmpHLyt->addWidget(tempButton);
	tempButton = NULL;

	tempButton = new QPushButton(tr("&Cancel"));
	QObject::connect(tempButton, SIGNAL(clicked(bool)), this, SLOT(cancelClicked(bool)));
	tmpHLyt->addWidget(tempButton);
	tempButton = NULL;

	tempWidget = new QWidget();
	tempWidget->setLayout(tmpHLyt);
	mainLayout->addWidget(tempWidget);
	tempWidget = NULL;

	setLayout(mainLayout);
}

// get coordinates.  Returns -1 if invalid.
int IndexedPaletteEditorTableWidgetSplitDlg::getX() {
	return _x;
}
int IndexedPaletteEditorTableWidgetSplitDlg::getY() {
	return _y;
}

void IndexedPaletteEditorTableWidgetSplitDlg::cancelClicked(bool checked) {
	_x = -1;
	_y = -1;
	reject();
}

void IndexedPaletteEditorTableWidgetSplitDlg::okClicked(bool checked) {
	if(_xBox != NULL && _yBox != NULL) {
		_x = _xBox->value();
		_y = _yBox->value();
	}
	accept();
}
