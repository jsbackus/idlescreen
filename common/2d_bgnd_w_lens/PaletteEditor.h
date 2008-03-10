/**
 * This class represents a dialog box that allows the user to add/edit/delete
 * all available palettes.  Utilizes IndexedPaletteDialog for actual
 * modification of the palettes.  Does not have the ability to undo!
 */

#ifndef __PALETTEEDITOR_H__
#define __PALETTEEDITOR_H__

#include <QDialog>
//#include <QListWidget>

#include "PaletteSelectWidget.h"
#include "ConfigManager.h"
#include "../IndexedPalette/IndexedPaletteDialog.h"

class PaletteEditor : public QDialog {

	Q_OBJECT

public:
	PaletteEditor(ConfigManager* confMgr = NULL, QWidget* parent = 0, Qt::WindowFlags f = 0);
	~PaletteEditor();

public slots:
	void closeClicked(bool checked = false);
	void done(int r = 0);

//signals:
	//void paletteUpdated(void);

private:

	//QHash<QString, IndexedPaletteProfile*>* _paletteHash;
	//QListWidget* _listWidget;
	PaletteSelectWidget* _listWidget;

	IndexedPaletteDialog* _palDlg;

	QString _palOldName;

	ConfigManager* _confMgr;
	QString _windowTitle;
};

#endif