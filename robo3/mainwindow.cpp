


#include "mainwindow.h"
#include "ui_mainwindow.h"



#include <QDebug>
#include<QUdpSocket>
#include<math.h>
#include <QApplication>
#include <QMainWindow>
#include <QTextStream>
#include <QMessageBox>
#include <QFileDialog>
#include <QThread>




#include <QDebug>
#include<QUdpSocket>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QThread>
#include <QtWebKit/QtWebKit>
#include<QFile>
#include <QTextStream>
#include <cmath>
#include<math.h>




#include <wiringPi.h>           //WiringPi headers
#include <stdio.h>              //Needed for the printf function below
#include <stdlib.h>
#include <string>
#include <errno.h>
#include "PCA9685.h"
#include "PCA9685.cpp"
#include <unistd.h>
#include <time.h>
#include <stdint.h>
#include "libSonar.h"
#include "libSonar.cpp"





#include "PID_v1.cpp"
#include <cstdio>
#include <cmath>
#include <fstream>
#include "hmc5883l.h"
#include "DHT.h"          // biblioteka DHT
#include "DHT.cpp"

#include "mcp3008Spi.h"
#include "mcp3008Spi.cpp"
#include <iostream>
using namespace std;




// -----------------------------------------------------------------



// -------------------------------------------------------------------------


          //  digitalWrite(7,1); //zaświeć diodę LED na gpio7
            //digitalRead(7);
 //delay( 1000 ); // wait 1 second


//Pin numbers below are the WiringPi pin numbers

#define input_A_L  24
#define input_B_L  25
#define input_A_R  28
#define input_B_R  29
// -------------------------------------------------------------------------
#define Kierunek_obrotu_L_1  22
#define Kierunek_obrotu_L_2  23
#define Kierunek_obrotu_R_1  26
#define Kierunek_obrotu_R_2  27
// -------------------------------------------------------------------------
#define Kanal_PWM_Silnik_L	6
#define Kanal_PWM_Silnik_R	12
#define Kanal_PWM_Serwo	15
// -------------------------------------------------------------------------
volatile	int encoderPos_L =0;
volatile	int sprawdz_enkoder =0;
volatile   int encoderPos_R =0;
volatile   int encoderA_Last_L=0;
volatile   int encoderB_Last_L=0;
volatile   int encoderA_Last_R=0;
volatile   int encoderB_Last_R=0;

volatile	int encoderPos_L_skret =0;
volatile    int encoderPos_R_skret =0;
// -------------------------------------------------------------------------
PCA9685 pwm_silnik;
float speed_L=0;
float speed_R=0;
// -------------------------------------------------------------------------
#define True  1
#define False 0
/*
#define TRIG1 16
#define ECHO1 15
#define TRIG2 3
#define ECHO2 2
#define TRIG3 0
#define ECHO3 7
#define TRIG4 6
#define ECHO4 5
#define TRIG5 4
#define ECHO5 1
*/
#define TRIG1 3
#define ECHO1 2
#define TRIG2 0
#define ECHO2 7
#define TRIG3 16
#define ECHO3 15
#define TRIG4 6
#define ECHO4 5
#define TRIG5 4
#define ECHO5 1
    Sonar sonar;


    Sonar sonar1[6];
    double SetpointL, InputL, OutputL;
    double SetpointR, InputR, OutputR;
    double aggKp=2.344, aggKi=73.5, aggKd=0.0007;       // bardzo dobre nastawy
    double consKp=1, consKi=16.25, consKd=0.0007; // bardzo dobre nastawy
    //Specify the links and initial tuning parameters

  //  double aggKp=2.93, aggKi=91.6, aggKd=0;       // bardzo dobre nastawy
  //  double consKp=1, consKi=16.25, consKd=0.0007; // bardzo dobre nastawy


    PID myPIDL(&InputL, &OutputL, &SetpointL, consKp, consKi, consKd, DIRECT);
    //Specify the links and initial tuning parameters
    PID myPIDR(&InputR, &OutputR, &SetpointR, consKp, consKi, consKd, DIRECT);
    //turn the PID on




// -------------------------------------------------------------------------
#define DHTPIN 21          // numer pinu sygnałowego
#define DHTTYPE DHT22    // typ czujnika (DHT11). Jesli posiadamy DHT22 wybieramy DHT22
DHT dht(DHTPIN, DHTTYPE); // definicja czujnika
// -------------------------------------------------------------------------
int stopnie_skretu=0;
// -------------------------------------------------------------------------
//float MainWindow::predkosc_silnikow_PID4( double Setpoint , double Input);
// -------------------------------------------------------------------------
//double MainWindow::fuzylogic_cel();
// -------------------------------------------------------------------------

int finish=0;
float przebyta_droga=0;
double lx=0; // punkty na osi x w lokalnym układzie współrzędnych
    double ly=0; // punkty na osi x w lokalnym układzie współrzędnych
    double xg=50; // pozycja x w globalnym układzie współrzędnych robota cel
    double yg=0; // pozycja y w globalnym układzie współrzędnych robota cel
    double poz_enkoder_left=0; // pozycja enkodera z silnika lewego
    double poz_enkoder_right=0; // pozycja enkodera z silnika lewego
  double DR=20.0; // double DR=22.7;// double DR=227;   // średnica robota w mm
  double MPP=0.0224; // double MPP=0.0343; // double MPP=0.343;  // promień koła w cm  lub *0.1 cm  ponoć◙ kephera ma r=8mm a piasło 0.08
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



// -------------------------------------------------------------------------
    // ---------ZMIENNE DO SKRETU-------------------------------------------------------------
    int finishh=0;
    float przebyta_drogaa=0;
    double lxx=0; // punkty na osi x w lokalnym układzie współrzędnych
        double lyy=0; // punkty na osi x w lokalnym układzie współrzędnych
        double xgg=2000; // pozycja x w globalnym układzie współrzędnych robota cel
        double ygg=0; // pozycja y w globalnym układzie współrzędnych robota cel
        double poz_enkoder_leftt=0; // pozycja enkodera z silnika lewego
        double poz_enkoder_rightt=0; // pozycja enkodera z silnika lewego
      double DRR=20.0; // double DR=22.7;// double DR=227;   // średnica robota w mm
      double MPPP=0.0224; // double MPP=0.0343; // double MPP=0.343;  // promień koła w cm  lub *0.1 cm  ponoć◙ kephera ma r=8mm a piasło 0.08
        double kat_thetaa=0; //  orientacja robota w globalnym układzie współrzędnych
        double cll=0;// pozycja enkodera lewego
        double crr=0;// pozycja enkodera prawego
        double cl_oldd=0;// stara pozycja enkodera lewego
        double cr_oldd=0;// stara pozycja enkodera prawego
        double zz=0; // odległość od celu
        //double i=0;
        double psii=0; // kąt obrotu od celu jeśli 0 to na wprost celu
        double pii=3.14;
        double dll,drr; // różnica o jaką obróciło się około
        double anglee,radiuss,forwardd,laterall,dxx,dyy;



    // -------------------------------------------------------------------------
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
int MainWindow::inicjalizacja_wiringPi()
{
        if (wiringPiSetup() == -1)
    {
        printf("Nie mozna wystartowac wiringPi!\n");
        return 1;
    }
    else return 0;

}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
void MainWindow::ustawienie_pinow()
{

    pinMode(Kierunek_obrotu_L_1,OUTPUT);
    pinMode(Kierunek_obrotu_L_2,OUTPUT);
    pinMode(Kierunek_obrotu_R_1,OUTPUT);
    pinMode(Kierunek_obrotu_R_2,OUTPUT);
    pinMode(input_A_L,INPUT);
    pinMode(input_B_L,INPUT);
    pullUpDnControl(input_A_L,PUD_UP);
    pullUpDnControl(input_B_L,PUD_UP);
    pinMode(input_A_R,INPUT);
    pinMode(input_B_R,INPUT);
    pullUpDnControl(input_A_R,PUD_UP);
    pullUpDnControl(input_B_R,PUD_UP);
    pinMode(TRIG1, OUTPUT);
    pinMode(ECHO1, INPUT);
    pullUpDnControl(ECHO1,PUD_UP);
}
// ------------------------------------------------------------------------- gpio readall

int get_encoder_turn_L()
{


    int    result = 0;
    int new_a =digitalRead(input_A_L);
    int new_b =digitalRead(input_B_L);

    if( new_a != encoderA_Last_L or new_b != encoderB_Last_L )
    {
        if (encoderA_Last_L == 0 and new_a == 1 )
            result = (encoderB_Last_L * 2 - 1);
       // else
      //  {
            if (encoderB_Last_L == 0 and new_b == 1 )
                result = -(encoderA_Last_L * 2 - 1);
        //}
    }

    encoderA_Last_L = new_a;
    encoderB_Last_L = new_b;

   // if(result>=0)
    //sprawdz_enkoder=sprawdz_enkoder+1;
    // if(result<0)
    //sprawdz_enkoder--;
//printf("SL: %d\n",sprawdz_enkoder);
    return result;


}
// -------------------------------------------------------------------------
int get_encoder_turn_R()
{
        int    result = 0;
    int new_a =digitalRead(input_A_R);
    int new_b =digitalRead(input_B_R);

    if( new_a != encoderA_Last_R or new_b != encoderB_Last_R )
    {
        if (encoderA_Last_R == 0 and new_a == 1 )
            result = (encoderB_Last_R * 2 - 1);
      //  else
      //  {
            if (encoderB_Last_R == 0 and new_b == 1 )
                result = -(encoderA_Last_R * 2 - 1);
    //	}
    }

    encoderA_Last_R = new_a;
    encoderB_Last_R = new_b;

    return result;

}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
void MainWindow::clockwise_L()
{
    digitalWrite(Kierunek_obrotu_L_1, 1);
    digitalWrite(Kierunek_obrotu_L_2, 0);
}
// -------------------------------------------------------------------------
void MainWindow::counter_clockwise_L()
{
    digitalWrite(Kierunek_obrotu_L_1, 0);
    digitalWrite(Kierunek_obrotu_L_2, 1);
}
// -------------------------------------------------------------------------
void MainWindow::clockwise_R()
{
    digitalWrite(Kierunek_obrotu_R_1, 1);
    digitalWrite(Kierunek_obrotu_R_2, 0);
 }
// -------------------------------------------------------------------------
void MainWindow::counter_clockwise_R()
{
    digitalWrite(Kierunek_obrotu_R_1, 0);
    digitalWrite(Kierunek_obrotu_R_2, 1);
}
// -------------------------------------------------------------------------
void MainWindow::set_frequence()
{
    //pwm_silnik.init(1,0x40);
    pwm_silnik.setPWMFreq (500);
}
// -------------------------------------------------------------------------

void obsluga_przerwania_enkodera_L(void) {



   int change = get_encoder_turn_L();

    if (change != 0 )
    {
        encoderPos_L = encoderPos_L + change;
        encoderPos_L_skret = encoderPos_L_skret + change;

       // printf("L: %d\n",encoderPos_L);
    }

}

// -------------------------------------------------------------------------

// -------------------------------------------------------------------------

void obsluga_przerwania_enkodera_R(void) {

   int change = get_encoder_turn_R();

    if (change != 0 )
    {
        encoderPos_R = encoderPos_R + change;
        encoderPos_R_skret = encoderPos_R_skret + change;
      //  printf("P: %d\n",encoderPos_R);
       // qDebug()<<encoderPos_R;
    }


}

// -------------------------------------------------------------------------

void MainWindow::inicjalizacja_przerwania_enkoderow()
{
// set Pin 17/0 generate an interrupt on high-to-low transitions
  // and attach myInterrupt() to the interrupt
  // INT_EDGE_RISING  , INT_EDGE_FALLING  , INT_EDGE_BOTH
    if ( wiringPiISR (input_A_L, INT_EDGE_BOTH, &obsluga_przerwania_enkodera_L) < 0 )
    {
      printf ( "Unable to setup ISR: \n");
    }
    if ( wiringPiISR (input_B_L, INT_EDGE_BOTH, &obsluga_przerwania_enkodera_L) < 0 )
    {
      printf ( "Unable to setup ISR: \n");
    }
    if ( wiringPiISR (input_A_R, INT_EDGE_BOTH, &obsluga_przerwania_enkodera_R) < 0 )
    {
      printf ( "Unable to setup ISR: \n");
    }
    if ( wiringPiISR (input_B_R, INT_EDGE_BOTH, &obsluga_przerwania_enkodera_R) < 0 )
    {
      printf ( "Unable to setup ISR: \n");
    }
}
// -------------------------------------------------------------------------

