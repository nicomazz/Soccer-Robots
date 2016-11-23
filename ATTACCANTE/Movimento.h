
#ifndef _MOVIMENTO
#define _MOVIMENTO

#include "Definitions.h"
#include "MisuraUsAffidabile.h"
#include "Motori.h"       // libreria con tutti i movimenti dei motori possibili
#include "Sensori.h"

// qui presenti le funzioni di movimento




class Movimento
{
public:
    Movimento();
    
	
    void _delay(int ms);         // delay che tiene conto del fuoricampo
    void controlla_fuori_campo();//
	
    void setSensori(sensori & sens); // imposta che sensori usare
    void aggiusta_orientamento(int vel = VelR); 
    void orientamentoGiusto_assolutamente(); 
    
    void orienta_PID();
    
    bool gestisciFuoriCampo(int volta); // fa rientrare il robot in campo
    void CercaPalla();
    void dribbler(); // se non è affidabile la lunghezza, e ussu da poca distanza, per togliere la palla agli altri robot

    void InPortaAngolando();
    void SegnaInPorta(); // va semplicemente avanti
    void giraAdAngolo(int angolo); // da 0 a 179 in senso orario, da -1 a -180 in sento antiorario
    bool aggiusta_pos_per_segnare();
    void giraAdAngoloPerSegnare(); // capisce se girare a dx o sinistra per tirare in porta

    bool vai_a_posizione(int x, int y,bool leggi = true); // funziona che è una bellezza
    
    bool difendi_porta();   // DA FARE!!  
    
    
    int compass;
private:
    

    sensori s;
    
    
    bool aggiusta_orientamento_PID(int orientamento);
    
    
    /////// PROVA PID CON ROTAZIONE
    float P; //Proporzionale
    float I; //Integrativa
    float D; //Derivativa
    int previousError;// = 0;
    float previousTime;// = time;
    ///////
    
};
#endif
