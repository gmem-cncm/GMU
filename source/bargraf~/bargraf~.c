//#undef BARGRAF2
//#define BARGRAF2

#include "ext.h"
#include "ext_obex.h"
#include "jpatcher_api.h"	// required for ui objects
#include "jgraphics.h"		// required for ui objects
#include "z_dsp.h"
#include <math.h>

//inline t_jrgba RGBA(double r, double g, double b, double a) { t_jrgba v; v.red = r; v.green = g; v.blue = b; v.alpha = a; return v; }

t_class	*s_bargraf_class = 0;

typedef struct _bargraf
{
	t_pxjbox	j_pxjbox;		// replacement of t_object
	t_int		b_levelOut ;	// si saturation
    t_int		b_flag;			// flag pour remettre min et max ‡ la valeur courante du vecteur quand metro tick
    t_int		b_sens;			// true = vertical bargraf
	
	/*********  DSP  ***********/
    t_float		b_maxvaldB;  	// sampled value in dB
    t_float		b_maxval;		// sampled linear value
	t_float		b_absmaxvaldB;	// last max value in dB
	t_float		b_maxlast;  	// last max value (~positive dB)
	t_float		b_maxlastdB;  	// last max value (dB)
	t_float		b_val;      	// current value (")
	void*		b_out1;
	void*		b_out2;
	void*		b_out3;
	
	t_float		b_interval;		// number of ms during each line sr/f
    char		b_clockstate;	// state of the clock on/off
	t_int		b_startclock;	// start clock flag
    void*		b_clock; 

	long		b_countmax;		// number of measures for peaks 
	long		b_count;		// number of measuers counter 
	char		b_satur;		// keep saturation
	char		b_showv;		// show value
	char		b_showb;		// show border
	char		b_showf;		// show frame rect
	char		b_nodraw;		// disable drawing
	char		b_mouseover;	// mouse is over ?
	char		b_drawmode;		// drawing modes
	char		b_memdrawmode;	// memory (see mouseclick)

	/******** COLORS *********/
	t_jrgba		b_brgb;			//background
	t_jrgba		b_frgb;			//foreground
	t_jrgba		b_tcolor;		//textcolor
	t_jrgba		b_frame;		//frame
	t_jrgba		b_hisha;		//highlighted shadow
	t_jrgba		b_dasha;		//dark shadow
	t_jrgba		b_hiaxe;		//axe clairs
	t_jrgba		b_daaxe;		//axe foncés
	t_jrgba		b_c1;			//first : green
	t_jrgba		b_c2;
	t_jrgba		b_c3;
	t_jrgba		b_c4;
	t_jrgba		b_c5;			//last : red

} t_bargraf;

// prototypes
void bargraf_assist(t_bargraf *x, void *b, long m, long a, char *s);
t_bargraf* bargraf_new(t_symbol *s, long argc, t_atom *argv);
void bargraf_free(t_bargraf *x);
void bargraf_paint(t_bargraf *x, t_object *view);
void bargraf_mousedown(t_bargraf *x, t_object *patcherview, t_pt pt, long modifiers);
void bargraf_mouseenter(t_bargraf *x, t_object *patcherview, t_pt pt, long modifiers);
void bargraf_mouseleave(t_bargraf *x, t_object *patcherview, t_pt pt, long modifiers);
void bargraf_float(t_bargraf *x, double f);
void bargraf_int(t_bargraf *x, long n);
void bargraf_clear(t_bargraf *x);
void bargraf_bang(t_bargraf *x);
void bargraf_saturation(t_bargraf *x, long n);
void bargraf_tick(t_bargraf *x);
void bargraf_version(t_bargraf *x);
t_max_err bargraf_stdargs(t_dictionary *d, t_symbol *s, long argc, t_atom *argv);
t_max_err bargraf_setclock(t_bargraf *x, void *attr, long ac, t_atom *av);
t_max_err bargraf_setdrawmode(t_bargraf *x, void *attr, long ac, t_atom *av);

void bargraf_dsp64(t_bargraf *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags);
void bargraf_perform64(t_bargraf *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam);

