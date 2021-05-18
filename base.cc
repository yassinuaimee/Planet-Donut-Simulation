/* Nom:
 * base.cc
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
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <memory>
#include "base.h"
#include "robot.h"
#include "message.h"
#include "constantes.h"
#include "geomod.h"
#include "gisement.h"
#include "constantes.h"


namespace
{
    std::vector<unsigned> E_uid;
    int compteur(0);
    bool autre_triangulation(false);
}

bool verif_uid(const unsigned);
bool lecture_bool(std::stringstream&);

std::shared_ptr<Prospection> creation_robot_prospection(unsigned, std::stringstream&);
std::shared_ptr<Forage> creation_robot_forage(unsigned, std::stringstream&);
std::shared_ptr<Transport> creation_robot_transport(unsigned, std::stringstream&);
std::shared_ptr<Communication> creation_robot_communication(unsigned,
															std::stringstream&);

bool init_liste_propecteur(int, std::ifstream&,
						   std::vector<std::shared_ptr<Prospection>>&);
bool init_liste_forage(int, std::ifstream&,
					   std::vector<std::shared_ptr<Forage>>&);
bool init_liste_transport(int, std::ifstream&,
						  std::vector<std::shared_ptr<Transport>>&);
bool init_liste_communication(int, std::ifstream&, Cercle&,
							  std::vector<std::shared_ptr<Communication>>&);

bool communication_centre(std::vector<std::shared_ptr<Communication>>&, Cercle&);

void test_adjacence(std::vector<std::shared_ptr<Robot>>&, std::shared_ptr<Robot>&,
                    std::shared_ptr<Robot>&);


///////////////////////////////////////////////////////////////////////////////////////
/*
//================================================================================//
 //BASE//
//================================================================================//
*/
///////////////////////////////////////////////////////////////////////////////////////

Base creation_base(std::string line, std::ifstream & entree)
{
    std::stringstream data(line);
    double x(0.0), y(0.0), ressources(0.0);
    int nbP(0), nbF(0), nbT(0),nbC(0);
    data>>x>>y>>ressources>>nbP>>nbF>>nbT>>nbC;
    Base base(x, y, ressources, nbP, nbF, nbT, nbC, entree);
    return base;
}

//================================================================================//

Base::Base(double x, double y, double ressources,
           int nbP, int nbF, int nbT, int nbC, std::ifstream & entree )
:centre(x, y, rayon_base), ressources(ressources), error_base(false), active(true),
 nbP(nbP), nbF(nbF), nbT(nbT), nbC(nbC)
{
    if(ressources<=0)
    {
        active=false;
    }
    if(active)
    {
        if(nbP!=0)
        {
            if(init_liste_propecteur(nbP, entree, this->E_P))
            {
                error_base=true;
            }
            
            for(int i(0); i<nbP; ++i)
            {
                (this->E_R).push_back(this->E_P[i]);
            }
        }
        if(nbF!=0)
        {
            if(init_liste_forage(nbF, entree, this->E_F))
            {
                error_base=true;
            }
            for(int i(0); i<nbF; ++i)
            {
                (this->E_R).push_back(this->E_F[i]);
            }
        }
        if(nbT!=0)
        {
            if(init_liste_transport(nbT, entree, this->E_T))
            {
                error_base=true;
            }
            for(int i(0); i<nbT; ++i)
            {
                (this->E_R).push_back(this->E_T[i]);
            }
        }
        if(nbC!=0)
        {
            if(init_liste_communication(nbC, entree, this->centre, this->E_C))
            {
                error_base=true;
            }
            for(int i(0); i<nbC; ++i)
            {
                (this->E_R).push_back(this->E_C[i]);
            }
        }
        else
        {
            std::cout<<message::missing_robot_communication(x, y);
            error_base=true;
        }
    }
    E_uid.clear();
}

/*
//================================================================================//
 //MÉTHODES EVOLUTION DE LA SIMULATION//
//================================================================================//
*/

