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
    void affiche();
    Cercle get_centre();
    double get_x();
    double get_y();
    
private:
	std::vector<std::unique_ptr<Prospection>> E_P;
	std::vector<std::unique_ptr<Forage>> E_F;
	std::vector<std::unique_ptr<Transport>> E_T;
	std::vector<std::unique_ptr<Communication>> E_C;
	Cercle centre;
    Point point_centre;
    double ressources;
	int nbP, nbF, nbT, nbC;
};

//================================================================================//

Base creation_base(std::string, std::ifstream &);

#endif
