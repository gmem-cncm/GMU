
/************************************************************************
 *
 *			                >>>>>>> liveGranUL2~ <<<<<<<
 *
 *						  multi-buffer enveloppe externe,
 *                          multi-buffer son externe.
 *              continuite des grains lors d'un changement de buffer.
 *                         controle en float et/ou en audio
 * 					selection de buffer son par entree signal
 *                           ----------------------
 *                              GMEM 2002-2004
 *         Laurent Pottier / Loic Kessous / Charles Bascou / Leopold Frey
 *
 * -----------------------------------------------------------------------
 *
 *
 * N.B.
 *      * Pour compatibilite maximum Mac/PC :
 *        pas d'accents dans les commentaires svp
 *
 *      * Faites des commentaires !
 *
 ************************************************************************/

/*** Header file ***/

#ifndef __LIVEGRANUL2_H__
#define __LIVEGRANUL2_H__

#include "ext.h"
#include "z_dsp.h"
#include "buffer.h"
#include <string.h>
#include <math.h>
#define NVOICES 4096 // nombre maximum de voix
#define NSBUF 64	// nombre maximum de buffers son
#define NEBUF 64	// nombre maximum de buffers enveloppe
#define MIN_LENGTH 0.1 // longueur minimum des grains en ms  // DISABLED

#define DEF_BUF_LENGTH 44100 // longueur du buffer son ...

// Le mode perf_debug cree une 3-5-7-9eme sortie a l'objet
// Cette sortie trace ses etats utiles (si [poll 1])
//
//			* voices
//			* enveloppe active
//          * buffer son actif
//			* longueur du buffer son actif
//          * noms des buffers son et enveloppe actif (pour ne pas s'y perdre)
//			* nom du fichier dans le buffer actif (idem)
//
#define PERF_DEBUG 1

// MACRO UTILES
#define SAT(a,min,max) (a < min) ? min : ( (a > max) ? max : a )
#define SATADD(a,min,max) (a < min) ? a + min : ( (a > max) ? max : a )
#define MOD(val,mod) val - (mod * floor( (float)val / mod ))

#define MODI(val,mod) val - (mod * ((val/mod) + (val >> 31))) // modulo LONG 32 bits version

#define MIN_LOOP_LENGTH 0.1

#define _SETSYM(ap, x) ((ap)->a_type = A_SYM, (ap)->a_w.w_sym = (x))
#define _SETLONG(ap, x) ((ap)->a_type = A_LONG, (ap)->a_w.w_long = (x))
#define _SETFLOAT(ap, x) ((ap)->a_type = A_FLOAT, (ap)->a_w.w_float = (x))

// INTERPOLATION TRICK DEFINES ///    TODO : EXPLAIN

#define TABLE_BITS 10
#define TABLE_SIZE (1 << TABLE_BITS)

#define LOST_BITS 	6

#define FRAC_BITS (TABLE_BITS + LOST_BITS)
#define FRAC_SIZE (1 << FRAC_BITS)

// INTERPOLATION COEFF STRUCTURE

typedef struct linear_interp
{
    float a;
    float b;
    
} t_linear_interp;

// INTERPOLATION TRICK MACRO

#define interp_index_scale(f) 	((f) * FRAC_SIZE)
#define interp_get_int(i) 	 	((i) >> FRAC_BITS)
#define interp_get_frac(i)		((i) & (FRAC_SIZE - 1))

#define interp_get_table_index(i)	(((i) >> LOST_BITS) & (TABLE_SIZE - 1))

// LOOP LONG INDEX DEFINES

#define LOOP_FRAC_BITS 30
#define LOOP_FRAC_SIZE (unsigned long)(1 <<  LOOP_FRAC_BITS)

#define loop_index_scale(f) (f * (LOOP_FRAC_SIZE))
#define loop_index_sat(i) (i & (LOOP_FRAC_SIZE - 1))
#define loop_index_get_int(i) (i & (1 << LOOP_FRAC_BITS))

// STRUCTURE DE L'OBJET

typedef struct _liveGranul
{
    t_pxobject x_obj;
    
    // BUFFER SON
    
    long	  x_buf_frames;		// le nombre de sample dans le buffer~ son
    double	 *x_buf_samples;	// pointeur sur le tableau d'�chantillon du buffer~ son
    long	x_buf_recpt; 		// pointeur d'ecriture dans le buffer
    
    long	x_buf_recpt_pad;
    
    int x_freeze;
    long x_freeze_pad;
    long x_freeze_pad_curr;
    
    
    // BUFFER ENVELOPPE
    t_buffer *x_env_buf[NEBUF];			// le buffer~ enveloppe
    t_symbol *x_env_sym[NEBUF];			// le symbol correspondant au nom du buffer~ enveloppe
    long	  x_env_frames[NEBUF];		// le nombre de sample dans le buffer~ enveloppe
    float	 *x_env_samples[NEBUF];		// pointeur sur le tableau d'�chantillon du buffer~ enveloppe
    int		  x_nenvbuffer;				// nombre de buffer enveloppe
    int       x_active_env;				// le buffer enveloppe actif est celui dans lequel les nouveaux grains sont pr�lev�s.
    
    // PARAMETRES D'HIVERS
    
    int		  x_nvoices;				// nombre de voix
    int		  x_sinterp;				// type d'interpolation
    long      x_nouts;					// nombre de sorties (2-4-6-8)
    int		  x_nvoices_active;			// nombre de voix actives
    int		  x_loop;					// loop on/off (0 : no loops , 1 : loop entire buffer, 2 : loop between loopstart & end )
    double 	  x_loopstart;
    double 	  x_loopend;
    
    int 	  x_constr_type;			// si 0 length contraite, si 1 delay contrainte
    
    // GRAINS NON-SIGNAL
    int		  x_askfor;					// etat signalant une demande de grain (par un bang)
    // Parametre en mode non-signal
    double     x_begin, x_transpos, x_amp, x_pan, x_length, x_dist, x_teta, x_bufno;
    // Spatialisation : gain par hp
    double     x_hp1, x_hp2, x_hp3, x_hp4, x_hp5, x_hp6, x_hp7, x_hp8;
    
    double     * x_hp;
    
    // GRAINS SIGNAL
    long  *x_ind;				// index de chaque voix
    long  *x_remain_ind;		// nombre d'ech restant a calculer pour chaque grain
    double *x_sind;				// index courant dans le buffer~
    
    // vecteurs des voix (pitch, amp, pan, beg, len...)
    double *Vbeg;
    double *Vtranspos, *Vamp, *Vlength, *Vpan, *Vdist;
    
    
    int   x_env_dir;					// direction de lecture de l'enveloppe
    double *envinc;				// pas d'avancement dans le buffer enveloppe (par rapport � la longueur du grain
    double *envind;				// indice de d�part dans le buffer enveloppe (debut si lecture normale, fin si lecture inversee)
    double *x_env;				// enveloppe de chaque voix
    int   *Venv;				// numero du buffer enveloppe dans lequel sera pris le grain
    
    int   *x_delay;				// delay du declenchement de grain par rapport au vecteur (voir perform)
    int   *x_voiceOn;			// voix active
    
    
    double **Vhp;				// valeur de pan pour chaque voix
    
    // INTERPOLATION COEF TABLE
    
    t_linear_interp * x_linear_interp_table;
    
    // DSP
    double x_sr;						// frequence d'echantillonnage
    // booleens vrais si signal connecte sur l'entree i
    short x_in2con, x_in3con, x_in4con, x_in5con, x_in6con, x_in7con, x_in8con;
    double x_sigin;					// valeur a l'entree de type signal	(voir dsp)
    t_int **w;
    t_float *inlets;
    t_float *outlets;
    
    // DEBUG
#ifdef PERF_DEBUG
    int ask_poll;
    void  * info;
    t_atom  info_list[5];
#endif
    
} t_liveGranul;

//%%%%%%%%%%%%%%%%%% declaration des routines %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//

// Instanciation
void *liveGranul_new(t_symbol *s, short ac, t_atom *av);	// routine de creation d'une instance de l'objet

int liveGranul_alloc(t_liveGranul *x);
int liveGranul_desalloc(t_liveGranul *x);

// Buffer son
void liveGranul_set(t_liveGranul *x,t_symbol *msg, short ac, t_atom * av);		// definition des buffer~ son
//void liveGranul_setInit(t_liveGranul *x, t_symbol *s);	// definition du buffer~ son (� l'initialisation du patch)
void liveGranul_swap(t_liveGranul *x);					// bascule buffer son
int liveGranul_bufferinfos(t_liveGranul *x);				// affectation des param�tres des buffers
//void liveGranul_nbuffer(t_liveGranul *x, long n);		// nombre de buffer son

