/* Nom:
 * gui.h
 *
 * Auteurs:
 * Yassin Al-Nuaimee
 * Adrien Maillet Gonzalez
 *
 * Version: 12
 *
 *
 */

#ifndef GUI_H
#define GUI_H

#include <gtkmm.h>
#include "geomod.h"

class MyArea : public Gtk::DrawingArea
{
public:
    MyArea();
    virtual ~MyArea();
protected:
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
    //void draw_frame(const Cairo::RefPtr<Cairo::Context>& cr);
    
private:
    
};

//==================================================================================//

class Interface : public Gtk::Window
{
public:
    Interface();
    virtual ~Interface();
protected:
    void on_button_clicked_exit();
    void on_button_clicked_open();
    void on_button_clicked_save();
    void on_button_clicked_start();
    void on_button_clicked_step();
    void on_button_clicked_toggle_link();
    void on_button_clicked_toggle_range();
    
    //bool on_key_press_event(GdkEventKey * key_event);
    
    Gtk::Box m_Box, m_Box_Left, m_Box_Right;
    
    Gtk::Button m_Button_exit;
    Gtk::Button m_Button_open;
    Gtk::Button m_Button_save;
    Gtk::Button m_Button_start;
    Gtk::Button m_Button_step;
    Gtk::Button m_Button_toggle_link;
    Gtk::Button m_Button_toggle_range;
    
    MyArea m_Area;
};

#endif
