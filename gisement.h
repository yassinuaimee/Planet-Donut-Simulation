#ifndef GISEMENT_H_INCLUDED
#define GISEMENT_H_INCLUDED

#include <string>
#include "geomod.h"

class Gisement 
{
public:
	Gisement(double, double, double, double,  std::vector<Gisement> &);
	Cercle get_field();
	double get_centre_x();
	double get_centre_y();
	double get_rayon();
	
private:
	Cercle field;
	double capacite; 
	
};

Gisement Decodage_Ligne( std::string, std::vector<Gisement> &);

#endif