// Bufffer enveloppe
void liveGranul_setenv(t_liveGranul *x, t_symbol *msg, short ac, t_atom * av);	// definition le buffer~ enveloppe
void liveGranul_envbuffer(t_liveGranul *x, long n);		// # du buffer env actif
void liveGranul_nenvbuffer(t_liveGranul *x, long n);		// nombre de buffer enveloppe

// Init
void liveGranul_init(t_liveGranul *x);					// initialisation des buffers
// Grains non-signal
void liveGranul_bang(t_liveGranul *x);					// routine pour un bang, declenchement d'un grain
void liveGranul_float(t_liveGranul *x, double f);			// routine de recuperation des parametres non-signal

void liveGranul_grain(t_liveGranul *x, t_symbol *s, short ac, t_atom *av);

// Parametres
void liveGranul_sinterp(t_liveGranul *x, long n);		// interpollation dans la lecture du buffer
void liveGranul_poll(t_liveGranul *x, long n);		// nombres de voix en sorties
void liveGranul_clear(t_liveGranul *x);				// panique ! effacement des grains en cours
void liveGranul_nvoices(t_liveGranul *x, long n);			// definition du nombre de voix (polyphonie)
void liveGranul_tellme(t_liveGranul *x);					// demande d'information sur l'etat de l'objet
void liveGranul_freeze(t_liveGranul *x, long n); // stoppe l'enregistrement
void liveGranul_constr(t_liveGranul *x, long n);


// spatialisation
float spat (float x, float d, int n);
float spat2 (float x, float d);
void panner(double * out, int n, double teta, double d);

//void liveGranul_dist(t_liveGranul *x, double f);			// modification de la distance (pour le pan)


void liveGranul_assist(t_liveGranul *x, void *b, long m, long a, char *s);	// assistance info inlet, out1et

// DSP
void liveGranul_free(t_liveGranul *x);										// liberation de la memoire
void liveGranul_dsp64(t_liveGranul *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags);			// signal processing
void liveGranul_perform64(t_liveGranul *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam);


void zero_outs(t_liveGranul *x,double **outs,int n);


// BUFFER
int buffer_record(t_liveGranul *x, double * in,float * out_index, int n);		// ecriture buffer
int bufferenv_check(t_liveGranul *x, int num);	// validite buffer env
int liveGranul_bufferinfos(t_liveGranul *x);				// obtention des infos de buffers


// utilit
float atom2float(t_atom * av, int ind);
t_symbol* atom2symbol(t_atom * av, int ind);

// Debug
#ifdef PERF_DEBUG

void liveGranul_info(t_liveGranul *x,long n);

#endif

#endif


void *liveGranul_class;
t_symbol *ps_buffer;


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//

int C74_EXPORT main(void)
{
    t_class *c = class_new("liveGranul~", (method)liveGranul_new, (method)liveGranul_free, sizeof(t_liveGranul), (method)NULL, A_GIMME, 0L);
    
    class_addmethod(c, (method)liveGranul_setenv, "setenv", A_GIMME, 0);	// definition du buffer~ enveloppe
    class_addmethod(c, (method)liveGranul_envbuffer, "envbuffer", A_LONG, 0);	// # env buffer actif
    
    class_addmethod(c, (method)liveGranul_freeze, "freeze", A_LONG, 0);		// rcord ON
    class_addmethod(c, (method)liveGranul_constr, "constraint", A_LONG, 0);		// contrainte
    class_addmethod(c, (method)liveGranul_nvoices, "nvoices", A_LONG, 0);	// nombre de voix (polyphonie)
    class_addmethod(c, (method)liveGranul_tellme, "tellme",0);				// demande d'info
    class_addmethod(c, (method)liveGranul_bang, "bang", 0);    						// routine pour un bang, declenchement d'un grain
    class_addmethod(c, (method)liveGranul_grain, "grain",    A_GIMME,    0);        // d�clenchement des grains par message
    class_addmethod(c, (method)liveGranul_float, "float", A_FLOAT, 0);							// affectation des valeurs par des flottants
    
    class_addmethod(c, (method)liveGranul_assist, "assist", A_CANT, 0);		// assistance in out
    class_addmethod(c, (method)liveGranul_dsp64, "dsp64", A_CANT, 0);			// signal processing
    class_addmethod(c, (method)liveGranul_sinterp, "sinterp", A_LONG, 0);	// interpollation dans la lecture du buffer pour �viter les clics
    class_addmethod(c, (method)liveGranul_clear, "clear",0);				// panique ! effacement des grains en cours
    class_addmethod(c, (method)liveGranul_clear, "panic",0);				// panique ! effacement des grains en cours
    
#ifdef PERF_DEBUG
    class_addmethod(c, (method)liveGranul_poll, "poll", A_LONG, 0);
    class_addmethod(c, (method)liveGranul_info, "info", A_LONG, 0);
#endif
    
    class_dspinit(c);
    class_register(CLASS_BOX, c);
    liveGranul_class = c;
    ps_buffer = gensym("buffer~");
    
    post("liveGranul~ 64bit / Build CB %s %s",__DATE__,__TIME__);
    
    return 0;
}

// Reception d'un bang, declenche un grain
void liveGranul_bang (t_liveGranul *x)
{
	x->x_askfor = 1;
}


// declenchement grain par liste
// delay(ms) begin detune amp length pan dist buffer envbuffer
void liveGranul_grain(t_liveGranul *x, t_symbol *s, short ac, t_atom *av)
{
	int j,p;
	int nvoices = x->x_nvoices;
	int xn = 0;
	
	if(ac < 8)
	{	
		post("livegranul~ : grain args are <decldelay(ms)> <delay> <detune> <amp> <length> <pan> <dist> <envbuffer>");
		return;
	}
    

    
    long t_mindelay = 0;
    long t_maxdelay = x->x_buf_frames - 1;
    long t_maxlength = 4646000;
    
    
    long begin;
    
	
	double t_offset = atom2float(av,0);
	double t_delay = atom2float(av,1);
	double t_detune = atom2float(av,2);
	double t_amp = atom2float(av,3);
	double t_length = atom2float(av,4);
	double t_pan = atom2float(av,5);
	double t_dist = atom2float(av,6);
    
    double t_length2;
	
	int t_envbuffer = (int)atom2float(av,7);
	
	double srms = x->x_sr*0.001; // double for precision
	
	t_offset = t_offset * srms;
	t_delay = t_delay * srms;
	p = nvoices;
	
	while(--p && x->x_voiceOn[p] ) { }  // avance jusqu a la 1ere voix libre
    
    if(p)
    {
        x->x_voiceOn[p] = 1;
        
        //t_delay = MAX(t_delay,0.);
        
        
        x->Vtranspos[p] = t_detune ;			// valeur de pitch
        x->Vamp[p]		= t_amp;						// amplitude

        x->Venv[p] =  bufferenv_check(x, t_envbuffer );	// enveloppe active pour ce grain
        
        t_length2 = fabs(t_length*srms);
        
        t_mindelay = 0;
        t_maxdelay = x->x_buf_frames-1;
        t_maxlength = 4646000;
        
        if(x->x_constr_type)
        {
            if(!x->x_freeze || x->x_freeze_pad)
            {
                if(x->Vtranspos[p] != 1. )
                {
                    if(x->Vtranspos[p] > 1.)
                        t_mindelay = (x->Vtranspos[p]-1.) * t_length2 - x->x_freeze_pad_curr;
                    else
                        t_maxdelay =  x->x_buf_frames - x->x_freeze_pad_curr + (x->Vtranspos[p]-1.) * t_length2;
                }
            }
            else /// if FREEZE
            {
                if(x->Vtranspos[p] > 0.)
                    t_mindelay = (x->Vtranspos[p]) * t_length2 - x->x_freeze_pad_curr;
                else
                    t_maxdelay =  x->x_buf_frames - x->x_freeze_pad_curr + x->Vtranspos[p] * t_length2;
                
            }
        }else
        {
            if(!x->x_freeze || x->x_freeze_pad)
            {
                if(x->Vtranspos[p] != 1. )
                {
                    if(x->Vtranspos[p] > 1.)
                        t_maxlength =  (t_delay + x->x_freeze_pad_curr) / (x->Vtranspos[p]-1.);
                    else
                        t_maxlength =  -(x->x_buf_frames - t_delay - x->x_freeze_pad_curr) / (x->Vtranspos[p]-1.);
                }else
                    t_maxlength = 4646000; // NOT MORE OK ???
            }
            else /// if FREEZE
            {
                if(x->Vtranspos[p] > 0.)
                    t_maxlength =  (t_delay + x->x_freeze_pad_curr) / (x->Vtranspos[p]);
                else
                    t_maxlength =  -(x->x_buf_frames - t_delay - x->x_freeze_pad_curr) / (x->Vtranspos[p]);
                
            }
        }
        
        
        t_mindelay -= t_offset; // allow negative delay to trig grains in the future present
        
        if(t_length<0)
        {	
            x->Vlength[p]	= t_length2;
            x->Vlength[p]	= MIN(x->Vlength[p],t_maxlength);
            x->envinc[p]	= -1.*(float)(x->x_env_frames[t_envbuffer] - 1.) / x->Vlength[p] ;
            x->envind[p]	= x->x_env_frames[t_envbuffer] - 1;
        }
        else
        {
            x->Vlength[p]	= t_length2;
            x->Vlength[p]	= MIN(x->Vlength[p],t_maxlength);
            x->envinc[p]	= (float)(x->x_env_frames[t_envbuffer] - 1.) / x->Vlength[p] ;
            x->envind[p]	= 0. ;
        }
        
        t_delay = SATADD(t_delay,t_mindelay,t_maxdelay); // constr delay
        begin = x->x_buf_recpt - t_delay-(1-x->x_freeze);//*(t_offset+1);
        x->x_sind[p] = x->Vbeg[p] = MOD(begin, x->x_buf_frames);	// index dans buffer
        //post("grain t_delay sind %f %f   ", t_delay, x->x_sind[p]);
        x->x_ind[p] = 0;
        x->x_remain_ind[p] = (long) x->Vlength[p];
        x->x_delay[p] = t_offset;   // delay de declenchement dans le vecteur  
        
        
        x->Vpan[p]		=   t_pan;						// pan
        x->Vdist[p]		= t_dist;					// distance
        panner(x->Vhp[p],x->x_nouts,x->Vpan[p],x->Vdist[p]);

        
        x->x_ind[p] = 0;
        x->x_remain_ind[p] = (long) x->Vlength[p];
        
    }
    
	
}


