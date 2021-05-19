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
    //int compteur(-1);
    std::shared_ptr<Communication> robot_centre;
    std::vector<std::shared_ptr<Robot>> L_remote;
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
compteur(-1), nbP(nbP), nbF(nbF), nbT(nbT), nbC(nbC)
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
    
    E_uid.clear();
}

/*
//================================================================================//
 //MÉTHODES CREATION LISTES ADJACENCE//
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
    for(auto& robot : E_R)
    {
        robot->set_visited(false);
    }
    robot_depart->vide_adj();
    test_adjacence(E_R, robot_depart, robot_depart);
}

//================================================================================//

void test_adjacence(std::vector<std::shared_ptr<Robot>>& E_R,
                    std::shared_ptr<Robot>& robot_test,
                    std::shared_ptr<Robot>& robot_depart)
{
    robot_test->set_visited(true);
    for(auto& robot : E_R)
    {
        if(robot->get_visited()==false and robot->communication(robot_test))
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
    robot_centre=E_C[robot_index];
    L_remote.clear();
    L_remote=robot_centre->get_adj();
    E_C[robot_index]->creation_connexion(E_R, E_C[robot_index]);//va associer les booleens remote ou pas remote
}


/*
//================================================================================//
 //MAINTENANCE//
//================================================================================//
*/

void Base::maintenance()
{
    for(auto& robot : E_R)
    {
        if(centre.get_centre().same_position(robot->get_position()) and
           robot->get_dp()>=10)
        {
            ressources=ressources-cost_repair*robot->get_dp();
            robot->init_dp();
        }
    }
}

/*
//================================================================================//
 //CREATION DE ROBOTS//
//================================================================================//
*/


void Base::creation()
{
    
    creation_forage();
    creation_transport();
    creation_communication();
	
                                                       
    nbP=(int) E_P.size();
    nbF=(int) E_F.size();
    nbT=(int) E_T.size();
    nbC=(int) E_C.size();
}



//================================================================================//

void Base::creation_forage()
{
    
    for(auto& prospecteur : E_P)
    {
        if(prospecteur->get_remote())
        {
            if(prospecteur->get_found() and
               not(prospecteur->get_forage_envoyer()))
            {
                Point but(prospecteur->get_centre_gisement());
                bool forage_deja_envoyer(false);
                for(auto& forage : E_F)
                {
                    if(forage->get_but().same_position(but))
                    {
                        forage_deja_envoyer=true;
                    }
                }
                if(not(forage_deja_envoyer))
                {
                    prospecteur->set_forage_envoyer(true);
                    unsigned size_E_R((unsigned)E_R.size());
                    std::shared_ptr<Forage> forage(new Forage(size_E_R+1,
                                                              0,
                                                              centre.get_x(),
                                                              centre.get_y(),
                                                              but.get_x(),
                                                              but.get_y(),
                                                              false));
                    E_F.push_back(forage);
                    E_R.push_back(forage);
                    nbF+=1;
                    return;
                }
            }
        }
    }
}

//================================================================================//

void Base::creation_transport()
{
    if(ressources>200 and nbT<2)
    {
        for(auto& forage : E_F)
        {
            if(forage->get_atteint())
            {
                unsigned size_E_R((unsigned)E_R.size());
                std::shared_ptr<Transport> transport(new Transport(size_E_R+1,
                                                          0,
                                                          centre.get_x(),
                                                          centre.get_y(),
                                                          forage->get_x(),
                                                          forage->get_y(),
                                                          false, false));
                E_T.push_back(transport);
                E_R.push_back(transport);
                nbT+=1;
            }
        }
    }
    else if(ressources>1000 and nbT<3)
    {
        
    }
}

//================================================================================//

void Base::creation_communication()
{
    compteur+=1;
    std::cout<<"le comteur vaut: "<<compteur<<std::endl;
    if(ressources>250 and E_C.size()<47 and compteur<16)
    {
        switch(compteur%16)
        {
            case 0: creation_robCom1();
                   break;
            case 1: creation_robCom2();
                   break;
            case 2: creation_robCom3();
                   break;
            case 3: creation_robCom4();
                   break;
            case 4: creation_robCom5();
                   break;
            case 5: creation_robCom6();
                   break;
            case 6 : creation_robCom7();
                   break;
            case 7: creation_robCom8();
                   break;
            case 8: creation_robCom9();
                   break;
            case 9: creation_robCom10();
                   break;
            case 10: creation_robCom11();
                   break;
            case 11: creation_robCom12();
                   break;
            case 12: creation_robCom13();
                   break;
            case 13: creation_robCom14();
                   break;
            case 14: creation_robCom15();
                   break;
            case 15: creation_robCom16();
                   break;
        }
    }
}



