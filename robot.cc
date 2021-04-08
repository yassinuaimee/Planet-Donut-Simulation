#include <iostream>
#include "robot.h"
#include "message.h"
#include "geomod.h"
#include "gisement.h"

using namespace std;

//Robot
Robot::Robot(unsigned uid, double dp, double x, double y, double xb, double yb, bool atteint)
: uid(uid), dp(dp), Pos(x,y), Posbut(cb, yb), atteint(atteint)
{}


double Robot::get_Pos_x()
{
	return Pos.x;
}
	
double Robot:: get_Pos_y() 
{
	return Pos.y;
}

double Robot::get_PosBut_x() 
{
	return PosBut.x;
}

double Robot::get_PosBut_y()
{
	return PosBut.y;
}

//Robot prospection
Prospection::Prospection(unsigned uid, double dp, double x, double y, double xb, 
                         double yb, bool atteint, bool retour, bool found, double xg, 
                         double yg, double rayong, double capaciteg) 
: Robot(uid, dp, x, y, xb, yb, atteint), retour(retour), found(found), gisement(xg, yg, rayong, capaciteg) 
{}
Prospection::Prospection(unsigned uid, double dp, double x, double y, double xb, 
                         double yb, bool atteint, bool retour, bool found)
:Robot(uid, dp, x, y, xb, yb, atteint), retour(retour), found(found)
{}
//Robot Forage

Forage::Forage(unsigned uid, double dp, double x, double y, double xb, double yb, bool atteint)
:Robot(uid, dp, x, y, xb, yb, atteint)
{}

//Robot Transport
Transport::Transport(unsigned uid, double dp, double x, double y, double xb, 
                         double yb, bool atteint, bool retour)
:Robot(uid, dp, x, y, xb, yb, atteint),retour(retour)
{} 

//Robot communication
Communication::Communication(unsigned uid, double dp, double x, double y, double xb, double yb, bool atteint)
:Robot(uid, dp, x, y, xb, yb, atteint)
{}

