#ifndef GEOMOD_H_INCLUDED
#define GEOMOD_H_INCLUDED
#include <array>



class Cercle
{
public:
    bool point_appartient(std::array<double,2>);
    bool intersection_cercle(Cercle);
    std::array<double,2> get_centre();
    double get_rayon();
private:
    std::array<double,2> centre;
    double rayon;
};

bool equal_zero(double parametre);
void set_max(char*);
double get_max();
double  get_epsilon_zero();
double norme_plus_petit_vecteur(std::array<double,2> depart,std::array<double,2> arrivee,std::array<double,2> vecteur);
bool  test_egalite_points(std::array<double,2>,std::array<double,2>);
bool appartient_cercle(std::array<double,2>,std::array<double,2>);
#endif
