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
#include<qobject.h>


#include "PID_v1.h"



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
       QString adrespc;
       int pozycja_serwo;
       double Setpoint_L, Input_L, Output_L;
       double Setpoint_R, Input_R, Output_R;

       long startTime_robot;
       long endTime_robot;
       double differ_time;
       QString enkoder;
       QString send,send2;

       bool flaga_jedz_prosto=false;
       bool flaga_stop=false;
       bool flaga_jedz_do_przodu=false;
       bool flaga_jedz_do_tylu=false;
       bool flaga_jedz_do_celu=false;
       void processTheDatagram(QByteArray datagram, QHostAddress qha);
       void vListIPaddress();
       void uaktualnij_dane(const QString& dane);
       void get_addres();








      // float predkosc_silnikow_PID4( double Setpoint , double Input);
       double fuzylogic_cel();
       double fuzylogic_skret(int kat);
       int inicjalizacja_wiringPi();
       void ustawienie_pinow();
       float sonary_odleglosc(int numer);
       float sonary_odleglosc_numer();
       //int  get_encoder_turn_L();
      // int  get_encoder_turn_R();
       void  clockwise_L();
       void  counter_clockwise_L();
       void  clockwise_R();
       void  counter_clockwise_R();
       void  set_frequence();
      // void  obsluga_przerwania_enkodera_L(void);
       //void  obsluga_przerwania_enkodera_R(void);
       void  inicjalizacja_przerwania_enkoderow();
       void  sterowanie_serwo();
       void  silnik_L_Forward(float speed);
       void  silnik_R_Forward(float speed);
       void  silnik_L_Backward(float speed);
       void  silnik_R_Backward(float speed);
       void  silnik_set_speed_Forward(float speed);
       void  silnik_set_speed_Backward(float speed);
       void  silnik_stop();
       void  silnik_increas_speed(float speed);
       void  silnik_increas_speed_L(float speed);
       void  silnik_increas_speed_R(float speed);
       void  silnik_decrease_speed(float speed);
       void  silnik_decrease_speed_L(float speed);
       void  silnik_decrease_speed_R(float speed);
       void  go_to_encoder_pos(int pos);
       void  go_to_encoder_droga(float pos);
       void  skret_w_lewo(float stopnie);
       void  skret_w_prawo(float stopnie);
       void  skret_robota_w_obu_kierunkach(float stopnie);
       float  regulator1(float w_zad, float  wy_o);
       float  regulator2(float w_zad, float  wy_o);
       void  robot_setVel(float predkosc);
       int  robot();
       float  predkosc_silnikow();
       float  predkosc_silnikow_fast();
       float predkosc_silnikow_fast_prawego();
       float predkosc_silnikow_fast_lewego();
       void Predkosc_kola_lewego_PID(float cel_speed);
       void Predkosc_kola_prawego_PID(float cel_speed);
       float predkosc_silnikow_PID_obu(float speed);
       float  droga_silnikow_fast();
       float  predkosc_silnikow_enocder_tick();
       void  zapis_do_pliku();
       void  zapis_do_pliku_encoder();
       void  predkosc_silnikow_PID(float speed);
       void  predkosc_silnikow_PID2(float speed);
       void  predkosc_silnikow_PID3(float speed);
       float predkosc_silnikow_PID4( double  , double );
       void  predkosc_silnikow_PID_slow(float speed);
       void  temperatura_wilgotnosc();
       void  magnetometr();
       float  predkosc_silnikow_PID5( double Setpoint , double Input);
       float  predkosc_silnikow_PID6( double Setpoint , double Input);
       int  robot2();
       void  spi_napiecie(int channel);
       int  odczyt_czujnikow_CNY70(int channel);
        void jedz_po_prostej(int odleglosc);
        void robot_setVel_Left(float predkosc);
        void robot_setVel_Right(float predkosc);
        void  setup();

        void linefallower();

public slots:
    void odbierzDane(QString wiadomosc);
    void Wypisz_PC();
    void Wykonaj_co_100_ms();


private slots:
    void readPendingDatagrams();
    //void updateUdpPort(QString);

    void on_pushButton_clicked();

    void on_horizontalSlider_sliderMoved(int position);

signals:
    void newAngle(int angle);
    void wyslij_do_mainwindow(const QString& buffer,const QString& sender,const QString& senderPort );
    void foo(const QString& arg);

private:
    Ui::MainWindow *ui;
     QTimer *timer;
     QTimer *timer100;

};

#endif // MAINWINDOW_H
