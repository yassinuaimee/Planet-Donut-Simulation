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

Base::Base(double x, double y, double ressources, double nbP, double nbF, double nbT, double nbC, vector<Base> & Eb,vector<Gisement>& Eg, ifstream & entree )
: centre(x, y), ressources(ressources), nbP(nbP), nbF(nbF), nbT(nbT), nbC(nbC)
{
	Cercle base_cercle(centre, rayon_base*2);//Je met un rayon de deux pour pouvoir faire la verification sur cercle + point au lieu de faire cercle+cercle
	
	for(size_t i(0); i<Eb.size(); ++i)//Boucle de vérification que deux bases ne se superposent pas
	{
		if(base_cercle.point_appartient((Eb[i]).get_centre()))
		{
			cout<<message::base_superposition(x, y,  (Eb[i]).get_x(),  (Eb[i]).get_y() );
			exit(0);
		}
	}
	
	for(size_t i(0); i<Eg.size(); ++i)//Boucle de vérification que Base et Field ne se superposent pas
	{
		if(((Eg[i]).get_field()).point_appartient(centre))
		{
			cout<<message::base_field_superposition(centre.get_x(), centre.get_y(), (Eg[i]).get_centre_x, (Eg[i]).get_centre_y);
			exit(0);
		}
	}
	
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
	
	bool communication_centre(false);
	
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
		if(equal_zero(x-centre.get_x()) and equal_zero(y-centre.get_y()))
			communication_centre=true;
		
	}
	if(not(communication_centre))
	{
		cout<<message::missing_robot_communication(centre.get_x(), centre.get_y());
		exit(0);
	}
}



// definition de Base

Base Decodage_Ligne(string line, vector<Base> & Eb, vector<Gisement>& Eg, ifstream & entree)
{
	istringstream data(line);
	double x(0.0), y(0.0), ressources(0.0), nbP(0.0), nbF(0.0), nbT(0.0),nbC(0.0);
	data>> x >> y >> ressources >> nbP >> nbF >> nbT >> nbC;
	Base base(x, y, ressources, nbP, nbF, nbT, nbC, Eb, Eg, entree)
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

Point Base::get_centre()
{
	return centre;
}

double Base::get_x()
{
	return centre.get_x();
}

double Base::get_y()
{
	return centre.get_y();
}
