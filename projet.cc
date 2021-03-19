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
	int i(atoi(argv[1] )); //Permet la conversion de chaine de caractere ASCII vers entier
	
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
							Point point(x_init, y_init);
							point.normalisation();
							
							message_geomod::print_point(x_init, y_init, point.get_x(), point.get_y());
							
						}
					break;
		case(3):if(argc-2!=5)
						{
							message_geomod::bad_argc();
							exit(0);
						}
						else
						{
						double init_max( atof(argv[2]) );
						set_max(init_max); //Setter de max
						
						Point point_1( atof(argv[3] ) , atof(argv[4] ) );
						Point point_2( atof(argv[5] ) , atof(argv[6] ) );
						
						point_1.normalisation();
						point_2.normalisation();
						
						Vecteur vecteur;
						
						vecteur.norme_plus_petit_vecteur(point_1 , point_2 , vecteur);
						
						message_geomod::print_vect( vecteur.get_norme(), vecteur.get_x() , vecteur.get_y() );
							
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