void ext_main(void *r)
{
	t_class *c;
	
	c = class_new("bargraf~", (method)bargraf_new, (method)bargraf_free, sizeof(t_bargraf), (method)NULL, A_GIMME, 0L);
	
	c->c_flags |= CLASS_FLAG_NEWDICTIONARY; // required (why? for default values ?)
	
	jbox_initclass(c, /*JBOX_COLOR | JBOX_FIXWIDTH |*/ JBOX_FONTATTR);

	// init the class and add default attributes
	// JBOX_COLOR < adds color attributes
	// JBOX_FIXWIDTH < reset to default size when fix width command is called
	
	class_addmethod(c, (method) bargraf_paint,	"paint",	A_CANT,		0);
	class_addmethod(c, (method) bargraf_bang,	"bang",		0);
	class_addmethod(c, (method) bargraf_int,	"int",		A_LONG,		0);
	CLASS_METHOD_ATTR_PARSE(c, "int", "undocumented", gensym("long"), 0, "1");
	class_addmethod(c, (method) bargraf_float,	"float",	A_FLOAT,	0);
	class_addmethod(c, (method) bargraf_int,	"in1",		A_LONG,		0);
	CLASS_METHOD_ATTR_PARSE(c, "in1", "undocumented", gensym("long"), 0, "1");
	class_addmethod(c, (method) bargraf_float,	"ft1",		A_FLOAT,	0);
	CLASS_METHOD_ATTR_PARSE(c, "ft1", "undocumented", gensym("long"), 0, "1");
	class_addmethod(c, (method) bargraf_clear,	"clear",	0);
	class_addmethod(c, (method)	bargraf_saturation,	"mode",	A_LONG,		0); // for backward compatibility
	CLASS_METHOD_ATTR_PARSE(c, "mode", "undocumented", gensym("long"), 0, "1");
	class_addmethod(c, (method)	bargraf_version, "version",	0);

	class_addmethod(c, (method) bargraf_mousedown, "mousedown", A_CANT, 0); // click on object
	class_addmethod(c, (method) bargraf_mouseenter, "mouseenter", A_CANT, 0); // mouseenter on object
	class_addmethod(c, (method) bargraf_mouseleave, "mouseleave", A_CANT, 0); // mouseleave on object

	class_addmethod(c, (method) bargraf_assist, "assist", A_CANT, 0);
	class_addmethod(c, (method) jbox_notify, "notify", A_CANT, 0);
	// for automatic redraw when attributes are modified (for ex by the inspector)
	
	//class_addmethod(c, (method) bargraf_getdrawparams, "getdrawparams", A_CANT, 0);

	class_addmethod(c, (method) bargraf_dsp64, "dsp64", A_CANT, 0);

	// for getting max4 attributes/sizes...
	 class_addmethod(c, (method)bargraf_stdargs, "stdargs", A_CANT, 0);
	
	// default size
	CLASS_ATTR_DEFAULT(c, "patching_rect", 0, "0 0 24 122");

	// ATTRIBUTES
	// nodraw
	CLASS_ATTR_CHAR(c, "nodraw", 0, t_bargraf, b_nodraw);
	CLASS_ATTR_STYLE_LABEL(c, "nodraw", 0, "onoff", "Disable painting");
	CLASS_ATTR_DEFAULT(c, "nodraw", 0, "0");
	CLASS_ATTR_CATEGORY(c, "nodraw", 0, "Value");
	CLASS_ATTR_INVISIBLE(c, "nodraw", 0);

	// drawing mode
	CLASS_ATTR_CHAR(c, "drawmode", 0, t_bargraf, b_drawmode);
 	CLASS_ATTR_ACCESSORS(c, "drawmode", (method)NULL, (method)bargraf_setdrawmode);
	CLASS_ATTR_STYLE_LABEL(c, "drawmode", 0, "enumindex", "Drawing mode");
	CLASS_ATTR_ENUMINDEX(c, "drawmode", 0, "Standard \"Draw when mouse is over\" \"Calculate and draw when mouse is over\"");
	CLASS_ATTR_DEFAULT_SAVE(c, "drawmode", 0, "0");
	CLASS_ATTR_FILTER_CLIP(c, "drawmode", 0, 2);
	CLASS_ATTR_CATEGORY(c, "drawmode", 0, "Value");

	// interval
	CLASS_ATTR_FLOAT(c, "interval", 0, t_bargraf, b_interval);
	CLASS_ATTR_STYLE_LABEL(c, "interval", 0, "text", "Poll interval (ms)");
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c, "interval", 0, "100");
	CLASS_ATTR_FILTER_CLIP(c, "interval", 5., 5000.);
	CLASS_ATTR_CATEGORY(c, "interval", 0, "Value");

	// saturation
	CLASS_ATTR_CHAR(c, "saturation", 0, t_bargraf, b_satur);
	CLASS_ATTR_STYLE_LABEL(c, "saturation", 0, "onoff", "Keep Saturation");
	CLASS_ATTR_DEFAULT_SAVE(c, "saturation", 0, "0");
	CLASS_ATTR_CATEGORY(c, "saturation", 0, "Value");

	// count
	CLASS_ATTR_LONG(c, "count", 0, t_bargraf, b_countmax);
	CLASS_ATTR_STYLE_LABEL(c, "count", 0, "text", "Number of cycle to keep peaks");
	CLASS_ATTR_DEFAULT_SAVE(c, "count", 0, "12");
	CLASS_ATTR_FILTER_CLIP(c, "count", 1, 1024);
	CLASS_ATTR_CATEGORY(c, "count", 0, "Value");

	// clock
	CLASS_ATTR_CHAR(c, "clock", 0, t_bargraf, b_clockstate);
 	CLASS_ATTR_ACCESSORS(c, "clock", (method)NULL, (method)bargraf_setclock);
	CLASS_ATTR_STYLE_LABEL(c, "clock", 0, "onoff", "Clock state");
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c, "clock", 0, "1");
	CLASS_ATTR_CATEGORY(c, "clock", 0, "Value");

	// showv
	CLASS_ATTR_CHAR(c, "show_value", 0, t_bargraf, b_showv);
	CLASS_ATTR_STYLE_LABEL(c, "show_value", 0, "enumindex", "Show Amplitude Value");
	CLASS_ATTR_ENUMINDEX(c, "show_value", 0, "Off \"Current dB value\" \"Current linear value\" \"Local dB peaks\" \"Maximum dB value\"");
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c, "show_value", 0, "0");
	CLASS_ATTR_CATEGORY(c, "show_value", 0, "Value");

	// showb
	CLASS_ATTR_CHAR(c, "border", 0, t_bargraf, b_showb);
	CLASS_ATTR_STYLE_LABEL(c, "border", 0, "onoff", "Show Border");
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c, "border", 0, "1");
	CLASS_ATTR_CATEGORY(c, "border", 0, "Value");
	
	// showf
	CLASS_ATTR_CHAR(c, "frame", 0, t_bargraf, b_showf);
	CLASS_ATTR_STYLE_LABEL(c, "frame", 0, "onoff", "Show Frame");
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c, "frame", 0, "1");
	CLASS_ATTR_CATEGORY(c, "frame", 0, "Value");

	// COLORS
	// Background, Foreground, 1. Cold, 2. Mild, 3. Warm, 4. Warning (Hot), 5. Overload, Highlighted Shadow, Dark Shadow, Highlighted Axes, Dark Axes, Frame
	// Background Color
	CLASS_ATTR_RGBA(c, "brgb", 0, t_bargraf, b_brgb);
	CLASS_ATTR_STYLE_LABEL(c, "brgb", 0, "rgba", "Background Color");
	CLASS_ATTR_DEFAULT_SAVE(c, "brgb", 0, "0.61 0.61 0.61 1");
	CLASS_ATTR_CATEGORY(c, "brgb", 0, "Color");
	// Foreground Color
	CLASS_ATTR_RGBA(c, "frgb", 0, t_bargraf, b_frgb);
	CLASS_ATTR_STYLE_LABEL(c, "frgb", 0, "rgba", "Foreground Color");
	CLASS_ATTR_DEFAULT_SAVE(c, "frgb", 0, "0 0 0 1");
	CLASS_ATTR_CATEGORY(c, "frgb", 0, "Color");

	// Cold Color
	CLASS_ATTR_RGBA(c, "rgb1", 0, t_bargraf, b_c1);
	CLASS_ATTR_STYLE_LABEL(c, "rgb1", 0, "rgba", "Cold Color");
	CLASS_ATTR_DEFAULT_SAVE(c, "rgb1", 0, "0 0.73 0 1");
	CLASS_ATTR_CATEGORY(c, "rgb1", 0, "Color");
	// Mild Color
	CLASS_ATTR_RGBA(c, "rgb2", 0, t_bargraf, b_c2);
	CLASS_ATTR_STYLE_LABEL(c, "rgb2", 0, "rgba", "Mild Color");
	CLASS_ATTR_DEFAULT_SAVE(c, "rgb2", 0, "0.3 0.79 0 1");
	CLASS_ATTR_CATEGORY(c, "rgb2", 0, "Color");
	// Warm Color
	CLASS_ATTR_RGBA(c, "rgb3", 0, t_bargraf, b_c3);
	CLASS_ATTR_STYLE_LABEL(c, "rgb3", 0, "rgba", "Warm Color");
	CLASS_ATTR_DEFAULT_SAVE(c, "rgb3", 0, "0.49 0.88 0 1");
	CLASS_ATTR_CATEGORY(c, "rgb3", 0, "Color");
	// Warming (Hot) Color
	CLASS_ATTR_RGBA(c, "rgb4", 0, t_bargraf, b_c4);
	CLASS_ATTR_STYLE_LABEL(c, "rgb4", 0, "rgba", "Warming (Hot) Color");
	CLASS_ATTR_DEFAULT_SAVE(c, "rgb4", 0, "1 1 0 1");
	CLASS_ATTR_CATEGORY(c, "rgb4", 0, "Color");
	// Overload Color
	CLASS_ATTR_RGBA(c, "rgb5", 0, t_bargraf, b_c5);
	CLASS_ATTR_STYLE_LABEL(c, "rgb5", 0, "rgba", "Overload Color");
	CLASS_ATTR_DEFAULT_SAVE(c, "rgb5", 0, "1 0 0 1");
	CLASS_ATTR_CATEGORY(c, "rgb5", 0, "Color");

	// Highlighted Shadow Color
	CLASS_ATTR_RGBA(c, "rgb6", 0, t_bargraf, b_hisha);
	CLASS_ATTR_STYLE_LABEL(c, "rgb6", 0, "rgba", "Highlighted Shadow Color");
	CLASS_ATTR_DEFAULT_SAVE(c, "rgb6", 0, "0.88 0.88 0.88 1");
	CLASS_ATTR_CATEGORY(c, "rgb6", 0, "Color");
	// Dark Shadow Color
	CLASS_ATTR_RGBA(c, "rgb7", 0, t_bargraf, b_dasha);
	CLASS_ATTR_STYLE_LABEL(c, "rgb7", 0, "rgba", "Dark Shadow Color");
	CLASS_ATTR_DEFAULT_SAVE(c, "rgb7", 0, "0.43 0.43 0.43 1");
	CLASS_ATTR_CATEGORY(c, "rgb7", 0, "Color");
	// Highlighted Axes Color
	CLASS_ATTR_RGBA(c, "rgb8", 0, t_bargraf, b_hiaxe);
	CLASS_ATTR_STYLE_LABEL(c, "rgb8", 0, "rgba", "Highlighted Axes Color");
	CLASS_ATTR_DEFAULT_SAVE(c, "rgb8", 0, "0.88 0.88 0.88	1");
	CLASS_ATTR_CATEGORY(c, "rgb8", 0, "Color");
	// Dark Axes Color
	CLASS_ATTR_RGBA(c, "rgb9", 0, t_bargraf, b_daaxe);
	CLASS_ATTR_STYLE_LABEL(c, "rgb9", 0, "rgba", "Dark Axes Color");
	CLASS_ATTR_DEFAULT_SAVE(c, "rgb9", 0, "0.43 0.43 0.43 1");
	CLASS_ATTR_CATEGORY(c, "rgb9", 0, "Color");
	// Frame Color
	CLASS_ATTR_RGBA(c, "rgb10", 0, t_bargraf, b_frame);
	CLASS_ATTR_STYLE_LABEL(c, "rgb10", 0, "rgba", "Frame Color");
	CLASS_ATTR_DEFAULT_SAVE(c, "rgb10", 0, "0 0 0 1");
	CLASS_ATTR_CATEGORY(c, "rgb10", 0, "Color");
	
	// Text Color
	CLASS_ATTR_RGBA(c, "textcolor", 0, t_bargraf, b_tcolor);
	CLASS_ATTR_STYLE_LABEL(c, "textcolor", 0, "rgba", "Text Color");
	CLASS_ATTR_DEFAULT_SAVE(c, "textcolor", 0, "0.75 0.75 0.75 1");
	CLASS_ATTR_CATEGORY(c, "textcolor", 0, "Font");

	// ORDER
	CLASS_ATTR_ORDER(c, "brgb", 0, "1");
	CLASS_ATTR_ORDER(c, "frgb", 0, "2");
	CLASS_ATTR_ORDER(c, "rgb1", 0, "3");
	CLASS_ATTR_ORDER(c, "rgb2", 0, "4");
	CLASS_ATTR_ORDER(c, "rgb3", 0, "5");
	CLASS_ATTR_ORDER(c, "rgb4", 0, "6");
	CLASS_ATTR_ORDER(c, "rgb5", 0, "7");
	CLASS_ATTR_ORDER(c, "rgb6", 0, "8");
	CLASS_ATTR_ORDER(c, "rgb7", 0, "9");
	CLASS_ATTR_ORDER(c, "rgb8", 0, "10");
	CLASS_ATTR_ORDER(c, "rgb9", 0, "11");
	CLASS_ATTR_ORDER(c, "rgb10", 0, "12");
	CLASS_ATTR_ORDER(c, "textcolor", 0, "13");

	class_dspinitjbox(c);
	
	s_bargraf_class = c;
	class_register(CLASS_BOX, s_bargraf_class);
	
	post("Bargraf~ for max7 GMEM Marseille France");
	post("\tBuild cb - %s %s",__DATE__,__TIME__);
	return ;
}

