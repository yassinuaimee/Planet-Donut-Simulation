#ifndef GISEMENT_H_INCLUDED
#define GISEMENT_H_INCLUDED
#include <string>

Gisement Decodage_Ligne(string, const vector<Gisement> &,const unsigned&);

class Gisement 
{
public:
	Gisement(double, double, double, double, const vector<Gisement> &,const unsigned&);
	Cercle field();
	
private:
	Cercle field;
	double capacite; 
	
};

#endif
