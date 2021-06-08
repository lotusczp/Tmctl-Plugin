#include "LTmctlSettingsWidget.h"
#include "ui_LTmctlSettingsWidget.h"

LTmctlSettingsWidget::LTmctlSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LTmctlSettingsWidget)
{
    ui->setupUi(this);
    ui->buttonConnect->setEnabled(true);
    ui->buttonDisconnect->setEnabled(false);
}

LTmctlSettingsWidget::~LTmctlSettingsWidget()
{
    delete ui;
}

void LTmctlSettingsWidget::receiveDeviceConnected()
{
    ui->buttonConnect->setEnabled(false);
    ui->buttonDisconnect->setEnabled(true);
}

void LTmctlSettingsWidget::on_buttonConnect_clicked()
{
    emit sendScanInterval(ui->lineEdit->text().toInt()<=0 ? 200 : ui->lineEdit->text().toInt());
    emit tryConnect(ui->lineEdit->text());
}

void LTmctlSettingsWidget::on_buttonDisconnect_clicked()
{
    emit tryDisconnect();

    ui->buttonConnect->setEnabled(true);
    ui->buttonDisconnect->setEnabled(false);
}
