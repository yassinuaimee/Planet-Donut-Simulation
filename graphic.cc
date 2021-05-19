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

/*
//================================================================================//
 //CERLCES//
//================================================================================//
*/

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
    (*ptcr)->arc(x, y, 40, 0, 2*M_PI);
    (*ptcr)->stroke();
    (*ptcr)->set_line_width(1.0);
    (*ptcr)->arc(x, y, 10, 0, 2*M_PI);
    (*ptcr)->fill_preserve();
    (*ptcr)->stroke();
}

void cercle_robot_dessin(double x, double y,int indice)
{
    color_base(indice);
    (*ptcr)->set_line_width(1.0);
    (*ptcr)->arc(x, y, 10, 0, 2*M_PI);
    (*ptcr)->fill_preserve();
    (*ptcr)->stroke();
}

/*
//================================================================================//
 //SYMBOLES ROBOTS//
//================================================================================//
*/

void affiche_symbole_prospecteur(double x, double y, int index)
{
    color_base(index);
    (*ptcr)->set_line_width(5.0);
    (*ptcr)->move_to(x-10,y+15);
    (*ptcr)->line_to(x+10,y+35);
    (*ptcr)->move_to(x+10,y+15);
    (*ptcr)->line_to(x-10,y+35);
    (*ptcr)->stroke();
}

void affiche_symbole_forage(double x, double y, int index)
{
    color_base(index);
    (*ptcr)->set_line_width(5.0);
    (*ptcr)->move_to(x,y+15);
    (*ptcr)->line_to(x,y-15);
    (*ptcr)->move_to(x-15,y);
    (*ptcr)->line_to(x+15,y);
    (*ptcr)->move_to(x-10,y-10);
    (*ptcr)->line_to(x+10,y+10);
    (*ptcr)->move_to(x-10,y+10);
    (*ptcr)->line_to(x+10,y-10);
    (*ptcr)->stroke();
}

void affiche_symbole_transport(double x, double y, int index, bool retour)
{
    color_base(index);
    (*ptcr)->set_line_width(5.0);
    (*ptcr)->rectangle(x-15, y-15, 30, 30);
    (*ptcr)->stroke();
    if(retour)
    {
        (*ptcr)->rectangle(x-10, y-10, 20, 20);
        (*ptcr)->set_source_rgb(0.1,0.1,0.1);
        (*ptcr)->fill_preserve();
        (*ptcr)->stroke();
    }
    
   
}

void affiche_symbole_communication(double x, double y, int index)
{
    color_base(index);
    (*ptcr)->set_line_width(5.0);
    (*ptcr)->arc(x, y+10, 10, M_PI/4, 3*M_PI/4);
    (*ptcr)->stroke();
    (*ptcr)->arc(x, y+15, 15, M_PI/4, 3*M_PI/4);
    (*ptcr)->stroke();
    (*ptcr)->arc(x, y+10, 20, M_PI/4, 3*M_PI/4);
    (*ptcr)->stroke();
    
}

/*
//================================================================================//
 //LIGNES DROITES//
//================================================================================//
*/

void ligne_dessin(double x1, double y1, double x2, double y2)
{
    (*ptcr)->set_line_width(5.0);
    (*ptcr)->set_source_rgb(0.6,0,1);
    (*ptcr)->move_to(x1,y1);
    (*ptcr)->line_to(x1+x2,y1+y2);
    (*ptcr)->stroke();
}

/*
//================================================================================//
 //COULEURS DE LA BASE//
//================================================================================//
*/

void color_base(int index)
{
    switch(index%6)
    {
        case 0:
            (*ptcr)->set_source_rgb(1, 0, 0);//rouge ROUGE VERT BLEU
            break;
        case 1:
            (*ptcr)->set_source_rgb(0, 1, 0);//vert
            break;
        case 2:
            (*ptcr)->set_source_rgb(0, 0, 1);//bleu
            break;
        case 3:
            (*ptcr)->set_source_rgb(1, 1, 0);//jaune
            break;
        case 4:
            (*ptcr)->set_source_rgb(1, 0, 1);//magenta
            break;
        case 5:
            (*ptcr)->set_source_rgb(0, 1, 1);//cyan
            break;
        default:
            (*ptcr)->set_source_rgb(0.1, 0.1, 0.1);//cyan
            break;
    }
}
