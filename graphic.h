/* Nom:
 * graphic.h
 *
 * Auteurs:
 * Yassin Al-Nuaimee
 * Adrien Maillet Gonzalez
 *
 * Version: 2
 *
 *
 */

#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <iostream>
#include <gtkmm.h>
#include <cairomm/context.h>
#include "gui.h"

void activate_graphic(const Cairo::RefPtr<Cairo::Context>&);

void cercle_noir_dessin(double, double, double);
void cercle_communication_dessin(double, double);
void cercle_base_dessin(double, double, int);
void cercle_robot_dessin(double, double, int);

void ligne_dessin(double, double, double, double);
void color_base(int index);

#endif 
