#include <QApplication>
#include <QCommandLineParser>
#include <unistd.h>
#include "trayapp.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("scrcpybud");
    app.setQuitOnLastWindowClosed(false);

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addOption({"detached", "Run in the background (fork to background)."});
    parser.process(app);

    if (parser.isSet("detached")) {
        pid_t pid = fork();
        if (pid < 0) {
            return 1;
        }
        if (pid > 0) {
            // parent exits immediately
            return 0;
        }
        // child continues as daemon? hopefully
        setsid();
    }

    TrayApp tray;
    return app.exec();
}
