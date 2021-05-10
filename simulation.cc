/* Nom:
 * simulation.cc
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
#include <sstream>
#include <fstream>
#include <string>
#include "gisement.h"
#include "base.h"
#include "geomod.h"
#include "simulation.h"
#include "message.h"
#include "constantes.h"

//using namespace std;

//================================================================================//

Simulation::Simulation()
: nbG(0), nbB(0)
{}

//================================================================================//

unsigned Simulation::get_nbG()
{
    return nbG;
}

//================================================================================//

unsigned Simulation::get_nbB()
{
    return nbB;
}

//================================================================================//

void Simulation::lecture(std::ifstream & entree)
{
    std::string line;
    double valeur;
    
    //LECTURE GISEMENTS=====================================================
    
    unsigned test_nbG(0);
    
    while(getline(entree>>std::ws, line))
    {
        std::istringstream data(line);
        if(not(data>>nbG))
        {
            continue;
        }
        else
        {
            break;
        }
    }
    
    if(nbG!=0)
    {
        while(getline(entree>>std::ws, line))
        {
            std::istringstream data(line);
        
            if(not(data>>valeur))
            {
                continue;
            }
            
            Gisement gisement=creation_gisement(line);
            gisement.verification(Eg);
            Eg.push_back(creation_gisement(line));
            
            ++test_nbG;
            
            if(test_nbG==nbG)
            {
                break;
            }
        }
    }
    
    //LECTURE BASES=====================================================
    
    unsigned test_nbB(0);
    
    while(getline(entree>>std::ws, line)) //Partie pour la lecture de nbG
    {
        std::istringstream data(line);
        if(not(data>>nbB))
        {
            continue;
        }
        else
        {
            break;
        }
    }
    
    if(nbB!=0)
    {
        while(getline(entree>>std::ws, line))
        {
            std::istringstream data(line);
        
            if(not(data>>valeur))
            {
                continue;
            }
            
            Eb.push_back( creation_base(line, entree));
            ++test_nbB;
            
            if(test_nbB==nbB)
            {
                break;
            }
        }
    }
    std::cout<<"On est en train de lire le fichier\n";
    this->verifications();
    this->affiche_texte();
}

//================================================================================//

void Simulation::verifications()
{
    std::cout<<"On efectue les verifications\n";
    for(auto& base : Eb)//Vérification intersection BASES et GISEMENTS
    {
        for(auto& gisement : Eg)
        {
            if((gisement.get_field()).intersection_cercle(base.get_centre()))
            {
                std::cout<<message::base_field_superposition(base.get_x(),
                                                        base.get_y(),
                                                        gisement.get_x(),
                                                        gisement.get_y());
                exit(0);
            }
        }
    }
    
    
    for(size_t i(0); i<nbB; ++i)//Verification intersection BASES
    {
        for(size_t j(i+1); j<nbB; ++j)
        {
            if(((Eb[i]).get_centre()).intersection_cercle((Eb[j]).get_centre()))
            {
                std::cout<<message::base_superposition((Eb[i]).get_x(),
                                                  (Eb[i]).get_y(),
                                                  (Eb[j]).get_x(),
                                                  (Eb[j]).get_y() );
                exit(0);
            }
        }
    }
}

//================================================================================//

void Simulation::affiche_texte()
{
    std::cout<<nbG<<" #Nombre GISEMENTS"<<std::endl;
    for(auto& gisement : Eg)
    {
        gisement.affiche_texte();
    }
    
    std::cout<<std::endl<<nbB<<" #Nombre BASES"<<std::endl;
    for(auto& base : Eb)
    {
        base.affiche_texte();
    }
}

//================================================================================//

void Simulation::clear()
{
    nbB=0;
    nbG=0;
    Eb.clear();
    Eg.clear();
}

//================================================================================//

void Simulation::affiche_dessin()
{
    for(auto& gisement : Eg)
    {
        gisement.affiche_dessin();
    }
    for(auto i(0);i<nbB;++i)
    {
        Eb[i].affiche_dessin(i);
    }
}

//================================================================================//

void Simulation::affiche_range(bool toggle_range)
{
    std::cout<<"On affiche le range\n";
    if(toggle_range)
    {
        std::cout<<"toggle_range = true\n";
        for(int i(0);i<nbB;++i)
        {
            Eb[i].affiche_range();
        }
    }
    else
    {
        std::cout<<"toggle_range = false\n";
    }
}

//================================================================================//

int Simulation::get_base_nbP(size_t i)
{
    return Eb[i].get_nbP();
}

//================================================================================//

int Simulation::get_base_nbF(size_t i)
{
    return Eb[i].get_nbF();
}

//================================================================================//

int Simulation::get_base_nbT(size_t i)
{
    return Eb[i].get_nbT();
}

//================================================================================//

int Simulation::get_base_nbC(size_t i)
{
    return Eb[i].get_nbC();
}

//================================================================================//

double Simulation::get_base_ressources(size_t i)
{
    return Eb[i].get_ressources();
}
