#ifndef SIMULATION_H_INCLUDED
#define SIMULATION_H_INCLUDED

#include <vector>

void lecture(ifstream &){}

class Simulation
{
public:
	Simulation();
private:
	unsigned nbG;
	unsigned nbB;
	vector<Base> Eb;
	vector<Gisement> Eg;
	
};


#endif
