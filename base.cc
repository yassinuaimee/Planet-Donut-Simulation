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

//===================================================================================//

Base::Base(double x, double y, double ressources, int nbP, int nbF, int nbT, int nbC, ifstream & entree )
: centre(x, y, rayon_base), ressources(ressources), nbP(nbP), nbF(nbF), nbT(nbT), nbC(nbC)
{
	string line;
	int test_nbP(0);
	
    while(test_nbP<nbP)
    
    {
        getline(entree, line);
        
		istringstream data(line);
		unsigned uid(0);
		if(not(data>>uid))//verif sauts ligne
			continue;
        
        verif_uid(uid);
        
        (this->E_P).push_back(decodage_ligne_prospection(uid, line));
		++test_nbP;
			
	}
	
	int test_nbF(0);
	
	while(test_nbF<nbF)
	{
        getline(entree, line);
		istringstream data(line);
		unsigned uid(0);
		if( !(data>>uid))//permet de faire la vérification si on a un saut à la ligne ou une connerie comme ça
			continue;
		
		double dp(0.0), x(0.0), y(0.0), xb(0.0), yb(0.0);
		bool atteint(false);
		data>>dp>>x>>y>>xb>>yb>>atteint;
        verif_uid(uid);
		Forage forage(uid, dp, x, y, xb, yb, atteint);
		(this->E_F).push_back(forage);
		++test_nbF;
	}
	int test_nbT(0);
	
	while( test_nbT<nbT)
	{
        getline(entree, line);
		istringstream data(line);
		unsigned uid(0);
		if( !(data>>uid))//permet de faire la vérification si on a un saut à la ligne ou une connerie comme ça
			continue;
			
		double dp(0.0), x(0.0), y(0.0), xb(0.0), yb(0.0);
		bool atteint(false), retour(false);
		data>>dp>>x>>y>>xb>>yb>>atteint>>retour;
        verif_uid(uid);
		Transport transport(uid, dp, x, y, xb, yb, atteint, retour);
		(this->E_T).push_back(transport);
		++test_nbT;
	}
	
	bool communication_centre(false);
	int test_nbC(0);

		
	
	while(test_nbC<nbC)
	{
        getline(entree, line);
		istringstream data(line);
		int uid(0);
		if( not(data>>uid) )
			continue;
			
		double dp(0.0), x(0.0), y(0.0), xb(0.0), yb(0.0);
		bool atteint(false);
		data>>dp>>x>>y>>xb>>yb>>atteint;
		
		
		verif_uid(uid);
        
		Communication communication(uid, dp, x, y, xb, yb, atteint);
		(this->E_C).push_back(communication);
		++test_nbC;
		
		if(equal_zero( sqrt( pow(x-centre.get_centre_x(), 2 ) + pow( y-centre.get_centre_y(), 2))))
			
		communication_centre=true;
		
	}
	
	if(not(communication_centre))
	{
		cout<<message::missing_robot_communication(centre.get_centre_x(), centre.get_centre_y());
		exit(0);
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
    cout<<"   "<<centre.get_x()<<"  "<<centre.get_centre_y()<<" "<<ressources<<" "<<nbP<<" "<<nbF<<" "<<nbT<<" "<<nbC<<endl;
    for(auto& prospection : E_P)
    {
        prospection.affiche();
    }
    cout<<endl;
    for(auto& forage : E_F)
    {
        forage.affiche();
    }
    cout<<endl;
    for(auto& transport : E_T)
    {
        transport.affiche();
    }
    cout<<endl;
    for(auto& communication : E_C)
    {
        communication.affiche();
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


Prospection decodage_ligne_prospection(unsigned uid, string line)
{

    
    istringstream data(line);
    double dp(0.0), x(0.0), y(0.0), xb(0.0), yb(0.0);
    bool atteint(false), retour(false), found(false);
    data>>dp>>x>>y>>xb>>yb>>atteint>>retour>>found;
    if(found)
    {
        double xg(0.0), yg(0.0), rayong(0.0), capaciteg(0.0);
        data>>xg>>yg>>rayong>>capaciteg;
        Prospection prospection(uid, dp, x, y, xb, yb, atteint, retour, found, xg, yg, rayong, capaciteg);
        return prospection;
    }
    else
    {
        Prospection prospection(uid, dp, x, y, xb, yb, atteint, retour, found);
        return prospection;
    }
    
}
