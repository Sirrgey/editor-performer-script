#include "scriptinterface.h"

ScriptInterface::ScriptInterface()
{

}

// Добавляем в структуру закрашенный круг
void ScriptInterface::drawFilledCircle(int x, int y, int radius, const QString &color)
{
    Shape shape;
    shape.type = Shape::FilledCircle;
    shape.center = QPoint(x, y);
    shape.radius = radius;
    shape.color = QColor(color);
    shapes.append(shape);
}

// Добавляем в структуру окружность
void ScriptInterface::drawCircle(int x, int y, int radius, const QString &color)
{
    Shape shape;
    shape.type = Shape::Circle;
    shape.center = QPoint(x, y);
    shape.radius = radius;
    shape.color = QColor(color);
    if (!shape.color.isValid()) {
        shape.color = Qt::black; // fallback
    }
    shapes.append(shape);

}

// Добавляем в структуру треугольник
void ScriptInterface::drawTriangle(int x, int y, int x2, int y2, int x3, int y3)
{
    Shape shape;
    shape.type = Shape::Triangle;
    shape.points << QPoint(x,y)
                 << QPoint(x2,y2)
                 << QPoint(x3,y3);
    shape.color = QColor("black");
    shapes.append(shape);
}

// Добавляем в структуру квадрат
void ScriptInterface::drawRectangle(int x, int y, int w, int h, const QString &color)
{
    Shape shape;
    shape.type = Shape::Rectangle;
    shape.points << QPoint(x, y) << QPoint(x + w, y + h);
    shape.color = QColor(color);
    shapes.append(shape);
}

// Добавляем в структуру линию
void ScriptInterface::drawLine(int x1, int y1, int x2, int y2, const QString &color, int penWidth)
{
    Shape shape;
    shape.type = Shape::Line;
    shape.points <<QPoint(x1, y1) << QPoint(x2, y2);
    shape.color = QColor(color);
    shape.penWidth = penWidth;
    shapes.append(shape);
}

// Очищаем структуру
void ScriptInterface::reset()
{
   shapes.clear();
}
