#include <wiringPi.h>           //WiringPi headers
#include <stdio.h>              //Needed for the printf function below
#include <stdlib.h>
#include <string>
#include <stdlib.h>
#include <errno.h>
#include "PCA9685.h"
#include "PCA9685.cpp"
#include <unistd.h>
#include <time.h>
#include <stdint.h>
#include "libSonar.h"
#include "libSonar.cpp"
#include "PID_v1.h"
#include "PID_v1.cpp"
#include <cstdio>
#include <cmath>
#include <fstream>
#include "hmc5883l.h"

using namespace std;
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
#define Kanal_PWM_Silnik_R	7
#define Kanal_PWM_Serwo	0
// -------------------------------------------------------------------------
volatile	int encoderPos_L =0;
volatile	int sprawdz_enkoder =0;
volatile   int encoderPos_R =0;
volatile   int encoderA_Last_L=0;
volatile   int encoderB_Last_L=0;
volatile   int encoderA_Last_R=0;
volatile   int encoderB_Last_R=0; 
// -------------------------------------------------------------------------
PCA9685 pwm_silnik;
float speed_L=0;
float speed_R=0;
// -------------------------------------------------------------------------
#define True  1
#define False 0
#define TRIG 6
#define ECHO 5

	Sonar sonar;
    

// -------------------------------------------------------------------------
float predkosc_silnikow_PID4( double Setpoint , double Input);
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------

// -------------------------------------------------------------------------

// -------------------------------------------------------------------------

// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
int inicjalizacja_wiringPi()
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
void ustawienie_pinow()
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
	pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);
    pullUpDnControl(ECHO,PUD_UP); 
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
void clockwise_L()
{
    digitalWrite(Kierunek_obrotu_L_1, 1);    
    digitalWrite(Kierunek_obrotu_L_2, 0);
}
// ------------------------------------------------------------------------- 
void counter_clockwise_L()
{
    digitalWrite(Kierunek_obrotu_L_1, 0);
    digitalWrite(Kierunek_obrotu_L_2, 1);
}
// -------------------------------------------------------------------------
void clockwise_R()
{
    digitalWrite(Kierunek_obrotu_R_1, 1);    
    digitalWrite(Kierunek_obrotu_R_2, 0);
 }
// ------------------------------------------------------------------------- 
void counter_clockwise_R()
{
    digitalWrite(Kierunek_obrotu_R_1, 0);
    digitalWrite(Kierunek_obrotu_R_2, 1); 
}
// -------------------------------------------------------------------------
void set_frequence()
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
	    printf("L: %d\n",encoderPos_L);
	}
   
}
 
// -------------------------------------------------------------------------
 
// -------------------------------------------------------------------------
 
void obsluga_przerwania_enkodera_R(void) {
    
   int change = get_encoder_turn_R();
    
    if (change != 0 )
    {
	    encoderPos_R = encoderPos_R + change;
	    printf("P: %d\n",encoderPos_R);
	}
   

}
 
// ------------------------------------------------------------------------- 
 
void inicjalizacja_przerwania_enkoderow()
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

void sterowanie_serwo()
{
	
	    printf ("Testing testing\n");
	//make sure you use the right address values.
	PCA9685 pwm_s;
	pwm_s.init(1,0x40);
	printf ("Setting frequency..");
	pwm_s.setPWMFreq (60);
	int count = 0;
	while (count++<4) 
	{
		pwm_s.setPWM(0,0,150);	
		usleep(1000 * 1000);
		
		pwm_s.setPWM(0,0,600);
		
		usleep(1000 * 1000);
	}
	
    pwm_s.setPWMFreq (500);
    pwm_s.setPWM(0,0,0);
}


// -------------------------------------------------------------------------
void silnik_L_Forward(float speed)
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
	speed = speed -(s*20.9);
	
    if (speed > 4095)
        speed = 4095;
	speed_L=speed;
	pwm_silnik.setPWM(Kanal_PWM_Silnik_L,0,speed);
	
}


// -------------------------------------------------------------------------


// -------------------------------------------------------------------------
void silnik_R_Forward(float speed)
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
void silnik_L_Backward(float speed)
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
void silnik_R_Backward(float speed)
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
void silnik_set_speed_Forward(float speed)
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
	pwm_silnik.setPWM(Kanal_PWM_Silnik_R,0,speed);
	    speed = speed -(s*20.9); 
	pwm_silnik.setPWM(Kanal_PWM_Silnik_L,0,speed);

}


// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
void silnik_set_speed_Backward(float speed)
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
	pwm_silnik.setPWM(Kanal_PWM_Silnik_R,0,speed);
	speed_L=speed;
	speed_R=speed;
}


// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
void silnik_stop()
{
	set_frequence();

	pwm_silnik.setPWM(Kanal_PWM_Silnik_L,0,0);
	pwm_silnik.setPWM(Kanal_PWM_Silnik_R,0,0);
	speed_L=0;
	speed_R=0;
}


// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
void silnik_increas_speed(float speed)
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
void silnik_increas_speed_L(float speed)
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
void silnik_increas_speed_R(float speed)
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
void silnik_decrease_speed(float speed)
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
void silnik_decrease_speed_L(float speed)
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
void silnik_decrease_speed_R(float speed)
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
void go_to_encoder_pos(int pos)
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

float regulator1(float w_zad, float  wy_o)
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

float regulator2(float w_zad, float  wy_o)
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
void robot_setVel(float predkosc)
{
	cout << "pred rob : " <<  predkosc << endl;
	if(predkosc<0)
	silnik_set_speed_Backward(-predkosc);
	if(predkosc>0)
	silnik_set_speed_Forward(predkosc);
}
// -------------------------------------------------------------------------
int robot()
{
	Sonar sonar2;
	sonar2.init(TRIG, ECHO);
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
int robot2()
{
	Sonar sonar2;
	sonar2.init(TRIG, ECHO);
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
    wejscie=predkosc_silnikow_PID4(cel,wyjscie);
    if(wejscie<=0.05 and wejscie>=-0.05 )
    {
		cout << " wej :"<< wejscie << endl;
		robot_setVel(0);
		silnik_stop();
	}
	else
	{
		cout << " wej2 :"<< wejscie << endl;
		robot_setVel(wejscie);
     }
      // uśpienie na 2 sekundy 
	 printf( "wejscie: %f wyjscie: %f: cel: %f  \n\n\n", wejscie,wyjscie, cel);	 
	
     //printf("predkosc robota   %f\n" ,  wejscie); // wyświetlenie komunikatu stop    
  // uśpienie na 2 sekundy 
}
}
return 0;
}
// -------------------------------------------------------------------------




// -------------------------------------------------------------------------
float predkosc_silnikow()
{
	float predkosc_liniowa=0;
	float promien=0.035 ; // w metrach [m]
	float Vo1, Vo2;
	
	// 480 impulsów to jeden pełny obrót 
	Vo1=encoderPos_L;
	//delayMicroseconds(1000); // opóżnienie 1 milisekunda
	delay(1000); // opóżnienie 1 sekunda
	//sleep(60);
	Vo2=encoderPos_L;
	//cout << " encoderPos_L  "<< Vo1 << "encoderPos_L " << Vo2 <<" \n" << endl;
	float predkosc_obrotowa=abs(Vo2-Vo1)/560;  // predkość obrotów na sekunde [obr/s]
	//float predkosc_obrotowa=(abs(Vo2-Vo1)/560)*60;  // predkość obrotów na sekunde [obr/min]
	//cout << "Predkosc obrotowa" << predkosc_obrotowa  << " obr/s " << endl;
	
	//predkosc_liniowa = predkosc_obrotowa /( 3.14 * 2 * 60 * promien); // predkosc dla [obr/min]
	predkosc_liniowa = (predkosc_obrotowa * (3.14 * 2 * promien)); // predkosc dla [m/s]
	return predkosc_liniowa;
}


// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
float predkosc_silnikow_fast()
{
	float predkosc_liniowa=0;
	//float promien=0.035 ; // w metrach [m]
	float promien=3.5 ; // w metrach [cm]
	float Vo1, Vo2;
	
	// 480 impulsów to jeden pełny obrót 
	Vo1=encoderPos_L;
	//delayMicroseconds(10000); // opóżnienie 10 milisekund
	delay(100); // opóżnienie 0.1 sekunda
	//sleep(60);
	Vo2=encoderPos_L;
	//cout << " encoderPos_L  "<< Vo1 << "encoderPos_L " << Vo2 <<" \n" << endl;
	float predkosc_obrotowa=(abs(Vo2-Vo1)/560);  // predkość obrotów na sekunde [obr/s]
	//float predkosc_obrotowa=(abs(Vo2-Vo1)/560)*60;  // predkość obrotów na sekunde [obr/min]
	//cout << "Predkosc obrotowa" << predkosc_obrotowa  << " obr/s " << endl;
	
	//predkosc_liniowa = predkosc_obrotowa /( 3.14 * 2 * 60 * promien); // predkosc dla [obr/min]
	predkosc_liniowa = (predkosc_obrotowa * (3.14 * 2 * promien)/10); // predkosc dla [m/s]
	//predkosc_liniowa = (predkosc_obrotowa * (3.14 * 2 * promien)*10); // predkosc dla [cm/s]
	return predkosc_liniowa;
}






// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
float predkosc_silnikow_enocder_tick()
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
void zapis_do_pliku()
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
void zapis_do_pliku_encoder()
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
void predkosc_silnikow_PID(float speed)
{
	
	float wyjscie=predkosc_silnikow_fast();
    float wejscie=regulator1(speed,wyjscie);
    wejscie=wejscie/1000;
    robot_setVel(wejscie);
	printf( "Ustawiona predkosc: %f aktualna predkosc: %f: cel: %f \n\n\n", wejscie,wyjscie, speed);	 
	
}

// -------------------------------------------------------------------------



// -------------------------------------------------------------------------

void predkosc_silnikow_PID2(float speed)
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

void predkosc_silnikow_PID3(float speed)
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

float predkosc_silnikow_PID4(double Setpoint, double  Input)
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
  
  printf( "Ustawiona predkosc pid: %f aktualna predkosc: %f: cel: %f \n\n\n", Output,Input, Setpoint);	 
	
//}
  
  return Output;
  
}




// -------------------------------------------------------------------------
//

// -------------------------------------------------------------------------

void predkosc_silnikow_PID_slow(float speed)
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
  Input = predkosc_silnikow();
  Setpoint = 0.5;

  //turn the PID on
  myPID.SetMode(AUTOMATIC);

  
  while(1)
{
  Input = predkosc_silnikow();

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
	
}
  
  
  
}




// -------------------------------------------------------------------------
//
// -------------------------------------------------------------------------
void magnetometr()
{
    HMC5883L hmc5883l;

  // Initialize

  if( hmc5883l_init(&hmc5883l) != HMC5883L_OKAY ) {
      fprintf(stderr, "Error: %d\n", hmc5883l._error);
      exit(1);
  }

 hmc5883l_read(&hmc5883l);  
  printf("X, Y, Z:\t%f | %f | %f \n", 
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
                
        


}




// -------------------------------------------------------------------------


// -------------------------------------------------------------------------
void setup()
{
	inicjalizacja_wiringPi();
	ustawienie_pinow();
	inicjalizacja_przerwania_enkoderow();
	
	pwm_silnik.init(1,0x40);
	pwm_silnik.setPWMFreq (500);
	
	sonar.init(TRIG, ECHO);
}




// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
int main()
{

	setup();

	//sterowanie_serwo();
	
	go_to_encoder_pos(960);    // 1920  480 560 impulsów to cały obrót 

	//silnik_set_speed_Forward(0);
	//delay(1000);
   // zapis_do_pliku();
   //zapis_do_pliku_encoder();
   



  
   
   int i=0;
 while ( 1 ) 
	{
    // magnetometr();
           
  //   i++;           
  //  delay(1000);
    
//printf("Distance: %dcm\n", getCM());
   //printf("Distance: %f cm\n", get_distance());
  //  sleep(5);
   //cout << "Distance is " << sonar.distance(30000) << " cm." << endl;
  // silnik_stop();
    //predkosc_silnikow_PID(0);
    //predkosc_silnikow_PID4(10);
   // robot();
  // robot2();
   //cout << "Predkosc fast " << predkosc_silnikow_fast() << " m/s " << endl;
   //cout << "Predkosc      " << predkosc_silnikow() << " m/s " << endl;

  
	}

        


    return 0;

}
// -------------------------------------------------------------------------
