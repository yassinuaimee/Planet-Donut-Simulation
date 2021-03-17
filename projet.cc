#include <iostream>
using namespace std;

#include "geomod.h"
#include "message_geomod.h"
void verification_nb_arguments(const int,char* argv[] );


int main(int argc,char* argv[] )
{
//verification_nb_arguments(argc,argv[]);

cout<<argc<<endl;
cout<<argv[0]<<endl;
}



void verification_nb_arguments(const int n, char* argv[])
{
	switch(*(reinterpret_cast<int*>(argv[1] ))){
		
		case(1): if(n-2!=1)
						{
							message_geomod::bad_argc();
							exit(0);
						}
		case(2):if(n-2!=3)
						{
							message_geomod::bad_argc();
							exit(0);
						}
			
		case(3):if(n-2!=5)
						{
							message_geomod::bad_argc();
							exit(0);
						}
		case(4):if(n-2!=5)
						{
							message_geomod::bad_argc();
							exit(0);
						}
		case(5):if(n-2!=6)
						{
							message_geomod::bad_argc();
							exit(0);
						}
						
		case(6):if(n-2!=7)
						{
							message_geomod::bad_argc();
							exit(0);
						}
		default: 
				message_geomod::bad_argc();
				exit(0);
}
}
