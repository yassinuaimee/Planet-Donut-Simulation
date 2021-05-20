/* Nom:
 * gisement.h
 *
 * Auteurs:
 * Yassin Al-Nuaimee
 * Adrien Maillet Gonzalez
 *
 * Version: 12
 *
 *
 */

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
    void affiche_texte();
    void affiche_texte(std::ofstream&);
    void affiche_dessin();
    bool verification(std::vector<Gisement>&);
    Cercle get_field();
	double get_x();
	double get_y();
	double get_rayon();
    double get_capacite();
    void delta_capacite(double);
    bool get_error_gisement();
    bool in_E_G(std::vector<Gisement>&);
	
private:
	Cercle field;
    double capacite;
    bool error_gisement;
};

//================================================================================//

Gisement creation_gisement(std::string);

#endif
