
#include "ext.h"
#include <stdlib.h>
#include <math.h>

#define MAX_N_POINTS 4096

//#define PERF_DEBUG 1

typedef struct _rand_dist_list
{
    t_object x_obj;
    double cur_out;
    double sum_tab[MAX_N_POINTS];
    double total_sum;
    long n_points;
    int normalise;
    double norm_max;
} t_rand_dist_list;

void *rand_dist_list_new(t_symbol *s, short ac, t_atom *av);
void rand_dist_list_normalise(t_rand_dist_list *x, double f);
void rand_dist_list_bang(t_rand_dist_list *x);
void rand_dist_list_list(t_rand_dist_list *x, t_symbol *msg, short ac, t_atom * av);
void rand_dist_list_assist(t_rand_dist_list *x, void *b, long m, long a, char *s);
void rand_dist_list_free(t_rand_dist_list *x);
float rand_dist_list_gen(t_rand_dist_list *x);

#ifdef PERF_DEBUG
void  *info;
t_atom  info_list[2];
int ask_poll;
#endif

static t_class * rand_dist_list_class;

int C74_EXPORT main(void)
{
    t_class * c = class_new("rand_dist_list", (method)rand_dist_list_new, (method)rand_dist_list_free, sizeof(t_rand_dist_list), (method)NULL, A_GIMME, 0);
    
    class_addmethod(c, (method)rand_dist_list_bang,     "bang",     0);
    class_addmethod(c, (method)rand_dist_list_list,     "list",     A_GIMME,    0);
    class_addmethod(c, (method)rand_dist_list_assist,   "assist",   A_CANT,     0);
    class_addmethod(c, (method)rand_dist_list_normalise,"normalise",A_FLOAT,    0);
    
    class_register(CLASS_BOX, c);
    rand_dist_list_class = c;
    
    post("rand_dist_list 64bit / Build CB - %s %s",__DATE__,__TIME__);
    
    return 0;
}

void rand_dist_list_bang (t_rand_dist_list *x)
{
    x->cur_out = rand_dist_list_gen(x);
    outlet_float(x->x_obj.o_outlet,x->cur_out);
}

#ifdef PERF_DEBUG
void rand_dist_list_poll(t_rand_dist_list *x)
{		ask_poll = 1;}
#endif


void rand_dist_list_normalise(t_rand_dist_list *x, double f) {
    if(f<=0.)
        x->normalise = 0;
    else
    {
        x->normalise = 1;
        x->norm_max=f;
    }
}

void rand_dist_list_list(t_rand_dist_list *x, t_symbol *msg, short ac, t_atom * av)
{
    int p=0;
    double sum=0.;
    
    for(int i=0; i< ac && i<MAX_N_POINTS; i++)
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

float rand_dist_list_gen(t_rand_dist_list *x)
{
    double urand;
    int p=0;
    
    urand = x->total_sum * (double)(rand())/RAND_MAX;
    
    while(p < x->n_points && x->sum_tab[p] < urand)
        p++;
    
    if(x->normalise)
        return x->norm_max*(float)(p)/x->n_points;
    else
        return (float)(p);
}

void *rand_dist_list_new(t_symbol *s, short ac, t_atom *av)
{
    t_rand_dist_list *x;
    
    x = (t_rand_dist_list *)object_alloc(rand_dist_list_class);
    if(x)
    {
        floatout(x);
        x->cur_out = 0.;
        x->total_sum =1.;
        x->n_points=0;
        x->normalise=0;
        x->norm_max=1.;
    }
    return (x);
}

void rand_dist_list_assist(t_rand_dist_list *x, void *b, long m, long a, char *s)
{
    switch(m) {
        case 1: // inlet
            switch(a)
            {
                case 0:
                    sprintf(s, " list, bang, normalise)");
                    break;
                    
            }
            break;
        case 2: // outlet
            switch(a)
            {
                case 0:
                    sprintf(s, "rand output");
                    break;
            }
            break;
    }
}

void rand_dist_list_free(t_rand_dist_list *x)
{	
}
