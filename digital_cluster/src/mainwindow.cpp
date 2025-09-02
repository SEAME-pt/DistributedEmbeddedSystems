#include "../include/mainwindow.h"

//adding layouts and widgets to main window
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), client(new QMqttClient(this))
{
    left_dial = new Speed(this);
    right_dial = new Battery(this);
    object = new Object(this);
    temp = new Temperature(this);
    autonomy = new Autonomy(this);
    center_dial = new Lane(this);
    QVBoxLayout* mainlayout = new QVBoxLayout();
    QHBoxLayout* layout = new QHBoxLayout(); 
    QVBoxLayout* lane_layout = new QVBoxLayout();
    QHBoxLayout* centerbar = new QHBoxLayout();
    QWidget* centralWidget = new QWidget(this);
    
    layout->addWidget(left_dial, 1,  Qt::AlignTop | Qt::AlignLeft); 
    lane_layout->addWidget(center_dial, 0, Qt::AlignCenter);
    lane_layout->setStretch(0, 1);
    layout->addLayout(lane_layout, 1);
    layout->addWidget(right_dial, 1, Qt::AlignTop | Qt::AlignRight);
    
    // Create layouts
    object->setFixedSize(60, 60);

    // Create a main container for the entire bar
    QWidget *barContainer = new QWidget();
    QHBoxLayout *barLayout = new QHBoxLayout(barContainer);
    barLayout->setContentsMargins(20, 0, 80, 0); // Left/right margins

    // Left section with object
    barLayout->addWidget(object, 0, Qt::AlignLeft);

    // Center section with temp and autonomy
    QWidget *centerWidget = new QWidget();
    QHBoxLayout *centerLayout = new QHBoxLayout(centerWidget);
    centerLayout->setContentsMargins(0, 0, 0, 0);
    centerLayout->addStretch(1);
    temp->setFixedHeight(80);       // or whatever looks right
    autonomy->setFixedHeight(80);   // match temp

    centerLayout->addWidget(temp, 0, Qt::AlignTop);
    centerLayout->addSpacing(30);
    centerLayout->addWidget(autonomy, 0, Qt::AlignTop);
    centerLayout->addStretch(1);

    barLayout->addWidget(centerWidget, 1); // Take remaining space

    mainlayout->addLayout(layout, 2);
    mainlayout->addWidget(barContainer, 1); // Add the container widget

    centralWidget->setLayout(mainlayout);
    setCentralWidget(centralWidget);
    setStyleSheet("background-color: rgb(0, 0, 20);");
    init_mqtt();
}

//close main window at destruction
MainWindow::~MainWindow()
{
    std::cout << "Remove Window" << std::endl;
    close();
}

//connecting to mqtt via cloud or localhost or to jetracer via network
void    MainWindow::init_mqtt()
{
    // client->setHostname("972e24210b544ba49bfb9c1d3164d02b.s1.eu.hivemq.cloud"); //cloud
    // client->setPort(8883);
    QString user = qgetenv("user");
    client->setUsername(user); 
    QString pass = qgetenv("password");
    client->setPassword(pass); 
    // client->setHostname("10.21.221.67"); //when on the same network
    client->setPort(1883); //cross compiling
    client->setHostname("127.0.0.1"); //when cross-compiling with jetracer

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
    QMqttTopicFilter speed50("jetracer/speed_50");
    auto speed50_sub = client->subscribe(speed50);
    QMqttTopicFilter speed80("jetracer/speed_80");
    auto speed80_sub = client->subscribe(speed80);
    if (!speed_sub || !bat_sub | !autono_sub || !temp_sub || !lane_sub || !speed50_sub || !speed80_sub) {
        qDebug() << "Failed to subscribe to topic";
    } 
}

//receiving message and updating current
void    MainWindow::message_received(const QByteArray &message, const QMqttTopicName &topic)
{
    qDebug() << topic.name() << ":" << message;
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
                center_dial->set_lane(msg);
            }, Qt::AutoConnection);
        }
        else if (topic.name() == "jetracer/speed_50" || topic.name() == "jetracer/speed_80") {
            QMetaObject::invokeMethod(this, [this, msg, topic]() {
                object->set_object(msg, topic.name());
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

Speed*    MainWindow::get_speed()
{
    return left_dial;
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

Object*   MainWindow::get_object()
{
    return object;
}