// getting max4 attributes/sizes...
t_max_err bargraf_stdargs(t_dictionary *d, t_symbol *s, long argc, t_atom *argv)
{
	int left, top, width, height;
	t_atom tmp;//, tmp_r, tmp_g, tmp_b;
	
	// left / top / width / height / interval / mode / 3*brgb / 3*frgb / 3*c1 / 3*c2 / 3*c3 / 3*c4 / 3*c5 / 3*hisha / 3*dasha / 3*hiaxe / 3*daaxe / 3*bframe / 3*countmax / 3*clockstate
	
	left = argv[0].a_w.w_long;
	top = argv[1].a_w.w_long;
	height = argv[2].a_w.w_long;
	width = argv[3].a_w.w_long;
	
	tmp = argv[2];
	argv[2] = argv[3];
	argv[3] = tmp;
		
	if(d)
	{
		dictionary_appendatoms(d, gensym("patching_rect"), 4, argv);
		argv += 4;
		dictionary_appendatoms(d, gensym("interval"), 1, argv);
		argv++;
		dictionary_appendatoms(d, gensym("saturation"), 1, argv);
		argv++;
		// TODO get the right colors :
		// frgb
	/*	tmp_r.a_type = A_FLOAT;
		tmp_r.a_w.w_float = argv[0].a_w.w_long/256;
		argv[0] = tmp_r;
		tmp_g.a_type = A_FLOAT;
		tmp_g.a_w.w_float = argv[1].a_w.w_long/256;
		argv[1] = tmp_g;
		tmp_b.a_type = A_FLOAT;
		tmp_b.a_w.w_float = argv[2].a_w.w_long/256;
		argv[2] = tmp_b;
		dictionary_appendatoms(d, gensym("frgb"), 3, argv);//*/
		argv += 3;
		// brgb
		argv += 3;
		// rgb1, rgb2, rgb3, rgb4, rgb5, rgb6, rgb7, rgb8, rgb9, rgb10
		argv += 3;
		argv += 3;
		argv += 3;
		argv += 3;
		argv += 3;
		argv += 3;
		argv += 3;
		argv += 3;
		argv += 3;
		argv += 3;
		dictionary_appendatoms(d, gensym("count"), 1, argv);
		argv++;
		dictionary_appendatoms(d, gensym("clock"), 1, argv);
		argv++;
		/*
		 int i = 0;
		// post("%s",s->s_name);
		 while(i < argc)
		 {
			 switch (argv[i].a_type) {
				 case A_LONG:
					 post("arg : %d - long %ld", i, argv[i].a_w.w_long);
					 break;
				 case A_FLOAT:
					 post("arg : %d - float %f", i, argv[i].a_w.w_float);
					 break;
				 case A_SYM:
					 post("arg : %d - sym %s", i, argv[i].a_w.w_sym->s_name);
					 break;
				 default:
					 post("arg : %d - ? %d", i , argv[i].a_type);
					 break;
			 }
			 i++;
		 }//*/
	}
	return MAX_ERR_NONE;
}