void Base::update_voisin(Base& base_2)
{
    for(int i(0); i<nbP+nbF+nbT+nbC; ++i)
    {
        base_2.test_voisin(E_R[i]);
    }
}

//================================================================================//

void Base::test_voisin(std::shared_ptr<Robot>& robot_depart)
{
    for(auto robot : E_R)
    {
        robot->set_visited(false);
    }
    robot_depart->vide_adj();
    //robot_depart->ajoute_liste_adjacence(robot_depart);
    test_adjacence(E_R, robot_depart, robot_depart);//Fonction récursive
}

//================================================================================//

void test_adjacence(std::vector<std::shared_ptr<Robot>>& E_R,
                    std::shared_ptr<Robot>& robot_test,
                    std::shared_ptr<Robot>& robot_depart)
{
    robot_test->set_visited(true);
    for(auto robot : E_R)
    {
        if(robot->get_visited()==false and robot->communication(robot_test))//J'ai trouvé mon erreur, j'avais mal fait ma fonction de test communication
        {
            if(robot_depart->in_L_adj(robot)==false)
            {
                robot_depart->ajoute_liste_adjacence(robot);
                test_adjacence(E_R, robot, robot_depart);
            }
        }
    }
}

//================================================================================//

void Base::connexion()
{
    int robot_index(0);
    
    for(int i(0); i<nbC; ++i)
    {
        if(E_C[i]->get_position().same_position(centre.get_centre()))
        {
            robot_index=i;
            break;
        }
    }
    E_remote.clear();
    E_autonomous.clear();
    
    E_C[robot_index]->creation_remote_autonomous(E_remote, E_autonomous, E_R,
                                                 E_C[robot_index]);
    
}

//================================================================================//

void Base::maintenance()
{
    for(auto& robot : E_R)
    {
        if(centre.point_appartient(robot->get_position()))
        {
            ressources=ressources-cost_repair*robot->get_dp();
            robot->init_dp();
        }
    }
}

//================================================================================//

void Base::creation()
{
	
	compteur+=1;
	
	std::cout<<"le comteur vaut: "<<compteur<<std::endl;
	if(ressources>250 and E_C.size()<=100)
	{
		switch(compteur%3)
		{
			case 0: if (autre_triangulation)
					{
						creation_robots4(compteur);
					} else {
						creation_robots1(compteur);
					}
			        break;
			case 1: if (autre_triangulation)
					{
						creation_robots5(compteur);
					} else {
						creation_robots2(compteur);
					}
			        break;
			case 2: if (autre_triangulation)
					{
						creation_robots6(compteur);
					} else {
						creation_robots3(compteur);
					}
			        break;
			
		}
		                                               
	}
	if(compteur==6)
	{
		compteur=0;
		autre_triangulation=(!autre_triangulation);
		std::cout<<"le bool vaut: "<<autre_triangulation<<"\n";
	}
}

//================================================================================//

void Base::creation_robots1(int i)//Chaque scénario va créer au max 3 robots
{
	std::cout<<E_R.size()<<"\n";
	std::cout<<ressources<<"\n";
	std::shared_ptr<Communication> c1(new Communication(E_R.size()+1, 0, 
		                                                    centre.get_x(), 
		                                                    centre.get_y(),
                                                            centre.get_x()+295/2*i,
		                                                    centre.get_y()+295*i*(sqrt(3)/2),
		                                                     false));
	ressources-=cost_com;
	std::cout<<ressources<<"\n";
	E_C.push_back(c1);
	E_R.push_back(c1);
	std::cout<<E_R.size()<<"\n";
	std::shared_ptr<Communication> c2(new Communication(E_R.size()+1, 0, centre.get_x(), 
	                                                    centre.get_y(),
															   centre.get_x()-295/2*i,
	                                                    centre.get_y()+295*i*(sqrt(3)/2),
															   false));
	ressources-=cost_com;
	E_C.push_back(c2);
	E_R.push_back(c2);
	std::cout<<E_R.size()<<"\n";
	std::cout<<ressources<<"\n";
	std::shared_ptr<Prospection> p(new Prospection(E_R.size()+1, 0, centre.get_x(),
												  centre.get_y(), centre.get_x(),
												  centre.get_y()-295*i,
												   false, false, false));
	ressources-=cost_prosp;
	E_P.push_back(p);
	E_R.push_back(p);                                                
	std::cout<<E_R.size()<<"\n";
	std::cout<<ressources<<"\n";    
}