// effacement de tous les grains en cours (panique)
void liveGranul_clear (t_liveGranul *x)
{
	int i;
	for (i=0; i< NVOICES; i++) x->x_voiceOn[i] = 0;
}

// voir en sortie le nombre de voix, le buffer son actif etc...
#ifdef PERF_DEBUG
	void liveGranul_poll(t_liveGranul *x, long n)
	{
		x->ask_poll = (n > 0 ? 1 : 0);
	}
	
#endif



float atom2float(t_atom * av, int ind)
{
	switch (av[ind].a_type)
		{
			case A_FLOAT :
				return av[ind].a_w.w_float;
				break;
			case A_LONG :
				return av[ind].a_w.w_long;
				break;
			default :
				return 0.;
				
		}


}

t_symbol * atom2symbol(t_atom * av, int ind)
{
	switch (av[ind].a_type)
		{
			case A_SYM :
				return av[ind].a_w.w_sym;
				break;
			default :
				return 0;
				
		}


}



// Fonction de verification de la validit� du buffer demand�
int bufferenv_check(t_liveGranul *x, int num)
{
	return ( x->x_env_buf[num] == 0 ) ? 0 : num ;
	
}


// Fonction pour spatialiser sur n voix
float spat (float x, float d, int n)
{
	float x1;
	
	x1 = fmod(x, 1.0);	
	x1 = (x1 < 0.5 ? 0.5 - x1 : x1 - 0.5) ;		// x1 = abs(x - 0.5) ;
	x1 = (1 - (x1 * n * d)) ;					// fct lineaire ;
	x1 = (x1 < 0 ? 0 : x1) ;					//  max(0, x1 );
	return pow(x1 , 0.5) * ((d / 2) + 0.5) ;	// (racine de x1) compensee pour la distance;
}

// Fonction pour spatialiser sur 2 voix
float spat2 (float x, float d)
{
	float x1;
	
	x1 = fmod(x, 1.0) ;
	x1 = (x1 < 0.5 ? 0.5 - x1 : x1 - 0.5) ;		// x1 = abs(x - 0.5) ;
	x1 = (1 - (x1 * 2 * d)) ; 					// fct lineaire ;
	x1 = (x1 < 0 ? 0 : x1) ; 					//  max(0, (1 - (x1 * 2)) ;
//	return pow(x1 , 0.5) * ((d / 2) + 0.5) ;	// (racine de x1) compensee pour la distance;
	return pow(x1 * ((d / 2) + 0.5) , 0.5)  ;	// (racine de x1) compensee pour la distance
}

