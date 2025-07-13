#ifndef SCRIPTINTERFACE_H
#define SCRIPTINTERFACE_H

#pragma once
#include <QObject>
#include <QVariant>
#include <QDebug>
#include "Shape.h"

class ScriptInterface : public QObject
{
    Q_OBJECT
public:
    ScriptInterface();

    Q_INVOKABLE void drawFilledCircle(int x, int y, int radius, const QString &color);
    Q_INVOKABLE void drawCircle(int x, int y, int radius, const QString &color);
    Q_INVOKABLE void drawTriangle(int x, int y, int x2, int y2, int x3, int y3);
    Q_INVOKABLE void drawRectangle(int x, int y, int w, int h, const QString &color);
    Q_INVOKABLE void drawLine(int x, int y, int x2, int y2, const QString &color, int penWidth);
    QList<Shape> shapes;

    void reset();
};

#endif // SCRIPTINTERFACE_H
