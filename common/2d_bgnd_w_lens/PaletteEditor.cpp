
#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QInputDialog>

#include "PaletteEditor.h"

PaletteEditor::PaletteEditor(ConfigManager* confMgr, QWidget* parent, Qt::WindowFlags f) {
	QDialog(parent, f);
	_windowTitle = tr("Palette Editor");
	setWindowTitle(_windowTitle);
	setWindowModality(Qt::WindowModal);
	QIcon icon(":/app_icon.png");
	setWindowIcon(icon);

	_listWidget = NULL;
	_confMgr = confMgr;
	_palDlg = NULL;

	//populate dialog box
	QWidget* tempWidget = NULL;
	QPushButton* tempButton = NULL;
	QHBoxLayout* tempLayout = NULL;

	QVBoxLayout* mainLayout = new QVBoxLayout();

	_listWidget = new PaletteSelectWidget(confMgr);
	mainLayout->addWidget(_listWidget);

	tempLayout = new QHBoxLayout();

	tempLayout->addStretch(0);

	tempButton = new QPushButton(tr("&Close"));
	tempButton->setToolTip(tr("Close this dialog box."));
	tempButton->setDefault(true);
	QObject::connect(tempButton, SIGNAL(clicked(bool)), this, SLOT(closeClicked(bool)));
	tempLayout->addWidget(tempButton);
	tempButton = NULL;

	tempLayout->addStretch(0);

	tempWidget = new QWidget();
	tempWidget->setLayout(tempLayout);
	tempLayout = NULL;

	mainLayout->addWidget(tempWidget);
	tempWidget = NULL;

	setLayout(mainLayout);
}
PaletteEditor::~PaletteEditor() {

	//the layout should delete the object.
	_listWidget = NULL;

	/*
	if(_listWidget != NULL) {
		delete _listWidget;
		_listWidget = NULL;
	}
	*/

	if(_palDlg != NULL) {
		delete _palDlg;
		_palDlg = NULL;
	}

	_confMgr = NULL;
}

void PaletteEditor::closeClicked(bool checked) {
	close();
	emit finished(0);
//	close();
}

void PaletteEditor::done(int r) {
	close();
	emit finished(r);
//	close();
}
