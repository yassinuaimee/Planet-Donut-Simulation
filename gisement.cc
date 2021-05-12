/* Nom:
 * gisement.cc
 *
 * Auteurs:
 * Yassin Al-Nuaimee
 * Adrien Maillet Gonzalez
 *
 * Version: 12
 *
 *
 */
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include "gisement.h"
#include "message.h"
#include "geomod.h"
#include "constantes.h"


//===================================================================================//

Gisement::Gisement()
: field(0.0, 0.0, 0.0), capacite(0.0)
{}

//===================================================================================//

Gisement::Gisement(double x, double y, double rayon, double capacite)
: field(x, y, rayon), capacite(capacite), error_gisement(false)
{
    if(capacite<0)
    {
        error_gisement=true;
    }
    if (rayon<rayon_min or rayon>rayon_max)
    {
        error_gisement=true;
        
    }
}

//===================================================================================//

Gisement creation_gisement(std::string line)
{
    std::istringstream data(line);
    double x(0.), y(0.), rayon(0.), capacite(0.);
    data>> x >> y >> rayon >> capacite;
    Gisement gisement(x, y, rayon, capacite);
    return gisement;
    
};

//===================================================================================//

bool Gisement::verification(std::vector<Gisement>& Eg)
{
    bool error(false);
    for(auto& gisement : Eg)
    {
        if(field.intersection_cercle(gisement.get_field()))
        {
            std::cout<<message::field_superposition(field.get_x(),
                                               field.get_y(),
                                               gisement.get_x(),
                                               gisement.get_y());
            error=true;
        }
    }
    return error;
}

//===================================================================================//

Cercle Gisement::get_field()
{
	return field;
}

//===================================================================================//

double Gisement::get_x()
{
	return field.get_x();
}

//===================================================================================//

double Gisement::get_y()
{
	return field.get_y();
}

//===================================================================================//

double Gisement::get_rayon()
{
	return field.get_rayon();
}

//===================================================================================//

double Gisement::get_capacite()
{
    return capacite;
}

//===================================================================================//

bool Gisement::get_error_gisement()
{
    return error_gisement;
}

//===================================================================================//

void Gisement::affiche_texte()
{
    std::cout<<"\t"<<field.get_x()<<" "
				   <<field.get_y()<<" "
				   <<field.get_rayon()<<" "
				   <<capacite<<std::endl;
}

//===================================================================================//

void Gisement::affiche_texte(std::ofstream& sortie)
{
    sortie<<"\t"<<field.get_x()<<" "
                   <<field.get_y()<<" "
                   <<field.get_rayon()<<" "
                   <<capacite<<std::endl;
}
//===================================================================================//

void Gisement::affiche_dessin()
{
    field.affiche_dessin(1,0);
}
