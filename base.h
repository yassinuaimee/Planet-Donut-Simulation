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
    void affiche_texte();
    void affiche_dessin(int);
    Cercle get_centre();
    double get_x();
    double get_y();
    int get_nbP();
    int get_nbF();
    int get_nbT();
    int get_nbC();
    double get_ressources();
    
private:
	std::vector<std::unique_ptr<Prospection>> E_P;
	std::vector<std::unique_ptr<Forage>> E_F;
	std::vector<std::unique_ptr<Transport>> E_T;
	std::vector<std::unique_ptr<Communication>> E_C;
	Cercle centre;
    double ressources;
	int nbP, nbF, nbT, nbC;
};

//================================================================================//

Base creation_base(std::string, std::ifstream &);

#endif
