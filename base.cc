#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include "base.h"
#include "robot.h"
#include "message.h"
#include "geomod.h"
#include "gisement.h"
#include "constantes.h"

using namespace std;

namespace
{
    std::vector<unsigned> E_uid;
}

void verif_uid(const unsigned);

bool lecture_bool(istringstream&);

Prospection* decodage_ligne_prospection(unsigned, std::istringstream&);
Forage* decodage_ligne_forage(unsigned, std::istringstream&);
Transport* decodage_ligne_transport(unsigned, std::istringstream&);
Communication* decodage_ligne_communication(unsigned, std::istringstream&);

std::vector<Prospection*> liste_propecteur(int, std::ifstream&);
std::vector<Forage*> liste_forage(int, std::ifstream&);
std::vector<Transport*> liste_transport(int, std::ifstream&);
std::vector<Communication*> liste_communication(int, std::ifstream&, Cercle&);

void communication_centre(std::vector<Communication*>&, Cercle&);

//===================================================================================//
//std::vector<Forage*> liste_forage(std::ifstream&);
//std::vector<Transport*> liste_transport(std::ifstream&);
//std::vector<Communication*> liste_communication(std::ifstream&);

std::vector<Prospection*> liste_propecteur(int nbP, std::ifstream& entree)
{
    string line;
    int test_nbP(0);
    vector<Prospection*> E_P;
    
    while(test_nbP<nbP)
    
    {
        getline(entree>>ws, line);
        
        istringstream data(line);
        unsigned uid(0);
        
        if(not(data>>uid))//verif sauts ligne
            continue;
        
        verif_uid(uid);
        E_P.push_back(decodage_ligne_prospection(uid, data));
        ++test_nbP;
            
    }
    
    return E_P;
}

std::vector<Forage*> liste_forage(int nbF, std::ifstream& entree)
{
    string line;
    int test_nbF(0);
    vector<Forage*> E_F;
    
    while(test_nbF<nbF)
    {
        getline(entree>>ws, line);
        istringstream data(line);
        unsigned uid(0);
        if( !(data>>uid))//verif sauts ligne
            continue;
        verif_uid(uid);
        E_F.push_back(decodage_ligne_forage(uid, data));
        
        
        
        ++test_nbF;
    }
    return E_F;
    
}

std::vector<Transport*> liste_transport(int nbT, std::ifstream& entree)
{
    string line;
    int test_nbT(0);
    vector<Transport*> E_T;
    
    while(test_nbT<nbT)
    {
        getline(entree>>ws, line);
        istringstream data(line);
        unsigned uid(0);
        if( !(data>>uid))//permet de faire la vérification si on a un saut à la ligne ou une connerie comme ça
            continue;
        verif_uid(uid);
        E_T.push_back(decodage_ligne_transport(uid, data));
        
        
        ++test_nbT;
    }
    return E_T;
    
}

std::vector<Communication*> liste_communication(int nbC, std::ifstream& entree, Cercle& centre)
{
    string line;
    int test_nbC(0);
    vector<Communication*> E_C;
    
    

        
    
    while(test_nbC<nbC)
    {
        getline(entree>>ws, line);
        istringstream data(line);
        int uid(0);
        if( not(data>>uid) )
            continue;
        verif_uid(uid);
        E_C.push_back(decodage_ligne_communication(uid, data));
        ++test_nbC;
        
    }
    communication_centre(E_C, centre);
    /*
    for(auto& communication : E_C)
    {
        if(equal_zero( sqrt( pow(x-centre.get_centre_x(), 2 ) + pow( y-centre.get_centre_y(), 2))))
            communication_centre=true;
    }
    if(not(communication_centre))
    {
        cout<<message::missing_robot_communication(centre.get_centre_x(), centre.get_centre_y());
        exit(0);
    }
    */
    return E_C;
}
void communication_centre(std::vector<Communication*>& E_C, Cercle& centre)
{
    bool centre_ok(false);
    for(auto& robot : E_C)
    {
        if(centre.point_appartient(robot->get_position()))
            centre_ok=true;
    }
    if(not(centre_ok))
    {
        cout<<message::missing_robot_communication(centre.get_centre_x(), centre.get_centre_y());
        exit(0);
    }
}


