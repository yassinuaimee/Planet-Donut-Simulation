#include <iostream>
using namespace std;
#include <cmath>
#include <array>
#include "geomod.h"



namespace
{
double epsilon_zero;
double max_(0);
}

///======================================================================================================///
bool equal_zero(double parametre)//Permet de faire le test d'égalité pour les types double
{
    if(abs(parametre)<epsilon_zero)
        return true;
    else
        return false;
}
///======================================================================================================///
void set_max(double init_max)//Va donner la taille de notre "planète"
{
	max_=init_max;//C'est maxi dangereux ça je crois qu'il faut pas faire ça
	
	if(max_<=0)//Si la valeur de max_ n'est pas bonne alors on doit sortir car c'est pas bon
	{
		exit(0);
	}
    epsilon_zero=max_*pow(10,-10);//Va pouvoir definir la précision de notre 0
}
///======================================================================================================///
double get_max()//Getter pour avoir accès à la valeur de max hors du module
{
    return max_;
}
///======================================================================================================///
double get_epsilon_zero()//Getter pour avoir accès à la valeur de epsilon_zero hors du module
{
    return epsilon_zero;
}

///======================================================================================================///

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
POINT
*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////


Point::Point(double x, double y)
	: x( normalisation_point(x) ) , y( normalisation_point(y) )
	{}
	
Point::Point()
	: x(0.0) , y(0.0)
	{}
	
void Point::normalisation()
{
	x=normalisation_point(x);//Pas sur qu'on a le droit d'utiliser fonction à l'extérieur
	y=normalisation_point(y);
}

double Point::get_x()
{
	return x;
}

double Point::get_y()
{
	return y;
}

void Point::set_x(double x_)
{
	x=x_;
}

void Point::set_y(double y_)
{
	y=y_;
}

bool Point::operator==(Point & autre) const
{
	double norme(0.0);
	norme= norme_plus_petit_vecteur( x ,y , autre.get_x() , autre.get_y() );
	if(equal_zero(norme))
		return true;
	else
		return false;
}
///======================================================================================================///

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
VECTEUR
*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
Vecteur::Vecteur(double x_ , double y_)
: x(x_) , y(y_)
{
	norme = sqrt(x*x+y*y);
}

Vecteur::Vecteur()
: x(0.0) ,y(0.0) , norme(0.0)
{}

double Vecteur::norme_plus_petit_vecteur( Point& p1 , Point& p2 , Vecteur& vecteur)
{
	double distance_min(0.),distance_test(0.);
	double delta_x(0.),delta_y(0.);
	double init_x1(p1.get_x() ) , init_y1(p1.get_y());
	double init_x2(p2.get_x() ) , init_y2(p2.get_y());
	
	delta_x=init_x2-init_x1;
	delta_y=init_y2-init_y1;
	
	distance_min=sqrt( pow(delta_x,2) + pow(delta_y,2) );
	

	double vecteur_x( delta_x ), vecteur_y( delta_y );
	
	
	 for(int kx=-1;kx<=1;++kx)
    {
        for(int ky=-1;ky<=1;++ky)
        {
			
			delta_x=(init_x2+kx*2*max_)-init_x1;
            delta_y=(init_y2+ky*2*max_)-init_x1;
            
            
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
	vecteur.set_norme(distance_min);
	return distance_min;
	
}

double Vecteur::norme_plus_petit_vecteur( Point& p1 , Point& p2 ) //Fonction de surcharge qui a besoin que de deux arguments
{
	double distance_min(0.),distance_test(0.);
	double delta_x(0.),delta_y(0.);
	double init_x1(p1.get_x() ) , init_y1(p1.get_y());
	double init_x2(p2.get_x() ) , init_y2(p2.get_y());
	
	delta_x=init_x2-init_x1;
	delta_y=init_y2-init_y1;
	
	distance_min=sqrt( pow(delta_x,2) + pow(delta_y,2) );
	
	
	
	 for(int kx=-1;kx<=1;++kx)
    {
        for(int ky=-1;ky<=1;++ky)
        {
			
			delta_x=(init_x2+kx*2*max_)-init_x1;
            delta_y=(init_y2+ky*2*max_)-init_x1;
            
            
            distance_test=sqrt(pow(delta_x,2)+pow(delta_y,2));
            
            
             if(distance_min>distance_test)
            {
				
				distance_min=distance_test;
            
			}
		}
	}
	return distance_min;
	
}

double Vecteur::get_x()
{
	return x;
}

double Vecteur::get_y()
{
	return y;
}

double Vecteur::get_norme()
{
	return norme;
}

void Vecteur::set_x( double x_)
{
	x=x_;
}

void Vecteur::set_y( double y_)
{
	y=y_;
}

void Vecteur::set_norme(double norme_)
{
	norme=norme_;
}


///======================================================================================================///
double normalisation_point(double v)//Permet de remettre les points qui sont hors du graph
                          //de nouveau dans le graph
{
	while( v>max_ or v<-max_)
	{
		if(v>max_)
		{
			v=v-(2*max_);
		}
		else if (v<-max_)
		{
			v=v+(2*max_);
		}
    
	}
	return v;
}
///======================================================================================================///
void normalisation_point(array<double,2> v)//On demande explicitement le fait  d'avoir une surcharge dans la consigne
{
    normalisation_point(v[0]);
    normalisation_point(v[1]);
}
///======================================================================================================///


double norme_plus_petit_vecteur( const double init_x1, const double init_y1, 
                                 const double init_x2, const double init_y2 )
{
	double distance_min(0.);
	double delta_x(0.),delta_y(0.);
	
	delta_x=init_x2-init_x1;
	delta_y=init_y2-init_y1;
	
	distance_min=sqrt( pow(delta_x,2) + pow(delta_y,2) );
	
	double distance_test( distance_min );
	

	 for(int kx=-1;kx<=1;++kx)
    {
        for(int ky=-1;ky<=1;++ky)
        {
			
			delta_x=(init_x2+kx*2*max_)-init_x1;
            delta_y=(init_y2+ky*2*max_)-init_x1;
            
            
            distance_test=sqrt(pow(delta_x,2)+pow(delta_y,2));
            
            
             if(distance_min>distance_test)
            {
				
				distance_min=distance_test;
            
				
			}
		}
	}
	
	return distance_min;
	
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
CERCLE
*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////



Cercle::Cercle(Point centre, double rayon)
: centre( centre ) , rayon(rayon) 
{}
Cercle::Cercle()
: centre( Point(0.0 , 0.0) ) , rayon(0.0)
{}


Point Cercle::get_centre()
{
    return centre;
}

double Cercle::get_rayon()
{
    return rayon;
}
double Cercle::get_centre_x()
{
	return centre.get_x();
}


double Cercle::get_centre_y()
{
	return centre.get_y();
}

///======================================================================================================///
bool Cercle::point_appartient(Point point )
{
    double n(norme_plus_petit_vecteur( centre.get_x(), centre.get_y(), point.get_x(), 
                                       point.get_y() ));
    if(n<rayon-epsilon_zero)
        return true;
    else
        return false;
}
///======================================================================================================///
bool Cercle::intersection_cercle(Cercle cercle)
{
    double  n(norme_plus_petit_vecteur(centre.get_x(), centre.get_y(), 
	                                   cercle.centre.get_x(), 
	                                   cercle.centre.get_y() ) );
    
    if(n < rayon + cercle.get_rayon() - epsilon_zero)
        return true;
    else
        return false;
}

