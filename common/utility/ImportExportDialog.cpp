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
#include <QTreeWidgetItem>
#include <QList>

#include "ImportExportDialog.h"

ImportExportDialog::ImportExportDialog(bool bImport, QWidget* parent, Qt::WindowFlags f) {
	_tree = NULL;
	_bImport = bImport;

	//set up dialog box
	QWidget* tempWidget = NULL;
	QPushButton* tempButton = NULL;
	QHBoxLayout* tmpLayout = NULL;
	QVBoxLayout* mainLayout = new QVBoxLayout();
	if(mainLayout == NULL)
		return;
	QString tempToolTip;

	// tree widget
	_tree = new QTreeWidget();
	if(_tree == NULL)
		return;
	QObject::connect(_tree, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(itemChanged(QTreeWidgetItem*, int)));
	_tree->setHeaderLabel("");
	mainLayout->addWidget(_tree);

	// button layout
	tmpLayout = new QHBoxLayout();
	if(tmpLayout == NULL)
		return;
	
	tmpLayout->addStretch(0);

	if(bImport) {
		tempButton = new QPushButton(tr("&Import"));
	} else {
		tempButton = new QPushButton(tr("&Export"));
	}
	if(tempButton == NULL)
		return;
	tempButton->setDefault(true);
	QObject::connect(tempButton, SIGNAL(clicked(bool)), this, SLOT(okClicked(bool)));
	tmpLayout->addWidget(tempButton);
	tempButton = NULL;

	tempButton = new QPushButton(tr("&Cancel"));
	if(tempButton == NULL)
		return;
	tempButton->setDefault(true);
	QObject::connect(tempButton, SIGNAL(clicked(bool)), this, SLOT(cancelClicked(bool)));
	tmpLayout->addWidget(tempButton);
	tempButton = NULL;

	tempWidget = new QWidget();
	tempWidget->setLayout(tmpLayout);
	mainLayout->addWidget(tempWidget);
	tempWidget = NULL;

	setLayout(mainLayout);
}

ImportExportDialog::~ImportExportDialog() {
	if(_tree != NULL) {
		delete _tree;
		_tree = NULL;
	}
}

// delete items in the list
void ImportExportDialog::clearAll() {
	if(_tree == NULL)
		return;
	_tree->clear();
}

void ImportExportDialog::clearSubItems(const QString itemName) {
	if(_tree == NULL)
		return;
	QList<QTreeWidgetItem*> list = _tree->findItems(itemName, Qt::MatchFixedString | Qt::MatchCaseSensitive);
	
	// search whole list and remove items from all that are returned.  should only be 1 item.
	for(int i=0; i<list.size();i++) {
		QTreeWidgetItem* tmp = list.at(i);
		if(tmp != NULL) {
			tmp->takeChildren();
		}
	}
}

// adds subitems under the specified item.  If the
// item doesn't exist, it is created.
void ImportExportDialog::addSubItems(const QString itemName, const QStringList subItemNames, const bool bChecked) {
	if(_tree == NULL)
		return;
	QList<QTreeWidgetItem*> list = _tree->findItems(itemName, Qt::MatchFixedString | Qt::MatchCaseSensitive);
	
	// if the list of returned items is 0, create a new item.
	// if it is greater than 1, only grab the first one.
	QTreeWidgetItem* item = NULL;
	if(list.size() == 0) {
		QStringList data;
		data.append(itemName);
		item = new QTreeWidgetItem(data);
		if(item != NULL) {
			if(bChecked) {
				item->setCheckState(0,Qt::Checked);
			} else {
				item->setCheckState(0,Qt::Unchecked);
			}
			_tree->addTopLevelItem(item);
		}
	} else {
		item = list.at(0);
		if( (bChecked && item->checkState(0) == Qt::Unchecked) ||
			(!bChecked && item->checkState(0) == Qt::Checked) ) {
			item->setCheckState(0,Qt::PartiallyChecked);
		}
	}

	// now iterate through the list of subItems, adding each in turn
	QTreeWidgetItem* subItem = NULL;
	for(int i = 0; i<subItemNames.size(); i++) {
		QStringList data;
		data.append(subItemNames.at(i));
		subItem = new QTreeWidgetItem(data);
		if(subItem != NULL) {
			if(bChecked) {
				subItem->setCheckState(0,Qt::Checked);
			} else {
				subItem->setCheckState(0,Qt::Unchecked);
			}
			item->addChild(subItem);
		}
	}
}

// returns a QStringList of all of the subitems under the specified
// item with the specified state.
QStringList ImportExportDialog::getSubItems(const QString itemName, const bool bChecked) {
	QStringList retVal;
	if(_tree == NULL)
		return retVal;

	QList<QTreeWidgetItem*> list = _tree->findItems(itemName, Qt::MatchFixedString | Qt::MatchCaseSensitive);
	
	// search whole list, check each item's children to see if they match
	// the requested state, and append those that are to a QStringList.
	for(int i=0; i<list.size();i++) {
		QTreeWidgetItem* item = list.at(i);
		if(item != NULL) {
			QList<QTreeWidgetItem*> children = item->takeChildren();
			item->addChildren(children);
			for(int j=0;j<children.size();j++) {
				QTreeWidgetItem* subItem = children.at(j);
				if(subItem != NULL) {
					// check to see if this subItem has the same checked state.
					// if so, add it to the list
					if( (bChecked && subItem->checkState(0) == Qt::Checked) ||
						(!bChecked && subItem->checkState(0) == Qt::Unchecked) ) {
						retVal.append(subItem->text(0));
					}
				}				
			}
		}
	}
	return retVal;
}

void ImportExportDialog::okClicked(bool checked) {
	if(_tree == NULL) {
		reject();
	}
	accept();
}

void ImportExportDialog::cancelClicked(bool checked) {
	reject();
}
void ImportExportDialog::itemChanged(QTreeWidgetItem* item, int column) {
	if(item == NULL)
		return;

	QTreeWidgetItem* parent = item->parent();
	
	// if the parent's state doesn't equal this state, set the parent's
	// state to partially checked
	if(parent != NULL && item->checkState(0) != parent->checkState(0)) {
		parent->setCheckState(0, Qt::PartiallyChecked);
	} else if(parent == NULL) {
		// if this item doesn't have a parent, change the state
		// of all children, as long as the state isn't PartiallyChecked
		QList<QTreeWidgetItem*> children = item->takeChildren();
		item->addChildren(children);
		Qt::CheckState state = item->checkState(0);
		if(state != Qt::PartiallyChecked) {
			for(int i=0; i<children.size();i++) {
				QTreeWidgetItem* subItem = children.at(i);
				if(subItem != NULL) {
					subItem->setCheckState(0, state);
				}
			}
		}
	}
}
