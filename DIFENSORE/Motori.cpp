#include "Motori.h"
#include <Arduino.h>
const float molt = 1.6f;//1.6;//.53;//1;//1.53; //0.8; //1.53;

void setVel(float vel)
{
 //   molt = vel;
}

int a,b,c;

void M1(int velocita)
{
    int vel = velocita*molt; // uscite 0..255
    a = velocita;
    if (vel >= 0)
    {
        analogWrite(Motor1A,vel);
        digitalWrite(Motor1B,LOW);
    }
    else
    {
        analogWrite(Motor1B,abs(vel));
        digitalWrite(Motor1A,LOW);
    }
}

void M2(int velocita)
{
    int vel= velocita*molt; // uscite 0..255
    b = velocita;
    if (vel >= 0)
    {
        analogWrite(Motor2A,vel);
        digitalWrite(Motor2B,LOW);
    }
    else
    {
        analogWrite(Motor2B,abs(vel));
        digitalWrite(Motor2A,LOW);
    }
}
void M3(int velocita)
{
    int vel = velocita*molt; // uscite 0..255
    c = velocita;
    if (vel >= 0)
    {
        analogWrite(Motor3A,vel);
        digitalWrite(Motor3B,LOW);
    }
    else
    {
        analogWrite(Motor3B,abs(vel));
        digitalWrite(Motor3A,LOW);
    }
}


/*
 
 -Schema:
 +   +
 1 /     \ 2
 -    _    -
 -  3   +
 
 */
#define M(x,y,z) M1(x); M2(y); M3(z)

void FermaMotori()      {  M(0,0,0);        }



void RuotaDx(int vel)         	{  M(vel,-vel,-vel);}
void RuotaSx(int vel)	        {  M(-vel,vel,vel);}

void Avanti()          {  M(100,100,0);      }
void Indietro()        {  M(-100,-100,0);    }
void Destra()          {  M(40,-40,100);     }
void Sinistra()        {  M(-40,40,-100);    }


void IndIndDx()         {  M(-60,-100,0);    }
void IndDx()            {  M(0,-100,100);    }
void IndDxDx()          {  M(0,-70,60);      }

void AvDxDx()           {  M(70,0,60);       }
void AvDx()             {  M(100,0,100);     }
void AvAvDx()           {  M(100,60,0);      }


void AvAvSx()           {  M(60,100,0);      }
void AvSx()             {  M(-100,0,-100);    }
void AvSxSx()           {  M(0,70,-60);      }


void IndSxSx()          {  M(-70,0,-60);     }
void IndSx()            {  M(-100,0,-100);   }
void IndIndSx()         { M(-100,-60,0);    }



// velocita' va da 0..100
void Avanti_con_correzione(int vel, int compass)
{
	M(vel,vel,-compass*3.3);
} // usa ciò che dice la bussola per andare dritto

void Indietro_con_correzione(int vel, int compass)
{
	M(-vel,-vel, compass*3.3);
}

void Inverti_direzione()
{
	M(-a,-b,-c);
	
    //FermaMotori();
}
void Ferma_subito_motori()
{
	
	
    M(-a,-b,-c);
	delay(20);
	M(0,0,0);
}

