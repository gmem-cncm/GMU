#include "ext.h"
#include "ext_user.h"
#include "jpatcher_api.h"
#include "jgraphics.h"

#define _SETSYM(ap, x) ((ap).a_type = A_SYM, (ap).a_w.w_sym = (x))
#define _SETLONG(ap, x) ((ap).a_type = A_LONG, (ap).a_w.w_long = (x))
#define _SETFLOAT(ap, x) ((ap).a_type = A_FLOAT, (ap).a_w.w_float = (x))

typedef struct _idleubutton
{
	t_jbox b_box;
    long b_state;
    long b_inside;
    // options
    long b_hilite;
    long b_toggle;
    long b_idle;
    long b_drawborder;
    // colors
    t_jrgba c_bg;
    t_jrgba c_border;
    t_jrgba c_hilite;
    // outputs
    t_atom  out_list[2];
	void *o_down;
	void *o_up;
	void *o_drag;
    void *o_inside;
	void *o_idle;
} t_idleubutton;

static t_class *idleubutton_class;

void *idleubutton_new(t_symbol *s, long argc, t_atom *argv);
void idleubutton_free(t_idleubutton *x);
void idleubutton_assist(t_idleubutton *x, void *b, long m, long a, char *s);
void idleubutton_paint(t_idleubutton *x, t_object *patcherview);
void idleubutton_bang(t_idleubutton *x);

void idleubutton_mousedown(t_idleubutton *x, t_object *patcherview, t_pt pt, long modifiers);
void idleubutton_mousedrag(t_idleubutton *x, t_object *patcherview, t_pt pt, long modifiers);
void idleubutton_mouseup(t_idleubutton *x, t_object *patcherview, t_pt pt, long modifiers);
//void idleubutton_mouseenter(t_idleubutton *x, t_object *patcherview, t_pt pt, long modifiers);
//void idleubutton_mouseleave(t_idleubutton *x, t_object *patcherview, t_pt pt, long modifiers);
void idleubutton_mousemove(t_idleubutton *x, t_object *patcherview, t_pt pt, long modifiers);

void inside(t_idleubutton *x, t_pt pt, t_object *patcherview);

