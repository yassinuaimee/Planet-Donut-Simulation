#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "base.h"
#include "robot.h"
#include "message.h"
#include "geomod.h"
#include "gisement.h"

using namespace std;

//===================================================================================//

Base::Base(double x, double y, double ressources, double nbP, double nbF, double nbT, double nbC, vector<Base> & Eb, ifstream & entree )
: centre(x, y), ressources(ressources), nbP(nbP), nbF(nbF), nbT(nbT), nbC(nbC)
{
	
	for(size_t i(0); i<nbP; ++i)
	{
		getline(entree, line);
		istringstream data(line);
		unsigned uid(0);
		double dp(0.0), x(0.0), y(0.0), xb(0.0), yb(0.0);
		bool atteint(false), retour(false), found(false);
		data>>uid>>dp>>x>>y>>xb>>yb>>atteint>>retour>>found;
		verif_uid(uid, this->E_uid);
		E_uid.push_back(uid);
		if(found)
		{
			double xg(0.0), yg(0.0), rayong(0.0), capaciteg(0.0);
			data>>xg>>yg>>rayong>>capaciteg;
			Prospection prospection(uid, dp, x, y, xb, yb, atteint, retour, found, xg, yg, rayong, capaciteg)
			(this->E_P).push_back(prospection);
		}
		else
		{
			Prospection prospection(uid, dp, x, y, xb, yb, atteint, retour, found)
			(this->E_P).push_back(prospection);
		}
	}
	
	for(size_t i(0); i<nbF; ++i)
	{
		getline(entree, line);
		istringstream data(line);
		unsigned uid(0);
		double dp(0.0), x(0.0), y(0.0), xb(0.0), yb(0.0);
		bool atteint(false);
		data>>uid>>dp>>x>>y>>xb>>yb>>atteint;
		verif_uid(uid, this->E_uid);
		E_uid.push_back(uid);
		Forage forage(uid, dp, x, y, xb, yb, atteint);
		(this->E_F).push_back(forage);
		
	}
	
	for(size_t i(0); i<nbT; ++i)
	{
		getline(entree, line);
		istringstream data(line);
		unsigned uid(0);
		double dp(0.0), x(0.0), y(0.0), xb(0.0), yb(0.0);
		bool atteint(false), retour(false);
		data>>uid>>dp>>x>>y>>xb>>yb>>atteint>>retour;
		verif_uid(uid, this->E_uid);
		E_uid.push_back(uid);
		Transport transport(uid, dp, x, y, xb, yb, atteint, retour);
		(this->E_T).push_back(transport);
	}
	
	for(size_t i(0); i<nbC; ++i)
	{
		getline(entree, line);
		istringstream data(line);
		unsigned uid(0);
		double dp(0.0), x(0.0), y(0.0), xb(0.0), yb(0.0);
		bool atteint(false);
		data>>uid>>dp>>x>>y>>xb>>yb>>atteint;
		verif_uid(uid, this->E_uid);
		E_uid.push_back(uid);
		Communication communication(uid, dp, x, y, xb, yb, atteint);
		(this->E_C).push_back(communication);
		
	}
	
}



// definition de Base

Base Decodage_Ligne(string line, vector<Base> & Eb, ifstream & entree)
{
	istringstream data(line);
	double x(0.0), y(0.0), ressources(0.0), nbP(0.0), nbF(0.0), nbT(0.0),nbC(0.0);
	data>> x >> y >> ressources >> nbP >> nbF >> nbT >> nbC;
	Base base(x, y, ressources, nbP, nbF, nbT, nbC, Eb, entree)
}

void verif_uid(const unsigned uid, vector<unsigned> & E_uid)
{
	for(auto& element : E_uid)
	{
		if(element==uid)
		{
			cout<<message::identical_robot_uid(uid);
			exit(0);
		}
	}
}
