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
						
						double init_x1( normalisation_point( atof(argv[3] ) ));
						double init_y1( normalisation_point( atof(argv[4] ) ));
						double init_x2( normalisation_point( atof(argv[5] ) ));
						double init_y2( normalisation_point( atof(argv[6] ) ));
						
						
						double vecteur_x(0.0),vecteur_y(0.0);
						double norme(0);
						
						array<double,3> norme_et_coordonnees(  norme_plus_petit_vecteur(init_x1 , init_y1 , init_x2 ,init_y2 ) );//A l'intérieur on appelle la fonction qui va permettre de créer le vecteur et de calculer la norme
						norme=norme_et_coordonnees[0];
						vecteur_x=norme_et_coordonnees[1];
						vecteur_y=norme_et_coordonnees[2];
						message_geomod::print_vect( norme, vecteur_x , vecteur_y );
							
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