// Panning suivant nombres de hps par voix(min:1  max:16)
//void pannerV(t_bufGranul *x, int voice)
void panner(double * out, int n, double teta, double d)
{
    //int n = x->x_nouts;
    double delta = 1./(double)n ;
    //double d = ((x->Vdist[voice] < 0 ? 0 : x->Vdist[voice]) > 1 ? 1 : x->Vdist[voice]);
    //double teta = (x->Vpan[voice] < 0 ? 0 : x->Vpan[voice]) ;
    
    switch (n)
    {
        case 1 :
            out[0] = 1.;
        case 2 :
            out[0] = spat2( teta + 0.5, d) ;
            out[1] = spat2( teta + 0.0, d) ;
            break;
            
        case 3 :
            out[0] = spat( teta + 0.5, d, n) ;
            out[1] = spat( teta + 0.5 - (1 * delta), d, n) ;
            out[2] = spat( teta + 0.5 + (1 * delta), d, n) ;
            break;
            
        case 4 :
            out[0] = spat( teta + 0.5, d, n) ;
            out[1] = spat( teta + 0.5 - (1 * delta), d, n) ;
            out[2] = spat( teta + 0.5 - (2 * delta), d, n) ;
            out[3] = spat( teta + 0.5 + (1 * delta), d, n) ;
            break;
        case 5 :
            out[0] = spat( teta + 0.5, d, n) ;
            out[1] = spat( teta + 0.5 - (1 * delta), d, n) ;
            out[2] = spat( teta + 0.5 - (2 * delta), d, n) ;
            out[3] = spat( teta + 0.5 + (2 * delta), d, n) ;
            out[4] = spat( teta + 0.5 + (1 * delta), d, n) ;
            break;
        case 6 :
            out[0] = spat( teta + 0.5, d, n) ;
            out[1] = spat( teta + 0.5 - (1 * delta), d, n) ;
            out[2] = spat( teta + 0.5 - (2 * delta), d, n) ;
            out[3] = spat( teta + 0.5 - (3 * delta), d, n) ;
            out[4] = spat( teta + 0.5 + (2 * delta), d, n) ;
            out[5] = spat( teta + 0.5 + (1 * delta), d, n) ;
            break;
        case 7 :
            out[0] = spat( teta + 0.5, d, n) ;
            out[1] = spat( teta + 0.5 - (1 * delta), d, n) ;
            out[2] = spat( teta + 0.5 - (2 * delta), d, n) ;
            out[3] = spat( teta + 0.5 - (3 * delta), d, n) ;
            out[4] = spat( teta + 0.5 + (3 * delta), d, n) ;
            out[5] = spat( teta + 0.5 + (2 * delta), d, n) ;
            out[6] = spat( teta + 0.5 + (1 * delta), d, n) ;
            break;
        case 8 :
            out[0] = spat( teta + 0.5, d, n) ;
            out[1] = spat( teta + 0.5 - (1 * delta), d, n) ;
            out[2] = spat( teta + 0.5 - (2 * delta), d, n) ;
            out[3] = spat( teta + 0.5 - (3 * delta), d, n) ;
            out[4] = spat( teta + 0.5 - (4 * delta), d, n) ;
            out[5] = spat( teta + 0.5 + (3 * delta), d, n) ;
            out[6] = spat( teta + 0.5 + (2 * delta), d, n) ;
            out[7] = spat( teta + 0.5 + (1 * delta), d, n) ;
            break;
        case 9 :
            out[0] = spat( teta + 0.5, d, n) ;
            out[1] = spat( teta + 0.5 - (1 * delta), d, n) ;
            out[2] = spat( teta + 0.5 - (2 * delta), d, n) ;
            out[3] = spat( teta + 0.5 - (3 * delta), d, n) ;
            out[4] = spat( teta + 0.5 - (4 * delta), d, n) ;
            out[5] = spat( teta + 0.5 + (4 * delta), d, n) ;
            out[6] = spat( teta + 0.5 + (3 * delta), d, n) ;
            out[7] = spat( teta + 0.5 + (2 * delta), d, n) ;
            out[8] = spat( teta + 0.5 + (1 * delta), d, n) ;
            break;
        case 10 :
            out[0] = spat( teta + 0.5, d, n) ;
            out[1] = spat( teta + 0.5 - (1 * delta), d, n) ;
            out[2] = spat( teta + 0.5 - (2 * delta), d, n) ;
            out[3] = spat( teta + 0.5 - (3 * delta), d, n) ;
            out[4] = spat( teta + 0.5 - (4 * delta), d, n) ;
            out[5] = spat( teta + 0.5 - (5 * delta), d, n) ;
            out[6] = spat( teta + 0.5 + (4 * delta), d, n) ;
            out[7] = spat( teta + 0.5 + (3 * delta), d, n) ;
            out[8] = spat( teta + 0.5 + (2 * delta), d, n) ;
            out[9]  = spat( teta + 0.5 + (1 * delta), d, n) ;
            break;
        case 11 :
            out[0] = spat( teta + 0.5, d, n) ;
            out[1] = spat( teta + 0.5 - (1 * delta), d, n) ;
            out[2] = spat( teta + 0.5 - (2 * delta), d, n) ;
            out[3] = spat( teta + 0.5 - (3 * delta), d, n) ;
            out[4] = spat( teta + 0.5 - (4 * delta), d, n) ;
            out[5] = spat( teta + 0.5 - (5 * delta), d, n) ;
            out[6] = spat( teta + 0.5 + (5 * delta), d, n) ;
            out[7] = spat( teta + 0.5 + (4 * delta), d, n) ;
            out[8] = spat( teta + 0.5 + (3 * delta), d, n) ;
            out[9]  = spat( teta + 0.5 + (2 * delta), d, n) ;
            out[10] = spat( teta + 0.5 + (1 * delta), d, n) ;
            break;
        case 12 :
            out[0] = spat( teta + 0.5, d, n) ;
            out[1] = spat( teta + 0.5 - (1 * delta), d, n) ;
            out[2] = spat( teta + 0.5 - (2 * delta), d, n) ;
            out[3] = spat( teta + 0.5 - (3 * delta), d, n) ;
            out[4] = spat( teta + 0.5 - (4 * delta), d, n) ;
            out[5] = spat( teta + 0.5 - (5 * delta), d, n) ;
            out[6] = spat( teta + 0.5 - (6 * delta), d, n) ;
            out[7] = spat( teta + 0.5 + (5 * delta), d, n) ;
            out[8] = spat( teta + 0.5 + (4 * delta), d, n) ;
            out[9]  = spat( teta + 0.5 + (3 * delta), d, n) ;
            out[10] = spat( teta + 0.5 + (2 * delta), d, n) ;
            out[11] = spat( teta + 0.5 + (1 * delta), d, n) ;
            break;
            
        case 13 :
            out[0] = spat( teta + 0.5, d, n) ;
            out[1] = spat( teta + 0.5 - (1 * delta), d, n) ;
            out[2] = spat( teta + 0.5 - (2 * delta), d, n) ;
            out[3] = spat( teta + 0.5 - (3 * delta), d, n) ;
            out[4] = spat( teta + 0.5 - (4 * delta), d, n) ;
            out[5] = spat( teta + 0.5 - (5 * delta), d, n) ;
            out[6] = spat( teta + 0.5 - (6 * delta), d, n) ;
            out[7] = spat( teta + 0.5 + (6 * delta), d, n) ;
            out[8] = spat( teta + 0.5 + (5 * delta), d, n) ;
            out[9]  = spat( teta + 0.5 + (4 * delta), d, n) ;
            out[10] = spat( teta + 0.5 + (3 * delta), d, n) ;
            out[11] = spat( teta + 0.5 + (2 * delta), d, n) ;
            out[12] = spat( teta + 0.5 + (1 * delta), d, n) ;
            break;
            
        case 14 :
            out[0] = spat( teta + 0.5, d, n) ;
            out[1] = spat( teta + 0.5 - (1 * delta), d, n) ;
            out[2] = spat( teta + 0.5 - (2 * delta), d, n) ;
            out[3] = spat( teta + 0.5 - (3 * delta), d, n) ;
            out[4] = spat( teta + 0.5 - (4 * delta), d, n) ;
            out[5] = spat( teta + 0.5 - (5 * delta), d, n) ;
            out[6] = spat( teta + 0.5 - (6 * delta), d, n) ;
            out[7] = spat( teta + 0.5 - (7 * delta), d, n) ;
            out[8] = spat( teta + 0.5 + (6 * delta), d, n) ;
            out[9]  = spat( teta + 0.5 + (5 * delta), d, n) ;
            out[10] = spat( teta + 0.5 + (4 * delta), d, n) ;
            out[11] = spat( teta + 0.5 + (3 * delta), d, n) ;
            out[12] = spat( teta + 0.5 + (2 * delta), d, n) ;
            out[13] = spat( teta + 0.5 + (1 * delta), d, n) ;
            break;
            
            
        case 15 :
            out[0] = spat( teta + 0.5, d, n) ;
            out[1] = spat( teta + 0.5 - (1 * delta), d, n) ;
            out[2] = spat( teta + 0.5 - (2 * delta), d, n) ;
            out[3] = spat( teta + 0.5 - (3 * delta), d, n) ;
            out[4] = spat( teta + 0.5 - (4 * delta), d, n) ;
            out[5] = spat( teta + 0.5 - (5 * delta), d, n) ;
            out[6] = spat( teta + 0.5 - (6 * delta), d, n) ;
            out[7] = spat( teta + 0.5 - (7 * delta), d, n) ;
            out[8] = spat( teta + 0.5 + (7 * delta), d, n) ;
            out[9]  = spat( teta + 0.5 + (6 * delta), d, n) ;
            out[10] = spat( teta + 0.5 + (5 * delta), d, n) ;
            out[11] = spat( teta + 0.5 + (4 * delta), d, n) ;
            out[12] = spat( teta + 0.5 + (3 * delta), d, n) ;
            out[13] = spat( teta + 0.5 + (2 * delta), d, n) ;
            out[14] = spat( teta + 0.5 + (1 * delta), d, n) ;
            break;
            
        case 16 :
            out[0] = spat( teta + 0.5, d, n) ;
            out[1] = spat( teta + 0.5 - (1 * delta), d, n) ;
            out[2] = spat( teta + 0.5 - (2 * delta), d, n) ;
            out[3] = spat( teta + 0.5 - (3 * delta), d, n) ;
            out[4] = spat( teta + 0.5 - (4 * delta), d, n) ;
            out[5] = spat( teta + 0.5 - (5 * delta), d, n) ;
            out[6] = spat( teta + 0.5 - (6 * delta), d, n) ;
            out[7] = spat( teta + 0.5 - (7 * delta), d, n) ;
            out[8] = spat( teta + 0.5 - (8 * delta), d, n) ;
            out[9] = spat( teta + 0.5 + (7 * delta), d, n) ;
            out[10] = spat( teta + 0.5 + (6 * delta), d, n) ;
            out[11] = spat( teta + 0.5 + (5 * delta), d, n) ;
            out[12] = spat( teta + 0.5 + (4 * delta), d, n) ;
            out[13] = spat( teta + 0.5 + (3 * delta), d, n) ;
            out[14] = spat( teta + 0.5 + (2 * delta), d, n) ;
            out[15] = spat( teta + 0.5 + (1 * delta), d, n) ;
            break;
            
    }
    
}


//// SPAT PERFORMS


// zero the outputs
void zero_outs(t_liveGranul *x,double **outs,int n)
{
	int i;
	for(i=0;i<x->x_nouts;i++)
	memset(outs[i],0,n*sizeof(double));
}




// Reception des valeurs sur les entrees non-signal
void liveGranul_float(t_liveGranul *x, double f)
{
	int z_in = x->x_obj.z_in - 1;
	if (z_in == 0)
		post("a float in first inlet don't do anything");
	else if (z_in == 1)
		x->x_begin = (f>0.) ? f : 0. ;
	else if (z_in == 2)
		x->x_transpos = f;
	else if (z_in == 3)
		x->x_amp = (f>0.) ? f : 0. ;
	else if (z_in == 4){	 if(f<0)
									 {
										x->x_length = (-f > MIN_LENGTH)? -f : MIN_LENGTH;
										x->x_env_dir = -1;
									 } else {
										x->x_length= (f > MIN_LENGTH)? f : MIN_LENGTH;
										x->x_env_dir = 1;
									 }
								}
	else if (z_in == 5){	 
									x->x_pan = f;

								}		
	else if (z_in == 6){	 
									x->x_dist = f;

								}
	
}