//const int coseno[] = { 100,99,99,99,99,99,99,99,99,98,98,98,97,97,97,96,96,95,95,94,93,93,92,92,91,90,89,89,88,87,86,85,84,83,82,81,80,79,78,77,76,75,74,73,71,70,69,68,66,65,64,62,61,60,58,57,55,54,53,51,50,48,46,45,43,42,40,39,37,35,34,32,30,29,27,25,24,22,20,19,17,15,13,12,10,8,7,5,3,1,0,-1,-3,-5,-6,-8,-10,-12,-13,-15,-17,-18,-20,-22,-24,-25,-27,-29,-30,-32,-34,-35,-37,-38,-40,-42,-43,-45,-46,-48,-49,-51,-52,-54,-55,-57,-58,-60,-61,-62,-64,-65,-66,-68,-69,-70,-71,-73,-74,-75,-76,-77,-78,-79,-80,-81,-82,-83,-84,-85,-86,-87,-88,-89,-89,-90,-91,-91,-92,-93,-93,-94,-95,-95,-96,-96,-96,-97,-97,-98,-98,-98,-99,-99,-99,-99,-99,-99,-99,-99,-99,-99,-99,-99,-99,-99,-99,-99,-99,-98,-98,-98,-97,-97,-97,-96,-96,-95,-95,-94,-94,-93,-92,-92,-91,-90,-89,-89,-88,-87,-86,-85,-84,-83,-83,-82,-81,-79,-78,-77,-76,-75,-74,-73,-72,-70,-69,-68,-67,-65,-64,-63,-61,-60,-58,-57,-56,-54,-53,-51,-50,-48,-47,-45,-44,-42,-40,-39,-37,-36,-34,-32,-31,-29,-27,-26,-24,-22,-21,-19,-17,-15,-14,-12,-10,-8,-7,-5,-3,-1,0,1,3,4,6,8,10,11,13,15,17,18,20,22,23,25,27,28,30,32,33,35,37,38,40,42,43,45,46,48,49,51,52,54,55,57,58,59,61,62,64,65,66,67,69,70,71,72,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,88,89,90,91,91,92,93,93,94,95,95,96,96,96,97,97,98,98,98,98,99,99,99,99,99,99,99};
//onst int seno[] = { 0,1,3,5,6,8,10,12,13,15,17,19,20,22,24,25,27,29,30,32,34,35,37,39,40,42,43,45,46,48,49,51,52,54,55,57,58,60,61,62,64,65,66,68,69,70,71,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,89,90,91,92,92,93,93,94,95,95,96,96,97,97,97,98,98,98,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,98,98,98,97,97,97,96,96,95,95,94,94,93,92,92,91,90,89,89,88,87,86,85,84,83,82,81,80,79,78,77,76,75,74,73,72,70,69,68,67,65,64,63,61,60,58,57,56,54,53,51,50,48,47,45,43,42,40,39,37,35,34,32,31,29,27,26,24,22,20,19,17,15,14,12,10,8,7,5,3,1,0,-1,-3,-5,-6,-8,-10,-12,-13,-15,-17,-18,-20,-22,-24,-25,-27,-29,-30,-32,-34,-35,-37,-38,-40,-42,-43,-45,-46,-48,-49,-51,-52,-54,-55,-57,-58,-60,-61,-62,-64,-65,-66,-68,-69,-70,-71,-72,-74,-75,-76,-77,-78,-79,-80,-81,-82,-83,-84,-85,-86,-87,-88,-89,-89,-90,-91,-91,-92,-93,-93,-94,-95,-95,-96,-96,-96,-97,-97,-98,-98,-98,-98,-99,-99,-99,-99,-99,-99,-99,-99,-99,-99,-99,-99,-99,-99,-99,-99,-98,-98,-98,-97,-97,-97,-96,-96,-95,-95,-94,-94,-93,-92,-92,-91,-90,-89,-89,-88,-87,-86,-85,-84,-84,-83,-82,-81,-80,-78,-77,-76,-75,-74,-73,-72,-70,-69,-68,-67,-65,-64,-63,-61,-60,-59,-57,-56,-54,-53,-51,-50,-48,-47,-45,-44,-42,-40,-39,-37,-36,-34,-32,-31,-29,-27,-26,-24,-22,-21,-19,-17,-15,-14,-12,-10,-9,-7,-5,-3,-2,};

void MuoviAngolo(int angolo)
{
    while (angolo < 0) angolo+= 360;
    while (angolo > 360) angolo-= 360;
	
    float cos1 = cos(GRADI_IN_RADIANTI((angolo+330) % 360));
    float coseno = cos(GRADI_IN_RADIANTI((angolo+30) % 360));
    float seno = sin(GRADI_IN_RADIANTI(angolo));
   // Serial.println(cos1);
    M(cos1*100,coseno*100,seno*100);
     // M(coseno[(angolo+330) % 360],coseno[(angolo+30) % 360],seno[angolo]);
}

void MuoviAngolo_con_correzione(int angolo, int compass)
{
    while (angolo < 0) angolo+= 360;
    while (angolo > 360) angolo-= 360;
 //   M(coseno[(angolo+330) % 360]-compass*5,coseno[(angolo+30) % 360]-compass*5,seno[angolo]-compass*5);
}

void prova_motori()
{
    M3(50);
    delay(2000);
    M3(-50);
    delay(2000);
    M3(0);
    
    M2(50);
    delay(2000);
    M2(-50);
    delay(2000);
    M2(0);
    
    M1(50);
    delay(2000);
    M1(-50);
    delay(2000);
    M1(0);
}



