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

/**
 * This class maintains the history list.  Implemented as an array of
 * indices of the list of screen savers.
 *
 * @author jeff backus
 * @date 06/15/07
 */

#ifndef __HISTORYLIST_H__
#define __HISTORYLIST_H__

#include <QStringList>
#include <QString>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>

class HistoryList {

public:
	/*
	 * Constructor takes QStringLists of the screen savers and 
	 * the history list.
	 */
	HistoryList(QStringList* scrList, QStringList* historyList=NULL, int maxHistorySize=0);
	~HistoryList();

	/**
	 * Attempts to load from the specified QDomElement.
	 */
	void load(QDomElement &root);

	/**
	 * Attempts to save to the specified QDomElement.
	 */
	void save(QDomDocument& doc, QDomElement& root);

	/*
	 * Turns the history array of indices back into a QStringList.
	 */
	QStringList convHistoryToStringList();

	/*
	 * Shift an item into the history list.
	 */
	void addScrToHistory(int scrIndex);

	/*
	 * Search for index in the history list.
	 */
	bool isInHistory(int scrIndex);

	/*
	 * Retrieves the current number of entries in the history list.
	 */
	int getHistorySize();

	/*
	 * Retrieves the maximum size of the history list.
	 */
	int getHistoryMaxSize();

	/*
	 * Changes the max size of history list.
	 */
	void setHistoryMaxSize(int size);

	/*
	 * Retrieves the index at the specified history index.
	 */
	int getAt(int index);

	/**
	 * Empties the list.
	 */
	void clear();

private:
	int _maxSize;
	int _currentSize;
	int* _list;
	QStringList* _scrList;
};

#endif