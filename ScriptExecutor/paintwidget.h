#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QJSEngine>
#include <QMessageBox>
#include "datareceiver.h"

class paintwidget : public QWidget
{
    Q_OBJECT
public:
    paintwidget(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event) override;
    static bool change;
    void setShapes();

public slots:
    void setTmp(const QList<Shape>& shapes);

signals:
    void receivedScript();

private:
    QList<Shape> tmp;
    QList<Shape> shapes;
    DataReceiver *datarec;
};

#endif // PAINTWIDGET_H
