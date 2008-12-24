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
#include <QString>
#include <QTextBrowser>
#include <QUrl>
#include <QWidget>

//begin debug
#include <QMessageBox>
#include "../../common/utility/misc_funcs.h"
//end debug

#include "HelpDialog.h"

HelpDialog::HelpDialog(QString urlStr, QWidget* parent, Qt::WindowFlags f) {
	QDialog(parent, f);

	setWindowTitle(tr("Help"));
	QIcon icon(":/app_icon.png");
	setWindowIcon(icon);

	QPushButton* tempButton = NULL;
	QHBoxLayout* tmpHLyt = NULL;
	QWidget* tempWidget = NULL;
	QVBoxLayout* layout = new QVBoxLayout();

	// text
	QTextBrowser* browser = new QTextBrowser();
	if(browser == NULL)
		return;

	QUrl urlObj(urlStr);
	
	//QMessageBox::information(this, urlObj.toString(), boolToString(urlObj.isValid()), QMessageBox::Ok);

	browser->setSource(urlObj);
	layout->addWidget(browser);

	//buttons at bottom
	tmpHLyt = new QHBoxLayout();
	if(tmpHLyt == NULL)
		return;

	tmpHLyt->addStretch(0);

	tempButton = new QPushButton(tr("&Close"));
	tempButton->setDefault(true);
	QObject::connect(tempButton, SIGNAL(clicked()), this, SLOT(accept()));
	tmpHLyt->addWidget(tempButton);

	tempWidget = new QWidget();
	tempWidget->setLayout(tmpHLyt);
	layout->addWidget(tempWidget);
	tempWidget = NULL;

	setLayout(layout);
}

