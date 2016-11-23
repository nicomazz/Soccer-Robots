//#define DEBUG_SENSORI

#include <Wire.h>            //I2C Library

#include "TSL2561.h"       // libreria sensore di luminosita'


#include "Motori.h"       // libreria con tutti i movimenti dei motori possibili
#include "Definitions.h" // definizioni utili
#include "Sensori.h"
#include "Movimento.h"
#include "MisuraUsAffidabile.h"
#include "PCF8574.h"


#ifdef OLED_ATTACCATO
#include "U8glib.h"
#endif



#pragma mark - variabili globali

///GLOBAL VAR

// TODO fare una libreria per gesitre tutti i sensori qui usati, evitando tante funzioni
//#define LUM_ATTACCATO
TSL2561 lum(TSL2561_ADDR_LOW);    // sensore di luminosità

bool PallaPresa = false;  //**<quando si prende la palla, va a true

bool AffidabileLati;
bool AffidabileLunghezza;

int compass;              //direzione della bussola

sensori s;// = sensori::getIstance();
Movimento m;


//prototipo
void aggiusta_orientamento(int);


#pragma mark - SENSORE DI LUCE

/**/

/*
 -------------------------------------
 -------------------------------------
 -------GUARDA-LINEE-FUORICAMPO-------
 -------------------------------------
 -------------------------------------
 */



/**
 * -------------------------------------
 * ---------------MOTORI----------------
 * -------------------------------------
 * -Schema:
 * +   +
 * 1 /     \ 2
 * -     _   -
 * -  3   +
 * 
 * - TUTTI PWM
 * - VALORI PER MOTORI: da -100 a 100
 * TRATTINO MOTORI DALLA PARTE OPPOSTA DELL'USB
 * 
 */



#pragma mark - SENSORI IR

/**
 * -------------------------------------
 * -------------------------------------
 * ----------------TSOP----------------- 
 * -------------------------------------
 * -------------------------------------
 * 
 * TSOP numerati in senso orario, a partire dal numero 0 posto dietro al robot
 * 
 * @see tsop[24]
 * 
 */


/*
 -------------------------------------
 -------------------------------------
 -------------FUORI-CAMPO-------------
 -------------------------------------
 -------------------------------------

*/



#pragma mark - MOVIMENTO

/* 
 -------------------------------------
 -------------------------------------
 -------------CERCA-PALLA-------------
 -------------------------------------
 -------------------------------------
 
 */

bool palla_presa()
{
  if (lum.getLuminosity(TSL2561_VISIBLE) < lucePallaPresa) return true;
  else return false;
}
bool palla_vicina()
{
  if (s.getSegnale_TSOP_MAX() > 580) return false; // palla lontana
  return true;
}

void leggi_affidabilita()
{
    s.affidabilita();
    AffidabileLati       =  s.Affidabile_dxsx;
    AffidabileLunghezza  =  s.Affidabile_sugiu;
    s.setLed(1,AffidabileLati);
    s.setLed(2,AffidabileLunghezza);
}

const int angoliTSOP[TSOP_NUM] = {-170,-100,-60,0,0,60,100,180};// angoli di movimento associato ad ogni tsop per cercare la palla
  //cambiati sia questi sia us quando esce
  
  //VA AVANTI SOLO QUANDO CE L'HA DAVANTI

