#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <cstdio>
#include <QTimer>
#include <QProcess>
#include <cstdlib>
#include <QThread>
#include "customdial.h"
#include "batterydial.h"
#include <QVBoxLayout>
#include <QtMqtt/QtMqtt>
#include <QtMqtt/QMqttClient>
#include <QtMqtt/QMqttTopicFilter>
#include <QtMqtt/QMqttMessage>
#include <QtMqtt>
#include <QApplication>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    private slots:
    //void publishMessage();  
    void onMqttConnected();
    void onMessageReceived(const QByteArray &message, const QMqttTopicName &topic); // Declaração para receber mensagens
    void onMqttStateChanged(QMqttClient::ClientState state);
    void init_mqtt();

private:
    CustomDial *left_dial = nullptr;
    BatteryDial *right_dial = nullptr;
    QVBoxLayout *layout = nullptr;
    QWidget *centralWidget = nullptr;
    QMqttClient *mqttClient;
};
#endif // MAINWINDOW_H
