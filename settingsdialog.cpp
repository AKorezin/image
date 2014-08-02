#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "QDebug"
settingsDialog::settingsDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::settingsDialog)
{
	ui->setupUi(this);
	this->setWindowTitle("Настройки");
	newSet=new QSettings;
	defSet=new QSettings;
	oldSet=new QSettings;
}

settingsDialog::~settingsDialog()
{
	delete ui;
}

QSettings *settingsDialog::getSettings()
{

	return oldSet;
}

void settingsDialog::setSettings(QSettings *curSet)
{
	oldSet=curSet;
}