void MainWindow::sterowanie_serwo()
{

        printf ("Testing testing\n");
    //make sure you use the right address values.
    PCA9685 pwm_s;
    pwm_s.init(1,0x40);

    printf ("Setting frequency..");
    pwm_s.setPWMFreq (60);
    int count = 0;


        pwm_s.setPWM(Kanal_PWM_Serwo,0,pozycja_serwo);
      //  usleep(1000 * 50);
      //  usleep(1000 * 250);
       // pwm_s.setPWM(Kanal_PWM_Serwo,0,300);

      //  usleep(1000 * 1000);


  //  pwm_s.setPWMFreq (500);
  //  pwm_s.setPWM(Kanal_PWM_Serwo,0,0);
}


// -------------------------------------------------------------------------
void MainWindow::silnik_L_Forward(float speed)
{
    set_frequence();
    float s=speed;
        if(speed>10)
        speed=10;
    if(speed<0)
        speed=0;
    clockwise_L();
    //usleep(100);
     speed = speed * 409;
     //speed = speed +(s*300);
  //  speed = speed -(s*20.9);

    if (speed > 4095)
        speed = 4095;
    speed_L=speed;
    pwm_silnik.setPWM(Kanal_PWM_Silnik_L,0,speed);

}


// -------------------------------------------------------------------------


// -------------------------------------------------------------------------
void MainWindow::silnik_R_Forward(float speed)
{
    set_frequence();
    if(speed>10)
        speed=10;
    if(speed<0)
        speed=0;
    clockwise_R();
    //usleep(100);
    speed = speed * 409;

    if (speed > 4095)
        speed = 4095;
    speed_R=speed;
    pwm_silnik.setPWM(Kanal_PWM_Silnik_R,0,speed);
}


// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
void MainWindow::silnik_L_Backward(float speed)
{
    set_frequence();
    float s=speed;
        if(speed>10)
        speed=10;
    if(speed<0)
        speed=0;
    counter_clockwise_L();
     speed = speed * 409;
    speed = speed -(s*20.9);

    if (speed > 4095)
        speed = 4095;
    speed_L=speed;
    pwm_silnik.setPWM(Kanal_PWM_Silnik_L,0,speed);
}


// -------------------------------------------------------------------------


// -------------------------------------------------------------------------
void MainWindow::silnik_R_Backward(float speed)
{
    set_frequence();
    if(speed>10)
        speed=10;
    if(speed<0)
        speed=0;
    counter_clockwise_R();
    //usleep(100);
    speed = speed * 409;

    if (speed > 4095)
        speed = 4095;
    speed_R=speed;
    pwm_silnik.setPWM(Kanal_PWM_Silnik_R,0,speed);
}


// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
void MainWindow::silnik_set_speed_Forward(float speed)
{
    set_frequence();
    float s=speed;
    if(speed>10)
        speed=10;
    if(speed<0)
        speed=0;
    clockwise_L();
    clockwise_R();
    speed = speed * 409;

    if (speed > 4095)
        speed = 4095;
    speed_L=speed;
    speed_R=speed;
    pwm_silnik.setPWM(Kanal_PWM_Silnik_L,0,speed);
    pwm_silnik.setPWM(Kanal_PWM_Silnik_R,0,speed*1.25);

}


// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
void MainWindow::silnik_set_speed_Backward(float speed)
{
    set_frequence();
    if(speed>10)
        speed=10;
    if(speed<0)
        speed=0;
    counter_clockwise_L();
    counter_clockwise_R();
    speed = speed * 409;

    if (speed > 4095)
        speed = 4095;

    pwm_silnik.setPWM(Kanal_PWM_Silnik_L,0,speed);
    pwm_silnik.setPWM(Kanal_PWM_Silnik_R,0,speed*1.25);
    speed_L=speed;
    speed_R=speed;
}


// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
void MainWindow::silnik_stop()
{
    set_frequence();

    pwm_silnik.setPWM(Kanal_PWM_Silnik_L,0,0);
    pwm_silnik.setPWM(Kanal_PWM_Silnik_R,0,0);
    speed_L=0;
    speed_R=0;
}


// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
void MainWindow::silnik_increas_speed(float speed)
{
    if(speed>10)
        speed=10;
    if(speed<0)
        speed=0;
    clockwise_L();
    clockwise_R();
    speed = speed * 409;
    speed= speed+speed_L;
    if (speed > 4095)
        speed = 4095;

    pwm_silnik.setPWM(Kanal_PWM_Silnik_L,0,speed);
    pwm_silnik.setPWM(Kanal_PWM_Silnik_R,0,speed);

}


// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
void MainWindow::silnik_increas_speed_L(float speed)
{
    set_frequence();
    if(speed>10)
        speed=10;
    if(speed<0)
        speed=0;
    clockwise_L();
    speed = speed * 409;
    speed= speed+speed_L;
    if (speed > 4095)
        speed = 4095;

    pwm_silnik.setPWM(Kanal_PWM_Silnik_L,0,speed);

}


// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
void MainWindow::silnik_increas_speed_R(float speed)
{
    set_frequence();
    if(speed>10)
        speed=10;
    if(speed<0)
        speed=0;
    clockwise_R();
    speed = speed * 409;
    speed= speed+speed_R;
    if (speed > 4095)
        speed = 4095;

    pwm_silnik.setPWM(Kanal_PWM_Silnik_R,0,speed);

}


// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
void MainWindow::silnik_decrease_speed(float speed)
{
    set_frequence();
    if(speed>10)
        speed=10;
    if(speed<0)
        speed=0;
    clockwise_L();
    clockwise_R();
    speed = speed * 409;
    speed= speed-speed_L;
    if (speed < 0)
        speed = 0;

    pwm_silnik.setPWM(Kanal_PWM_Silnik_L,0,speed);
    pwm_silnik.setPWM(Kanal_PWM_Silnik_R,0,speed);

}


// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
void MainWindow::silnik_decrease_speed_L(float speed)
{
    set_frequence();
    if(speed>10)
        speed=10;
    if(speed<0)
        speed=0;
    clockwise_L();
    speed = speed * 409;
    speed= speed-speed_L;
    if (speed < 0)
        speed = 0;

    pwm_silnik.setPWM(Kanal_PWM_Silnik_L,0,speed);

}


// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
void MainWindow::silnik_decrease_speed_R(float speed)
{
    set_frequence();
    if(speed>10)
        speed=10;
    if(speed<0)
        speed=0;
    clockwise_R();
    speed = speed * 409;
    speed= speed-speed_R;
    if (speed < 0)
        speed = 0;

    pwm_silnik.setPWM(Kanal_PWM_Silnik_R,0,speed);

}


// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
void MainWindow::go_to_encoder_pos(int pos)
{

    int e_L=encoderPos_L;
    int e_R=encoderPos_R;
    int e_L_new=0;
    int e_R_new=0;
    int speed = 3;
    //int s=speed;

    clockwise_L();
    clockwise_R();
    //counter_clockwise_L();
    //counter_clockwise_R();
    speed = speed * 409;
    if (speed > 4095)
        speed = 4095;

    pwm_silnik.setPWM(Kanal_PWM_Silnik_R,0,speed);
   // speed = speed -370;//(s*25.9);
    pwm_silnik.setPWM(Kanal_PWM_Silnik_L,0,speed);

    /*
        while(e_R_new>=-pos or e_L_new <=pos)
    {
    e_L_new=encoderPos_L-e_L;
    e_R_new=encoderPos_R-e_R;
    if(e_R_new<=-pos)
        pwm_silnik.setPWM(Kanal_PWM_Silnik_R,0,0);
    if(e_L_new >=pos)
        pwm_silnik.setPWM(Kanal_PWM_Silnik_L,0,0);
    }
    */

    while(e_L_new>=-pos or e_R_new<=pos)
    {
    e_L_new=encoderPos_L-e_L;
    e_R_new=encoderPos_R-e_R;
    if(e_L_new<=-pos)
        pwm_silnik.setPWM(Kanal_PWM_Silnik_L,0,0);
    if(e_R_new>=pos)
        pwm_silnik.setPWM(Kanal_PWM_Silnik_R,0,0);
    }

    pwm_silnik.setPWM(Kanal_PWM_Silnik_L,0,0);
    pwm_silnik.setPWM(Kanal_PWM_Silnik_R,0,0);
}

// -------------------------------------------------------------------------


// -------------------------------------------------------------------------
void MainWindow::go_to_encoder_droga(float pos)
{
    encoderPos_L_skret=0;
    encoderPos_R_skret=0;

    float e_L=encoderPos_L_skret/44.28;
    float e_R=encoderPos_R_skret/44.28;


    float e_L_new=0;
    float e_R_new=0;
    float speed = 2.9;
    //int s=speed;

  //  clockwise_L();
   // clockwise_R();
    //counter_clockwise_L();
    //counter_clockwise_R();
   // speed = speed * 409;
   // if (speed > 4095)
   //     speed = 4095;

   // pwm_silnik.setPWM(Kanal_PWM_Silnik_R,0,speed*1.23);
   // speed = speed -370;//(s*25.9);
   // pwm_silnik.setPWM(Kanal_PWM_Silnik_L,0,speed);

    bool ok;
    QString qqq = ui->textEdit->toPlainText();
    double ddd= qqq.toDouble(&ok) ;

    silnik_R_Forward(speed*1.04);
    silnik_L_Forward(speed);
  //  silnik_R_Forward(speed*1.056);
    /*
        while(e_R_new>=-pos or e_L_new <=pos)
    {
    e_L_new=encoderPos_L-e_L;
    e_R_new=encoderPos_R-e_R;
    if(e_R_new<=-pos)
        pwm_silnik.setPWM(Kanal_PWM_Silnik_R,0,0);
    if(e_L_new >=pos)
        pwm_silnik.setPWM(Kanal_PWM_Silnik_L,0,0);
    }
    */

    while(e_L_new<=pos || e_R_new<=pos)
    {
    e_L_new=encoderPos_L_skret/44.9;
    e_R_new=encoderPos_R_skret/44.9;
    if(e_L_new>=pos)
        pwm_silnik.setPWM(Kanal_PWM_Silnik_L,0,0);
    if(e_R_new>=pos)
        pwm_silnik.setPWM(Kanal_PWM_Silnik_R,0,0);
    }
    finish=1;
    pwm_silnik.setPWM(Kanal_PWM_Silnik_L,0,0);
    pwm_silnik.setPWM(Kanal_PWM_Silnik_R,0,0);
}

// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
void MainWindow::skret_w_lewo(float stopnie)
{

    encoderPos_L_skret=0;
    encoderPos_R_skret=0;
    float speed = 2.7;

    counter_clockwise_L();
    clockwise_R();
/*
    speed = speed * 409;
    if (speed > 4095)
        speed = 4095;

    pwm_silnik.setPWM(Kanal_PWM_Silnik_R,0,speed);
   // speed = speed -370;//(s*25.9);
    pwm_silnik.setPWM(Kanal_PWM_Silnik_L,0,speed);
*/

    silnik_L_Backward(speed);
    silnik_R_Forward(speed*1.00);
 /*     double obrot=fuzylogic_skret(stopnie);
    while(obrot<=stopnie)
    {

    obrot=fuzylogic_skret(stopnie);

    //obrot=fuzylogic_skret();
    }

    pwm_silnik.setPWM(Kanal_PWM_Silnik_L,0,0);
    pwm_silnik.setPWM(Kanal_PWM_Silnik_R,0,0);
    */

    stopnie=stopnie*7.05;



    while(encoderPos_L_skret>=-stopnie || encoderPos_R_skret<=stopnie)
    {

    if(encoderPos_L_skret<=-stopnie)
        pwm_silnik.setPWM(Kanal_PWM_Silnik_L,0,0);
    if(encoderPos_R_skret>=stopnie)
        pwm_silnik.setPWM(Kanal_PWM_Silnik_R,0,0);
    }

    pwm_silnik.setPWM(Kanal_PWM_Silnik_L,0,0);
    pwm_silnik.setPWM(Kanal_PWM_Silnik_R,0,0);
   }



// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
void MainWindow::skret_w_prawo(float stopnie)
{

    //float e_L=encoderPos_L;
    //float e_R=encoderPos_R;
    //float e_L_new=0;
    //float e_R_new=0;
    float speed = 2.7;
    //int s=speed;

    clockwise_L();
    counter_clockwise_R();
    //counter_clockwise_L();
    //counter_clockwise_R();
  /*  speed = speed * 409;
    if (speed > 4095)
        speed = 4095;

    pwm_silnik.setPWM(Kanal_PWM_Silnik_R,0,speed);
   // speed = speed -370;//(s*25.9);
    pwm_silnik.setPWM(Kanal_PWM_Silnik_L,0,speed);
*/
    silnik_L_Forward(speed);
    silnik_R_Backward(speed*1.00);

    encoderPos_L_skret=0;
    encoderPos_R_skret=0;
  //  double obrot=fuzylogic_skret(stopnie);
/*
    while(obrot>=-stopnie)
    {
    //if(obrot>=stopnie)
    //	{
    //	pwm_silnik.setPWM(Kanal_PWM_Silnik_L,0,0);
    //	pwm_silnik.setPWM(Kanal_PWM_Silnik_R,0,0);
    //	}

        obrot=fuzylogic_skret(stopnie);
    }
   */
    stopnie=stopnie*7.05; //

    while(encoderPos_L_skret<=-stopnie || encoderPos_R_skret>=stopnie)
    {
//qDebug() << "aassdd  ";
    if(encoderPos_L_skret>=-stopnie)
        pwm_silnik.setPWM(Kanal_PWM_Silnik_L,0,0);
    if(encoderPos_R_skret<=stopnie)
        pwm_silnik.setPWM(Kanal_PWM_Silnik_R,0,0);
    }

    pwm_silnik.setPWM(Kanal_PWM_Silnik_L,0,0);
    pwm_silnik.setPWM(Kanal_PWM_Silnik_R,0,0);
}

// -------------------------------------------------------------------------

void  MainWindow::skret_robota_w_obu_kierunkach(float stopnie)
{
    if(stopnie>0)
        skret_w_lewo(stopnie);
    if(stopnie<0)
        skret_w_prawo(stopnie);

}

// -------------------------------------------------------------------------

float MainWindow::regulator1(float w_zad, float  wy_o)
{
 float k=0.4;
 float k_i=40;
 float k_d=-0.001;

  //float k=1;
 //float k_i=50;
// float k_d=0;

  //float k=0.001;
 //float k_i=15;
 //float k_d=0;

// float k=0.5;
// float k_i=0.00001;
// float k_d=0.001;


 //float k=1;
 //float k_i=0.00001;
 //float k_d=0;

 // zmienne pomocnicze
  float p,i,d,r;
float u; // uchyb regulacji
 static float u_p=0;  // uchyb regulacji w poprzenim wywołaniu
 static float su=0; // suma minionych uchybów regulacji

u= w_zad - wy_o;   // aktualny uchyb regulacji

// wyznaczenie skłądnika proporcjonalnego (P)
p=k*u;

// wyznaczenie skłądnika całkującego(I)
su=su+u;
i=k_i *su;

// wyznaczenie skłądnika różniczkującego (D)
d= k_d*(u-u_p);
u_p=u; // zapamiętaj chwilową wartość błędów do przysłych obliczeń

r= p+i+d;  // sygnał wyjściowy regulatora ;

if(r<-4000)
    r=-4000;
if(r>4000)
    r=4000;

return r;


}

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

float MainWindow::regulator2(float w_zad, float  wy_o)
{
 //float k=5;
 //float k_i=0.01;
 //float k_d=0.001;


// float k=0.5;
// float k_i=0.00001;
// float k_d=0.001;


 float k=1;
 float k_i=0.00001;
 float k_d=0;

 // zmienne pomocnicze
  float p,i,d,r;
float u; // uchyb regulacji
 static float u_p=0;  // uchyb regulacji w poprzenim wywołaniu
 static float su=0; // suma minionych uchybów regulacji

u= w_zad - wy_o;   // aktualny uchyb regulacji

// wyznaczenie skłądnika proporcjonalnego (P)
p=k*u;

// wyznaczenie skłądnika całkującego(I)
su=su+u;
i=k_i *su;

// wyznaczenie skłądnika różniczkującego (D)
d= k_d*(u-u_p);
u_p=u; // zapamiętaj chwilową wartość błędów do przysłych obliczeń

r= p+i+d;  // sygnał wyjściowy regulatora ;

if(r<-9)
    r=-9;
if(r>9)
    r=9;

return r;


}

// -------------------------------------------------------------------------
void MainWindow::robot_setVel(float predkosc)
{
    //cout << "pred rob : " <<  predkosc << endl;
    if(predkosc<0)
    silnik_set_speed_Backward(-predkosc);
    if(predkosc>=0)
    silnik_set_speed_Forward(predkosc);
}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
void MainWindow::robot_setVel_Left(float predkosc)
{
    //cout << "pred rob : " <<  predkosc << endl;
    if(predkosc<0)
    {

    silnik_L_Backward(-predkosc);
    if(predkosc<=-9)
        silnik_L_Backward(-9);
    }
    if(predkosc>=0)
    {
    silnik_L_Forward(predkosc);
    if(predkosc>9)
        silnik_L_Forward(9);
    }
}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
void MainWindow::robot_setVel_Right(float predkosc)
{
    //cout << "pred rob : " <<  predkosc << endl;
    if(predkosc<0)
    {
    silnik_R_Backward(-predkosc);
    if(predkosc<-9)
        silnik_R_Backward(-9);
    }

    if(predkosc>=0)
    {
    silnik_R_Forward(predkosc);
    if(predkosc>9)
        silnik_R_Forward(9);
    }
}
// -------------------------------------------------------------------------



int MainWindow::robot()
{
    Sonar sonar2;
    sonar2.init(TRIG1, ECHO1);
// Kod sterowania robotem

float wejscie=0;
float wyjscie=0;
float cel=15;



 int wartosc_sonara;

float sonar[1];
//int liczba_sonarow=1;

while(1)
{

    wartosc_sonara=sonar2.distance(30000);



if(wartosc_sonara>240)
{
    //wartosc_sonara=wartosc_sonara-1;
    wartosc_sonara=wartosc_sonara-wejscie;

}



for(int i=0; i<1; i++)
{

sonar[i]=wartosc_sonara ;

//printf( "liczba: %d sonar[%f]: %f   \n", liczba_sonarow,i, sonar[i]);
}

if(sonar[0]>2000)
      {

             robot_setVel(5);  // ustawienie prędkości 8000 mm/s
             printf("jazda prosto\n"); // wyświetlenie komunikatu skręt
      }

if(sonar[0]<2000)
      {


   wyjscie=sonar[0];
    wejscie=regulator2(cel,wyjscie);
    robot_setVel(wejscie);

      // uśpienie na 2 sekundy
     printf( "wejscie: %f wyjscie: %f: cel: %f predkosc: %f \n\n\n", wejscie,wyjscie, cel,wejscie);

     //printf("predkosc robota   %f\n" ,  wejscie); // wyświetlenie komunikatu stop
  // uśpienie na 2 sekundy
}
}
return 0;
}
// -------------------------------------------------------------------------




// -------------------------------------------------------------------------
float MainWindow::predkosc_silnikow()
{
    float predkosc_liniowa=0;
    float promien=0.0343 ; // w metrach [m]
    float Vo1, Vo2;

    // 480 impulsów to jeden pełny obrót
    Vo1=encoderPos_L;
    //delayMicroseconds(1000); // opóżnienie 1 milisekunda
    delay(1000); // opóżnienie 1 sekunda
    //sleep(60);
    Vo2=encoderPos_L;
    //cout << " encoderPos_L  "<< Vo1 << "encoderPos_L " << Vo2 <<" \n" << endl;
    float predkosc_obrotowa=abs(Vo2-Vo1)/960;  // predkość obrotów na sekunde [obr/s]
    //float predkosc_obrotowa=(abs(Vo2-Vo1)/560)*60;  // predkość obrotów na sekunde [obr/min]
    //cout << "Predkosc obrotowa" << predkosc_obrotowa  << " obr/s " << endl;

    //predkosc_liniowa = predkosc_obrotowa /( 3.14 * 2 * 60 * promien); // predkosc dla [obr/min]
    predkosc_liniowa = (predkosc_obrotowa * (3.14 * 2 * promien)); // predkosc dla [m/s]
    return predkosc_liniowa;
}


// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
float MainWindow::predkosc_silnikow_fast()
{
    float predkosc_liniowa=0;
    //float promien=0.035 ; // w metrach [m]
    float promien=3.43 ; // w metrach [cm]
    float Vo1, Vo2;

    // 480 impulsów to jeden pełny obrót
    Vo1=encoderPos_L;
    //delayMicroseconds(10000); // opóżnienie 10 milisekund
    delay(100); // opóżnienie 0.1 sekunda
    //sleep(60);
    Vo2=encoderPos_L;
    //cout << " encoderPos_L  "<< Vo1 << "encoderPos_L " << Vo2 <<" \n" << endl;
    float predkosc_obrotowa=(abs(Vo2-Vo1)/960);  // predkość obrotów na sekunde [obr/s]
    //float predkosc_obrotowa=(abs(Vo2-Vo1)/560)*60;  // predkość obrotów na sekunde [obr/min]
    //cout << "Predkosc obrotowa" << predkosc_obrotowa  << " obr/s " << endl;

    //predkosc_liniowa = predkosc_obrotowa /( 3.14 * 2 * 60 * promien); // predkosc dla [obr/min]
    predkosc_liniowa = (predkosc_obrotowa * (3.14 * 2 * promien)/10); // predkosc dla [m/s]
    //predkosc_liniowa = (predkosc_obrotowa * (3.14 * 2 * promien)*10); // predkosc dla [cm/s]

    QString dane;
    dane=QString("10,%1").arg(predkosc_liniowa*100);
    uaktualnij_dane(dane);

    return predkosc_liniowa;
}






// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
float MainWindow::predkosc_silnikow_fast_lewego()
{
    float predkosc_liniowa=0;
    //float promien=0.035 ; // w metrach [m]
    float promien=3.43 ; // w metrach [cm]
    float Vo1, Vo2;

    // 480 impulsów to jeden pełny obrót
    Vo1=encoderPos_L;
    //delayMicroseconds(10000); // opóżnienie 10 milisekund
    delay(100); // opóżnienie 0.1 sekunda
    //sleep(60);
    Vo2=encoderPos_L;
    //cout << " encoderPos_L  "<< Vo1 << "encoderPos_L " << Vo2 <<" \n" << endl;
    float predkosc_obrotowa=(abs(Vo2-Vo1)/960);  // predkość obrotów na sekunde [obr/s]
    //float predkosc_obrotowa=(abs(Vo2-Vo1)/560)*60;  // predkość obrotów na sekunde [obr/min]
    //cout << "Predkosc obrotowa" << predkosc_obrotowa  << " obr/s " << endl;

    //predkosc_liniowa = predkosc_obrotowa /( 3.14 * 2 * 60 * promien); // predkosc dla [obr/min]
    predkosc_liniowa = (predkosc_obrotowa * (3.14 * 2 * promien)/10); // predkosc dla [m/s]
    //predkosc_liniowa = (predkosc_obrotowa * (3.14 * 2 * promien)*10); // predkosc dla [cm/s]



    return predkosc_liniowa;
}






// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
float MainWindow::predkosc_silnikow_fast_prawego()
{
    float predkosc_liniowa=0;
    //float promien=0.035 ; // w metrach [m]
    float promien=3.43 ; // w metrach [cm]
    float Vo1, Vo2;

    // 480 impulsów to jeden pełny obrót
    Vo1=encoderPos_R;
    //delayMicroseconds(10000); // opóżnienie 10 milisekund
    delay(100); // opóżnienie 0.1 sekunda
    //sleep(60);
    Vo2=encoderPos_R;
    //cout << " encoderPos_L  "<< Vo1 << "encoderPos_L " << Vo2 <<" \n" << endl;
    float predkosc_obrotowa=(abs(Vo2-Vo1)/960);  // predkość obrotów na sekunde [obr/s]
    //float predkosc_obrotowa=(abs(Vo2-Vo1)/560)*60;  // predkość obrotów na sekunde [obr/min]
    //cout << "Predkosc obrotowa" << predkosc_obrotowa  << " obr/s " << endl;

    //predkosc_liniowa = predkosc_obrotowa /( 3.14 * 2 * 60 * promien); // predkosc dla [obr/min]
    predkosc_liniowa = (predkosc_obrotowa * (3.14 * 2 * promien)/10); // predkosc dla [m/s]
    //predkosc_liniowa = (predkosc_obrotowa * (3.14 * 2 * promien)*10); // predkosc dla [cm/s]



    return predkosc_liniowa;
}






// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
float MainWindow::droga_silnikow_fast()
{
    float droga=0;
    float predkosc_liniowa=0;
    //float promien=0.035 ; // w metrach [m]
    float promien=3.43 ; // w metrach [cm]
    float Vo1, Vo2;

    // 480 impulsów to jeden pełny obrót
    Vo1=encoderPos_L;
    //delayMicroseconds(10000); // opóżnienie 10 milisekund
    delay(100); // opóżnienie 0.1 sekunda
    //sleep(60);
    Vo2=encoderPos_L;
    //cout << " encoderPos_L  "<< Vo1 << "encoderPos_L " << Vo2 <<" \n" << endl;
    float predkosc_obrotowa=(abs(Vo2-Vo1)/960);  // predkość obrotów na sekunde [obr/s]
    //float predkosc_obrotowa=(abs(Vo2-Vo1)/560)*60;  // predkość obrotów na sekunde [obr/min]
    //cout << "Predkosc obrotowa" << predkosc_obrotowa  << " obr/s " << endl;

    //predkosc_liniowa = predkosc_obrotowa /( 3.14 * 2 * 60 * promien); // predkosc dla [obr/min]
    predkosc_liniowa = (predkosc_obrotowa * (3.14 * 2 * promien)/10); // predkosc dla [m/s]
    //predkosc_liniowa = (predkosc_obrotowa * (3.14 * 2 * promien)*10); // predkosc dla [cm/s]
    droga=predkosc_liniowa*0.1;
    return droga;
}


// -------------------------------------------------------------------------
float MainWindow::predkosc_silnikow_enocder_tick()
{
    //float predkosc_liniowa=0;
    //float promien=0.035 ; // w metrach [m]
    //float promien=3.5 ; // w metrach [cm]
    float Vo1, Vo2;

    // 480 impulsów to jeden pełny obrót
    Vo1=encoderPos_L;
    //delayMicroseconds(10000); // opóżnienie 10 milisekund
    delay(10); // opóżnienie 0.001 sekunda
    //sleep(60);
    Vo2=encoderPos_L;
    //cout << " encoderPos_L  "<< Vo1 << "encoderPos_L " << Vo2 <<" \n" << endl;
    float predkosc_obrotowa=(abs(Vo2-Vo1));  // predkość obrotów na sekunde [obr/s]

        return predkosc_obrotowa;
}






// -------------------------------------------------------------------------
void MainWindow::zapis_do_pliku()
{
    fstream plik;
plik.open( "/home/pi/projects/n/plik.txt", ios::in | ios::out );
if( plik.good() == true )
{
    std::cout << "Uzyskano dostep do pliku!" << std::endl;
    //tu operacje na pliku
} else std::cout << "Dostep do pliku zostal zabroniony!" << std::endl;

    if( plik.good() == true )
    {
        printf("otwarto ");


    }

//plik << Op::str(z) << " " << Op::str(psi)  << " " <<  Op::str(outputVariable1->getOutputValue()) << " " << Op::str(outputVariable2->getOutputValue()) << endl;
float tab_speed_L[105];
float tab_predkosc_silnikow[105];

silnik_set_speed_Forward(6);

for(int i=0; i<100; i++)
{
    //if(i>20)
    //	silnik_set_speed_Forward(6);

    tab_speed_L[i]=speed_L/409;
    tab_predkosc_silnikow[i]=predkosc_silnikow() ;
}
plik <<  "0 0"  << endl;

for(int i=0; i<100; i++)
{
    plik <<  tab_speed_L[i]  << " "  << tab_predkosc_silnikow[i] << endl;
}



//plik <<  " " << endl;

    plik.close();
    printf("zamknieto ");
}
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
void MainWindow::zapis_do_pliku_encoder()
{
    fstream plik;
plik.open( "/home/pi/projects/n/plik_enc.txt", ios::in | ios::out );
if( plik.good() == true )
{
    std::cout << "Uzyskano dostep do pliku!" << std::endl;
    //tu operacje na pliku
} else std::cout << "Dostep do pliku zostal zabroniony!" << std::endl;

    if( plik.good() == true )
    {
        printf("otwarto ");


    }

//plik << Op::str(z) << " " << Op::str(psi)  << " " <<  Op::str(outputVariable1->getOutputValue()) << " " << Op::str(outputVariable2->getOutputValue()) << endl;
float tab_speed_L[550];
float tab_predkosc_silnikow[550];

silnik_set_speed_Forward(6);

for(int i=0; i<100; i++)
{
    //if(i>20)
    //	silnik_set_speed_Forward(6);

    tab_speed_L[i]=speed_L/409;
    tab_predkosc_silnikow[i]=predkosc_silnikow_enocder_tick() ;
}
plik <<  "0 0"  << endl;

for(int i=0; i<100; i++)
{
    plik <<  tab_speed_L[i]  << " "  << tab_predkosc_silnikow[i] << endl;
}



//plik <<  " " << endl;

    plik.close();
    printf("zamknieto ");
}
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
void MainWindow::predkosc_silnikow_PID(float speed)
{

    float wyjscie=predkosc_silnikow_fast();
    float wejscie=regulator1(speed,wyjscie);
    wejscie=wejscie/1000;
    robot_setVel(wejscie);
    printf( "Ustawiona predkosc: %f aktualna predkosc: %f: cel: %f \n\n\n", wejscie,wyjscie, speed);

}

// -------------------------------------------------------------------------



// -------------------------------------------------------------------------

void MainWindow::predkosc_silnikow_PID2(float speed)
{
    //Define Variables we'll be connecting to
double Setpoint, Input, Output;

//Specify the links and initial tuning parameters
double Kp=0.1, Ki=1, Kd=0.1;
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);
//initialize the variables we're linked to
  Input = predkosc_silnikow_fast();
  Setpoint = 10;

  //turn the PID on
  myPID.SetMode(AUTOMATIC);

  while(1)
{
  Input = predkosc_silnikow_fast();
  myPID.Compute();
  //analogWrite(PIN_OUTPUT, Output);
  robot_setVel(-Output);

  printf( "Ustawiona predkosc: %f aktualna predkosc: %f: cel: %f \n\n\n", Input,Output, Setpoint);

}



}




// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

void MainWindow::predkosc_silnikow_PID3(float speed)
{
//Define Variables we'll be connecting to
double Setpoint, Input, Output;

//Define the aggressive and conservative Tuning Parameters
//double aggKp=0.05, aggKi=1.42, aggKd=0;
//double consKp=0.00000000001, consKi=0.24, consKd=0;

//double aggKp=1, aggKi=12, aggKd=0;
//double consKp=0.1, consKi=5, consKd=0;

double aggKp=2.93, aggKi=91.6, aggKd=0;       // bardzo dobre nastawy
double consKp=1, consKi=16.25, consKd=0.0007; // bardzo dobre nastawy

//double aggKp=1.4, aggKi=63, aggKd=0;       // bardzo dobre nastawy
//double consKp=aggKp, consKi=aggKi, consKd=aggKd; // bardzo dobre nastawy


//Specify the links and initial tuning parameters
PID myPID(&Input, &Output, &Setpoint, consKp, consKi, consKd, DIRECT);

//initialize the variables we're linked to
  Input = predkosc_silnikow_fast();
  Setpoint = 0.2;

  //turn the PID on
  myPID.SetMode(AUTOMATIC);


  while(1)
{
  Input = predkosc_silnikow_fast();

  double gap = abs(Setpoint-Input); //distance away from setpoint
  if (gap < 0.1)
  {  //we're close to setpoint, use conservative tuning parameters
    myPID.SetTunings(consKp, consKi, consKd);
  }
  else
  {
     //we're far from setpoint, use aggressive tuning parameters
     myPID.SetTunings(aggKp, aggKi, aggKd);
  }

  myPID.Compute();
  robot_setVel(Output);

  printf( "Ustawiona predkosc pid: %f aktualna predkosc: %f: cel: %f \n\n\n", Output,Input, Setpoint);

}



}




// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

float MainWindow::predkosc_silnikow_PID4( double Setpoint=10 , double Input=10)
{
//Define Variables we'll be connecting to
double  Output;

//Define the aggressive and conservative Tuning Parameters
//double aggKp=0.05, aggKi=1.42, aggKd=0;
//double consKp=0.00000000001, consKi=0.24, consKd=0;

double aggKp=0.5, aggKi=7.54, aggKd=0.0007;       // bardzo dobre nastawy
double consKp=0.3314, consKi=4.57, consKd=0.0007; // bardzo dobre nastawy

//double aggKp=1.4, aggKi=63, aggKd=0;       // bardzo dobre nastawy
//double consKp=aggKp, consKi=aggKi, consKd=aggKd; // bardzo dobre nastawy

//double aggKp=0.8, aggKi=26.3, aggKd=0;
//double consKp=aggKp, consKi=aggKi, consKd=aggKd;

//double aggKp=3, aggKi=6, aggKd=2;
//double consKp=aggKp, consKi=aggKi, consKd=aggKd;

//Specify the links and initial tuning parameters
PID myPID(&Input, &Output, &Setpoint, consKp, consKi, consKd, DIRECT);

//initialize the variables we're linked to
//  Input = predkosc_silnikow_fast();
 // Setpoint = 0.5;

  //turn the PID on
  myPID.SetMode(AUTOMATIC);


 // Input = predkosc_silnikow_fast();

  double gap = abs(Setpoint-Input); //distance away from setpoint
  if (gap < 3)
  {  //we're close to setpoint, use conservative tuning parameters
    myPID.SetTunings(consKp, consKi, consKd);
  }
  else
  {
     //we're far from setpoint, use aggressive tuning parameters
     myPID.SetTunings(aggKp, aggKi, aggKd);
  }

  myPID.Compute();
  //robot_setVel(Output);

 // printf( "Ustawiona predkosc pid: %f aktualna predkosc: %f: cel: %f \n\n\n", Output,Input, Setpoint);

//}

float out= Output;
  return out;

}




