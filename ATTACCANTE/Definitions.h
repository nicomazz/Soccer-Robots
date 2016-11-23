//#define OLED_ATTACCATO
///// IMPORTANTE DA DEFINIRE

#define default_vel 2.55

#define LarghezzaCampo    182 
#define LunghezzaCampo    240 
#define DistanzaPorte       200 
#define distanzaEsterno     35 

#define OLED_CENTRO 4

#define FuzzyEgual(x,v,th) (v-th <= x && x <= v+th)
#define StaTra(l,x,h) l <= x && x <= h

#define DEGREE_TO_RAD(x) (x* M_PI / 180)
#define RADIANTI_IN_GRADI(r) (r*180/M_PI)

#define INT_MAX 32767

#define BUZZER 12

///// TSOP
#define TSOPNULL 2500

#define TSOP_NUM 14 // numero di tsop usati

#define TSOP_FIRST 54 // pin di A0
#define TSOP_LAST  68


#define ALIM_TSOP_1 43  // pin di alimentazione tsop
#define ALIM_TSOP_2 42


#define VelR 50 // velocità di rotazione di default

//indirizzo bussola
#define CompassAddress  0x1E// (0x3C >> 1)   //0x1E



//Motors PIN Def


#define Motor1A 11
#define Motor1B 10
#define Motor2A 7
#define Motor2B 6
#define Motor3A 9
#define Motor3B 8

//I2C Def
#define _SDA 20
#define _SCL 21


///// US PIN Def

// distanza tra i 2 sensori
#define ErroreMaxUs     30
#define DistanzaUsSuGiu 9
#define DistanzaUsDxSx  20

//PIN
#define US1Pin  2  //DAVANTI
#define US2Pin  3  //DIETRO
#define US3Pin  4  //DESTRA
#define US4Pin  5  //SINISTRA


#define USDx s.Ultrasuoni[1]
#define USSx s.Ultrasuoni[3]
#define USSu s.Ultrasuoni[2]
#define USGiu s.Ultrasuoni[0]


#define sUSDx Ultrasuoni[1]
#define sUSSx Ultrasuoni[3]
#define sUSSu Ultrasuoni[2]
#define sUSGiu Ultrasuoni[0]


#define DX  Ultrasuoni[1]
#define SX  Ultrasuoni[3]
#define SU  Ultrasuoni[2]
#define GIU Ultrasuoni[0]

#define kUltima_valida 1000 // millisecondi in cui l'ultima misurazione valida è considerata tale

#define COL_OUT 16
#define COL_RESET 17

//DEFINE LED

#define LED_LOOP 41
#define LED_PALLA_PRESA 39
#define LED_AVANTI 37
#define LED_INDIETRO 35
#define LED_DX 33
#define LED_SX 31
#define LED_A 29
#define LED_B 27
#define LED_C 25
#define LED_ORIENTAMENTO 23


// da ridefinire
#define DentroCampo(x) (x == VERDE || x == NERO)


///// BUTTON-TRIMMER general pourpose
#define Button1 13




#define lucePallaPresa 30 // valore del sensore di luminosità posto davanti sotto il quale si sà che la palla è presa
/// TODO verificare se parte da dentro il cerchio, o da fuori, per provare a fare lo slaloom dell'avversario (cosa molto difficile)

#define PartenzaDentroCerchio X
#define PartenzaFuoriCerchio X


#define RAD_TO_DEGREE(x) ((x*180) / M_PI)