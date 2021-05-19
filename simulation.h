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
    void verifications();
    void clear();
    
    void update();
    void adjacence();
    
    void affiche_texte();
    void affiche_texte(std::ofstream&);
    void affiche_dessin(bool);
    void affiche_range(bool);
    void affiche_link(bool);
    
    unsigned get_nbG();
    unsigned get_nbB();
    int get_base_nbP(size_t);
    int get_base_nbF(size_t);
    int get_base_nbT(size_t);
    int get_base_nbC(size_t);
    bool get_error_file();
    double get_base_ressources(size_t);

private:
	unsigned nbG;
	unsigned nbB;
	std::vector<Base> Eb;
	std::vector<Gisement> Eg;
    bool error_file;
};

#endif
