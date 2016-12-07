#include <stdio.h> 
#include <wiringPi.h> 
#include <stdlib.h>

int main()
{
    //system("mpc stop");
    //system("mpc clear");   
   // system("mpc add http://31.192.216.7:8000/rmf_maxxx");
   // system("mpc play");   
system("mpc add http://31.192.216.7:8000/rmf_maxxx");
 
 
    if (wiringPiSetup() == -1) 
    { 
        printf("Nie mozna wystartowac wiringPi!\n"); 
        return 1; 
    } 
 
    pinMode(7,OUTPUT);
    pinMode(9,INPUT);
    pullUpDnControl(9,PUD_UP);
    
    //char key=0; //zmienna pamiętająca naciśnięcie przycisku
    //char play=0; //zmienna pamiętająca odtwarzanie radia
    
    
     for(;;)
   {

        if (digitalRead(9)==0  ) //jeśli przycisk naciśnięty i pamięć naciśnięcia pusta i radio nie jest odtwarzane
        {
           // key=1; //ustaw pamięć przycisku
          //  play=0; //ustaw pamięć odtwarzania
            delay(100); //eliminacja drgania styków
            
            system("mpc play");
            
            digitalWrite(7,1); //zaświeć diodę LED na gpio7
        }

        if (digitalRead(9)==1  ) //jeśli przycisk naciśnięty i pamięć naciśnięcia pusta i radio jest odtwarzane
       {
         //   key=1; //ustaw pamięć przycisku
           // play=0; //skasuj pamięć odtwarzania
            delay(100);
            system("mpc pause");
          // system("mpc clear");
            digitalWrite(7,0); //wygaś diodę LED na gpio7
        }

       // if (digitalRead(9)==1 && key==1) key=0; //jeśli przycisk zwolniony i pamięć przycisku ustawiona to skasuj pamięć przycisku
       // delay(100);

    }


    return 0;

}