void CercaPalla()
{    
  if (s.TSOP_max == TSOPNULL || s.TSOP_max == -1){ 
    FermaMotori(); 
    return;
  }
  if (s.TSOP_max == 3 || s.TSOP_max == 4 )
  {
    
        Avanti_con_correzione(100,compass = s.getOrientamento());
     
  }
 else if (s.TSOP_max == 0 || s.TSOP_max == 7) Indietro_con_correzione(100, s.getOrientamento());
 else 
    MuoviAngolo(angoliTSOP[s.TSOP_max]);
}
void CercaPalla2()
{
if (s.TSOP_max == TSOPNULL || s.TSOP_max == -1){ 
    FermaMotori(); 
    return;
  }
  if (s.TSOP_max == 3 || s.TSOP_max == 4 )
  {
    
        Avanti_con_correzione(100,compass = s.getOrientamento());
     
  }
 else if (s.TSOP_max == 0 || s.TSOP_max == 7) Indietro_con_correzione(100, s.getOrientamento());
 else 
 {
       int angolo; 
      if ( s.TSOP_max == 1 || s.TSOP_max == 2) angolo = -110;
      else angolo = 110;
      
      long long t_iniz = millis();
     
    while( s.TSOP_max != 3 && s.TSOP_max!= 4 && millis()-t_iniz < 1000)
      {
          MuoviAngolo(angolo);
          s.readAllTsop(false); // false per non scaricare i condensatori, per una lettura più veloce
       }
     if ( millis()-t_iniz >= 1000)
               Avanti_con_correzione(100,compass = s.getOrientamento());

  
  
  
  }

}
 int ang[TSOP_NUM] = {-135,-90,-45,-20,20,45,90,135};

void CercaPalla3()
{
    
     if (s.TSOP_max == TSOPNULL || s.TSOP_max == -1)
    {
        FermaMotori();
        return;
    }
    if (!palla_vicina())
    {
            MuoviAngolo(ang[s.TSOP_max]);
            m._delay(100);
            return;

    }
     else if ( s.TSOP_max == 3 || s.TSOP_max == 4)
    {
        if (palla_presa()) // se ha la palla in bocca
        {
          Serial.println("PALLA  PRESA!!");
            //m.InPortaAngolando();
            //m._delay(500);
            Avanti(); // intanto va avanti e basta
            s.readUSDXSX(); // legge solo gli ultrasuoni a destra e a sinistra
            s.readUsSu();
            
            if (USSu > 10 && USSu < 110)
            {
            // da verificare gli angoli, ma dovrebbe andare
                m.giraAdAngoloPerSegnare();
              //  Avanti();
                m._delay(300);
                Avanti();
            }
            else
            {
                if (USSu < 10 ) // ha assolutamente un robot sopra
                {
                    m.dribbler();
                }
                else
                {
                    m.InPortaAngolando();
                    m._delay(200);
                }
            }
        }
        else
        {
            Avanti_con_correzione(100,s.getOrientamento());
            m._delay(130);
        }
    }
    else if (s.TSOP_max == 0)
    {
        MuoviAngolo(150);
        m._delay(50);
    }
    else if(s.TSOP_max==7)
    {
        MuoviAngolo(-150);
        m._delay(50);
    }
    else if(s.TSOP_max == 1 || s.TSOP_max == 2 || s.TSOP_max == 5 || s.TSOP_max == 6)
    {
        if(s.TSOP_max == 1 || s.TSOP_max == 2) MuoviAngolo(-130);
        else MuoviAngolo(130);
        //Indietro_con_correzione(100,s.getOrientamento());
        m._delay(50);
    }
   
}

#define sogliaRotazione 15
void aggiusta_orientamento_assolutamente()
{
  compass = s.getOrientamento();
    
    if (FuzzyEgual(compass, 0, 10)); // se è già dritto, esce subito
    else {
        do {
            aggiusta_orientamento(compass);
            compass = s.getOrientamento();		// fino a quando non è nella direzione giusta
        } while (!FuzzyEgual(compass,0,10));
    }
}
/* già definiti in Definitons.h
 #define USDx Ultrasuoni[2]
 #define USSx Ultrasuoni[3]
 #define USSu Ultrasuoni[0]
 #define USGiu Ultrasuoni[1]
 #define FuzzyEgual(x,v,FuzzyEgual) (v-FuzzyEgual <= x && x <= v+FuzzyEgual) 
 */

//DA CHIAMARE DOPO AVER LETTO GLI ULTRASUONI
/**
 * 	@brief verifica in anzi tutto che il robot sia orientato in modo giusto
 * 	verifica poi se si può contare sugli ultrasuoni dxsx e sugiu
 * 	a seconda di ciò che è più affidabile, fa le relative azioni
 * @return true se si è posizionato nel punto giusto, ed e' pronto a segnare, false altrimenti
 */





