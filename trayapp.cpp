// trayapp.cpp - little friend for scrcpy :)
// Date: 12/02/2026 (DD/MM/YYYY)
// Created by: Charlie (chardidathing)

#include "trayapp.h"
#include <QApplication>
#include <QProcess>
#include <QIcon>
#include <QRegularExpression>
#include <QCursor>
#include <QGuiApplication>

TrayApp::TrayApp(QObject *parent) : QObject(parent) {
    m_tray = new QSystemTrayIcon(QIcon::fromTheme("phone"), this);
    m_menu = new QMenu();
    m_tray->setContextMenu(m_menu);
    m_tray->setToolTip("scrcpybud");

    connect(m_tray, &QSystemTrayIcon::activated, this, &TrayApp::onTrayActivated);

    m_tray->show();
    refreshDevices();
}

void TrayApp::refreshDevices() {
    m_devices.clear();

    QProcess proc;
    proc.start("adb", {"devices", "-l"});
    proc.waitForFinished(5000);

    const QString output = QString::fromUtf8(proc.readAllStandardOutput());
    const QStringList lines = output.split('\n', Qt::SkipEmptyParts);

    for (const QString &line : lines) {
        if (line.startsWith("List") || line.trimmed().isEmpty())
            continue;

        const QStringList parts = line.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
        if (parts.size() < 2 || parts[1] != "device")
            continue;

        const QString serial = parts[0];
        QString model = serial;

        // try to extract model (e.g. model:Pixel_10)
        for (const QString &part : parts) {
            if (part.startsWith("model:")) {
                model = part.mid(6).replace('_', ' ');
                break;
            }
        }

        m_devices.insert(serial, model);
    }

    buildMenu();
}

void TrayApp::buildMenu() {
    m_menu->clear();

    if (m_devices.isEmpty()) {
        m_menu->addAction("No devices found")->setEnabled(false);
    } else {
        for (auto it = m_devices.cbegin(); it != m_devices.cend(); ++it) {
            QAction *action = m_menu->addAction(
                QString("%1 (%2)").arg(it.value(), it.key()));
            action->setData(it.key());
            connect(action, &QAction::triggered, this, &TrayApp::launchScrcpy);
        }
    }

    m_menu->addSeparator();
    m_menu->addAction("Refresh Devices", this, &TrayApp::refreshDevices);
    m_menu->addSeparator();
    m_menu->addAction("Quit", this, &TrayApp::quit);
}

void TrayApp::launchScrcpy() {
    auto *action = qobject_cast<QAction *>(sender());
    if (!action)
        return;

    const QString serial = action->data().toString();
    QProcess::startDetached("scrcpy", {"-s", serial});
}

void TrayApp::onTrayActivated(QSystemTrayIcon::ActivationReason reason) {
    if (reason == QSystemTrayIcon::Trigger) {
        // tried 2 fix left click on wayland,,,, i'm stupid idk
        if (QGuiApplication::platformName() != "wayland")
            m_menu->popup(QCursor::pos());
    }
}

void TrayApp::quit() {
    QApplication::quit();
}
