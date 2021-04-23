#ifndef GISEMENT_H_INCLUDED
#define GISEMENT_H_INCLUDED

#include <string>
#include <vector>
#include "geomod.h"

class Gisement 
{
public:
	Gisement(double, double, double, double);
	Gisement();
    void affiche();
    void verification(std::vector<Gisement>&);
    Cercle get_field();
	double get_x();
	double get_y();
	double get_rayon();
    double get_capacite();
	
private:
	Cercle field;
	double capacite; 
};

//================================================================================//

Gisement creation_gisement(std::string);

#endif
