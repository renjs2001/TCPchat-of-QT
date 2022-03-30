#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    ui->port_num->setText("8888");
    ui->send_btn->setEnabled(false);

    server = new QTcpServer();

    //连接信号槽
    connect(server,&QTcpServer::newConnection,this,&MainWindow::server_New_Connect);

}

MainWindow::~MainWindow()
{

    server->close();
    server->deleteLater();
    delete ui;
}


void MainWindow::on_connect_clicked()
{
    if(ui->connect->text() == tr("侦听"))
    {
        //从输入框获取端口号
        int port = ui->port_num->text().toInt();

        //监听指定的端口
        if(!server->listen(QHostAddress::Any, port))
        {
            //若出错，则输出错误信息
            qDebug()<<server->errorString();
            return;
        }
        //修改按键文字
        ui->connect->setText("取消侦听");

        QDateTime current_date_time =QDateTime::currentDateTime();
        QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz ddd");
        current_date="<font color = gray>" + current_date + "</font>";
        ui->_output->append(current_date);
        ui->_output->append("侦听成功");

        qDebug()<< "Listen succeessfully!";
    }
    else
    {


        for(int i = 1; i <= count; ++i){
        if(socket_list[i]->state() == QAbstractSocket::ConnectedState)
        {
            socket_list[i]->disconnectFromHost();
            socket_list[i]->abort();
            socket_list[i]->deleteLater();

        }
        }

        QDateTime current_date_time =QDateTime::currentDateTime();
        QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz ddd");
        current_date="<font color = gray>" + current_date + "</font>";
        ui->_output->append(current_date);
        ui->_output->append("取消侦听");

        ui->comboBox->clear();
        ui->comboBox->addItem("请选择发送对象");
        ui->client_list->clear();
        //取消侦听
        server->close();
        //修改按键文字
        ui->connect->setText("侦听");
        //发送按键失能
        ui->send_btn->setEnabled(false);

    }


}



void MainWindow::server_New_Connect()
{
    //获取客户端连接
    QTcpSocket* temp_sock;
    socket = server->nextPendingConnection();
    temp_sock = socket;
    ++count;
    socket_list.insert(count,temp_sock);
    //连接QTcpSocket的信号槽，以读取新数据
    QObject::connect(socket_list[count], &QTcpSocket::readyRead, this, &MainWindow::socket_Read_Data);
    QObject::connect(socket_list[count], &QTcpSocket::disconnected, this, &MainWindow::socket_Disconnected);
    //发送按键使能
    ui->send_btn->setEnabled(true);

    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz ddd");
    current_date="<font color = gray>" + current_date + "</font>";
    ui->_output->append(current_date);
    ui->_output->append("一个客户端连接成功");

    ui->comboBox->addItem(QString::number(count));
    ui->client_list->addItem("序号： " + QString::number(count)+
                             "  ip地址" + socket_list[count]->peerAddress().toString()
                             + "  端口号： " +QString::number(socket_list[count]->peerPort()) +
                             "  主机名： " + socket_list[count]->peerName());

    qDebug() << "A Client connect!";
}

void MainWindow::socket_Read_Data()
{
    QByteArray buffer;
    //读取缓冲区数据
    for(int i=1; i<=count;++i){
    buffer = socket_list[i]->readAll();
    if(!buffer.isEmpty())
    {
        QString str = "";
        str+=tr(buffer);
        //刷新显示
        QDateTime current_date_time =QDateTime::currentDateTime();
        QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz ddd");
        current_date="<font color = red>" + current_date + "来自客户端"+QString::number(i) + "</font>";
        ui->_output->append(current_date);
        ui->_output->append(str);
    }
    }
}

void MainWindow::socket_Disconnected()
{
    //发送按键失能
    ui->send_btn->setEnabled(false);
    qDebug() << "Disconnected!";

    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz ddd");
    current_date="<font color = gray>" + current_date + "</font>";
    ui->_output->append(current_date);
    ui->_output->append("连接断开");
    ui->comboBox->clear();
    ui->comboBox->addItem("请选择发送对象");
    ui->client_list->clear();

    for(int i=1;i<=count;++i){
        if(socket_list[i]->state()==0 ||socket_list[i]->state()==6){
        }
        else{
            ui->comboBox->addItem(QString::number(i));
            ui->client_list->addItem("序号： " + QString::number(i)+
                                     "  ip地址" + socket_list[i]->peerAddress().toString()
                                     + "  端口号： " +QString::number(socket_list[i]->peerPort()) +
                                     "  主机名： " + socket_list[i]->peerName());

        }
    }

}

void MainWindow::on_send_btn_clicked()
{


    if(ui->comboBox->currentIndex()== 0){
        QString str_error = "<font color = gray>请选择发送对象</font>";
        ui->_output->append(str_error);
    }
    else{

    qDebug() << "Send: " << ui->_input->toPlainText();

    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz ddd");
    current_date="<font color = blue>" + current_date +"发送给" +QString::number(ui->comboBox->currentText().toInt()) + "</font>";
    ui->_output->append(current_date);
    ui->_output->append(ui->_input->toPlainText());
    //获取文本框内容并以UTF-8码形式发送
    socket_list[ui->comboBox->currentText().toInt()]->write(ui->_input->toPlainText().toUtf8());
    socket_list[ui->comboBox->currentText().toInt()]->flush();
    ui->_input->clear();
    }
}

void MainWindow::on_pushButton_clicked()
{
    QString str_guide="<font color=orange>灰色为提示信息，红色为接受到的消息，"
                      "蓝色为发送的消息可选择指定对象发送消息，"
                      "对象列表右侧已经给出点击侦听，"
                      "服务端才能接受到客户端的连接请求支持多个客户端同时与服务端进行一对一通信</font>";
    ui->_output->append(str_guide);
}