void bargraf_version(t_bargraf *x)
{
	object_post((t_object *)x,"Copyright © 2009 Bargraf~ for Max5 GMEM Marseille France");
	object_post((t_object *)x,"\tBuild LF - %s %s",__DATE__,__TIME__);
}

void bargraf_assist(t_bargraf *x, void *b, long m, long a, char *s)
{
    switch(m)
	{
	case ASSIST_INLET:
		switch(a) {
			case 0 : sprintf(s,"(Signal) Signal to draw, (int/float) Delay between ticks (ms), clear, bang");
				break;
			case 1 : sprintf(s,"(int/float) Delay between ticks (ms)");
				break;
		}
		break;
	case ASSIST_OUTLET:
		switch(a) {

			case 0 : sprintf(s,"(float) Amplitude (dB)");
				break;
			case 1 : sprintf(s,"(float) Amplitude (Linear)");
				break;
			case 2 : sprintf(s,"(float) Peak Amplitude (dB)");
				break;

		}
		break;
	}
}

t_bargraf* bargraf_new(t_symbol *s, long argc, t_atom *argv)
{
	t_bargraf* x = NULL;
	t_max_err err = MAX_ERR_GENERIC;
	t_dictionary *d;
	long flags;
	
	// instead of traditionnal arguments passed by argc argv
	// arguments are passed by a dictionnary
	// required
	if (!(d=object_dictionaryarg(argc,argv)))
		return NULL;
	
	// allocate memory
	x = (t_bargraf*) object_alloc(s_bargraf_class);
	
	if(!x)
		return NULL;
	flags = 0 
			| JBOX_DRAWFIRSTIN 
			| JBOX_NODRAWBOX
			| JBOX_DRAWINLAST
			| JBOX_TRANSPARENT	
	//		| JBOX_NOGROW
	//		| JBOX_GROWY
			| JBOX_GROWBOTH
			| JBOX_HILITE
	//		| JBOX_BACKGROUND
	//		| JBOX_TEXTFIELD
			| JBOX_DRAWBACKGROUND
	//		| JBOX_DEFAULTNAMES
			;

	// ORDER IS REALLY IMPORTANT

	// creates the box and transfer it the dictionnary
	err = jbox_new(&x->j_pxjbox.z_box, flags, argc, argv); 

	// set the b_firstin pointer of the jbox to this object -> usefull for jbox_get_object()
	x->j_pxjbox.z_box.b_firstin = (t_object*) x;

	// set dictionnary attributes to the object
	attr_dictionary_process(x, d);

	dsp_setupjbox((t_pxjbox *)x,1);

	// object initialization
	floatin ((t_pxjbox *)x,1);
  	x->b_out1 = floatout(x);
  	x->b_out2 = floatout(x);
  	x->b_out3 = floatout(x);
 
	x->b_maxval = 0.;
	x->b_maxvaldB = -96.;
    x->b_absmaxvaldB = -96.;
 	x->b_val = 0;
	x->b_levelOut = false;
	x->b_count = 0;
	x->b_memdrawmode = 2;

    x->b_clock = clock_new(x, (method)bargraf_tick);
//  clock_fdelay(x->b_clock, x->b_interval);
	
	// initialize any paint-related data here
	jbox_ready(&x->j_pxjbox.z_box);

	
	return x;
}

