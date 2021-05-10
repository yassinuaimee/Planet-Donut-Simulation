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

//================================================================================//

void Robot::affiche_range()
{
    position.cercle_communication();
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

void Prospection::affiche_texte()
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

//================================================================================//

void Prospection::affiche_range()
{
    position.cercle_communication();
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

void Forage::affiche_texte()
{
    std::cout<<"\t\t"<<uid<<" "<<dp<<" "
        <<position.get_x()<<" "<<position.get_y()<<" "
        <<but.get_x()<<" "<<but.get_y()<<" "
        <<atteint<<std::endl;
}

//================================================================================//

void Forage::affiche_dessin(int index)
{
    position.affiche_dessin(2,index);
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

void Transport::affiche_texte()
{
    std::cout<<"\t\t"<<uid<<" "<<dp<<" "
        <<position.get_x()<<" "<<position.get_y()<<" "
        <<but.get_x()<<" "<<but.get_y()<<" "
        <<atteint<<" "<<retour<<std::endl;
}

//================================================================================//

void Transport::affiche_dessin(int index)
{
    position.affiche_dessin(3,index);
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

void Communication::affiche_texte()
{
    std::cout<<"\t\t"<<uid<<" "<<dp<<" "
        <<position.get_x()<<" "<<position.get_y()<<" "
        <<but.get_x()<<" "<<but.get_y()<<" "
        <<atteint<<std::endl;
}

//================================================================================//

void Communication::affiche_dessin(int index)
{
    position.affiche_dessin(4,index);
}