/*
//================================================================================//
 //DEPLACEMENTS ROBOTS//
//================================================================================//
*/

void Base::deplacement_robots()
{
    for(auto& robot : E_R)
    {
        robot->deplacement(centre.get_centre());
    }
}

//================================================================================//

void Base::update_prospection()
{
    for(auto& robot : E_P)
    {
        robot->deplacement(centre.get_centre());
    }
}

//================================================================================//

void Base::update_forage()
{
    for(auto& robot : E_F)
    {
        robot->deplacement(centre.get_centre());
    }
}

//================================================================================//

void Base::update_transport()
{
    for(auto& robot : E_T)
    {
        robot->deplacement(centre.get_centre());
        if(robot->get_atteint() and robot->get_retour())
            {
                std::cout<<"atteint = "<<robot->get_atteint()<<", retour : "<<robot->get_atteint()<<"\n";
                ressources=ressources+250;
                robot->reinitialise(E_G);
            }
    }
}

//================================================================================//

void Base::update_communicaiton()
{
    for(auto& robot : E_C)
    {
        robot->deplacement(centre.get_centre());
    }
}

/*
//================================================================================//
 //EVOLUTION DE LA SIMULATION//
//================================================================================//
*/

void Base::evolution(std::vector<Gisement>& Eg)
{
    
    liste_gisements(Eg);
    evolution_prospecteur(Eg);//Dans les fonctions evolution on va pouvoir changer les coordonnées du but en fonction de la situation
    //evolution_forage();
    //evolution_transport();
    //evolution_communication();
    //deplacement_robots();
    
    contact_gisement_forage(Eg);
    contact_gisement_transport(Eg);
    update_prospection();
    update_forage();
    update_transport();
    update_communicaiton();
}
//================================================================================//

void Base::liste_gisements(std::vector<Gisement>& Eg)//Creation de la liste avec tous les gisements connus CA MARCHE!!!!!!!!
{
    E_G.clear();
    std::vector<bool> bools;
    for(auto& robot : L_remote)
    {
        bools.clear();
        if(robot->get_type()==1)//Prospecteurs
        {
            bools=robot->get_bools();
            if(bools[2]==true)//represente le bool found
            {
                if(not(robot->get_gisement().in_E_G(E_G)))
                {
                    E_G.push_back(robot->get_gisement());
                }
            }
        }
        if(robot->get_type()==2)//Forage
        {
            bools=robot->get_bools();
            if(bools[0]==true)//représente le bool atteint
            {
                for(auto& gisement : Eg)
                {
                    if(gisement.get_field().point_appartient(robot->get_position()))
                    {
                        if(not(gisement.in_E_G(E_G)))
                        {
                            E_G.push_back(gisement);
                        }
                    }
                }
            }
        }
    }
}

//================================================================================//

