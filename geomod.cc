/* Nom:
 * geomod.cc
 * 
 * Auteurs:
 * Yassin Al-Nuaimee
 * Adrien Maillet Gonzalez
 * 
 * Version: 12
 * 
 * 
 */
 
#include <iostream>
#include <cmath>
#include <array>
#include "geomod.h"
#include "constantes.h"
#include "graphic.h"


namespace
{
	double max_(dim_max);
	double epsilon_zero(max_*pow(10,-10));
}

void affiche_cercle_gisement(double,double,double);
void affiche_cercle_communication(double, double);
void affiche_cercle_base(double,double, int);
void affiche_cercle_robot(double,double, int);

///////////////////////////////////////////////////////////////////////////////////////
/*
//================================================================================//
 //POINT//
//================================================================================//
*/
///////////////////////////////////////////////////////////////////////////////////////


Point::Point(double x, double y)
: x(normalisation_point(x)), y(normalisation_point(y))
{}
	
//================================================================================//

Point::Point()
: x(0.0), y(0.0)
{}

//================================================================================//

void Point::normalisation()
{
	x=normalisation_point(x);
	y=normalisation_point(y);
}


/*
//================================================================================//
 //TEST D'EGALITE//
//================================================================================//
*/

bool Point::operator==(Point & autre) const
{
	double norme(0.0);
	norme= norme_plus_petit_vecteur( x, y, autre.get_x(), autre.get_y());
    if(equal_zero(norme))
    {
        return true;
    }
    else
    {
        return false;
    }
}

//================================================================================//

bool Point::same_position(Point autre)
{
    double norme(0.0);
    norme= norme_plus_petit_vecteur( x, y, autre.get_x(), autre.get_y());
    if(equal_zero(norme))
    {
        return true;
    }
    else
    {
        return false;
    }
}


/*
//================================================================================//
 //METHODES D'AFFICHAGE//
//================================================================================//
*/

void Point::ligne_reseau(Point p)
{
    std::array<double,2> direction(plus_courte_direction(*this, p));
    
    //Allé retour nécessaire pour que le rebouclement fonctionne correctement
    ligne_dessin(x, y, direction[0], direction[1]);
    ligne_dessin(p.get_x(), p.get_y(), -direction[0], -direction[1]);
    
    
}

//================================================================================//

void Point::cercle_communication()
{
    bool x_plus(false), x_moins(false), y_plus(false), y_moins(false);
    cercle_communication_dessin(x, y);
    if(x+rayon_comm>dim_max)
    {
        cercle_communication_dessin(x-2*dim_max, y);
        x_plus=true;
    }
    if(x-rayon_comm<-dim_max)
    {
        cercle_communication_dessin(x+2*dim_max, y);
        x_moins=true;
    }
    if(y+rayon_comm>dim_max)
    {
        cercle_communication_dessin(x, y-2*dim_max);
        y_plus=true;
    }
    if(y-rayon_comm<-dim_max)
    {
        cercle_communication_dessin(x, y+2*dim_max);
        y_moins=true;
    }
    if(x_plus and y_plus)
    {
        cercle_communication_dessin(x-2*dim_max, y-2*dim_max);
    }
    if(x_plus and y_moins)
    {
        cercle_communication_dessin(x-2*dim_max, y+2*dim_max);
    }
    if(x_moins and y_moins)
    {
        cercle_communication_dessin(x+2*dim_max, y+2*dim_max);
    }
    if(x_moins and y_plus)
    {
        cercle_communication_dessin(x+2*dim_max, y-2*dim_max);
    }
}

void Point::affiche_dessin(int type,int index, bool retour)//Pratique si dans le futur on veut differencier affichage robots
{
    affiche_cercle_robot(x, y, index);
    switch(type)
    {
        case 1://cas du prospecteur
            affiche_symbole_prospecteur(x, y, index);
            break;
        case 2:
            affiche_symbole_forage(x, y, index);
            break;
        case 3:
            affiche_symbole_transport(x, y, index, retour);
            break;
        case 4:
            affiche_symbole_communication(x, y, index);
            break;
        default:
            break;
            
    }
}


/*
//================================================================================//
 //GETTERS & SETTERS//
//================================================================================//
*/

double Point::get_x()
{
    return x;
}

//================================================================================//

double Point::get_y()
{
    return y;
}

//================================================================================//

void Point::set_x(double x_)
{
    if(x_>dim_max)
    {
        x=x_-2*dim_max;
    }
    else if(x_<-dim_max)
    {
        x=x_+2*dim_max;
    }
    else
    {
        x=x_;
    }
    
}

//================================================================================//