//================================================================================//

void Base::creation_robots2(int i)//Chaque scénario va créer au max 3 robots
{
	std::cout<<E_R.size()<<"\n";
	std::cout<<ressources<<"\n";
	std::shared_ptr<Communication> c1(new Communication(E_R.size()+1, 0, 
												centre.get_x(), 
												centre.get_y(),
												 centre.get_x()-295/2*i,
	                                             centre.get_y()+295*i*(sqrt(3)/2), false));
	ressources-=cost_com;
	std::cout<<ressources<<"\n";
	E_C.push_back(c1);
	E_R.push_back(c1);
	std::cout<<E_R.size()<<"\n";
	std::shared_ptr<Communication> c2(new Communication(E_R.size()+1, 0, centre.get_x(), 
												centre.get_y(),
											    centre.get_x(),
												centre.get_y()-295*i,
													   false));
	ressources-=cost_com;
	E_C.push_back(c2);
	E_R.push_back(c2);
	std::cout<<E_R.size()<<"\n";
	std::cout<<ressources<<"\n";
	std::shared_ptr<Prospection> p(new Prospection(E_R.size()+1, 0, centre.get_x(),
										  centre.get_y(), centre.get_x()+295/2*i,
		                                  centre.get_y()+295*i*(sqrt(3)/2),
										   false, false, false));
	ressources-=cost_prosp;
	E_P.push_back(p);
	E_R.push_back(p);                                                
	std::cout<<E_R.size()<<"\n";
	std::cout<<ressources<<"\n";    
}

//================================================================================//

void Base::creation_robots3(int i)//Chaque scénario va créer au max 3 robots
{
	std::cout<<E_R.size()<<"\n";
	std::cout<<ressources<<"\n";
	std::shared_ptr<Communication> c1(new Communication(E_R.size()+1, 0, 
												centre.get_x(), 
												centre.get_y(),
												centre.get_x(),
												centre.get_y()-295*i, false));
	ressources-=cost_com;
	std::cout<<ressources<<"\n";
	E_C.push_back(c1);
	E_R.push_back(c1);
	std::cout<<E_R.size()<<"\n";
	std::shared_ptr<Communication> c2(new Communication(E_R.size()+1, 0, centre.get_x(), 
												centre.get_y(),
													   centre.get_x()+295/2*i,
		                                  centre.get_y()+295*i*(sqrt(3)/2),
													   false));
	ressources-=cost_com;
	E_C.push_back(c2);
	E_R.push_back(c2);
	std::cout<<E_R.size()<<"\n";
	std::cout<<ressources<<"\n";
	std::shared_ptr<Prospection> p(new Prospection(E_R.size()+1, 0, centre.get_x(),
										  centre.get_y(), centre.get_x()-295/2*i,
	                                             centre.get_y()+295*i*(sqrt(3)/2),
										   false, false, false));
	ressources-=cost_prosp;
	E_P.push_back(p);
	E_R.push_back(p);                                                
	std::cout<<E_R.size()<<"\n";
	std::cout<<ressources<<"\n";    
}

//================================================================================//

