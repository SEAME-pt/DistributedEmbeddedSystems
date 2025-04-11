#include "./include/mainwindow.h"
#include <QApplication>
#include <QLocale>
#include <QTimer>
#include <unistd.h>
#include <iostream>
#include <csignal>

MainWindow *w = nullptr;


void cleanup(int sig) {
    if (w) {
        w->close();  // Close the main window
    }
    QApplication::quit();  // Quit the application
}


int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_UseHighDpiPixmaps);
    app.setOverrideCursor(QCursor(Qt::BlankCursor));
    MainWindow window;
    w = &window; 
    w->setFixedSize(1280, 400);
    w->setWindowState(Qt::WindowFullScreen);
    w->show();
    std::signal(SIGINT, cleanup); 
    std::signal(SIGTSTP, cleanup); 
    return app.exec();
}
