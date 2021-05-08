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
#include "gui.h"
void cercle_dessin(double, double, double, const Cairo::RefPtr<Cairo::Context>&);
void cercle_communication_dessin(double, double, const Cairo::RefPtr<Cairo::Context>&);
void ligne_dessin(double, double, double, double, const Cairo::RefPtr<Cairo::Context>&);
void color_base(int index, const Cairo::RefPtr<Cairo::Context>&);

#endif 
