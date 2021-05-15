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
    void affiche_texte();
    void affiche_texte(std::ofstream&);
    void affiche_dessin(int);
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
    
private:
    std::vector<std::shared_ptr<Robot>> E_R;
	std::vector<std::shared_ptr<Prospection>> E_P;
	std::vector<std::shared_ptr<Forage>> E_F;
	std::vector<std::shared_ptr<Transport>> E_T;
	std::vector<std::shared_ptr<Communication>> E_C;
	Cercle centre;
    double ressources;
    bool error_base;
	int nbP, nbF, nbT, nbC;
};

//================================================================================//

Base creation_base(std::string, std::ifstream &);

#endif
