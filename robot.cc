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
#include <cmath>
#include "robot.h"
#include "message.h"
#include "geomod.h"
#include "gisement.h"
#include "constantes.h"


///////////////////////////////////////////////////////////////////////////////////////
/*
//================================================================================//
 //ROBOT//
//================================================================================//
*/
///////////////////////////////////////////////////////////////////////////////////////

Robot::Robot(unsigned uid, double dp, double x, double y,
             double xb, double yb, bool atteint)
: uid(uid), dp(dp), position(x, y), but(xb, yb) , atteint(atteint)
{}

//================================================================================//

void Robot::vide_adj()
{
    L_adjacence.clear();
}

//================================================================================//

Robot::~Robot()
{}

//================================================================================//

bool Robot::communication(std::shared_ptr<Robot> & robot)
{
    Point point_robot(robot->get_position());
    double x(position.get_x()-point_robot.get_x());
    double y(position.get_y()-point_robot.get_y());
             
    if((sqrt(pow(x,2)+pow(y,2))-rayon_comm)<=0)
    {
        return true;
    }
    return false;
}

//================================================================================//

void Robot::deplacement(double x_base, double y_base)
{
	if(not(atteint))
	{
		test_return_base(x_base, y_base);
	    double x(position.get_x()), y(position.get_y());
	    double xb(but.get_x()), yb(but.get_y());
	    double delta_x(xb-x), delta_y(yb-y);
	    double norme(sqrt(pow(delta_x,2)+pow(delta_y,2)));
	    
	    if(norme>=5)
	    {
	        position.set_x(x+5*delta_x/norme);
	        position.set_y(y+5*delta_y/norme);
	        dp=dp+5;
	    }
	    else
	    {
			atteint=true;
		}
	}
}


/*
//================================================================================//
 //LISTE D'ADJACENCE//
//================================================================================//
*/

void Robot::ajoute_liste_adjacence(std::shared_ptr<Robot>& robot)
{
    L_adjacence.push_back(robot);
}

//================================================================================//

bool Robot::in_L_adj(std::shared_ptr<Robot>& robot)
{
    bool appartient(false);
    for(auto robot_adj : L_adjacence)
    {
        if(robot_adj==robot)
        {
            appartient=true;
        }
    }
    return appartient;
}


/*
//================================================================================//
 //MÉTHODES D'AFFICHAGE//
//================================================================================//
*/

void Robot::affiche_adjacence()//Stub utilisé à cause d'une erreur lors creation L_adj
{
    for(size_t i(0); i<L_adjacence.size(); ++i)
    {
        std::cout<<"\tRobot : "<<i<<", x = "<<L_adjacence[i]->get_x()
                                  <<", y = "<<L_adjacence[i]->get_y()<<"\n";
    }
}

//================================================================================//

void Robot::affiche_range()
{
    position.cercle_communication();
}

//================================================================================//

void Robot::affiche_link()
{
    double x1(position.get_x()), y1(position.get_y());
    for(auto& robot_adjacent : L_adjacence)
    {
        double x2(robot_adjacent->get_x()), y2(robot_adjacent->get_y());
        
        if(sqrt(pow(x1-x2,2)+pow(y1-y2,2))-rayon_comm<=0)
        {
            position.ligne_reseau(robot_adjacent->get_position());
        }
    }
}


/*
//================================================================================//
 //GETTERS//
//================================================================================//
*/

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

double Robot::get_dp()
{
    return dp;
}

//================================================================================//

void Robot::init_dp()
{
    dp=0;
}


/*
//================================================================================//
 //VISITED GETTER/SETTER//
//================================================================================//
*/

bool Robot::get_visited()
{
    return visited;
}

//================================================================================//

void Robot::set_visited(bool valeur)
{
    visited=valeur;
}


///////////////////////////////////////////////////////////////////////////////////////
/*
//================================================================================//
 //PROSPECTION//
//================================================================================//
*/
///////////////////////////////////////////////////////////////////////////////////////

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

