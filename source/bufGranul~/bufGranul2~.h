
/************************************************************************
 *
 *			                >>>>>>> BUFGRANUL2~ <<<<<<<
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
 
#ifndef __BUFGRANUL2_H__
#define __BUFGRANUL2_H__

#include "ext.h"
#include "z_dsp.h"
#include "buffer.h"
#include <string.h>
#include <math.h>

#define NMAXPOLY 1024 // nombre maximum de voix
#define NVOICES 2048 // nombre maximum de voix materielle ( x2 pour le swapping )
#define NSBUF 2048	// nombre maximum de buffers son
#define NEBUF 2048	// nombre maximum de buffers enveloppe
#define MIN_LENGTH 0.1 // longueur minimum des grains en ms  // DISABLED 

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
#define MOD(val,mod) val - (mod * floor( (double)val / mod ))

#define MODI(val,mod) val - (mod * ((val/mod) + (val >> 31))) // modulo LONG 32 bits version 

#define MIN_LOOP_LENGTH 0.1

#define _SETSYM(ap, x) ((ap)->a_type = A_SYM, (ap)->a_w.w_sym = (x))
#define _SETLONG(ap, x) ((ap)->a_type = A_LONG, (ap)->a_w.w_long = (x))
#define _SETFLOAT(ap, x) ((ap)->a_type = A_FLOAT, (ap)->a_w.w_float = (x))

// INTERPOLATION TRICK DEFINES ///    TODO : EXPLAIN

#define TABLE_BITS 10
#define TABLE_SIZE (1 << TABLE_BITS)


#define SINC_TABLE_SIZE 16385

#define MAX_VECTORSIZE 4096

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

typedef struct _bufGranul
{
    t_pxobject x_obj;

// BUFFER SON
	t_buffer *x_buf[NSBUF];				// le buffer~ son ( determine sa "readability" aussi )
	int		 x_buf_valid_index[NSBUF];	// fonction d'un index de buffer vers un index de buffer valide
    t_symbol *x_buf_sym[NSBUF];			// le symbol correspondant au nom du buffer~ son 
    t_symbol *x_buf_filename[NSBUF];	// nom du fichier dans le buffer~ son 
    int		  x_buf_nchan[NSBUF];		// nombre de canaux du buffer~ son
    long	  x_buf_frames[NSBUF];		// le nombre de sample dans le buffer~ son
	float	 *x_buf_samples[NSBUF];		// pointeur sur le tableau d'échantillon du buffer~ son
	double     x_buf_sronsr[NSBUF];		// freq ech buffer sur freq ech globale
	int		  x_nbuffer;				// nombre de buffer son
 	int       x_active_buf;				// le buffer son actif est celui dans lequel les nouveaux grains sont prélevés.
// BUFFER SON LOOP
	long 	  x_buf_loopstart[NSBUF];	// debut de boucle
	long 	  x_buf_loopend[NSBUF];		// fin de boucle
	long 	  x_buf_looplength[NSBUF];	// taille de boucle

// BUFFER ENVELOPPE
    t_buffer *x_env_buf[NEBUF];			// le buffer~ enveloppe
    t_symbol *x_env_sym[NEBUF];			// le symbol correspondant au nom du buffer~ enveloppe
    long	  x_env_frames[NEBUF];		// le nombre de sample dans le buffer~ enveloppe
    float	 *x_env_samples[NEBUF];		// pointeur sur le tableau d'échantillon du buffer~ enveloppe
	int		  x_nenvbuffer;				// nombre de buffer enveloppe
 	int       x_active_env;				// le buffer enveloppe actif est celui dans lequel les nouveaux grains sont prélevés.

// PARAMETRES D'HIVERS
    int		  x_nvoices;				// nombre de voix
    int		  x_sinterp;				// type d'interpolation
    long      x_nouts;					// nombre de sorties (2-4-6-8)
    long      x_bchan_offset;           // channel offset in the snd buffer
    int		  x_nvoices_active;			// nombre de voix actives
    int		  x_loop;					// loop on/off (0 : no loops , 1 : loop entire buffer, 2 : loop between loopstart & end )
    double 	  x_loopstart;
    double 	  x_loopend;
    int       x_poly_mode;              // 0 : wait 1 : round_robin 2 : max_time_lasted 3 : max_time_remaining
     
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
    double *Vtranspos, *Vamp,
		  *Vlength, *Vpan, *Vdist; 

    long * Vloop;
    double * Vloopstart, * Vloopend; // loop start and length per voice ( Vloop == 2 means entire buffer)
    
    long   *Vbuf;				// numero du buffer son dans lequel sera pris le grain

    int   x_env_dir;					// direction de lecture de l'enveloppe
    double *envinc;				// pas d'avancement dans le buffer enveloppe (par rapport à la longueur du grain
	double *envind;				// indice de départ dans le buffer enveloppe (debut si lecture normale, fin si lecture inversee)
	double *x_env;				// enveloppe de chaque voix
    long   *Venv;				// numero du buffer enveloppe dans lequel sera pris le grain

    int   *x_delay;				// delay du declenchement de grain par rapport au vecteur (voir perform)
    int   *x_voiceOn;			// voix active si = 2 : fadingout for kill
    
										// valeur de pan pour chaque voix
    double ** Vhp;

// INTERPOLATION COEF TABLE

	t_linear_interp * x_linear_interp_table;
    double * x_sinc_interp_table;
    double * x_blackman_table;
    double * x_sinc_norm_table;
    
// FADEIN FADEOUT KILL
    double * x_fadein;
    double * x_kill_fadeout;
    double * x_unity_gain;
    
    long x_microtiming;
	 
// DSP
    double x_sr;						// frequence d'echantillonnage
									// booleens vrais si signal connecte sur l'entree i
    short x_in2con, x_in3con, x_in4con, x_in5con, x_in6con, x_in7con, x_in8con; 
    double x_sigin;					// valeur a l'entree de type signal	(voir dsp)
    
// DEBUG
	#ifdef PERF_DEBUG
	int ask_poll;
    void  * info;
    t_atom  info_list[5];
    #endif

} t_bufGranul;

//%%%%%%%%%%%%%%%%%% declaration des routines %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//

// Instanciation
void *bufGranul_new(t_symbol *s, short ac, t_atom *av);	// routine de creation d'une instance de l'objet

int bufGranul_alloc(t_bufGranul *x);
int bufGranul_desalloc(t_bufGranul *x);

// Buffer son
void bufGranul_set(t_bufGranul *x,t_symbol *msg, short ac, t_atom * av);		// definition des buffer~ son 
//void bufGranul_setInit(t_bufGranul *x, t_symbol *s);	// definition du buffer~ son (à l'initialisation du patch)
void bufGranul_swap(t_bufGranul *x);					// bascule buffer son
int bufGranul_bufferinfos(t_bufGranul *x);				// affectation des paramètres des buffers
//void bufGranul_nbuffer(t_bufGranul *x, long n);		// nombre de buffer son

// Bufffer enveloppe
void bufGranul_setenv(t_bufGranul *x, t_symbol *msg, short ac, t_atom * av);	// definition le buffer~ enveloppe
void bufGranul_envbuffer(t_bufGranul *x, long n);		// # du buffer env actif
void bufGranul_nenvbuffer(t_bufGranul *x, long n);		// nombre de buffer enveloppe

// Init
void bufGranul_init(t_bufGranul *x);					// initialisation des buffers
// Grains non-signal
void bufGranul_bang(t_bufGranul *x);					// routine pour un bang, declenchement d'un grain
void bufGranul_grain(t_bufGranul *x, t_symbol *s, short ac, t_atom *av); // declenchement d'un grain par liste
void bufGranul_float(t_bufGranul *x, double f);			// routine de recuperation des parametres non-signal

// Parametres
void bufGranul_sinterp(t_bufGranul *x, long n);		// interpollation dans la lecture du buffer
void bufGranul_poll(t_bufGranul *x, long n);		// nombres de voix en sorties
void bufGranul_clear(t_bufGranul *x);				// panique ! effacement des grains en cours

void bufGranul_killall(t_bufGranul *x);				// small fadeout
void bufGranul_kill(t_bufGranul *x, long k);        // kill a specific voice
void bufGranul_setvoice(t_bufGranul *x, long k);    // TODO : address specific voice

void bufGranul_polymode(t_bufGranul *x, long k);
int bufGranul_poly_assign_voice(t_bufGranul *x);
void bufGranul_poly_check_and_kill(t_bufGranul *x); // when waiting voices are playing


void bufGranul_nvoices(t_bufGranul *x, long n);			// definition du nombre de voix (polyphonie)
void bufGranul_bchan_offset(t_bufGranul *x, long n);    // buffer channel offset
void bufGranul_tellme(t_bufGranul *x);					// demande d'information sur l'etat de l'objet
void bufGranul_loop(t_bufGranul *x, t_symbol *s, short ac, t_atom *av); // definition de loop points dans le buffer
void bufGranul_microtiming(t_bufGranul *x, long flag);        // nombres de voix en sorties

// spatialisation
float spat (float x, float d, int n);
float spat2 (float x, float d);
void panner(double * out, int n, double teta, double d);

void bufGranul_assist(t_bufGranul *x, void *b, long m, long a, char *s);	// assistance info inlet, out1et

// DSP
void bufGranul_free(t_bufGranul *x);										// liberation de la memoire
void bufGranul_dsp64(t_bufGranul *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags);

void bufGranul_perform(t_bufGranul *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam);


// BUFFER
int buffer_check(t_bufGranul *x, int num);		// validite buffer
int bufferenv_check(t_bufGranul *x, int num);	// validite buffer env
int bufGranul_bufferinfos(t_bufGranul *x);				// obtention des infos de buffers


// utilit
float atom2float(t_atom * av, int ind);
t_symbol* atom2symbol(t_atom * av, int ind);

// Debug
#ifdef PERF_DEBUG

	void bufGranul_info(t_bufGranul *x,long n);

#endif

#endif
