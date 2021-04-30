/* Nom:
 * robot.cc
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
#include <sstream>
#include <fstream>
#include "robot.h"
#include "message.h"
#include "geomod.h"
#include "gisement.h"

//using namespace std;


/*
//================================================================================//
 //ROBOT//
//================================================================================//
*/

Robot::Robot(unsigned uid, double dp, double x, double y,
             double xb, double yb, bool atteint)
: uid(uid), dp(dp), position(x, y), but(xb, yb) , atteint(atteint)
{}

//================================================================================//

double Robot::get_x()
{
	return position.get_x();
}
	
//================================================================================//

double Robot::get_y()
{
	return position.get_y();
}

//================================================================================//

double Robot::get_xb() 
{
	return but.get_x();
}

//================================================================================//

double Robot::get_yb()
{
	return but.get_y();
}

//================================================================================//

Point Robot::get_position()
{
    return position;
}


/*
//================================================================================//
 //ROBOT-PROSPECTION//
//================================================================================//
*/

Prospection::Prospection(unsigned uid, double dp, double x, double y,
                         double xb, double yb, bool atteint, bool retour,
                         bool found, double xg, double yg, double rayong, 
						 double capaciteg)
:   Robot(uid, dp, x, y, xb, yb, atteint),
    retour(retour), found(found), gisement(xg, yg, rayong, capaciteg)
{}

//================================================================================//

Prospection::Prospection(unsigned uid, double dp, double x, double y,
                         double xb, double yb, bool atteint, bool retour, bool found)
: Robot(uid, dp, x, y, xb, yb, atteint), retour(retour), found(found), gisement()
{}

//================================================================================//

void Prospection::affiche()
{
    if(found)
    {
        std::cout<<"\t\t"<<uid<<" "
                    <<dp<<" "
                    <<position.get_x()<<" "<<position.get_y()<<" "
                    <<but.get_x()<<" "<<but.get_y()<<" "
                    <<atteint<<" "<<retour<<" "<<found<<" "
                    <<gisement.get_x()<<" "<<gisement.get_y()<<" "
                    <<gisement.get_rayon()<<" "
                    <<gisement.get_capacite()<<std::endl;
    }
    else
    {
        std::cout<<"\t\t"<<uid<<" "<<dp<<" "
            <<position.get_x()<<" "<<position.get_y()<<" "
            <<but.get_x()<<" "<<but.get_y()<<" "
            <<atteint<<" "<<retour<<" "<<found<<std::endl;
    }
}


/*
//================================================================================//
 //ROBOT-FORAGE//
//================================================================================//
*/

Forage::Forage(unsigned uid, double dp, double x, double y,
               double xb, double yb, bool atteint)
: Robot(uid, dp, x, y, xb, yb, atteint)
{}

//================================================================================//

void Forage::affiche()
{
    std::cout<<"\t\t"<<uid<<" "<<dp<<" "
        <<position.get_x()<<" "<<position.get_y()<<" "
        <<but.get_x()<<" "<<but.get_y()<<" "
        <<atteint<<std::endl;
}


/*
//================================================================================//
 //ROBOT-TRANSPORT//
//================================================================================//
*/

Transport::Transport(unsigned uid, double dp, double x, double y,
                     double xb, double yb, bool atteint, bool retour)
: Robot(uid, dp, x, y, xb, yb, atteint), retour(retour)
{}

//================================================================================//

void Transport::affiche()
{
    std::cout<<"\t\t"<<uid<<" "<<dp<<" "
        <<position.get_x()<<" "<<position.get_y()<<" "
        <<but.get_x()<<" "<<but.get_y()<<" "
        <<atteint<<" "<<retour<<std::endl;
}


/*
//================================================================================//
 //ROBOT-COMMUNICATION//
//================================================================================//
*/

Communication::Communication(unsigned uid, double dp, double x, double y,
                             double xb, double yb, bool atteint)
: Robot(uid, dp, x, y, xb, yb, atteint)
{}

//================================================================================//

void Communication::affiche()
{
    std::cout<<"\t\t"<<uid<<" "<<dp<<" "
        <<position.get_x()<<" "<<position.get_y()<<" "
        <<but.get_x()<<" "<<but.get_y()<<" "
        <<atteint<<std::endl;
}
