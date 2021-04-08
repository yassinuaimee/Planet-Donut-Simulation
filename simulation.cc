#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include "gisement.h"
#include "base.h"
#include "geomod.h"
#include "simulation.h"


using namespace std;

void lecture(ifstream & entree)
{
	unsigned nbG;
	unsigned nbB;
	vector<Base> Eb;
	vector<Gisement> Eg;
	
	bool nbG_ok(false), nbB_ok(false);
	bool Eg_ok(false), Eb_ok(false);
	bool nbP(false), nbF(false), nbT_ok(false), nbC_ok(false);
	
	string line;
	double valeur;
	while(getline(entree, line)) //Partie pour la lecture de nbG
	{
		istringstream data(line);
		if( !(data>>valeur))
			continue;
		if(not( nbG_ok))
		{
			nbG=valeur;
			nbG_ok=true;
			break;
		}
	}
	unsigned test_nbG(0);
	
	while(getline(entree, line))
	{
		istringstream data(line);
		if( !(data>>valeur))
			continue;
		
		if(not( Eg_ok))
		{
			Eg.push_back( Decodage_Ligne(line ,Eg));
			++test_nbG;
			
			
			
		if(test_nbG==nbG)
		{
			Eg_ok=true;
			
			for(size_t i(0); i<Eg.size(); ++i)//Permet de voir tout s'il n'y a pas intersection
			{
				cout<<"Eg["<<i<<"]  : centre x : "<<( (Eg[i]).get_field()).get_centre_x()<<", centre y: "<<(( Eg[i]).get_field()).get_centre_y()<<endl;
			}
			cout<<endl<<"Il n'y a pas d'intersection si on est la."<<endl<<endl;
			break;
		}
		}
	}
	
	while(getline(entree, line)) //Partie pour la lecture de nbB
	{
		istringstream data(line);
		if( !(data>>valeur))
			continue;
		if(not( nbB_ok))
		{
			nbB=valeur;
			nbB_ok=true;
			break;
		}
	}
	unsigned test_nbG(0);
	
	while(getline(entree, line))
	{
		istringstream data(line);
		if(!(data>>valeur))
			continue;
		
		if(not(Eb_ok)
	}
	
}


//===================================================================================//
//definition de Simulation

//A faire plus tard!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
/*
Simulation::Simulation()
:
{
	
}
 */
