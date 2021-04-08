#ifndef ROBOT_H_INCLUDED
#define ROBOT_H_INCLUDED

#include "gisement.h"

class Robot
{
protected:
	unsigned uid;
public:

};

class Prospection : public Robot
{
private:
	const double cout_prosp;
	const double maxD_prosp;
	bool found;
	Gisement gisement;  //Mémorise un objet de la classe gisement.
	bool retour;
public:
};

class Forage : public Robot
{
private:
	const double cout_forage;
	const double maxD_forage;
public:
};



class Transport : public Robot
{
private:

public:

};

class Communication : public Robot
{
private:

public:

};
#endif
