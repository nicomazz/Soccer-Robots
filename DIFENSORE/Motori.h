//#include <Arduino.h>
#include "Definitions.h"

void M1(int vel);
void M2(int vel);
void M3(int vel);

void setVel(float vel);
void Inverti_direzione();
void Ferma_subito_motori();

void Indietro_con_correzione(int vel, int compass);
void Avanti_con_correzione(int vel, int compass); 
void MuoviAngolo_con_correzione(int angolo, int compass);

void prova_motori();


void FermaMotori();
void RuotaDx(int vel);
void RuotaSx(int vel);
void Indietro();

void IndIndDx();
void IndDx();
void IndDxDx();

void Destra();

void AvDxDx();
void AvDx();
void AvAvDx();

void Avanti();

void AvAvSx();
void AvSx();
void AvSxSx();

void Sinistra();

void IndSxSx();
void IndSx();
void IndIndSx();



void MuoviAngolo(int angolo);

