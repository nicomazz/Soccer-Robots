#include "Sensori.h"


int sensori::Ultrasuoni[4] = {0};
//costruttore generale
sensori::sensori()
{
#ifdef DEBUG
   Serial.println("inizializzo Sensori");
#endif
}


/*
    SENSORE DI COLORE
 */

void sensori::reset_col()
{
   digitalWrite(COL_RESET, col_reset_state);
   col_reset_state ^= 1; // lo inverte
}

/*
 BUZZER
 */


void sensori::bip(int ms)
{
   tone(BUZZER, 440, ms);
}

#ifdef OLED_ATTACCATO
//_OLED
/*
  scrive in un array ciò che dopo si dovrà scrivere nell'_OLED.
  si scriverà effettivamente quando verrà chiamata la funzione scrivi__OLED
        POS:
        |--------|
        |   0    |
        |1 4/5 2 |
        |   3    |
        |--------|
*/
void sensori::Oled(int n, int pos)
{
   _OLED.scrivi(n, pos);
}
void sensori::scrivi_oled()
{
   _OLED.display();
}

#endif

/*
 COMPASS
 */

void sensori::init_bussola()
{
   compass.init_bussola();
}
int sensori::getOrientamento()
{
#ifdef DEBUG
   unsigned long tempo_iniziale = millis();
#endif

   int somma = 0;
   for (int i = 0; i < 10; i++)
      somma += compass.getOrientation();
#ifdef DEBUG
   Serial.print("tempo lettura bussola: ");
   Serial.println(millis() - tempo_iniziale);
#endif
   // legge 10 volte e fa la media
   return somma / 10;
   //return compass.getOrientation();
}

/*
 ULTRASUONI
 */

//MANDA A VIDEO I VALORI DI TUTTI GLI ULTRASUONI
void sensori::debug_US()
{
   ultrasuoni.debug_US_serial();
}
//LEGGE TUTTI GLI ULTRASUONI, E METTE I LORO VALORI IN Ultrasuoni
int sensori::readUsSu()
{
   Ultrasuoni[2] = ultrasuoni.getUS(2);
   delay(20);
   return Ultrasuoni[2];
}
void sensori::readUS()
{
#ifdef DEBUG
   unsigned long tempo_iniziale = millis();
#endif

   for (int i = 0; i < 4; i++)
   {
      Ultrasuoni[i] = ultrasuoni.getUS(i);
      if ( Ultrasuoni[i] == 0) bip(100);
      delay(20);
   }

#ifdef DEBUG
   Serial.print("tempo lettura ultrasuoni: ");
   Serial.println(millis() - tempo_iniziale);
#endif
}
void sensori::readUSDXSX()
{
   Ultrasuoni[1] = ultrasuoni.getUS(1); delay(20); // prende gli ultrasuoni davanti e dietro
   Ultrasuoni[3] = ultrasuoni.getUS(3);
}
void sensori::readUS_giusti()
{
   readUS();

   double millis_iniziali = millis();
   //al massimo per 5 secondi
   while (millis() - millis_iniziali < 5000 && (sUSSu + sUSGiu > LunghezzaCampo || sUSSx + sUSDx > LarghezzaCampo))
      readUS();
}
//LEGGE TUTTI I TSOP, E SALVA IL MASSIMO IN TSOP_max
char sensori::readAllTsop(bool condensatori)
{
   return TSOP_max = Tsop.readTsopAll(condensatori);
}
//MANDA I VALORI DEI TSOP A VIDEO ( usato per processing )
void sensori::debugTsopSerial()
{
   Tsop.debug_TSOP_serial();
}

