#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "gisement.h"
#include "message.h"
#include "geomod.h"

using namespace std;


//===================================================================================//
//definition de Gisement
Gisement::Gisement(double x, double y, double rayon, double capacite, vector<Gisement> & Eg)//On verifie direct dedans que les autres gisements n'intersectent pas
: field( x, y, rayon), capacite(capacite)
{
	if(capacite<0)
		exit(0);
	
	for(size_t i(0); i<Eg.size(); ++i)
	{
		if(field.intersection_cercle( ( ( Eg[i] ).get_field() ) ) )
		{
			cout<<message::field_superposition(field.get_centre_x(), field.get_centre_y(),  (Eg[i]).get_centre_x(),  (Eg[i]).get_centre_y() );
			exit(0);
		}
	}
	
}

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

Gisement Decodage_Ligne(string line, vector<Gisement> & Eg)
{
	istringstream data(line);
	double x(0.), y(0.), rayon(0.), capacite(0.);
	data>> x >> y >> rayon >> capacite;
	Gisement gisement(x, y, rayon, capacite, Eg);
	return gisement;
	
};
