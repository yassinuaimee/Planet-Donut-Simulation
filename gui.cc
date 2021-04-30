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
{}

MyArea::~MyArea()
{
}

void MyArea::draw_frame(const Cairo::RefPtr<Cairo::Context>& cr)
{
    cr->set_line_width(10.0);
    cr->set_source_rgb(0.0, 0.0, 0.0);
    cr->rectangle(0,0,get_allocation().get_width(),get_allocation().get_height());
    cr->stroke();
}


bool MyArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
    draw_frame(cr);
    Gtk::Allocation allocation = get_allocation();
    const int width = allocation.get_width();
    const int height = allocation.get_height();

    // coordinates for the center of the window
    int xc, yc;
    xc = width / 2;
    yc = height / 2;

    cr->set_line_width(5.0);

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

Interface::Interface()
: m_Box(Gtk::ORIENTATION_HORIZONTAL,10),
m_Box_Left(Gtk::ORIENTATION_VERTICAL,10),
m_Box_Right(Gtk::ORIENTATION_VERTICAL,10),
m_Box_General(Gtk::ORIENTATION_VERTICAL,10),
m_Box_Toggle_Display(Gtk::ORIENTATION_VERTICAL,10),

    m_Button_exit("exit"),
    m_Button_open("open"),
    m_Button_save("save"),
    m_Button_start("start"),
    m_Button_step("step"),
    m_Button_toggle_link("toggle link"),
    m_Button_toggle_range("toggle range"),

count(0),
start(false)

{
    set_title("Planet Donut - DEMO");
    set_border_width(10);
    
    m_Box.set_size_request(200,200);
    
    add(m_Box);
    
    m_Box.pack_start(m_Box_Left);
    m_Box.pack_start(m_Box_Right);
    
    m_Box_Left.add(m_Frame1);
    m_Box_Left.add(m_Frame2);
    
    m_Frame1.add(m_Box_General);
    m_Frame2.add(m_Box_Toggle_Display);
    
    m_Box.pack_start(m_Box_Right);
    
    
    m_Frame1.set_label("General");
    m_Frame2.set_label("Toggle Display");
    
    m_Box_General.pack_start(m_Button_exit,false,false);
    m_Box_General.pack_start(m_Button_open,false,false);
    m_Box_General.pack_start(m_Button_save,false,false);
    m_Box_General.pack_start(m_Button_start,false,false);
    m_Box_General.pack_start(m_Button_step,false,false);
    m_Box_Toggle_Display.pack_start(m_Button_toggle_link,false,false);
    m_Box_Toggle_Display.pack_start(m_Button_toggle_range,false,false);

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
    
    Glib::signal_timeout().connect( sigc::mem_fun(*this, &Interface::on_idle),80);//Ligne avec le timer
    
    show_all_children();//J'ai pris tellement longtemps à capter cette erreur
}

bool Interface::on_idle()
{
  
  if(start)
  {
    std::cout << "Mise à jour de la simulation numéro : " << ++count << std::endl;
  }
  
  return true;  // return false when done
}

bool Interface::on_key_press_event(GdkEventKey * key_event)
{
    if(key_event->type == GDK_KEY_PRESS)
    {
        switch(gdk_keyval_to_unicode(key_event->keyval))
        {
            case '1':
                std::cout << "Touche 1" << std::endl;
                break;
            case 's':
                std::cout << "Start" << std::endl;
                break;
            case 'q':
                std::cout<<"Quit\n";
                exit(0);
                break;
        }
    }
    
    return Gtk::Window::on_key_press_event(key_event);
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
    Gtk::FileChooserDialog dialog("Please choose a file",
            Gtk::FILE_CHOOSER_ACTION_OPEN);
    dialog.set_transient_for(*this);

    //Add response buttons the the dialog:
    dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
    dialog.add_button("_Open", Gtk::RESPONSE_OK);

    m_Button_open.set_label("choosing a file");

    //Show the dialog and wait for a user response:
    int result = dialog.run();

    m_Button_open.set_label("Done choosing a file");

    //Handle the response:
    switch(result)
    {
      case(Gtk::RESPONSE_OK):
      {
        std::cout << "Open clicked." << std::endl;

        //Notice that this is a std::string, not a Glib::ustring.
        std::string filename = dialog.get_filename();
        std::cout << "File selected: " <<  filename << std::endl;
        break;
      }
      case(Gtk::RESPONSE_CANCEL):
      {
        std::cout << "Cancel clicked." << std::endl;
        break;
      }
      default:
      {
        std::cout << "Unexpected button clicked." << std::endl;
        break;
      }
    }
}

void Interface::on_button_clicked_save()
{
    std::cout<<"Save\n";
}

void Interface::on_button_clicked_start()
{
    if(start)
    {
        std::cout<<"Stop\n";
        start=false;
        m_Button_start.set_label("start");
        
    }
    else
    {
        std::cout<<"Start\n";
        start=true;
        m_Button_start.set_label("stop");
    }
}

void Interface::on_button_clicked_step()
{
    std::cout<<"Step\n";
    if(not start)
    {
        std::cout<<"Step OK\n";
        std::cout << "Mise à jour de la simulation numéro : " << ++count << std::endl;
    }
}

void Interface::on_button_clicked_toggle_link()
{
    std::cout<<"Toggle Link\n";
}

void Interface::on_button_clicked_toggle_range()
{
    std::cout<<"Toggle Range\n";
}
