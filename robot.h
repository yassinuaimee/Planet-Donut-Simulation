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
#include <memory>
#include "gisement.h"

class Robot
{
public:
    Robot(unsigned, double, double, double, double, double, bool);
    double get_x();
    double get_y();
    Point get_position();
    double get_xb();
    double get_yb();
    bool get_visited();
    void set_visited(bool);
    void affiche_range();
    void affiche_link();
    void affiche_adjacence();//Fonction qui fait office de stub, ça marche pas du tout pour le moment mskn
    bool communication(std::shared_ptr<Robot>&);
    void ajoute_liste_adjacence(std::shared_ptr<Robot>&);
    bool in_L_adj(std::shared_ptr<Robot>&);
    
protected:
	unsigned uid;
	double dp;
	Point position;
	Point but;
	bool atteint;
    bool visited;
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
    void affiche_texte();
    void affiche_texte(std::ofstream&);
    void affiche_dessin(int);
    
private:
    bool retour;
	bool found;
	Gisement gisement;
};

//================================================================================//

class Forage : public Robot
{
public:
    Forage(unsigned, double, double, double, double, double, bool);
    void affiche_texte();
    void affiche_texte(std::ofstream&);
    void affiche_dessin(int);
};

//================================================================================//

class Transport : public Robot
{
public:
    Transport(unsigned, double, double, double, double, double, bool, bool);
    void affiche_texte();
    void affiche_texte(std::ofstream&);
    void affiche_dessin(int);

private:
	bool retour;
};

//================================================================================//

class Communication : public Robot
{
public:
    Communication(unsigned, double, double, double, double, double, bool);
    void affiche_texte();
    void affiche_texte(std::ofstream&);
    void affiche_dessin(int);
    void creation_remote_autonomous(std::vector<std::shared_ptr<Robot>>&,
                                    std::vector<std::shared_ptr<Robot>>&,
                                    std::vector<std::shared_ptr<Robot>>&);
};
#endif