void bargraf_free(t_bargraf *x)
{
	dsp_freejbox((t_pxjbox *)x);
	clock_free(x->b_clock);
	jbox_free(&x->j_pxjbox.z_box);
}

void bargraf_paint(t_bargraf *x, t_object *view)
{
	t_jgraphics *g; // graphic context
	t_rect rect;
	t_jfont *jf;
	t_jtextlayout *jtl;
	char text[16];	
	t_float weight, height;
	t_float wr, wrL, wrR, wBG;
	t_float hr, hT, hB, hBG, hFG;
	t_float pad_value_font, pad_value_recth, pad_value_rectw;
	t_float penW, penH ;
	t_int i = 10;
	t_int flag;
	t_float pos, rel_pos;

	if(x->b_nodraw)
		return;
	if((x->b_drawmode && !x->b_mouseover) || x->j_pxjbox.z_box.b_hidden)
	{
		if(x->b_showf)
		{
			t_jgraphics *g;
			t_rect rect;
			g = (t_jgraphics*) patcherview_get_jgraphics(view);
			jbox_get_rect_for_view(&x->j_pxjbox.z_box.b_ob, view, &rect);
			jgraphics_set_source_jrgba(g, &x->b_frame);
			jgraphics_rectangle_draw_fast(g, 0, 0, rect.width, rect.height, 1);

		}		
		return;
	}
		

	g = (t_jgraphics*) patcherview_get_jgraphics(view);
	jbox_get_rect_for_view(&x->j_pxjbox.z_box.b_ob, view, &rect);

	weight 	= 	rect.width ;
	height 	= 	rect.height ;
	x->b_sens = (rect.height > rect.width);

	if (x->b_sens)
	{
		weight 	= 	rect.width ;
		height 	= 	rect.height ;
	} else {
		jgraphics_translate(g, rect.width, rect.height);
		jgraphics_scale(g, -1, 1);
		jgraphics_rotate(g, -JGRAPHICS_PIOVER2);
		height 	= 	rect.width ;
		weight 	= 	rect.height ;
	}

	penW = (weight< 48 ? 1 : 2);	
	penH = (height < 96 ? 1 : 2);	

	wr = weight/6. ;
	hr = height/(x->b_showv && x->b_sens ? 14. : 12);
	wrL = 2*wr;
	wrR = wr;
	hT = hr;
	hB = hr;

	pad_value_font = 2;
	pad_value_rectw = 0;
	pad_value_recth = 0.5*hr;
		
	if(!x->b_showb)
	{
		wrL = 0;
		wrR = 0;
		penW = 0;
		penH = 0;
		hT = 0;
		hB = 0;
		pad_value_font = 0;
		pad_value_recth = 0;
		hr = height/(x->b_showv && x->b_sens ? 12 : 10);
	}
	hFG = x->b_showv && x->b_sens ? 2*hr : 0;

	wBG = weight - wrL - wrR;
	hBG = height - hT - hB - hFG;
	
	if(x->b_showb)
	{
		//----------- border ------------------//
		jgraphics_rectangle(g, 0, 0, weight, height);
		jgraphics_set_source_jrgba(g, &x->b_brgb);
		jgraphics_fill(g);

		//-----------  contour ext -----------------//
		jgraphics_rectangle(g, 1, 1, weight-1-penW, penH);
		jgraphics_rectangle(g, 1, 1, penW, height-2);
		jgraphics_set_source_jrgba(g, &x->b_hisha);
		jgraphics_fill(g);

		jgraphics_rectangle(g, weight-1-penW, 1, penW, height-1-penH);
		jgraphics_rectangle(g, 1+penW, height-1-penH, weight-2-penW, penH);
		jgraphics_set_source_jrgba(g, &x->b_dasha);	
		jgraphics_fill(g);
		
		//-----------  contour int -----------------//
		jgraphics_rectangle(g, weight - wrR, hT-penH, penW, hBG+2*penH);
		jgraphics_rectangle(g, wrL, hT+hBG, wBG + penW, penH);
		jgraphics_set_source_jrgba(g, &x->b_hisha);
		jgraphics_fill(g);
		
		jgraphics_rectangle(g, wrL - penW, hT-penH, wBG + penW, penH);
		jgraphics_rectangle(g, wrL - penW, hT-penH, penW, hBG+2*penH);
		jgraphics_set_source_jrgba(g, &x->b_dasha);
		jgraphics_fill(g);

		//---------- echelle -------------//

		while(i > 0)
		{
			jgraphics_rectangle(g, penW+1, hT+i*hr-1, wrL-2*penW-1, 1);
			i -= 2;		
		}
		jgraphics_set_source_jrgba(g, &x->b_daaxe);
		jgraphics_fill(g);

		i = 10;
		while(i > 0)
		{
			jgraphics_rectangle(g, penW+1, hT+i*hr, wrL-2*penW-1, 1);
			i -= 2;		
		}
		jgraphics_set_source_jrgba(g, &x->b_hiaxe);
		jgraphics_fill(g);
	}

	//-------------- METER BACKGROUND -----------//
	jgraphics_rectangle(g, wrL, hT, wBG, hBG);
	jgraphics_set_source_jrgba(g, &x->b_frgb);
	jgraphics_fill(g);

	// Tracé de la valeur
	flag = x->b_val / 25.;
	pos = x->b_val * hr * 8./100.;
	rel_pos = fmod(pos, 2*hr);

	switch(flag)
	{
	case 0:
		jgraphics_rectangle(g, wrL, hT+10*hr, wBG, -rel_pos);
		jgraphics_set_source_jrgba(g, &x->b_c1);
		jgraphics_fill(g);
		break;
	case 1:
		jgraphics_rectangle(g, wrL, hT+8*hr, wBG, -rel_pos);
		jgraphics_set_source_jrgba(g, &x->b_c2);
		jgraphics_fill(g);
		jgraphics_rectangle(g, wrL, hT+8*hr, wBG, 2*hr);
		jgraphics_set_source_jrgba(g, &x->b_c1);
		jgraphics_fill(g);
		break;
	case 2:
		jgraphics_rectangle(g, wrL, hT+6*hr, wBG, -rel_pos);
		jgraphics_set_source_jrgba(g, &x->b_c3);
		jgraphics_fill(g);
		jgraphics_rectangle(g, wrL, hT+6*hr, wBG, 2*hr);
		jgraphics_set_source_jrgba(g, &x->b_c2);
		jgraphics_fill(g);
		jgraphics_rectangle(g, wrL, hT+8*hr, wBG, 2*hr);
		jgraphics_set_source_jrgba(g, &x->b_c1);
		jgraphics_fill(g);
		break;
	case 3:
		jgraphics_rectangle(g, wrL, hT+4*hr, wBG, -rel_pos);
		jgraphics_set_source_jrgba(g, &x->b_c4);
		jgraphics_fill(g);
		jgraphics_rectangle(g, wrL, hT+4*hr, wBG, 2*hr);
		jgraphics_set_source_jrgba(g, &x->b_c3);
		jgraphics_fill(g);
		jgraphics_rectangle(g, wrL, hT+6*hr, wBG, 2*hr);
		jgraphics_set_source_jrgba(g, &x->b_c2);
		jgraphics_fill(g);
		jgraphics_rectangle(g, wrL, hT+8*hr, wBG, 2*hr);
		jgraphics_set_source_jrgba(g, &x->b_c1);
		jgraphics_fill(g);
		break;
	default:
	case 4:
		jgraphics_rectangle(g, wrL, hT+2*hr, wBG, -rel_pos);
		jgraphics_set_source_jrgba(g, &x->b_c5);
		jgraphics_fill(g);
		jgraphics_rectangle(g, wrL, hT+2*hr, wBG, 2*hr);
		jgraphics_set_source_jrgba(g, &x->b_c4);
		jgraphics_fill(g);
		jgraphics_rectangle(g, wrL, hT+4*hr, wBG, 2*hr);
		jgraphics_set_source_jrgba(g, &x->b_c3);
		jgraphics_fill(g);
		jgraphics_rectangle(g, wrL, hT+6*hr, wBG, 2*hr);
		jgraphics_set_source_jrgba(g, &x->b_c2);
		jgraphics_fill(g);
		jgraphics_rectangle(g, wrL, hT+8*hr, wBG, 2*hr);
		jgraphics_set_source_jrgba(g, &x->b_c1);
		jgraphics_fill(g);
		break;
	}
	
	// Tracé des peaks
	if(!x->b_levelOut)
	{
		t_float last_max = x->b_maxlast * hr * 8./100.;
		t_float last_max_posh = hT+10*hr-last_max-1;
		t_int last_max_color = x->b_maxlast / 25.;
		last_max_posh = last_max_posh < hT + 4 ? hT + 4 : last_max_posh;
		if  (x->b_val < x->b_maxlast && x->b_maxlast > 0)
		{	
			jgraphics_rectangle(g, wrL, last_max_posh, wBG, 4);
			switch (last_max_color)
			{
			case 0:
				jgraphics_set_source_jrgba(g, &x->b_c1);
				break;
			case 1:
				jgraphics_set_source_jrgba(g, &x->b_c2);
				break;
			case 2:
				jgraphics_set_source_jrgba(g, &x->b_c3);
				break;
			case 3:
				jgraphics_set_source_jrgba(g, &x->b_c4);
				break;
			default:
				jgraphics_set_source_jrgba(g, &x->b_c5);
				break;
			}
			jgraphics_fill(g);
		}
	} else {
		jgraphics_rectangle(g, wrL, hT, wBG, 2*hr);
		jgraphics_set_source_jrgba(g, &x->b_c5);
		jgraphics_fill(g);
	}
		
	if(x->b_sens)
	{	
		if(x->b_showv) 
		{
			// draw value background
			jgraphics_rectangle(g, wrL + pad_value_rectw, hT+hBG+hB-pad_value_recth, wBG-2*pad_value_rectw, hFG);
			jgraphics_set_source_jrgba(g, &x->b_frgb);
			jgraphics_fill(g);
			
			if(x->b_showb)
			{
				// contour value
				jgraphics_rectangle(g, weight - pad_value_rectw - wrR, hT+hBG+hB-pad_value_recth-penH, penW, hFG+2*penH);
				jgraphics_rectangle(g, wrL+pad_value_rectw, height-pad_value_recth, wBG - 2 *pad_value_rectw, penH);
				jgraphics_set_source_jrgba(g, &x->b_hisha);
				jgraphics_fill(g);
			
				jgraphics_rectangle(g, wrL+pad_value_rectw-penW, hT+hBG+hB-pad_value_recth-penH, penW, hFG+2*penH);
				jgraphics_rectangle(g, wrL+pad_value_rectw, hT+hBG+hB-pad_value_recth-penH, wBG - 2*pad_value_rectw, penH);
				jgraphics_set_source_jrgba(g, &x->b_dasha);
				jgraphics_fill(g);
			}
		
//			if(!x->b_sens)
//			{
//				jgraphics_scale(g, -1, 1);
//				jgraphics_translate(g, -rect.height, 0);//-rect.height+rect.width);
//				jgraphics_rotate(g, -JGRAPHICS_PIOVER2);
//				// TODO affichage correct du texte
//				jgraphics_translate(g, -rect.width, -rect.width);
//				t_jmatrix * t = malloc(sizeof(t_jmatrix));
//				t_float rad = -JGRAPHICS_PIOVER2;
//				t_float padx = 0;
//				t_float pady = 0;
//				jgraphics_matrix_init(t, cos(rad), sin(rad), -sin(rad), cos(rad), padx, pady);
//				jgraphics_transform(g, t);
//			}
			// draw dB value (choose dbval, linval, maxdbval ????)
			jf = jfont_create(jbox_get_fontname((t_object *)x)->s_name, jbox_get_font_slant((t_object *)x), jbox_get_font_weight((t_object *)x), jbox_get_fontsize((t_object *)x));
			jtl = jtextlayout_create();
			switch(x->b_showv)
			{
			case 1:
				sprintf(text,"%.2f",x->b_maxvaldB);
				break;
			case 2:
				sprintf(text,"%.2f",x->b_maxval);
				break;
			case 3:
				sprintf(text,"%.2f",x->b_maxlastdB);
				break;
			case 4:
				sprintf(text,"%.2f",x->b_absmaxvaldB);
				break;
			}
//			if(x->b_satur || x->b_levelOut)
//				sprintf(text,"%.2f",x->b_absmaxvaldB);
//			else
//				sprintf(text,"%.2f",x->b_maxvaldB);
			jtextlayout_set(jtl, text, jf, wrL+pad_value_font, hT+hBG+hB-pad_value_recth+pad_value_font, wBG - 2*pad_value_font, hFG - 2*pad_value_font, JGRAPHICS_TEXT_JUSTIFICATION_CENTERED, JGRAPHICS_TEXTLAYOUT_NOWRAP);
			jtextlayout_settextcolor(jtl, &x->b_tcolor); 
			jtextlayout_draw(jtl, g);
			jtextlayout_destroy(jtl);
			jfont_destroy(jf);
		} else {
			// draw ~
			jf = jfont_create(jbox_get_fontname((t_object *)x)->s_name, jbox_get_font_slant((t_object *)x), jbox_get_font_weight((t_object *)x), jbox_get_fontsize((t_object *)x));
			jtl = jtextlayout_create();
			sprintf(text,"~");
			jtextlayout_set(jtl, text, jf, wrL, hT+hBG, wBG, hB, JGRAPHICS_TEXT_JUSTIFICATION_CENTERED, JGRAPHICS_TEXTLAYOUT_NOWRAP);

			jtextlayout_settextcolor(jtl, &x->b_frgb); 
			jtextlayout_draw(jtl, g);
			jtextlayout_destroy(jtl);
			jfont_destroy(jf);
		}
	}

	if(x->b_showf)
	{
		//-----------  tracé ext -----------------//
	
		// TODO FIND A BETTER WAY TO DRAW A SQUARE WITHOUT FILLING IT
		// THAT ISN'T draw_fast -> can not be rotated
//		jgraphics_set_source_jrgba(g, &x->b_frame);
//		jgraphics_rectangle_draw_fast(g, 0, 0, rect.width, rect.height, 1);
		jgraphics_rectangle(g, 0, 0, weight, 1);
		jgraphics_rectangle(g, weight-1, 0, 1, height);
		jgraphics_rectangle(g, 0, height-1, weight, 1);
		jgraphics_rectangle(g, 0, 0, 1, height);
		jgraphics_set_source_jrgba(g, &x->b_frame);
		jgraphics_fill(g);
//		jgraphics_set_line_width(g, 1);
//		jgraphics_move_to(g, 0, 0);
//		jgraphics_line_to(g, weight, 0);
//		jgraphics_line_to(g, weight, height);
//		jgraphics_line_to(g, 0, height);
//		jgraphics_close_path(g);
//		jgraphics_set_source_jrgba(g, &x->b_frame);
	}
}