Base::Base(double x, double y, double ressources, int nbP, int nbF, int nbT, int nbC, ifstream & entree )
: centre(x, y, rayon_base), ressources(ressources), nbP(nbP), nbF(nbF), nbT(nbT), nbC(nbC)
{
    
    ///Initialisation de la liste des PROSPECTEURS==========================================================================
    (this->E_P)=liste_propecteur(nbP, entree);
    (this->E_F)=liste_forage(nbF, entree);
    (this->E_T)=liste_transport(nbT, entree);
    (this->E_C)=liste_communication(nbC, entree, centre);
}
void Base::destruction()
{
    
    for(auto& ptr_Prospection: E_P)
    {
        delete ptr_Prospection;
        ptr_Prospection=nullptr;
    }
    for(auto& ptr_Forage: E_F)
    {
        delete ptr_Forage;
        ptr_Forage=nullptr;
    }
    for(auto& ptr_Transport: E_T)
    {
        delete ptr_Transport;
        ptr_Transport=nullptr;
    }
    for(auto& ptr_Communication: E_C)
    {
        delete ptr_Communication;
        ptr_Communication=nullptr;
    }
}





// definition de Base

Base decodage_ligne_base(string line, ifstream & entree)
{
	istringstream data(line);
	double x(0.0), y(0.0), ressources(0.0);
	int nbP(0), nbF(0), nbT(0),nbC(0);
	data>> x >> y >> ressources >> nbP >> nbF >> nbT >> nbC;
	Base base(x, y, ressources, nbP, nbF, nbT, nbC, entree);
    return base;
}

void verif_uid(const unsigned uid)
{
	for(auto& element : E_uid)
	{
		if(element==uid)
		{
			cout<<message::identical_robot_uid(uid);
			exit(0);
		}
	}
    E_uid.push_back(uid);
}

void Base::affiche()
{
    cout<<"\t"<<centre.get_x()<<" "<<centre.get_centre_y()<<" "<<ressources<<" "<<nbP<<" "<<nbF<<" "<<nbT<<" "<<nbC<<endl;
    for(auto& prospection : E_P)
    {
        prospection->affiche();
    }
    cout<<endl;
    for(auto& forage : E_F)
    {
        forage->affiche();
    }
    cout<<endl;
    for(auto& transport : E_T)
    {
        transport->affiche();
    }
    cout<<endl;
    for(auto& communication : E_C)
    {
        communication->affiche();
    }
    cout<<endl;
    
}

Cercle Base::get_centre()
{
	return centre;
}

double Base::get_x()
{
	return centre.get_centre_x();
}

double Base::get_y()
{
	return centre.get_centre_y();
}

bool lecture_bool(istringstream& data)
{
    string booleen;
    data>>booleen;
    if(booleen=="false")
        return false;
    else
        return true;
}

Prospection* decodage_ligne_prospection(unsigned uid, istringstream& data)
{

    string booleen;
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
        Prospection* p(new Prospection(uid, dp, x, y, xb, yb, atteint, retour, found, xg, yg, rayong, capaciteg));
        return p;
    }
    else
    {
        Prospection* p(new Prospection(uid, dp, x, y, xb, yb, atteint, retour, found));
        return p;
    }
    
}
Forage* decodage_ligne_forage(unsigned uid, istringstream& data)
{
    double dp(0.0), x(0.0), y(0.0), xb(0.0), yb(0.0);
    bool atteint(false);
    data>>dp>>x>>y>>xb>>yb;
    atteint=lecture_bool(data);
    Forage* forage(new Forage(uid, dp, x, y, xb, yb, atteint));
    return forage;
}

Transport* decodage_ligne_transport(unsigned uid, istringstream& data)
{
    double dp(0.0), x(0.0), y(0.0), xb(0.0), yb(0.0);
    bool atteint(false), retour(false);
    data>>dp>>x>>y>>xb>>yb;
    atteint=lecture_bool(data);
    retour=lecture_bool(data);
    Transport* transport(new Transport(uid, dp, x, y, xb, yb, atteint, retour));
    return transport;
}

Communication* decodage_ligne_communication(unsigned uid, istringstream& data)
{
    double dp(0.0), x(0.0), y(0.0), xb(0.0), yb(0.0);
    bool atteint(false);
    data>>dp>>x>>y>>xb>>yb;
    atteint=lecture_bool(data);
    
    Communication* communication(new Communication(uid, dp, x, y, xb, yb, atteint));
    return communication;
}
