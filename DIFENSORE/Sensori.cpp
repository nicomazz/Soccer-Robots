#include "Sensori.h"

 /*sensori& sensori::getIstance()
{
        static sensori istance;
        return istance;
}
*/

// chiamata solo in fase di inizializzazione dal costuttore
void sensori::init()
{
     // costruttore generale
#ifdef OLED_ATTACCATO
    OLED = U8GLIB_SSD1306_128X64(U8G_I2C_OPT_NONE);
#endif
#ifdef LUM_ATTACCATO
    lum = TSL2561(TSL2561_ADDR_LOW);
    lum.setGain(TSL2561_GAIN_0X); //**< sensibilita' al minimo, da cambiare se serve
    lum.setTiming(TSL2561_INTEGRATIONTIME_13MS); //**< misurazione più veloce e imprecisa, ma ce la facciamo andar bene
#endif
    expander = PCF8574(I2C_EXPANDER_ADDRESS);
    Tsop.setExpander(expander);
    
    TSOP_max = TSOPNULL;
}

/*
 SENSORE DI LUMINOSITà
 */
 /*
int sensori::get_lum()
{
  return lum.getLuminosity(TSL2561_VISIBLE);
}
bool sensori::readPalla_Presa()
{
 if (lum.getLuminosity(TSL2561_VISIBLE) <= lucePallaPresa) PallaPresa = true;
  else PallaPresa = false;
  return PallaPresa;
}
*/

// inizializzazione elementi statici
int sensori::Ultrasuoni[4] = {0};
int sensori::n_istanze = 0;
int sensori::compass_xyz[3] = {0};
bool sensori::inizializzato = false;

//costruttore generale
sensori::sensori()
{
  
	n_istanze++;
if (inizializzato == false)
{
  	init();
        inizializzato = true;
}
  //inizializzato = false;
}
void sensori::setLed(short ledN, bool on)
{
  if (ledN >= 0 && ledN <=3)
    expander.write(ledN,on);
}
bool sensori::linea_bianca()
{
   return color.linea_bianca(); 
}
bool sensori::linea_nera()
{
   return color.linea_nera(); 
}
int sensori::getColor1()
{
   return color.read_color1();
}
int sensori::getColor2()
{
   return color.read_color2();
}

//RESTITUISCE L'ORIENTAMENTO ATTUALE
int sensori::getOrientamento()
{
  // legge 10 volte e fa la media
  int somma = 0;
  for (int i = 0; i < 10; i++)
    somma += compass.getOrientation();
   
 return somma/ 10;
    //return compass.getOrientation();
}
void sensori::init_bussola()
{
    compass.init_bussola();
}
/*
 SENSORE DI LUMINOSITà
 */
