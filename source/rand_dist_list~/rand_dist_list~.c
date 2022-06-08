
#include "ext.h"
#include "z_dsp.h"
#include "buffer.h"
#include <stdlib.h>
#include <math.h>

#define MAX_N_POINTS 4096

//#define PERF_DEBUG 1

typedef struct _rand_dist_list
{
    t_pxobject x_obj;
    
    double cur_out;
    double def_out;
    
    double sum_tab[MAX_N_POINTS];
    double total_sum;
    long n_points;
    
    int normalise;
    double norm_max;
    
    double freq;
    long period;
    long dec;
    
    double sr; // frŽquence d'Žchantillonnage
    double sigin; //valeur ˆ l'entrŽ de type signal
    int in_connected;
    int is_active;
    
} t_rand_dist_list;

void *rand_dist_list_new(t_symbol *s, short ac, t_atom *av);
void rand_dist_list_free(t_rand_dist_list *x);
void rand_dist_list_assist(t_rand_dist_list *x, void *b, long m, long a, char *s);
void rand_dist_list_normalise(t_rand_dist_list *x, double f);
void rand_dist_list_bang(t_rand_dist_list *x);
void rand_dist_list_float(t_rand_dist_list *x, double f);
void rand_dist_list_list(t_rand_dist_list *x, t_symbol *msg, short ac, t_atom * av);
void rand_dist_list_active(t_rand_dist_list *x, long n);
void rand_dist_list_default(t_rand_dist_list *x, double f);
void rand_dist_list_dsp64(t_rand_dist_list *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags);
double rand_dist_list_gen(t_rand_dist_list *x);
void rand_dist_list_perform64(t_rand_dist_list *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam);

#ifdef PERF_DEBUG
void  *info;
t_atom  info_list[2];
int ask_poll;
#endif

static t_class *rand_dist_list_class;

int C74_EXPORT main(void)
{
    t_class * c = class_new("rand_dist_list~", (method)rand_dist_list_new, (method)rand_dist_list_free, sizeof(t_rand_dist_list), (method)NULL, A_GIMME, 0);
    
    class_addmethod(c, (method)rand_dist_list_normalise,"normalise", A_FLOAT,0);
    
    class_addmethod(c, (method)rand_dist_list_active, "active",A_LONG,0);
    class_addmethod(c, (method)rand_dist_list_default, "default",A_FLOAT,0);
    class_addmethod(c, (method)rand_dist_list_bang, "bang");
    class_addmethod(c, (method)rand_dist_list_float, "float", A_FLOAT, 0);
    class_addmethod(c, (method)rand_dist_list_list,"list",A_GIMME,0);
    class_addmethod(c, (method)rand_dist_list_assist, "assist", A_CANT, 0);
    class_addmethod(c, (method)rand_dist_list_dsp64, "dsp64", A_CANT, 0);
    
    class_dspinit(c);
    class_register(CLASS_BOX, c);
    rand_dist_list_class = c;
    
    post("rand_dist_list~ 64bit / Build LF - %s %s",__DATE__,__TIME__);
    
    return 0;
}

void rand_dist_list_bang (t_rand_dist_list *x)
{
    x->dec = x->period;
}

#ifdef PERF_DEBUG
void rand_dist_list_poll(t_rand_dist_list *x)
{		ask_poll = 1;}
#endif

void rand_dist_list_active(t_rand_dist_list *x, long n){
    
    x->is_active = (n) ? 1 : 0 ;
}

void rand_dist_list_default(t_rand_dist_list *x, double f)
{
    x->def_out = f;
}

void rand_dist_list_normalise(t_rand_dist_list *x, double f)
{
    if(f<=0.)
        x->normalise = 0;
    else
    {
        x->normalise = 1;
        x->norm_max=f;
    }
}

void rand_dist_list_float(t_rand_dist_list *x, double f)
{
    if (x->x_obj.z_in == 0)
        post("a float in first inlet don't do anything");
    else {
        f = (f > 0.001) ? f : 0.001;
        double fl = (double)(x->sr = sys_getsr()) / 2;
        f = (f < fl) ? f : fl;
        x->freq = f;
        x->period = (long)(x->sr / f);
        //object_post((t_object *)x, "Fred %f, Period %ld", x->freq, x->period);
    }
}