void movimento90Gradi()
{
    if (s.TSOP_max == -1)
    {
        FermaMotori();
        return;
    }
    if (palla_vicina())
    {
      Serial.println("VICINA!");
    if ( s.TSOP_max == 3 || s.TSOP_max == 4 )  
          Avanti_con_correzione(100,s.getOrientamento());
      else 
        MuoviAngolo(ang[s.TSOP_max] + 90* (ang[s.TSOP_max] <0 ? -1:1));
       //if ( s.TSOP_max == 0 || s.TSOP_max == 7 )
         // m._delay(100);
    }
    else
    {
      
   /*   if (s.TSOP_max == 4 || s.TSOP_max == 5 || s.TSOP_max == 6)
        Destra();
       else if (s.TSOP_max == 1 || s.TSOP_max == 2 || s.TSOP_max == 3)
       Sinistra();
       else */
        MuoviAngolo(ang[s.TSOP_max]);
      // m._delay(300);
    } 
}

/*
 -------------------------------------
 -------------------------------------
 ---------------MOTORI----------------
 -------------------------------------
 -------------------------------------
 -Schema:
 +   +
 1 /     \ 2
 -     _   -
 -  3   +
 
 - TUTTI PWM
 - VALORI PER MOTORI: da -100 a 100
 */
void init_motors()
{
  pinMode(Motor1A,OUTPUT);
  pinMode(Motor1B,OUTPUT);
  pinMode(Motor2A,OUTPUT);
  pinMode(Motor2B,OUTPUT);
  pinMode(Motor3A,OUTPUT);
  pinMode(Motor3B,OUTPUT);
}

unsigned long tpr = 0;
bool att =true;

void led_loop()
{
    //s.setLed(0,att);
    att = !att;
}
//TSL2561 lumin(0x29);
void debug_sensori()
{
  // sensore di luminosità:
  Serial.print("palla presa: ");
  Serial.println(palla_presa());
  
  //tsop:
  Serial.print("TSOP: ");
  s.debugTsopSerial();
  Serial.print("tsop che prende di più: ");
  Serial.println(int(s.readAllTsop()));

  
  //bussola:
  Serial.print("Compass: ");
  Serial.println(s.getOrientamento());
  
  //ultrasuoni
  Serial.print("US: ");
  s.debug_US();
  
  //tutto affidabile
  Serial.print("Tutte le misure us sono affidabili?");
  Serial.println(s.affidabilita());
  
  Serial.print("bianco sotto? :"); Serial.println(s.linea_bianca());
    
}
void setup()
{
  
  Wire.begin();
  Serial.begin(9600);
  Serial.println("partito");
  
   lum.setGain(TSL2561_GAIN_0X); //**< sensibilita' al minimo, da cambiare se serve
   lum.setTiming(TSL2561_INTEGRATIONTIME_13MS); //**< misurazione più veloce e imprecisa, ma ce la facciamo andar bene
   s.init_bussola(); 

   s.setTsopOn();
  init_motors(); 
  
  // TUTTE LE INIZIALIZZAZIONI SONO STATE FATTE ALLA CREAZIONE DI Sensori s.
 // compass = 0;
  
 // prova_motori();
  //delay(10000);
//Serial.println(A0);  
//FermaMotori();

}
int cnt = 0;
void loop()
{

#ifdef DEBUG_SENSORI
  debug_sensori();
  delay(4000);
  return;
#endif

Serial.println();
Serial.println();
Serial.println("orientamento");
  m.aggiusta_orientamento();
  Serial.println("tsop");

  Serial.println(int(s.readAllTsop()));
 // delay(500);
 // return;
  Serial.println("movimento");

  CercaPalla3();
  Serial.println("fuori_campo");
  
  
  m._delay(1);

/*
  
 // cnt++; ora lo farà sempre
  if ( cnt % 10 == 0)
  {
       s.readUSDXSX(); // legge solo gli ultrasuoni a destra e a sinistra
       Serial.print("US sx: ");
       Serial.println(USSx);
       Serial.print("US dx: ");
       Serial.println(USDx);
       if ( USSx == 0 || USDx == 0);
       else if ( USSx < USDx) m.corr_orientamento = 30;
       else m.corr_orientamento = -30;
  }*/
  

}