void Point::set_y(double y_)
{
    if(y_>dim_max)
    {
        y=y_-2*dim_max;
    }
    else if(y_<-dim_max)
    {
        y=y_+2*dim_max;
    }
    else
    {
        y=y_;
    }
}


///////////////////////////////////////////////////////////////////////////////////////
/*
//================================================================================//
 //VECTEUR//
//================================================================================//
*/
///////////////////////////////////////////////////////////////////////////////////////


/*
//================================================================================//
 //CONSTRUCTEURS//
//================================================================================//
*/

Vecteur::Vecteur(double x, double y)
: x(x), y(y)
{
	norme = sqrt(x*x+y*y);
}

//================================================================================//

Vecteur::Vecteur()
: x(0.0), y(0.0), norme(0.0)
{}


/*
//================================================================================//
 //TEST DE DISTANCES//
//================================================================================//
*/

double Vecteur::norme_plus_petit_vecteur(Point& p1, Point& p2, Vecteur& vecteur)
{
    double distance_min(0.), distance_test(0.);
	double delta_x(0.), delta_y(0.);
	double init_x1(p1.get_x()) ,init_y1(p1.get_y());
	double init_x2(p2.get_x()) ,init_y2(p2.get_y());
	
	delta_x=init_x2-init_x1;
	delta_y=init_y2-init_y1;
	
	distance_min=sqrt(pow(delta_x,2) + pow(delta_y,2));
	
	double vecteur_x(delta_x), vecteur_y(delta_y);
	
    for(int kx=-1;kx<=1;++kx)
    {
        for(int ky=-1;ky<=1;++ky)
        {
            delta_x=(init_x2+kx*2*max_)-init_x1;
            delta_y=(init_y2+ky*2*max_)-init_y1;
            
            distance_test=sqrt(pow(delta_x,2)+pow(delta_y,2));
            
            
            if(distance_min>distance_test)
            {
				distance_min=distance_test;
                
                vecteur_x=delta_x;
				vecteur_y=delta_y;
			}
		}
	}
	vecteur.set_x(vecteur_x);
	vecteur.set_y(vecteur_y);
	return distance_min;
	
}

//================================================================================//

double Vecteur::norme_plus_petit_vecteur(Point& p1, Point& p2) //Surcharge utile
{
	double distance_min(0.), distance_test(0.);
	double delta_x(0.), delta_y(0.);
	double init_x1(p1.get_x()), init_y1(p1.get_y());
	double init_x2(p2.get_x()), init_y2(p2.get_y());
	
	delta_x=init_x2-init_x1;
	delta_y=init_y2-init_y1;
	
	distance_min=sqrt(pow(delta_x,2) + pow(delta_y,2));
	
    for(int kx=-1;kx<=1;++kx)
    {
        for(int ky=-1;ky<=1;++ky)
        {
			delta_x=(init_x2+kx*2*max_)-init_x1;
            delta_y=(init_y2+ky*2*max_)-init_y1;
            
            distance_test=sqrt(pow(delta_x,2)+pow(delta_y,2));
            
            if(distance_min>distance_test)
            {
                distance_min=distance_test;
            }
		}
	}
	return distance_min;
}


/*
//================================================================================//
 //GETTERS & SETTERS//
//================================================================================//
*/

double Vecteur::get_x()
{
	return x;
}

//================================================================================//

double Vecteur::get_y()
{
	return y;
}

//================================================================================//

double Vecteur::get_norme()
{
	return norme;
}

//================================================================================//

void Vecteur::set_x(double x_)
{
	x=x_;
    norme=sqrt(pow(x,2)+pow(y,2));
}

//================================================================================//

void Vecteur::set_y(double y_)
{
	y=y_;
    norme=sqrt(pow(x,2)+pow(y,2));
}


///////////////////////////////////////////////////////////////////////////////////////
/*
//================================================================================//
 //CERCLE//
//================================================================================//
*/
///////////////////////////////////////////////////////////////////////////////////////

/*
//================================================================================//
 //CONSTRUCTEURS//
//================================================================================//
*/

Cercle::Cercle(Point centre, double rayon)
: centre(centre), rayon(rayon)
{}

//================================================================================//

Cercle::Cercle(double x, double y, double rayon)
: centre(x, y), rayon(rayon)
{}

//================================================================================//

Cercle::Cercle()
: centre(0.,0.), rayon(0.0)
{}


/*
//================================================================================//
 //METHODES TESTANT LES DISTANCES//
//================================================================================//
*/

bool Cercle::point_appartient(Point point)
{
    double n(norme_plus_petit_vecteur(centre.get_x(),
                                      centre.get_y(),
                                      point.get_x(),
                                      point.get_y()));
    if(n<rayon-epsilon_zero)
    {
        return true;
        
    }
    else
    {
        return false;
        
    }
}

//================================================================================//

