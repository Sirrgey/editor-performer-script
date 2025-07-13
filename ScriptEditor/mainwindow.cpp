#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDebug"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), udpsocket(nullptr)
{
     ui->setupUi(this);
     ui->textEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
     QRegExp regExp("(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)");
     ui->lineEdit_address->setValidator(new QRegExpValidator(regExp,this));
     QRegExp regExpPort("\\d+");
     ui->lineEdit_port->setValidator(new QRegExpValidator(regExpPort, this));
     ui->lineEdit_address->setText("127.0.0.1");
     ui->lineEdit_port->setText("55");

     udpsocket = new QUdpSocket(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_exit_triggered() // закрытие приложения
{
    close(); //событие closeEvent
}

void MainWindow::closeEvent(QCloseEvent *event) //обработка сигнала закрытия формы - нажатие кнопки крестик(выход)
{
//    //имя файла
//    QString file_name;
//    //получаем имя файла(путь к файлу)
//    file_name = MainWindow::windowTitle();
    // случай если файл существует в системе(такой путь есть)
    if (QFile::exists(path_file)){
        QFile file;
        file.setFileName(path_file);
        file.open(QIODevice::ReadOnly);
        QByteArray ba;//массив для передачи данных
        ba.clear();
        long long int size;//размер файла
        size=file.size();
        ba=file.read(size);//чтение из файла
        //проверка на то, что файл сохранить не нужно(то есть он не изменен)
        //спрашиваем подтверждение на "выход" у пользователя
        if ( QString::fromUtf8(ba) == ui->textEdit->toPlainText()) {
            if (QMessageBox::question(this,"Выходим?","Точно выходим?",QMessageBox::Yes|QMessageBox::No)==QMessageBox::Yes)
                event->accept(); // закрываем приложение
            else
                event->ignore(); // отмена выхода
        }
        //если файл был изменен, то предлагаем пользователю его сохранить перед дальнейшим действием
        else {
            QMessageBox msgBox;
            msgBox.setText("Внимание! Данные были изменены! Жалаете ли вы их сохранить перед выходом?");
            msgBox.setInformativeText("Yes - сохранить\nNo - продолжить без сохранения\nCancel - отмена");
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
            msgBox.setIcon(QMessageBox::Information);
            int res = msgBox.exec();
            switch (res) {
            //пользователь выбрал сохранить данные
            case QMessageBox::Yes:
                on_action_savefile_triggered();
                msgBox.close();
                event->accept();
                break;
            //пользователь выбрал сохранить данные
            case QMessageBox::No:
                msgBox.close();
                event->accept();
                break;
            //пользователь выбрал отменить операцию
            case QMessageBox::Cancel:
                msgBox.close();
                event->ignore();
                break;


            }

        }

    }
    // случай если файл не существует в системе(таково пути нет)
    else if (!ui->textEdit->toPlainText().isEmpty()) {
        QMessageBox msgBox;
        msgBox.setText("Внимание! Данные были изменены! Жалаете ли вы их сохранить перед выходом?");
        msgBox.setInformativeText("Yes - сохранить\nNo - продолжить без сохранения\nCancel - отмена");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        msgBox.setIcon(QMessageBox::Information);
        int res = msgBox.exec();
        switch (res)
        {
        case QMessageBox::Yes:
            if (on_action_savefile_triggered()) {
                event->accept();
                msgBox.close();
                return;
            }
            event->ignore();
            msgBox.close();
            return;
           // break;
        case QMessageBox::No:
            msgBox.close();
            event->accept();
            return;
           // break;
        case QMessageBox::Cancel:
            msgBox.close();
            event->ignore();
            return;
           // break;


        }
    }

}


void MainWindow::on_action_newfile_triggered() // создание нового файла
{
    // имя файла
//    QString file_name;
//    //получаем имя файла(путь к нему)
//    file_name = MainWindow::windowTitle();
    // случай если файл существует в системе(такой путь есть), сохраняем его без помощи окна
    if (QFile::exists(path_file)){
        QFile file;
        file.setFileName(path_file);
        file.open(QIODevice::ReadOnly);
        QByteArray ba;//массив для передачи данных
        ba.clear();
        long long int size;//размер файла
        size=file.size();
        ba=file.read(size);//чтение из файла
        if (QString::fromUtf8(ba) == ui->textEdit->toPlainText()) {
            no_change = false;
            //создаем новый пустой файл
            MainWindow::setWindowTitle("Безымянный - Cкрипт");
            path_file = "";
            ui->textEdit->clear();
            return;
            //
        }
        else {
            QMessageBox msgBox;
            msgBox.setText("Внимание! Данные были изменены! Жалаете ли вы их сохранить перед созданием нового документа?");
            msgBox.setInformativeText("Yes - сохранить\nNo - продолжить без сохранения\nCancel - отмена");
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
            msgBox.setIcon(QMessageBox::Information);
            // получаем id нажатой кнопки
            int res = msgBox.exec();
            no_change = false;
            switch (res)
            {
            case QMessageBox::Yes:
                on_action_savefile_triggered();
                ui->textEdit->clear();
                msgBox.close();
                MainWindow::setWindowTitle("Безымянный - Cкрипт");

                break;
            case QMessageBox::No:
                msgBox.close();
                MainWindow::setWindowTitle("Безымянный - Скрипт");
                ui->textEdit->clear();

                break;
            case QMessageBox::Cancel:
                msgBox.close();

                break;


            }

        }

    }
    else if (!ui->textEdit->toPlainText().isEmpty()) {
        QMessageBox msgBox;
        msgBox.setText("Внимание! Данные были изменены! Жалаете ли вы их сохранить перед созданием нового документа?");
        msgBox.setInformativeText("Yes - сохранить\nNo - продолжить без сохранения\nCancel - отмена");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        msgBox.setIcon(QMessageBox::Information);
        int res = msgBox.exec();
        switch (res)
        {
        case QMessageBox::Yes:
            if (on_action_savefile_triggered()){
                MainWindow::setWindowTitle("Безымянный - Cкрипт");
                ui->textEdit->clear();
            }
            msgBox.close();

            break;
        case QMessageBox::No:
            msgBox.close();
            MainWindow::setWindowTitle("Безымянный - Cкрипт");
            path_file = "";
            ui->textEdit->clear();

            break;
        case QMessageBox::Cancel:
            msgBox.close();

            break;


        }
    }

}

bool  MainWindow::on_action_savefile_triggered() // сохранение данных в уже существующий файл
{
   // имя файла
//   QString file_name;
//   //получаем имя файла(путь к нему)
//   file_name = MainWindow::windowTitle();
   // случай если файл существует в системе(такой путь есть), сохраняем его без помощи окна
    no_change = false;
   if (QFile::exists(path_file)) {
       QFile file;
       file.setFileName(path_file);
       file.open(QIODevice::WriteOnly);
       file.write(ui->textEdit->toPlainText().toUtf8());//запись в файл

       file.close();//закрываем
       QFileInfo fileInfo(file.fileName());
       QString file_short_name = fileInfo.fileName();
       MainWindow::setWindowTitle(file_short_name);
       return true;

       //no_change = true;
   }
   // случай если файл не существует в системе(таково пути нет), сохраняем его при помощи окна(впервые)
   else {
      return  on_action_savefileas_triggered();
   }
}

bool MainWindow::on_action_savefileas_triggered() // сохранение файла впервые с выбраным именем
{
    no_change = false;
    QString file_name = QFileDialog::getSaveFileName(this, tr("Сохранить файл"), QDir::homePath(), tr("QtScript Files (*.qs)"));
    if (!file_name.isEmpty()) {
        QFile file(file_name);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QMessageBox::warning(this, "Ошибка", "Не удалось открыть файл для записи.");
            return false;
        }

        QTextStream out(&file);
        out.setCodec("UTF-8");
        out << ui->textEdit->toPlainText();
        file.close();

        path_file = file_name;
        directory_path = QFileInfo(file_name).absolutePath(); // сохранить путь

        QString file_short_name = QFileInfo(file_name).fileName();
        setWindowTitle(file_short_name);

        return true;
    }
    return false;
}



void MainWindow::on_action_openfile_triggered() // открытие файла
{
    no_change = false;
    //имя файла
    // случай если файл существует в системе
    if (QFile::exists(path_file)) {
        QFile file;
        file.setFileName(path_file);
        file.open(QIODevice::ReadOnly);
        QByteArray ba;//массив для передачи данных
        ba.clear();
        long long int size;//размер файла
        size=file.size();
        ba=file.read(size);//чтение из файла
        file.close();
        //проверка на то, что файл сохранить не нужно(то есть он не изменен)
        qDebug() << QString::fromUtf8(ba);
        if (QString::fromUtf8(ba) == ui->textEdit->toPlainText()) {
            QString file_dir;//имя файла
            file_dir = QFileDialog::getOpenFileName(this,tr("Открыть файл"),directory_path, tr("Text files (*.qs)"));
            //проверяем выбран ли фаил
            if (!file_dir.isEmpty())
            {//фаил выбран
                ui->textEdit->clear();
                QFile file;
                file.setFileName(file_dir);
                file.open(QIODevice::ReadOnly);
                QByteArray ba;//массив для передачи данных
                ba.clear();
                long long int size;//размер файла
                size=file.size();
                ba=file.read(size);//чтение из файла
                ui->textEdit->append(QString::fromUtf8(ba));//копирование в поле редактора
                file.close();//закрываем
                path_file = file_dir;
                QFileInfo fileInfo(file.fileName());
                QString file_short_name = fileInfo.fileName();
                MainWindow::setWindowTitle(file_short_name);
            }
            return;
        }
        //если файл был изменен, то предлагаем пользователю его сохранить перед дальнейшим дейсвием
        //показываем окно выбора
        else{
            QMessageBox msgBox;
            msgBox.setText("Внимание! Данные были изменены! Жалаете ли вы их сохранить перед открытием нового документа?");
            msgBox.setInformativeText("Yes - сохранить\nNo - продолжить без сохранения\nCancel - отмена");
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
            msgBox.setIcon(QMessageBox::Information);
            int res = msgBox.exec();
            switch (res)
            {
            //пользователь выбрал сохранить данные
            case QMessageBox::Yes:
                on_action_savefile_triggered();//сохраняем текущие данные
                on_action_openfile_triggered();//открываем файл
                msgBox.close(); // скрываем окно с предупреждением(выбором)
                break;
            //пользователь выбрал не сохранять данные
            case QMessageBox::No:
                on_action_openfile_triggered();//открываем файл
                msgBox.close();// скрываем окно с предупреждением(выбором)
                return;
                break;
            //пользователь выбрал отменить  операцию
            case QMessageBox::Cancel:
                msgBox.close();// скрываем окно с предупреждением(выбором)
                break;
            }
        }
    }
    // случай, когда файла еще нет в системе, пользователю предлагают сохранить изменнные данные(окно выбора)
    else if (!ui->textEdit->toPlainText().isEmpty()) {
        QMessageBox msgBox;
        msgBox.setText("Внимание! Данные были изменены! Жалаете ли вы их сохранить перед открытием нового документа?");
        msgBox.setInformativeText("Yes - сохранить\nNo - продолжить без сохранения\nCancel - отмена");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        msgBox.setIcon(QMessageBox::Information);
        int res = msgBox.exec(); // получаем id нажатой кнопки
        QString file_dir;//имя файла
        switch (res)
        {
        //пользователь выбрал сохранить данные
        case QMessageBox::Yes:
            if (on_action_savefile_triggered()){
                //получаем имя файла( путь к файлу)
                file_dir = QFileDialog::getOpenFileName(this,tr("Открыть файл"),directory_path, tr("Text files (*.qs)"));
                //проверяем выбран ли фаил
                if (!file_dir.isEmpty())
                {//фаил выбран
                    ui->textEdit->clear();
                    QFile file;
                    file.setFileName(file_dir);
                    file.open(QIODevice::ReadOnly);
                    QByteArray ba;//массив для передачи данных
                    ba.clear();
                    long long int size;//размер файла
                    size=file.size();
                    ba=file.read(size);//чтение из файла
                    ui->textEdit->append(QString::fromUtf8(ba));//копирование в поле редактора
                    file.close();//закрываем
                    path_file = file_dir;
                    QFileInfo fileInfo(file.fileName());
                    QString file_short_name = fileInfo.fileName();
                    MainWindow::setWindowTitle(file_short_name);
                }
            }
            msgBox.close();
            break;
        //пользователь выбрал не сохранять данные

        case QMessageBox::No:
                file_dir = QFileDialog::getOpenFileName(this,tr("Открыть файл"),directory_path, tr("Text files (*.qs)"));
                //проверяем выбран ли фаил
                if (!file_dir.isEmpty())
                {//фаил выбран
                    ui->textEdit->clear();
                    QFile file;
                    file.setFileName(file_dir);
                    file.open(QIODevice::ReadOnly);
                    QByteArray ba;//массив для передачи данных
                    ba.clear();
                    long long int size;//размер файла
                    size=file.size();
                    ba=file.read(size);//чтение из файла
                    ui->textEdit->append(QString::fromUtf8(ba));//копирование в поле редактора
                    file.close();//закрываем
                    path_file = file_dir;
                    QFileInfo fileInfo(file.fileName());
                    QString file_short_name = fileInfo.fileName();
                    MainWindow::setWindowTitle(file_short_name);
                }
                msgBox.close();// скрываем окно с предупреждением(выбором)
                break;

        //пользователь выбрал отменить операцию
        case QMessageBox::Cancel:
            msgBox.close();// скрываем окно с предупреждением(выбором)
            break;


        }
    }
    else {
        //имя файла
        QString file_dir;
        //получаем имя файла( путь к файлу)
        file_dir = QFileDialog::getOpenFileName(this,tr("Открыть файл"),directory_path, tr("Text files (*.qs)"));
        //проверяем выбран ли фаил
        if (!file_dir.isEmpty())
        {//фаил выбран
            ui->textEdit->clear();
            QFile file;
            file.setFileName(file_dir);
            file.open(QIODevice::ReadOnly);
            QByteArray ba;//массив для передачи данных
            ba.clear();
            long long int size;//размер файла
            size=file.size();
            ba=file.read(size);//чтение из файла
            ui->textEdit->append(QString::fromUtf8(ba));//копирование в поле редактора
            file.close();//закрываем
            path_file = file_dir;
            QFileInfo fileInfo(file.fileName());
            QString file_short_name = fileInfo.fileName();
            MainWindow::setWindowTitle(file_short_name);
        }
    }

}




void MainWindow::on_action_zoomin_triggered() //увеличение масштаба текста
{
    // получаем текущий шрифт у textEdit
    QFont font = ui->textEdit->font();
    // получаем текущий размер шрифта у textEdit
    int size = font.pointSize();
    // устанавливаем новый размер шрифта у textEdit
    font.setPointSize(size + 15);
    // присваиваем textEdit новый измененный шрифт
    ui->textEdit->setFont(font);
}


void MainWindow::on_action_zoomout_triggered()//уменьшение масштаба текста
{
    // получаем текущий шрифт у textEdit
    QFont font = ui->textEdit->font();
    // получаем текущий размер шрифта у textEdit
    int size = font.pointSize();
    // устанавливаем новый размер шрифта у textEdit
    font.setPointSize(size - 15);
    // присваиваем textEdit новый измененный шрифт
    ui->textEdit->setFont(font);
}


void MainWindow::on_action_reset_triggered() // сброс шрифта
{
    // получаем текущий шрифт у textEdit
    QFont font = ui->textEdit->font();
    // устанавливаем новый размер шрифта (размер по умолчанию) у textEdit
    font.setPointSize(9);
    // присваиваем textEdit новый измененный шрифт
    ui->textEdit->setFont(font);
}


void MainWindow::on_action_newwindow_triggered()// создаем еще одно окно
{
    MainWindow *w2 = new MainWindow(); //создаем окно
    w2->show(); // показываем окно
}




//при изменении  textEdit
void MainWindow::on_textEdit_textChanged()
{

        if (path_file == "") {
            if (ui->textEdit->toPlainText().isEmpty()) {
                this->setWindowTitle("Безымянный - Cкрипт");
                path_file = "";
            }
            else {

                QString modtitle = this->windowTitle();
                if (!modtitle.contains("*")){
                    this->setWindowTitle("*" + modtitle);
                }
            }
        }
        else {
            QFile file;
            file.setFileName(path_file);
            file.open(QIODevice::ReadOnly);
            QByteArray ba;//массив для передачи данных
            ba.clear();
            long long int size;//размер файла
            size=file.size();
            ba=file.read(size);//чтение из файла
            file.close();
            //проверка на то, что файл сохранить не нужно(то есть он не изменен)
            if (QString::fromUtf8(ba) != ui->textEdit->toPlainText()) {

                QString modtitle = this->windowTitle();
                if (!modtitle.contains("*")){
                    this->setWindowTitle("*" + modtitle);
                }
            }
            else {
                QFileInfo fileInfo(file.fileName());
                QString file_short_name = fileInfo.fileName();
                MainWindow::setWindowTitle(file_short_name);

            }

        }

}


void MainWindow::on_pushButton_send_clicked()
{
    int port = ui->lineEdit_port->text().toInt();
    QString address = ui->lineEdit_address->text();
    QString text = ui->textEdit->toPlainText();
    if (text.isEmpty())
    {
        QMessageBox messageBox;
        messageBox.critical(0,"Ошибка","Сообщение не может быть пустым!");
        return;
    }
    if (port != 0)
    {
        QByteArray data;
        data = text.toUtf8();
        udpsocket->writeDatagram(data, QHostAddress(address), port);

    }
    else
    {
        QMessageBox messageBox;
        messageBox.critical(0,"Ошибка","Такого порта нет!");
        return;
    }
}