// -------------------------------------------------------------------------
//
// -------------------------------------------------------------------------
void MainWindow::Predkosc_kola_lewego_PID(float cel_speed)
{
   // float wyjscie;
   // wyjscie=predkosc_silnikow_fast_lewego();
   // wyjscie=wyjscie;
   // qDebug() << "sonaryy  ";
   // float wejscie=predkosc_silnikow_PID6(cel_speed,wyjscie);
   // qDebug() << "sonaryy1  ";
   //      silnik_L_Forward(wejscie);
  // printf( "xxxxUstawiona predkosc pid: %f aktualna predkosc: %f: cel: %f \n\n\n", wyjscie,wejscie, cel_speed);

 //   predkosc_silnikow_PID_slow(0.3);
  //  qDebug() << "Ustawiona predkosc pid:  " <<wejscie << "  aktualna predkosc: " <<  wyjscie << "cel:   "<< cel_speed ;
   // qDebug() << "sonaryy1  ";

    //Define Variables we'll be connecting to
 //   double Setpoint, Input, Output;

    double aggKp=2.93, aggKi=91.6, aggKd=0;       // bardzo dobre nastawy
    double consKp=1, consKi=16.25, consKd=0.0007; // bardzo dobre nastawy



    //Specify the links and initial tuning parameters
    PID myPID(&Input_L, &Output_L, &Setpoint_L, consKp, consKi, consKd, DIRECT);

    //initialize the variables we're linked to
      Input_L = predkosc_silnikow_fast_lewego();
      Setpoint_L = cel_speed;
qDebug() << "sonaryy4 ";
      //turn the PID on
      myPID.SetMode(AUTOMATIC);

qDebug() << "sonaryy5  ";

for(int i=0; i<=10;i++)
{

      Input_L = predkosc_silnikow_fast_lewego();
qDebug() << "sonaryy5  ";
      double gap = abs(Setpoint_L-Input_L); //distance away from setpoint
      if (gap < 0.10)
      {  //we're close to setpoint, use conservative tuning parameters
        myPID.SetTunings(consKp, consKi, consKd);
      }
      else
      {
         //we're far from setpoint, use aggressive tuning parameters
         myPID.SetTunings(aggKp, aggKi, aggKd);
      }
qDebug() << "sonaryy7  ";
      myPID.Compute();
      robot_setVel_Left(Output_L);

     // printf( "Ustawiona predkosc pid: %f aktualna predkosc: %f: cel: %f \n\n\n", Output,Input, Setpoint);
      qDebug() << "Ustawiona predkosc pid:  " <<Output_L << "  aktualna predkosc: " <<  Input_L << "cel:   "<< Setpoint_L ;

    }



}
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
void MainWindow::Predkosc_kola_prawego_PID(float cel_speed)
{

    double aggKp=2.93, aggKi=91.6, aggKd=0;       // bardzo dobre nastawy
    double consKp=1, consKi=16.25, consKd=0.0007; // bardzo dobre nastawy

    //Specify the links and initial tuning parameters
    PID myPID(&Input_R, &Output_R, &Setpoint_R, consKp, consKi, consKd, DIRECT);

    //initialize the variables we're linked to
      Input_R = predkosc_silnikow_fast_prawego();
      Setpoint_R = cel_speed;
qDebug() << "sonaryy4 ";
      //turn the PID on
      myPID.SetMode(AUTOMATIC);

qDebug() << "sonaryy5  ";
for(int i=0; i<=10;i++)
{
      Input_R = predkosc_silnikow_fast_prawego();
qDebug() << "sonaryy5  ";
      double gap = abs(Setpoint_R-Input_R); //distance away from setpoint
      if (gap < 0.10)
      {  //we're close to setpoint, use conservative tuning parameters
        myPID.SetTunings(consKp, consKi, consKd);
      }
      else
      {
         //we're far from setpoint, use aggressive tuning parameters
         myPID.SetTunings(aggKp, aggKi, aggKd);
      }
qDebug() << "sonaryy7  ";
      myPID.Compute();
      robot_setVel_Right(Output_R);

     // printf( "Ustawiona predkosc pid: %f aktualna predkosc: %f: cel: %f \n\n\n", Output,Input, Setpoint);
      qDebug() << "Ustawiona predkosc pidr:  " <<Output_R << "  aktualna predkoscr: " <<  Input_R << "celr:   "<< Setpoint_R ;

    }


}



// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

float MainWindow::predkosc_silnikow_PID_obu(float speed)
{



//initialize the variables we're linked to
  InputL = predkosc_silnikow_fast_lewego();
  SetpointL =speed;
  InputR = predkosc_silnikow_fast_prawego();
  SetpointR = speed;

  if(speed < 0)
  {
    InputL = - InputL;
    InputR = - InputR;
  }

  while(1)
{
      InputL = predkosc_silnikow_fast_lewego();
      InputR = predkosc_silnikow_fast_prawego();

      if(speed < 0)
      {
        InputL = - InputL;
        InputR = - InputR;
      }
  double gapL = abs(SetpointL-InputL); //distance away from setpoint
  double gapR = abs(SetpointR-InputR); //distance away from setpoint
  if (gapL < 0.10 || gapR < 0.10)
  {  //we're close to setpoint, use conservative tuning parameters
    myPIDL.SetTunings(consKp, consKi, consKd);
    myPIDR.SetTunings(consKp, consKi, consKd);
  }
  else
  {
     //we're far from setpoint, use aggressive tuning parameters
     myPIDL.SetTunings(aggKp, aggKi, aggKd);
     myPIDR.SetTunings(aggKp, aggKi, aggKd);
  }


  myPIDL.Compute();
  robot_setVel_Left(OutputL);
  qDebug() << "L Ustawiona predkosc pid:  " <<OutputL << "  aktualna predkosc: " <<  InputL << "cel:   "<< SetpointL ;


  myPIDR.Compute();
  robot_setVel_Right(OutputR);
 // printf( "Ustawiona predkosc pid: %f aktualna predkosc: %f: cel: %f \n\n\n", OutputL,InputL, SetpointL);
  qDebug() << "R Ustawiona predkosc pid:  " <<OutputR << "  aktualna predkosc: " <<  InputR << "cel:   "<< SetpointR ;

  readPendingDatagrams();
  if(flaga_stop)
      return 0;
  }



}
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

void MainWindow::predkosc_silnikow_PID_slow(float speed)
{
//Define Variables we'll be connecting to
double Setpoint, Input, Output;

//Define the aggressive and conservative Tuning Parameters
//double aggKp=0.05, aggKi=1.42, aggKd=0;
//double consKp=0.00000000001, consKi=0.24, consKd=0;

//double aggKp=1.67, aggKi=82, aggKd=0.003;       // bardzo dobre nastawy
//double consKp=0.45, consKi=6.75, consKd=0.0002; // bardzo dobre nastawy

double aggKp=2.93, aggKi=91.6, aggKd=0;       // bardzo dobre nastawy
double consKp=1, consKi=16.25, consKd=0.0007; // bardzo dobre nastawy




//double aggKp=1.4, aggKi=63, aggKd=0;       // bardzo dobre nastawy
//double consKp=aggKp, consKi=aggKi, consKd=aggKd; // bardzo dobre nastawy

//double aggKp=0.8, aggKi=26.3, aggKd=0;
//double consKp=aggKp, consKi=aggKi, consKd=aggKd;

//double aggKp=3, aggKi=6, aggKd=2;
//double consKp=aggKp, consKi=aggKi, consKd=aggKd;

//Specify the links and initial tuning parameters
PID myPID(&Input, &Output, &Setpoint, consKp, consKi, consKd, DIRECT);

//initialize the variables we're linked to
  Input = predkosc_silnikow_fast();
  Setpoint = 0.3;

  //turn the PID on
  myPID.SetMode(AUTOMATIC);


  while(1)
{
  Input = predkosc_silnikow_fast();

  double gap = abs(Setpoint-Input); //distance away from setpoint
  if (gap < 0.15)
  {  //we're close to setpoint, use conservative tuning parameters
    myPID.SetTunings(consKp, consKi, consKd);
  }
  else
  {
     //we're far from setpoint, use aggressive tuning parameters
     myPID.SetTunings(aggKp, aggKi, aggKd);
  }

  myPID.Compute();
  robot_setVel(Output);

  printf( "Ustawiona predkosc pid: %f aktualna predkosc: %f: cel: %f \n\n\n", Output,Input, Setpoint);
  qDebug() << "Ustawiona predkosc pid:  " <<Output << "  aktualna predkosc: " <<  Input << "cel:   "<< Setpoint ;

}



}
// -------------------------------------------------------------------------



// -------------------------------------------------------------------------

float MainWindow::predkosc_silnikow_PID6(double Setpoint, double Input)
{
//Define Variables we'll be connecting to
double  Output;

//Define the aggressive and conservative Tuning Parameters
//double aggKp=0.05, aggKi=1.42, aggKd=0;
//double consKp=0.00000000001, consKi=0.24, consKd=0;

//double aggKp=1.67, aggKi=82, aggKd=0.003;       // bardzo dobre nastawy
//double consKp=0.45, consKi=6.75, consKd=0.0002; // bardzo dobre nastawy

double aggKp=2.93, aggKi=91.6, aggKd=0;       // bardzo dobre nastawy
double consKp=1, consKi=16.25, consKd=0.0007; // bardzo dobre nastawy




//double aggKp=1.4, aggKi=63, aggKd=0;       // bardzo dobre nastawy
//double consKp=aggKp, consKi=aggKi, consKd=aggKd; // bardzo dobre nastawy

//double aggKp=0.8, aggKi=26.3, aggKd=0;
//double consKp=aggKp, consKi=aggKi, consKd=aggKd;

//double aggKp=3, aggKi=6, aggKd=2;
//double consKp=aggKp, consKi=aggKi, consKd=aggKd;

//Specify the links and initial tuning parameters
PID myPID(&Input, &Output, &Setpoint, consKp, consKi, consKd, DIRECT);



  //turn the PID on
  myPID.SetMode(AUTOMATIC);


  double gap = abs(Setpoint-Input); //distance away from setpoint
  if (gap < 0.05)
  {  //we're close to setpoint, use conservative tuning parameters
    myPID.SetTunings(consKp, consKi, consKd);
  }
  else
  {
     //we're far from setpoint, use aggressive tuning parameters
     myPID.SetTunings(aggKp, aggKi, aggKd);
  }

  myPID.Compute();

  //printf( "Ustawiona predkosc pid: %f aktualna predkosc: %f: cel: %f \n\n\n", Output,Input, Setpoint);

}




// -------------------------------------------------------------------------





void MainWindow::temperatura_wilgotnosc()
{
 // Odczyt temperatury i wilgotności powietrza
  float t = dht.readTemperature();
  float h = dht.readHumidity();


  // Sprawdzamy poprawność danych
   // Sprawdzamy czy są odczytane wartości
  if (isnan(t) || isnan(h))
  {
    // Jeśli nie, wyświetlamy informację o błędzie
    printf("Blad odczytu danych z czujnika");
  } else
  {
    // Jeśli tak, wyświetlamy wyniki pomiaru
 /*   printf("Wilgotnosc: ");
    printf("%f ", h);
    printf(" stopni \n");
    printf("Temperatura: ");
    printf("%f",t);
    printf(" *C / \n");   */
  }

 // qDebug()<<"temperatura ";
  QString dane;
  dane=QString("3,%1").arg(t);
  uaktualnij_dane(dane);
  dane=QString("4,%1").arg(h);
  uaktualnij_dane(dane);
}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
//
// -------------------------------------------------------------------------
void MainWindow::magnetometr()
{
    HMC5883L hmc5883l;

  // Initialize

  if( hmc5883l_init(&hmc5883l) != HMC5883L_OKAY ) {
      fprintf(stderr, "Error: %d\n", hmc5883l._error);
      exit(1);
  }

 hmc5883l_read(&hmc5883l);
 /* printf("X, Y, Z:\t%f | %f | %f \n",
                hmc5883l._data.x,
                hmc5883l._data.y,
                hmc5883l._data.z);
       printf("Scaled:\t%f | %f | %f \n",
                hmc5883l._data.x_scaled,
                hmc5883l._data.y_scaled,
                hmc5883l._data.z_scaled);

       printf("Orientation:\tDeg: %f | Rad: %f \n",
                hmc5883l._data.orientation_deg,
                hmc5883l._data.orientation_rad);
*/
        QString dane;
        dane=QString("5,%1").arg(hmc5883l._data.x_scaled);
        uaktualnij_dane(dane);
        dane=QString("6,%1").arg(hmc5883l._data.y_scaled);
        uaktualnij_dane(dane);
        dane=QString("7,%1").arg(hmc5883l._data.z_scaled);
        uaktualnij_dane(dane);
        dane=QString("8,%1").arg(hmc5883l._data.orientation_deg);
        uaktualnij_dane(dane);

}




// -------------------------------------------------------------------------

// -------------------------------------------------------------------------