void Base::creation_robots4(int i)//Chaque scénario va créer au max 3 robots
{
	std::cout<<E_R.size()<<"\n";
	std::cout<<ressources<<"\n";
	std::shared_ptr<Communication> c1(new Communication(E_R.size()+1, 0, 
												centre.get_x(), 
												centre.get_y(),
												centre.get_x()+295*i,
												centre.get_y(), false));
	ressources-=cost_com;
	std::cout<<ressources<<"\n";
	E_C.push_back(c1);
	E_R.push_back(c1);
	std::cout<<E_R.size()<<"\n";
	std::shared_ptr<Communication> c2(new Communication(E_R.size()+1, 0, centre.get_x(), 
												centre.get_y(),
												centre.get_x()+295*(sqrt(2)/2)*i,
	                                            centre.get_y()+295*(sqrt(2)/2)*i,
													   false));
	ressources-=cost_com;
	E_C.push_back(c2);
	E_R.push_back(c2);
	std::cout<<E_R.size()<<"\n";
	std::cout<<ressources<<"\n";
	std::shared_ptr<Prospection> p(new Prospection(E_R.size()+1, 0, centre.get_x(),
										  centre.get_y(), centre.get_x()+295*(sqrt(2)/2)*i,
	                                            centre.get_y()-295*(sqrt(2)/2)*i,
										   false, false, false));
	ressources-=cost_prosp;
	E_P.push_back(p);
	E_R.push_back(p);                                                
	std::cout<<E_R.size()<<"\n";
	std::cout<<ressources<<"\n";    
}

//================================================================================//

void Base::creation_robots5(int i)//Chaque scénario va créer au max 3 robots
{
	std::cout<<E_R.size()<<"\n";
	std::cout<<ressources<<"\n";
	std::shared_ptr<Communication> c1(new Communication(E_R.size()+1, 0, 
												centre.get_x(), 
												centre.get_y(),
												centre.get_x()+295*(sqrt(2)/2)*i,
	                                            centre.get_y()-295*(sqrt(2)/2)*i, false));
	ressources-=cost_com;
	std::cout<<ressources<<"\n";
	E_C.push_back(c1);
	E_R.push_back(c1);
	std::cout<<E_R.size()<<"\n";
	std::shared_ptr<Communication> c2(new Communication(E_R.size()+1, 0, centre.get_x(), 
												centre.get_y(),
												centre.get_y(),
												centre.get_x()+295*i,
													   false));
	ressources-=cost_com;
	E_C.push_back(c2);
	E_R.push_back(c2);
	std::cout<<E_R.size()<<"\n";
	std::cout<<ressources<<"\n";
	std::shared_ptr<Prospection> p(new Prospection(E_R.size()+1, 0, centre.get_x(),
										  centre.get_y(), centre.get_x()+295*(sqrt(2)/2)*i,
	                                            centre.get_y()+295*(sqrt(2)/2)*i,
										   false, false, false));
	ressources-=cost_prosp;
	E_P.push_back(p);
	E_R.push_back(p);                                                
	std::cout<<E_R.size()<<"\n";
	std::cout<<ressources<<"\n";    
}

//================================================================================//

void Base::creation_robots6(int i)//Chaque scénario va créer au max 3 robots
{
	std::cout<<E_R.size()<<"\n";
	std::cout<<ressources<<"\n";
	std::shared_ptr<Communication> c1(new Communication(E_R.size()+1, 0, 
												centre.get_x(), 
												centre.get_y(),
												centre.get_x()+295*(sqrt(2)/2)*i,
	                                            centre.get_y()+295*(sqrt(2)/2)*i, false));
	ressources-=cost_com;
	std::cout<<ressources<<"\n";
	E_C.push_back(c1);
	E_R.push_back(c1);
	std::cout<<E_R.size()<<"\n";
	std::shared_ptr<Communication> c2(new Communication(E_R.size()+1, 0, centre.get_x(), 
												centre.get_y(),
											    centre.get_x()+295*(sqrt(2)/2)*i,
	                                            centre.get_y()-295*(sqrt(2)/2)*i,
													   false));
	ressources-=cost_com;
	E_C.push_back(c2);
	E_R.push_back(c2);
	std::cout<<E_R.size()<<"\n";
	std::cout<<ressources<<"\n";
	std::shared_ptr<Prospection> p(new Prospection(E_R.size()+1, 0, centre.get_x(),
										  centre.get_y(), centre.get_y(),
												centre.get_x()+295*i,
										   false, false, false));
	ressources-=cost_prosp;
	E_P.push_back(p);
	E_R.push_back(p);                                                
	std::cout<<E_R.size()<<"\n";
	std::cout<<ressources<<"\n";    
}

