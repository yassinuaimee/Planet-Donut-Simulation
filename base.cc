/* Nom:
 * base.cc
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
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <memory>
#include "base.h"
#include "robot.h"
#include "message.h"
#include "geomod.h"
#include "gisement.h"
#include "constantes.h"

//using namespace std;

namespace
{
    std::vector<unsigned> E_uid;
}

void verif_uid(const unsigned);
bool lecture_bool(std::stringstream&);

std::unique_ptr<Prospection> creation_robot_prospection(unsigned, std::stringstream&);
std::unique_ptr<Forage> creation_robot_forage(unsigned, std::stringstream&);
std::unique_ptr<Transport> creation_robot_transport(unsigned, std::stringstream&);
std::unique_ptr<Communication> creation_robot_communication(unsigned, 
															std::stringstream&);

void init_liste_propecteur(int, std::ifstream&, 
						   std::vector<std::unique_ptr<Prospection>>&);
void init_liste_forage(int, std::ifstream&, 
					   std::vector<std::unique_ptr<Forage>>&);
void init_liste_transport(int, std::ifstream&, 
						  std::vector<std::unique_ptr<Transport>>&);
void init_liste_communication(int, std::ifstream&, Cercle&,
							  std::vector<std::unique_ptr<Communication>>&);

void communication_centre(std::vector<std::unique_ptr<Communication>>&, Cercle&);


///////////////////////////////////////////////////////////////////////////////////////
/*
BASE
*/
///////////////////////////////////////////////////////////////////////////////////////

Base creation_base(std::string line, std::ifstream & entree)
{
    std::stringstream data(line);
    double x(0.0), y(0.0), ressources(0.0);
    int nbP(0), nbF(0), nbT(0),nbC(0);
    data>>x>>y>>ressources>>nbP>>nbF>>nbT>>nbC;
    Base base(x, y, ressources, nbP, nbF, nbT, nbC, entree);
    return base;
}

//================================================================================//

Base::Base(double x, double y, double ressources,
           int nbP, int nbF, int nbT, int nbC, std::ifstream & entree )
:centre(x, y, rayon_base), ressources(ressources),
 nbP(nbP), nbF(nbF), nbT(nbT), nbC(nbC)
{
    if(nbP!=0)
    {
        init_liste_propecteur(nbP, entree, this->E_P);
    }
    if(nbF!=0)
    {
        init_liste_forage(nbF, entree, this->E_F);
        
    }
    if(nbT!=0)
    {
        init_liste_transport(nbT, entree, this->E_T);
        
    }
    if(nbC!=0)
    {
        init_liste_communication(nbC, entree, this->centre, this->E_C);
    }
    else
    {
        std::cout<<message::missing_robot_communication(x, y);
        exit(0);
        
    }
    E_uid.clear();//Permet de vider le vecteur E_uid pour la prochaine Base
}

//================================================================================//

Cercle Base::get_centre()
{
    return centre;
}

//================================================================================//

double Base::get_x()
{
    return centre.get_x();
}

//================================================================================//

double Base::get_y()
{
    return centre.get_y();
}

//================================================================================//

void Base::affiche_texte()
{
    std::cout<<"\t"<<centre.get_x()<<" "<<centre.get_y()<<" "
             <<ressources<<" "<<nbP<<" "<<nbF<<" "<<nbT<<" "<<nbC<<std::endl;
    for(auto& prospection : E_P)
    {
        prospection->affiche_texte();
    }
    std::cout<<std::endl;
    for(auto& forage : E_F)
    {
        forage->affiche_texte();
    }
    std::cout<<std::endl;
    for(auto& transport : E_T)
    {
        transport->affiche_texte();
    }
    std::cout<<std::endl;
    for(auto& communication : E_C)
    {
        communication->affiche_texte();
    }
    std::cout<<std::endl;
    
}
//================================================================================//

void Base::affiche_dessin(int index)
{
    centre.affiche_dessin(3, index);
    for(auto& prospection : E_P)
    {
        prospection->affiche_dessin(index);
    }
    for(auto& forage : E_F)
    {
        forage->affiche_dessin(index);
    }
    for(auto& transport : E_T)
    {
        transport->affiche_dessin(index);
    }
    for(auto& communication : E_C)
    {
        communication->affiche_dessin(index);
    }
}

//================================================================================//

void Base::affiche_range()
{
    for(auto& prospection : E_P)
    {
        prospection->affiche_range();
    }
    for(auto& forage : E_F)
    {
        forage->affiche_range();
    }
    for(auto& transport : E_T)
    {
        transport->affiche_range();
    }
    for(auto& communication : E_C)
    {
        communication->affiche_range();
    }
}

///////////////////////////////////////////////////////////////////////////////////////
/*
PROSPECTION
*/
///////////////////////////////////////////////////////////////////////////////////////

void init_liste_propecteur(int nbP, std::ifstream& entree, 
						   std::vector<std::unique_ptr<Prospection>>& E_P)
{
    std::string line;
    int test_nbP(0);
    
    while(test_nbP<nbP)
    {
        getline(entree>>std::ws, line);
        
        std::stringstream data(line);
        unsigned uid(0);
        
        if(not(data>>uid))
        {
            continue;
            
        }
        
        verif_uid(uid);
        E_P.push_back(creation_robot_prospection(uid, data));
        ++test_nbP;
    }
}

//================================================================================//

