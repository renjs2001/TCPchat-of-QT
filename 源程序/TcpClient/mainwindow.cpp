#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    socket = new QTcpSocket();
    this->setWindowTitle("任俊生2019214608计算机网络课程设计客户端");

        //连接信号槽
        QObject::connect(socket, &QTcpSocket::readyRead, this, &MainWindow::socket_Read_Data);
        QObject::connect(socket, &QTcpSocket::disconnected, this, &MainWindow::socket_Disconnected);

        ui->send_btn->setEnabled(false);
        ui->ip_addr->setText("127.0.0.1");
        ui->port_num->setText("8888");

}

MainWindow::~MainWindow()
{
    delete this->socket;
    delete ui;
}


void MainWindow::on_connect_btn_clicked()
{
    if(ui->connect_btn->text() == tr("连接"))
    {
        QString IP;
        int port;

        //获取IP地址
        IP = ui->ip_addr->text();
        //获取端口号
        port = ui->port_num->text().toInt();

        //取消已有的连接
        socket->abort();
        //连接服务器
        socket->connectToHost(IP, port);

        //等待连接成功
        if(!socket->waitForConnected(30000))
        {
            QDateTime current_date_time =QDateTime::currentDateTime();
            QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz ddd");
            current_date="<font color = gray>" + current_date + "</font>";
            ui->_output->append(current_date);
            ui->_output->append("连接失败");
            qDebug() << "Connection failed!";
            return;
        }


        QDateTime current_date_time =QDateTime::currentDateTime();
        QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz ddd");
        current_date="<font color = gray>" + current_date + "</font>";
        ui->_output->append(current_date);
        ui->_output->append("连接成功");
        qDebug() << "Connect successfully!";

        //发送按键使能
        ui->send_btn->setEnabled(true);
        //修改按键文字
        ui->connect_btn->setText("断开连接");
    }
    else
    {
        //断开连接
        socket->disconnectFromHost();
        //修改按键文字
        ui->connect_btn->setText("连接");
        ui->send_btn->setEnabled(false);
    }

}

void MainWindow::on_send_btn_clicked()
{
    qDebug() << "Send: " << ui->_input->toPlainText();

    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz ddd");
    current_date="<font color = blue>" + current_date + "</font>";
    ui->_output->append(current_date);
    ui->_output->append(ui->_input->toPlainText());

     //获取文本框内容并以UTF-8码形式发送
    socket->write(ui->_input->toPlainText().toUtf8());
    socket->flush();
    ui->_input->clear();
}

void MainWindow::on_guide_btn_clicked()
{
    QString str_guide="<font color=orange>灰色为提示信息"
                      "，红色为接受到的消息，蓝色为发送的消息"
                      "，当服务器端开始侦听对应端口时，才可连接成功"
                      "，默认ip地址为127.0.0.1回环地址"
                      "，可根据实际修改为对应ip地址</font>";
    ui->_output->append(str_guide);

}

void MainWindow::socket_Read_Data()
{
    QByteArray buffer;
    //读取缓冲区数据
    buffer = socket->readAll();
    if(!buffer.isEmpty())
    {
        QString str ="";
        str+=tr(buffer);
        //刷新显示
        ui->_output->append(str);
    }

}

void MainWindow::socket_Disconnected()
{
    //发送按键失能
    ui->send_btn->setEnabled(false);
    //修改按键文字
    ui->connect_btn->setText("连接");

    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz ddd");
    current_date="<font color = gray>" + current_date + "</font>";
    ui->_output->append(current_date);
    ui->_output->append("连接断开");

    qDebug() << "Disconnected!";
}