// limitation de la polyphonie
void liveGranul_nvoices(t_liveGranul *x, long n) 
{ 
	n = (n > 0) ? n : 1 ;
	x->x_nvoices = (n < NVOICES) ? n+1 : NVOICES; 
}

// interpollation dans la lecture du buffer (evite clic, crack et autre pop plop)
void liveGranul_sinterp(t_liveGranul *x, long n)
{ 
	x->x_sinterp = n ? 1 : 0 ; 
}





// Impression de l'etat de l'objet
void liveGranul_tellme(t_liveGranul *x)
{
	int i;
	post("  ");
	post("_______liveGranul~'s State_______"); 

	post("::global settings::");

	post("outputs : %ld",x->x_nouts);
    post("voices : %ld",x->x_nvoices);
    post("buffer length : %ld", x->x_buf_frames);
	post("________________________________");


	post("::envelope buffers::");

    post("-- Active envelope buffer : %ld --",x->x_active_env+1);
	post("-- Number of envelope buffers : %ld --",x->x_nenvbuffer);
	for (i=0; i< x->x_nenvbuffer ; i++)
	{
		if(x->x_env_sym[i]->s_name) post("   envelope buffer~ %ld name : %s",i+1,x->x_env_sym[i]->s_name);
	}
	post("________________________________");
	
	liveGranul_info(x,-1);


}

// Informations en sortie de l'objet
#ifdef PERF_DEBUG
	void   liveGranul_info(t_liveGranul *x, long n)
	{
		// Si n=	0 infos sur buffer son
		//			1 infos sur buffer enveloppe
		//		   -1 infos sur buffers...
		int i;
		
		if(x->ask_poll)
		{
			
			if(n==1 || n==-1)
			{
				_SETSYM( x->info_list, gensym("active_env"));
				_SETFLOAT(x->info_list+1,  x->x_active_env+1 ); 
				outlet_list(x->info,0l,2,x->info_list);
				
				for(i=0; i<x->x_nenvbuffer; i++)
				{
					if(x->x_env_sym[i])
					{
						_SETSYM(x->info_list, gensym("envbuffer"));
						_SETLONG(x->info_list+1, i);
						_SETSYM(x->info_list+2, x->x_env_sym[i]); 
						outlet_list(x->info,0l,3,x->info_list);
					}
				}
			}
		}
}
	

#endif


//NNNNNNN routine de creation de l'objet NNNNNNNNNNNNNNNNNNNNNNNNNNNN//

void *liveGranul_new(t_symbol *s, short ac, t_atom *av)
{   
	
	int i,j; 
	float f;
	int size;
    t_liveGranul *x = (t_liveGranul *)object_alloc((t_class*)liveGranul_class);
    //dsp_setup((t_pxobject *)x,1);  
    
    //x->x_obj.z_misc = Z_NO_INPLACE ;
          
    //creation des entres supplementaires---//

    dsp_setup((t_pxobject *)x,8);
    
    x->x_nouts = 2;	// pour que nb outs = 2 si pas specifie
    x->x_buf_frames = DEF_BUF_LENGTH;
    x->x_sr = sys_getsr();
    
    x->x_buf_frames = 441000;
    
	if(ac < 2)
	{
		post("Missing Arguments Name 1 Envelope Buffer Name 2 Outputs number (1-2-4-6-8)");
		return 0;
	} else {

		//////////////arguments/////////////////
		// note : 1er symbol buffer son , 2eme symbol buffer env, 3eme nombres de sorties


		for (i=0; i < NEBUF; i++)
		{
			x->x_env_buf[i]=0;
    		x->x_env_sym[i]=gensym("empty_buffer");
			x->x_env_samples[i]=0;
			x->x_env_frames[i]=-1;
		}

		for (j=0; j < ac; j++){
    		switch (av[j].a_type){
    		
    			case A_LONG:
    			//	post("argument %ld is a long : %ld, assigned to nb outs", (long)j,av[j].a_w.w_long);
    				x->x_nouts = av[j].a_w.w_long;
    			break;
    			
    			case A_SYM:
    			//	post("argument %ld is a symbol : name %s, assigned to env buffer~ name",(long)j,av[j].a_w.w_sym->s_name);

    					x->x_env_sym[j] = av[j].a_w.w_sym;
    			break;
    			
    			case A_FLOAT:
    					size = av[j].a_w.w_float * x->x_sr / 1000.;
    					x->x_buf_frames = SAT(size , 22050 , 4410000);
    			post("liveGranul~ : set buffer size to %10.1f ms", x->x_buf_frames * 1000. / x->x_sr);
				//post("%f",x->x_buf_frames);	
    			break;
    		}	
		}

		///////////////////////////////
		 // info outlet
		#ifdef PERF_DEBUG
		x->info = outlet_new((t_pxobject *)x, 0L);
		#endif
		
		//creation des sortie signal
        // limit n outs to 16
        x->x_nouts = SAT(x->x_nouts,1,16);
        
        //creation des sortie signal
        for (int hp=0; hp < x->x_nouts;hp++)
            outlet_new((t_pxobject *)x, "signal");
        
		
		// generation de la table de coeff d'interpolation
		x->x_linear_interp_table = (t_linear_interp *) t_getbytes( TABLE_SIZE * sizeof(struct linear_interp) );
		

		for(i=0; i<TABLE_SIZE ; i++)
		{
			f = (float)(i)/TABLE_SIZE; // va de 0 a 1
			
			x->x_linear_interp_table[i].a = 1 - f;
			x->x_linear_interp_table[i].b = f;
		}
		
		
		// allocation buffer son
		
		x->x_buf_samples = (double *) t_getbytes( x->x_buf_frames * sizeof(double) );
		

		//allocations des tableaux
		if( !liveGranul_alloc(x) || ! x->x_linear_interp_table || ! x->x_buf_samples)
		{
			post("error liveGranul~ : not enough memory to instanciate");
		}
		
		
		// initialisation des autres parametres
		x->x_nvoices = 64;
		x->x_askfor = 0;    
		x->x_begin = 0;
		x->x_transpos = 1.0;
		x->x_amp = 1.0;    
		x->x_length = 100;
		
		x->x_buf_recpt = 0;
    
		x->x_sinterp = 1;
		 x->x_loop = 1 ;
		 
		 x->x_freeze = 0;
		 x->x_constr_type = 1;
    
		x->x_pan = 1/(x->x_nouts*2);
		x->x_dist = 1;
		x->x_nvoices_active = 0;
		x->x_env_dir = 1;
        
		for (i=0; i< NVOICES; i++){       
     		x->x_ind[i] = -1;
     		x->x_env[i] = 0;
     		x->x_voiceOn[i] = 0;
		}

		// initialisation des parametres d'activite (modifie par la suite dans setenv et set)
		x->x_nenvbuffer = 1;
		x->x_active_env = 0;
		
		for( i=1 ; i<NSBUF ; i++)
		{
			x->x_env_buf[i] = 0;
			x->x_env_sym[i] = 0;
		}

	

		return (x);
	}

}
//NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN//

//--------assitance information inlet et out1et----------------------//
void liveGranul_assist(t_liveGranul *x, void *b, long m, long a, char *s)
{
	switch(m) {
		case 1: // inlet
			switch(a-1) {
				case -1 : 
				sprintf(s, " Input Signal");		break;			
				case 0:
				sprintf(s, " bang ,set , signal zero X-ing, ...");		break;
				case 1:
				sprintf(s, " delay (float & signal) ");						break;				
				case 2:
				sprintf(s, " transpos (float & signal) ");					break;
				case 3:
				sprintf(s, " amp (float & signal) ");						break;
				case 4:
				sprintf(s, " length (float & signal) ");					break;
				case 5:
				sprintf(s, " pan (float & signal) ");						break;																				
				case 6:
				sprintf(s, " dist (float & signal) ");						break;
				case 7:
				sprintf(s, " sound buffer num (float & signal) ");			break;																				
			}
		break;
		case 2: // outlet
			switch(a) {
				case 0:
				sprintf(s, "(signal) Output 1");				break;				
				case 1:
					if(x->x_nouts > 1) sprintf(s, "(signal) Output 2");
					else sprintf(s, "info out (voices...)");
					break;
				case 2:
					if(x->x_nouts > 2) sprintf(s, "(signal) Output 3");
					else sprintf(s, "info out (voices...)");
					break;
				case 3:
				sprintf(s, "(signal) Output 4");				break;
				case 4:
					if(x->x_nouts > 4) sprintf(s, "(signal) Output 5");
					else sprintf(s, "info out (voices...)");
					break;		
				case 5:
				sprintf(s, "(signal) Output 6");				break;
				case 6:
					if(x->x_nouts > 6) sprintf(s, "(signal) Output 7");
					else sprintf(s, "info out (voices...)");
					break;		
				case 7:
				sprintf(s, "(signal) Output 8");				break;									
				case 8:
				sprintf(s, "info out (voices...)");				break;									
			}
		break;
	}
}

