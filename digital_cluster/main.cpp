#include "./include/mainwindow.h"
#include <csignal>

QApplication* app = nullptr;

void cleanup(int) {
    if (app) {
        app->quit();
    }
}

int main(int argc, char *argv[]) {
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);
    app = &a;
    a.setAttribute(Qt::AA_UseHighDpiPixmaps);
    a.setOverrideCursor(QCursor(Qt::BlankCursor));
    MainWindow window;
    window.setFixedSize(1280, 400);
    window.setWindowState(Qt::WindowFullScreen);
    window.show();
    std::signal(SIGINT, cleanup);
    std::signal(SIGTSTP, cleanup);
    int result = a.exec();
    app = nullptr;
    return result;
}