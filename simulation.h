#ifndef SIMULATION_H_INCLUDED
#define SIMULATION_H_INCLUDED

#include <vector>
#include <fstream>
#include "base.h"
#include "gisement.h"
void lecture(std::ifstream &);

class Simulation
{
public:
	Simulation();
private:
	unsigned nbG;
	unsigned nbB;
	std::vector<Base> Eb;
	std::vector<Gisement> Eg;
	
};


#endif
