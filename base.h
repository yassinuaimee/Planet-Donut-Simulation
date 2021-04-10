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
	Cercle centre;
	double ressources;
	int nbP, nbF, nbT, nbC;
	
	

public:
	Base(double, double, double, int, int, int, int, std::ifstream & );
	Cercle get_centre();
	double get_x();
	double get_y();
};

void verif_uid(const unsigned, std::vector<unsigned> &);
Base decodage_ligne_base(std::string, std::ifstream &);

#endif
