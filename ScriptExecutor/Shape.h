#ifndef SHAPE_H
#define SHAPE_H
#include <QPainter>

struct Shape {
    enum Type { FilledCircle, Circle, Triangle, Rectangle, Line } type;

    // Для кругов:
    QPoint center;
    int radius;

    // Для треугольника, прямоугольника, линии:
    QVector<QPoint> points;

    QColor color;

    // Для линий можно добавить толщину
    int penWidth = 2;
};

#endif // SHAPE_H
