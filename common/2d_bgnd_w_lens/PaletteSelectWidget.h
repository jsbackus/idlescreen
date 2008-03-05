/**
 * This widget contains a selection box as well as buttons to add, edit, duplicate, and delete palettes.
 */

#ifndef __PALETTESELECTIONWIDGET_H__
#define __PALETTESELECTIONWIDGET_H__

#include <QWidget>
#include <QString>
#include <QListWidget>

/*
#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>
*/

#include "ConfigManager.h"
#include "../IndexedPalette/IndexedPaletteProfile.h"
#include "../IndexedPalette/IndexedPaletteDialog.h"

class PaletteSelectWidget : public QWidget {

	Q_OBJECT

public:
	PaletteSelectWidget(ConfigManager* _confMgr = NULL, QWidget* parent = 0, Qt::WindowFlags f = 0);
	~PaletteSelectWidget();

	/*
	 * Retrieves the name of the currently selected palette.
	 */
	QString getCurrentPaletteName();

	/*
	 * Sets the currently selected palette.
	 */
	void setCurrentPalette(QString name);

public slots:

	void addClicked(bool checked = false);
	void copyClicked(bool checked = false);
	void editClicked(bool checked = false);
	void removeClicked(bool checked = false);

	void addAccepted(void);
	void editAccepted(void);

signals:
	void selectionChanged(QString newName);

private:
	//populates the list box
	void populateBox(void);

	//searches the list for the specified string, returning its index, or -1.
	int findItem(QString name);

	//attempts to edit the specified palette.
	void editPalette(IndexedPaletteProfile& pal, bool bWasAddRename = false);

	ConfigManager* _confMgr;

	QListWidget* _list;
	
	IndexedPaletteDialog* _palDlg;
	QString _palOldName;
};
#endif