int sensori::getSegnale_TSOP_MAX()
{
   return Tsop.segnale_Tsop_max;
}
// METTE NELLE APPOSITE VARIABILI L'AFFIDABILITà DEGLI ULTRASUONI
bool sensori::affidabilita()
{

   Affidabile_dxsx  = ultrasuoni.US_affidabile_dx_sx();
   Affidabile_sugiu =  ultrasuoni.US_affidabile_su_giu();

   // se tutti e due sono affidabili, salva nella variabile i loro valori
   if (Affidabile_dxsx && Affidabile_sugiu)
      Ultima_misura_us_affidabile = MisuraUsAffidabile(DX, SX, SU, GIU, millis());
   else if (!Affidabile_dxsx && !Affidabile_sugiu)
   {
      ultima_abbastanza_affidabile(); // vede se riesce a metter l'ultima, basandosi sul tempo dell'ultima affidabile
   }
   return (Affidabile_dxsx && Affidabile_sugiu);
}
// volendo si può fare per ogni us
bool sensori::ultima_abbastanza_affidabile()
{
   if (Ultima_misura_us_affidabile.millis_misura == 0) return false; // questa non c'è

   // se è stata registrata entro kUltima_valida ms allora viene considerata valida ( vedi Definitions.h)
   if (millis() - Ultima_misura_us_affidabile.millis_misura < kUltima_valida)
   {
      SU = Ultima_misura_us_affidabile.su;
      GIU = Ultima_misura_us_affidabile.giu;
      DX = Ultima_misura_us_affidabile.dx;
      SX = Ultima_misura_us_affidabile.sx;

      Affidabile_dxsx = true;
      Affidabile_sugiu = true;
#ifdef DEBUG
      Serial.println("l'ultima misura si è rivelata affidabile");
#endif
      return true;
   }
   // se arriva qui, l'ultima misurazione è avvenuta troppo tempo fa

#define kErrore_massimo_percentuale 0.1
#define val_errore_perc(val,errore,val2)  ((val-val*errore) <= val2 && val2 <= (val+val*errore))

   // metodo basato su percentuale di errore
   // se tutte le misurazioni eccedono in abs meno del 10%, allora la misurazione è ancora affidabile
   // da provare a fare ciò singolarmente


   if (! val_errore_perc(Ultima_misura_us_affidabile.su, kErrore_massimo_percentuale, SU)) return false;
   if (! val_errore_perc(Ultima_misura_us_affidabile.giu, kErrore_massimo_percentuale, GIU)) return false;
   if (! val_errore_perc(Ultima_misura_us_affidabile.dx, kErrore_massimo_percentuale, DX)) return false;
   if (! val_errore_perc(Ultima_misura_us_affidabile.sx, kErrore_massimo_percentuale, SX)) return false;

   SU = Ultima_misura_us_affidabile.su;
   GIU = Ultima_misura_us_affidabile.giu;
   DX = Ultima_misura_us_affidabile.dx;
   SX = Ultima_misura_us_affidabile.sx;

   Affidabile_dxsx = true;
   Affidabile_sugiu = true;
#ifdef DEBUG
   Serial.println("l'ultima misura si è rivelata affidabile, dopo un'accurato controllo di percentuale");
#endif

   return true;


}


int sensori::getTsopN(char n)
{
   if (n < 0 || n > 13) return -1;
   return Tsop.getTsopN(TSOP_FIRST + n);
}

/*
 BUSSOLA
 */

//costruttore
sensori::bussola::bussola()
{
   // dev'essere chimato nel setup, ed è qui sotto ( init_bussola)
}
void sensori::bussola::Write(int address, int data)
{
   Wire.beginTransmission(CompassAddress);
   Wire.write(address);
   Wire.write(data);
   Wire.endTransmission();
}

