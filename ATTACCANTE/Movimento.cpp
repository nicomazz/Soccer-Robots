#include "Movimento.h"




/*
 4----3
 |    |
 |    |
 |I__I|
 1----2

 x   y
 1  0   0
 2  100 0
 3  100 100
 4  0   100
 */

Movimento::Movimento()
{}
void Movimento::setSensori(sensori & sens)
{
    s = sens;
}
void Movimento::controlla_fuori_campo()
{
    if (digitalRead(COL_OUT))
    {
        int volta = 0;
        // cerca di mettere a posto il fuori campo per massimo 15 secondi
        double millis_partenza = millis();
        int i = 0;
        while (millis() - millis_partenza < 15000 && gestisciFuoriCampo(i++) == false);
        FermaMotori();
        s.reset_col(); // notifica che ha ricevuto il messaggio
    }
}
void Movimento::_delay(int ms)
{
    double time = millis();
    while (millis() - time < ms)
        controlla_fuori_campo();
}



#define k_angolo_porta 40 // nel caso medio si troverò a 30 cm dal centro del campo. in caso sia sbagliato, alzare questo valore per avere un angolo più deciso
// attraverso gli ultrasuoni, quando ha già preso la palla, decide se muoversi in diagonale verso destra o sinistra per raggiungere la porta
void Movimento::InPortaAngolando()
{
    s.readUS(); // legge inizialmente gli ultrasuoni
    int dxsx = USSx + DistanzaUsDxSx + USDx; // quanta distanza ha a destra e a sinistra
    if ( dxsx > distanzaEsterno * 2)   // così si è sicuri che non vada fuoricampo
    {
        // cercherà di andare verso la porta con un certo angolo che dipenderà da quanto spazio ha avanti di lui
        // meno distanza ha davanti a lui, e più ampio sarà l'angolo.
        // una volta determinato l'angolo, si deciderà se questo è a destra o sinistra, dipendentemente da dove ha più spazio
        int angolo_per_segnare = RAD_TO_DEGREE( atan2(k_angolo_porta, USSu) ); // in radianti
        angolo_per_segnare *=  USSx < USDx ? 1 : -1;
        MuoviAngolo(angolo_per_segnare);
        //EVENTUALMENTE QUI PROVARE A FARE UNA NUOVA FUNZIONE PER FAR ADNARE IN UN CERTO ANGOLO, DANDO QUANCHE GRADO IN PIù O IN MENO ALLA RUOTA DIETRO, COSì DA FAR GIRARE TUTTO IL ROBOT
    }
}
void Movimento::giraAdAngolo(int angolo)
{
    s.modificaStartDir(angolo);
    Avanti_con_correzione(100, s.getOrientamento());
    s.modificaStartDir(-angolo); // in modo che la posizione iniziale rimanga tranquilla
    // bisogna stare attendi che la correzione dell'orientamento non lo rimetta a posto
}

