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

#include <QTextDocument>
#include <QTextEdit>
#include <QButtonGroup>
#include <QVBoxLayout>
#include <QPushButton>
#include <QRadioButton>
#include <QString>
#include <QTextStream>
#include <QFile>

#include "gpldialog.h"

GPLDialog::GPLDialog(bool bAccepted, QWidget* parent, Qt::WindowFlags f) {
	
	QDialog(parent, f);
	_bAccepted = bAccepted;
	setWindowTitle(tr("GPL"));
	QIcon icon(":/app_icon.png");
	setWindowIcon(icon);

	QVBoxLayout* layout = new QVBoxLayout();
	QTextEdit* label = new QTextEdit();
	QFile gplFile(":/gplv2.txt");
    if (!gplFile.open(QIODevice::ReadOnly | QIODevice::Text))
         return;
	QTextStream inStream(&gplFile);
	QString gplStr = inStream.readAll();
	QTextDocument* doc = new QTextDocument(gplStr);
	label->setDocument(doc);
	label->setTabStopWidth(10);
	label->setReadOnly(true);
	label->setMinimumWidth(500);
	layout->addWidget(label);

	//accept/decline radio buttons
	QButtonGroup* btnGrp = new QButtonGroup();
	btnGrp->setExclusive(true);
	
	QRadioButton* rButton = new QRadioButton(tr("I accept the license agreement"));
	rButton->setChecked(bAccepted);
	rButton->setEnabled(!bAccepted);
	QObject::connect(rButton, SIGNAL(toggled(bool)), this, SLOT(gplAcceptDown(bool)));
	btnGrp->addButton(rButton);
	layout->addWidget(rButton);

	rButton = new QRadioButton(tr("I do NOT accept the  license agreement"));
	rButton->setChecked(!bAccepted);
	rButton->setEnabled(!bAccepted);
	btnGrp->addButton(rButton);
	layout->addWidget(rButton);

	//ok button
	QPushButton* okButton = new QPushButton(tr("&OK"));
	okButton->setDefault(true);
	QObject::connect(okButton, SIGNAL(clicked()), this, SLOT(done()));
	layout->addWidget(okButton);

	setLayout(layout);
	
}

void GPLDialog::accept(void) {
	emit accepted();
	close();
	//emit finished(0);
}

void GPLDialog::done(int r) {
	emit finished(r);
	close();
}

void GPLDialog::reject(void) {
	emit rejected();
	close();
}

bool GPLDialog::isAccepted() {
	return _bAccepted;
	//return true;
}

void GPLDialog::gplAcceptDown(bool checked) {
	_bAccepted = checked;
}
