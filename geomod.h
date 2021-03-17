#ifndef GEOMOD_H_INCLUDED
#define GEOMOD_H_INCLUDED
#include <array>
using namespace std;
class Cercle
{
public:
    bool point_appartient(array<double,2>);
    bool intersection_cercle(Cercle);
    array<double,2> get_centre();
    double get_rayon();
private:
    array<double,2> centre;
    double rayon;
};



bool equal_zero(double parametre);
void set_max();
double get_max();
double norme_plus_petit_vecteur(array<double,2> depart,array<double,2> arrivee,array<double,2> vecteur);
bool  test_egalite_points(array<double,2>,array<double,2>);
bool appartient_cercle(array<double,2>,array<double,2>);
#endif