bool Cercle::intersection_cercle(Cercle cercle)
{
    double  n(norme_plus_petit_vecteur(centre.get_x(),
                                       centre.get_y(),
	                                   cercle.centre.get_x(), 
	                                   cercle.centre.get_y()));
    
    if(n < rayon + cercle.get_rayon() - epsilon_zero)
    {
        return true;
        
    }
    else
    {
        return false;
        
    }
}


/*
//================================================================================//
 //MÉTHODES D'AFFICHAGE//
//================================================================================//
*/

void Cercle::affiche_dessin(int type, int index)//Va falloir donner en argument pointeur sur fonction de graphic
{
    double x(centre.get_x()), y(centre.get_y());
    switch (type) {
        case 1:
            affiche_cercle_gisement(x, y, rayon);
            break;
        case 2:
            affiche_cercle_communication(x, y);
            break;
        case 3:
            affiche_cercle_base(x, y, index);
            break;
        default:
            break;
    }
    
}

//================================================================================//

void affiche_dessin(int type, int index, double x, double y, bool retour)
{
    affiche_cercle_robot(x, y, index);
    switch(type)
    {
        case 1://cas du prospecteur
            affiche_symbole_prospecteur(x, y, index);
            break;
        case 2:
            affiche_symbole_forage(x, y, index);
            break;
        case 3:
            affiche_symbole_transport(x, y, index, retour);
            break;
        case 4:
            affiche_symbole_communication(x, y, index);
            break;
        default:
            break;
    }
}

//================================================================================//

void affiche_cercle_gisement(double x, double y, double rayon)
{
    bool x_plus(false), x_moins(false), y_plus(false), y_moins(false);
    cercle_noir_dessin(x, y, rayon);
    if(x+rayon>dim_max)
    {
        cercle_noir_dessin(x-2*dim_max, y, rayon);
        x_plus=true;
    }
    if(x-rayon<-dim_max)
    {
        cercle_noir_dessin(x+2*dim_max, y, rayon);
        x_moins=true;
    }
    if(y+rayon>dim_max)
    {
        cercle_noir_dessin(x, y-2*dim_max, rayon);
        y_plus=true;
    }
    if(y-rayon<-dim_max)
    {
        cercle_noir_dessin(x, y+2*dim_max, rayon);
        y_moins=true;
    }
    if(x_plus and y_plus)
    {
        cercle_noir_dessin(x-2*dim_max, y-2*dim_max, rayon);
    }
    if(x_plus and y_moins)
    {
        cercle_noir_dessin(x-2*dim_max, y+2*dim_max, rayon);
    }
    if(x_moins and y_moins)
    {
        cercle_noir_dessin(x+2*dim_max, y+2*dim_max, rayon);
    }
    if(x_moins and y_plus)
    {
        cercle_noir_dessin(x+2*dim_max, y-2*dim_max, rayon);
    }
}

void affiche_cercle_communication(double x, double y)
{
    bool x_plus(false), x_moins(false), y_plus(false), y_moins(false);
    cercle_communication_dessin(x, y);
    if(x+rayon_comm>dim_max)
    {
        cercle_communication_dessin(x-2*dim_max, y);
        x_plus=true;
    }
    if(x-rayon_comm<-dim_max)
    {
        cercle_communication_dessin(x+2*dim_max, y);
        x_moins=true;
    }
    if(y+rayon_comm>dim_max)
    {
        cercle_communication_dessin(x, y-2*dim_max);
        y_plus=true;
    }
    if(y-rayon_comm<-dim_max)
    {
        cercle_communication_dessin(x, y+2*dim_max);
        y_moins=true;
    }
    if(x_plus and y_plus)
    {
        cercle_communication_dessin(x-2*dim_max, y-2*dim_max);
    }
    if(x_plus and y_moins)
    {
        cercle_communication_dessin(x-2*dim_max, y+2*dim_max);
    }
    if(x_moins and y_moins)
    {
        cercle_communication_dessin(x+2*dim_max, y+2*dim_max);
    }
    if(x_moins and y_plus)
    {
        cercle_communication_dessin(x+2*dim_max, y-2*dim_max);
    }
}

//================================================================================//

void affiche_cercle_base(double x, double y, int index)
{
    cercle_base_dessin(x, y, index);
}

//================================================================================//

void affiche_cercle_robot(double x, double y, int index)
{
    cercle_robot_dessin(x, y, index);
}

/*
//================================================================================//
 //GETTERS//
//================================================================================//
*/

Point Cercle::get_centre()
{
    return centre;
}

//================================================================================//

double Cercle::get_rayon()
{
    return rayon;
}

//================================================================================//

double Cercle::get_x()
{
    return centre.get_x();
}

//================================================================================//

