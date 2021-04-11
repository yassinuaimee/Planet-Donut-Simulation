#ifndef ROBOT_H_INCLUDED
#define ROBOT_H_INCLUDED

#include <fstream>
#include "gisement.h"

class Robot
{
public:
    Robot(unsigned, double, double, double, double, double, bool);
    double get_x();
    double get_y();
    double get_xb();
    double get_yb();
    
protected:
	unsigned uid;
	double dp;   //compteur de distance parcourue
	Point position;
	Point but;
	bool atteint;

};

class Prospection : public Robot
{
private:
    bool retour;
	bool found;
	Gisement gisement;  //Mémorise un objet de la classe gisement.
	
	
public:
	Prospection(unsigned, double, double, double, double, double, bool, bool, bool, double, double, double, double);
	Prospection(unsigned, double, double, double, double, double, bool, bool, bool);
    void affiche();
};

class Forage : public Robot
{
private:
	
public:
	Forage(unsigned, double, double, double, double, double, bool);
    void affiche();
};



class Transport : public Robot
{
private:
	bool retour;
public:
	Transport(unsigned, double, double, double, double, double, bool, bool);
    void affiche();
};

class Communication : public Robot
{
private:

public:
	Communication(unsigned, double, double, double, double, double, bool);
    void affiche();
};
/*
Prospection decodage_ligne_prospection(std::ifstream &);
Forage decodage_ligne_forage(std::ifstream &);
Transport decodage_ligne_transport(std::ifstream &);
Communication decodage_ligne_communication(std::ifstream &);
 */
#endif
