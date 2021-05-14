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
    void update();
    void affiche_texte();
    void affiche_texte(std::ofstream&);
    void affiche_dessin();
    unsigned get_nbG();
    unsigned get_nbB();
    int get_base_nbP(size_t);
    int get_base_nbF(size_t);
    int get_base_nbT(size_t);
    int get_base_nbC(size_t);
    bool get_error_file();
    double get_base_ressources(size_t);
    void affiche_range(bool);

private:
	unsigned nbG;
	unsigned nbB;
	std::vector<Base> Eb;
	std::vector<Gisement> Eg;
    bool error_file;
};

#endif
