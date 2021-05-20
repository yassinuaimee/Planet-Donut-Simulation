/* Nom:
 * base.h
 *
 * Auteurs:
 * Yassin Al-Nuaimee
 * Adrien Maillet Gonzalez
 *
 * Version: 12
 *
 *
 */

#ifndef BASE_H_INCLUDED
#define BASE_H_INCLUDED

#include <vector>
#include <string>
#include <memory>
#include <fstream>
#include "robot.h"

class Base
{
public:
    Base(double, double, double, int, int, int, int, std::ifstream & );
    
    //CONNECTION
    void update_voisin(Base&);
    void test_voisin(std::shared_ptr<Robot>&);
    void connexion();
    
    //MAINTENANCE
    void maintenance();
    
    
    void update_prospection();
    void update_forage();
    void update_transport();
    void update_communicaton();
    void evolution(std::vector<Gisement>&);
    
    
    void contact_gisement_forage(std::vector<Gisement>&);
    void contact_gisement_transport(std::vector<Gisement> & Eg);
    
    
    void creation();
    void destruction();
    
    //AFFICHAGE
    void affiche_texte();
    void affiche_texte(std::ofstream&);
    void affiche_dessin(int, bool);
    void affiche_range();
    void affiche_link();
    
    //GETTERS
    Cercle get_centre();
    double get_x();
    double get_y();
    int get_nbP();
    int get_nbF();
    int get_nbT();
    int get_nbC();
    double get_ressources();
    bool get_error_base();
    bool get_active();//Permet de verifier qu'une base est bien active
    
private:
    void liste_gisements(std::vector<Gisement>&);//Initialise la liste des gisements connus
    
    void deplacement_robots();
    
    
    
    //EVOLUTION diff ROBOTS
    void evolution_prospecteur(std::vector<Gisement>&);
    void evolution_forage(std::vector<Gisement>&);
    void evolution_transport(std::vector<Gisement>&);
    void evolution_communication();
    
    //Tous les algo on devrait les mettre dans le private je pense que c'est mieux
    void algo_prosp();
    void algo_decision_forage();
    void algo_decision_transport();
    void algo_placement_robot_com();
   
    void reinitialise(std::shared_ptr<Transport>);
    void reinitialise(std::shared_ptr<Prospection>);
    void refresh_nb_robots();
    
    void creation_communication();
    void creation_robCom1();
    void creation_robCom2();
    void creation_robCom3();
    void creation_robCom4();
    void creation_robCom5();
    void creation_robCom6();
    void creation_robCom7();
    void creation_robCom8();
    void creation_robCom9();
    void creation_robCom10();
    void creation_robCom11();
    void creation_robCom12();
    void creation_robCom13();
    void creation_robCom14();
    void creation_robCom15();
    void creation_robCom16();
    void creation_robCom17();
    
    void creation_prospection();
    void creation_prospection1();
    void creation_prospection2();
    void creation_prospection3();
    void creation_prospection4();
    void creation_prospection5();
    void relance_prospection();
    
	void creation_forage();
    void creation_forage1(int);//Au debut vraiment meme si on a pas beaucoup de ressources on envoi forage
	void creation_forage2(int);//On devient de plus en plus chiant avec les ressources
    void creation_forage3(int);
    
    
    void creation_transport();
    void creation_transport1();
	void creation_transport2();
    void creation_transport3();
    void relance_transport(std::shared_ptr<Transport>);



    
    
    std::vector<std::shared_ptr<Robot>> E_R;
	std::vector<std::shared_ptr<Prospection>> E_P;
	std::vector<std::shared_ptr<Forage>> E_F;
	std::vector<std::shared_ptr<Transport>> E_T;
	std::vector<std::shared_ptr<Communication>> E_C;
    std::vector<Gisement> E_G;
	Cercle centre;
    double ressources;
    bool error_base, active;
    int compteur_C;
	int nbP, nbF, nbT, nbC;
};

//================================================================================//

Base creation_base(std::string, std::ifstream &);

#endif
