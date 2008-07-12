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

#include "HistoryList.h"

/*
 * Constructor takes QStringLists of the screen savers and 
 * the history list.
 */
HistoryList::HistoryList(QStringList* scrList, QStringList* historyList, int maxHistorySize) {
	//defaults
	_list = NULL;
	_maxSize = 0;
	_currentSize = 0;
	_scrList = scrList;

	//create space for the list
	setHistoryMaxSize(maxHistorySize);

	//look up each entry in the history list in the scr list.
	if(historyList != NULL) {
		int hiLength = historyList->size();
		int numEntries = (hiLength < maxHistorySize) ? hiLength : maxHistorySize;
		for(int _currentSize=0;	_currentSize < numEntries; _currentSize++) {
				_list[_currentSize] = scrList->indexOf(historyList->at(_currentSize));
		
		}
	}
}

HistoryList::~HistoryList(){
	if(_scrList != NULL)
		_scrList = NULL;

	if(_list != NULL) {
		delete [] _list;
		_list = NULL;
	}
}

/*
 * Turns the history array of indices back into a QStringList.
 */
QStringList HistoryList::convHistoryToStringList() {
	QStringList retVal;
	if(_scrList == NULL || _list == NULL) {
		return retVal;
	}

	for(int i=0;i<_currentSize;i++) {
		//look up the index in the list in the list of
		//screen savers and append it to the QStringList;
		retVal.append(_scrList->at(_list[i]));
	}

	return retVal;
}

/*
 * Shift an item into the history list.
 */
void HistoryList::addScrToHistory(int scrIndex) {
	//start at the end of the list shifting down.
	if(_currentSize < _maxSize)
			_currentSize++;
	for(int i=_currentSize-1;i>0;i--) {
		_list[i] = _list[i-1];
	}
	_list[0] = scrIndex;
}

/*
 * Search for index in the history list.
 */
bool HistoryList::isInHistory(int scrIndex) {
	if(_list == NULL) {
		return false;
	}

	for(int i=0;i<_currentSize;i++) {
		if(_list[i] == scrIndex) {
			return true;
		}
	}

	return false;
}

/*
 * Retrieves the current number of entries in the history list.
 */
int HistoryList::getHistorySize() {
	return _currentSize;
}

/*
 * Retrieves the maximum size of the history list.
 */
int HistoryList::getHistoryMaxSize() {
	return _maxSize;
}

/*
 * Changes the max size of history list.
 */
void HistoryList::setHistoryMaxSize(int size) {
	if(size <= 0) {
		if(_list != NULL) {
			delete [] _list;
			_list = NULL;
		}
		_maxSize = 0;
		_currentSize = 0;
		return;
	}

	int* newList = new int[size];
	int* oldList = _list;

	//copy & delete old list
	if(oldList != NULL) {
		for(int i=0; i < size; i++) {
			if(i < _currentSize) {
				newList[i] = oldList[i];
			} else {
				newList[i] = -1;
			}
		}
		delete [] oldList;
		oldList = NULL;
	}

	//set pointer and sizes
	_maxSize = size;
	_currentSize = (size < _currentSize) ? size : _currentSize;
	_list = newList;
}
/*
 * Retrieves the index at the specified history index or -1 if
 * the specified history index is not valid.
 */
int HistoryList::getAt(int index) {
	if(index < _currentSize) {
		return _list[index];
	}
	return -1;
}

/**
 * Empties the list.
 */
void HistoryList::clear() {
	_currentSize = 0;
}

/**
 * Attempts to load from the specified QDomElement.
 */
void HistoryList::load(QDomElement &root) {
	QDomElement tempElem;

	tempElem = root.firstChildElement("scr_history_size");
	if(!tempElem.isNull()) {
		setHistoryMaxSize(tempElem.text().toInt());
	}

	//Read in the history list.  We want to preserve the order of this list, indicated by the
	//stored location.  We will start with the last node in the hopes that the order is preserved
	//in the XML file and during read back.
	QDomElement tempNode = root.firstChildElement("scr_history").firstChildElement("history_entry");
	while(!tempNode.isNull()) {	
		QString path;
		int index;

		tempElem = tempNode.firstChildElement("path");
		if(!tempElem.isNull()) {
			path = tempElem.text();
		}

		tempElem = tempNode.firstChildElement("location");
		if(!tempElem.isNull()) {
			index = tempElem.text().toInt();
		}

		//insert the string at the appropriate location
		if(_list != NULL && index < _maxSize) {
			_list[index] = _scrList->indexOf(path);
			if(index >= _currentSize)
				_currentSize = index+1;
		}
		tempNode = tempNode.nextSiblingElement("history_entry");
	}
}

/**
 * Attempts to save to the specified QDomElement.
 */
void HistoryList::save(QDomDocument& doc, QDomElement& root) {
	if(_list == NULL || _scrList == NULL)
		return;

	QString tempStr;
	QDomText tempNode;
	QDomElement tempElem;

	// history size
	tempStr.setNum(_maxSize);
	tempNode = doc.createTextNode(tempStr);
	tempElem = doc.createElement("scr_history_size");
	tempElem.appendChild(tempNode);
	root.appendChild(tempElem);
//QMessageBox::information(NULL, "search_path", "blargh", QMessageBox::Ok);

	//history list
	QDomElement historyElem = doc.createElement("scr_history");
	QDomElement entryElem;
	for(int i=0;i<_currentSize; i++) {
		entryElem = doc.createElement("history_entry");
		//path
		tempNode = doc.createTextNode(_scrList->at(_list[i]));
		tempElem = doc.createElement("path");
		tempElem.appendChild(tempNode);
		entryElem.appendChild(tempElem);

		//history location
		tempStr.setNum(i);
		tempNode = doc.createTextNode(tempStr);
		tempElem = doc.createElement("location");
		tempElem.appendChild(tempNode);
		entryElem.appendChild(tempElem);

		historyElem.appendChild(entryElem);
	}
	root.appendChild(historyElem);
}