//-----------------------------------------------------------------------------------//





int liveGranul_bufferinfos(t_liveGranul *x)
{
	int cpt;
	t_buffer *b;
	float srms = x->x_sr*0.001;
	
	// retourne 0 : sortie nulle
	// retourne 1 : calcul des grains

	// Test si enabled
    if (x->x_obj.z_disabled) return -1;
    

	// Test sur la validit� et l'activite des buffers & initialisations


	for(cpt = 0; cpt < x->x_nenvbuffer ; cpt++)
	{
		if( x->x_env_sym[cpt] ) // si un nom de buffer a �t� donn� -> collecter les infos
		{
		
            if ((b = (t_buffer *)(x->x_env_sym[cpt]->s_thing)) && ob_sym(b) == ps_buffer && b->b_valid && !b->b_inuse) // attention inuse !!!
            {
                x->x_env_buf[cpt] = b;
                x->x_env_samples[cpt] = b->b_samples;
                x->x_env_frames[cpt] = b->b_frames;
                
                
            }
			else {
				//post("invalid %d",cpt);
				// le buffer 0 est celui utilis� si celui choisi est pas valide ... si il n'y est pas -> pas dsp !!!
				if(cpt == 0) {return 0;}
				x->x_env_buf[cpt] = 0;

			}
		}
	}

	return 1;
}


void liveGranul_freeze(t_liveGranul *x, long n) 
{ 
	if ( n > 0 && !x->x_freeze )
	{
	x->x_freeze = 1; 
	x->x_freeze_pad = DEF_BUF_LENGTH / 4;
	x->x_freeze_pad_curr = 0;
	x->x_buf_recpt_pad = x->x_buf_recpt;
	}else if (x->x_freeze)
	{
		x->x_freeze = 0; 
		x->x_buf_recpt = x->x_buf_recpt_pad;
	}
}

void liveGranul_constr(t_liveGranul *x, long n) 
{
	if(n)
		x->x_constr_type = 1;
	else
		x->x_constr_type = 0;
}

// BUFFER REC

int buffer_record(t_liveGranul *x,double *in, float * out_index, int n)
{
	long rec_pt = x->x_buf_recpt;
	long rec_pt_pad = x->x_buf_recpt_pad;
	double * buf = x->x_buf_samples;
	long frames = x->x_buf_frames;
	
	if(!x->x_freeze)
	{
		while(n--)
		{
			rec_pt = MOD(rec_pt,frames);
			buf[rec_pt++] = *in++;
		}
		rec_pt_pad = rec_pt;
		x->x_freeze_pad_curr = 0;
	}else
	{
		while(n-- && x->x_freeze_pad)
			{
				rec_pt_pad = MOD(rec_pt_pad,frames);
				buf[rec_pt_pad++] = *in++;
				x->x_freeze_pad--;
				x->x_freeze_pad_curr++;
			}
		
	}
	
	x->x_buf_recpt = rec_pt;
	x->x_buf_recpt_pad = rec_pt_pad;
	return 1;
}


// BUFFERS ENVELOPPE

void liveGranul_setenv(t_liveGranul *x, t_symbol *msg, short ac, t_atom * av)
{	
	// Changement du buffer enveloppe
	t_buffer *b;
	int num;
	int tmp;
	t_symbol *s;
// 	post("liveGranul_setenv %s",s->s_name);

	if(ac == 2)
	{
		num = (int) atom2float(av,0);
		if( num < 0 || num >= NEBUF )
		{
			post("liveGranul~ : <#buffer> out of range");
			return;
		}
			
		if( !(s = atom2symbol(av,1)) )
			return;
		
	}else if(ac == 1)
	{
		if( !(s = atom2symbol(av,0)) )
		return;
		num = 0;
		
	}else
	{	
		post("liveGranul~ : error setenv <#buffer> <buffer name> ");
			return;
	}
	
	x->x_env_sym[num] = s;
	if(x->x_nenvbuffer < num + 1) x->x_nenvbuffer = num+1;

	if ((b = (t_buffer *)(s->s_thing)) && ob_sym(b) == ps_buffer && b && b->b_valid)
	{
		x->x_env_buf[num] = b;
		x->x_env_sym[num] = s;
		x->x_env_samples[num] = b->b_samples;
		x->x_env_frames[num] = b->b_frames;
		liveGranul_info(x,1);
	} else {
		error("liveGranul~: no buffer~ %s", s->s_name);
		x->x_env_buf[num] = 0;
	}

}


// affectation du num�ro de buffer enveloppe actif
void liveGranul_envbuffer(t_liveGranul *x, long n)
{
	x->x_active_env = (n < 0)? 0 : ((n >= NEBUF)? NEBUF-1 : n );
	
}


// DSP

void liveGranul_dsp64(t_liveGranul *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags)
{
	x->x_sr = sys_getsr();
	
	

	// signal connected to inlets ?
   	x->x_in2con = count[2] > 0;
	x->x_in3con = count[3] > 0;
	x->x_in4con = count[4] > 0; 
   	x->x_in5con = count[5] > 0;
	x->x_in6con = count[6] > 0;
	x->x_in7con = count[7] > 0;
	x->x_in8con = count[8] > 0;
	



    object_method(dsp64, gensym("dsp_add64"), x, liveGranul_perform64, 0, NULL);

}



void liveGranul_free(t_liveGranul *x)
{
	dsp_free((t_pxobject *) x);
	
	liveGranul_desalloc(x);
	
	
	if(x->x_linear_interp_table)
		t_freebytes(x->x_linear_interp_table, TABLE_SIZE * sizeof(struct linear_interp) );
		
	if(x->x_buf_samples)
		t_freebytes(x->x_buf_samples, x->x_buf_frames * sizeof(double) );


	
}

// routines allocations

int liveGranul_alloc(t_liveGranul *x)
{
    int i;
    if( !(x->x_ind 		= (long *) t_getbytes(NVOICES * sizeof(long)))) return 0; 
    if( !(x->x_remain_ind = (long *) t_getbytes(NVOICES * sizeof(long)))) return 0;
    if( !(x->x_sind 		= (double *) t_getbytes(NVOICES * sizeof(double)))) return 0;
    
    if( !(x->Vbeg		= (double *) t_getbytes(NVOICES * sizeof(double)))) return 0;
    if( !(x->Vtranspos	= (double *) t_getbytes(NVOICES * sizeof(double)))) return 0;
    if( !(x->Vamp		= (double *) t_getbytes(NVOICES * sizeof(double)))) return 0;
    if( !(x->Vlength		= (double *) t_getbytes(NVOICES * sizeof(double)))) return 0;
    if( !(x->Vpan		= (double *) t_getbytes(NVOICES * sizeof(double)))) return 0;
    if( !(x->Vdist		= (double *) t_getbytes(NVOICES * sizeof(double)))) return 0;

    if( !(x->envinc		= (double *) t_getbytes(NVOICES * sizeof(double)))) return 0;
    if( !(x->envind		= (double *) t_getbytes(NVOICES * sizeof(double)))) return 0;
    if( !(x->x_env		= (double *) t_getbytes(NVOICES * sizeof(double)))) return 0;
    if( !(x->Venv		= (int *) t_getbytes(NVOICES * sizeof(int)))) return 0;

    if( !(x->x_delay		= (int *) t_getbytes(NVOICES * sizeof(int)))) return 0;
    if( !(x->x_voiceOn	= (int *) t_getbytes(NVOICES * sizeof(int)))) return 0;
    
    if( !(x->x_hp		= (double *) sysmem_newptr(x->x_nouts * sizeof(double)))) return 0;
    
    if( !(x->Vhp		= (double **) sysmem_newptr(NVOICES * sizeof(double*)))) return 0;
    
    for (int i = 0; i<NVOICES; i++) {
        if( !(x->Vhp[i]		= (double *) sysmem_newptr(x->x_nouts * sizeof(double)))) return 0;}


    return 1;
}

