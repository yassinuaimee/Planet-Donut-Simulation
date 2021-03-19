#ifndef GEOMOD_H_INCLUDED
#define GEOMOD_H_INCLUDED
#include <array>



/*
class Cercle
{
public:
	Cercle (Point, double); //On changer la valeur en entrée par un point
    bool point_appartient(Point);//On va devoir remplacer cette valeur par une variable de type Point
    bool intersection_cercle(Cercle);
    std::array<double,2> get_centre();
    double get_rayon();
private:
    std::array<double,2> centre;
    double rayon;
};

*/

class Point
{
public:
	Point(double , double );
	Point();
	void normalisation();
	double get_x();
	double get_y();
	void set_x(double);
	void set_y(double);
	bool operator==( Point const& ) const ;//Yassin va créer la fonction égalité vecteurs
private:
	double x;
	double y;
	
};

class Vecteur
{
public:
	Vecteur(double , double);
	Vecteur();
	double norme_plus_petit_vecteur( Point &, Point& ,Vecteur&);
	double get_x();
	double get_y();
	double get_norme();
	void set_x(double);
	void set_y(double);
	void set_norme(double);
private:
	double x;
	double y;
	double norme;
	
};






bool equal_zero(double);

void set_max(double);

double get_max();

double  get_epsilon_zero();

double normalisation_point(double);

double norme_plus_petit_vecteur(std::array<double,2> depart,std::array<double,2> arrivee,std::array<double,2> vecteur);

double norme_plus_petit_vecteur(const double,const double,const double,const double); //Surcharge de la fonction qui n'utilise pas de array comme paramètre
//Je fais le choix de return un array comme ça ya tout qui passe et c'est mieux(sinon on avait les deux valeur du vecteur qui restaient à 0 parcque c'est dans un autre module)


bool  test_egalite_points(std::array<double,2>,std::array<double,2>);
bool appartient_cercle(std::array<double,2>,std::array<double,2>);
#endif