//================================================================================//

void Base::update_autonomous()
{
    for(auto& robot : E_autonomous)
    {
        robot->deplacement(centre.get_x(), centre.get_y());
    }
}

//================================================================================//

void Base::update_remote()
{
    for(auto& robot : E_remote)
    {
        robot->deplacement(centre.get_x(), centre.get_y());
    }
}

//================================================================================//

void Base::decouverte_gisement(std::vector<Gisement>& Eg)
{
    for(auto& robot : E_P)
    {
        if(not(robot->get_found()) and not(robot->get_atteint()))
        {
            for(auto& gisement : Eg)
            {
                if(gisement.get_field().point_appartient(robot->get_position()))
                {
                    robot->add_gisement(gisement);
                }
            }
        }
    }
}

//================================================================================//

void Base::destruction()
{
    E_R.clear();
    E_P.clear();
    E_F.clear();
    E_T.clear();
    E_C.clear();
    nbP=0;
    nbF=0;
    nbT=0;
    nbC=0;
    active=false;
}


/*
//================================================================================//
 //MÉTHODES D'AFFICHAGE//
//================================================================================//
*/

void Base::affiche_texte()
{
    std::cout<<"\t"<<centre.get_x()<<" "<<centre.get_y()<<" "
             <<ressources<<" "<<nbP<<" "<<nbF<<" "<<nbT<<" "<<nbC<<std::endl;
    for(auto& prospection : E_P)
    {
        prospection->affiche_texte();
    }
    std::cout<<std::endl;
    for(auto& forage : E_F)
    {
        forage->affiche_texte();
    }
    std::cout<<std::endl;
    for(auto& transport : E_T)
    {
        transport->affiche_texte();
    }
    std::cout<<std::endl;
    for(auto& communication : E_C)
    {
        communication->affiche_texte();
    }
    std::cout<<std::endl;
}

//================================================================================//

void Base::affiche_texte(std::ofstream& sortie)
{
    sortie<<"\t"<<centre.get_x()<<" "<<centre.get_y()<<" "
             <<ressources<<" "<<nbP<<" "<<nbF<<" "<<nbT<<" "<<nbC<<std::endl;
    for(auto& prospection : E_P)
    {
        prospection->affiche_texte(sortie);
    }
    sortie<<std::endl;
    for(auto& forage : E_F)
    {
        forage->affiche_texte(sortie);
    }
    sortie<<std::endl;
    for(auto& transport : E_T)
    {
        transport->affiche_texte(sortie);
    }
    sortie<<std::endl;
    for(auto& communication : E_C)
    {
        communication->affiche_texte(sortie);
    }
    sortie<<std::endl;
    
}

//================================================================================//

void Base::affiche_dessin(int index)
{
    if(active)
    {
        centre.affiche_dessin(3, index);
        for(auto& prospection : E_P)
        {
            prospection->affiche_dessin(index);
        }
        for(auto& forage : E_F)
        {
            forage->affiche_dessin(index);
        }
        for(auto& transport : E_T)
        {
            transport->affiche_dessin(index);
        }
        for(auto& communication : E_C)
        {
            communication->affiche_dessin(index);
        }
    }
}

//================================================================================//

void Base::affiche_range()
{
    for(auto& robot : E_R)
    {
        robot->affiche_range();
    }
}

//================================================================================//

void Base::affiche_link()
{
    for(auto& robot : E_R)
    {
        robot->affiche_link();
    }
}


/*
//================================================================================//
 //GETTERS//
//================================================================================//
*/

