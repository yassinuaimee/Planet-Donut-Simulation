/* Nom:
 * graphic.cc
 *
 * Auteurs:
 * Yassin Al-Nuaimee
 * Adrien Maillet Gonzalez
 *
 * Version: 2
 *
 *
 */
#include <iostream>
#include <cmath>
#include "graphic.h"
#include "gui.h"
#include "constantes.h"


void cercle_dessin(double x, double y, double rayon,
             const Cairo::RefPtr<Cairo::Context>& cr)
{
    cr->arc(x, y, rayon, 0, 2*M_PI);
    cr->fill_preserve();
    cr->stroke();
}


void cercle_communication_dessin(double x, double y, const Cairo::RefPtr<Cairo::Context>& cr)
{
    cr->set_line_width(5.0);
    cr->set_source_rgb(0.7, 0.7, 0.7);//gris
    cr->arc(x, y, rayon_comm, 0, 2*M_PI);
    cr->stroke();
    
}

void ligne_dessin(double x1, double y1, double x2, double y2,
                  const Cairo::RefPtr<Cairo::Context>& cr)
{
    cr->set_line_width(10.0);
    cr->set_source_rgb(0.6,0,1);
    cr->move_to(x1,y1);
    cr->line_to(x1+x2,y1+y2);
    cr->stroke();
}

void color_base(int index, const Cairo::RefPtr<Cairo::Context>& cr)
{
    if(index%6==0)
    {
        cr->set_source_rgb(1, 0, 0);//rouge ROUGE VERT BLEU
    }
    if(index%6==1)
    {
        cr->set_source_rgb(0, 1, 0);//vert
    }
    if(index%6==2)
    {
        cr->set_source_rgb(0, 0, 1);//bleu
    }
    if(index%6==3)
    {
        cr->set_source_rgb(1, 1, 0);//jaune
    }
    if(index%6==4)
    {
        cr->set_source_rgb(1, 0, 1);//magenta
    }
    if(index%6==5)
    {
        cr->set_source_rgb(0, 1, 1);//cyan
    }
}
