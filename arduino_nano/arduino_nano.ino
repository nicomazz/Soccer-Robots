#include "Definitions.h"


// versione veloce, compara solo la componente CLEAR
// il set del color clear è nell'init
// RITORNA IL COLORE CHE HA SOTTO
byte VerificaColoreFast(){
  // per bianco
    for (int i = 0; i < 100; i++)
        if ( pulseIn(2, HIGH, 800) > biancoHF &&  pulseIn(4, HIGH, 800) > biancoHF) return false;
    return true;
}

void setup(){
  //Serial.begin(9600);
    pinMode(2,INPUT);
    pinMode(4,INPUT);

    pinMode(COL_RESET,INPUT);
    pinMode(COL_OUT,OUTPUT);
    pinMode(13,OUTPUT);
}

void debug(){
    Serial.print( pulseIn(2, HIGH, 800));
    Serial.print("  ");
    Serial.println( pulseIn(4, HIGH, 800));
}

void loop(){
    if (VerificaColoreFast())
    {
        // se c'è un colore bianco/nero sotto entra qui
        digitalWrite(13,HIGH);
        digitalWrite(COL_OUT,HIGH);
        boolean reset = digitalRead(COL_RESET);
       while (digitalRead(COL_RESET) == reset); // attende fin quando col_reset non viene invertito dall'arduino mega
       digitalWrite(COL_OUT,LOW);

    }
    else 
        digitalWrite(13,LOW);

}
