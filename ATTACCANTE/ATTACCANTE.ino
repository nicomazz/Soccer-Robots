#define kCOLOR_SENSOR
#define kCOMPASS
#define kOLED
//#define DEBUG
//#define DEBUG_SENSORI

#include <Wire.h>
#include <Servo.h>
#include "TSL2561.h"             // sensore luminosita

#include "Motori.h"           // libreria con tutti i movimenti dei motori possibili
#include "Definitions.h"      // definizioni utili
#include "Sensori.h"       // per interfacciamento con tutti i sensori ( compreso OLED)
#include "Movimento.h"
#include "MisuraUsAffidabile.h"

#ifdef OLED_ATTACCATO
#include "U8glib.h"           // grafica oled
#include "OLED.h"
#endif



#pragma mark - variabili globali


bool PallaPresa = false;  //**<quando si ha la palla, va a true

bool AffidabileLati;    // affidabile dx-sx
bool AffidabileLunghezza;  // affidabile su-giu

int compass;              //direzione della bussola
sensori s;           // per leggere tutti i sensori
Movimento m;         //funzioni di movimento

TSL2561 lum(TSL2561_ADDR_FLOAT); // sensore di luminosità per pallapresa


Servo kick; //attualmente non montato
bool att = true;
/**
 * Led di stato per debug
 * */
byte pinLed[] = {LED_LOOP , LED_PALLA_PRESA, LED_AVANTI, LED_INDIETRO, LED_DX , LED_SX , LED_A, LED_B , LED_C , LED_ORIENTAMENTO};
void led(int n, bool on)
{
   if (n <= 9) // quindi rientra nei pin_led ed. led(0,true);
      digitalWrite(pinLed[n], on);
   else if ( n >= 23 && n <= 41) // define diretto es. led(LED_LOOP,true);
      digitalWrite(n, on);
}
void led_loop()
{
   led(0, att);
   att = !att;
}
void all(bool on)
{
   for (int i = 0; i <= 9; i++)
      digitalWrite(pinLed[i], on);
}
void blink_all_led()
{
   for (int i = 0; i <  3; i++)
   {
      all(true);
      delay(20);
      all(false);
      delay(20);
   }
}


/**
 * per sparare con il kiker, ora non usato
 * */
void kicker()
{
   kick.write(90);
   delay(150);
   kick.write(0);
}
/**
 * legge se la palla è presa
 * */
bool palla_presa()
{
   if (lum.getLuminosity(TSL2561_VISIBLE)  != 0 && lum.getLuminosity(TSL2561_VISIBLE) < lucePallaPresa  )
   {
      led(LED_PALLA_PRESA, true);
      return true;
   }
   else
   {
      led(LED_PALLA_PRESA, false);
      return false;
   }
}
/**
 * ci sono 2 stati, palla vicina o palla lontana. Si capisce confrontando con un valore
 * "sperimentale"
 * */
bool palla_vicina()
{
   if (s.getSegnale_TSOP_MAX() > 580) return false; // palla lontana
   return true;
}


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


#pragma mark - MOVIMENTO

/*
 -------------------------------------
 -------------------------------------
 -------------CERCA-PALLA-------------
 -------------------------------------
 -------------------------------------

 */


void leggi_affidabilita()
{
   s.affidabilita();
   AffidabileLati       =  s.Affidabile_dxsx;
   AffidabileLunghezza  =  s.Affidabile_sugiu;
   led(2, AffidabileLati);
   led(3, AffidabileLunghezza);
}
/**
 *
 * CI SONO DIVERSI TIPI DI CERCAPALLA, QUELLO USATO ATTUALMENTE
 * È MOVIMENTO 90 GRADI. TUTTI GLI ALTRI SONO PROVE PRECEDENTI
 *
 * */


/**
 * angoli a cui il robot deve andare quando ha come tsop che prende di più l'i-esimo
 * */
int angoliTSOP[TSOP_NUM] = {
   180, -160, -145, -130, -90, 0, 0, 0, 0, 90, 130, 145, 160, 180
}; 

void CercaPalla()
{
   if (s.TSOP_max == -1) { // non c'è segnale
      FermaMotori();
      return;
   }
   if (s.TSOP_max == 5 || s.TSOP_max == 6 || s.TSOP_max == 7 || s.TSOP_max == 8) //palla davanti
   {
      if (palla_presa()) // se ha la palla presa
      {
         m.InPortaAngolando();
         m._delay(500);
      }
      else
      {
         Avanti_con_correzione(100, s.getOrientamento());
         m._delay(100);
      }
   }
   else if (s.TSOP_max == 0) // se è quello dietro a sz
   {
      MuoviAngolo(145);
      m._delay(100);
   }
   else if (s.TSOP_max == 13) //dietro a dx
   {
      MuoviAngolo(-145);
      m._delay(100);
   }
   else if (s.TSOP_max == 1 || s.TSOP_max == 12)
   {
      Indietro_con_correzione(100, s.getOrientamento());
      m._delay(100);
   }
   else
   {
      MuoviAngolo(angoliTSOP[s.TSOP_max]);
      if (s.TSOP_max == 1 || s.TSOP_max == 2 || s.TSOP_max == 11)
         m._delay(100);
   }
}