void Base::evolution_prospecteur(std::vector<Gisement>& Eg)
{
    for(size_t i(0); i<E_P.size();++i)
    {
        if(not(E_P[i]->get_found()) and not(E_P[i]->get_atteint()))
        {
            for(auto& gisement : Eg)
            {
                if(gisement.get_field().point_appartient(E_P[i]->get_position()))
                {
                    E_P[i]->add_gisement(gisement);//On a deja la condition de taille du gisement à l'intérieur
                }
            }
        }
    }
    for(size_t i(0); i<E_P.size();++i)//Changement de direction des prospecteur si un gisement a deja été trouvé CA MARCHE!!!!!
    {
        if(E_P[i]->get_remote())
        {
            for(auto& gisement : E_G)
            {
                if(E_P[i]->get_atteint()==false and
                   E_P[i]->get_but().get_x() -300<gisement.get_x()
                   and E_P[i]->get_but().get_x()+300>gisement.get_x()
                   and E_P[i]->get_but().get_y() -300<gisement.get_y()
                   and E_P[i]->get_but().get_y()+300>gisement.get_y())
                {
                    std::cout<<"Passage dans la boucle changement de direction\n";
                    if(i-1>=0)
                    {
                        E_P[i]->set_but(E_P[i-1]->get_but().get_x()+300,
                                        E_P[i-1]->get_but().get_y()-300);
                    }
                    else if(E_P.size()>3)
                    {
                        E_P[i]->set_but(E_P[E_P.size()-2]->get_but().get_x()-300,
                                        E_P[E_P.size()-2]->get_but().get_y()+300);
                    }
                    else if(E_P.size()<=3)
                    {
                        E_P[i]->set_but(E_P[E_P.size()-1]->get_but().get_x()+300,
                                        E_P[E_P.size()-1]->get_but().get_y()-300);
                    }
                    else
                    {
                        E_P[i]->set_but(300,300);
                    }
                }
            }
        }
    }
    for(size_t i(0); i<E_P.size(); ++i)
    {
        if(E_P[i]->get_atteint()==true and E_P[i]->get_found()==false)
        {
            if(i-1>=0)
            {
                E_P[i]->set_but(E_P[i-1]->get_but().get_x()+300,
                                E_P[i-1]->get_but().get_y()-300);
            }
            else if(E_P.size()>3)
            {
                E_P[i]->set_but(E_P[E_P.size()-2]->get_but().get_x()+300,
                                E_P[E_P.size()-2]->get_but().get_y()-300);
            }
            else if(E_P.size()<=3)
            {
                E_P[i]->set_but(E_P[E_P.size()-1]->get_but().get_x()-300,
                                E_P[E_P.size()-1]->get_but().get_y()+300);
            }
            else
            {
                E_P[i]->set_but(300,300);
            }
        }
    }
}

//================================================================================//

void Base::contact_gisement_forage(std::vector<Gisement> & Eg)
{
    for(auto& forage : E_F)
    {
        if(forage->get_atteint()==false)
        {
            for(auto& gisement : Eg)
            {
                Point point(forage->get_position());
                Cercle gisement_cercle(gisement.get_field());
                if(gisement_cercle.point_appartient(point))
                {
                    forage->set_atteint(true);
                    return;
                }
            }
        }
    }
}

//================================================================================//

void Base::contact_gisement_transport(std::vector<Gisement> & Eg)
{
    for(auto& transport : E_T)
    {
        if(transport->get_atteint()==false and transport->get_retour()==false)
        {
            for(auto& gisement : Eg)
            {
                Point point(transport->get_position());
                Cercle gisement_cercle(gisement.get_field());
                if(gisement_cercle.point_appartient(point))
                {
                    transport->set_retour(true);
                    transport->set_atteint(false);
                    
                    gisement.set_capacite(250);
                    return;
                }
            }
        }
    }
}


/*
//================================================================================//
 //DESTRUCTION//
//================================================================================//
*/

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

