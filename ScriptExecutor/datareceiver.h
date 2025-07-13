#ifndef DATARECEIVER_H
#define DATARECEIVER_H
#pragma once
#include <QObject>
#include <QtNetwork/QUdpSocket>//библиотека для работы с UDP
#include <QHostInfo>// для проверки IP
#include <QNetworkInterface>
#include <QJSEngine>
#include <QPainter>
#include "Shape.h"
#include "scriptinterface.h"



class DataReceiver : public QObject
{
    Q_OBJECT
public:
    explicit DataReceiver(int port);

    void read_data();


private:
    QUdpSocket* socket;
    QHostAddress* adress;
    int port, our_port;

signals:
    void shapesReady(const QList<Shape>&);
};

#endif // DATARECEIVER_H
