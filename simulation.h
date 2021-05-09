/* Nom:
 * simulation.h
 *
 * Auteurs:
 * Yassin Al-Nuaimee
 * Adrien Maillet Gonzalez
 *
 * Version: 12
 *
 *
 */

#ifndef SIMULATION_H_INCLUDED
#define SIMULATION_H_INCLUDED

#include <vector>
#include <fstream>
#include "base.h"
#include "gisement.h"

class Simulation
{
    
public:
	Simulation();
    void lecture(std::ifstream &);
    void clear();
    void verifications();
    void affiche_texte();
    void affiche_dessin();
    unsigned get_nbG();
    unsigned get_nbB();
private:
	unsigned nbG;
	unsigned nbB;
	std::vector<Base> Eb;
	std::vector<Gisement> Eg;
};

#endif
