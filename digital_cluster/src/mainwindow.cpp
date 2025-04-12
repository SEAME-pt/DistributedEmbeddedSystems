#include "../include/mainwindow.h"
#include <iostream>
#include <QDir>
#include <QCoreApplication>
#include <QFontDatabase>
#include <unistd.h>

//adding layouts and widgets to main window
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
     , client(new QMqttClient(this))
{
    setStyleSheet("background-color: rgb(0, 0, 20);");
    left_dial = new Speed(this);
    right_dial = new Battery(this);
    QHBoxLayout* layout = new QHBoxLayout(); 
    layout->addWidget(left_dial, 1,  Qt::AlignTop | Qt::AlignLeft); 
    
    center_dial = new Lane(this);
    QVBoxLayout* centerLayout = new QVBoxLayout();
    centerLayout->addWidget(center_dial, 0, Qt::AlignCenter);
    layout->addLayout(centerLayout, 1);
    
    layout->addWidget(right_dial, 1, Qt::AlignTop | Qt::AlignRight);
    QVBoxLayout* mainlayout = new QVBoxLayout();
    mainlayout->addLayout(layout, 2);

    temp = new Temperature(this);
    autonomy = new Autonomy(this);
    QHBoxLayout* layoutbar = new QHBoxLayout();
    layoutbar->setSpacing(width() / 20);
    layoutbar->addWidget(temp, 0, Qt::AlignBottom | Qt::AlignRight);
    layoutbar->addWidget(autonomy, 0, Qt::AlignBottom | Qt::AlignLeft);
    mainlayout->addLayout(layoutbar, 1);
    QWidget* centralWidget = new QWidget(this);
    centralWidget->setLayout(mainlayout);
    setCentralWidget(centralWidget);
    init_mqtt();
}

//close main window at destruction
MainWindow::~MainWindow()
{
    close();
}

//connecting to mqtt via cloud or localhost or to jetracer via network
void    MainWindow::init_mqtt()
{
    QString user = qgetenv("user");
    client->setUsername(user); 
    QString pass = qgetenv("password");
    client->setPassword(pass); 
    client->setHostname("972e24210b544ba49bfb9c1d3164d02b.s1.eu.hivemq.cloud"); //cloud
    client->setPort(8883);
    // client->setHostname("10.21.221.67"); //when on the same network
    // client->setPort(1883); //cross compiling
    // client->setHostname("127.0.0.1"); //when cross-compiling with jetracer

    connect(client, &QMqttClient::connected, this, &MainWindow::connected);
    connect(client, &QMqttClient::messageReceived, this, &MainWindow::message_received);
    connect(client, &QMqttClient::errorChanged, this, [](QMqttClient::ClientError error) {
        qDebug() << "MQTT Client error:" << error;
    });
    client->connectToHostEncrypted(); //for cloud needs to be encrypted, for jetracer network or localhost its not encrypted
}

//subscribing to topic of mqtt
void    MainWindow::connected()
{
    QMqttTopicFilter topic("jetracer/speed");
    auto speed_sub = client->subscribe(topic);
    QMqttTopicFilter battery("jetracer/battery");
    auto bat_sub = client->subscribe(battery);
    QMqttTopicFilter temp("jetracer/temperature");
    auto temp_sub = client->subscribe(temp);
    QMqttTopicFilter autono("jetracer/autonomy");
    auto autono_sub = client->subscribe(autono);
    QMqttTopicFilter lane("jetracer/lane_touch");
    auto lane_sub = client->subscribe(lane);
    if (!speed_sub || !bat_sub | !autono_sub || !temp_sub || !lane_sub) {  
        qDebug() << "Failed to subscribe to topic";
    } 
}

//receiving message and updating current
void    MainWindow::message_received(const QByteArray &message, const QMqttTopicName &topic)
{
    qDebug() << "Message received on topic" << topic.name() << ":" << message;
    bool ok;
    double msg = message.toDouble(&ok); 
    if (ok) {
        if (topic.name() == "jetracer/speed") {
            QMetaObject::invokeMethod(this, [this, msg]() {
                left_dial->set_current(static_cast<float>(msg));
            }, Qt::AutoConnection);
        }
        else if (topic.name() == "jetracer/battery") {
            QMetaObject::invokeMethod(this, [this, msg]() {
                right_dial->set_current(msg);
            }, Qt::AutoConnection);
        }
        else if (topic.name() == "jetracer/temperature") {
            QMetaObject::invokeMethod(this, [this, msg]() {
                temp->set_temperature(msg);
            }, Qt::AutoConnection);
        }
        else if (topic.name() == "jetracer/autonomy") {
            QMetaObject::invokeMethod(this, [this, msg]() {
                autonomy->set_autonomy(msg);
            }, Qt::AutoConnection);
        }
        else if (topic.name() == "jetracer/lane_touch") {
            QMetaObject::invokeMethod(this, [this, msg]() {
                center_dial->set_res(msg);
                center_dial->update();
            }, Qt::AutoConnection);
        }
    } else {
        qDebug() << "Invalid data received";
    }
}

QMqttClient*    MainWindow::get_client()
{
    return client;
}

Battery*    MainWindow::get_battery()
{
    return right_dial;
}

Autonomy*   MainWindow::get_autonomy()
{
    std::cout << "Getting autonomy\n";
    return autonomy;
}

Temperature*   MainWindow::get_temperature()
{
    return temp;
}

Lane*   MainWindow::get_lane()
{
    return center_dial;
}