uint8_t* sensori::bussola::Read(int address, int length)
{
   Wire.beginTransmission(CompassAddress);
   Wire.write(address);
   Wire.endTransmission();

   Wire.beginTransmission(CompassAddress);
   Wire.requestFrom(CompassAddress, length);

   uint8_t buffer[length];
   if (Wire.available() == length)
   {
      for (uint8_t i = 0; i < length; i++)
      {
         buffer[i] = Wire.read();
      }
   }
   Wire.endTransmission();

   return buffer;
}
#define ConfigurationRegisterA 0x00
#define ConfigurationRegisterB 0x01
#define ModeRegister 0x02
#define DataRegisterBegin 0x03
#define Measurement_Continuous 0x00
#define Measurement_SingleShot 0x01
#define Measurement_Idle 0x03
void sensori::bussola::init_bussola()
{
   m_Scale = 1;
   Write(ConfigurationRegisterB, 0x01 << 5); // guadagno
#ifdef DEBUG
   Serial.println("Setting measurement mode to continous.");
#endif
   Write(ModeRegister, Measurement_Continuous);

   startDir = 0xFFFF; // serve solo inizialmente per prendere la direzione della porta avversaria
   startDir = getOrientation();
#ifdef DEBUG
   Serial.println("bussola inizializzata");
#endif

}
//per prendere l'orientamento
int sensori::bussola::getOrientation()
{
   uint8_t* buffer = Read(DataRegisterBegin, 6);
   int x, y, z;
   x = y = z = 0;

   x =  (buffer[0] << 8) | buffer[1];
   z =  (buffer[2] << 8) | buffer[3];
   y =  (buffer[4] << 8) | buffer[5];

   x *= m_Scale;
   z *= m_Scale;
   y *= m_Scale;

   int dir = 0;

   if (y != 0) dir = atan2(x, y) * 180.0 / 3.14; // atan2 = arctg
   else if ( x < 0) dir = 180.0;
   else if (x > 0) dir = 0.0;
   dir += 180;

   // correzioni non si sa bene perchè, ma questo è ciò che ci permette di usarla.
   /*    if (StaTra(-1, dir, 61)) dir *= 1.5;
       else if (StaTra(60, dir, 221)) dir = 90 + (dir-60)*0.625;
       else if (StaTra(220, dir, 301)) dir = 180 + (dir-220)*1.125;
       else if (StaTra(300, dir, 360)) dir = 270 + (dir-300)* 1.5;*/

   if (startDir != 0xFFFF)             //**< inizialmente startDir e' a 0xFFFF
   {  //**< qui ci entrera' sempre, tranne all'inizio
      dir -= startDir;
      if (dir > 360) dir -= 360;
      else if (dir < 0) dir += 360;

      if (dir > 180) dir -= 360; //**< così va negativa se è troppo girato a sx, positivo altrimenti
   }
   return dir ;
}

void sensori::modificaStartDir(int diff)
{
   compass.modifica_start_dir(diff);
}

void sensori::bussola::modifica_start_dir(int diff)
{
   startDir += diff;
   startDir = (startDir + 360) % 360;
}


/**
 * ULTRASUONI
 */
//costuttore
sensori::US::US()
{
   usPIN[0]  = US1Pin;
   usPIN[1]  = US2Pin;
   usPIN[2]  = US3Pin;
   usPIN[3]  = US4Pin;

   for (char i = 0; i < 4; i++)
      pinMode(usPIN[i], OUTPUT),  digitalWrite( usPIN[i], LOW );
#ifdef DEBUG
   Serial.println("ultrasuoni inizializzati");
#endif
}
// prendela distanza che da l'ultrasuoni i E [0,3] in centimetri
int sensori::US::getUS(int N)
{
   digitalWrite( usPIN[N], HIGH );                                                           // invia impulso di 10 microsecondi
   delayMicroseconds( 10 );
   digitalWrite( usPIN[N], LOW );

   pinMode(usPIN[N], INPUT);                                                                 // cambia il pin su input, per ricevere il ritardo

   long duration = pulseIn( usPIN[N], HIGH, 15000 ); // così arriva al massimo a misurare 1.7 metri ( 15 ms)

   pinMode(usPIN[N], OUTPUT);                                                              //pensiamo al futuro
   int distance = float(0.034) * duration / 2;
   //250 = 0.0034* x / 2

   return distance;                                                                      // distanza in centimetri
}

void sensori::US::debug_US_serial()
{
   int val;
   for (int i = 0; i < 4; i++) {
      Serial.print(i);
      Serial.print(":  ");
      Serial.print(val = getUS(i));
      if ( val == 0)
      {
         Serial.print("  NON FUNZIONA");
         tone(BUZZER, 440, 30);

         if ( i == 0) Serial.print(" GIU ");
         else if ( i == 1) Serial.print(" DESTRA ");
         else if ( i == 2) Serial.print(" SU ");
         else if ( i == 3) Serial.print(" SINISTRA ");


      }
      Serial.println(" ");
   }
   Serial.println();
   Serial.println();
}


/**
 * @brief se gli US posti a destra e sinistra hanno una misurazione complessiva pari alla relativa larghezza del campo
 * ritorna true, altrimenti false
 */
///TODO ATTENZIONE: FORSE SONO DA METTERE APPOSTO QUESTE DUE PARTI
bool sensori::US::US_affidabile_dx_sx()
{
   int dxsx = sUSDx + sUSSx + DistanzaUsDxSx;
#ifdef DEBUG
   Serial.print("USDX+USSX :");
   Serial.println(dxsx);
#endif
   // qui si potrebbe dire che se è tra la porta e il bordo vada assolutamente indietro
   if (FuzzyEgual(dxsx, LarghezzaCampo, ErroreMaxUs)) return true; // non ha robot in mezzo a rompere le palle
   return false;
}
/**
 * @brief se gli US posti in alto e in basso hanno una misurazione complesssiva pari alla relativa lunghezza del campo
 * ritorna true, altrimenti false
 */