float MainWindow::predkosc_silnikow_PID5( double Setpoint , double Input)
{
//Define Variables we'll be connecting to
double  Output;

//Define the aggressive and conservative Tuning Parameters
//double aggKp=0.05, aggKi=1.42, aggKd=0;
//double consKp=0.00000000001, consKi=0.24, consKd=0;

//double aggKp=1, aggKi=12, aggKd=0;
//double consKp=0.1, consKi=5, consKd=0;

//double aggKp=2.93, aggKi=91.6, aggKd=0;       // bardzo dobre nastawy
//double consKp=1, consKi=16.25, consKd=0.0007; // bardzo dobre nastawy

double aggKp=1, aggKi=16.25, aggKd=0;       // bardzo dobre nastawy
double consKp=aggKp, consKi=aggKi, consKd=aggKd; // bardzo dobre nastawy



//Specify the links and initial tuning parameters
PID myPID(&Input, &Output, &Setpoint, consKp, consKi, consKd, DIRECT);

//initialize the variables we're linked to
  //Input = predkosc_silnikow_fast();
  //Setpoint = 0.2;

  //turn the PID on
  myPID.SetMode(AUTOMATIC);


 // while(1)
//{
  //Input = predkosc_silnikow_fast();

  double gap = abs(Setpoint-Input); //distance away from setpoint
  if (gap < 0.1)
  {  //we're close to setpoint, use conservative tuning parameters
    myPID.SetTunings(consKp, consKi, consKd);
  }
  else
  {
     //we're far from setpoint, use aggressive tuning parameters
     myPID.SetTunings(aggKp, aggKi, aggKd);
  }

  myPID.Compute();
 // robot_setVel(Output);
  //printf( "_");
//  printf( "Ustawiona predkosc pid: %lf aktualna dystans: %lf: cel: %lf \n", Output,Input, Setpoint);



  float a=float(Output);
  return a;

}



// -------------------------------------------------------------------------
int MainWindow::robot2()
{


    Sonar sonar2;
    sonar2.init(TRIG1, ECHO1);
// Kod sterowania robotem

float wejscie=0;
float wyjscie=0;
float cel=14;



 int wartosc_sonara;

float sonar[1];
//int liczba_sonarow=1;

while(1)
{

    wartosc_sonara=sonar2.distance(30000);



//if(wartosc_sonara>10)
//{
    //wartosc_sonara=wartosc_sonara-1;
    //wartosc_sonara=cel-wartosc_sonara;

//}



for(int i=0; i<1; i++)
{

sonar[i]=wartosc_sonara ;

//printf( "liczba: %d sonar[%f]: %f   \n", liczba_sonarow,i, sonar[i]);
}

if(sonar[0]>2000)
      {

             robot_setVel(5);  // ustawienie prędkości 8000 mm/s
             printf("jazda prosto\n"); // wyświetlenie komunikatu skręt
      }

if(sonar[0]<2000)
      {


   wyjscie=sonar[0];
    //wejscie=predkosc_silnikow_PID4(cel,wyjscie);
   wejscie=predkosc_silnikow_PID4(cel,wyjscie);
    if(wejscie<=0.1 and wejscie>=-0.1 )
    {
        //cout << " wej :"<< wejscie << endl;
        robot_setVel(0.0);
        silnik_stop();

    }
    else
    {

        //cout << " wej2 :"<< wejscie << endl;

        robot_setVel(wejscie);

     }
      // uśpienie na 2 sekundy
    //printf( "wejscie: %f wyjscie: %f: cel: %f  \n", wejscie,wyjscie, cel);

     //printf("predkosc robota   %f\n" ,  wejscie); // wyświetlenie komunikatu stop
  // uśpienie na 2 sekundy
}

}


return 0;
}
// -------------------------------------------------------------------------


// -------------------------------------------------------------------------
void MainWindow::spi_napiecie(int channel)
{

mcp3008Spi a2d("/dev/spidev0.0", SPI_MODE_0, 1000000, 8);
    int i = 20;
        int a2dVal = 0;
    int a2dChannel = channel;
        unsigned char data[3];


 // while(i > 0)
 //   {
        data[0] = 1;  //  first byte transmitted -> start bit
        data[1] = 0b10000000 |( ((a2dChannel & 7) << 4)); // second byte transmitted -> (SGL/DIF = 1, D2=D1=D0=0)
        data[2] = 0; // third byte transmitted....don't care

        a2d.spiWriteRead(data, sizeof(data) );

        a2dVal = 0;
                a2dVal = (data[1]<< 8) & 0b1100000000; //merge data[1] & data[2] to get result
                a2dVal |=  (data[2] & 0xff);
     //   sleep(1);
     //   cout << "The Result is: " << a2dVal << endl;
        i--;
  //  }

        float wartosc= a2dVal/160.0;
       // qDebug() << "112231   " << wartosc;
        float stan_baterii = (wartosc-2.25)*133.0;
        QString dane;
       // qDebug() << "112233   " << stan_baterii;
        if(stan_baterii>100)
            stan_baterii=100.00;
        if(stan_baterii<0)
            stan_baterii=0.00;
      //  qDebug() << "112234   " << stan_baterii;
        dane=QString("2,%1").arg(stan_baterii);
      //  qDebug() << "112235   " << dane;
        uaktualnij_dane(dane);

}

// -------------------------------------------------------------------------


// -------------------------------------------------------------------------
int MainWindow::odczyt_czujnikow_CNY70(int channel)
{

mcp3008Spi a2d("/dev/spidev0.0", SPI_MODE_0, 1000000, 8);
    int i = 20;
        int a2dVal = 0;
    int a2dChannel = channel;
        unsigned char data[3];


 // while(i > 0)
 //   {
        data[0] = 1;  //  first byte transmitted -> start bit
        data[1] = 0b10000000 |( ((a2dChannel & 7) << 4)); // second byte transmitted -> (SGL/DIF = 1, D2=D1=D0=0)
        data[2] = 0; // third byte transmitted....don't care

        a2d.spiWriteRead(data, sizeof(data) );

        a2dVal = 0;
                a2dVal = (data[1]<< 8) & 0b1100000000; //merge data[1] & data[2] to get result
                a2dVal |=  (data[2] & 0xff);
     //   sleep(1);
      //  cout << "The Result is: " << a2dVal << endl;
        i--;
  //  }



return a2dVal;
}

// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
void MainWindow::linefallower()
{

send="24,";
//for(int i=2;i<=4;i++)
for(int i=3;i<=7;i++)
{
  // delay(30);
   send+=QString::number(i);
   send+="*";
   send+=QString::number(odczyt_czujnikow_CNY70(i));
   //delay(20);
   send+="*";

}

uaktualnij_dane(send);

}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------



// -------------------------------------------------------------------------
double MainWindow::fuzylogic_cel()
{

 //  if (z>=2 or z==0)   // jeśli odległość mniejsza niż 60 to zatrzymaj robota
 //   {


   // kSetEncoders(ref,0,0);
  //  while(1)
  //  {
        //poz_enkoder_left=10;  // odczytanie pozycji enkodera lewego
        //poz_enkoder_right=10;
        cl=encoderPos_L/1; // ustawienie pozycja enkodera lewego
        cr=encoderPos_R/1;
        if ((cl_old != cl) | (cr_old != cr))  // jeśli został wykoanny ruch
        {
                dl = cl - cl_old;
                dr = cr - cr_old;
                cl_old = cl;
                cr_old = cr;

                if (dl != dr) // jeśli koła obróćiły się o różną odległość
                {
                          angle = (dr - dl)/DR*MPP; // kąt obrotu robota
                          radius = (DR/2)*(dl + dr)/(dr - dl); // odległość promienia skrętu R (ICC) od śrdoka robota
                          forward = radius*sin(angle); // jazda na przód ale w tym wypadku po łuku
                          lateral = radius*(1.0 - cos(angle)); // jazda w bok
                }
                else
                {

                          angle = 0.0; // jeżeli obroty kół były takie same to kąt 0
                          forward = dl*MPP; // jazda na przód
                          lateral = 0.0; // jazda w bok
                }

                        dx = cos(kat_theta);
                        dy = sin(kat_theta);
                        lx = lx + forward*dx - lateral*dy; //  obliczanie pozycji x
                        ly = ly + forward*dy + lateral*dx; //  obliczanie pozycji y
                        kat_theta =(kat_theta+ angle);   // zmiana pozycji kąta

           }
                            if (kat_theta > 2*pi)  // jeżeli kąt większy niż 2pi to odejmnij 2pi
                            {
                             kat_theta =kat_theta - 2*pi;
                            }


                        if (kat_theta < -2*pi)
                        {
                         kat_theta = kat_theta+2*pi; // jeżeli kąt większy niż 2pi to dodaj 2pi
                        }

                    psi=kat_theta-(atan2((yg-ly),(xg-lx))); // oblicz obrót od celu
                    z=sqrt(pow((xg-lx),2)+pow((yg-ly),2)); // oblicz odległość od celu
                    printf("L: %d   R: %d    \n", -encoderPos_L, encoderPos_R);
                    printf("x: %lf   y: %lf  xg: %lf   yg: %lf  \n", lx, ly, xg,yg);
                    printf("psi: %lf   z: %lf  kat_theta: %lf\n", psi*180/3.14 , z, kat_theta*180/3.14);
                    if (z<2)   // jeśli odległość mniejsza niż 60 to zatrzymaj robota

                    {
                    //ksetspeed(ref,0,0);
                    //kSetEncoders(ref,0,0);
               //    silnik_set_speed_Forward(0);
                  //      lewy_silnik= QString("%1").arg(0);
                   //     prawy_silnik= QString("%1").arg(0);
                    printf("Udalo sie dojechac \n");
                    printf("L: %d   R: %d    \n", -encoderPos_L, encoderPos_R);
                    printf("x: %lf   y: %lf  xg: %lf   yg: %lf  \n", lx, ly, xg,yg);
                   printf("psi: %lf   z: %lf  kat_theta: %lf\n", psi*180/3.14 , z, kat_theta*180/3.14);

                        encoderPos_L=0;
                        encoderPos_R=0;
                  //  break;
                    }

     //   }

    QString dane;
    dane=QString("16,%1*%2").arg(lx).arg(ly);
    uaktualnij_dane(dane);

 return kat_theta*180/3.14;
}
// -------------------------------------------------------------------------




