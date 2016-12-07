#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QDialog>
#include <QtNetwork>
#include <QObject>
#include <QUdpSocket>
#include <QApplication>

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QWebView>

#include <ActiveQt/QAxWidget>
#include "windows.h"


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


    void otworz_plik();	 // otwieranie pliku
    void zapisz_plik();	 // zapisywanie pliku
    void zapisz_plik_jako();	 // zapisywanie pliku



public slots:
    void mousePressEvent(QMouseEvent *event);
    void drawLine();
    void reset();
    void drawCross(QRgb colorlline);
    void rysujPrzeszkode(int x,int y ,int szer,int wys,QRgb colorrect);
    void odbierzDane(QString wiadomosc);
    void rysujTrase(QMouseEvent *event);
    void fuzylogic();
    void fuzylogic_jedz();
    void fuzylogic_cel();
    void Readfile(QString filename);

private slots:
    void readPendingDatagrams();
    void updateUdpPort(QString);





   // void on_przycisk_clicked();

    void zapisz_plik(QString fileName);
    void on_actionOtw_rz_triggered();
    void on_actionZapisz_triggered();
    void on_actionZapisz_jako_triggered();
    void on_actionZamknij_triggered();



    void on_pushButton_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

    void on_verticalSlider_sliderMoved(int position);
    void on_pushButton_5_clicked();
    void on_pushButton_6_clicked();









    void on_checkBox_toggled(bool checked);

    void on_checkBox_3_toggled(bool checked);

    void on_pushButton_11_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_7_clicked();

signals:
    void newAngle(int angle);
    void wyslij_do_mainwindow(const QString& buffer,const QString& sender,const QString& senderPort );
    void foo(const QString& arg);

private:
    Ui::MainWindow *ui;

    QGraphicsScene *scene;

    short int count;
    short int count2;
    QPointF lastpoint,pomoc_point,linecross;
    QImage img;
    QPointF p1, p2;
    QRgb color,colorlline,colorllinewhite,colorrect;




    double x=0; // punkty na osi x w lokalnym układzie współrzędnych
    double y=0; // punkty na osi x w lokalnym układzie współrzędnych
    double xg=0; // pozycja x w globalnym układzie współrzędnych robota cel
    double yg=0; // pozycja y w globalnym układzie współrzędnych robota cel
    double poz_enkoder_left=0; // pozycja enkodera z silnika lewego
    double poz_enkoder_right=0; // pozycja enkodera z silnika lewego
    double DR=100;   // średnica robota w mm
    double MPP=0.1;  // promień koła w cm  lub *0.1 cm  ponoć◙ kephera ma r=8mm a piasło 0.08
    double kat_theta=0; //  orientacja robota w globalnym układzie współrzędnych
    double cl=0;// pozycja enkodera lewego
    double cr=0;// pozycja enkodera prawego
    double cl_old=0;// stara pozycja enkodera lewego
    double cr_old=0;// stara pozycja enkodera prawego
    double z=0; // odległość od celu
    //double i=0;
    double psi=0; // kąt obrotu od celu jeśli 0 to na wprost celu
    double pi=3.14;
    double dl,dr; // różnica o jaką obróciło się około
    double angle,radius,forward,lateral,dx,dy;






};

#endif // MAINWINDOW_H