bool sensori::US::US_affidabile_su_giu()
{
   int SuGiu = sUSSu + sUSGiu + DistanzaUsSuGiu;
   // da fare che se usdx,sx dicono che siamo nel raggio delle porte, controlli la distanza porte
#ifdef DEBUG
   Serial.print("USSU+USGIU :");
   Serial.println(SuGiu);
#endif
   if (FuzzyEgual(SuGiu, LunghezzaCampo, ErroreMaxUs) ||
         FuzzyEgual(SuGiu, DistanzaPorte, ErroreMaxUs)) return true;
   return false;
}


/*
 TSOP
 */
sensori::TSOP::TSOP()
{
   pinMode(ALIM_TSOP_1, OUTPUT);
   pinMode(ALIM_TSOP_2, OUTPUT);
   setTsop(true); // usati in analogico
   scaricaCondensatori = 0;
#ifdef DEBUG
   Serial.println("Tsop inizializzati");
#endif
}
//da l'alimentazione o meno ai tsop, granzie al transistor
// true: accesi, false: spenti, sono comandati da un transistor, per far scaricare i condensatori
// ogni 4 cicli. Sperimentalmente così funzionano meglio
void sensori::TSOP::setTsop(bool accesi)
{
   digitalWrite(ALIM_TSOP_1, !accesi); // attivo quando è basso
   digitalWrite(ALIM_TSOP_2, !accesi);
}

// restutuisce l valore letto del tsop N
/**
 * @brief ritorna l'indice del tsop che ha segnale maggiore.
 * con la palla in questione (in modalita' pulsata) si ha in uscita dal tsop
 * un onda quadra di duty-cycle proporzionale al segnale.
 * Più c'è segnale è maggiore il duty-cycle
 */
int sensori::TSOP::getTsopN(int N)
{
   //if (N - TSOP_FIRST  == 12) return TSOPNULL;
   setTsop(true);
   int val = analogRead(N);
   if ( val < 100 || val > 1000) val = TSOPNULL;
   return val;
}
void sensori::TSOP::controlla_scarica_condensatori()
{
   // DOPO SPERIMENTAZIONI, I CONDENSATORI SI SCARICANO DOPO 55 MS
   if (++scaricaCondensatori > 7)
   {
      scaricaCondensatori = 0;
      setTsop(false);
      delay(55);
      //    Delay_fuori_campo(10);
      // m._delay(10);
      setTsop(true);
      delay(55);
      //  m._delay(100); // ATTENZIONE, DA RIMODIFICARE CON DELAY FUORI CAMPO
      //Delay_fuori_campo(100);
   }
}
// legge i valori di tutti i tsop, e salva in  TSOP_max quello che prende più
char sensori::TSOP::readTsopAll(bool condensatori)
{
   if (condensatori) controlla_scarica_condensatori();

   char Tsop_max = 0;
   int val_minimo  = 2000;
   char i = TSOP_FIRST; // numero pin A0//A0;
   int val_att;

   short TsopN = 0;
   for (; i < TSOP_LAST  ; i++, TsopN++)
   {
      val_att = getTsopN(i); // legge il singolo

      if ( val_att < val_minimo )   // verifica qual'è il minimo
      {
         val_minimo = val_att; Tsop_max = TsopN;
      }
   }
   if (val_minimo == 2000) Tsop_max = -1, segnale_Tsop_max = -1; // se nessuno prende decentemente
   else segnale_Tsop_max = val_minimo;
   return Tsop_max;
}


void sensori::TSOP::debug_TSOP_serial()
{
   int i = TSOP_FIRST; // numero pin A0//A0;
   int n = 0;
   // controlla_scarica_condensatori();

   Serial.println("s"); // carattere di start per processing
   int val[TSOP_NUM];
   for (; n < TSOP_NUM ; i++) val[n++] = getTsopN(i); // prima li legge
   for (i = 0; i < TSOP_NUM; i++)
      if (val[i] != TSOPNULL) Serial.println(val[i]); // poi li invia tramite seriale
      else Serial.println(1);


}