double MainWindow::fuzylogic_skret(int kat)
{

    encoderPos_L_skret=0;
    encoderPos_R_skret=0;
        lxx=0; // punkty na osi x w lokalnym układzie współrzędnych
        lyy=0; // punkty na osi x w lokalnym układzie współrzędnych
        xgg=10000; // pozycja x w globalnym układzie współrzędnych robota cel
        ygg=0; // pozycja y w globalnym układzie współrzędnych robota cel
        poz_enkoder_leftt=0; // pozycja enkodera z silnika lewego
        poz_enkoder_rightt=0; // pozycja enkodera z silnika lewego
      DRR=20.0; // double DR=22.7;// double DR=227;   // średnica robota w mm //  im wiecej tym większy skręt
      MPPP=0.0224; // double MPP=0.0343; // double MPP=0.343;  // promień koła w cm  lub *0.1 cm  ponoć◙ kephera ma r=8mm a piasło 0.08
        kat_thetaa=0; //  orientacja robota w globalnym układzie współrzędnych
        cll=0;// pozycja enkodera lewego
        crr=0;// pozycja enkodera prawego
        cl_oldd=0;// stara pozycja enkodera lewego
        cr_oldd=0;// stara pozycja enkodera prawego
        zz=0; // odległość od celu
       //double i=0;
        psii=0; // kąt obrotu od celu jeśli 0 to na wprost celu
        pii=3.14;
        dll=0;
        drr=0; // różnica o jaką obróciło się około
        anglee=0;
        radiuss=0;
        forwardd=0;
        laterall=0;
        dxx=0;
        dyy=0;
if(kat>0)
{
          while((kat_thetaa*180/3.14)<kat)
          {

        cll=encoderPos_L_skret/1; // ustawienie pozycja enkodera lewego
        crr=encoderPos_R_skret/1;
        if ((cl_oldd != cll) | (cr_oldd != crr))  // jeśli został wykoanny ruch
        {
                dll = cll - cl_oldd;
                drr = crr - cr_oldd;
                cl_oldd = cll;
                cr_oldd = crr;

                if (dll != drr) // jeśli koła obróćiły się o różną odległość
                {
                          anglee = (drr - dll)/DRR*MPPP; // kąt obrotu robota
                          radiuss = (DRR/2)*(dll + drr)/(drr - dll); // odległość promienia skrętu R (ICC) od śrdoka robota
                          forwardd = radiuss*sin(anglee); // jazda na przód ale w tym wypadku po łuku
                          laterall = radiuss*(1.0 - cos(anglee)); // jazda w bok
                }
                else
                {

                          anglee = 0.0; // jeżeli obroty kół były takie same to kąt 0
                          forwardd = dll*MPPP; // jazda na przód
                          laterall = 0.0; // jazda w bok
                }

                        dxx = cos(kat_thetaa);
                        dyy = sin(kat_thetaa);
                        lxx = lxx + forwardd*dxx - laterall*dyy; //  obliczanie pozycji x
                        lyy = lyy + forwardd*dyy + laterall*dxx; //  obliczanie pozycji y
                        kat_thetaa =(kat_thetaa+ anglee);   // zmiana pozycji kąta

           }
                            if (kat_thetaa > 2*pii)  // jeżeli kąt większy niż 2pi to odejmnij 2pi
                            {
                             kat_thetaa =kat_thetaa - 2*pii;
                            }


                        if (kat_thetaa < -2*pii)
                        {
                         kat_thetaa = kat_thetaa+2*pii; // jeżeli kąt większy niż 2pi to dodaj 2pi
                        }

                    psii=kat_thetaa-(atan2((ygg-lyy),(xgg-lxx))); // oblicz obrót od celu
                    zz=sqrt(pow((xgg-lxx),2)+pow((ygg-lyy),2)); // oblicz odległość od celu
                    printf("LL: %d   RR: %d    \n", encoderPos_L_skret, encoderPos_R_skret);
                    printf("x: %lf   y: %lf  xg: %lf   yg: %lf  \n", lxx, lyy, xgg,ygg);
                    printf("psi : %lf   z : %lf  kat_theta : %lf\n", psii*180/3.14 , zz, kat_thetaa*180/3.14);

                  //  if( kat_thetaa*180/3.14>=90)
                  //      return kat_thetaa*180/3.14;
    }

}



if(kat<0)
{
while((kat_thetaa*180/3.14)>=(kat+20))
{

cll=encoderPos_L_skret/1; // ustawienie pozycja enkodera lewego
crr=encoderPos_R_skret/1;
if ((cl_oldd != cll) | (cr_oldd != crr))  // jeśli został wykoanny ruch
{
      dll = cll - cl_oldd;
      drr = crr - cr_oldd;
      cl_oldd = cll;
      cr_oldd = crr;

      if (dll != drr) // jeśli koła obróćiły się o różną odległość
      {
                anglee = (drr - dll)/DRR*MPPP; // kąt obrotu robota
                radiuss = (DRR/2)*(dll + drr)/(drr - dll); // odległość promienia skrętu R (ICC) od śrdoka robota
                forwardd = radiuss*sin(anglee); // jazda na przód ale w tym wypadku po łuku
                laterall = radiuss*(1.0 - cos(anglee)); // jazda w bok
      }
      else
      {

                anglee = 0.0; // jeżeli obroty kół były takie same to kąt 0
                forwardd = dll*MPPP; // jazda na przód
                laterall = 0.0; // jazda w bok
      }

              dxx = cos(kat_thetaa);
              dyy = sin(kat_thetaa);
              lxx = lxx + forwardd*dxx - laterall*dyy; //  obliczanie pozycji x
              lyy = lyy + forwardd*dyy + laterall*dxx; //  obliczanie pozycji y
              kat_thetaa =(kat_thetaa+ anglee);   // zmiana pozycji kąta

 }
                  if (kat_thetaa > 2*pii)  // jeżeli kąt większy niż 2pi to odejmnij 2pi
                  {
                   kat_thetaa =kat_thetaa - 2*pii;
                  }


              if (kat_thetaa < -2*pii)
              {
               kat_thetaa = kat_thetaa+2*pii; // jeżeli kąt większy niż 2pi to dodaj 2pi
              }

          psii=kat_thetaa-(atan2((ygg-lyy),(xgg-lxx))); // oblicz obrót od celu
          zz=sqrt(pow((xgg-lxx),2)+pow((ygg-lyy),2)); // oblicz odległość od celu
          printf("LL: %d   RR: %d    \n", encoderPos_L_skret, encoderPos_R_skret);
          printf("x: %lf   y: %lf  xg: %lf   yg: %lf  \n", lxx, lyy, xgg,ygg);
          printf("psi : %lf   z : %lf  kat_theta : %lf\n", psii*180/3.14 , zz, kat_thetaa*180/3.14);

        //  if( kat_thetaa*180/3.14>=90)
        //      return kat_thetaa*180/3.14;
    }

}



 return kat_thetaa*180/3.14;
}




// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
float MainWindow::sonary_odleglosc(int numer)
{

/*
QString s1[6];
s1[1]="17,";
s1[1]+=QString::number(1);
s1[1]+="*";
s1[1]+=QString::number(sonar1[1].distance(30000));
s1[1]+="*";
s1[1]+=QString::number(2);
s1[1]+="*";
s1[1]+=QString::number(sonar1[2].distance(30000));
s1[1]+="*";
s1[1]+=QString::number(3);
s1[1]+="*";
s1[1]+=QString::number(sonar1[3].distance(30000));
s1[1]+="*";
s1[1]+=QString::number(4);
s1[1]+="*";
s1[1]+=QString::number(sonar1[4].distance(30000));
s1[1]+="*";
s1[1]+=QString::number(5);
s1[1]+="*";
s1[1]+=QString::number(sonar1[5].distance(30000));
*/
QString send;
send="17,";
for(int i=1;i<=5;i++)
{
   send+=QString::number(i);
   send+="*";
   send+=QString::number(sonar1[i].distance(30000));
   send+="*";
}
uaktualnij_dane(send);
//uaktualnij_dane(s1[1]);
//qDebug() << "sonaryy  "<<s1[1];
//uaktualnij_dane("17,1*10*2*15*3*20*4*25*5*30");

}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
float MainWindow::sonary_odleglosc_numer()
{

send="18,";
send2="17,";
//for(int i=2;i<=4;i++)
for(int i=1;i<=5;i++)
{
  // delay(30);
   send+=QString::number(i);
   send+="*";
   send+=QString::number(sonar1[i].distance(5000));
   //delay(20);
   send+="*";

}
send2+=send.section(',',1,1);
uaktualnij_dane(send);
uaktualnij_dane(send2);
}
// -------------------------------------------------------------------------


// -------------------------------------------------------------------------
void MainWindow::setup()
{
    setenv("WIRINGPI_GPIOMEM", "1", 1);
    inicjalizacja_wiringPi();
    ustawienie_pinow();
    inicjalizacja_przerwania_enkoderow();

    pwm_silnik.init(1,0x40);
    pwm_silnik.setPWMFreq (500);

    sonar.init(TRIG1, ECHO1);



    sonar1[1].init(TRIG1,ECHO1);
    sonar1[2].init(TRIG2,ECHO2);
    sonar1[3].init(TRIG3,ECHO3);
    sonar1[4].init(TRIG4,ECHO4);
    sonar1[5].init(TRIG5,ECHO5);

    myPIDL.Init(&InputL, &OutputL, &SetpointL, consKp, consKi, consKd, DIRECT);
    //Specify the links and initial tuning parameters
    myPIDR.Init(&InputR, &OutputR, &SetpointR, consKp, consKi, consKd, DIRECT);
    //turn the PID on

    myPIDL.SetMode(AUTOMATIC);
    //turn the PID on
    myPIDR.SetMode(AUTOMATIC);

dht.begin();            // inicjalizacja czujnika


}






















// -------------------------------------------------------------------------

