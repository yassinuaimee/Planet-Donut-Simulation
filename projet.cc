#include <iostream>
using namespace std;
#include <array>
#include "geomod.h"
#include "message_geomod.h"
void verification_nb_arguments(const int, char**);
void case_1(const int, char**);
void case_2(const int, char**);
void case_3(const int, char**);
void case_4(const int, char**);
void case_5(const int, char**);
void case_6(const int, char**);

int main(int argc,char* argv[] )
{
	verification_nb_arguments(argc,argv);
	
}



void verification_nb_arguments(const int argc, char* argv[])
{
	switch(atoi(argv[1] )){
		
		case(1): 
			case_1(argc, argv);
			break;
			
		case(2):
			case_2(argc, argv);
			break;
			
		case(3):
			case_3(argc, argv);
			break;
			
		case(4):
			case_4(argc, argv);
			break;
			
		case(5):
			case_5(argc, argv);
			break;	
			
		case(6):
			case_6(argc, argv);
			break;
			
		default: 
			cout<<"Default"<<endl;
			message_geomod::bad_argc();
			exit(0);
			break;
}
}
void case_1(int argc, char* argv[])
{
	if(argc-2!=1)
	{
		message_geomod::bad_argc();
		exit(0);
	}
	else
	{
		double init_max(atof(argv[2]));
		set_max(init_max); //Setter de max
		message_geomod::print_max(init_max, get_max(), get_epsilon_zero());//Recupérer max et Epsilon_zero avec leur getteurs
			}
}

void case_2(int argc, char* argv[])
{
	if(argc-2!=3)
	{
		message_geomod::bad_argc();
		exit(0);
	}
	else
	{
		double init_max(atof(argv[2])); //C'est la meilleure méthode , c'est plus clean on initialise une seule fois la valeur de init_max comme ça c'est bon
		double x_init(atof(argv[3]));
		double y_init(atof(argv[4]));
							
		set_max(init_max); //Setter de max
							
		Point point(x_init, y_init);//On fais la normalisation directement dans les fonctions c'est plus pratique
							
		message_geomod::print_point(x_init, y_init, point.get_x(), 
				                                        point.get_y());
			}
}

void case_3(int argc, char* argv[])
{
	if(argc-2!=5)
	{
		message_geomod::bad_argc();
		exit(0);
	}
	else
	{
	set_max( atof(argv[2]) ); //Setter de max
						
	Point point_1( atof(argv[3] ), atof(argv[4] ) );
	Point point_2( atof(argv[5] ), atof(argv[6] ) );
						
	Vecteur vecteur;
						
	vecteur.norme_plus_petit_vecteur(point_1, point_2 , vecteur);
						
	message_geomod::print_vect( vecteur.get_norme(), vecteur.get_x() , 
				                                    vecteur.get_y() );
							
			}
}

void case_4(int argc, char* argv[])
{
	if(argc-2!=5)
	{
		message_geomod::bad_argc();
		exit(0);
	}
	else
	{
		set_max( atof(argv[2]) ); //Setter de max
		Point point_1( atof(argv[3] ) , atof(argv[4] ) );
		Point point_2( atof(argv[5] ) , atof(argv[6] ) );
						
		message_geomod::print_equal_point( point_1 == point_2, 
																	point_1.get_x(), 
																	point_1.get_y(), 
																	point_2.get_x(), 
																	point_2.get_y());
			}
}

void case_5(int argc, char* argv[])
{
	if(argc-2!=6)
	{
		message_geomod::bad_argc();
		exit(0);
	}
	else
	{
		set_max( atof(argv[2]) ); //Setter de max
							
		Point point( atof(argv[3] ), atof(argv[4] ) );
		Point centre( atof(argv[5] ), atof(argv[6] ) );
		Cercle cercle(centre, atof(argv[7] ));
							
		message_geomod::print_include_point( cercle.point_appartient(point), 
																point.get_x(), 
																point.get_y(), 
																cercle.get_centre_x(), 
																cercle.get_centre_y(), 
																cercle.get_rayon() );
			}
}

void case_6(int argc, char* argv[])
{
	if(argc-2!=7)
	{
		message_geomod::bad_argc();
		exit(0);
	}
	else
	{
		set_max( atof(argv[2]) ); //Setter de max
						
		Point centre_1( atof(argv[3] ), atof(argv[4] ) );
		Cercle cercle_1(centre_1, atof(argv[5] ));
						
		Point centre_2( atof(argv[6] ), atof(argv[7] ) );
		Cercle cercle_2(centre_2, atof(argv[8] ));
						
		message_geomod::print_intersect(cercle_1.intersection_cercle(cercle_2), 
															cercle_1.get_centre_x(), 
															cercle_1.get_centre_y(), 
															cercle_1.get_rayon(), 
															cercle_2.get_centre_x(), 
															cercle_2.get_centre_y(), 
															cercle_2.get_rayon());
						
			}
}