Cercle Base::get_centre()
{
    return centre;
}

//================================================================================//

double Base::get_x()
{
    return centre.get_x();
}

//================================================================================//

double Base::get_y()
{
    return centre.get_y();
}

//================================================================================//

bool Base::get_error_base()
{
    return error_base;
}

//================================================================================//

bool Base::get_active()
{
    return active;
}


///////////////////////////////////////////////////////////////////////////////////////
/*
//================================================================================//
 //PROSPECTION//
//================================================================================//
*/
///////////////////////////////////////////////////////////////////////////////////////

bool init_liste_propecteur(int nbP, std::ifstream& entree,
						   std::vector<std::shared_ptr<Prospection>>& E_P)
{
    bool error(false);
    std::string line;
    int test_nbP(0);
    
    while(test_nbP<nbP)
    {
        getline(entree>>std::ws, line);
        
        std::stringstream data(line);
        unsigned uid(0);
        
        if(not(data>>uid))
        {
            continue;
            
        }
        if(verif_uid(uid))//Si le booléen de retour est à vrai on peut direct dire que ça marche pas
        {
            error=true;
        }
        E_P.push_back(creation_robot_prospection(uid, data));
        ++test_nbP;
    }
    return error;
}

//================================================================================//

std::shared_ptr<Prospection> creation_robot_prospection(unsigned uid,
                                                        std::stringstream& data)
{
    std::string booleen;
    double dp(0.), x(0.), y(0.), xb(0.), yb(0.);
    bool atteint(false), retour(false), found(false);
    data>>dp>>x>>y>>xb>>yb;
    
    atteint=lecture_bool(data);
    retour=lecture_bool(data);
    found=lecture_bool(data);
    
    if(found)
    {
        double xg(0.0), yg(0.0), rayong(0.0), capaciteg(0.0);
        data>>xg>>yg>>rayong>>capaciteg;
        std::shared_ptr<Prospection> p(new Prospection(uid, dp, x, y, xb, yb,
                                                       atteint, retour, found,
                                                       xg, yg,
                                                       rayong, capaciteg));
        return p;
    }
    else
    {
        std::shared_ptr<Prospection> p(new Prospection(uid, dp, x, y, xb, yb,
                                                       atteint, retour, found));
        return p;
    }
}


///////////////////////////////////////////////////////////////////////////////////////
/*
//================================================================================//
 //FORAGE//
//================================================================================//
*/
///////////////////////////////////////////////////////////////////////////////////////

bool init_liste_forage(int nbF, std::ifstream& entree,
                       std::vector<std::shared_ptr<Forage>>& E_F)
{
    bool error(false);
    std::string line;
    int test_nbF(0);
    
    while(test_nbF<nbF)
    {
        getline(entree>>std::ws, line);
        std::stringstream data(line);
        unsigned uid(0);
        if(not(data>>uid))
        {
            continue;
        }
        if(verif_uid(uid))
        {
            error=true;
        }
        E_F.push_back(creation_robot_forage(uid, data));
        
        ++test_nbF;
    }
    return error;
}

//================================================================================//

std::shared_ptr<Forage> creation_robot_forage(unsigned uid, std::stringstream& data)
{
    double dp(0.0), x(0.0), y(0.0), xb(0.0), yb(0.0);
    bool atteint(false);
    data>>dp>>x>>y>>xb>>yb;
    atteint=lecture_bool(data);
    std::shared_ptr<Forage> forage(new Forage(uid, dp, x, y, xb, yb, atteint));
    return forage;
}

///////////////////////////////////////////////////////////////////////////////////////
/*
//================================================================================//
 //TRANSPORT//
//================================================================================//
*/
///////////////////////////////////////////////////////////////////////////////////////

