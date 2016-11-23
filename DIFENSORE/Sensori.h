#include "TSL2561.h"       // libreria sensore di luminosita'
#include "Definitions.h"
#include "MisuraUsAffidabile.h"
//#include "U8glib.h"
#include "PCF8574.h"


#ifndef Sensor
#define Sensor

// è ( o meglio, era) un singeton
class sensori
{
  
public:
    static bool inizializzato;
   //static sensori& getIstance(); // per mezzo di questo si accede all'unica istanza
    sensori();
 
    //Expander i2c
    PCF8574 expander;
    void setLed(short ledN, bool on); // vanno da 0 a 3, compresi 
    
    
    ///TSOP
    char TSOP_max;             //tsop con più segnale
    static int Ultrasuoni[4];  // valori degli ultrasuoni
    char readAllTsop(bool condensatori = true);        // legge tutti i tsop e restituisce l'indice del maggiore
    void debugTsopSerial();    // scrive tramite seriale tutti i valori dei tsop
    int getTsopN(char n);      // prende l'uscita del tsop i-esimo
    void setTsopOn();
    int getSegnale_TSOP_MAX();
    
    //BUSSOLA
    static int compass_xyz[3];
    int getOrientamento();
    void init_bussola();
    void modificaStartDir(int diff);
    
#ifdef OLED_ATTACCATO
    //OLED
     int n_oled[5]; // sx_su dx_su sx_giu dx_giu grande
     U8GLIB_SSD1306_128X64 OLED;
     void Oled(int n, int pos);
     void scrivi_oled();
#endif
    
    //US
    void debug_US();
    void readUS();
    void readUSDXSX();
    int readUsSu();
    
    //COLORE
    bool linea_nera();
    bool linea_bianca();
    int getColor1();
    int getColor2();
    
    //AFFIDABILITA POSIZIONE
    //l'ultima misura degli ultrasuoni dove risulta tutto affidabile viene messa qui dentro, con i millisecondi in cui è stata effettuata
    MisuraUsAffidabile Ultima_misura_us_affidabile;
    bool ultima_abbastanza_affidabile(); // controlla la differenza di tempo, e vede se è abbastanza affidabile
    bool affidabilita(); // se è tutto affidabile ritorna true, false altrimenti
    bool Affidabile_dxsx;
    bool Affidabile_sugiu;
    
    static int n_istanze; // a scopi di debug
private:
    
  
    void init();

    struct colore
    {
        colore();
        bool linea_bianca();
        bool linea_nera();
        int read_color1();
        int read_color2();
    } color;
    
    struct bussola
    {
        bussola();
        void init_bussola();
        int getOrientation();
        static int startDir;
        
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
        void setExpander(PCF8574 _expander);
        void setTsop(bool accesi); // toglie o da l'alimentazine ai tsop
        
        void controlla_scarica_condensatori();
        char readTsopAll(bool condensatori);
        char readTsopAll_medium(int volte);
        int getTsopN(short N);
        void set_tsop_MUX(short n);

        int segnale_Tsop_max;
        
        void debug_TSOP_serial();
        char scaricaCondensatori;
        PCF8574 expander;
    } Tsop;
    
    
   
    
};

#endif