int C74_EXPORT main(void)
{
    t_class * c = class_new("idleubutton", (method)idleubutton_new, (method)idleubutton_free, sizeof(t_idleubutton), 0L, A_GIMME, 0);
    
    c->c_flags |= CLASS_FLAG_NEWDICTIONARY;
    jbox_initclass(c, JBOX_COLOR);

	class_addmethod(c,  (method)idleubutton_assist,       "assist",	A_CANT, 0);
    class_addmethod(c,  (method)idleubutton_mousedown,	"mousedown",	A_CANT, 0);
    class_addmethod(c,  (method)idleubutton_mousedrag,	"mousedrag",	A_CANT, 0);
    class_addmethod(c,  (method)idleubutton_mouseup,      "mouseup",      A_CANT, 0);
    //class_addmethod(c,  (method)idleubutton_mouseenter,	"mouseenter",	A_CANT, 0);
    //class_addmethod(c,  (method)idleubutton_mouseleave,	"mouseleave",	A_CANT, 0);
    class_addmethod(c,  (method)idleubutton_mousemove,	"mousemove",	A_CANT, 0);
    class_addmethod(c,  (method)idleubutton_assist,		"assist",       A_CANT, 0);
    class_addmethod(c,  (method)idleubutton_paint,		"paint",	A_CANT, 0);
    
    CLASS_ATTR_CHAR(c, "hilite", 0, t_idleubutton, b_hilite);
    CLASS_ATTR_STYLE_LABEL(c, "hilite", 0, "onoff", "Hilite button when clicked");
    CLASS_ATTR_DEFAULT_SAVE_PAINT(c, "hilite", 0, "1");
    
    CLASS_ATTR_CHAR(c, "toggle", 0, t_idleubutton, b_toggle);
    CLASS_ATTR_STYLE_LABEL(c, "toggle", 0, "onoff", "Toggle Mode");
    CLASS_ATTR_DEFAULT_SAVE_PAINT(c, "toggle", 0, "0");
    
    CLASS_ATTR_CHAR(c, "idletrack", 0, t_idleubutton, b_idle);
    CLASS_ATTR_STYLE_LABEL(c, "idletrack", 0, "onoff", "Idle Mouse");
    CLASS_ATTR_DEFAULT_SAVE_PAINT(c, "idletrack", 0, "0");
    
    CLASS_ATTR_CHAR(c, "drawborder", 0, t_idleubutton, b_drawborder);
    CLASS_ATTR_STYLE_LABEL(c, "drawborder", 0, "onoff", "Draw Border");
    CLASS_ATTR_DEFAULT_SAVE_PAINT(c, "drawborder", 0, "0");
    
    CLASS_ATTR_RGBA(c, "color", 0, t_idleubutton, c_bg);
    CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "color", 0, "1. 1. 1. 0.");
    CLASS_ATTR_STYLE_LABEL(c,"color",0,"rgba","Background Color");
    
    CLASS_ATTR_RGBA(c, "bordercolor", 0, t_idleubutton, c_border);
    CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "bordercolor", 0, "0.5 0.5 0.5 1.");
    CLASS_ATTR_STYLE_LABEL(c,"bordercolor",0,"rgba","Border Color");
    
    CLASS_ATTR_RGBA(c, "hilitecolor", 0, t_idleubutton, c_hilite);
    CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "hilitecolor", 0, "0.6 0.5 0. 0.3");
    CLASS_ATTR_STYLE_LABEL(c,"hilitecolor",0,"rgba","Hilite Color");
    
    CLASS_ATTR_DEFAULT(c,"patching_rect",0, "0. 0. 20. 20.");

    class_register(CLASS_BOX, c);
    idleubutton_class = c;
    
    post("idleubutton 64bit / Build LF - %s %s",__DATE__,__TIME__);

    return 0;
}

void *idleubutton_new(t_symbol *s, long argc, t_atom *argv)
{
    t_idleubutton *x = NULL;
    t_dictionary *d = NULL;
    long boxflags;
    
    if (!(d = object_dictionaryarg(argc,argv)))
        return NULL;
    
    x = (t_idleubutton *)object_alloc(idleubutton_class);
    boxflags = 0
    | JBOX_DRAWFIRSTIN
    | JBOX_NODRAWBOX
    | JBOX_DRAWINLAST
    | JBOX_TRANSPARENT
    | JBOX_GROWBOTH
    | JBOX_DRAWBACKGROUND
    ;
    
    jbox_new((t_jbox *)x, boxflags, argc, argv);
    x->b_box.b_firstin = (void *)x;
    x->o_idle = listout((t_object *)x);
    x->o_inside = intout((t_object *)x);
    x->o_drag = listout((t_object *)x);
    x->o_down = bangout((t_object *)x);
    x->o_up = bangout((t_object *)x);
    x->b_state = 0;
    x->b_inside = 0;
    attr_dictionary_process(x,d);
    jbox_ready((t_jbox *)x);
    return x;
}

void idleubutton_free(t_idleubutton *x)
{
    jbox_free((t_jbox *)x);
}

void idleubutton_assist(t_idleubutton *x, void *b, long m, long a, char *s)
{
    switch(m) {
        case 1: // inlet
            sprintf(s, "parameters");
            break;
        case 2: // outlet
            switch(a) {
                case 0:
                    sprintf(s, "bang on mouseup");
                    break;
                case 1:
                    sprintf(s, "bang on mousedown");
                    break;
                case 2:
                    sprintf(s, "drag coordinates");
                    break;
                case 3:
                    sprintf(s, "inside flag when dragging");
                    break;
                case 4:
                    sprintf(s, "idle coordinates");
                    break;
            }
            break;
    }
}

