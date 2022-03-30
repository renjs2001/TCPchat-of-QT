#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDateTime>
#include <QTcpSocket>
#include <QTcpServer>
#include <QHash>

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
    void on_connect_clicked();



    void server_New_Connect();

    void socket_Read_Data();

    void socket_Disconnected();

    void on_send_btn_clicked();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    QTcpServer* server;
    QTcpSocket* socket;
    QHash<int,QTcpSocket*> socket_list;
    int count = 0;
};
#endif // MAINWINDOW_H