/**
   quando si scontra davanti tenta di togliersi dalla collisione
*/
void CercaPalla2()
{

   if (s.TSOP_max == -1) {
      FermaMotori();
      return;
   }
   if (s.TSOP_max == 6 || s.TSOP_max == 7)
   {
      if (palla_presa()) // se ha la palla in bocca
      {
         m.InPortaAngolando();
         m._delay(500);
      }
      else
      {
         Avanti_con_correzione(100, s.getOrientamento());
         m._delay(100);
      }
   }
   else if (s.TSOP_max == 0)
   {
      MuoviAngolo(160);
      m._delay(100);
   }
   else if (s.TSOP_max == 13)
   {
      MuoviAngolo(-160);
      m._delay(100);
   }
   else if (s.TSOP_max == 1 || s.TSOP_max == 12 || s.TSOP_max == 2 || s.TSOP_max == 11)
   {
      if (s.TSOP_max == 1 || s.TSOP_max == 2) MuoviAngolo(-160);
      else MuoviAngolo(160);
      m._delay(100);
   }
   else if (s.TSOP_max > 2 && s.TSOP_max <= 5)
   {
      MuoviAngolo(-90);
      m._delay(90);
   }
   else if (s.TSOP_max >= 8 && s.TSOP_max < 11)
   {
      MuoviAngolo(100);
      m._delay(100);
   }
   
}

//definisce se ruotare a -30 o +30 per segnare in porta angolando, tramite gli ultrasuoni sx e dx, e il compass
bool gira_sx_per_segnare()
{
   // *cos
   int sx_reale = USSx;
   int dx_reale = USDx;
   int angolo = s.getOrientamento();
   float cos_angolo = cos( DEGREE_TO_RAD( angolo));

   // da verificare
#ifdef DEBUG
   Serial.print("ANGOLO: "); Serial.println(angolo);
   Serial.print("sx letto: "); Serial.print(USSx); Serial.print(" sx stimato: "); Serial.println(USSx * cos_angolo);
   Serial.print("dx letto: "); Serial.print(USDx); Serial.print(" dx stimato: "); Serial.println(USDx * cos_angolo);
#endif
   return (USSx * cos_angolo > USDx * cos_angolo);
}

void CercaPalla3()
{

   if (s.TSOP_max == -1)
   {
      FermaMotori();
      return;
   }
   if ( s.TSOP_max == 6 || s.TSOP_max == 7)
   {
      if (palla_presa()) // se ha la palla in bocca
      {
         Avanti(); // intanto va avanti e basta
         s.readUSDXSX(); // legge solo gli ultrasuoni a destra e a sinistra
         s.readUsSu();

         if (USSu > 10)
         {
            m.giraAdAngoloPerSegnare();

            //legge di nuovo i tsop (Senza condensatori), e se è ancora davanti, lascia un piccolo delay
            s.readAllTsop(false);
            if  (s.TSOP_max == 5 || s.TSOP_max == 6 || s.TSOP_max == 7 || s.TSOP_max == 8)
               m._delay(400);
         }
         else
         {
            if (USSu < 3 ) // ha assolutamente un robot sopra
               m.dribbler();
            else
            {
               m.InPortaAngolando();
               m._delay(200);
            }
         }
      }
      else
      {
         Avanti_con_correzione(100, s.getOrientamento());
         m._delay(130);
      }
   }
   else if (s.TSOP_max == 0)
   {
      MuoviAngolo(160);
      m._delay(100);
   }
   else if (s.TSOP_max == 13)
   {
      MuoviAngolo(-160);
      m._delay(100);
   }
   else if (s.TSOP_max == 1 || s.TSOP_max == 12 || s.TSOP_max == 2 || s.TSOP_max == 11)
   {
      if (s.TSOP_max == 1 || s.TSOP_max == 2) 
         MuoviAngolo(-160);
      else 
         MuoviAngolo(160);
      m._delay(100);
   }
   else if (s.TSOP_max > 2 && s.TSOP_max <= 5)
   {
      MuoviAngolo(-115);
      m._delay(100);
   }
   else if (s.TSOP_max >= 8 && s.TSOP_max < 11)
   {
      MuoviAngolo(115);
      m._delay(100);
   }
}


#define sogliaRotazione 15


//DA CHIAMARE DOPO AVER LETTO GLI ULTRASUONI
/**
 *    @brief verifica in anzi tutto che il robot sia orientato in modo giusto
 *    verifica poi se si può contare sugli ultrasuoni dxsx e sugiu
 *    a seconda di ciò che è più affidabile, fa le relative azioni
 *    @return true se si è posizionato nel punto giusto, ed e' pronto a segnare, false altrimenti
 */