void Base::affiche_dessin(int index, bool toggle_range)
{
    if(active)
    {
        centre.affiche_dessin(3, index);
        for(auto& prospection : E_P)
        {
            prospection->affiche_dessin(index, toggle_range);
        }
        for(auto& forage : E_F)
        {
            forage->affiche_dessin(index, toggle_range);
        }
        for(auto& transport : E_T)
        {
            transport->affiche_dessin(index, toggle_range);
        }
        for(auto& communication : E_C)
        {
            communication->affiche_dessin(index, toggle_range);
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
void Base::creation_robCom1()
{
    std::shared_ptr<Communication> c1(new Communication((unsigned) E_R.size()+1, 0,
                                                            centre.get_x(),
                                                            centre.get_y(),
                                                            centre.get_x()+295,
                                                            centre.get_y(),
                                                             false));
    ressources-=cost_com;
    E_C.push_back(c1);
    E_R.push_back(c1);
    std::shared_ptr<Communication> c2(new Communication((unsigned) E_R.size()+1, 0,
                                                        centre.get_x(),
                                                        centre.get_y(),
                                                        centre.get_x(),
                                                        centre.get_y()+295,
                                                               false));
    ressources-=cost_com;
    E_C.push_back(c2);
    E_R.push_back(c2);
    std::shared_ptr<Communication> c3(new Communication((unsigned) E_R.size()+1, 0,
                                                            centre.get_x(),
                                                            centre.get_y(),
                                                            centre.get_x()-295,
                                                            centre.get_y(),
                                                             false));
    ressources-=cost_com;
    E_C.push_back(c3);
    E_R.push_back(c3);
    
    
}

//================================================================================//
void Base::creation_robCom2()
{
    std::shared_ptr<Communication> c1(new Communication((unsigned) E_R.size()+1, 0,
                                                            centre.get_x(),
                                                            centre.get_y(),
                                                            centre.get_x(),
                                                            centre.get_y()-295,
                                                             false));
    ressources-=cost_com;
    E_C.push_back(c1);
    E_R.push_back(c1);
    std::shared_ptr<Communication> c2(new Communication((unsigned) E_R.size()+1, 0,
                                                        centre.get_x(),
                                                        centre.get_y(),
                                                        centre.get_x()+295,
                                                        centre.get_y()+295,
                                                               false));
    ressources-=cost_com;
    E_C.push_back(c2);
    E_R.push_back(c2);
    std::shared_ptr<Communication> c3(new Communication((unsigned) E_R.size()+1, 0,
                                                            centre.get_x(),
                                                            centre.get_y(),
                                                            centre.get_x()+295,
                                                            centre.get_y()-295,
                                                             false));
    ressources-=cost_com;
    E_C.push_back(c3);
    E_R.push_back(c3);
    
    
}

//================================================================================//

void Base::creation_robCom3()
{
    std::shared_ptr<Communication> c1(new Communication((unsigned) E_R.size()+1, 0,
                                                            centre.get_x(),
                                                            centre.get_y(),
                                                            centre.get_x()-295,
                                                            centre.get_y()-295,
                                                             false));
    ressources-=cost_com;
    E_C.push_back(c1);
    E_R.push_back(c1);
    std::shared_ptr<Communication> c2(new Communication((unsigned) E_R.size()+1, 0,
                                                        centre.get_x(),
                                                        centre.get_y(),
                                                        centre.get_x()-295,
                                                        centre.get_y()+295,
                                                               false));
    ressources-=cost_com;
    E_C.push_back(c2);
    E_R.push_back(c2);
    std::shared_ptr<Communication> c3(new Communication((unsigned) E_R.size()+1, 0,
                                                            centre.get_x(),
                                                            centre.get_y(),
                                                            centre.get_x()+2*295,
                                                            centre.get_y(),
                                                             false));
    ressources-=cost_com;
    E_C.push_back(c3);
    E_R.push_back(c3);
    
    
}

//================================================================================//

void Base::creation_robCom4()
{
    std::shared_ptr<Communication> c1(new Communication((unsigned) E_R.size()+1, 0,
                                                            centre.get_x(),
                                                            centre.get_y(),
                                                            centre.get_x()-2*295,
                                                            centre.get_y(),
                                                             false));
    ressources-=cost_com;
    E_C.push_back(c1);
    E_R.push_back(c1);
    std::shared_ptr<Communication> c2(new Communication((unsigned) E_R.size()+1, 0,
                                                        centre.get_x(),
                                                        centre.get_y(),
                                                        centre.get_x()+2*295,
                                                        centre.get_y()+295,
                                                               false));
    ressources-=cost_com;
    E_C.push_back(c2);
    E_R.push_back(c2);
    std::shared_ptr<Communication> c3(new Communication((unsigned) E_R.size()+1, 0,
                                                            centre.get_x(),
                                                            centre.get_y(),
                                                            centre.get_x()+2*295,
                                                            centre.get_y()-295,
                                                             false));
    ressources-=cost_com;
    E_C.push_back(c3);
    E_R.push_back(c3);
    
    
}

//================================================================================//

void Base::creation_robCom5()
{
    std::shared_ptr<Communication> c1(new Communication((unsigned) E_R.size()+1, 0,
                                                            centre.get_x(),
                                                            centre.get_y(),
                                                            centre.get_x(),
                                                            centre.get_y()-2*295,
                                                             false));
    ressources-=cost_com;
    E_C.push_back(c1);
    E_R.push_back(c1);
    std::shared_ptr<Communication> c2(new Communication((unsigned) E_R.size()+1, 0,
                                                        centre.get_x(),
                                                        centre.get_y(),
                                                        centre.get_x(),
                                                        centre.get_y()+2*295,
                                                               false));
    ressources-=cost_com;
    E_C.push_back(c2);
    E_R.push_back(c2);
    std::shared_ptr<Communication> c3(new Communication((unsigned) E_R.size()+1, 0,
                                                            centre.get_x(),
                                                            centre.get_y(),
                                                            centre.get_x()-2*295,
                                                            centre.get_y()+295,
                                                             false));
    ressources-=cost_com;
    E_C.push_back(c3);
    E_R.push_back(c3);
    
    
}

//================================================================================//

void Base::creation_robCom6()
{
    std::shared_ptr<Communication> c1(new Communication((unsigned) E_R.size()+1, 0,
                                                            centre.get_x(),
                                                            centre.get_y(),
                                                            centre.get_x()+295,
                                                            centre.get_y()-2*295,
                                                             false));
    ressources-=cost_com;
    E_C.push_back(c1);
    E_R.push_back(c1);
    std::shared_ptr<Communication> c2(new Communication((unsigned) E_R.size()+1, 0,
                                                        centre.get_x(),
                                                        centre.get_y(),
                                                        centre.get_x()-295,
                                                        centre.get_y()+2*295,
                                                               false));
    ressources-=cost_com;
    E_C.push_back(c2);
    E_R.push_back(c2);
    std::shared_ptr<Communication> c3(new Communication((unsigned) E_R.size()+1, 0,
                                                            centre.get_x(),
                                                            centre.get_y(),
                                                            centre.get_x()-2*295,
                                                            centre.get_y()-295,
                                                             false));
    ressources-=cost_com;
    E_C.push_back(c3);
    E_R.push_back(c3);
    
    
}

//================================================================================//

void Base::creation_robCom7()
{
    std::shared_ptr<Communication> c1(new Communication((unsigned) E_R.size()+1, 0,
                                                            centre.get_x(),
                                                            centre.get_y(),
                                                            centre.get_x()-295,
                                                            centre.get_y()-2*295,
                                                             false));
    ressources-=cost_com;
    E_C.push_back(c1);
    E_R.push_back(c1);
    std::shared_ptr<Communication> c2(new Communication((unsigned) E_R.size()+1, 0,
                                                        centre.get_x(),
                                                        centre.get_y(),
                                                        centre.get_x()+295,
                                                        centre.get_y()+2*295,
                                                               false));
    ressources-=cost_com;
    E_C.push_back(c2);
    E_R.push_back(c2);
    std::shared_ptr<Communication> c3(new Communication((unsigned) E_R.size()+1, 0,
                                                            centre.get_x(),
                                                            centre.get_y(),
                                                            centre.get_x()-2*295,
                                                            centre.get_y()+2*295,
                                                             false));
    ressources-=cost_com;
    E_C.push_back(c3);
    E_R.push_back(c3);
    
    
}

//================================================================================//

void Base::creation_robCom8()
{
    std::shared_ptr<Communication> c1(new Communication((unsigned) E_R.size()+1, 0,
                                                            centre.get_x(),
                                                            centre.get_y(),
                                                            centre.get_x()+2*295,
                                                            centre.get_y()+2*295,
                                                             false));
    ressources-=cost_com;
    E_C.push_back(c1);
    E_R.push_back(c1);
    std::shared_ptr<Communication> c2(new Communication((unsigned) E_R.size()+1, 0,
                                                        centre.get_x(),
                                                        centre.get_y(),
                                                        centre.get_x()-2*295,
                                                        centre.get_y()+2*295,
                                                               false));
    ressources-=cost_com;
    E_C.push_back(c2);
    E_R.push_back(c2);
    std::shared_ptr<Communication> c3(new Communication((unsigned) E_R.size()+1, 0,
                                                            centre.get_x(),
                                                            centre.get_y(),
                                                            centre.get_x()-2*295,
                                                            centre.get_y()-2*295,
                                                             false));
    ressources-=cost_com;
    E_C.push_back(c3);
    E_R.push_back(c3);
    
    
}

//================================================================================//

void Base::creation_robCom9()
{
    std::shared_ptr<Communication> c1(new Communication((unsigned) E_R.size()+1, 0,
                                                            centre.get_x(),
                                                            centre.get_y(),
                                                            centre.get_x()+3*295,
                                                            centre.get_y(),
                                                             false));
    ressources-=cost_com;
    E_C.push_back(c1);
    E_R.push_back(c1);
    std::shared_ptr<Communication> c2(new Communication((unsigned) E_R.size()+1, 0,
                                                        centre.get_x(),
                                                        centre.get_y(),
                                                        centre.get_x()-3*295,
                                                        centre.get_y(),
                                                               false));
    ressources-=cost_com;
    E_C.push_back(c2);
    E_R.push_back(c2);
    std::shared_ptr<Communication> c3(new Communication((unsigned) E_R.size()+1, 0,
                                                            centre.get_x(),
                                                            centre.get_y(),
                                                            centre.get_x(),
                                                            centre.get_y()-3*295,
                                                             false));
    ressources-=cost_com;
    E_C.push_back(c3);
    E_R.push_back(c3);
    
    
}

//================================================================================//

void Base::creation_robCom10()
{
    std::shared_ptr<Communication> c1(new Communication((unsigned) E_R.size()+1, 0,
                                                            centre.get_x(),
                                                            centre.get_y(),
                                                            centre.get_x()+3*295,
                                                            centre.get_y()+295,
                                                             false));
    ressources-=cost_com;
    E_C.push_back(c1);
    E_R.push_back(c1);
    std::shared_ptr<Communication> c2(new Communication((unsigned) E_R.size()+1, 0,
                                                        centre.get_x(),
                                                        centre.get_y(),
                                                        centre.get_x()-3*295,
                                                        centre.get_y()+295,
                                                               false));
    ressources-=cost_com;
    E_C.push_back(c2);
    E_R.push_back(c2);
    std::shared_ptr<Communication> c3(new Communication((unsigned) E_R.size()+1, 0,
                                                            centre.get_x(),
                                                            centre.get_y(),
                                                            centre.get_x()+295,
                                                            centre.get_y()-3*295,
                                                             false));
    ressources-=cost_com;
    E_C.push_back(c3);
    E_R.push_back(c3);
    
    
}

//================================================================================//

void Base::creation_robCom11()
{
    std::shared_ptr<Communication> c1(new Communication((unsigned) E_R.size()+1, 0,
                                                        centre.get_x(),
                                                        centre.get_y(),
                                                        centre.get_x()+3*295,
                                                        centre.get_y()-295,
                                                        false));
    ressources-=cost_com;
    E_C.push_back(c1);
    E_R.push_back(c1);
    std::shared_ptr<Communication> c2(new Communication((unsigned) E_R.size()+1, 0,
                                                        centre.get_x(),
                                                        centre.get_y(),
                                                        centre.get_x()-3*295,
                                                        centre.get_y()-295,
                                                        false));
    ressources-=cost_com;
    E_C.push_back(c2);
    E_R.push_back(c2);
    std::shared_ptr<Communication> c3(new Communication((unsigned) E_R.size()+1, 0,
                                                        centre.get_x(),
                                                        centre.get_y(),
                                                        centre.get_x()-295,
                                                        centre.get_y()-3*295,
                                                        false));
    ressources-=cost_com;
    E_C.push_back(c3);
    E_R.push_back(c3);
    
    
}

//================================================================================//

void Base::creation_robCom12()
{
    std::shared_ptr<Communication> c1(new Communication((unsigned) E_R.size()+1, 0,
                                                        centre.get_x(),
                                                        centre.get_y(),
                                                        centre.get_x()+3*295,
                                                        centre.get_y()+2*295,
                                                        false));
    ressources-=cost_com;
    E_C.push_back(c1);
    E_R.push_back(c1);
    std::shared_ptr<Communication> c2(new Communication((unsigned) E_R.size()+1, 0,
                                                        centre.get_x(),
                                                        centre.get_y(),
                                                        centre.get_x()-3*295,
                                                        centre.get_y()+2*295,
                                                        false));
    ressources-=cost_com;
    E_C.push_back(c2);
    E_R.push_back(c2);
    std::shared_ptr<Communication> c3(new Communication((unsigned) E_R.size()+1, 0,
                                                        centre.get_x(),
                                                        centre.get_y(),
                                                        centre.get_x()+2*295,
                                                        centre.get_y()-3*295,
                                                        false));
    ressources-=cost_com;
    E_C.push_back(c3);
    E_R.push_back(c3);
    
    
}

//================================================================================//

void Base::creation_robCom13()
{
    std::shared_ptr<Communication> c1(new Communication((unsigned) E_R.size()+1, 0,
                                                        centre.get_x(),
                                                        centre.get_y(),
                                                        centre.get_x()+3*295,
                                                        centre.get_y()-2*295,
                                                        false));
    ressources-=cost_com;
    E_C.push_back(c1);
    E_R.push_back(c1);
    std::shared_ptr<Communication> c2(new Communication((unsigned) E_R.size()+1, 0,
                                                        centre.get_x(),
                                                        centre.get_y(),
                                                        centre.get_x()-3*295,
                                                        centre.get_y()-2*295,
                                                        false));
    ressources-=cost_com;
    E_C.push_back(c2);
    E_R.push_back(c2);
    std::shared_ptr<Communication> c3(new Communication((unsigned) E_R.size()+1, 0,
                                                        centre.get_x(),
                                                        centre.get_y(),
                                                        centre.get_x()-2*295,
                                                        centre.get_y()-3*295,
                                                         false));
    ressources-=cost_com;
    E_C.push_back(c3);
    E_R.push_back(c3);
    
    
}


//================================================================================//

void Base::creation_robCom14()
{
    std::shared_ptr<Communication> c1(new Communication((unsigned) E_R.size()+1, 0,
                                                        centre.get_x(),
                                                        centre.get_y(),
                                                        centre.get_x()+3*295,
                                                        centre.get_y()-3*295,
                                                             false));
    ressources-=cost_com;
    E_C.push_back(c1);
    E_R.push_back(c1);
    std::shared_ptr<Communication> c2(new Communication((unsigned) E_R.size()+1, 0,
                                                        centre.get_x(),
                                                        centre.get_y(),
                                                        centre.get_x()-3*295,
                                                        centre.get_y()-3*295,
                                                               false));
    ressources-=cost_com;
    E_C.push_back(c2);
    E_R.push_back(c2);
    std::shared_ptr<Communication> c3(new Communication((unsigned) E_R.size()+1, 0,
                                                            centre.get_x(),
                                                            centre.get_y(),
                                                            centre.get_x()+3*295,
                                                            centre.get_y()+3*295,
                                                             false));
    ressources-=cost_com;
    E_C.push_back(c3);
    E_R.push_back(c3);
    
    
}

//================================================================================//

void Base::creation_robCom15()
{
    std::shared_ptr<Communication> c1(new Communication((unsigned) E_R.size()+1, 0,
                                                            centre.get_x(),
                                                            centre.get_y(),
                                                            centre.get_x()-295,
                                                            centre.get_y()-2*295,
                                                             false));
    ressources-=cost_com;
    E_C.push_back(c1);
    E_R.push_back(c1);
    std::shared_ptr<Communication> c2(new Communication((unsigned) E_R.size()+1, 0,
                                                        centre.get_x(),
                                                        centre.get_y(),
                                                        centre.get_x()-3*295,
                                                        centre.get_y()+3*295,
                                                               false));
    ressources-=cost_com;
    E_C.push_back(c2);
    E_R.push_back(c2);
    std::shared_ptr<Communication> c3(new Communication((unsigned) E_R.size()+1, 0,
                                                            centre.get_x(),
                                                            centre.get_y(),
                                                            centre.get_x()+295,
                                                            centre.get_y()+3*295,
                                                             false));
    ressources-=cost_com;
    E_C.push_back(c3);
    E_R.push_back(c3);
    
    
}

//================================================================================//

void Base::creation_robCom16()
{
    std::shared_ptr<Communication> c1(new Communication((unsigned) E_R.size()+1, 0,
                                                            centre.get_x(),
                                                            centre.get_y(),
                                                            centre.get_x()+2*295,
                                                            centre.get_y()-2*295,
                                                             false));
    ressources-=cost_com;
    E_C.push_back(c1);
    E_R.push_back(c1);
    std::shared_ptr<Communication> c2(new Communication((unsigned) E_R.size()+1, 0,
                                                        centre.get_x(),
                                                        centre.get_y(),
                                                        centre.get_x()-2*295,
                                                        centre.get_y()+3*295,
                                                               false));
    ressources-=cost_com;
    E_C.push_back(c2);
    E_R.push_back(c2);
    std::shared_ptr<Communication> c3(new Communication((unsigned) E_R.size()+1, 0,
                                                            centre.get_x(),
                                                            centre.get_y(),
                                                            centre.get_x()-295,
                                                            centre.get_y()+3*295,
                                                             false));
    ressources-=cost_com;
    E_C.push_back(c3);
    E_R.push_back(c3);
    
    
}

