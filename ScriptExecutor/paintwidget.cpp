#include "paintwidget.h"

bool paintwidget::change = false;

paintwidget::paintwidget(QWidget *parent) : QWidget(parent)
{
    // Создаем приемник данных с идентификатором 55
    datarec = new DataReceiver(55);
    connect(datarec, &DataReceiver::shapesReady, this, &paintwidget::setTmp);
}

//Рисуем фигуры
void paintwidget::paintEvent(QPaintEvent *event)
{
    //Если пользователь не нажал кнопку отрисовать, но выходим
    if (!paintwidget::change)
        return;
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    for (auto it = shapes.begin(); it != shapes.end(); it++)
    {
        const Shape& shape = *it;
        switch (shape.type)
        {
            case Shape::Triangle:
                if (shape.points.size() == 3)
                {
                    QPolygon polygon(shape.points);
                    painter.setBrush(shape.color);
                    painter.setPen(Qt::black);
                    painter.drawPolygon(polygon);
                }
                break;

            case Shape::Circle:
                painter.setBrush(shape.color);
                painter.setPen(QPen(shape.color, 2));
                painter.drawEllipse(shape.center, shape.radius, shape.radius);
                break;

            case Shape::Line:
                qDebug() << "Line";
                if (shape.points.size() == 2) {

                    QPen pen(shape.color, shape.penWidth);
                    painter.setPen(pen);

                    painter.drawLine(shape.points[0], shape.points[1]);
                }
                break;

            case Shape::FilledCircle:
                painter.setBrush(shape.color);
                painter.setPen(Qt::NoPen);
                painter.drawEllipse(shape.center, shape.radius, shape.radius);
                break;

            case Shape::Rectangle:
                if (shape.points.size() == 2) {
                    QRect rect(shape.points[0], shape.points[1]);
                    painter.setBrush(shape.color);
                    painter.setPen(Qt::black);
                    painter.drawRect(rect);
                }
                break;
        }
    }
}
// Устанавливает новые фигуры для отображения
void paintwidget::setShapes()
{
    shapes.clear();
    this->shapes = tmp;
}
// Принимает новые фигуры от DataReceiver
void paintwidget::setTmp(const QList<Shape> &shapes)
{
    if (shapes.isEmpty())
    {
        return;
    }
    tmp = shapes;  // Сохраняем во временное хранилище
    emit receivedScript();  // Сигнализируем о получении новых данных
}