double Cercle::get_y()
{
    return centre.get_y();
}


///////////////////////////////////////////////////////////////////////////////////////
/*
//================================================================================//
 //FONCTIONS HORS CLASSES//
//================================================================================//
*/
///////////////////////////////////////////////////////////////////////////////////////

double normalisation_point(double v)//Utilisée par toutes les autres
{
	while(v>max_ or v<-max_)
	{
		if(v>max_)
		{
			v=v-(2*max_);
		}
		else if(v<-max_)
		{
			v=v+(2*max_);
		}
	}
    return v;
}

//================================================================================//

void normalisation_point(std::array<double,2> v)//Surcharge demadée dans la consigne
{
    normalisation_point(v[0]);
    normalisation_point(v[1]);
}

//================================================================================//

bool equal_zero(double parametre)//Permet de faire le test d'égalité pour les doubles
{
    if(abs(parametre)<epsilon_zero)
    {
        return true;
    }
    else
    {
        return false;
    }
}

//================================================================================//

void set_max(double init_max)//Va donner la taille de notre "planète"
{
	max_=init_max;
	
	if(max_<=0)//Vérifier que la valeur de max a du sens
	{
		exit(0);
	}
    epsilon_zero=max_*pow(10,-10);//Definir la précision de notre "0"
}

//================================================================================//

double get_max()
{
    return max_;
}
//================================================================================//

double get_epsilon_zero()
{
    return epsilon_zero;
}

//================================================================================//

double norme_plus_petit_vecteur( const double init_x1, 
								 const double init_y1, 
								 const double init_x2,
								 const double init_y2 )
{
	double distance_min(0.);
	double delta_x(0.), delta_y(0.);
	
	delta_x=init_x2-init_x1;
	delta_y=init_y2-init_y1;
	
	distance_min=sqrt(pow(delta_x,2) + pow(delta_y,2));
	
	double distance_test(distance_min);
	
	for(int kx=-1; kx<=1; ++kx)
    {
        for(int ky=-1; ky<=1; ++ky)
        {
			delta_x=(init_x2+kx*2*max_)-init_x1;
            delta_y=(init_y2+ky*2*max_)-init_y1;
            
            distance_test=sqrt(pow(delta_x,2)+pow(delta_y,2));
            
            if(distance_min>distance_test)
            {
                distance_min=distance_test;
            }
		}
	}
	return distance_min;
}

//================================================================================//

std::array<double,2> plus_courte_direction(Point & p1, Point & p2)
{
    std::array<double,2> direction({0,0});
    double distance_min(0.), distance_test(0.);
    double delta_x(0.), delta_y(0.);
    double init_x1(p1.get_x()), init_y1(p1.get_y());
    double init_x2(p2.get_x()), init_y2(p2.get_y());
    
    delta_x=init_x2-init_x1;
    delta_y=init_y2-init_y1;
    direction[0]=delta_x;
    direction[1]=delta_y;
    distance_min=sqrt(pow(delta_x,2) + pow(delta_y,2));
    
    for(int kx=-1;kx<=1;++kx)
    {
        for(int ky=-1;ky<=1;++ky)
        {
            delta_x=(init_x2+kx*2*max_)-init_x1;
            delta_y=(init_y2+ky*2*max_)-init_y1;
            
            distance_test=sqrt(pow(delta_x,2)+pow(delta_y,2));
            
            if(distance_min>distance_test)
            {
                direction[0]=delta_x;
                direction[1]=delta_y;
                distance_min=distance_test;
            }
        }
    }
    return direction;
}


std::array<double,3> plus_court_deplacement(Point & p1, Point & p2)
{
    std::array<double,3> direction({0,0,0});
    double distance_min(0.), distance_test(0.);
    double delta_x(0.), delta_y(0.);
    double init_x1(p1.get_x()), init_y1(p1.get_y());
    double init_x2(p2.get_x()), init_y2(p2.get_y());
    
    delta_x=init_x2-init_x1;
    delta_y=init_y2-init_y1;
    direction[0]=delta_x;
    direction[1]=delta_y;
    distance_min=sqrt(pow(delta_x,2) + pow(delta_y,2));
    
    for(int kx=-1;kx<=1;++kx)
    {
        for(int ky=-1;ky<=1;++ky)
        {
            delta_x=(init_x2+kx*2*max_)-init_x1;
            delta_y=(init_y2+ky*2*max_)-init_y1;
            
            distance_test=sqrt(pow(delta_x,2)+pow(delta_y,2));
            
            if(distance_min>distance_test)
            {
                direction[0]=delta_x;
                direction[1]=delta_y;
                distance_min=distance_test;
            }
        }
    }
    
    direction[2]=distance_min;
    return direction;
}
