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
             double xb, double yb, bool atteint, int type)
: uid(uid), dp(dp), position(x, y), but(xb, yb) , atteint(atteint), type(type),
  remote(false)
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

void Robot::deplacement(Point base)
{
	if(not(atteint) and not(reach_max_dp()))
	{
        double x(position.get_x()), y(position.get_y());
	    double xb(but.get_x()), yb(but.get_y());
	    double delta_x(xb-x), delta_y(yb-y);
	    double norme(sqrt(pow(delta_x,2)+pow(delta_y,2)));
	    
	    if(norme>5)
	    {
	        position.set_x(x+5*delta_x/norme);
	        position.set_y(y+5*delta_y/norme);
	        dp=dp+5;
	    }
        else if(norme<5)
        {
            position.set_x(xb);
            position.set_y(yb);
            dp=dp+norme;
            atteint=true;
        }
    }
}

//================================================================================//

std::vector<Gisement> Robot::get_gisements()
{
    std::vector<Gisement> L_gisements;
    for(auto& robot : L_adjacence)
    {
        if(robot->get_type()==1)
        {
            if(robot->get_atteint()==true)
            {
                std::cout<<"le robot "<<&robot<<" a trouvé un gisement\n";
            }
            
        }
    }
    return L_gisements;
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
 //GETTERS & SETTERS//
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

Point Robot::get_but()
{
    return but;
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

//================================================================================//

unsigned Robot::get_uid()
{
    return uid;
}
//================================================================================//

bool Robot::get_atteint()
{
    return atteint;
}

//================================================================================//

void Robot::set_atteint(bool x)
{
    atteint=x;
}

//================================================================================//

int Robot::get_type()
{
    return type;
}

//================================================================================//

void Robot::set_remote(bool x)
{
    remote=x;
}

//================================================================================//

bool Robot::get_remote()
{
    return remote;
}

//================================================================================//

Gisement Robot::get_gisement()//Vraiment de la merde mais ça me simplifie plein plein de trucs
{
    Gisement gisement;
    return gisement;
}

//================================================================================//

void Robot::set_but(Point p)
{
    but.set_x(p.get_x());
    but.set_y(p.get_y());
}

//================================================================================//

void Robot::set_but(double x, double y)
{
    but.set_x(x);
    but.set_y(y);
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
:   Robot(uid, dp, x, y, xb, yb, atteint, 1),
    retour(retour), found(found), gisement(xg, yg, rayong, capaciteg),
    forage_envoyer(false)
{}

//================================================================================//

Prospection::Prospection(unsigned uid, double dp, double x, double y,
                         double xb, double yb, bool atteint, bool retour, bool found)
: Robot(uid, dp, x, y, xb, yb, atteint, 1), retour(retour), found(found), gisement(),
forage_envoyer(false)
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

void Prospection::test_return_base(Point base)
{
    if(atteint==false)
    {
        if(10*dim_max-dp-6<norme_plus_petit_vecteur(position.get_x(),
                                                  position.get_y(),
                                                  base.get_x(),
                                                  base.get_y()))
        {
            but.set_x(base.get_x());
            but.set_y(base.get_y());
        }
    }
}
//================================================================================//

void Prospection::deplacement(Point base)
{
    if(not(atteint) and not(reach_max_dp()))
    {
        test_return_base(base);
        double x(position.get_x()), y(position.get_y());
        double xb(but.get_x()), yb(but.get_y());
        double delta_x(xb-x), delta_y(yb-y);
        double norme(sqrt(pow(delta_x,2)+pow(delta_y,2)));
        
        if(norme>5)
        {
            position.set_x(x+5*delta_x/norme);
            position.set_y(y+5*delta_y/norme);
            dp=dp+5;
        }
        else if(norme<5)
        {
            position.set_x(xb);
            position.set_y(yb);
            dp=dp+norme;
            atteint=true;
        }
    }
    if(found)
    {
        but.set_x(base.get_x());
        but.set_y(base.get_y());
        atteint=false;
    }
}

//================================================================================//

void Prospection::add_gisement(Gisement gisement_found)
{
    if(gisement_found.get_capacite()>=400)
    {
        found=true;
        gisement=gisement_found;
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

void Prospection::affiche_dessin(int index, bool toggle_range)
{
    position.affiche_dessin(1, index);
    if(toggle_range)
    {
        if(position.get_x()-rayon_comm<(-dim_max))
        {
            ::affiche_dessin(1, index, position.get_x()+2*dim_max, position.get_y());
        }
        if(position.get_y()+rayon_comm>dim_max)
        {
           ::affiche_dessin(1, index, position.get_x(), position.get_y()-2*dim_max);
        }
    }
}

//================================================================================//

bool Prospection::get_found()
{
    return found;
}

//================================================================================//

bool Prospection::get_forage_envoyer()
{
    return forage_envoyer;
}

//================================================================================//

double Prospection::get_ressources()
{
    return gisement.get_capacite();
}

//================================================================================//

void Prospection::set_forage_envoyer(bool x)
{
    forage_envoyer=x;
}

//================================================================================//

Cercle Prospection::get_cercle_gisement()
{
    return gisement.get_field();
}

//================================================================================//

Point Prospection::get_centre_gisement()
{
    return gisement.get_field().get_centre();
}

//================================================================================//

double Prospection::get_rayon_gisement()
{
    return gisement.get_field().get_rayon();
}

//================================================================================//

Gisement Prospection::get_gisement()
{
    return gisement;
}

//================================================================================//
std::vector<bool> Prospection::get_bools()
{
    std::vector<bool> bools;
    bools.push_back(atteint);
    bools.push_back(retour);
    bools.push_back(found);
    return bools;
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
: Robot(uid, dp, x, y, xb, yb, atteint, 2)
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

void Forage::affiche_dessin(int index, bool toggle_range)
{
    position.affiche_dessin(2,index);
    if(toggle_range)
    {
        if(position.get_x()-rayon_comm<(-dim_max))
        {
            ::affiche_dessin(2, index, position.get_x()+2*dim_max, position.get_y());
        }
        if(position.get_y()+rayon_comm>dim_max)
        {
           ::affiche_dessin(2, index, position.get_x(), position.get_y()-2*dim_max);
        }
    }
}

//================================================================================//
std::vector<bool> Forage::get_bools()
{
    std::vector<bool> bools;
    bools.push_back(atteint);
    return bools;
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
: Robot(uid, dp, x, y, xb, yb, atteint, 3), retour(retour)
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

void Transport::reinitialise(std::vector<Gisement>& E_G)
{
    atteint=false;
    retour=false;
    for(auto& gisement : E_G)
    {
        double capacite(gisement.get_capacite());
        if(capacite>200)
        {
            std::cout<<"Boucle nouvelles coordonnées du vecteur\n";
            but.set_x(gisement.get_x());
            but.set_y(gisement.get_y());
        }
    }
}

//================================================================================//

void Transport::deplacement(Point base)
{
    if(not(atteint) and not(reach_max_dp()))
    {
        double x(position.get_x()), y(position.get_y());
        double xb(but.get_x()), yb(but.get_y());
        double delta_x(xb-x), delta_y(yb-y);
        double norme(sqrt(pow(delta_x,2)+pow(delta_y,2)));
        
        if(norme>5)
        {
            position.set_x(x+5*delta_x/norme);
            position.set_y(y+5*delta_y/norme);
            dp=dp+5;
        }
        else if(norme<5)
        {
            position.set_x(xb);
            position.set_y(yb);
            dp=dp+norme;
            atteint=true;
            if(retour==false)
            {
                retour=true;
                atteint=false;
                but.set_x(base.get_x());
                but.set_y(base.get_y());
            }
            std::cout<<"Robot de transport arrivé\n";
        }
    }
    Point p(base.get_x(),base.get_y());
    if(retour==true and position.same_position(p))
    {
        atteint=false;
    }
}



//================================================================================//

bool Transport::get_retour()
{
    return retour;
}

//================================================================================//

void Transport::set_retour(bool x)
{
    retour=x;
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

void Transport::affiche_dessin(int index, bool toggle_range)
{
    position.affiche_dessin(3,index, retour);
    if(toggle_range)
    {
        if(position.get_x()-rayon_comm<(-dim_max))
        {
            ::affiche_dessin(3, index, position.get_x()+2*dim_max, position.get_y(), retour);
        }
        if(position.get_y()+rayon_comm>dim_max)
        {
           ::affiche_dessin(3, index, position.get_x(), position.get_y()-2*dim_max, retour);
        }
    }
}

//================================================================================//

std::vector<bool> Transport::get_bools()
{
    std::vector<bool> bools;
    bools.push_back(atteint);
    bools.push_back(retour);
    return bools;
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
: Robot(uid, dp, x, y, xb, yb, atteint, 4), communication_ok(false)
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

void Communication::affiche_dessin(int index, bool toggle_range)
{
    position.affiche_dessin(4, index);
    if(toggle_range)
    {
        if(position.get_x()-rayon_comm<(-dim_max))
        {
            ::affiche_dessin(4, index,position.get_x()+2*dim_max, position.get_y());
        }
        if(position.get_y()+rayon_comm>dim_max)
        {
           ::affiche_dessin(4, index, position.get_x(), position.get_y()-2*dim_max);
        }
    }
}

//================================================================================//

void Communication::creation_connexion(std::vector<std::shared_ptr<Robot>>& E_R,
                                       std::shared_ptr<Communication>& robot_centre)
{
    for(auto& robot : E_R)
    {
        bool in_adjacence(false);
        
        for(auto& robot_adj : L_adjacence)
        {
            if(robot==robot_adj)
            {
                in_adjacence=true;
                break;
            }
        }
        if(robot==robot_centre)
        {
            in_adjacence=true;
        }
        if(in_adjacence)
        {
            robot->set_remote(true);
        }
        else
        {
            robot->set_remote(false);
        }
    }
}

//================================================================================//

std::vector<bool> Communication::get_bools()
{
    std::vector<bool> bools;
    bools.push_back(atteint);
    return bools;
}

//================================================================================//

std::vector<std::shared_ptr<Robot>> Communication::get_adj()
{
    return L_adjacence;
}

