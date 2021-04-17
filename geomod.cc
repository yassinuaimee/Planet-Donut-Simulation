/* Nom:
 * geomod.cc
 * 
 * Auteurs:
 * Yassin Al-Nuaimee
 * Adrien Maillet Gonzalez
 * 
 * Version: 10
 * 
 * 
 */
 
#include <iostream>
#include <cmath>
#include <array>
#include "geomod.h"
#include "constantes.h"

//using namespace std;

namespace
{
	double max_(dim_max);
	double epsilon_zero(max_*pow(10,-10));
}



///////////////////////////////////////////////////////////////////////////////////////
/*
POINT
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

//================================================================================//

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
	x=x_;
}

//================================================================================//

void Point::set_y(double y_)
{
	y=y_;
}

//================================================================================//

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


///////////////////////////////////////////////////////////////////////////////////////
/*
VECTEUR
*/
///////////////////////////////////////////////////////////////////////////////////////

Vecteur::Vecteur(double x, double y)
: x(x), y(y)
{
	norme = sqrt(x*x+y*y);
}

//================================================================================//

Vecteur::Vecteur()
: x(0.0), y(0.0), norme(0.0)
{}

//================================================================================//

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

//================================================================================//

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
CERCLE
*/
///////////////////////////////////////////////////////////////////////////////////////

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

//================================================================================//

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

//================================================================================//

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


///////////////////////////////////////////////////////////////////////////////////////
/*
FONCTIONS HORS CLASSES
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