void rand_dist_list_list(t_rand_dist_list *x, t_symbol *msg, short ac, t_atom * av)
{
    int i;
    int p=0;
    double sum=0.;
    
    for(i=0; i< ac && i<MAX_N_POINTS; i++)
    {
        switch (av[i].a_type)
        {
            case A_FLOAT :
                sum += av[i].a_w.w_float;
                x->sum_tab[p++]=sum;
                break;
            case A_LONG :
                sum += av[i].a_w.w_long;
                x->sum_tab[p++]=sum;
                break;
       }
    }
    
    x->total_sum = sum;
    x->n_points = p;
}

double rand_dist_list_gen(t_rand_dist_list *x)
{
    double urand;
    int p=0;
    
    urand = x->total_sum * (double)(rand())/RAND_MAX;
    
    while(p < x->n_points && x->sum_tab[p] < urand ) p++;
    
    if(x->normalise)
        return x->norm_max*(double)(p)/x->n_points;
    else
        return p;
}

void *rand_dist_list_new(t_symbol *s, short ac, t_atom *av)
{
    long k;
    double f;
    t_rand_dist_list *x = (t_rand_dist_list *)object_alloc(rand_dist_list_class);
    if(x)
    {
        dsp_setup((t_pxobject *)x,2);
        
        outlet_new((t_pxobject *)x, "signal");
        
        x->cur_out = 0.;
        x->def_out = 0.;
        
        x->total_sum=1.;
        x->n_points=0;
        
        x->normalise=0;
        x->norm_max=1.;
        
        x->freq=20.;
        x->period=50;
        
        x->sigin=0.; //valeur ˆ l'entrŽ de type signal
        x->in_connected=0;
        x->is_active=1;
        x->sr = sys_getsr();
        
        
        if(ac > 0 && av[0].a_type==A_FLOAT)
        {
            x->def_out = av[0].a_w.w_float;
        }
        
        if(ac > 1 && av[1].a_type==A_FLOAT)
        {
            f = av[1].a_w.w_float;
            x->freq = (f > 0.001) ? f : 0.001;
            x->period = (long)(x->sr / x->freq) ;
        }
        
        k= (long)(x->def_out);
        k = (k < MAX_N_POINTS) ? k : MAX_N_POINTS - 1 ;
        k = (k > 0 ) ? k : 0;
        x->sum_tab[k]=1.;
    }
    
    return (x);
}

void rand_dist_list_assist(t_rand_dist_list *x, void *b, long m, long a, char *s)
{
    switch(m) {
        case 1: // inlet
            switch(a) {
                case 0:
                    sprintf(s, " list, signal zero X-ing, normalise)");
                    break;
                case 1:
                    sprintf(s, " freq (float) ");
                    break;
            }
            break;
        case 2: // outlet
            switch(a) {
                case 0:
                    sprintf(s, "(signal) rand output");
                    break;
            }
            break;
    }
}

void rand_dist_list_perform64(t_rand_dist_list *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
    t_double *in = ins[0];
    t_double *out = outs[0];
    
    int n = sampleframes;
    int dec = x->dec;
    double sigin = x->sigin ;
    double prevsigin  ;
    double cur_out = x->cur_out;
    x->period = (long)(x->sr / x->freq);
    
    if(x->is_active || x->n_points <= 0)
    {
        if(x->in_connected) // zero x-ing generation
        {
            while(n--)
            {
                prevsigin = sigin;    	sigin = *in++;
                
                if (prevsigin <= 0 && sigin > 0)
                    cur_out = rand_dist_list_gen(x);
                
                *out++ = cur_out;
            }
            x->sigin = sigin;
        }
        else // intern generation
        {
            while(n--)
            {
                if(--dec <= 0)
                {
                    cur_out = rand_dist_list_gen(x);
                    dec = x->period;
                }
                *out++ = cur_out;
            }
        }
    }
    else
        while(n--)
            *out++ = x->def_out;
    
    x->cur_out = cur_out;
    x->dec = dec;
}

void rand_dist_list_dsp64(t_rand_dist_list *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags)
{
    x->sr = sys_getsr();
    srand(gettime());
    x->in_connected = count[0];
    
    object_method(dsp64, gensym("dsp_add64"), x, rand_dist_list_perform64, 0, NULL);
}

void rand_dist_list_free(t_rand_dist_list *x)
{
    dsp_free((t_pxobject *) x);
}
