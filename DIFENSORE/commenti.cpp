// /*
//   
//    QUI CI ANDRANNO TUTTE LE PARTI COMMENTATE, CHE MAGARI DIVENTERANNO UTILI IN FUTURO. NON è NULLA DA CONSIDERARE.
// 
//  
//   
//
//
//  
////MOTORI:
// 
// 
// -------------------------------------
// -------------------------------------
// -------------MOTORS-DEBUG------------
// -------------------------------------
// -------------------------------------
// 
//
//// da commentare in caso non serva:
//int MOTORS[3]; // 3 motori
//int indiceMotori;
//
//
//void writeLCDMotors()
//{
//    lcd.clear();
//    lcd.setCursor(0,0);
//    lcd.print(MOTORS[0]);
//    lcd.print("  ");
//    lcd.setCursor(7,0);
//    lcd.print(MOTORS[1]);
//    lcd.setCursor(0,1);
//    lcd.print(MOTORS[2]);
//    
//}
//void eseguiMotori()
//{
//    M1(MOTORS[0]); M2(MOTORS[1]); M3(MOTORS[2]);
//    delay(2000);
//    FermaMotori();
//}
//
//void DEBUG_motors_Init()
//{
//    pinMode(Button1,INPUT);
//    pinMode(Button2,INPUT);
//    
//}
//void controllaCambiamentiMotori()
//{
//    if ( digitalRead(Button1) == LOW)
//    {
//        indiceMotori++;
//        delay(500);
//    }
//    else if (digitalRead(Button2) == LOW)
//    {
//        eseguiMotori();
//        delay(500);
//    }
//    indiceMotori %= 3;
//    
//    MOTORS[indiceMotori] = analogRead(TRIMMER);
//    MOTORS[indiceMotori] = MOTORS[indiceMotori]/5.12 -100 ;
//}
// 
//     if (AffidabileLati )
//                 {
//                     int tmp = startDir;
//                     if (USDx > USSx) startDir += 330;
//                     else startDir +=30 ;
//                     startDir = startDir % 360;   
//                     aggiusta_orientamento();
//                     startDir = tmp;
//                 }
// 
// 
// 
// 
// movimento associato agli angoli dei tsop. es:
// tsop 0 e' messo dietro, queindi a lui è associato indietro,
// tsop 7 e' davanti, quindi a lui è associato avanti
//
//void (*fmovSemplificato[7]) (void) =
//{
//	Indietro,
//	IndSx,
//	AvSx,
//	Avanti,
//	AvDx,
//	IndDx,
//	Indietro
//};
//void (*fmov[14])(void) = // noi usiamo questa
//{
//    Avanti,
//    
//    IndIndSx,
//    IndSx,
//    IndSxSx,
//    
//    
//    AvSxSx,
//    AvSx,
//    AvAvSx,
//    
//    Avanti,
//    
//    AvAvDx,
//    AvDx,
//    AvDxDx,
//    
//    IndDxDx,
//    
//    IndDx,
//    IndIndDx
//};
///*
//void (*fmovMedio[14])(void) =
//{
//    Indietro,
//    
//    IndDxDx,
//    IndDx,
//    IndIndDx,
//    IndIndSx,
//    IndSxSx,
//    
//    AvSxSx,
//    
//    Avanti,
//
//    AvDxDx,
//
//    
//    IndDxDx,
//    IndIndDx,
//    IndIndSx,
//    IndSx,
//    IndSxSx
//    
//}; //fmov[i]() per utilizzarla
//
//
//
//void Avanti();
//void Indietro();
//void Destra();
//void Sinistra();
//
//void IndIndDx(); 
//void IndDx(); 
//void IndDxDx();
//
//void IndIndSx(); 
//void IndSx(); 
//void IndSxSx();
//
//void AvDxDx();
//void AvDx();
//void AvAvDx();
//
//void AvSxSx();
//void AvSx();
//void AvAvSx();
//
//
// @see CercaPalla();
// in sintesi, per portare la palla davanti al robot.
//
//void (*fmovMedio[14])(void) = // NON CONSIDERARE, (pultroppo)
//{
//    Indietro,
//    
//    IndDxDx,
//    IndDx,
//    IndIndDx,
//    IndIndSx,
//    IndSxSx,
//    
//    AvSxSx,
//    
//    Avanti,
//    
//    AvDxDx,
//    
//    IndDxDx,
//    IndIndDx,
//    IndIndSx,
//    IndSx,
//    IndSxSx
//    
//};
//
//
//void angolazioneVersoPorta()
//{
//  if (!AffidabileLati || !AffidabileLunghezza) return;
//
//  bool destra = (USDx > USSx ? false:true); // deve muoversi a destre? risposta.
//
//  int x;
//  int y;
//
//  x = LarghezzaCampo/2 - min(USDx,USSx);
//  y = USSu; // 1/PC del campo 
//
//  int angolo = atan2(x,y) * 180 / PI;
//
//  CompassVersoPorta = compass - angolo *(destra? 1:-1);
//  angoloPortaTempo = millis();
//  VersoPorta = true;
//
//}
//
//
// if (VersoPorta)
//  {
//    if (millis() - angoloPortaTempo > 2000)
//      VersoPorta = false;
//
//  }
//  
//  if (VersoPorta)
//  {
//          if (CompassVersoPorta < 0) RuotaSx(30);
//          else RuotaDx(30);
//          delay(50);
//          FermaMotori();
//  }
//  else
//  {
// 
//     if (AffidabileLati)
//    {
//        if (USDx < distanzaEsterno) Sinistra();
//        else if (USSx < distanzaEsterno) Destra();
//        delay(100);
//    }
//    
//    if (AffidabileLunghezza)
//    {
//        if (USSu < distanzaEsterno) Indietro();
//        else if (USGiu < distanzaEsterno) Avanti();
//        delay(100);
//    }
//	
// 
// 
// lcd.clear();
// lcd.print(tpVal(0));
// lcd.setCursor(0,1);
// lcd.print(tpNum(0));
// 
// if (PallaVicina(tpVal(0))) fmovMedio[tpNum(0)];
// 
//fmov[tpNum(0)]();
//
//if (PallaVicina(tpNum(0)))
//{
//    if (millis()-
//        }
//        //fmovMedio[tpNum(0)]();
//        if (PallaLontana(tpVal(0)))                                           // se è lontana, si dirige
//        fmov[tpNum(0)]();                                                // automaticamenteino fino a lì
//        else if (PallaTroppoVicina(tpVal(0)) && tpNDiverso(0,5,6,7,8,9))    // se è troppo vicina, ma non è davanti al robot
//        fmov[(tpNum(0)+7)%14]();                                       // va nella direzione opposta
//        else                                                              //alla distanza giusta
//        {
//            if (tpNum(0) == 0)                                          //se è esattamente dietro controlla se è
//            {                                                          //meglio andare per destra o sinistra
//                if (tpNum(1) == 1) fmovMedio[1];
//                else if (tpNum(1) == 13) fmovMedio[13];
//                else Destra();                                      // mal che vada, va a destra, DA DEFINIRE MEGLIO CON GLI ULTRASUONI
//            }
//            else fmovMedio[tpNum(0)]();                           //fmovMedio e' un array di puntatori a funzioni che definiscono cosa
//            // fare nel caso il tsop x abbia il massimo segnale, cosi' si risparmiano molti if, o switch.
//        }
//
// 
// 
// bool correggi_fuori_campo()
//{
// 
// // fatto con filosofia ruotato a dx -, a sx +
// readUS();
//  compass = getCompass();
//  if (!TH(compass,0,20))
//  {
//    aggiusta_orientamento();
//    return false;
//  }
//
//lcd.clear();
//  if (USGiu < distanzaEsterno)
//  {
//    MuoviAngolo(compass);
//    lcd.print("giu");
//  }
//  else if (USDx < distanzaEsterno)
//  {
//    MuoviAngolo(-90+compass);
//    lcd.print("destra");
//  }
//  else if (USSx < distanzaEsterno)
//   {
//    MuoviAngolo(90+compass);
//    lcd.print("sinistra");
//   }
//  else if (USSu < distanzaEsterno)
//  {
//    MuoviAngolo(180+compass);
//    lcd.print("su");
//  }
//  else 
//  {    
//    return false;
//    lcd.clear();
//    lcd.print("ho fallito");
//    compass = getCompass();
//      if (!TH(compass,0,20))
//    {
//      aggiusta_orientamento();
//
//    }
//    Inverti_direzione();
//    
//    delay(500);
//
//    return false;	
//  }
//  delay(400);
//  return true;
//
//}
// 
// bool AffidabileLati = USAffidabiliDXSX();
// bool AffidabileLunghezza  =  USAffidabiliSUGIU();
// if (AffidabileLati && AffidabileLunghezza)
// {
// if (USDx < distanzaEsterno) Sinistra();
// else if (USSx < distanzaEsterno) Destra();
// else if (USSu < distanzaEsterno) Indietro();
// else if (USGiu < distanzaEsterno) Avanti();
// else
// {
// if (USSu*7 < USGiu) { Indietro(); delay(500);}
// else if (USSu > USGiu*5) Avanti();
// else  fmovSemplificato[tpMAX / 2] ();
// }
// }
// else fmovSemplificato[tpMAX / 2] ();
// 
// 
// 
// 
// 
// 
// int A,B,C; // var di supporto
//	
//	if (a == 0) A = 0;
//	else if (a>0) A = -100;
//	else A = 100;
//	
//	if (b == 0) B = 0;
//	else if (b>0) B = -100;
//	else B = 100;
//	
//	if (c == 0) C = 0;
//	else if (c>0) C = -100;
//	else C = 100;
//	
//	M(A,B,C);
// 
// 
// 
// 
// 
// 
// 
// 
// 
// 
// 
// 
// 
// 
// 
// 
// 
// 
// // COLORE
// 
// 
// 
// 
// void VerificaColore() /// versione lenta, verifica tutte le componenti, è abbastanza inutile
// {
// readAllColor();
// int sum = Colore[0]+Colore[1]+Colore[2]+Colore[3];
// 
// if (StaTra(neroL,sum,neroH)) Color1 = LINEA_NERA;
// else if (StaTra(verdeL,sum,verdeH)) Color1 = CAMPO_VERDE;
// else if (StaTra(biancoL,sum,biancoH)) Color1 = LINEA_BIANCA;
// else Color1 = 0;
// }
// //coloriCampo
// // VERSIONE LENTA, CHE NON USIAMO PIù
// /////////// non da modificare
// #define biancoL 16
// #define biancoH 24
// 
// #define verdeL 36
// #define verdeH 45
// 
// #define neroL 150
// #define neroH 500
// 
// 
//
//  @brief setta i pin S2 e S3 in modo tale da settare i sensori di colore per inviare un dato colore. 
// 
//void setColor(int color)
//{
//  digitalWrite(ColorS2Pin, (bitRead(color,2) ? HIGH:LOW)); 			
//  digitalWrite(ColorS3Pin, (bitRead(color,1) ? HIGH:LOW));
//}
//
//void readAllColor()
//{
//  for (int i = RED; i <= GREEN; i++) // da 0 a 3 incluso
//  {
//    setColor(i);
//    Colore[i] = pulseIn(Color1Pin, HIGH, 800);
//  }
//}
// 
// 
// 
// // roba vecchia per il colore
// 
// 
//void delayConFuoriCampo(int mS)
//{
//  unsigned long TEMPO = millis();
//  
//  while (millis() -TfuoriUscita  < 1100 && millis()- TEMPO < mS)
//  {}
//  
//  while (millis() - TEMPO < mS) {
//    FuoriCampo();
//  }
//}
//
//void FuoriCampo()
//{
//  int val = VerificaColoreFast();
//  if (val == -1) return;	
//  else 
//  {
//    lcd.clear();
//    lcd.print("FUORI CAMPO!");
//    lcd.print(0,1);
//    lcd.print(val);
//    TfuoriUscita = millis();
//    Inverti_direzione();
//    delayConFuoriCampo(1000);
//    ///correggi_fuori_campo();
//  }
//}
//
// 
// 
// 
// 
// 
// 
// 
// void aggiusta_orientamento(int vel = VelR)
//{
//  if ( compass > 0) RuotaDx(vel);
//  else if (compass < 0) RuotaSx(vel);
//
//  delay(((abs(compass) > 100) ? float(4): float(8)) * abs(compass)); // calcoli sotto
//
//  
//  if (vel == VelR) // se la velocità è quella di default, quindi non è stato chiamato dal fuori campo
//  {
//		if ( compass < 0) RuotaDx(100);
//		else if (compass > 0) RuotaSx(100);
//		delay(10);
//  }
//  FermaMotori();
// 
// 
// 
// 
// 
// 
// 
// 
// 
// 
//codice per posizionamento in mezzo al campo
//readUS();
//if (Ultrasuoni[2] > Ultrasuoni[3])
//Destra();
//else Sinistra();
//delay(20);
//
//int val = getCompass();
//if (val > 0)  RuotaDx(); // nota che ruotadx e sx è a 5'
//else  RuotaSx();
//
// 
// 
// 
// 
// 
// 
// 
// 
// 
// 
// 
// 
// 
// 
// 
// // MUX
// 
// 
// int out[4] = {MP_A, MP_B, MP_C, MP_D};
// 
// 
// void init_MUX()
// {
// pinMode(MP_A, OUTPUT);
// pinMode(MP_B, OUTPUT);
// pinMode(MP_C, OUTPUT);
// pinMode(MP_D, OUTPUT);
// pinMode(MP_OUT, INPUT);
// 
// 
// }
// 
// 
// void mp_Index(int nTSOP)
// {
// // fa continuamente un and bit a bit per verificare che il bit (i) sia alto
// for (int i = 0; i < 4; i++)
// if (nTSOP & bit(i)) digitalWrite(out[i],HIGH);
// else digitalWrite(out[i],LOW);
// }
// 
// 
//   
// #define MP_A 12
// #define MP_B 11
// #define MP_C 10
// #define MP_D 9
// #define MP_OUT 13
//  
// 
// 
// 
// 
// 
// 
// 
// 
// 
// 
// 
// 
// 
// 
// 
// 
// 
// 
// 
// //TSOP
// 
//int tsopReset = 0;
//void tsopRESET()
//{
//    digitalWrite(TsopDietro,LOW);
//    digitalWrite(TsopDavanti,LOW);
//    
//    delay(500);
//    
//    digitalWrite(TsopDietro,HIGH);
//    digitalWrite(TsopDavanti,HIGH);
//    delay(30);
//}
// 
// 
// 
// 
// // NEL CASO I TSOP AVESSERO FUNZIONATO CON LA LETTURA CON PULSEIN, QUESTA SAREBBE ANDATA DA DIO,
// // RITORNAVA L'ANGOLAZIONE ESATTA DELLA PALLA.
// 
// int angolazionePalla(bool letti = true)
// {
// Serial.println();
// Serial.println("---------------------");
// if (!letti) readTsopAll(); // legge tutti i tsop
// 
// int k = 13;
// int maggiore = tpVal(13);
// while( maggiore == TSOPNULL && k > 0) maggiore = tpVal(--k);
// if (k == 0) return -1;
// 
// 
// // differenza tra il maggiore e il minore, per dare un metodo di confronto agli altri
// int minore = tpVal(0);
// if (minore == TSOPNULL) return -1;
// 
// Serial.print("Magg: ");
// Serial.print(maggiore);
// Serial.print(" Min: ");
// Serial.println(minore);
// float delta = maggiore - minore;
// 
// Serial.print(" DELTA: ");
// Serial.print(delta);
// Serial.println();
// 
// int variazioni[14];
// 
// for (int i = 0; i < 14; i++)
// {
// if (tpRaw(i) != TSOPNULL)
// {
// float tmp =  (float) (tpRaw(i) - minore) / delta *100;
// variazioni[i] =  100 - tmp; // da valori da 0 a 100, a seconda di quanto vicino è la palla, e del segnale totale dei tsop
// }
// else variazioni[i] = 0;
// }
// 
// Serial.println(" VARIAZIONI: ");
// 
// for (int i = 0; i < 14; i++)
// {
// Serial.print(variazioni[i]);
// Serial.print(" ");
// }
// Serial.println();
// 
// 
// int VarSemplificate[7][2];
// 
// Serial.print("SEMPLIFICATO: ");
// VarSemplificate[0][0] = variazioni[0];
// VarSemplificate[0][1] = 0;
// for (int i = 1; i < 7; i++)
// {
// 
// 
// if (variazioni[i] < variazioni[15-i])
// {
// VarSemplificate[i][0] = variazioni[15-i]-variazioni[i];
// VarSemplificate[i][1] = -  25.7 *i;
// }
// else if (variazioni[i] > variazioni[14-i])
// {
// VarSemplificate[i][0] = variazioni[i]-variazioni[15-i];
// VarSemplificate[i][1] =  25.7 *i;
// }
// else
// {
// VarSemplificate[i][0] = 0;
// VarSemplificate[i][1] = 0;
// }
// Serial.print(VarSemplificate[i][0]);
// Serial.print(" ");
// }
// Serial.println();
// 
// int sum = 0,cnt = 0;
// for (int i = 0; i < 8; i++)
// {
// sum += VarSemplificate[i][0] * VarSemplificate[i][1];
// cnt += VarSemplificate[i][0];
// }
// unsigned long somma = 0; int cnt = 0;
// for (int i = 0; i < 14; i++)
// {
// 
// somma = somma + ( variazioni[i] * angoli[i]);
// cnt += variazioni[i];
// }
// Serial.print("variazione: ");
// Serial.print(somma);
// Serial.print("  cnt: ");
// Serial.println(cnt);
// 
// somma /= cnt;
// Serial.print(somma);
// return somma;
// 
// }
// 
// 
// 
// 
// 
// 
// 
// 
// 
//
// 
// 
// 
// 
// 
//
//vecchia per tsop:
//
//
//
//
//CONNETTORE FATTO IN QUESTO MODO:
//3  2  1  0  13  12  11    // cavo chiaro
//4  5  6  7  8   9   10    // cavo scuro
////40..53
//#define firstTSOP 30
//#define lastTSOP 43
//
//in mezzo al cavo grigio c'e' il 7,
//dove ci sono i taglietti sono quelli >7.
//il taglietto deve andare in direzione dell'lcd
//vanno dal 30 al 43
//
//#define FT firstTSOP  < i tsop sono nella porta dietro dell'arduino mega.
//int OrdineTsop[14] = {FT+6,FT+8,FT+10,FT+12,FT+13,FT+11,FT+9,FT+7,FT+5,FT+3,FT+1, FT,FT+2,FT+4};
//
//
//
//
//
//int angoli[14]; // angoli associati ai tsop
//for (int i = firstTSOP; i <= lastTSOP; i++)
//pinMode(i,INPUT);
//
//
//pinMode(TsopDietro, OUTPUT);  // 4 5 6 7 8 9 10
//pinMode(TsopDavanti,OUTPUT);  // 11 12 13 0 1 2 3
//
//digitalWrite(TsopDavanti,HIGH);
//digitalWrite(TsopDietro,HIGH);
//
//for (int i = 0; i < 14; i++)
//{
//    Serial.print(angoli[i]);
//    Serial.print("  ");
//}
//Serial.println();
//
//
// 
// 
// 
//
//
// 
// 
// int getTsopN(int n)
// {
// int val = pulseIn(n,HIGH,80000);
// controlla_errore(n,val);
// if (val == -1 || val == 0) val = TSOPNULL;
// return val;
// }
// 
// 
// //DA NON CONSIDERARE
// void readTsopAllWithMedium(int volte)
// {
// for (int i = 0; i <= 14; i++) {tsop[i]  = tsop[i+28] = 0;  tsop[i+14] = i;}
// 
// for (int v = 0; v < volte; v++)
// for(int i= 0; i<14; i++)
// tsop[i] += getTsopN(OrdineTsop[i]);
// SortN2();
// }
// 
// 
// 
// 
// 
// 
// 
// 
// void sort(int num) // NON CONSIDERARE
// {
// int minim = 0xFFFF;
// int maxim = 0;
// for (int i = 0; i < num; i++)
// if (tsop[i] != -1)
// {
// minim = min(minim,tsop[i]);
// maxim = max(maxim,tsop[i]);
// }
// 
// int dim = maxim-minim+1;
// byte bitMapNum[dim];
// 
// for (int i = 0; i < dim; i++)
// bitMapNum[i] = -1;
// 
// for (int i = 0; i < num; i++)
// bitMapNum[tsop[i]-minim] = i;
// 
// int k = 0;
// for (int i = 0; i < dim; i++) {
// if (bitMapNum[i] != -1)
// {
// tsop[k+14] = bitMapNum[i];
// tsop[k++] = minim+i;
// }
// }
// }
// 
// 
// 
// 
// 
// 
// 
// 
// 
// 
// 
// 
// 
// 
// 
// 
// 
// 
// 
// 
// // SENSORE DI COLORE
// 
//
//
//TSL2561_COMMAND_BIT |
//TSL2561_COMMAND_BIT |
//TSL2561_COMMAND_BIT |
//TSL2561_COMMAND_BIT |
//
//lum.write8( TSL2561_COMMAND_BIT |TSL2561_REGISTER_THRESHHOLDL_LOW,threshholdL_L);
//lum.write8( TSL2561_COMMAND_BIT |TSL2561_REGISTER_THRESHHOLDL_HIGH,threshholdL_H);
//lum.write8( TSL2561_COMMAND_BIT |TSL2561_REGISTER_THRESHHOLDH_LOW,threshholdH_H);
//lum.write8( TSL2561_COMMAND_BIT |TSL2561_REGISTER_THRESHHOLDH_HIGH,threshholdH_L);
//
//
//0x11 -> 5:4 INTR Control Select. This field determines mode of interrupt logic
//3:0 Interrupt persistence. Controls rate of interrupts to the host processor
//
//lum.write8(TSL2561_COMMAND_BIT | TSL2561_REGISTER_INTERRUPT,0x11);
//
//// questo penso non funzioni, ma non avevo idee migliori.
//lum.write8(TSL2561_CLEAR_BIT,1);
//
//
// */
// 
//