int liveGranul_desalloc(t_liveGranul *x)
{
	int i;
	
	if( x->x_ind )			t_freebytes(x->x_ind, NVOICES * sizeof(long)) ; 
    if( x->x_remain_ind )  	t_freebytes(x->x_remain_ind, NVOICES * sizeof(long)) ;
    if( x->x_sind )			t_freebytes(x->x_sind, NVOICES * sizeof(double)) ;
    
    if( x->Vbeg	) 			t_freebytes(x->Vbeg, NVOICES * sizeof(double)) ;
    if( x->Vtranspos ) 		t_freebytes(x->Vtranspos, NVOICES * sizeof(double)) ;
    if( x->Vamp	)			t_freebytes(x->Vamp, NVOICES * sizeof(double)) ;
    if( x->Vlength	)		t_freebytes(x->Vlength, NVOICES * sizeof(double)) ;
    if( x->Vpan	)			t_freebytes(x->Vpan, NVOICES * sizeof(double)) ;
    if( x->Vdist )			t_freebytes(x->Vdist, NVOICES * sizeof(double)) ;

    if( x->envinc )			t_freebytes(x->envinc, NVOICES * sizeof(double)) ;
    if( x->envind )			t_freebytes(x->envind, NVOICES * sizeof(double)) ;
    if( x->x_env )			t_freebytes(x->x_env, NVOICES * sizeof(double)) ;
    if( x->Venv )			t_freebytes(x->Venv, NVOICES * sizeof(int)) ;

    if( x->x_delay )		t_freebytes(x->x_delay, NVOICES * sizeof(int)) ;
    if( x->x_voiceOn )		t_freebytes(x->x_voiceOn, NVOICES * sizeof(int)) ;
    
    if( x->Vhp )
    {
	    for(i=0;i<x->x_nouts;i++)
	    	 x->Vhp[i] =		t_freebytes(x->Vhp[i], NVOICES * sizeof(double));
    
   		t_freebytes(x->Vhp, x->x_nouts * sizeof(float*)) ;
    }
    
    if( x->w )  			t_freebytes(x->w, (11+x->x_nouts) * sizeof(double**)) ;
	
 return 1; 

}

//***********************************************************************************//
//***********************************************************************************//
//*******************************     DSP      **************************************//
//***********************************************************************************//
//***********************************************************************************//


