#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include "gisement.h"
#include "base.h"
#include "geomod.h"
#include "simulation.h"
#include "message.h"
#include "constantes.h"

using namespace std;



Simulation::Simulation()
: nbG(0), nbB(0)
{}

unsigned Simulation::get_nbG()
{
    return nbG;
}

unsigned Simulation::get_nbB()
{
    return nbB;
}





void Simulation::lecture(ifstream & entree)
{
    string line;
    double valeur;
    unsigned test_nbG(0);
    
    while(getline(entree, line)) //Partie pour la lecture de nbG
    {
        istringstream data(line);
        if( !(data>>nbG))
            continue;
        else
            break;
    }
    
    
    while(getline(entree, line))
    {
        istringstream data(line);
        
        if(not(data>>valeur))
            continue;
        
        Eg.push_back(decodage_ligne_gisement(line));
        ++test_nbG;
            
        if(test_nbG==nbG)
            break;
        
    }
    //On passe à la lecture des BASES
    unsigned test_nbB(0);
    
    while(getline(entree, line)) //Partie pour la lecture de nbG
    {
        istringstream data(line);
        if( !(data>>nbG))
            continue;
        else
            break;
    }
    
    
    while(getline(entree, line))
    {
        istringstream data(line);
        
        if(not(data>>valeur))
            continue;
        
        Eb.push_back( decodage_ligne_base(line, entree));
        ++test_nbB;
            
        if(test_nbB==nbB)
            break;
        
    }
}


void Simulation::verifications()
{
    for(size_t i(0); i<nbG; ++i)
    {
        for(size_t j(i+1); j<nbG; ++j)
        {
            if(( (Eg[i]).get_field() ).intersection_cercle( (Eg[i]).get_field()) )
            {
                cout<<message::field_superposition((Eg[i]).get_centre_x(),
                                                   (Eg[i]).get_centre_y(),
                                                   (Eg[j]).get_centre_x(),
                                                   (Eg[j]).get_centre_y());
                exit(0);
            }
        }
    }
    
    
    for(auto& base : Eb)//Boucle de vérification que Base et Field ne se superposent pas
    {
        for(auto& gisement : Eg)
        {
            if((gisement.get_field()).intersection_cercle(base.get_centre()))
            {
                cout<<message::base_field_superposition(base.get_x(),
                                                        base.get_y(),
                                                        gisement.get_centre_x(),
                                                        gisement.get_centre_x());
                exit(0);
            }
        }
    }
    
    for(size_t i(0); i<nbB; ++i)
    {
        
        for(size_t j(i+1); j<nbB; ++j)
        {
            if(((Eb[i]).get_centre()).intersection_cercle((Eb[j]).get_centre()))
            {
                cout<<message::base_superposition((Eb[i]).get_x(),
                                                  (Eb[i]).get_y(),
                                                  (Eb[j]).get_x(),
                                                  (Eb[j]).get_y() );
                exit(0);
            }
        }
        
    }
    
}