std::unique_ptr<Prospection> creation_robot_prospection(unsigned uid,
                                                        std::stringstream& data)
{
    std::string booleen;
    double dp(0.), x(0.), y(0.), xb(0.), yb(0.);
    bool atteint(false), retour(false), found(false);
    data>>dp>>x>>y>>xb>>yb;
    
    atteint=lecture_bool(data);
    retour=lecture_bool(data);
    found=lecture_bool(data);
    
    if(found)
    {
        double xg(0.0), yg(0.0), rayong(0.0), capaciteg(0.0);
        data>>xg>>yg>>rayong>>capaciteg;
        std::unique_ptr<Prospection> p(new Prospection(uid, dp, x, y, xb, yb,
                                                       atteint, retour, found,
                                                       xg, yg,
                                                       rayong, capaciteg));
        return p;
    }
    else
    {
        std::unique_ptr<Prospection> p(new Prospection(uid, dp, x, y, xb, yb,
                                                       atteint, retour, found));
        return p;
    }
    
}


///////////////////////////////////////////////////////////////////////////////////////
/*
FORAGE
*/
///////////////////////////////////////////////////////////////////////////////////////

void init_liste_forage(int nbF, std::ifstream& entree,
                       std::vector<std::unique_ptr<Forage>>& E_F)
{
    std::string line;
    int test_nbF(0);
    
    while(test_nbF<nbF)
    {
        getline(entree>>std::ws, line);
        std::stringstream data(line);
        unsigned uid(0);
        if(not(data>>uid))
        {
            continue;
        }
        verif_uid(uid);
        E_F.push_back(creation_robot_forage(uid, data));
        
        ++test_nbF;
    }
}

//================================================================================//

std::unique_ptr<Forage> creation_robot_forage(unsigned uid, std::stringstream& data)
{
    double dp(0.0), x(0.0), y(0.0), xb(0.0), yb(0.0);
    bool atteint(false);
    data>>dp>>x>>y>>xb>>yb;
    atteint=lecture_bool(data);
    std::unique_ptr<Forage> forage(new Forage(uid, dp, x, y, xb, yb, atteint));
    return forage;
}

///////////////////////////////////////////////////////////////////////////////////////
/*
TRANSPORT
*/
///////////////////////////////////////////////////////////////////////////////////////

void init_liste_transport(int nbT, std::ifstream& entree,
                          std::vector<std::unique_ptr<Transport>>& E_T)
{
    std::string line;
    int test_nbT(0);
    
    while(test_nbT<nbT)
    {
        getline(entree>>std::ws, line);
        std::stringstream data(line);
        unsigned uid(0);
        if(not(data>>uid))
        {
            continue;
        }
        
        verif_uid(uid);
        E_T.push_back(creation_robot_transport(uid, data));
        
        ++test_nbT;
    }
}

//================================================================================//

std::unique_ptr<Transport> creation_robot_transport(unsigned uid,
                                                    std::stringstream& data)
{
    double dp(0.0), x(0.0), y(0.0), xb(0.0), yb(0.0);
    bool atteint(false), retour(false);
    data>>dp>>x>>y>>xb>>yb;
    atteint=lecture_bool(data);
    retour=lecture_bool(data);
    std::unique_ptr<Transport> transport(new Transport(uid, dp, x, y, xb, yb,
                                                       atteint, retour));
    return transport;
}


///////////////////////////////////////////////////////////////////////////////////////
/*
COMMUNICATION
*/
///////////////////////////////////////////////////////////////////////////////////////

void init_liste_communication(int nbC, std::ifstream& entree, Cercle& centre,
                              std::vector<std::unique_ptr<Communication>>& E_C)
{

    std::string line;
    int test_nbC(0);
    
    while(test_nbC<nbC)
    {
        getline(entree>>std::ws, line);
        std::stringstream data(line);
        int uid(0);
        if(not(data>>uid))
        {
            continue;
        }
        verif_uid(uid);
        E_C.push_back(creation_robot_communication(uid, data));
        ++test_nbC;
        
    }
    communication_centre(E_C, centre);
}

//================================================================================//

std::unique_ptr<Communication> creation_robot_communication(unsigned uid,
                                                            std::stringstream& data)
{
    double dp(0.0), x(0.0), y(0.0), xb(0.0), yb(0.0);
    bool atteint(false);
    data>>dp>>x>>y>>xb>>yb;
    atteint=lecture_bool(data);
    
    std::unique_ptr<Communication> communication(new Communication(uid, dp, x, y,
                                                                   xb, yb,
                                                                   atteint));
    return communication;
}

//================================================================================//

void communication_centre(std::vector<std::unique_ptr<Communication>>& E_C,
                          Cercle& centre)
{
    bool centre_ok(false);
    for(auto& robot : E_C)
    {
        if((robot->get_position()).same_position(centre.get_centre()))
        {
            centre_ok=true;
        }
    }
    if(not(centre_ok))
    {
        std::cout<<message::missing_robot_communication(centre.get_x(),
                                                        centre.get_y());
        exit(0);
    }
}


///////////////////////////////////////////////////////////////////////////////////////
/*
FONCTIONS SUPPLEMENTAIRES
*/
///////////////////////////////////////////////////////////////////////////////////////

void verif_uid(const unsigned uid)
{
	for(auto& element : E_uid)
	{
		if(element==uid)
		{
			std::cout<<message::identical_robot_uid(uid);
			exit(0);
		}
	}
    E_uid.push_back(uid);
}

//================================================================================//

bool lecture_bool(std::stringstream& data)
{
    std::string booleen;
    data>>booleen;
    if(booleen=="false")
    {
        return false;
    }
    else
    {
        return true;
    }
}

//================================================================================//

int Base::get_nbP()
{
    return nbP;
}

//================================================================================//

int Base::get_nbF()
{
    return nbF;
}

//================================================================================//

int Base::get_nbT()
{
    return nbT;
}

//================================================================================//

int Base::get_nbC()
{
    return nbC;
}

//================================================================================//

double Base::get_ressources()
{
    return ressources;
}
