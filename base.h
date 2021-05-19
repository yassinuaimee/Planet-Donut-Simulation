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
    
    void update_voisin(Base&);
    void test_voisin(std::shared_ptr<Robot>&);
    void connexion();
    void maintenance();
    void update_prospection();
    void update_forage();
    void update_transport();
    void update_communicaiton();
    void evolution(std::vector<Gisement>&);
    void contact_gisement_forage(std::vector<Gisement>&);
    void contact_gisement_transport(std::vector<Gisement> & Eg);
    void creation();
    void destruction();
    
    
    void affiche_texte();
    void affiche_texte(std::ofstream&);
    void affiche_dessin(int, bool);
    void affiche_range();
    void affiche_link();
    
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
    void liste_gisements(std::vector<Gisement>&);
    void deplacement_robots();
    void evolution_prospecteur(std::vector<Gisement>&);
    void evolution_forage();
    void evolution_transport();
    void evolution_communication();
    
    //Tous les algo on devrait les mettre dans le private je pense que c'est mieux
    void algo_prosp();
    void algo_decision_forage();
    void algo_decision_transport();
    void algo_placement_robot_com();
    void creation_robots1(int);
    void creation_robots2(int);
    void creation_robots3(int); //nous avons cree deux maniere de propager les robots et chaque maniere se diferencie en 3 methodes differentes
    void creation_robots4(int);
    void creation_robots5(int);
    void creation_robots6(int);
    void creation_communication();
    void creation_forage();
    void creation_transport();
    void reinitialise(std::shared_ptr<Transport>);
    void reinitialise(std::shared_ptr<Prospection>);
    
    
    std::vector<std::shared_ptr<Robot>> E_R;
	std::vector<std::shared_ptr<Prospection>> E_P;
	std::vector<std::shared_ptr<Forage>> E_F;
	std::vector<std::shared_ptr<Transport>> E_T;
	std::vector<std::shared_ptr<Communication>> E_C;
    std::vector<Gisement> E_G;
	Cercle centre;
    double ressources;
    bool error_base, active;
	int nbP, nbF, nbT, nbC;
};

//================================================================================//

Base creation_base(std::string, std::ifstream &);

#endif
