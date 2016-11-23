//#define OLED_ATTACCATO
//#define LUM_ATTACCATO
///// IMPORTANTE DA DEFINIRE
#define LarghezzaCampo    182//200 //130// 182 // 125//scuola130
#define LunghezzaCampo    240//240 //186// 240// 240//scuola186
#define DistanzaPorte       200  // spazio tra una porta e l'altra
#define distanzaEsterno     35 // Distanza tra linee e muro di fuoricampo

#define OLED_CENTRO 4
#define I2C_EXPANDER_ADDRESS 0x38

#define FuzzyEgual(x,v,th) (v-th <= x && x <= v+th)
#define StaTra(l,x,h) l <= x && x <= h

#define GRADI_IN_RADIANTI(g) (g* M_PI / 180)
#define RADIANTI_IN_GRADI(r) (r*180/M_PI)

#define INT_MAX 32767

///// TSOP

#define TSOPNULL 2500



#define TSOP_NUM 8 // numero di tsop usati
#define TSOP_OUT 7 // A7

#define VelR 50 // velocità di rotazione di default

//indirizzo bussola
#define CompassAddress  0x1E// (0x3C >> 1)   //0x1E

//Motors PIN Def

#define Motor1A 5
#define Motor1B 3
#define Motor2A 11
#define Motor2B 10
#define Motor3A 6
#define Motor3B 9

/*
#define Motor1A 11
#define Motor1B 10
#define Motor2A 9
#define Motor2B 6
#define Motor3A 5
#define Motor3B 3
*/
//I2C Def
#define _SDA 20
#define _SCL 21


///// US PIN Def

// distanza tra i 2 sensori
#define ErroreMaxUs     30
#define DistanzaUsSuGiu 10
#define DistanzaUsDxSx  20

//PIN
#define US1Pin  4  //DIETRO GIUSTO
#define US2Pin  7 //AVANTI GIUSTO
#define US3Pin  2  //DESTRA GIUSTO 
#define US4Pin  8  //SINISTRA GIUSTO


#define USDx s.Ultrasuoni[1]
#define USSu s.Ultrasuoni[2]
#define USSx s.Ultrasuoni[3]
#define USGiu s.Ultrasuoni[0]


#define sUSDx Ultrasuoni[1]
#define sUSSx Ultrasuoni[3]
#define sUSSu Ultrasuoni[2]
#define sUSGiu Ultrasuoni[0]


#define DX  Ultrasuoni[1]
#define SX  Ultrasuoni[3]
#define SU  Ultrasuoni[2]
#define GIU Ultrasuoni[0]


//MUX
#define mux_a 7
#define mux_b 6
#define mux_c 5
#define ALIM_TSOP 4


#define kUltima_valida 1000 // millisecondi in cui l'ultima misurazione valida è considerata tale

#define COL_1 A0
#define COL_2 A1

#define BIANCO 4
#define NERO 9
//DEFINE LED

#define LED_LOOP 41
#define LED_A 29
#define LED_B 27
#define LED_C 25


// da ridefinire
#define DentroCampo(x) (x == VERDE || x == NERO)


///// BUTTON-TRIMMER general pourpose
#define Button1 13




#define lucePallaPresa 5 // valore del sensore di luminosità posto davanti sotto il quale si sà che la palla è presa
/// TODO verificare se parte da dentro il cerchio, o da fuori, per provare a fare lo slaloom dell'avversario (cosa molto difficile)

#define PartenzaDentroCerchio X
#define PartenzaFuoriCerchio X


#define RAD_TO_DEGREE(x) ((x*180) / M_PI)
#define DEGREE_TO_RAD(g) ((g* M_PI) / 180)

/**
//INUTILIZZATI ( POTREBBERO SERVIRE IN FUTURO )
#define tpStr(x) tsop[x]        //**<potenza segnale
#define tpNum(x) tsop[x+TSOP_NUM]     //**<indice associato alla potenenza segnale
#define tpRaw(x) tsop[x+TSOP_NUM*2]


*/