bool Prospection::reach_max_dp()
{
    bool max(false);
    if(dp>10*dim_max-5)
    {
        max=true;
    }
    return max;
}

//================================================================================//

void Prospection::test_return_base(double& x_base, double& y_base)
{
	if(10*dim_max-dp<norme_plus_petit_vecteur(position.get_x(), 
											  position.get_y(), 
											  x_base, 
											  y_base))
	{
		but.set_x(x_base);
		but.set_y(y_base);
	}
}

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

void Prospection::affiche_texte(std::ofstream& sortie)
{
    if(found)
    {
        sortie<<"\t\t"<<uid<<" "
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
        sortie<<"\t\t"<<uid<<" "<<dp<<" "
            <<position.get_x()<<" "<<position.get_y()<<" "
            <<but.get_x()<<" "<<but.get_y()<<" "
            <<atteint<<" "<<retour<<" "<<found<<std::endl;
    }
}

//================================================================================//

void Prospection::affiche_dessin(int index)
{
    position.affiche_dessin(1, index);
    if(position.get_x()-rayon_comm<(-dim_max))
    {
        ::affiche_dessin(1, index, position.get_x()+2*dim_max, position.get_y());
    }
    if(position.get_y()+rayon_comm>dim_max)
    {
       ::affiche_dessin(1, index, position.get_x(), position.get_y()-2*dim_max);
    }
}


///////////////////////////////////////////////////////////////////////////////////////
/*
//================================================================================//
 //FORAGE//
//================================================================================//
*/
///////////////////////////////////////////////////////////////////////////////////////

Forage::Forage(unsigned uid, double dp, double x, double y,
               double xb, double yb, bool atteint)
: Robot(uid, dp, x, y, xb, yb, atteint)
{}

//================================================================================//

bool Forage::reach_max_dp()
{
    bool max(false);
    if(dp>1.42*dim_max-5)
    {
        max=true;
    }
    return max;
}

//================================================================================//

void Forage::test_return_base(double& x_base, double& y_base)
{
	if(1.42*dim_max-dp<norme_plus_petit_vecteur(position.get_x(), 
											  position.get_y(), 
											  x_base, 
											  y_base))
	{
		but.set_x(x_base);
		but.set_y(y_base);
	}
}

//================================================================================//

void Forage::affiche_texte()
{
    std::cout<<"\t\t"<<uid<<" "<<dp<<" "
        <<position.get_x()<<" "<<position.get_y()<<" "
        <<but.get_x()<<" "<<but.get_y()<<" "
        <<atteint<<std::endl;
}

//================================================================================//

void Forage::affiche_texte(std::ofstream& sortie)
{
    sortie<<"\t\t"<<uid<<" "<<dp<<" "
        <<position.get_x()<<" "<<position.get_y()<<" "
        <<but.get_x()<<" "<<but.get_y()<<" "
        <<atteint<<std::endl;
}

//================================================================================//

void Forage::affiche_dessin(int index)
{
    position.affiche_dessin(2,index);
    if(position.get_x()-rayon_comm<(-dim_max))
    {
        ::affiche_dessin(2, index, position.get_x()+2*dim_max, position.get_y());
    }
    if(position.get_y()+rayon_comm>dim_max)
    {
       ::affiche_dessin(2, index, position.get_x(), position.get_y()-2*dim_max);
    }
}


///////////////////////////////////////////////////////////////////////////////////////
/*
//================================================================================//
 //TRANSPORT//
//================================================================================//
*/
///////////////////////////////////////////////////////////////////////////////////////

Transport::Transport(unsigned uid, double dp, double x, double y,
                     double xb, double yb, bool atteint, bool retour)
: Robot(uid, dp, x, y, xb, yb, atteint), retour(retour)
{}

//================================================================================//

bool Transport::reach_max_dp()
{
    bool max(false);
    if(dp>5*dim_max-5)
    {
        max=true;
    }
    return max;
}

//================================================================================//

void Transport::test_return_base(double& x_base, double& y_base)
{
	if(5*dim_max-dp<norme_plus_petit_vecteur(position.get_x(), 
											  position.get_y(), 
											  x_base, 
											  y_base))
	{
		but.set_x(x_base);
		but.set_y(y_base);
	}
}

