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

#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QRadioButton>
#include <QString>
#include <QWidget>
#include <QLabel>

#include "AboutDialog.h"
#include "gpl_related/gpldialog.h"

//#include "../../../common/gpl_related/gpldialog.h"

AboutDialog::AboutDialog(QString appName, QString versionStr, QWidget* parent, Qt::WindowFlags f) {
	QDialog(parent, f);

	setWindowTitle(tr("About"));
	QIcon icon(":/app_icon.png");
	setWindowIcon(icon);

	QPushButton* tempButton = NULL;
	QWidget* tempWidget = NULL;
	QLabel* tempLabel = NULL;
	QHBoxLayout* tmpHLyt = NULL;
	QVBoxLayout* layout = new QVBoxLayout();

	// text
	tempLabel = new QLabel(appName);
	layout->addWidget(tempLabel);
	tempLabel = new QLabel("Version: "+versionStr);
	layout->addWidget(tempLabel);
	tempLabel = new QLabel("Part of the Idle Screen Project");
	layout->addWidget(tempLabel);
	tempLabel = new QLabel("http://idlescreen.googlepages.com");
	layout->addWidget(tempLabel);
	tempLabel = new QLabel("");
	layout->addWidget(tempLabel);
	tempLabel = new QLabel("(c) 2008 Jeff Backus and the Idle Screen Project");
	layout->addWidget(tempLabel);


	//buttons at bottom
	tmpHLyt = new QHBoxLayout();
	tempButton = new QPushButton(tr("&Read License"));
	QObject::connect(tempButton, SIGNAL(clicked(bool)), this, SLOT(gplClicked(bool)));
	tmpHLyt->addWidget(tempButton);

	tempButton = new QPushButton(tr("&About Qt"));
	QObject::connect(tempButton, SIGNAL(clicked(bool)), this, SLOT(qtClicked(bool)));
	tmpHLyt->addWidget(tempButton);

	tmpHLyt->addStretch(0);

	tempButton = new QPushButton(tr("&OK"));
	tempButton->setDefault(true);
	QObject::connect(tempButton, SIGNAL(clicked()), this, SLOT(accept()));
	tmpHLyt->addWidget(tempButton);

	tempWidget = new QWidget();
	tempWidget->setLayout(tmpHLyt);
	layout->addWidget(tempWidget);
	tempWidget = NULL;

	setLayout(layout);
}

void AboutDialog::gplClicked(bool checked) {
	// assume true, since the user shouldn't be able to get to
	// this point otherwise
	GPLDialog dlg(true);
	dlg.exec();
}

void AboutDialog::qtClicked(bool checked) {
	QMessageBox::aboutQt(this, "About");
}

