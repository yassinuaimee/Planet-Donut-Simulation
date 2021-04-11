#ifndef GISEMENT_H_INCLUDED
#define GISEMENT_H_INCLUDED

#include <string>
#include "geomod.h"

class Gisement 
{
public:
	Gisement(double, double, double, double);
	Gisement();
    void affiche();
    void verification(std::vector<Gisement>&);
    Cercle get_field();
	double get_centre_x();
	double get_centre_y();
	double get_rayon();
    double get_capacite();
	
private:
	Cercle field;
	double capacite; 
};

Gisement decodage_ligne_gisement(std::string);

#endif
