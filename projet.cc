/* Nom:
 * projet.cc
 * 
 * Auteurs:
 * Yassin Al-Nuaimee
 * Adrien Maillet Gonzalez
 * 
 * Version: 10
 * 
 * 
 */

#include <iostream>
#include <array>
#include <fstream>
#include "geomod.h"
#include "simulation.h"
#include "message.h"

using namespace std;

void lecture_main(int, char **);

int main(int argc, char ** argv) 
{ 
    lecture_main(argc, argv);
}

void lecture_main(int argc, char** argv)
{
    ifstream fichier(argv[1]);     //argv[1]
    
    if(fichier.fail() or argc!=2)
        exit(0);
    
    Simulation simulation;
    
    simulation.lecture(fichier);
    simulation.verifications();
    
    simulation.affichage();
    
    cout <<message::success();
        
    
    fichier.close();
}
