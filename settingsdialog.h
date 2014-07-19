#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include "QSettings"
namespace Ui {
class settingsDialog;
}

class settingsDialog : public QDialog
{
	Q_OBJECT

public:
	explicit settingsDialog(QWidget *parent = 0);
	~settingsDialog();
	QSettings *getSettings();
	void setSettings(QSettings*);
private:
	Ui::settingsDialog *ui;
	QSettings *defSet,*newSet,*oldSet;

};

#endif // SETTINGSDIALOG_H
