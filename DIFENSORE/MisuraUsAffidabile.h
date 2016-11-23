	

#ifndef MisuraUs
#define MisuraUs

struct MisuraUsAffidabile
    {
        int dx, sx,su,giu;
        int dxsx, sugiu;
        unsigned long millis_misura;
        MisuraUsAffidabile(int _dx = -1,int _sx = -1,int _su = -1, int _giu = -1, unsigned long _millis = 0)
        {
            dx = _dx, sx = _sx;
            su = _su, giu = _giu;
            millis_misura = _millis;
            
            dxsx = dx + sx;
            sugiu = su + giu;
        }
    };
#endif