void MainWindow::odbierzDane(QString wiadomosc)
{
QString wybierz,info ;
//qDebug() << "111111111111111111118 ";

wybierz = wiadomosc.section(',', 0, 0);   // str == "1"
info    = wiadomosc.section(',', 1, 1);   // str == "24stopnie"

//qDebug() << "wiadomosc  "<<wiadomosc;
qDebug() << "wybierz  "<<wybierz;
qDebug() << "info  "<<info;
int odlegosc, licznik,skret[100],dystans[100];
double v_l,v_r,z;
bool ok;



switch (wybierz.toInt()) {
case 1:
   // ui->lineEdit->setText(info);
  // silnik_set_speed_Forward(5);
  // flaga_jedz_prosto=true;
    flaga_stop=false;
   predkosc_silnikow_PID_obu(0.18);
//go_to_encoder_droga(50);
    break;
case 2:
   // ui->lineEdit->setText(info);
    //stopnie_skretu=90;
    //qDebug() << "stopnie_skretu  "<<stopnie_skretu;
    //skret_w_lewo(stopnie_skretu);
    flaga_stop=true;
    silnik_L_Backward(2.5);
    silnik_R_Forward(2.5);
   // skret_w_lewo(90);
   // fuzylogic_skret(90);
    break;
case 3:
   // ui->lineEdit->setText(info);
   // stopnie_skretu=90;
   //  qDebug() << "stopnie_skretu  "<<stopnie_skretu;
   // skret_w_prawo(stopnie_skretu);
    flaga_stop=true;
   silnik_R_Backward(2.5);
   silnik_L_Forward(2.5);
   // skret_w_prawo(-90);
   // fuzylogic_skret(-90);
    break;
case 4:
   // ui->lineEdit->setText(info);
    flaga_stop=false;
    //silnik_set_speed_Backward(5);
    predkosc_silnikow_PID_obu(-0.18);
    break;
case 5:
   // ui->lineEdit->setText(info);
    silnik_stop();
    flaga_stop=true;
    flaga_jedz_prosto=false;
    flaga_jedz_do_celu=false;

    break;
case 6:
   // ui->lineEdit->setText(info);
     odlegosc=info.toDouble(&ok);   // str == "1"
    jedz_po_prostej(odlegosc);
    break;
case 7:
   // ui->lineEdit->setText(info);
    encoderPos_L =0;
    encoderPos_R =0;
    break;
case 8:
   // ui->lineEdit->setText(info);
   // fuzylogic_cel();
    v_l=info.section('*', 0, 0).toDouble(&ok);   // str == "1"
    v_r=info.section('*', 1, 1).toDouble(&ok);   // str == "1"
    z=info.section('*', 2, 2).toDouble(&ok);   // str == "1"
   qDebug() << "vl   "<<v_l << " v_R " << v_r;




   //qDebug() << "vl   "<<v_l << " v_R " << v_r;
    if(z<8.0)
    {
        silnik_stop();
        flaga_jedz_do_celu=false;
    }
    else
    {
         flaga_jedz_do_celu=true;
        robot_setVel_Left(v_l/1.10);
        robot_setVel_Right(v_r/1.10);


    delay(100);
    enkoder=QString("14,%1*%2").arg(encoderPos_L).arg(encoderPos_R);

    uaktualnij_dane(enkoder);
    }


    break;
case 9:
    adrespc=info;
    break;
case 10:
    pozycja_serwo=info.toInt(&ok);
    sterowanie_serwo();
    delay(200);
    pwm_silnik.setPWMFreq (500);
    pwm_silnik.setPWM(Kanal_PWM_Serwo,0,0);
    break;
case 11:
   // ui->lineEdit->setText(info);

    v_l=info.section('*', 0, 0).toDouble(&ok);   // str == "1"
    v_r=info.section('*', 1, 1).toDouble(&ok);   // str == "1"
   //qDebug() << "vl   "<<v_l << " v_R " << v_r;
    if(v_l> 0.0)
        v_l=(v_l/3.0) + 1.5;   //v_l=(v_l/4.0) + 2.0;
    if(v_l< 0.0)
        v_l=(v_l/3.0) - 1.5;
    if(v_r> 0.0)
        v_r=(v_r/3.0) + 1.5;
    if(v_r< 0.0)
        v_r=(v_r/3.0) - 1.5;

   /* if(v_l>3.6)
        v_l=3.6;
    if(v_r>3.6)
        v_r=3.6;
    if(v_l<-3.6)
        v_l=-3.6;
    if(v_r<-3.6)
        v_r=-3.6;

    if (v_l >1.0 && v_l < 2.1)
            v_l=2.1;

    if (v_r >1.0 && v_r > 2.1)
            v_r=2.1;
    if (v_l <-1.0 && v_l > -2.1)
            v_l=-2.1;

    if (v_r <-1.0 && v_r > -2.1)
            v_r=-2.1;
            */

    qDebug() << "v_l  "<<v_l;
    qDebug() << "v_r  "<<v_r;
    robot_setVel_Left(v_l/1.0);
    robot_setVel_Right(v_r/1.0);
    break;
case 12:
   // ui->lineEdit->setText(info);
    flaga_stop=false;
    silnik_set_speed_Backward(4.0);
    delay(300);
    break;
case 13:
   // ui->lineEdit->setText(info);
 sonary_odleglosc_numer();
    break;

case 14:
   // ui->lineEdit->setText(info);
   // fuzylogic_cel();
    v_l=info.section('*', 0, 0).toDouble(&ok);   // str == "1"
    v_r=info.section('*', 1, 1).toDouble(&ok);   // str == "1"
    z=info.section('*', 2, 2).toDouble(&ok);   // str == "1"
   //qDebug() << "vl   "<<v_l << " v_R " << v_r;
    if(z<10.0)
        silnik_stop();
    else
    {
    robot_setVel_Left(v_l/3);
    robot_setVel_Right(1.2*v_r/3);


    delay(1000);
     enkoder=QString("19,%1*%2").arg(encoderPos_L).arg(encoderPos_R);

    uaktualnij_dane(enkoder);
    }
    break;
case 15:

   // z=info.section('*', 0, 0).toDouble(&ok);   // str == "1"
 //  go_to_encoder_droga(100);
//   delay(1000);
 //  skret_w_lewo(90);
 //  delay(1000);
 //    skret_w_prawo(-90);
  //   delay(1000);
    licznik=info.section('*',0,0).toInt(&ok);
    for(int i=1; i<=licznik; i=i+1)
    {
      skret[i]=info.section('*',i*2-1,i*2-1).toInt(&ok);
      dystans[i]=info.section('*',i*2,i*2).toInt(&ok);
    }
    for(int i=1; i<=licznik; i=i+1)
    {
    //qDebug() << "skret  " << skret[i];
    //qDebug() << "dystans " << dystans[i];
        skret_robota_w_obu_kierunkach(skret[i]);
        delay(1000);
        go_to_encoder_droga(dystans[i]);
        delay(1000);
    }
    break;

case 16:
   // ui->lineEdit->setText(info);
   // fuzylogic_cel();
    v_l=info.section('*', 0, 0).toDouble(&ok);   // str == "1"
    v_r=info.section('*', 1, 1).toDouble(&ok);   // str == "1"
    z=info.section('*', 2, 2).toDouble(&ok);   // str == "1"
   qDebug() << "vl   "<<v_l << " v_R " << v_r;




   //qDebug() << "vl   "<<v_l << " v_R " << v_r;
    if(z<8.0)
    {
        silnik_stop();
        flaga_jedz_do_celu=false;
        uaktualnij_dane("21,1");
    }
    else
    {
         flaga_jedz_do_celu=true;
        robot_setVel_Left(v_l/1.0);
        robot_setVel_Right(v_r/1.0);


  //  delay(5);
    enkoder=QString("20,%1*%2").arg(encoderPos_L).arg(encoderPos_R);

    uaktualnij_dane(enkoder);
    }
    break;
case 17:
   // ui->lineEdit->setText(info);
    flaga_stop=false;
    silnik_set_speed_Backward(4.0);
    delay(100);
    enkoder=QString("20,%1*%2").arg(encoderPos_L).arg(encoderPos_R);
    break;

case 18:
   // ui->lineEdit->setText(info);
   // fuzylogic_cel();
    v_l=info.section('*', 0, 0).toDouble(&ok);   // str == "1"
    v_r=info.section('*', 1, 1).toDouble(&ok);   // str == "1"
    z=info.section('*', 2, 2).toDouble(&ok);   // str == "1"
   qDebug() << "vl   "<<v_l << " v_R " << v_r;




   //qDebug() << "vl   "<<v_l << " v_R " << v_r;
    if(z<9.0)
    {
        silnik_stop();
        //flaga_jedz_do_celu=false;
    }
    else
    {
         flaga_jedz_do_celu=true;
        robot_setVel_Left(v_l/1.0);
        robot_setVel_Right(v_r/1.0);

    }
    delay(50);
    enkoder=QString("23,%1*%2").arg(encoderPos_L).arg(encoderPos_R);
qDebug() << "vasdasdl   "<<v_l << " v_R " << v_r;
    uaktualnij_dane(enkoder);



    break;

case 19:
   // ui->lineEdit->setText(info);

    v_l=info.section('*', 0, 0).toDouble(&ok);   // str == "1"
    v_r=info.section('*', 1, 1).toDouble(&ok);   // str == "1"
 //  qDebug() << "vl   "<<v_l << " v_R " << v_r;
/*    if(v_l> 0.0)
        v_l=(v_l/3.0) + 1.5;   //v_l=(v_l/4.0) + 2.0;
    if(v_l< 0.0)
        v_l=(v_l/3.0) - 1.5;
    if(v_r> 0.0)
        v_r=(v_r/3.0) + 1.5;
    if(v_r< 0.0)
        v_r=(v_r/3.0) - 1.5;


        */
   /* if(v_l>3.6)
        v_l=3.6;
    if(v_r>3.6)
        v_r=3.6;
    if(v_l<-3.6)
        v_l=-3.6;
    if(v_r<-3.6)
        v_r=-3.6;

    if (v_l >1.0 && v_l < 2.1)
            v_l=2.1;

    if (v_r >1.0 && v_r > 2.1)
            v_r=2.1;
    if (v_l <-1.0 && v_l > -2.1)
            v_l=-2.1;

    if (v_r <-1.0 && v_r > -2.1)
            v_r=-2.1;
            */

    qDebug() << "v_l  "<<v_l;
    qDebug() << "v_r  "<<v_r;
    robot_setVel_Left(v_l/1.0);
    robot_setVel_Right(v_r/1.0);
    break;

default:
    break;
}






}
// -------------------------------------------------------------------------



// -------------------------------------------------------------------------
void MainWindow::get_addres()
{
    nPort = 5556;
    QString port= QString::number(nPort);
    //IPaddress
   // ui->lineEditPort->setText(QString::number(nPort));
    //connect(ui->lineEditPort,SIGNAL(textChanged(QString)),this,SLOT(updateUdpPort(QString)));
}
// -------------------------------------------------------------------------
void MainWindow::initSocket()
{
    udpSocket = new QUdpSocket(this);
//    udpSocket->bind(QHostAddress::LocalHost, 33333);
    udpSocket->bind(QHostAddress::Any, nPort);

   // nPort = 5550;
    IPaddress="192.168.2.38";
//udpSocket->bind(QHostAddress::Any, nPort);
   // udpSocket->bind(QHostAddress("192.168.2.2"), 5556);
    udpSocket->bind(QHostAddress(adrespc), 5556);
    connect(udpSocket, SIGNAL(readyRead()),
            this, SLOT(readPendingDatagrams()));

}
// -------------------------------------------------------------------------
void MainWindow::readPendingDatagrams()
{
    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        udpSocket->readDatagram(datagram.data(), datagram.size(),
                                &sender, &senderPort);

        processTheDatagram(datagram,sender);
    }
}
// -------------------------------------------------------------------------
void MainWindow::processTheDatagram(QByteArray datagram, QHostAddress qha){
    bool ok;
    int n = datagram.toInt(&ok,10);
    QString qsz;
    QString wiadomosc;
    if(ok){
        emit newAngle(n);
        if ( (n < 5) || (n > 70))
        {
            qsz = trUtf8("%1 <font color = red>%2</color>").
                  arg(qha.toString()).
                  arg(datagram.data());
            wiadomosc=datagram.data();
        }
        else
        {
            qsz = trUtf8("%1 <font color = blue>%2</color>").
                  arg(qha.toString()).
                  arg(datagram.data());
            wiadomosc=datagram.data();
        }
    }else
    {
        qsz = trUtf8("%1 <font color = red>%2</color>").
              arg(qha.toString()).
              arg(datagram.data());
        wiadomosc=datagram.data();
    }
    //ui->plainTextEdit->appendHtml(qsz);
    odbierzDane(wiadomosc);

}





void MainWindow::uaktualnij_dane(const QString& dane)
{
 QByteArray Dane;
 Dane.append(dane);
//Sleeper::msleep(2);
//udpSocket->writeDatagram(Dane,QHostAddress("192.168.2.3"),5555);
 udpSocket->writeDatagram(Dane,QHostAddress(adrespc),5555);
//udpSocket->writeDatagram(Dane,QHostAddress(IPaddress),nPort+1);
qDebug() << "Message from: bufforu " << Dane;


}
// -------------------------------------------------------------------------
void MainWindow::vListIPaddress(){
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // use the first non-localhost IPv4 address
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (/*ipAddressesList.at != QHostAddress::LocalHost &&*/
            ipAddressesList.at(i).protocol() == QAbstractSocket::IPv4Protocol)

            {
                qDebug() << trUtf8("ipAddress : %1").
                        arg(ipAddressesList.at(i).toString());
                //break;
            }
    }
}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------

// -------------------------------------------------------------------------

// -------------------------------------------------------------------------

// -------------------------------------------------------------------------

// -------------------------------------------------------------------------

// -------------------------------------------------------------------------

// -------------------------------------------------------------------------














































MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setup();
    get_addres();

    initSocket();
    vListIPaddress();

    robot_setVel_Left(0);
    robot_setVel_Right(0);






    timer = new QTimer(this);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(Wypisz_PC()));
    timer->start(333); //timer->start(333); //time specified in ms


   // timer100 = new QTimer(this);
   // QObject::connect(timer, SIGNAL(timeout()), this, SLOT(Wykonaj_co_100_ms()));
   // timer100->start(500); //time specified in ms

  //  skret_w_lewo( 90);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    robot2();
    uaktualnij_dane("3,25");
    sterowanie_serwo();
}



void MainWindow::Wypisz_PC()
{
     startTime_robot=micros();

    temperatura_wilgotnosc();
    spi_napiecie(0);
    magnetometr();
   // predkosc_silnikow_fast();
  //  sonary_odleglosc(1);
 //  sonary_odleglosc_numer();
    //qDebug()<<"Q_FUNC_INFO";
   // QString dane;
   // dane=QString("3,%1").arg(t);
   // uaktualnij_dane(dane);
//sonary_odleglosc_numer();
    //initSocket();
//Predkosc_kola_lewego_PID(0.5);
//Predkosc_kola_prawego_PID(0.5);

    linefallower();

if(flaga_jedz_do_celu==true)
    uaktualnij_dane(enkoder);
//delay(1000);
endTime_robot=micros();
differ_time=endTime_robot-startTime_robot;
differ_time=differ_time/1000000.0;
uaktualnij_dane("22,"+QString::number(differ_time));
}

void MainWindow::Wykonaj_co_100_ms()
{
sonary_odleglosc_numer();
}




void MainWindow::jedz_po_prostej(int odleglosc)
{
silnik_set_speed_Forward(0);
fuzylogic_cel();
go_to_encoder_droga(odleglosc);
// go_to_encoder_pos(940);
while(finish==0 )
{
fuzylogic_cel();

delay(100); // opóżnienie 1 sekunda
}
}

void MainWindow::on_horizontalSlider_sliderMoved(int position)
{
    pozycja_serwo=position;
    sterowanie_serwo();
}
