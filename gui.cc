/* Nom:
 * gui.cc
 *
 * Auteurs:
 * Yassin Al-Nuaimee
 * Adrien Maillet Gonzalez
 *
 * Version: 1
 *
 *
 */
#include <iostream>
#include <cairomm/context.h>
#include "gui.h"
#include "geomod.h"

//=================================================================================//

MyArea::MyArea()
{
}

MyArea::~MyArea()
{
}

bool MyArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
  Gtk::Allocation allocation = get_allocation();
  const int width = allocation.get_width();
  const int height = allocation.get_height();

  // coordinates for the center of the window
  int xc, yc;
  xc = width / 2;
  yc = height / 2;

  cr->set_line_width(10.0);

  // draw red lines out from the center of the window
  cr->set_source_rgb(0.8, 0.0, 0.0);
  cr->move_to(0, 0);
  cr->line_to(xc, yc);
  cr->line_to(0, height);
  cr->move_to(xc, yc);
  cr->line_to(width, yc);
  cr->stroke();

  return true;
}
//=================================================================================//
Interface::Interface():

    m_Box(Gtk::ORIENTATION_HORIZONTAL,10),
    m_Box_Left(Gtk::ORIENTATION_VERTICAL, 10),
    m_Box_Right(Gtk::ORIENTATION_VERTICAL, 10),

    m_Button_exit("exit"),
    m_Button_open("open"),
    m_Button_save("save"),
    m_Button_start("start"),
    m_Button_step("step"),
    m_Button_toggle_link("toggle link"),
    m_Button_toggle_range("toggle range")

{
    set_title("Planet Donut - DEMO");
    //set_border_width(10);
    m_Box.set_size_request(400,400);
    add(m_Box);
    m_Box.pack_start(m_Box_Left);
    m_Box.pack_start(m_Box_Right);
    
    m_Box_Left.pack_start(m_Button_exit,false,false);//Temporaire aussi
    m_Box_Left.pack_start(m_Button_open,false,false);
    m_Box_Left.pack_start(m_Button_save,false,false);
    m_Box_Left.pack_start(m_Button_start,false,false);
    m_Box_Left.pack_start(m_Button_step,false,false);
    m_Box_Left.pack_start(m_Button_toggle_link,false,false);
    m_Box_Right.pack_start(m_Button_toggle_range,false,false);//Le right est temporaire

    m_Area.set_size_request(500,500);
    m_Box_Right.pack_start(m_Area);
    
    m_Button_exit.signal_clicked().connect(sigc::mem_fun(*this,
                &Interface::on_button_clicked_exit));

    
    m_Button_open.signal_clicked().connect(sigc::mem_fun(*this,
                &Interface::on_button_clicked_open));
    
    m_Button_save.signal_clicked().connect(sigc::mem_fun(*this,
                &Interface::on_button_clicked_save));
    
    m_Button_start.signal_clicked().connect(sigc::mem_fun(*this,
                &Interface::on_button_clicked_start));
    
    m_Button_step.signal_clicked().connect(sigc::mem_fun(*this,
                &Interface::on_button_clicked_step));
    
    m_Button_toggle_link.signal_clicked().connect(sigc::mem_fun(*this,
                &Interface::on_button_clicked_toggle_link));
    
    m_Button_toggle_range.signal_clicked().connect(sigc::mem_fun(*this,
                &Interface::on_button_clicked_toggle_range));
    
}

Interface::~Interface()
{}

void Interface::on_button_clicked_exit()
{
    std::cout<<"exit\n";
    exit(0);
}

void Interface::on_button_clicked_open()
{
    std::cout<<"Open\n";
}

void Interface::on_button_clicked_save()
{
    std::cout<<"Save\n";
}

void Interface::on_button_clicked_start()
{
    std::cout<<"Start\n";
}

void Interface::on_button_clicked_step()
{
    std::cout<<"Step\n";
}

void Interface::on_button_clicked_toggle_link()
{
    std::cout<<"Toggle Link\n";
}

void Interface::on_button_clicked_toggle_range()
{
    std::cout<<"Toggle Range\n";
}
