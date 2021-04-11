#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "gisement.h"
#include "message.h"
#include "geomod.h"
#include "constantes.h"

using namespace std;

//===================================================================================//

Gisement::Gisement()
: field(0.0, 0.0, 0.0), capacite(0.0)
{}

//===================================================================================//

Gisement::Gisement(double x, double y, double rayon, double capacite)//On verifie direct dedans que les autres gisements n'intersectent pas
: field( x, y, rayon), capacite(capacite)
{
    if(capacite<0)
        exit(0);
    if (rayon<rayon_min or rayon>rayon_max)
        exit(0);
    
}

//===================================================================================//

Gisement decodage_ligne_gisement(string line)
{
    istringstream data(line);
    double x(0.), y(0.), rayon(0.), capacite(0.);
    data>> x >> y >> rayon >> capacite;
    Gisement gisement(x, y, rayon, capacite);
    return gisement;
    
};

//===================================================================================//

void Gisement::verification(std::vector<Gisement>& Eg)
{
    for(auto& gisement : Eg)
    {
        if(field.intersection_cercle(gisement.get_field()))
        {
            cout<<message::field_superposition(field.get_centre_x(),
                                               field.get_centre_y(),
                                               gisement.get_centre_x(),
                                               gisement.get_centre_y());
            exit(0);
        }
    }
}

//===================================================================================//

Cercle Gisement::get_field()
{
	return field;
}

//===================================================================================//

double Gisement::get_centre_x()
{
	return field.get_centre_x();
}

//===================================================================================//

double Gisement::get_centre_y()
{
	return field.get_centre_y();
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

void Gisement::affiche()
{
    cout<<"\t"<<field.get_x()<<" "<<field.get_y()<<" "<<field.get_rayon()<<" "<<capacite<<endl;
}
