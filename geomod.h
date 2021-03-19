#ifndef GEOMOD_H_INCLUDED
#define GEOMOD_H_INCLUDED
#include <array>



class Cercle
{
public:
	Cercle (std::array<double,2>, double);
    bool point_appartient(std::array<double,2>);
    bool intersection_cercle(Cercle);
    std::array<double,2> get_centre();
    double get_rayon();
private:
    std::array<double,2> centre;
    double rayon;
};

bool equal_zero(double);

void set_max(double);

double get_max();

double  get_epsilon_zero();

double normalisation_point(double);

double norme_plus_petit_vecteur(std::array<double,2> depart,std::array<double,2> arrivee,std::array<double,2> vecteur);
std::array<double,3> norme_plus_petit_vecteur(const double,const double,const double,const double); //Surcharge de la fonction qui n'utilise pas de array comme paramètre
//Je fais le choix de return un array comme ça ya tout qui passe et c'est mieux(sinon on avait les deux valeur du vecteur qui restaient à 0 parcque c'est dans un autre module)


bool  test_egalite_points(std::array<double,2>,std::array<double,2>);
bool appartient_cercle(std::array<double,2>,std::array<double,2>);
#endif
