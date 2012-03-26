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

private slots:
    void on_itemFormat_textChanged(const QString &text);

    void on_resetFormatButton_clicked();

private:
    Ui::SettingsDialog *ui;
};

#endif // SETTINGSDIALOG_H