//================================================================================//

void Transport::affiche_texte()
{
    std::cout<<"\t\t"<<uid<<" "<<dp<<" "
        <<position.get_x()<<" "<<position.get_y()<<" "
        <<but.get_x()<<" "<<but.get_y()<<" "
        <<atteint<<" "<<retour<<std::endl;
}

//================================================================================//

void Transport::affiche_texte(std::ofstream& sortie)
{
    sortie<<"\t\t"<<uid<<" "<<dp<<" "
        <<position.get_x()<<" "<<position.get_y()<<" "
        <<but.get_x()<<" "<<but.get_y()<<" "
        <<atteint<<" "<<retour<<std::endl;
}

//================================================================================//

void Transport::affiche_dessin(int index)
{
    position.affiche_dessin(3,index);
    if(position.get_x()-rayon_comm<(-dim_max))
    {
        ::affiche_dessin(3, index, position.get_x()+2*dim_max, position.get_y());
    }
    if(position.get_y()+rayon_comm>dim_max)
    {
       ::affiche_dessin(3, index, position.get_x(), position.get_y()-2*dim_max);
    }
}


///////////////////////////////////////////////////////////////////////////////////////
/*
//================================================================================//
 //COMMUNICATION//
//================================================================================//
*/
///////////////////////////////////////////////////////////////////////////////////////

Communication::Communication(unsigned uid, double dp, double x, double y,
                             double xb, double yb, bool atteint)
: Robot(uid, dp, x, y, xb, yb, atteint)
{}

//================================================================================//

bool Communication::reach_max_dp()
{
    bool max(false);
    if(dp>1.42*dim_max-5)
    {
        max=true;
    }
    return max;
}

//================================================================================//

void Communication::test_return_base(double& x_base, double& y_base)
{
	if(1.42*dim_max-dp<norme_plus_petit_vecteur(position.get_x(), 
											  position.get_y(), 
											  x_base, 
											  y_base))
	{
		but.set_x(x_base);
		but.set_y(y_base);
	}
}

//================================================================================//

void Communication::affiche_texte()
{
    std::cout<<"\t\t"<<uid<<" "<<dp<<" "
        <<position.get_x()<<" "<<position.get_y()<<" "
        <<but.get_x()<<" "<<but.get_y()<<" "
        <<atteint<<std::endl;
}

//================================================================================//

void Communication::affiche_texte(std::ofstream& sortie)
{
    sortie<<"\t\t"<<uid<<" "<<dp<<" "
        <<position.get_x()<<" "<<position.get_y()<<" "
        <<but.get_x()<<" "<<but.get_y()<<" "
        <<atteint<<std::endl;
}

//================================================================================//

void Communication::affiche_dessin(int index)
{
    position.affiche_dessin(4, index);
    if(position.get_x()-rayon_comm<(-dim_max))
    {
        ::affiche_dessin(4, index,position.get_x()+2*dim_max, position.get_y());
    }
    if(position.get_y()+rayon_comm>dim_max)
    {
       ::affiche_dessin(4, index, position.get_x(), position.get_y()-2*dim_max);
    }
}

//================================================================================//

void Communication::creation_remote_autonomous(
                                    std::vector<std::shared_ptr<Robot>>& E_remote,
                                    std::vector<std::shared_ptr<Robot>>& E_autonomous,
                                    std::vector<std::shared_ptr<Robot>>& E_R,
                                    std::shared_ptr<Communication>& robot_centre)
{
    for(auto& robot_base : E_R)
    {
        bool in_adjacence(false);
        
        for(auto& robot_adj : L_adjacence)
        {
            if(robot_base==robot_adj)
            {
                in_adjacence=true;
            }
        }
        if(robot_base==robot_centre)
        {
            in_adjacence=true;
        }
        if(in_adjacence)
        {
            E_remote.push_back(robot_base);
        }
        else
        {
            E_autonomous.push_back(robot_base);
        }
    }
}