void idleubutton_paint(t_idleubutton *x, t_object *patcherview)
{
    t_rect rect;
    t_jgraphics *g = (t_jgraphics*) patcherview_get_jgraphics(patcherview);		// obtain graphics context
    jbox_get_rect_for_view((t_object *)x, patcherview, &rect);
    
    jgraphics_set_source_jrgba(g, (x->b_hilite && x->b_state == 1) ? &x->c_hilite : &x->c_bg);
    jgraphics_rectangle(g, 0., 0., rect.width, rect.height);
    jgraphics_fill(g);
    
    if(x->b_drawborder)
    {
        // paint outline
        jgraphics_set_source_jrgba(g, &x->c_border);
        jgraphics_set_line_width(g, 1.);
        jgraphics_rectangle(g, 0., 0., rect.width, rect.height);
        jgraphics_stroke(g);
    }
}

void idleubutton_mousedown(t_idleubutton *x, t_object *patcherview, t_pt pt, long modifiers)
{
    if(x->b_toggle)
    {
        if(x->b_state == 1)
            x->b_state = 0;
        else
            x->b_state = 1;
    } else {
        x->b_state = 1;
    }
    inside(x, pt, patcherview);
    _SETFLOAT(x->out_list[0], pt.x);
    _SETFLOAT(x->out_list[1], pt.y);
    outlet_list(x->o_drag, NULL, 2, x->out_list);
    outlet_bang(x->o_down);
    jbox_redraw((t_jbox *)x);
}

void idleubutton_mousedrag(t_idleubutton *x, t_object *patcherview, t_pt pt, long modifiers)
{
    _SETFLOAT(x->out_list[0], pt.x);
    _SETFLOAT(x->out_list[1], pt.y);
    outlet_list(x->o_drag, NULL, 2, x->out_list);
    inside(x, pt, patcherview);
    jbox_redraw((t_jbox *)x);
}
void idleubutton_mouseup(t_idleubutton *x, t_object *patcherview, t_pt pt, long modifiers)
{
    if(!x->b_toggle)
        x->b_state = 0;
    _SETFLOAT(x->out_list[0], pt.x);
    _SETFLOAT(x->out_list[1], pt.y);
    outlet_list(x->o_drag, NULL, 2, x->out_list);
    inside(x, pt, patcherview);
    outlet_bang(x->o_up);
    jbox_redraw((t_jbox *)x);
}

/*
void idleubutton_mouseenter(t_idleubutton *x, t_object *patcherview, t_pt pt, long modifiers)
{
    x->b_inside = 1;
}

void idleubutton_mouseleave(t_idleubutton *x, t_object *patcherview, t_pt pt, long modifiers)
{
    x->b_inside = 0;
}//*/

void idleubutton_mousemove(t_idleubutton *x, t_object *patcherview, t_pt pt, long modifiers)
{
    if(x->b_idle)
    {
        _SETFLOAT(x->out_list[0], pt.x);
        _SETFLOAT(x->out_list[1], pt.y);
        outlet_list(x->o_idle, NULL, 2, x->out_list);
    }
}

void inside(t_idleubutton *x, t_pt pt, t_object *patcherview)
{
    if(patcherview_get_presentation(patcherview))
    {
        if(pt.x <= 0 || pt.y <= 0 || pt.x >= x->b_box.b_presentation_rect.width || pt.y >= x->b_box.b_presentation_rect.height)
        {
            x->b_inside = 0;
        } else {
            x->b_inside = 1;
        }
    } else {
        if(pt.x <= 0 || pt.y <= 0 || pt.x >= x->b_box.b_patching_rect.width || pt.y >= x->b_box.b_patching_rect.height)
        {
            x->b_inside = 0;
        } else {
            x->b_inside = 1;
        }
    }
    outlet_int(x->o_inside, x->b_inside);
}
