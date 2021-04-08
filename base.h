#ifndef BASE_H_INCLUDED
#define BASE_H_INCLUDED

#include <vector>
#include <string>
#include <fstream>
#include "robot.h"

class Base
{
private:
	std::vector<unsigned> E_uid;
	std::vector<Prospection> E_P;
	std::vector<Forage> E_F;
	std::vector<Transport> E_T;
	std::vector<Communication> E_C;
	Point centre;
	double ressources;
	double nbP, nbF, nbT, nbC;
	
	

public:
	Base(double, double, double, double, double, double, double, std::vector<Base> &, vector<Gisement>&, std::ifstream & );
	Point get_centre();
	double get_x();
	double get_y();
};

void verif_uid(const unsigned, std::vector<unsigned> &);
Base Decodage_Ligne(string, std::vector<Base> &,vector<Gisement>&, std::ifstream &);

#endif
