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
#include <vector>
#include <string>
#include <gtkmm.h>
#include <fstream>
#include <memory>
#include <cairomm/context.h>
#include "gui.h"
#include "geomod.h"
#include "constantes.h"
#include "graphic.h"
#include "simulation.h"

#define GTK_COMPATIBILITY_MODE
#ifdef GTK_COMPATIBILITY_MODE
namespace Gtk
{
  template<class T, class... T_Args>
  auto make_managed(T_Args&&... args) -> T*
  {
    return manage(new T(std::forward<T_Args>(args)...));
  }
}
#endif

namespace
{
    Simulation simulation;
    bool toggle_link(false);
    bool toggle_range(false);
    Frame frame;
}

void init_simulation(int, char**);


//=================================================================================//

MyArea::MyArea()
{
    frame.xMax=dim_max;
    frame.xMin=-dim_max;
    frame.yMax=dim_max;
    frame.yMin=-dim_max;
}

//=================================================================================//

MyArea::~MyArea()
{
}

//=================================================================================//

bool MyArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
    activate_graphic(cr);
    Gtk::Allocation allocation = get_allocation();
    const int width = allocation.get_width();
    const int height = allocation.get_height();

    double size(0.0);
    
    if(width>=height)
    {
        size=height;
    }
    else
    {
        size=width;
    }
    
    cr->translate(size/2, size/2);
    cr->scale(size/(frame.xMax - frame.xMin), -size/(frame.yMax - frame.yMin));
    draw_frame(cr);
    
    
    simulation.affiche_range(toggle_range);
    simulation.affiche_link(toggle_link);
    simulation.affiche_dessin();

    
    return true;
}

//=================================================================================//

void MyArea::draw_frame(const Cairo::RefPtr<Cairo::Context>& cr)
{
    cr->set_line_width(10.0);
    cr->set_source_rgb(0.7, 0.7, 0.7);
    cr->rectangle(-dim_max,-dim_max,2*dim_max,2*dim_max);
    cr->stroke();
}

//=================================================================================//

void MyArea::refresh()
{
    auto win = get_window();
    if(win)
    {
        Gdk::Rectangle r(0,0, get_allocation().get_width(),
                              get_allocation().get_height());
        win->invalidate_rect(r,false);
    }
}

//=================================================================================//

Interface::Interface(int argc, char** argv)
:   m_Box(Gtk::ORIENTATION_VERTICAL, 20),
    m_Box_Up(Gtk::ORIENTATION_HORIZONTAL, 20),
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
    m_Box_Up.pack_start(m_Box_Left, false, true);
    m_Box_Up.pack_start(m_Area);
    m_Box_Left.pack_start(m_Frame1, false, true);
    m_Box_Left.pack_start(m_Frame2, false, true);
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
    m_Box.add(_scrolled_window);
	_scrolled_window.set_size_request(-1, 200);
	_scrolled_window.add(_tree_view);
	_scrolled_window.set_policy(Gtk::PolicyType::POLICY_AUTOMATIC,
							  Gtk::PolicyType::POLICY_AUTOMATIC);
	_scrolled_window.set_hexpand();
	_tree_view.append_column("uid", _columns._col_uid);
	_tree_view.append_column("nbP", _columns._col_nbP);
	_tree_view.append_column("nbF", _columns._col_nbF);
	_tree_view.append_column("nbT", _columns._col_nbT);
	_tree_view.append_column("nbC", _columns._col_nbC);
	_tree_view.append_column_numeric("Amount resource", _columns._col_resource, "%.2f");
	auto cell = Gtk::make_managed<Gtk::CellRendererProgress>();
	int cols_count = _tree_view.append_column("Mission completeness", *cell);
	auto progress_col = _tree_view.get_column(cols_count - 1);
	if(progress_col)
	progress_col->add_attribute(cell->property_value(),
								_columns._col_resource_percentage);
    init_simulation(argc, argv);
	tree_view_update();
    show_all_children();
}

//=================================================================================//

void init_simulation(int argc, char** argv)
{
    if(argc==2)
    {
        std::ifstream fichier(argv[1]);
        if(not(fichier.fail()))
        {
            simulation.clear();
            simulation.lecture(fichier);
            if(simulation.get_error_file())
            {
                simulation.clear();
            }
            simulation.adjacence();
        }
        fichier.close();
    }
}

//=================================================================================//

bool Interface::on_idle()
{
  if(start)
  {
      std::cout << "Mise à jour de la simulation numéro : " << ++count << std::endl;
      simulation.update();
  }
  return true;
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
    dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
    dialog.add_button("_Open", Gtk::RESPONSE_OK);
    m_Button_open.set_label("choosing a file");
    int result = dialog.run();
    m_Button_open.set_label("Open");
    switch(result)
    {
        case(Gtk::RESPONSE_OK):
        {
            std::cout << "Open clicked." << std::endl;
            std::string filename = dialog.get_filename();
            std::ifstream fichier(filename);
            if(not(fichier.fail()))
            {
                simulation.clear();
                simulation.lecture(fichier);
                if(simulation.get_error_file())
                {
                    simulation.clear();
                }
                simulation.adjacence();
                count=0;
                m_Area.refresh();
                this->tree_view_update();
            }
            fichier.close();
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
    std::cout<<"Save\n Entrer le nom du fichier de sortie : ";
    std::string filename;
    std::cin>>filename;
    std::ofstream fichier(filename);
    simulation.affiche_texte(fichier);
    fichier.close();
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
        simulation.update();
    }
}

//=================================================================================//

void Interface::on_button_clicked_toggle_link()
{
    if(not toggle_link)
    {
        toggle_link=true;
    }
    else
    {
        toggle_link=false;
    }
    m_Area.refresh();
}

//=================================================================================//

void Interface::on_button_clicked_toggle_range()
{
    if(not toggle_range)
    {
        toggle_range=true;
    }
    else
    {
        toggle_range=false;
    }
    m_Area.refresh();
}

// ===================== the parts to adapt have a comment ==================

void Interface::tree_view_update()
{
    Glib::RefPtr<Gtk::ListStore> ref_tree_model = Gtk::ListStore::create(_columns);
    _tree_view.set_model(ref_tree_model);
    
    std::vector<SimData> report(simulation.get_nbB());
    
    for(size_t i = 0 ; i <simulation.get_nbB(); ++i)
    {
        report[i].uid = i;
        report[i].nbP = simulation.get_base_nbP(i);
        report[i].nbF = simulation.get_base_nbF(i);
        report[i].nbT = simulation.get_base_nbT(i);
        report[i].nbC = simulation.get_base_nbC(i);
        report[i].ressource   = simulation.get_base_ressources(i);
        report[i].ressource_p = 100*(simulation.get_base_ressources(i))/(10*iniR);

        auto row = *(ref_tree_model->append());
        row[_columns._col_uid] = report[i].uid;
        row[_columns._col_nbP] = report[i].nbP;
        row[_columns._col_nbF] = report[i].nbF;
        row[_columns._col_nbT] = report[i].nbT;
        row[_columns._col_nbC] = report[i].nbC;
        row[_columns._col_resource] = report[i].ressource;
        row[_columns._col_resource_percentage] = report[i].ressource_p;
    }
}
