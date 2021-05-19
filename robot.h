/* Nom:
 * robot.h
 *
 * Auteurs:
 * Yassin Al-Nuaimee
 * Adrien Maillet Gonzalez
 *
 * Version: 12
 *
 *
 */

#ifndef ROBOT_H_INCLUDED
#define ROBOT_H_INCLUDED

#include <fstream>
#include <vector>
#include <array>
#include <memory>
#include "gisement.h"

class Robot
{
public:
    Robot(unsigned, double, double, double, double, double, bool, int);
    virtual ~Robot();
    virtual bool reach_max_dp() = 0;
    virtual void deplacement(Point);
    void vide_adj();
    virtual std::vector<bool> get_bools() = 0;
    virtual Gisement get_gisement();
   
    
    
    void affiche_range();
    
    void affiche_link();
    void affiche_adjacence();//Fonction qui fait office de stub, ça marche pas du tout pour le moment mskn
    bool communication(std::shared_ptr<Robot>&);
    void ajoute_liste_adjacence(std::shared_ptr<Robot>&);
    bool in_L_adj(std::shared_ptr<Robot>&);
    std::vector<Gisement> get_gisements();
    
    void set_remote(bool);
    bool get_remote();
    
    double get_x();
    double get_y();
    Point get_position();
    Point get_but();
    double get_xb();
    double get_yb();
    int get_type();
    unsigned get_uid();
    double get_dp();
    bool get_atteint();
    void init_dp();
    bool get_visited();
    void set_atteint(bool);
    void set_visited(bool);
    void set_but(Point);
    void set_but(double, double);
    
    
protected:
	unsigned uid;
	double dp;
	Point position;
	Point but;
	bool atteint;
    bool visited;
    int type;
    bool remote;
    std::vector<std::shared_ptr<Robot>> L_adjacence;
};

//================================================================================//

class Prospection : public Robot
{
public:
    Prospection(unsigned, double, double, double, double, double, 
				bool, bool, bool, 
				double, double, double, double);
    Prospection(unsigned, double, double, double, double, double, bool, bool, bool);
    bool reach_max_dp() override;
    void test_return_base(Point);
    void add_gisement(Gisement);
    void affiche_texte();
    void affiche_texte(std::ofstream&);
    void affiche_dessin(int, bool);
    bool get_found();
    Cercle get_cercle_gisement();
    Point get_centre_gisement();
    Gisement get_gisement() override;
    double get_ressources();
    double get_rayon_gisement();
    bool get_forage_envoyer();
    void set_forage_envoyer(bool);
    void deplacement(Point) override;
    std::vector<bool> get_bools() override;
    
private:
    bool retour;
	bool found;
	Gisement gisement;
    bool forage_envoyer;
};

//================================================================================//

class Forage : public Robot
{
public:
    Forage(unsigned, double, double, double, double, double, bool);
    bool reach_max_dp() override;
    void affiche_texte();
    void affiche_texte(std::ofstream&);
    void affiche_dessin(int, bool);
    std::vector<bool> get_bools() override;
};

//================================================================================//

class Transport : public Robot
{
public:
    Transport(unsigned, double, double, double, double, double, bool, bool);
    bool reach_max_dp() override;
    void affiche_texte();
    void affiche_texte(std::ofstream&);
    void affiche_dessin(int, bool);
    void deplacement(Point) override;
    bool get_retour();
    void set_retour(bool);
    void reinitialise(std::vector<Gisement>&);
    std::vector<bool> get_bools() override;


private:
	bool retour;
};

//================================================================================//

class Communication : public Robot
{
public:
    Communication(unsigned, double, double, double, double, double, bool);
    bool reach_max_dp() override;
    Point add_new_comm();
    void affiche_texte();
    void affiche_texte(std::ofstream&);
    void affiche_dessin(int, bool);
    void creation_connexion(std::vector<std::shared_ptr<Robot>>&,
                            std::shared_ptr<Communication>&);
    std::vector<bool> get_bools() override;
    std::vector<std::shared_ptr<Robot>> get_adj();
private:
    bool communication_ok;
};
#endif
