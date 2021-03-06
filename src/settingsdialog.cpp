#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "settings.h"

SettingsDialog::SettingsDialog(const Settings& settings, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    ui->systemDisks->setChecked(settings.showSystemDisks);
    ui->hideIcon->setChecked(settings.hideIcon);
    ui->autoMount->setChecked(settings.mountAutomaticaly);

    ui->deviceNotify->setChecked(settings.deviceNotifications);
    ui->mountNotify->setChecked(settings.mountNotifications);

    ui->itemFormat->setText(settings.itemFormat);
#ifdef WITH_LIBNOTIFY
    useLibNotify = new QCheckBox(tr("Use system notifications"));
    useLibNotify->setChecked(settings.useLibnotify);
    ui->notifyGroupBox->layout()->addWidget(useLibNotify);
#endif

    ui->detach->setChecked(settings.detachRemovable);
    ui->useForce->setChecked(settings.forceUnmount);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

Settings SettingsDialog::getSettings()
{
    Settings settings;
    settings.showSystemDisks = ui->systemDisks->isChecked();
    settings.hideIcon = ui->hideIcon->isChecked();
    settings.mountAutomaticaly = ui->autoMount->isChecked();

    settings.deviceNotifications = ui->deviceNotify->isChecked();
    settings.mountNotifications = ui->deviceNotify->isChecked();

#ifdef WITH_LIBNOTIFY
    settings.useLibnotify = useLibNotify->isChecked();
#endif

    settings.itemFormat = ui->itemFormat->text();

    settings.detachRemovable = ui->detach->isChecked();
    settings.forceUnmount = ui->useForce->isChecked();

    return settings;
}

void SettingsDialog::on_itemFormat_textChanged(const QString &text)
{
    ui->buttonBox->setEnabled(!text.isEmpty());
}

void SettingsDialog::on_resetFormatButton_clicked()
{
    ui->itemFormat->setText(SettingsManager::defaultItemFormat());
}