void Movimento::giraAdAngoloPerSegnare()
{

    int sx_stimato = USSx;//*cos_angolo;
    int dx_stimato = USDx;//*cos_angolo;

    // da verificare
#ifdef DEBUG
    Serial.print("sx letto: "); Serial.print(USSx); Serial.print(" sx stimato: "); Serial.println(USSx * cos_angolo);
    Serial.print("dx letto: "); Serial.print(USDx); Serial.print(" dx stimato: "); Serial.println(USDx * cos_angolo);
#endif

    int dxsx = sx_stimato + dx_stimato + DistanzaUsDxSx;

    if (dxsx < LarghezzaCampo - distanzaEsterno) // c'è qualcosa in mezzo a rompere i coglioni
    {
        // qui fa l'inverso, cioè va nella direzione con minor spazio, se ha distanza > di distanzaEsterno
        if (sx_stimato > dx_stimato)// && dx_stimato > distanzaEsterno)
            giraAdAngolo(-30);
        else if (sx_stimato < dx_stimato)// && sx_stimato > distanzaEsterno)
            giraAdAngolo(30);
        else
        {
            // qui può far ben poco
            Avanti_con_correzione(100, s.getOrientamento());
            Serial.println("NON SO COSA FARE :'( HO TROPPI ROBOT VICINI");
        }

    }
    else // qui va tranquillo
    {
        if (sx_stimato > dx_stimato) // ruota di 30 gradi verso la porta
            giraAdAngolo(30);
        else
            giraAdAngolo(-30);
    }
}
#define tempo_spostamento 500
bool Movimento::gestisciFuoriCampo( int volta )
{
    /// 1. l'orientamento dev'essere assolutamente corretto

    /*   if (volta == 100)
     {
        Serial.println("TRUE PER SFINIMENTO");
       return true;

     }*/
    Ferma_subito_motori();
    orientamentoGiusto_assolutamente();
    Ferma_subito_motori();

    // da qui in poi va preso per scontato che sia nella direzione giusta


    /// 2. lettura ultrasuoni, per verificare quanto siano affidabili le letture
    s.readUS_giusti();

    if (USSu > distanzaEsterno && USGiu  > distanzaEsterno && USSx > distanzaEsterno && USDx > distanzaEsterno)
        return true; // non è effettivamente fuoricampo

    int sugiu   = USSu + USGiu  + DistanzaUsSuGiu; // importante aggiungere la distanza tra i 2 ultrasuoni
    int dxsx    = USDx + USSx   + DistanzaUsDxSx;

#ifdef DEBUG
    Serial.print("sugiu: ");
    Serial.println(sugiu);

    Serial.print("dxsx: ");
    Serial.println(dxsx);

    Serial.print(" su:");
    Serial.println(USSu);

    Serial.print(" su:");
    Serial.println(USSu);
    Serial.println("fuoricampo");
#endif

    /*ATTENZIONE, RAGIONAMENTI STRANI, MA VERIFICATI*/
    // si sa se è fuori di sopra o di sotto, di conseguenza la direzione in cui andare. Una delle due misure sarà minore dell'altra,
    // e dunque maggiore di distanzaEsterno per soddisfare la prima condizione.
    if (sugiu >= distanzaEsterno * 2 && (USSu < distanzaEsterno || USGiu < distanzaEsterno) && (USSu != 0 && USGiu != 0)) // diversi da zero, perchè potrebbero esser stati sradicati via
    {
#ifdef DEBUG
        Serial.println("fuori campo su o giu");
#endif
        //verificare se è incastrato tra la porta è un grande problema, ma ora si risolve
        // 30cm (distanza bordo da linea) + 31 cm ( da linea a bordo porta)
        if (FuzzyEgual(dxsx, 57, 5)) // tra la porta e la linea ci sono 31 cm, 30 cm  tra la linea e il bordo,  diamo 4 cm di th
        {
#ifdef DEBUG
            Serial.println("tra porta e esterno");
#endif
            // qui si è sicuri che siamo su o giù, ma da una parte c'è la porta, che potrebbe trarre in inganno
            //OPZIONE 2: muoversi brutalmente indietro o in avanti, poi riprendere una decisione;
            if (USSu <= distanzaEsterno) Indietro();
            else if (USGiu <= distanzaEsterno) Avanti();

            delay(tempo_spostamento);
            Ferma_subito_motori();
            return false;
        }
        else// non si ha l'influenza della porta, probabilmente non è su o giù
        {
            // eventualmente da togliere da qua


#ifdef DEBUG
            Serial.println("niente influenza porta");
#endif
            //brutalmente, come sempre fatto l'anno scorso
            if (USSu <= distanzaEsterno && USSx <= distanzaEsterno)       MuoviAngolo(135);//  IndDx();
            else if (USSu <= distanzaEsterno && USDx <= distanzaEsterno)   MuoviAngolo(-135);// IndSx();
            // nuovo
            else if (USGiu <= distanzaEsterno && USSx <= distanzaEsterno)  MuoviAngolo(45);//AvDx();
            else if (USGiu <= distanzaEsterno && USDx <= distanzaEsterno) MuoviAngolo(-45);//  AvSx();
            //fine nuovo
            else if (USSu <= distanzaEsterno)   Indietro();
            else if (USGiu <= distanzaEsterno)  Avanti();

            delay(tempo_spostamento);
            Ferma_subito_motori();

            return true;
            //    }
        }
        //decidere dove muoversi
    }
    else if (dxsx > distanzaEsterno * 2)
    {
#ifdef DEBUG
        Serial.println("fuori a dx o sx");
#endif
        // qui si sa perfettamente da che parte andare, senza dubbio
        if (USSx < distanzaEsterno) Destra();
        else if (USDx < distanzaEsterno) Sinistra();
        delay(tempo_spostamento);
        Ferma_subito_motori();

        return true;
    }
    else if (FuzzyEgual(dxsx, 60, 4)) // caso di prima incastrato nella porta , ma non si sa se si è su, giu, quindi ogni mossa potrebbe mandarlo fuori campo
    {
        Serial.println("non so cosa fare");
        // bisogna aspettare fin quando si ha una condizione gestita, quindi ritorna falso, e ripova a far tutto
        return false;
    }
    else // sia USSu che USgiu saranno sicuramente maggiori di distanza esterno, altrimenti sarebbe dovuto entrare prima
    {
#ifdef DEBUG
        Serial.println("caso finale");
#endif
        // qui la situazione è più problematica, non si hanno sufficenti dati per decidere se andare a destra o sinistra,
        // oppure si va avanti o indietro, e si richiama la funzione
        if (USSu > USGiu) Avanti();
        else Indietro();
        delay(tempo_spostamento);
        Ferma_subito_motori();

        return false;
    }
}
void Movimento::aggiusta_orientamento(int vel)
{
    compass = s.getOrientamento();

    if (FuzzyEgual(compass, 0, 15)) return;
    if ( compass > 0) RuotaDx(vel);
    else if (compass < 0) RuotaSx(vel);
    //Serial.print("curvo "); Serial.println(compass);
    delay((abs(compass) > 100 ? float(4) : float(5)) * abs(compass)); // valori ottenuti da calcoli

    if (vel == VelR) // se la velocità è quella di default, quindi non è stato chiamato dal fuori campo
    {
        if ( compass < 0) RuotaDx(100);
        else if (compass > 0) RuotaSx(100);
        delay(10);
    }
    //Ferma_subito_motori();
    FermaMotori();
}
void Movimento::orientamentoGiusto_assolutamente()
{
    compass = s.getOrientamento();

    if (FuzzyEgual(compass, 0, 10)); // se è già dritto, esce subito
    else {
        // dato che ha problemi di loop, mettiamogli un tempo limite di orientamento giusto assolutamente
        long millis_iniziali = millis();
        do {
            aggiusta_orientamento(30);
            compass = s.getOrientamento();      // fino a quando non è nella direzione giusta
        } while (!FuzzyEgual(compass, 0, 10) && millis() - millis_iniziali < 4000); // al massimo per 4 secondi, vedremo se andrà bene
    }
}
/**
 * va alla posizione x,y del campo. si basa comunque sulla somma delle letture degli ultrasuoni
 * */
