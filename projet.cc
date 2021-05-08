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
#include <gtkmm/application.h>
#include "geomod.h"
#include "simulation.h"
#include "message.h"
#include "gui.h"

//using namespace std;

void lecture_main(int, char **);

int main(int argc, char ** argv) 
{
    
    //lecture_main(argc, argv);
    auto app = Gtk::Application::create(argc, argv, "org.gtkmm.example");
    Interface window(argc, argv);
    
    return app->run(window);
}

void lecture_main(int argc, char** argv)
{
    std::ifstream fichier(argv[1]);
    
    if(fichier.fail() or argc!=2)
    {
        exit(0);
    }
        
    Simulation simulation;
        
    simulation.lecture(fichier);
    simulation.verifications();
    //simulation.affichage();
    
    std::cout <<message::success();//Si on arrive la ça signifie que tout est bon
    fichier.close();
}
