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
#include <cairomm/context.h>
#include "graphic.h"
#include "gui.h"
#include "constantes.h"

static const Cairo::RefPtr<Cairo::Context>* ptcr(nullptr);


void activate_graphic(const Cairo::RefPtr<Cairo::Context>& cr)
{
    ptcr=&cr;
}

void cercle_noir_dessin(double x, double y, double rayon)
{
    (*ptcr)->set_line_width(1.0);
    (*ptcr)->set_source_rgb(0.1, 0.1, 0.1);
    (*ptcr)->arc(x, y, rayon, 0, 2*M_PI);
    (*ptcr)->fill_preserve();
    (*ptcr)->stroke();
}


void cercle_communication_dessin(double x, double y)
{
    (*ptcr)->set_line_width(5.0);
    (*ptcr)->set_source_rgb(0.7, 0.7, 0.7);//gris
    (*ptcr)->arc(x, y, rayon_comm, 0, 2*M_PI);
    (*ptcr)->stroke();
    
}

void cercle_base_dessin(double x, double y, int indice)
{
    color_base(indice);
    (*ptcr)->set_line_width(2.0);
    (*ptcr)->arc(x, y, 5, 0, 2*M_PI);
    (*ptcr)->stroke();
    (*ptcr)->set_line_width(1.0);
    (*ptcr)->arc(x, y, 1, 0, 2*M_PI);
    (*ptcr)->fill_preserve();
    (*ptcr)->stroke();
}

void cercle_robot_dessin(double x, double y,int indice)
{
    color_base(indice);
    (*ptcr)->set_line_width(1.0);
    (*ptcr)->arc(x, y, 1, 0, 2*M_PI);
    (*ptcr)->fill_preserve();
    (*ptcr)->stroke();
}


void ligne_dessin(double x1, double y1, double x2, double y2)
{
    (*ptcr)->set_line_width(10.0);
    (*ptcr)->set_source_rgb(0.6,0,1);
    (*ptcr)->move_to(x1,y1);
    (*ptcr)->line_to(x1+x2,y1+y2);
    (*ptcr)->stroke();
}

void color_base(int index)
{
    if(index%6==0)
    {
        (*ptcr)->set_source_rgb(1, 0, 0);//rouge ROUGE VERT BLEU
    }
    if(index%6==1)
    {
        (*ptcr)->set_source_rgb(0, 1, 0);//vert
    }
    if(index%6==2)
    {
        (*ptcr)->set_source_rgb(0, 0, 1);//bleu
    }
    if(index%6==3)
    {
        (*ptcr)->set_source_rgb(1, 1, 0);//jaune
    }
    if(index%6==4)
    {
        (*ptcr)->set_source_rgb(1, 0, 1);//magenta
    }
    if(index%6==5)
    {
        (*ptcr)->set_source_rgb(0, 1, 1);//cyan
    }
}