bool Movimento::vai_a_posizione(int x, int y, bool leggi)
{
    // 1.leggere us
    if (leggi) // potrebbe esser chiamata dpo aver letto gli ultrasuoni, dunque non avrebbe senso rileggerli
        s.readUS();
    //if (!s.affidabilita()) // non c'è nulla di affidabile, non si può, questo sarà forse da cambiare
    //   return false;

    // 2.convertire in posizione (pre: tutto affidabile)

    //2 opzioni:
    //1. basato sulle dimensioni del campo conosciute
    //int target_x = map(x,0,100,0,LarghezzaCampo);
    //int target_y = map(y,0,100,0,LunghezzaCampo);
    //2. basato sulla somma degli ultrasuoni dx e sx
    int target_x = map(x, 0, 100, 0, USSx + USDx);
    int target_y = map(y, 0, 100, 0, USSu + USGiu);


    // 3.reperire posizione robot
    int now_x = USSx + DistanzaUsDxSx / 2;
    int now_y = USGiu + DistanzaUsSuGiu / 2;

    //finchè non è nella pos giusta
    int volte = 0;
    while ( !FuzzyEgual(now_x, target_x, 10) || !FuzzyEgual(now_y, target_y, 10))
    {
        // 4. calcolare l'angolo (GUAI A CHI LO TOCCA, COSì è PERFETTO)
        int diff_x = now_x - target_x; // target_x-now_x;
        int diff_y = now_y - target_y; //target_y-now_y;
        int angolo = RAD_TO_DEGREE( atan2(diff_x, -diff_y)); // era in radianti

        //   MuoviAngolo(-angolo); // FUNZIONANTE sarebbe da aggiungere qualche grado, per maggiore sicurezza
        angolo += 20 * (angolo < 0 ? -1 : 1); // se qualcosa non funzniona, questa è da togliere
        MuoviAngolo(angolo);
        _delay(200);
        if (++volte == 10)
        {
            // ha provato troppe volte, meglio che esca.
            return false;
        }
        s.readUS();
        //  if (!s.affidabilita()) // non c'è nulla di affidabile, non si può, questo sarà forse da cambiare
        //  {    FermaMotori();
        //     return false;}
        now_x = USSx, now_y = USGiu;
    }

    FermaMotori();
    return true;
}

void Movimento::SegnaInPorta()
{
    Avanti_con_correzione(100, s.getOrientamento());
}

bool cambia = false;
void Movimento::dribbler() // se non è affidabile la lunghezza, e usSu da poca distanza
{
    if ( USSx > USDx)
        MuoviAngolo(-85);
    else
        MuoviAngolo(85);

    double millis_iniziali = millis();
    //  cambia = !cambia;
    while (millis() - millis_iniziali < 800 && s.readUsSu() < 10)
    {
        _delay(50);
    }
    Avanti_con_correzione(100, s.getOrientamento());
    _delay(300);
    setVel(default_vel);
}

