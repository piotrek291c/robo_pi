#include <stdio.h> 
#include <wiringPi.h> 
 
int main(int argc, char **argv) 
{ 
    //Zmienna pomocnicza 
    //int i; 
 
    //Komunikat poczatkowy 
    printf("Program testowy WiringPi!\n"); 
 
    if (wiringPiSetup() == -1) 
    { 
        printf("Nie mozna wystartowac wiringPi!\n"); 
        return 1; 
    } 
 
    //Ustawiamy jako wyjscie 
    //for (i=0;i>8;i++) pinMode(i,OUTPUT); 
    pinMode(7,OUTPUT); 
	pinMode(9,INPUT);
    pullUpDnControl(9,PUD_UP);
    //Zapisujemy 1 na wszystkich wyjsciach 
    //digitalWriteByte(255); 
 for(;;)
    {
		if (digitalRead(9)==0) //jeśli przycisk naciśnięty i pamięć naciśnięcia pusta i radio nie jest odtwarzane
        {
    //Zapisujemy 0 na wyjsciu 2 
    
    digitalWrite(7,0); 
        }

		if (digitalRead(9)==1) //jeśli przycisk naciśnięty i pamięć naciśnięcia pusta i radio nie jest odtwarzane
        {
    //Zapisujemy 0 na wyjsciu 2 
    
    digitalWrite(7,1); 
        }
 
    //Ustawiamy pin 1 jako wejscie 
    //pinMode(i,INPUT); 
 
    //Sciagamy pin 1 do masy 
    //pullUpDnControl(1,PUD_DOWN); 
 
    //Wyswietlamy wartosc pin 1 
    printf("Wartosc pinu 7 : %i\n",digitalRead(7)); 
    printf("Wartosc pinu 9 : %i\n",digitalRead(9)); 
 }
    return 0; 
}
