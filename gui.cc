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
#include <gtkmm.h>
#include <cairomm/context.h>
#include "gui.h"
#include "geomod.h"
#include "constantes.h"

static Frame frame;

//=================================================================================//

MyArea::MyArea()
{}

//=================================================================================//

MyArea::~MyArea()
{
}

//=================================================================================//

void MyArea::draw_frame(const Cairo::RefPtr<Cairo::Context>& cr)
{
    cr->set_line_width(10.0);
    cr->set_source_rgb(0.7, 0.7, 0.7);
    cr->rectangle(0,0,get_allocation().get_width(),get_allocation().get_height());
    cr->stroke();
}

//=================================================================================//

static void orthographic_projection(const Cairo::RefPtr<Cairo::Context>& cr,
                                    Frame frame)
{
    cr->translate(frame.width/2, frame.height/2);
    cr->scale(frame.width/(frame.xMax - frame.xMin),
              -frame.height/(frame.yMax - frame.yMin));
}

//=================================================================================//

bool MyArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
    draw_frame(cr);
    
    Gtk::Allocation allocation = get_allocation();
    const int width = allocation.get_width();
    const int height = allocation.get_height();

    frame.xMax = dim_max;
    frame.xMin = -dim_max;
    frame.yMax = dim_max;
    frame.yMin = -dim_max;
    frame.asp = (frame.xMax-frame.xMin)/(frame.yMax-frame.yMin);
    frame.width = width ;
    frame.height = height;
    
    orthographic_projection(cr, frame);
    
    
    cr->set_line_width(2.0);
    
    
    cr->fill_preserve();
    cr->stroke();
    
    
    
    Cercle test(900, -900, 300);
    
    test.affiche(cr);
    return true;
}

//=================================================================================//

Interface::Interface()
:   m_Box(Gtk::ORIENTATION_VERTICAL, 20),
    m_Box_Up(Gtk::ORIENTATION_HORIZONTAL, 20),
    m_Box_Down(Gtk::ORIENTATION_VERTICAL, 20),
    m_Box_Left(Gtk::ORIENTATION_VERTICAL, 20),
    m_Box_General(Gtk::ORIENTATION_VERTICAL, 20),
    m_Box_Toggle_Display(Gtk::ORIENTATION_VERTICAL, 20),

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
    set_default_size(400, 400);
    
    add(m_Box);
    
    
    m_Box.pack_start(m_Box_Up);
    m_Box.pack_start(m_Box_Down);
    
    m_Box_Up.pack_start(m_Box_Left);
    m_Box_Up.pack_start(m_Area);
    
    m_Box_Down.pack_start(m_ScrolledWindow);
    
    m_Box_Left.add(m_Frame1);
    m_Box_Left.add(m_Frame2);
    
    m_Frame1.add(m_Box_General);
    m_Frame2.add(m_Box_Toggle_Display);

    m_Frame1.set_label("General");
    m_Frame2.set_label("Toggle Display");
    
    m_Box_General.pack_start(m_Button_exit,false,false);
    m_Box_General.pack_start(m_Button_open,false,false);
    m_Box_General.pack_start(m_Button_save,false,false);
    m_Box_General.pack_start(m_Button_start,false,false);
    m_Box_General.pack_start(m_Button_step,false,false);
    
    m_Box_Toggle_Display.pack_start(m_Button_toggle_link,false,false);
    m_Box_Toggle_Display.pack_start(m_Button_toggle_range,false,false);

    m_Area.set_size_request(dim_max/2, dim_max/2);
    

    
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
    
    Glib::signal_timeout().connect(sigc::mem_fun(*this,
                &Interface::on_idle), 100);//Ligne avec le timer
    
    m_ScrolledWindow.add(m_TreeView);

    //Only show the scrollbars when they are necessary:
    //m_ScrolledWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

    m_Box_Down.pack_start(m_ScrolledWindow);

    //Create the Tree model:
    m_refTreeModel = Gtk::ListStore::create(m_Columns);
    m_TreeView.set_model(m_refTreeModel);

    //Fill the TreeView's model
    Gtk::TreeModel::Row row = *(m_refTreeModel->append());
    row[m_Columns.m_col_uid] = 31;
    row[m_Columns.m_col_nbP] = 16;
    row[m_Columns.m_col_nbF] = 2;
    row[m_Columns.m_col_nbT] = 1;
    row[m_Columns.m_col_nbC] = 6;
    row[m_Columns.m_col_amount_ressource] = 10;
    row[m_Columns.m_col_mission_acompleteness] = 15;

    row = *(m_refTreeModel->append());
    row[m_Columns.m_col_uid] = 19;
    row[m_Columns.m_col_nbP] = 6;
    row[m_Columns.m_col_nbF] = 20;
    row[m_Columns.m_col_nbT] = 19;
    row[m_Columns.m_col_nbC] = 6;
    row[m_Columns.m_col_amount_ressource] = 1000;
    row[m_Columns.m_col_mission_acompleteness] = 80;

    //Add the TreeView's view columns:
    //This number will be shown with the default numeric formatting.
    m_TreeView.append_column("Uid", m_Columns.m_col_uid);
    m_TreeView.append_column("nbP", m_Columns.m_col_nbP);
    m_TreeView.append_column("nbF", m_Columns.m_col_nbF);
    m_TreeView.append_column("nbT", m_Columns.m_col_nbT);
    m_TreeView.append_column("nbC", m_Columns.m_col_nbC);


    m_TreeView.append_column_numeric("Amount ressource", m_Columns.m_col_amount_ressource,
            "%010d" /* 10 digits, using leading zeroes. */);

    //Display a progress bar instead of a decimal number:
    auto cell = Gtk::manage(new Gtk::CellRendererProgress);
    int cols_count = m_TreeView.append_column("Mission completeness", *cell);
    auto pColumn = m_TreeView.get_column(cols_count - 1);
    if(pColumn)
    {
      pColumn->add_attribute(cell->property_value(), m_Columns.m_col_mission_acompleteness);
    }


    show_all_children();//J'ai pris tellement longtemps à capter cette erreur
}

//=================================================================================//


bool Interface::on_idle()
{
  if(start)
  {
      std::cout << "Mise à jour de la simulation numéro : " << ++count << std::endl;
  }
  
  return true;  // return false when done
}
//=================================================================================//

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
//=================================================================================//

Interface::~Interface()
{}

//=================================================================================//

void Interface::on_button_clicked_exit()
{
    std::cout<<"exit\n";
    exit(0);
}
//=================================================================================//

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
//=================================================================================//

void Interface::on_button_clicked_save()
{
    std::cout<<"Save\n";
}
//=================================================================================//

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

//=================================================================================//

void Interface::on_button_clicked_step()
{
    std::cout<<"Step\n";
    if(not start)
    {
        std::cout<<"Step OK\n";
        std::cout << "Mise à jour de la simulation numéro : " << ++count << std::endl;
    }
}
//=================================================================================//

void Interface::on_button_clicked_toggle_link()
{
    std::cout<<"Toggle Link\n";
}
//=================================================================================//

void Interface::on_button_clicked_toggle_range()
{
    std::cout<<"Toggle Range\n";
}
