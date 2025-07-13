#include "datareceiver.h"

DataReceiver::DataReceiver(int port)
{
    this->port = port;
    socket = new QUdpSocket(this);
    socket->bind(QHostAddress::Any,quint16(port));
    //включаем привязку сокета к порту our_port и принимаем сообщения с любых адресов
    connect(socket,&QUdpSocket::readyRead,this, &DataReceiver::read_data);
    //соединяем сигнал готовности данных с нашим слотом обаботки
}

void DataReceiver::read_data()
{
    char * data;
    int size;
    size = int (socket->pendingDatagramSize());//уточняем обьем данных в очереди
    QString stroka;
    if (size>0)
    {
        data = new char [size+1];
        socket->readDatagram(data,size);//читаем данные
        data[size]=0;//завершающий 0 - признак конца строки
        //так как передаем строки. МОжно терминировать нулесм при передаче (даже правильнее)
        stroka = QString::fromUtf8(data,size);
        delete [] data;
    }


    // Создаем объект интерфейса для работы с графикой
    ScriptInterface *interface = new ScriptInterface();

    // Инициализируем движок JavaScript
    QJSEngine engine;

    // Создаем JS-обертку для нашего интерфейса
    QJSValue obj = engine.newQObject(interface);

    // Регистрируем функции рисования в глобальном пространстве JS
    engine.globalObject().setProperty("drawFilledCircle", obj.property("drawFilledCircle"));
    engine.globalObject().setProperty("drawCircle", obj.property("drawCircle"));
    engine.globalObject().setProperty("drawTriangle", obj.property("drawTriangle"));
    engine.globalObject().setProperty("drawRectangle", obj.property("drawRectangle"));
    engine.globalObject().setProperty("drawLine", obj.property("drawLine"));

    // Выполняем JS-код, содержащий команды рисования
    engine.evaluate(stroka);  // stroka содержит скрипт с командами

    // Отправляем сигнал с готовыми фигурами для отрисовки
    emit shapesReady(interface->shapes);

}