const int ang[TSOP_NUM] = { -150, -90, -60, -45, -25, -10, 0, 0, 10, 25, 45, 60, 90, 150};

/**
* tecnica di movimento "a 90 gradi" rispetto all'angolo in cui si trova la palla.
* ad es: se la palla è esattamente a sx del robot, questo si muoverà esattamente all'indietro
* casi particolari: palla esattamente dietro, o esattamente davanti
*/
void movimento90Gradi()
{
   if (s.TSOP_max == -1)
   {
      FermaMotori();
      return;
   }
   if (palla_vicina()) // se la palla è vicina esegue il movimento 90 gradi
   {
      if (palla_presa() || s.TSOP_max == 6 || s.TSOP_max == 7 )//Avanti();
      {
         if (palla_presa()) // se ha la palla in bocca
         {
            Avanti_con_correzione(100, s.getOrientamento());

            s.readUSDXSX(); // legge solo gli ultrasuoni a destra e a sinistra
            s.readUsSu();

            if (USSu > 10 && USSu < 110)
            {
               /**
                *  nel caso non sia centrato nel campo, gira di 30 o -30 gradi, per cercare di centrare la porta
                * */
               // da verificare gli angoli, ma dovrebbe andare
               m.giraAdAngoloPerSegnare();

               m._delay(100);
               //legge di nuovo i tsop (Senza condensatori), e se è ancora davanti, lascia un piccolo delay
               s.readAllTsop(false);
               if  (s.TSOP_max == 5 || s.TSOP_max == 6 || s.TSOP_max == 7 || s.TSOP_max == 8)
                  m._delay(300);

            }
            else
            {
               if (USSu < 10 ) // ha assolutamente un robot sopra
               {
                  Serial.println(" DRIBBLER");
                  m.dribbler();
                  blink_all_led();
               }
               else
               {
                  m.InPortaAngolando();
                  //MOD 17032016
                  m._delay(500);

               }
            }


         }
         else
         {
            Avanti_con_correzione(100, s.getOrientamento());
            m._delay(200);
         }
      }
      else
         // riga principale della funzione, particolarmente interessante:
         // in particolare ang[] < 0 significa se è a destra o a sinistra al robot
         MuoviAngolo_con_correzione(ang[s.TSOP_max] + 90 * (ang[s.TSOP_max] < 0 ? -1 : 1), s.getOrientamento());
   }
   else // se la palla è lontana si dirige esattamente in direzione della palla
      MuoviAngolo_con_correzione(ang[s.TSOP_max], s.getOrientamento());
   
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
   pinMode(Motor1A, OUTPUT);
   pinMode(Motor1B, OUTPUT);
   pinMode(Motor2A, OUTPUT);
   pinMode(Motor2B, OUTPUT);
   pinMode(Motor3A, OUTPUT);
   pinMode(Motor3B, OUTPUT);
}

unsigned long tpr = 0;

#define tempo 600
void test_fuoricampo()
{
   //                          v false perchè non tenga conto dell'orientamento del robot
   int i = 0;
   while (m.gestisciFuoriCampo(i++) == false);
}
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
}
void setup()
{
   // double time_begin = millis();

   Wire.begin();
   Serial.begin(9600);
#ifdef DEBUG
   Serial.println("partito");
#endif
   s.mov = &m;

   s.init_bussola();
   m.setSensori(s);

   init_motors();

   // TUTTE LE INIZIALIZZAZIONI SONO STATE FATTE ALLA CREAZIONE DI Sensori s.
   compass = 0;
   //setVel(2.55); // <------------- per impostare velocità massima.
   // PER IL RESET DEL COLORE
   pinMode(COL_OUT, INPUT);
   pinMode(COL_RESET, OUTPUT);


   pinMode(Button1, INPUT);
   // M3 M2 M1 ORARIO DA 0

   lum.setGain(TSL2561_GAIN_0X); //**< sensibilita' al minimo, da cambiare se serve
   lum.setTiming(TSL2561_INTEGRATIONTIME_13MS); //**< misurazione più veloce e imprecisa, ma ce la facciamo andar bene

   //intanto andiamo dritti..
   Avanti();
   //Serial.print("tempo per inizializzazioni: ");
   //Serial.println(millis()-time_begin);
   //blink_all_led();
}

void loop()
{
//volendo aggiungere componente random agli ultrasuoni, per quando sono caduti
   led_loop();
#ifdef DEBUG
   unsigned long tempo_iniziale = millis();
#endif

#ifdef DEBUG_SENSORI
   debug_sensori();
   delay(4000);
   return;
#endif

   m.aggiusta_orientamento();
   Serial.println(int(s.readAllTsop()));

   movimento90Gradi();
   m._delay(20); 

#ifdef DEBUG
   Serial.print("tempo LOOP: ");
   Serial.println(millis() - tempo_iniziale);
#endif

}
