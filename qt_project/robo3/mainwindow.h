#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>



#include <QMainWindow>
#include <QWidget>
#include <QDialog>
#include <QtNetwork>
#include <QObject>
#include <QUdpSocket>
#include <QApplication>




namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void initSocket();
       QUdpSocket *udpSocket;
       QHostAddress qha;
       quint16 nPort;
       QString IPaddress;
       void processTheDatagram(QByteArray datagram, QHostAddress qha);
       void vListIPaddress();
       void uaktualnij_dane(const QString& dane);
       void get_addres();
public slots:
    void odbierzDane(QString wiadomosc);
private slots:
    void readPendingDatagrams();
    //void updateUdpPort(QString);

    void on_pushButton_clicked();

signals:
    void newAngle(int angle);
    void wyslij_do_mainwindow(const QString& buffer,const QString& sender,const QString& senderPort );
    void foo(const QString& arg);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