void liveGranul_perform64(t_liveGranul *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
    
    int n = sampleframes;
    
    t_double * in			= ins[0];
    t_double * t_xing		= ins[1];
    t_double * t_begin 		= ins[2];
    t_double * t_transpos 	= ins[3];
    t_double * t_amp		= ins[4];
    t_double * t_length 	= ins[5];
    t_double * t_pan 		= ins[6];
    t_double * t_dist 		= ins[7];
    
    t_double ** Vhps = x->Vhp;
    int nouts = x->x_nouts;
    int N = n;
    int xn = 0, p;
    
    double srms = x->x_sr*0.001; // double for precision
    double val;
    double sigin, prevsigin,savein; // values of actual and previous input sample
    
    double beginind = x->x_begin*srms; // double for precision (this number can be very large)
    double lengthind = x->x_length*srms;
    
    // temporary buffer index variable
    double * t_bufsamples = x->x_buf_samples;
    int t_active_env;
    t_buffer_obj	*env_buffer;
    float * t_envsamples;
    
    long t_maxlength;
    long t_maxdelay;
    long t_mindelay;
    long t_delay;
    long t_length2;
    long begin;
    
    int i,j,k;
    int nvoices = x->x_nvoices;
    
    // variables grains
    int t_bufframes = x->x_buf_frames;
    
    double finc;
    double temp_sind;
    double source_findex, target_findex;
    int dsp_i_begin;
    int dsp_i_end;
    
    long loop_length;
    long loop_start;
    
    int nech_process, nech_process_sat;
    
    //trick interp long index
    long base_lindex;
    long lindex;
    long lincr;
    
    long base_lindex_env;
    long lindex_env;
    long lincr_env;
    
    int buffer_index;
    int buffer_index_plus_1;
    int interp_table_index;
    
    t_linear_interp * interp_table = x->x_linear_interp_table;
    
    // debug var
#ifdef PERF_DEBUG
    int n_voices_active;
#endif
    
   	sigin = x->x_sigin;
   	
#ifdef PERF_DEBUG
    if(x->ask_poll)
    {
        n_voices_active=0;
        
        for(i=0; i< nvoices; i++) if (x->x_voiceOn[i] ) n_voices_active++;
        
        _SETSYM( x->info_list, gensym("voices"));
        _SETLONG(x->info_list+1, n_voices_active);
        outlet_list(x->info,0l,2,x->info_list);
    }
#endif
    
    buffer_record(x,in,NULL,n);
    

    liveGranul_bufferinfos(x);
    
    t_active_env = bufferenv_check(x,x->x_active_env);
    // %%%%%%%%%%     creation de grains   %%%%%%%%%%%%%%%%%
    
    p=nvoices;
    
    // Dans le cas d'un grain declenche par un bang
    savein = t_xing[0];
    if(x->x_askfor)
    {
        sigin = -1.;
        t_xing[0] = 1.;
        x->x_askfor = 0;
    }
    
    // Meme chose avec declenchement par zero-crossing sur tout le vecteur in, dans la limite fixee par nvoices
    while (n-- && p)
    {
        //-----signal zero-crossing ascendant declenche un grain----//
        prevsigin = sigin;
        sigin = t_xing[xn];
        
        
        if (prevsigin <= 0 && sigin > 0) // creation d'un grain
        {
            while(--p && x->x_voiceOn[p] ) { }  // avance jusqu a la 1ere voix libre
            
            if(p)
            {
                x->x_voiceOn[p] = 1; // index dans buffer
                t_delay = (x->x_in2con) * t_begin[xn]*srms + (1 - x->x_in2con)* beginind;
                t_delay = MAX(t_delay,0.);
                
                x->Vtranspos[p] = (x->x_in3con) * t_transpos[xn] + (1 - x->x_in3con) * x->x_transpos;			// valeur de pitch
                x->Vamp[p]		= (x->x_in4con) * t_amp[xn] + (1 - x->x_in4con) * x->x_amp;						// amplitude
                x->Venv[p] = t_active_env;	// enveloppe active pour ce grain
                
                //if detune < 1. : length < (bufsize-delay)/(detune -1.);
                //if detune > 1. : length < (delay)/(detune -1.);
                
                t_length2 = fabs((x->x_in5con) * t_length[xn]*srms + (1 - x->x_in5con) * lengthind);
                
                t_mindelay = 0;
                t_maxdelay = t_bufframes-1;
                t_maxlength = 4646000;
                
                if(x->x_constr_type)
                {
                    if(!x->x_freeze || x->x_freeze_pad)
                    {
                        if(x->Vtranspos[p] != 1. )
                        {
                            if(x->Vtranspos[p] > 1.)
                                t_mindelay = (x->Vtranspos[p]-1.) * t_length2 - x->x_freeze_pad_curr;
                            else
                                t_maxdelay =  t_bufframes - x->x_freeze_pad_curr + (x->Vtranspos[p]-1.) * t_length2;
                        }
                    }
                    else /// if FREEZE
                    {
                        if(x->Vtranspos[p] > 0.)
                            t_mindelay = (x->Vtranspos[p]) * t_length2 - x->x_freeze_pad_curr;
                        else
                            t_maxdelay =  t_bufframes - x->x_freeze_pad_curr + x->Vtranspos[p] * t_length2;
                        
                    }
                }else
                {
                    if(!x->x_freeze || x->x_freeze_pad)
                    {
                        if(x->Vtranspos[p] != 1. )
                        {
                            if(x->Vtranspos[p] > 1.)
                                t_maxlength =  (t_delay + x->x_freeze_pad_curr) / (x->Vtranspos[p]-1.);
                            else
                                t_maxlength =  -(t_bufframes - t_delay - x->x_freeze_pad_curr) / (x->Vtranspos[p]-1.);
                        }else
                            t_maxlength = 4646000; // NOT MORE OK ???
                    }
                    else /// if FREEZE
                    {
                        if(x->Vtranspos[p] > 0.)
                            t_maxlength =  (t_delay + x->x_freeze_pad_curr) / (x->Vtranspos[p]);
                        else
                            t_maxlength =  -(t_bufframes - t_delay - x->x_freeze_pad_curr) / (x->Vtranspos[p]);
                        
                    }
                }
                
                if(x->x_in5con)
                {
                    if(t_length[xn]<0)
                    {
                        x->Vlength[p]	= t_length2;
                        x->Vlength[p]	= MIN(x->Vlength[p],t_maxlength);
                        x->envinc[p]	= -1.*(double)(x->x_env_frames[t_active_env] - 1.) / x->Vlength[p] ;
                        x->envind[p]	= x->x_env_frames[t_active_env] - 1;
                    }
                    else
                    {
                        x->Vlength[p]	= t_length2;
                        x->Vlength[p]	= MIN(x->Vlength[p],t_maxlength);
                        x->envinc[p]	= (double)(x->x_env_frames[t_active_env] - 1.) / x->Vlength[p] ;
                        x->envind[p]	= 0. ;
                    }
                }
                else
                {
                    x->Vlength[p] = t_length2;
                    x->Vlength[p]	= MIN(x->Vlength[p],t_maxlength);
                    x->envinc[p] = x->x_env_dir * x->x_env_frames[t_active_env] / x->Vlength[p] ;
                    if(x->x_env_dir < 0)
                        x->envind[p] =  x->x_env_frames[t_active_env] - 1;
                    else
                        x->envind[p] = 0.;
                }
                
                t_delay = SATADD(t_delay,t_mindelay,t_maxdelay); // constr delay
                begin = x->x_buf_recpt - t_delay-(1-x->x_freeze)*(n+1);
                x->x_sind[p] = x->Vbeg[p] = MOD(begin, t_bufframes);	// index dans buffer
                x->x_ind[p] = 0;
                x->x_remain_ind[p] = (long) x->Vlength[p];
                x->x_delay[p] = xn;   // delay de declenchement dans le vecteur
                
                //SPAT
                x->Vpan[p]		= (x->x_in6con) * t_pan[xn] + (1 - x->x_in6con) *  x->x_pan;						// pan
                x->Vdist[p]		= (x->x_in7con) * t_dist[xn] + (1 - x->x_in7con) *  x->x_dist;					// distance
                panner(x->Vhp[p],x->x_nouts,x->Vpan[p],x->Vdist[p]);
                
            } else goto perform;
        }
        xn++ ; // boucle on incremente le pointeur dans le vecteur in et le delay de declenchement
    }
    
    // %%%%%%%%%%     fin creation     %%%%%%%%%%%%%%%%%
        
        
        
        perform :
        
        
        
        t_xing[0] = savein;
        
        zero_outs(x,outs,N);
        
        //&&&&&&&&&&&&&  Boucle DSP  &&&&&&&&&&&&&
        n = 0;
        j = N;
        
        for (i=0; i < nvoices; i++)
        {
            //si la voix est en cours
            if (x->x_voiceOn[i]) //&& x->x_ind[i] < x->Vlength[i] )
            {
                // si delay + grand que taille vecteur on passe � voix suivante
                if(x->x_delay[i] >= N)
                {
                    x->x_delay[i] -= N ;
                    goto next;
                }
                
                // nb d'ech a caluler pour ce vecteur
                nech_process = MIN( (N - x->x_delay[i]) , x->x_remain_ind[i] );
                
                // Selon le buffer dans lequel on doit prendre le son x->Vbuf[i] (voir creation des grains)
                // Il se peut que le buffer associ� au grain ne soit plus valide -> verification -> si non on prend buffer #0
                //t_bufnum = x->x_buf_valid_index[x->Vbuf[i]];
                //t_bufframes =  x->x_buf_frames[t_bufnum];
                //t_bufsamples = x->x_buf_samples[t_bufnum];
                //t_bufchan = x->x_buf_nchan[t_bufnum];
                
                t_envsamples = x->x_env_samples[x->Venv[i]];
                
                // pas d'increment en float
                finc = x->Vtranspos[i];
                
                // snd index source
                source_findex = x->x_sind[i];
                // snd index cible en fin de boucle
                target_findex = source_findex + nech_process * finc;
                
                
                
                // if loop
                
                /////////// LOOOP //////////
                nech_process_sat = nech_process;
                
                //target_findex = MOD(x->x_sind[i],x->x_buf_looplength[t_bufnum]);
                
                dsp_i_begin = x->x_delay[i];
                dsp_i_end = x->x_delay[i] + nech_process_sat;
                
                loop_start  = 0;
                loop_length = t_bufframes;
                
                // calcul du vrai float index en ech de lecture
                temp_sind = x->x_sind[i];
                temp_sind -= loop_start;
                temp_sind = loop_start + MOD(temp_sind,loop_length);
                
                // calcul de l'index de lecture buffer en long
                base_lindex = (long) temp_sind;
                lindex = interp_index_scale( temp_sind - (double) base_lindex );
                lincr  = interp_index_scale(finc);
                
                
                
                
                
                // Enveloppe long index & incr calcul
                base_lindex_env = (long) x->envind[i];
                lindex_env = interp_index_scale( x->envind[i] - (double) base_lindex_env );
                lincr_env  = interp_index_scale(x->envinc[i]);
                
                x->envind[i] += nech_process_sat * x->envinc[i];
                
                
                
                //***********************************
                // CALCUL EFFECTIF DES ECHANTILLONS
                //***********************************
                
                if( x->x_sinterp == 1 )  // loop + interp
                    for(j= dsp_i_begin; j < dsp_i_end; j++)
                    {
                        // Lecture de l'enveloppe
                        buffer_index = base_lindex_env + interp_get_int( lindex_env );
                        interp_table_index = interp_get_table_index( lindex_env );
                        x->x_env[i] = interp_table[interp_table_index].a * t_envsamples[buffer_index]
                        + interp_table[interp_table_index].b * t_envsamples[buffer_index + 1];
                        
                        lindex_env += lincr_env;
                        
                        //x->envind[i] += x->envinc[i]; // 2 REMOVE
                        
                        // Lecture de la forme d'onde
                        buffer_index = base_lindex + interp_get_int( lindex );
                        buffer_index -= loop_start;
                        buffer_index = MODI(buffer_index,loop_length);
                        
                        buffer_index_plus_1 = buffer_index + 1;
                        buffer_index_plus_1 = MODI(buffer_index_plus_1,loop_length);
                        //buffer_index_plus_1 = MODI(buffer_index_plus_1,x->x_buf_looplength[t_bufnum]);
                        
                        interp_table_index = interp_get_table_index( lindex );
                        val = interp_table[interp_table_index].a * t_bufsamples[buffer_index]
                        + interp_table[interp_table_index].b * t_bufsamples[buffer_index_plus_1];
                        
                        lindex += lincr;
                        
                        // calcul de la valeur env[i]*son[i]*amp[i]
                        val = x->x_env[i]*val*x->Vamp[i]; 
                        
                        for(k=0;k<nouts;k++)
                            outs[k][j] += Vhps[i][k]*val;
                        
                        
                    }
                else	// loop + non interp
                    for(j= dsp_i_begin; j < dsp_i_end; j++)
                    {
                        // Lecture de l'enveloppe
                        buffer_index = base_lindex_env + interp_get_int( lindex_env );
                        x->x_env[i] = t_envsamples[buffer_index];
                        
                        lindex_env += lincr_env;
                        
                        //x->envind[i] += x->envinc[i]; // 2 REMOVE
                        
                        // Lecture de la forme d'onde
                        buffer_index = base_lindex + interp_get_int( lindex );
                        buffer_index -= loop_start;
                        buffer_index = MODI(buffer_index,loop_length);
                        
                        val = t_bufsamples[buffer_index ];
                        
                        lindex += lincr;
                        
                        // calcul de la valeur env[i]*son[i]*amp[i]
                        val = x->x_env[i]*val*x->Vamp[i]; 
                        
                        for(k=0;k<nouts;k++)
                            outs[k][j] += Vhps[i][k]*val;
                        
                    } 
                
                
                //post("%d %d dsp_i_begin",dsp_i_begin,dsp_i_end);
                
                //********************************
                //  MAJ de l'�tat des grains
                
                x->x_sind[i] = target_findex;
                
                x->x_ind[i] += nech_process_sat;
                
                if( (x->x_remain_ind[i] -= nech_process_sat) <= 0)
                {	
                    x->x_voiceOn[i] = 0;
                    //post("remain kill");
                }
                
                // decremente delai
                x->x_delay[i] = MAX(x->x_delay[i] - N,0);
                
                
                
            } else { 
                
                //sinon = si la voix est libre// 
                x->x_voiceOn[i] = 0;		
            }
            
        next:
            j=0; //DUMMY
            
        } // fin for
        
        
        x->x_sigin = sigin;
    
}
//***********************************************************************************//



//THE END, that's all hulk!!!!!!
