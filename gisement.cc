#include <iostream>
#include <string>
#include "gisement.h"
#include "message.h"
#include "geomod.h"



//===================================================================================//
//definition de Gisement
Gisement::Gisement(double x_, double y_, double rayon_, double capacite_, const vector<Gisement> & Eg, const unsigned& nbG)
: 
{
	if(capacite<0)
		exit(0);
	capacite=capacite_;
	Point centre(x_, y_);
	Cercle field(centre, rayon_);
	for(auto& gisement : Eg)
	{
		if(field.intersection_cercle( gisement.get_field()))
		{
			message::field_superposition(field.get_centre_x, field.get_centre_y, (gisement.get_field()).get_centre_x, (gisement.get_field()).get_centre_y);
			exit(0);
		}
	}
	
}

Cercle Cercle::get_field()
{
	return field;
}

Gisement Gisement::Decodage_Ligne(string line, const vector<Gisement> & Eg,const unsigned& nbG)
{
	istringstream data(line);
	double x(0.), y(0.), rayon(0.), capacite(0.);
	data>> x >> y >> rayon >> capacite;
	Gisement gisement(x, y, rayon, capacite, Eg);
	return gisement;
	
};