bool init_liste_transport(int nbT, std::ifstream& entree,
                          std::vector<std::shared_ptr<Transport>>& E_T)
{
    bool error(false);
    std::string line;
    int test_nbT(0);
    
    while(test_nbT<nbT)
    {
        getline(entree>>std::ws, line);
        std::stringstream data(line);
        unsigned uid(0);
        if(not(data>>uid))
        {
            continue;
        }
        
        if(verif_uid(uid))
        {
            error=true;
        }
        E_T.push_back(creation_robot_transport(uid, data));
        
        ++test_nbT;
    }
    return error;
}

//================================================================================//

std::shared_ptr<Transport> creation_robot_transport(unsigned uid,
                                                    std::stringstream& data)
{
    double dp(0.0), x(0.0), y(0.0), xb(0.0), yb(0.0);
    bool atteint(false), retour(false);
    data>>dp>>x>>y>>xb>>yb;
    atteint=lecture_bool(data);
    retour=lecture_bool(data);
    std::shared_ptr<Transport> transport(new Transport(uid, dp, x, y, xb, yb,
                                                       atteint, retour));
    return transport;
}


///////////////////////////////////////////////////////////////////////////////////////
/*
//================================================================================//
 //COMMUNICATION//
//================================================================================//
*/
///////////////////////////////////////////////////////////////////////////////////////

bool init_liste_communication(int nbC, std::ifstream& entree, Cercle& centre,
                              std::vector<std::shared_ptr<Communication>>& E_C)
{
    bool error(false);
    std::string line;
    int test_nbC(0);
    
    while(test_nbC<nbC)
    {
        getline(entree>>std::ws, line);
        std::stringstream data(line);
        int uid(0);
        if(not(data>>uid))
        {
            continue;
        }
        if(verif_uid(uid))
        {
            error=true;
        }
        E_C.push_back(creation_robot_communication(uid, data));
        ++test_nbC;
        
    }
    if(communication_centre(E_C, centre))
    {
        error=true;
    }
    return error;
}

//================================================================================//

std::shared_ptr<Communication> creation_robot_communication(unsigned uid,
                                                            std::stringstream& data)
{
    double dp(0.0), x(0.0), y(0.0), xb(0.0), yb(0.0);
    bool atteint(false);
    data>>dp>>x>>y>>xb>>yb;
    atteint=lecture_bool(data);
    
    std::shared_ptr<Communication> communication(new Communication(uid, dp, x, y,
                                                                   xb, yb,
                                                                   atteint));
    return communication;
}

//================================================================================//

bool communication_centre(std::vector<std::shared_ptr<Communication>>& E_C,
                          Cercle& centre)
{
    bool centre_ok(false), error(false);
    for(auto& robot : E_C)
    {
        if((robot->get_position()).same_position(centre.get_centre()))
        {
            centre_ok=true;
        }
    }
    if(not(centre_ok))
    {
        std::cout<<message::missing_robot_communication(centre.get_x(),
                                                        centre.get_y());
        error=true;
    }
    return error;
}


///////////////////////////////////////////////////////////////////////////////////////
/*
//================================================================================//
 //FONCTIONS SUPPLEMENTAIRES//
//================================================================================//
*/
///////////////////////////////////////////////////////////////////////////////////////

bool verif_uid(const unsigned uid)
{
    bool error(false);
	for(auto& element : E_uid)
	{
		if(element==uid)
		{
			std::cout<<message::identical_robot_uid(uid);
			error=true;
		}
	}
    E_uid.push_back(uid);
    return error;
}

//================================================================================//

bool lecture_bool(std::stringstream& data)
{
    std::string booleen;
    data>>booleen;
    if(booleen=="false")
    {
        return false;
    }
    else
    {
        return true;
    }
}

//================================================================================//

int Base::get_nbP()
{
    return nbP;
}

//================================================================================//

int Base::get_nbF()
{
    return nbF;
}

//================================================================================//

int Base::get_nbT()
{
    return nbT;
}

//================================================================================//

int Base::get_nbC()
{
    return nbC;
}

//================================================================================//

double Base::get_ressources()
{
    return ressources;
}
