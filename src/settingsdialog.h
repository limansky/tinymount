#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
    class SettingsDialog;
}

class Settings;

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(const Settings& settings, QWidget *parent = 0);
    ~SettingsDialog();

    Settings getSettings();

private:
    Ui::SettingsDialog *ui;
};

#endif // SETTINGSDIALOG_H
