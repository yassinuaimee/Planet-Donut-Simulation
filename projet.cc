#include <iostream>
using namespace std;

#include "geomod.h"
#include "message_geomod.h"
void verification_nb_arguments(const int,char**);


int main(int argc,char* argv[] )
{
	cout<<"ARGC : "<<argc<<endl;
	verification_nb_arguments(argc,argv);

}



void verification_nb_arguments(const int argc, char* argv[])
{
	int i(atoi(argv[1] )); //Permet la conversion de chaine de caractere ASCII vers entier
	cout<<i<<endl;
	switch(i){
		case(1): if(argc-2!=1)
						{
							message_geomod::bad_argc();
							exit(0);
						}
					else
					{
						cout<<argv[1]<<endl;
					}
					break;
		case(2):if(argc-2!=3)
						{
							message_geomod::bad_argc();
							exit(0);
						}
					break;
		case(3):if(argc-2!=5)
						{
							message_geomod::bad_argc();
							exit(0);
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
