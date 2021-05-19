/* Nom:
 * simulation.cc
 *
 * Auteurs:
 * Yassin Al-Nuaimee
 * Adrien Maillet Gonzalez
 *
 * Version: 14
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


/*
//================================================================================//
 //MÉTHODES DE MAINTENANCE/CREATION DE LA SIMULATION//
//================================================================================//
*/

Simulation::Simulation()
: nbG(0), nbB(0), error_file(false)
{}

//================================================================================//

void Simulation::lecture(std::ifstream & entree)
{
    error_file=false;
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
            
            //On fait à la fois la vérification que le gisement est ok et on utilise le bool de sortie
            if(gisement.verification(Eg) or gisement.get_error_gisement())
            {
                error_file=true;
            }
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
            Eb.push_back(creation_base(line, entree));
            
            ++test_nbB;
            
            if(test_nbB==nbB)
            {
                break;
            }
        }
    }
    this->verifications();
    this->affiche_texte();
}

//================================================================================//

void Simulation::verifications()
{
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
                error_file=true;
            }
        }
        if(base.get_error_base())
        {
            error_file=true;
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
                error_file=true;
            }
        }
    }
}

//================================================================================//


void Simulation::clear()
{
    nbB=0;
    nbG=0;
    error_file=false;
    Eb.clear();
    Eg.clear();
}


/*
//================================================================================//
 //MÉTHODES EVOLUTION DE LA SIMULATION//
//================================================================================//
*/

void Simulation::update()
{
    for(unsigned i(0); i<nbB; ++i)
    {
        for(unsigned j(0); j<nbB; ++j)
        {
            Eb[i].update_voisin(Eb[j]);
        }
    }
    for(unsigned i(0); i<nbB; ++i)
    {
        if(Eb[i].get_ressources()<=finR)
        {
            Eb[i].connexion();
            Eb[i].maintenance();
            Eb[i].creation();
            Eb[i].evolution(Eg);
            
        }
    }
    for(unsigned i(0); i<nbB; ++i)
    {
        if(Eb[i].get_ressources()<=0)
        {
            Eb[i].destruction();
        }
    }

}

//================================================================================//

void Simulation::adjacence()//Pratique dans le cas où on souhaite affiche direct les link
                                //alors même que la simulation n'a même pas commencé
{
    for(unsigned i(0); i<nbB; ++i)
    {
        for(unsigned j(0); j<nbB; ++j)
        {
            Eb[i].update_voisin(Eb[j]);
        }
    }
}



/*
//================================================================================//
 //MÉTHODES D'AFFICHAGE//
//================================================================================//
*/

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

void Simulation::affiche_texte(std::ofstream& sortie)
{
    sortie<<nbG<<" #Nombre GISEMENTS"<<std::endl;
    for(auto& gisement : Eg)
    {
        gisement.affiche_texte(sortie);
    }
    sortie<<std::endl<<nbB<<" #Nombre BASES"<<std::endl;
    for(auto& base : Eb)
    {
        base.affiche_texte(sortie);
    }
}

//================================================================================//

void Simulation::affiche_dessin(bool toggle_range)
{
    for(auto& gisement : Eg)
    {
        gisement.affiche_dessin();
    }
    for(unsigned i(0);i<nbB;++i)
    {
        Eb[i].affiche_dessin(i, toggle_range);
    }
}

//================================================================================//

void Simulation::affiche_range(bool toggle_range)
{
    if(toggle_range)
    {
        for(unsigned i(0);i<nbB;++i)
        {
            Eb[i].affiche_range();
        }
    }
}

//================================================================================//

void Simulation::affiche_link(bool toggle_link)
{
    if(toggle_link)
    {
        for(unsigned i(0);i<nbB;++i)
        {
            Eb[i].affiche_link();
        }
    }
}

/*
//================================================================================//
 //GETTERS//
//================================================================================//
*/

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

//================================================================================//

bool Simulation::get_error_file()
{
    return error_file;
}
