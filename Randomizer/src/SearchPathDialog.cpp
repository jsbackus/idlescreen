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

#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QPushButton>
#include<QFileDialog>
#include<QIcon>

#include "SearchPathDialog.h"

SearchPathDialog::SearchPathDialog(QWidget* parent, Qt::WindowFlags f) {
	QDialog(parent, f);
	_listWidget = NULL;
	_editWidget = NULL;

	setWindowTitle(tr("Search Paths"));
	QIcon icon(":/app_icon.png");
	setWindowIcon(icon);

	QWidget* tempWidget = NULL;
	QPushButton* tempButton = NULL;
	QHBoxLayout* tempLayout = NULL;

	QVBoxLayout* mainLayout = new QVBoxLayout();

	tempLayout = new QHBoxLayout();

	_editWidget = new QLineEdit();
	_editWidget->setToolTip(tr("Enter a path here to add to the list."));
	tempLayout->addWidget(_editWidget);

	tempButton = new QPushButton(tr("&Browse"));
	tempButton->setToolTip(tr("Opens a standard directory browser to select a path to add."));
	QObject::connect(tempButton, SIGNAL(clicked(bool)), this, SLOT(browseClicked(bool)));
	tempLayout->addWidget(tempButton);
	tempButton = NULL;

	tempWidget = new QWidget();
	tempWidget->setLayout(tempLayout);
	tempLayout = NULL;

	mainLayout->addWidget(tempWidget);
	tempWidget = NULL;

	tempLayout = new QHBoxLayout();
	_listWidget = new QListWidget();
	_listWidget->setSelectionMode(QAbstractItemView::SingleSelection);
	_listWidget->setToolTip(tr("This is the list of paths to search for screen savers."));
	tempLayout->addWidget(_listWidget);

	QVBoxLayout* sideButtonLayout = new QVBoxLayout();

	tempButton = new QPushButton(tr("&Add"));
	tempButton->setToolTip(tr("Adds the path in the edit box to the list of searched paths."));
	QObject::connect(tempButton, SIGNAL(clicked(bool)), this, SLOT(addClicked(bool)));
	sideButtonLayout->addWidget(tempButton);
	tempButton = NULL;

	tempButton = new QPushButton(tr("&Remove"));
	tempButton->setToolTip(tr("Removes the currently selected path from the list of paths to search."));
	QObject::connect(tempButton, SIGNAL(clicked(bool)), this, SLOT(removeClicked(bool)));
	sideButtonLayout->addWidget(tempButton);
	tempButton = NULL;

	tempWidget = new QWidget();
	tempWidget->setLayout(sideButtonLayout);
	tempLayout->addWidget(tempWidget);
	tempWidget = NULL;

	tempWidget = new QWidget();
	tempWidget->setLayout(tempLayout);
	tempLayout = NULL;

	mainLayout->addWidget(tempWidget);
	tempWidget = NULL;

	tempLayout = new QHBoxLayout();
	tempLayout->addStretch(0);

	tempButton = new QPushButton(tr("&OK"));
	tempButton->setDefault(true);
	QObject::connect(tempButton, SIGNAL(clicked()), this, SLOT(accept()));
	tempLayout->addWidget(tempButton);
	tempButton = NULL;

	tempButton = new QPushButton(tr("&Cancel"));
	QObject::connect(tempButton, SIGNAL(clicked()), this, SLOT(reject()));
	tempLayout->addWidget(tempButton);
	tempButton = NULL;

	tempWidget = new QWidget();
	tempWidget->setLayout(tempLayout);
	tempLayout = NULL;

	mainLayout->addWidget(tempWidget);
	tempWidget = NULL;

	setLayout(mainLayout);

}

void SearchPathDialog::setPathList(QStringList pathList) {
	_pathList = pathList;
	_listWidget->addItems(_pathList);
}

QStringList SearchPathDialog::retrievePathList(void) {
	return _pathList;
}

void SearchPathDialog::accept(void) {
	getListData();
	emit accepted();
	emit finished(1);
	close();
}

void SearchPathDialog::done(int r) {
	getListData();
	emit finished(r);
	close();
}

void SearchPathDialog::reject(void) {
	emit rejected();
	emit finished(0);
	close();
}

void SearchPathDialog::addClicked(bool checked) {
	QString path = _editWidget->text();
	if(path.size() > 0 && _listWidget->findItems(path,Qt::MatchExactly).empty()) {
		_listWidget->addItem(path);
	}
	_editWidget->setText("");
}

void SearchPathDialog::removeClicked(bool checked) {
	int row = _listWidget->currentRow();
	QListWidgetItem* tmp = _listWidget->takeItem(row);
	delete tmp;
	tmp = NULL;
}

void SearchPathDialog::browseClicked(bool checked) {
	QString path = QFileDialog::getExistingDirectory(this,tr("Select Path To Search For Screen Savers"), "",
		QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

	path.replace("/","\\");

	_editWidget->setText(path);
}

//transfers from the list widget to the string list.
void SearchPathDialog::getListData() {
	_pathList.clear();
	for(int i = 0; i < _listWidget->count(); i++) {
		_pathList << _listWidget->item(i)->text();
	}
}

