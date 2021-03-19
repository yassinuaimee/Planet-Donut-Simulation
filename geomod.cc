#include <iostream>
using namespace std;
#include <cmath>
#include <array>
#include "geomod.h"



typedef array<double,2> Vecteur;
typedef array<double,2> Point;

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
void normalisation_point(array<double,2> v)//On demande explicitement le fait
                                            //d'avoir une surcharge
{
    normalisation_point(v[0]);
    normalisation_point(v[1]);
}
///======================================================================================================///

double norme_plus_petit_vecteur(Point depart,Point arrivee,Vecteur vecteur={0.,0.})
{
    double distance_min(0.),distance_test(0.);
    double delta_x(0.),delta_y(0.);
    distance_min=sqrt(pow(depart[0]-arrivee[0],2)+pow(depart[1]-arrivee[1],2));
    for(int kx=-1;kx<=1;++kx)
    {
        for(int ky=-1;ky<=1;++ky)
        {
            delta_x=(arrivee[0]+kx*2*max_)-depart[0];
            delta_y=(arrivee[1]+ky*2*max_)-depart[1];
            
            distance_test=sqrt(pow(delta_x,2)+pow(delta_y,2));
            
            if(distance_min>distance_test)
            {
                distance_min=distance_test;
                vecteur[0]=delta_x;/////////////////////////////////// ça marche pas ça c'est de la merde
                vecteur[1]=delta_y;
            }
        }
    }
    return distance_min;
}
///======================================================================================================///

array<double,3> norme_plus_petit_vecteur( const double init_x1, const double init_y1, const double init_x2, const double init_y2 )
{
	array<double,3> norme_et_coordonnees;
	double distance_min(0.);
	double delta_x(0.),delta_y(0.);
	
	delta_x=init_x2-init_x1;
	delta_y=init_y2-init_y1;
	
	distance_min=sqrt( pow(delta_x,2) + pow(delta_y,2) );
	
	double distance_test( distance_min );
	
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
	norme_et_coordonnees[0]=distance_min;
	norme_et_coordonnees[1]=vecteur_x;
	norme_et_coordonnees[2]=vecteur_y;

	return norme_et_coordonnees;
	
}
///======================================================================================================///
bool  test_egalite_points(Point a,Point b)
{
    if(equal_zero(norme_plus_petit_vecteur(a,b)))
        return true;
    else
        return false;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

Cercle::Cercle(Point set_centre, double set_rayon)
{
	centre=set_centre;
	rayon=set_rayon;
}

Point Cercle::get_centre()
{
    return centre;
}

double Cercle::get_rayon()
{
    return rayon;
}
///======================================================================================================///
bool Cercle::point_appartient(Point point)
{
    double n(norme_plus_petit_vecteur(centre,point));
    if(n<rayon-epsilon_zero)
        return true;
    else
        return false;
}
///======================================================================================================///
bool Cercle::intersection_cercle(Cercle cercle)
{
    double  n(norme_plus_petit_vecteur(centre,cercle.get_centre()));
    
    if(n < rayon + cercle.get_rayon() - epsilon_zero)
        return true;
    else
        return false;
}

