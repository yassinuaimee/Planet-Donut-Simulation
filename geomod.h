/* Nom:
 * geomod.h
 * 
 * Auteurs:
 * Yassin Al-Nuaimee
 * Adrien Maillet Gonzalez
 * 
 * Version: 5
 * 
 * 
 */
 
#ifndef GEOMOD_H_INCLUDED
#define GEOMOD_H_INCLUDED
#include <array>


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
	bool operator==( Point & ) const ;
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
	double norme_plus_petit_vecteur( Point &, Point &);
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



class Cercle
{
public:
	Cercle (Point, double); 
	Cercle (double, double, double);
	Cercle();
    bool point_appartient(Point);
    bool intersection_cercle(Cercle);
    Point get_centre();
    double get_centre_x();
    double get_centre_y();
    double get_rayon();
private:
    Point centre;
    double rayon;
};


bool equal_zero(double);

void set_max(double);

double get_max();

double  get_epsilon_zero();

double normalisation_point(double);



double norme_plus_petit_vecteur(const double,
								const double,
								const double,
								const double); 

#endif
