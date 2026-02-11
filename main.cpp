#include <QApplication>
#include <QCommandLineParser>
#include <unistd.h>
#include <cstring>
#include "trayapp.h"

int main(int argc, char *argv[]) {
    // Fork before Qt connects to the display server
    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "--detached") == 0) {
            pid_t pid = fork();
            if (pid < 0)
                return 1;
            if (pid > 0)
                return 0;
            setsid();
            break;
        }
    }

    QApplication app(argc, argv);
    app.setApplicationName("scrcpybud");
    app.setQuitOnLastWindowClosed(false);

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addOption({"detached", "Run in the background (fork to background)."});
    parser.process(app);

    TrayApp tray;
    return app.exec();
}