void bargraf_bang(t_bargraf *x)
{
	// clear
	bargraf_clear(x);
	jbox_redraw((t_jbox *)x);
}

void bargraf_mousedown(t_bargraf *x, t_object *patcherview, t_pt pt, long modifiers)
{
	if(modifiers & eCommandKey)
	{
		if(x->b_drawmode > 0)
		{
			x->b_memdrawmode = x->b_drawmode;
			x->b_drawmode = 0;
		} else {
			x->b_drawmode = x->b_memdrawmode;
		}
	} else {
		bargraf_bang(x);
	}
}

void bargraf_mouseenter(t_bargraf *x, t_object *patcherview, t_pt pt, long modifiers)
{
	x->b_mouseover = 1;
	if(x->b_drawmode == 2)
	{
		x->b_clockstate = 1;
		if(x->b_clock)
			clock_fdelay(x->b_clock, x->b_interval);
	}
	jbox_redraw((t_jbox *)x);
}

void bargraf_mouseleave(t_bargraf *x, t_object *patcherview, t_pt pt, long modifiers)
{
	x->b_mouseover = 0;
	if(x->b_drawmode == 2)
	{
		x->b_clockstate = 0;
		if(x->b_clock)
			clock_unset(x->b_clock);
	}
	jbox_redraw((t_jbox *)x);
}

