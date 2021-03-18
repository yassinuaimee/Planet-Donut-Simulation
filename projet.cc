#include <iostream>
using namespace std;
#include <array>
#include "geomod.h"
#include "message_geomod.h"
void verification_nb_arguments(const int,char**);


int main(int argc,char* argv[] )
{
	verification_nb_arguments(argc,argv);

}



void verification_nb_arguments(const int argc, char* argv[])
{
	int i(atof(argv[1] )); //Permet la conversion de chaine de caractere ASCII vers entier
	
	switch(i){
		case(1): if(argc-2!=1)
					{
						message_geomod::bad_argc();
						exit(0);
					}
					else
					{
						double init_max(atof(argv[2]));
						
						set_max(init_max); //Setter de max
						
						message_geomod::print_max(init_max,get_max(),get_epsilon_zero());//Recupérer max et Epsilon_zero avec leur getteurs
					}
					break;
		case(2):if(argc-2!=3)
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
							double x_mod(normalisation_point(x_init));
							double y_mod(normalisation_point(y_init));
							
							message_geomod::print_point(x_init, y_init, x_mod, y_mod);
							
						}
					break;
		case(3):if(argc-2!=5)
						{
							message_geomod::bad_argc();
							exit(0);
						}
						else
						{
						double init_max(atof(argv[2]));
						
						set_max(init_max); //Setter de max
						
						double init_x1( normalisation_point( (atof(argv[3])))); //On initialise la valeur directement avec une coordonnée qui a déjà été initialisée on va rapidos quoi
						double init_y1( normalisation_point( (atof(argv[4]))));
						double init_x2( normalisation_point( (atof(argv[5]))));
						double init_y2( normalisation_point( (atof(argv[6]))));
						
						array<double,2> point_1={init_x1,init_y1};
						array<double,2> point_2={init_x2,init_y2};
						array<double,2> vecteur={0,0}; //C'est ce vecteur qui va être modifié pour donner le vecteur finale entre les deux points
						
						double norme(0);
						
						norme = norme_plus_petit_vecteur(point_1 , point_2 , vecteur);
						message_geomod::print_vect( norme, vecteur[0] , vecteur[1]);
							
						}
					break;
		case(4):if(argc-2!=5)
						{
							message_geomod::bad_argc();
							exit(0);
						}
					break;
		case(5):if(argc-2!=6)
						{
							message_geomod::bad_argc();
							exit(0);
						}
					break;	
		case(6):if(argc-2!=7)
						{
							message_geomod::bad_argc();
							exit(0);
						}
					break;
		default: 
					cout<<"Default"<<endl;
					message_geomod::bad_argc();
					exit(0);
					break;
}
}
