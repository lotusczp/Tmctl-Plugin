#ifndef LTMCTLSETTINGSWIDGET_H
#define LTMCTLSETTINGSWIDGET_H

#include <QWidget>

namespace Ui {
class LTmctlSettingsWidget;
}

class LTmctlSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LTmctlSettingsWidget(QWidget *parent = nullptr);
    ~LTmctlSettingsWidget();

public slots:
    void receiveDeviceConnected();

signals:
    void tryConnect(QString a_strIpAddress);

    void tryDisconnect();

    void sendScanInterval(int a_iScanInterval);

private slots:
    void on_buttonConnect_clicked();

    void on_buttonDisconnect_clicked();

private:
    Ui::LTmctlSettingsWidget *ui;
};

#endif // LTMCTLSETTINGSWIDGET_H
