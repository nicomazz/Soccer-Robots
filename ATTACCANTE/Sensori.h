
#ifndef SENSORI
#define SENSORI

#include "TSL2561.h"       // libreria sensore di luminosita'
#include "Definitions.h"
#include "MisuraUsAffidabile.h"
#include <Arduino.h>

#ifdef OLED_ATTACCATO
  #include "OLED.h"
  #include "U8glib.h"
#endif

class Movimento; // (per usare poi il _delay)
class sensori
{
public:
    
    sensori();
    //void setMovimento(Movimento &_m);
    
    // BUZZER
    void bip(int ms);
    
    // LUMINOSITA'
    TSL2561 lum;
    bool PallaPresa;
    bool readPalla_Presa();
    
    bool col_reset_state; // per notificare all'arduin o nano il ricevuto messaggio si inverte lo stato di un pin
    void reset_col();
	
	///TSOP
    char TSOP_max;
    char readAllTsop(bool condensatori = true);
    void debugTsopSerial();
    int getTsopN(char n);
    int getSegnale_TSOP_MAX();
    
    //BUSSOLA
    int getOrientamento();
    void init_bussola();
    void modificaStartDir(int diff);
    
    //OLED 
#ifdef OLED_ATTACCATO
    OLED _OLED;
    void Oled(int n, int pos);
    void scrivi_oled();
#endif

    //US
    void debug_US();
    void readUS();
    void readUS_giusti();
    
    void readUSDXSX();
    int readUsSu();
    static int Ultrasuoni[4];

    
    //AFFIDABILITA POSIZIONE
    //l'ultima misura degli ultrasuoni dove risulta tutto affidabile viene messa qui dentro, con i millisecondi in cui è stata effettuata
    MisuraUsAffidabile Ultima_misura_us_affidabile;
    bool ultima_abbastanza_affidabile(); // controlla la differenza di tempo, e vede se è abbastanza affidabile
    bool affidabilita(); // se è tutto affidabile ritorna true, false altrimenti
    bool Affidabile_dxsx;
    bool Affidabile_sugiu;
    
    
    Movimento * mov; // per usare principalmente _delay //IMPORTANTE! DA SETTARE NEL SETUP
private:
    
   // Movimento m;
    
    struct bussola
    {
      
        bussola();
        void init_bussola();
        int getOrientation();
        int startDir;
      
        void modifica_start_dir(int diff);
        
        void Write(int address, int byte);
	uint8_t* Read(int address, int length);

       float m_Scale;
    } compass;
    
    struct US
    {
        US(); // costruttore
        void debug_US_serial();
        int getUS(int N);
        bool US_affidabile_dx_sx();
        bool US_affidabile_su_giu();
        
        // int Ultrasuoni[4]; // valori degli ultrasuoni
        short usPIN[4];
    } ultrasuoni;
    
    struct TSOP
    {
        TSOP();
     //   Movimento m;
        void setTsop(bool accesi); // toglie o da l'alimentazine ai tsop
        
        void controlla_scarica_condensatori();
        char readTsopAll(bool condensatori);
        char readTsopAll_medium(int volte);
        int getTsopN(int N);
        
        int segnale_Tsop_max;
        
        void debug_TSOP_serial();
        char scaricaCondensatori;
    } Tsop;
    
    
   
    
};

#endif
