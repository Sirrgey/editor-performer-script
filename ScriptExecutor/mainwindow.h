#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "paintwidget.h"
#include "datareceiver.h"
#include "scriptinterface.h"
#include <QMainWindow>
#include <QtNetwork/QUdpSocket>
#include <QVBoxLayout>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void receivedScript();

private:
    Ui::MainWindow *ui;
    paintwidget *painter;
    paintwidget *paintWidget;

};
#endif // MAINWINDOW_H
