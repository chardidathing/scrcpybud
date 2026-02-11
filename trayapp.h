#pragma once

#include <QSystemTrayIcon>
#include <QMenu>
#include <QProcess>
#include <QMap>

class TrayApp : public QObject {
    Q_OBJECT

public:
    explicit TrayApp(QObject *parent = nullptr);

private slots:
    void refreshDevices();
    void launchScrcpy();
    void onTrayActivated(QSystemTrayIcon::ActivationReason reason);
    void quit();

private:
    void buildMenu();

    QSystemTrayIcon *m_tray;
    QMenu *m_menu;
    QMap<QString, QString> m_devices; // serial -> model/description
};