/*
bool sensori::readPalla_Presa()
{
 if (lum.getLuminosity(TSL2561_VISIBLE) <= lucePallaPresa) PallaPresa = true;
  else PallaPresa = false;
  return PallaPresa;
}*/
//MANDA A VIDEO I VALORI DI TUTTI GLI ULTRASUONI
void sensori::debug_US()
{
    ultrasuoni.debug_US_serial();
}
//LEGGE TUTTI GLI ULTRASUONI, E METTE I LORO VALORI IN Ultrasuoni
void sensori::readUS()
{
    for (int i = 0; i < 4; i++)
        Ultrasuoni[i] = ultrasuoni.getUS(i), delay(100);
}
void sensori::readUSDXSX()
{
    Ultrasuoni[1] = ultrasuoni.getUS(1); delay(20); // prende gli ultrasuoni davanti e dietro
    Ultrasuoni[3] = ultrasuoni.getUS(3);
}
int sensori::readUsSu()
{
   return  Ultrasuoni[2] = ultrasuoni.getUS(2);
}
//LEGGE TUTTI I TSOP, E SALVA IL MASSIMO IN TSOP_max
char sensori::readAllTsop(bool condensatori)
{
    return TSOP_max = Tsop.readTsopAll(condensatori);//= Tsop.readTsopAll_medium(5); // fa la media di 5 letture
    //Tsop.readTsopAll();
}
int sensori::getSegnale_TSOP_MAX()
{
  return Tsop.segnale_Tsop_max;
}
//MANDA I VALORI DEI TSOP A VIDEO
void sensori::debugTsopSerial()
{
    Tsop.debug_TSOP_serial();
}
// METTE NELLE APPOSITE VARIABILI L'AFFIDABILITà DEGLI ULTRASUONI
bool sensori::affidabilita()
{
   
    Affidabile_dxsx  = ultrasuoni.US_affidabile_dx_sx();
    Affidabile_sugiu =  ultrasuoni.US_affidabile_su_giu();
    
    // se tutti e due sono affidabili, salva nella variabile i loro valori
    if (Affidabile_dxsx && Affidabile_sugiu)
        Ultima_misura_us_affidabile = MisuraUsAffidabile(DX,SX,SU,GIU,millis());
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
    if (millis()- Ultima_misura_us_affidabile.millis_misura < kUltima_valida)
    {
        SU = Ultima_misura_us_affidabile.su;
        GIU = Ultima_misura_us_affidabile.giu;
        DX = Ultima_misura_us_affidabile.dx;
        SX = Ultima_misura_us_affidabile.sx;
        
        Affidabile_dxsx = true;
        Affidabile_sugiu = true;
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
    
    return true;
    
}
int sensori::getTsopN(char n)
{
    if (n < 0 || n >= TSOP_NUM) return -1;
    return Tsop.getTsopN(n);
}

/*
 BUSSOLA
 */

//costruttore
sensori::bussola::bussola()
{
}
int sensori::bussola::startDir = 0xFFFF;
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
  if(Wire.available() == length)
  {
	  for(uint8_t i = 0; i < length; i++)
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

	Serial.println("Setting measurement mode to continous.");
	Write(ModeRegister, Measurement_Continuous); 

    startDir = 0xFFFF; // serve solo inizialmente per prendere la direzione della porta avversaria
    startDir = getOrientation();
    Serial.println("bussola inizializzata");
}
void sensori::modificaStartDir(int diff)
{
  compass.modifica_start_dir(diff);
}

void sensori::bussola::modifica_start_dir(int diff)
{
  startDir += diff;
  startDir = (startDir+ 360) % 360;
}
//per prendere l'orientamento
int sensori::bussola::getOrientation()
{  
  uint8_t* buffer = Read(DataRegisterBegin, 6);
  int x,y,z;
  x = y = z = 0;
  
 x =  (buffer[0] << 8) | buffer[1];
 z =  (buffer[2] << 8) | buffer[3];
 y =  (buffer[4] << 8) | buffer[5];
 

  compass_xyz[0] = x;
  compass_xyz[1] = y;
  compass_xyz[2] = z;

  
  int dir = 0;
    
   /* y>0) heading = 90 - atan2(x,y)*180 / pi
     (y<0) heading = 270 - atan2(x,y)*180 / pi
     (y=0, x<0) heading = 180.0
     (y=0, x>0) heading = 0.0*/
    
    if (y != 0) dir = atan2(x,y)*180.0/ 3.14;		// atan2 = arctg
    else if ( x < 0) dir = 180.0;
    else if (x > 0) dir = 0.0;
            
   // Oled(z,3);

    dir+= 180;
//    return dir;
    // correzioni non si sa bene perchè, ma questo è ciò che ci permette di usarla.
 /*   if (StaTra(-1, dir, 61)) dir *= 1.5;
    else if (StaTra(60, dir, 221)) dir = 90 + (dir-60)*0.625;
    else if (StaTra(220, dir, 301)) dir = 180 + (dir-220)*1.125;
    else if (StaTra(300, dir, 360)) dir = 270 + (dir-300)* 1.5;
*/
       
    if (startDir != 0xFFFF) 						//**< inizialmente startDir e' a 0xFFFF
    {								     		  //**< qui ci entrera' sempre, tranne all'inizio
        dir-= startDir;
        if (dir> 360) dir-=360;
        else if (dir < 0) dir+= 360;
        
        if (dir >180) dir-=360; //**< così va negativa se è troppo girato a sx, positivo altrimenti
    }
    return dir ;

}



/**
  COLORE
*/

sensori::colore::colore()
{

  pinMode(COL_1,INPUT);
  pinMode(COL_2,INPUT);
}
bool sensori::colore::linea_bianca()
{
  // perchè sia true deve avere per 100 letture consegutive sotto il colore bianco
  /*  Serial.print("dentro linea bianca: ");
    Serial.print(read_color1()); Serial.print("  "); Serial.println(read_color2());
    if (read_color1() <= 1 || read_color2() <= 1) return true;
    return false;*/
    
     // EVENTUALMETNE PROVARE QUESTO PEZZO
    int volte = 3;
    for (int i = 0; i < 10; i++)
    {
      if (read_color1() <= 3 || read_color2() <= 3) volte--;
      if (volte == 0) return true;
    }
    return false;
    
    
 //  return pulseIn(COL_1,HIGH,800) <= BIANCO || pulseIn(COL_2,HIGH,800)<= BIANCO; 
}
bool sensori::colore::linea_nera()
{
 // for (int i = 0; i < 10; i++)
  //if ( pulseIn(COL_1, HIGH, 800) < NERO &&  pulseIn(COL_2, HIGH, 800) < NERO) return false;
  // return true;
   return pulseIn(COL_1,HIGH,800) > NERO || pulseIn(COL_2,HIGH,800) > NERO; 
}
int sensori::colore::read_color1()
{
   return pulseIn(COL_1,HIGH,800);
}
int sensori::colore::read_color2()
{
   return pulseIn(COL_2,HIGH,800);
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
        pinMode(usPIN[i],OUTPUT),  digitalWrite( usPIN[i], LOW );
}
// prendela distanza che da l'ultrasuoni i E [0,3] in centimetri
int sensori::US::getUS(int N)
{
    digitalWrite( usPIN[N], HIGH );						                                                // invia impulso di 10 microsecondi
    delayMicroseconds( 10 );
    digitalWrite( usPIN[N], LOW );
    
    pinMode(usPIN[N], INPUT); 								                                                // cambia il pin su input, per ricevere il ritardo
    
    long duration = pulseIn( usPIN[N], HIGH,10000 );
    
    pinMode(usPIN[N], OUTPUT); 							                                                //pensiamo al futuro
    int distance = float(0.034) * duration / 2;
    
    return distance; 											                                                // distanza in centimetri
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
 * @brief se gli US posti a destra e sinistra hanno una misurazione complesssiva pari alla relativa larghezza del campo
 * ritorna true, altrimenti false
 */
///TODO ATTENZIONE: FORSE SONO DA METTERE APPOSTO QUESTE DUE PARTI
bool sensori::US::US_affidabile_dx_sx()
{
    int dxsx = sUSDx+sUSSx+DistanzaUsDxSx;
  //  Serial.print("dxsx ");
  //  Serial.println(dxsx);
    if (FuzzyEgual(dxsx,LarghezzaCampo,ErroreMaxUs)) return true; // non ha robot in mezzo a rompere le palle
    return false;
}
/**
 * @brief se gli US posti in alto e in basso hanno una misurazione complesssiva pari alla relativa lunghezza del campo
 * ritorna true, altrimenti false
 */
bool sensori::US::US_affidabile_su_giu()
{
    int SuGiu = sUSSu+sUSGiu+DistanzaUsSuGiu;
    // da fare che se usdx,sx dicono che siamo nel raggio delle porte, controlli la distanza porte
    //Serial.print("su gi ");
   // Serial.println(SuGiu);
    if (FuzzyEgual(SuGiu,LunghezzaCampo,ErroreMaxUs) ||
        FuzzyEgual(SuGiu,DistanzaPorte,ErroreMaxUs)) return true;
    return false;
}


/*
 TSOP
 */

void sensori::TSOP::setExpander(PCF8574 _expander)
{
  expander = _expander;
  //setTsop(true);
}
sensori::TSOP::TSOP()
{
  
   // setTsop(true); // usati in analogico
    scaricaCondensatori = 0;
}
//da l'alimentazione o meno ai tsop, granzie al transistor
// true: accesi, false: spenti, sono comandati da un transistor, per far scaricare i condensatori
// ogni 4 cicli. Sperimentalmente così funzionano meglio
void sensori::TSOP::setTsop(bool accesi)
{
//per ora sono sempre accesi
    expander.write(ALIM_TSOP,!accesi);
}
void sensori::setTsopOn()
{
  
  Tsop.setTsop(true);
}
// restutuisce l valore letto del tsop N
/**
 * @brief ritorna l'indice del tsop che ha segnale maggiore.
 * con la palla in questione (in modalita' pulsata) si ha in uscita dal tsop
 * un onda quadra di duty-cycle proporzionale al segnale.
 * Più c'è segnale è maggiore il duty-cycle
 */
int sensori::TSOP::getTsopN(short n)
{
	//if ( n == 4) return TSOPNULL; // il 4 attualmente non funziona
    set_tsop_MUX(n); // setta il tsop giusto nel multiplexer
    delay(40);
    //setTsop(true);
    int val = analogRead(7); // LEGGE IN A7
    if (n == 6) val-=60;
   /* if (n == 7) val+=50; // toglie segnale
    else if (n == 2) val+=30;
    else if (n == 5) val-=50; // aggiunge segnale
    else if (n == 6) val-=50;*/
    if (val > 950 || val < 100) val = TSOPNULL;
    return val;
}
void sensori::TSOP::controlla_scarica_condensatori()
{
    if (++scaricaCondensatori > 20)
    {
        scaricaCondensatori = 0;
        setTsop(false);
        delay(30);
        setTsop(true);
        delay(51);
    }
}
// in caso di estrema lentezza si può fare più veloce
void sensori::TSOP::set_tsop_MUX(short n)
{
  // nel mux i tsop sono in ordine inverso, cioè alla porta 0 c'è il tsop 7, porta 1 tsop 6, e così via
  short n_effettivo = 7-n;
  expander.write(mux_a,n_effettivo & 1);
  expander.write(mux_b,n_effettivo & 2);
  expander.write(mux_c,n_effettivo & 4);
}
// legge i valori di tutti i tsop, e salva in  TSOP_max quello che prende più
char sensori::TSOP::readTsopAll(bool condensatori)
{
    if (condensatori ) controlla_scarica_condensatori();
    char Tsop_max = 0;
    int val_minimo 	= 2000;
    char i = 0; 
    int val_att;
    
    short TsopN = 0;
    for(; TsopN < 8; TsopN++) /// define dei pin è decrescente
    {
        val_att = getTsopN(TsopN); // legge il singolo
        if ( val_att < val_minimo)   // verifica qual'è il minimo
        {
            val_minimo = val_att; Tsop_max = TsopN;
        }
    }
    if (val_minimo == 2000) Tsop_max = -1, segnale_Tsop_max = -1; // se nessuno prende decentemente
    else segnale_Tsop_max = val_minimo;
    return Tsop_max;
}
// legge i tsop come sopra, ma fa anche la media, sommando i val tot volte

char sensori::TSOP::readTsopAll_medium(int volte)
{
    //// prova per il tempo
    unsigned long tempo_iniziale = millis();
    ///
    
    
   // controlla_scarica_condensatori();
    
    char i = 97; // numero pin A0//A0;
    
    int tsop_val[TSOP_NUM]; // numero di tsop
    memset(tsop_val, 0, sizeof tsop_val);
    
    int val_att;
    
    // qui legge i tsop volte volte, sommando sempre a tsop_val il valore
    short TsopN 	= 0;
    for (int volta = 0; volta < volte; volta++)
    {
        TsopN = 0;
        for(; i >= 84  ; i--, TsopN++) /// define dei pin è decrescente
        {
            val_att = getTsopN(TsopN);
            if (val_att == TSOPNULL) tsop_val[TsopN] = 16000; // val troppo alto, rimarrà nullo
            else tsop_val[TsopN] += getTsopN(TsopN); // legge il singolo
        }
    }
    
    short Tsop_max = 0, val_min = tsop_val[0];
    
    // prendiamo quello che ha più segnale
    for (short i = 1; i < TSOP_NUM; i++)
        if (tsop_val[i] < val_min)
            Tsop_max = i, val_min = tsop_val[i];
   
    //// prova tempo
    Serial.print("tempo lettura con media: ");
    Serial.println(millis()-tempo_iniziale);
    ////
    
    return Tsop_max;
}

void sensori::TSOP::debug_TSOP_serial()
{
    int i = 0; // numero pin A0//A0;
    for (; i < TSOP_NUM ; i++) 
    {
        Serial.print(getTsopN(i)); 
        Serial.print(" ");
    }	
    Serial.println();
}

#ifdef OLED_ATTACCATO

void sensori::Oled(int n, int pos)
{
  if (pos >= 0 && pos <= OLED_CENTRO)
  {
    n_oled[pos] = n;
    if (pos != OLED_CENTRO) n_oled[OLED_CENTRO] = 0;
  }
}
void sensori::scrivi_oled()
{
   OLED.firstPage();  
  do {
     OLED.setFont(u8g_font_fub20n);//u8g_font_unifont);
   if (n_oled[OLED_CENTRO])
   {
      OLED.setFont(u8g_font_fub49n);//u8g_font_unifont);
      OLED.drawStr( 0, 64,  String(n_oled[OLED_CENTRO]).c_str());
      continue;
   }
   if (n_oled[0])
    OLED.drawStr(44, 20,  String(n_oled[0]).c_str());
   if (n_oled[1])
    OLED.drawStr( 0, 44,  String(n_oled[1]).c_str());
   if (n_oled[2])
      OLED.drawStr( 70, 44, String(n_oled[2]).c_str());
   if (n_oled[3])
      OLED.drawStr( 44, 64,  String(n_oled[3]).c_str());
    
  } while( OLED.nextPage() );
}
#endif