t_max_err bargraf_setdrawmode(t_bargraf *x, void *attr, long ac, t_atom *av)
{
	if (ac && av) {
		long n = atom_getlong(av);
		x->b_drawmode = n;
		if(x->b_drawmode < 2)
		{
			x->b_clockstate = 1;
			if(x->b_clock)
				clock_fdelay(x->b_clock, x->b_interval);			
		} else if(!x->b_mouseover) {
			x->b_clockstate = 0;
			if(x->b_clock)
				clock_unset(x->b_clock);
		}
	}
	return MAX_ERR_NONE;
}


void bargraf_clear(t_bargraf *x)
{
	x->b_maxlast = 0;
    x->b_absmaxvaldB = -96. ;
    x->b_levelOut = false ;
}

void bargraf_float(t_bargraf *x, double f)
{
	x->b_interval = f < 5 ? 5 : (f > 5000 ? 5000 : f);
}

void bargraf_int(t_bargraf *x, long n)
{
	bargraf_float(x, n);
}

void bargraf_saturation(t_bargraf *x, long n)
{
	x->b_satur = n;
	x->b_levelOut = false;
}

/******* CLOCK STUFF *********/
t_max_err bargraf_setclock(t_bargraf *x, void *attr, long ac, t_atom *av)
{
	if (ac && av) {
		long n = atom_getlong(av);
		if(n > 0)
		{
			x->b_clockstate = 1;
			if(x->b_clock)
				clock_fdelay(x->b_clock, x->b_interval);
		} else {
			if(x->b_clock)
				clock_unset(x->b_clock);
			x->b_clockstate = 0;
		}
		//object_post((t_object *) x, "clock %d", x->b_clockstate);
	}
	return MAX_ERR_NONE;
}

