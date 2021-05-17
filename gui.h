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
#include <fstream>
#include "geomod.h"

//==================================================================================//

struct Frame // Framing and window parameters
{
    double xMin;
    double xMax;
    double yMin;
    double yMax;
    
};

//==================================================================================//

struct SimData
{
    unsigned uid;
    unsigned nbP;
    unsigned nbF;
    unsigned nbT;
    unsigned nbC;
    double ressource;
    double ressource_p;
};

//==================================================================================//

class MyArea : public Gtk::DrawingArea
{
public:
    MyArea();
    virtual ~MyArea();
    void refresh();
protected:
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
    void draw_frame(const Cairo::RefPtr<Cairo::Context>& cr);
    
private:
};

//==================================================================================//

class Interface : public Gtk::Window
{
public:
    Interface(int, char**);
    virtual ~Interface();
    void tree_view_update();
    unsigned int get_count();
protected:
    MyArea m_Area;
    bool on_idle();
    void on_button_clicked_exit();
    void on_button_clicked_open();
    void on_button_clicked_save();
    void on_button_clicked_start();
    void on_button_clicked_step();
    void on_button_clicked_toggle_link();
    void on_button_clicked_toggle_range();
    bool on_key_press_event(GdkEventKey * key_event);
    
    Gtk::Box m_Box, m_Box_Up;
    Gtk::Box m_Box_Left, m_Box_General, m_Box_Toggle_Display;
    
    Gtk::Button m_Button_exit;
    Gtk::Button m_Button_open;
    Gtk::Button m_Button_save;
    Gtk::Button m_Button_start;
    Gtk::Button m_Button_step;
    Gtk::Button m_Button_toggle_link;
    Gtk::Button m_Button_toggle_range;
    
    Gtk::Frame m_Frame1, m_Frame2;
    class Model_columns : public Gtk::TreeModel::ColumnRecord
    {
    public:
        
     Model_columns()
     {
       add(_col_uid);
       add(_col_nbP);
       add(_col_nbF);
       add(_col_nbT);
       add(_col_nbC);
       add(_col_resource);
       add(_col_resource_percentage);
     }

     Gtk::TreeModelColumn<int> _col_uid;
     Gtk::TreeModelColumn<int> _col_nbP;
     Gtk::TreeModelColumn<int> _col_nbF;
     Gtk::TreeModelColumn<int> _col_nbT;
     Gtk::TreeModelColumn<int> _col_nbC;
     Gtk::TreeModelColumn<double> _col_resource;
     Gtk::TreeModelColumn<int> _col_resource_percentage;
  };

  Model_columns _columns;

  Gtk::ScrolledWindow _scrolled_window;
  Gtk::TreeView _tree_view;

   
private:
    unsigned int count;
    bool start;
};

#endif
