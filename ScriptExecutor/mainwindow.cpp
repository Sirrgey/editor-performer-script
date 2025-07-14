#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    paintWidget = new paintwidget(this);
    QVBoxLayout *layout = new QVBoxLayout(ui->widget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(paintWidget);
    connect(paintWidget, &paintwidget::receivedScript, this, &MainWindow::receivedScript);
    ui->pushButton->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    // Устанавливаем новые фигуры для отрисовки
    paintWidget->setShapes();
    //Устанавливаем флаг изменения, разрешающий перерисовку
    paintwidget::change = true;
    // Запрашиваем обновление виджета рисования
    paintWidget->update();
    ui->label->setText("");
    // Деактивируем кнопку до получения нового скрипта
    ui->pushButton->setEnabled(false);


}

void MainWindow::receivedScript()
{
    //Показываем уведомление о новом скрипте
    ui->label->setText("Пришел новый скрипт! Загрузите его!");
    // Активируем кнопку для отрисовки
    ui->pushButton->setEnabled(true);
}

