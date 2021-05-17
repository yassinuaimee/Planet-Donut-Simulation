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



int main(int argc, char ** argv) 
{
    auto app = Gtk::Application::create();
    Interface window(argc, argv);
    return app->run(window);
}