t_float dB2cent(t_float f)
{
	t_float val;
	if (f > 0.) // f est le niveau du signal en dB
	{
//		t_float tmp;
//		val = 101 + ((tmp = (f * 25. / 6.)) < 24. ? tmp : 24);
		val = 101;
	}
	else if (f > -6.)
		val = 75. + ((6. + f) * 25. / 6.);
	else if (f > -18.)
		val = 50. + ((18. + f) * 25. / 12.);
	else if (f > -42.)
		val = 25. + ((42. + f) * 25. / 24.);
	else if (f > -90.)
		val = 1. + ((90. + f) * 24. / 48.);
	else if (f > -96.)
		val = 1;
	else 
		val = 0;
	return val;
}

void bargraf_tick(t_bargraf *x)
{
	t_int n = sys_getdspstate();

	if(n)
	{
		x->b_flag = true;
		x->b_maxvaldB = (x->b_maxval <= 0.000015849 ? -96.0 : (20. * log10(x->b_maxval))) ;
		
		if (x->b_maxvaldB > x->b_absmaxvaldB)
			x->b_absmaxvaldB = x->b_maxvaldB ;
		
		    
		outlet_float(x->b_out1,  x->b_absmaxvaldB);
		outlet_float(x->b_out2, x->b_maxval);
		outlet_float(x->b_out3,  x->b_maxvaldB);
		
		x->b_val = dB2cent(x->b_maxvaldB);
		x->b_maxlast = x->b_val > x->b_maxlast ? x->b_val : x->b_maxlast;
		x->b_maxlastdB = x->b_maxvaldB > x->b_maxlastdB ? x->b_maxvaldB : x->b_maxlastdB;
		x->b_count++;
		
		if (x->b_satur == 1)
		{
			if(x->b_absmaxvaldB > 0.)
			{
				x->b_levelOut = true;
				x->b_count = 0;
			}
		} else {
			x->b_levelOut = x->b_maxlast > 100.;
			if (x->b_count > x->b_countmax)
			{
				x->b_maxlast = x->b_val;
				x->b_maxlastdB = x->b_maxvaldB;
				x->b_count = 0;
			}
		}
		
		jbox_redraw((t_jbox *)x);
		
		if(x->b_clockstate)
			clock_fdelay(x->b_clock, x->b_interval);
	}	
}

/********** DSP STUFF **************/
void bargraf_perform64(t_bargraf *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{	
  	t_int flag;
  	t_int n = sampleframes ;
	t_double *in = (t_double *)(ins[0]);
 
	t_float maxval, minval;
	
	if (x->j_pxjbox.z_disabled || x-> b_clockstate == 0)
		goto out;

	maxval = x->b_maxval;
	minval = -1. * x->b_maxval;
	flag = x->b_flag;

    while (n--) {

 		if(!flag) {
    		minval = (*in < minval ? *in : minval) ; 
    		maxval = (*in > maxval ? *in : maxval) ; 
    		in++;
    	}
    	else
    	{
     		minval = maxval = *in++;
    		flag = false;
    	}
    }
    
	x->b_maxval = (maxval > (minval * -1) ? maxval : (minval * -1));
   	x->b_flag = flag ;
   	
	if (x->b_startclock)
	{
		x->b_startclock = 0;
		clock_fdelay(x->b_clock,0);
	}
	
out: 
    return;
}

void bargraf_dsp64(t_bargraf *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags)
//(t_bargraf *x, t_signal **sp, short *count)
{
	if(count[0])
	{
        object_method(dsp64, gensym("dsp_add64"), x, bargraf_perform64, 0, NULL);
		x->b_startclock = x->b_clockstate;
	} 
}

/************ OTHER STUFF ************/

//void bargraf_getdrawparams(t_bargraf *x, t_object *patcherview, t_jboxdrawparams *params)
//{
//	object_post((t_object *)x, "bargraf_getdrawparams");
//}