#define ProporzioneCampo 4
#define PC ProporzioneCampo
#define ritardo 200
// questa è inutile, sarà da fare una cosa più semplice
bool Movimento::aggiusta_pos_per_segnare()
{

    if (!FuzzyEgual(compass, 0, 30)) // bussola letta nel loop
    {
        aggiusta_orientamento();
        return false;
    }

    if (!s.Affidabile_dxsx && !s.Affidabile_sugiu) //non è affidabile nulla, però può andare avanti senza sbattere
    {
        if (USSu > 40) Avanti_con_correzione(100, s.getOrientamento());
        else if (USDx > 40) Destra();
        else if (USSx > 40) Sinistra();

        _delay(ritardo);
        return false;
    }

    bool lati = false;
    bool lunghezza = false;

    if (s.Affidabile_dxsx)
        lati = (abs(USSx - USDx) < ErroreMaxUs ? true : false); // in x va bene?


    if (s.Affidabile_sugiu)
        lunghezza = (abs(USSu * (PC - 1) - USGiu) < ErroreMaxUs ? true : false); // in y va bene?



    //situazione_posizione_lcd(lati,lunghezza,s.Affidabile_dxsx,s.Affidabile_sugiu);

    if (lati && lunghezza) return true;     //**< tutto apposto


    else if (!lati && !lunghezza && (s.Affidabile_sugiu && s.Affidabile_dxsx)) //**< nulla ok, ma tutto affidabile
    {
        vai_a_posizione(50, 80, false); // false in modo tale che possa evitare di leggere gli ultrasuoni, velocizzando il tutto
        // qui si muoverà con la funznioe di movimento alle coordinate della porta
        // SegnaAngolando();
    }
    else if (!lunghezza && s.Affidabile_sugiu && !s.Affidabile_dxsx)
    {
        if (USSu * (PC - 1) > USGiu) Avanti_con_correzione(100, s.getOrientamento());
        else Indietro_con_correzione(100, s.getOrientamento());
        _delay(ritardo);
    }

    else if ((lati && (!lunghezza && s.Affidabile_sugiu)) || (!lunghezza && s.Affidabile_sugiu && !s.Affidabile_dxsx) ) //**< larghezza ok, lunghezza no, ma affidabile, o lunghezza no, larghezza non affidabile
    {
        if (USSu * (PC - 1) > USGiu)  Avanti_con_correzione(100, s.getOrientamento());
        else Indietro_con_correzione(100, s.getOrientamento());
        _delay(ritardo);
    }
    else if ((!lati && s.Affidabile_dxsx) && (!s.Affidabile_sugiu || lunghezza)) // lati non ok, lunghezza non importa
    {
        if (USDx > USSx) Destra();
        else Sinistra();
        _delay(ritardo);
    }
    else if (lunghezza && !s.Affidabile_dxsx) //**< lati non ok, non affidabili, lunghezza ok
    {
        // se non è affidabile quello ai lati, si dirige a destra se ha
        //più distanza, e non va fuori, a sinistra stesso dicendo

        if (USDx > USSx && USDx > 40) Destra();
        else if (USDx < USSx  && USSx > 40) Sinistra();
        _delay(ritardo);

    }

    else {
        // PER STATI NON DEFINITI, ORA NON DOVREBBERO ESSERCI PIù PROBLEMI

    }
    return false;

}
/// DA NON GUARDARE
void Movimento::orienta_PID()
{   /*
     int orientamento = s.getOrientation();
     while (! FuzzyEgual(orientamento, 0, 10))
     {
     aggiusta_orientamento_PID(orientamento);
     orientamento = s.getOrientation();
     }

     FermaMotori();*/
}
#define kP  3.3f
#define kI  430.0f
#define kD  10.0f
// non è assicurata la correttezza della seguente funzione
bool Movimento::aggiusta_orientamento_PID(int orientamento)
{
     // pid per la velocità di rotazione, è una prova attualmente
     double time = millis();
     double interval = time - previousTime;
     int error = orientamento;

     P = error * kP;
     I = I + (((error + previousError)/2) * interval);
     D = ((error - previousError) / interval)*kD;
     // PID = P + (I * kI) + D;
     previousTime = time;
     previousError = error;

     //s.printf("pid rotazione: %d composto da P: %d  I: %d  D:  %d",PID,P,I,D);

     if ( error > 0) RuotaDx(PID);
     else if (error < 0) RuotaSx(PID);
}
