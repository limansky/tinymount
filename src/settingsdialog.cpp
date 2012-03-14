#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "settings.h"

SettingsDialog::SettingsDialog(const Settings& settings, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    ui->systemDisks->setChecked(settings.showSystemDisks);
    ui->autoMount->setChecked(settings.mountAutomaticaly);

    ui->deviceNotify->setChecked(settings.deviceNotifications);
    ui->mountNotify->setChecked(settings.mountNotifications);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

Settings SettingsDialog::getSettings()
{
    Settings settings;
    settings.showSystemDisks = ui->systemDisks->isChecked();
    settings.mountAutomaticaly = ui->autoMount->isChecked();
    settings.deviceNotifications = ui->deviceNotify->isChecked();
    settings.mountNotifications = ui->deviceNotify->isChecked();
    return settings;
}
