#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "gisement.h"
#include "message.h"
#include "geomod.h"
#include "constantes.h"

using namespace std;


//===================================================================================//
Gisement decodage_ligne_gisement(string line)
{
    istringstream data(line);
    double x(0.), y(0.), rayon(0.), capacite(0.);
    data>> x >> y >> rayon >> capacite;
    Gisement gisement(x, y, rayon, capacite);
    return gisement;
    
};


//definition de Gisement
Gisement::Gisement(double x, double y, double rayon, double capacite)//On verifie direct dedans que les autres gisements n'intersectent pas
: field( x, y, rayon), capacite(capacite)
{
	if(capacite<0)
		exit(0);
	if (rayon<rayon_min or rayon>rayon_max)
        exit(0);
	
}


Gisement::Gisement()
: field(0.0, 0.0, 0.0), capacite(0.0)
{}

Cercle Gisement::get_field()
{
	return field;
}

double Gisement::get_centre_x()
{
	return field.get_centre_x();
}

double Gisement::get_centre_y()
{
	return field.get_centre_y();
}

double Gisement::get_rayon()
{
	return field.get_rayon();
}

