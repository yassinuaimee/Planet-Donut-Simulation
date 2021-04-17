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
    Point get_position();
    double get_xb();
    double get_yb();
    
protected:
	unsigned uid;
	double dp;
	Point position;
	Point but;
	bool atteint;
};

//================================================================================//

class Prospection : public Robot
{
public:
    Prospection(unsigned, double, double, double, double, double, bool, bool, bool, double, double, double, double);
    Prospection(unsigned, double, double, double, double, double, bool, bool, bool);
    void affiche();
    
private:
    bool retour;
	bool found;
	Gisement gisement;
};

//================================================================================//

class Forage : public Robot
{
public:
    Forage(unsigned, double, double, double, double, double, bool);
    void affiche();
    
private:
    
};

//================================================================================//

class Transport : public Robot
{
public:
    Transport(unsigned, double, double, double, double, double, bool, bool);
    void affiche();
    
private:
	bool retour;

};

//================================================================================//

class Communication : public Robot
{
public:
    Communication(unsigned, double, double, double, double, double, bool);
    void affiche();

private:

};
#endif
