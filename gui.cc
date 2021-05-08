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
#include <gtkmm.h>
#include <cairomm/context.h>
#include "gui.h"
#include "geomod.h"
#include "constantes.h"

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

constexpr unsigned max_tab(10);

struct Frame // Framing and window parameters
{
    double xMin;
    double xMax;
    double yMin;
    double yMax;
    
};

static Frame frame;

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

//=================================================================================//

MyArea::MyArea()
{
	frame.xMax= dim_max;
	frame.xMin=-dim_max;
	frame.yMax= dim_max;
	frame.yMin=-dim_max;
 }

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

bool MyArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
    //activate_graphic(cr);//A mettre dans GRAPHIC
    draw_frame(cr);
    
    Gtk::Allocation allocation = get_allocation();
    const int width = allocation.get_width();
    const int height = allocation.get_height();

    
    double new_aspect_ratio((double) width/height);
    double size(0.0);
    
    if(width>=height)
    {
        size=height;
        allocation.set_width(800);
        std::cout<<"vida loca\n";
        this->set_allocation(allocation);
    }
    else
    {
        size=width;
        allocation.set_height(1000);
        this->set_allocation(allocation);
    }
    cr->translate(size/2, size/2);
    cr->scale(size/(frame.xMax - frame.xMin), -size/(frame.yMax - frame.yMin));
    
    
    
    Cercle test(700, -900, 300);
    Point p1(900,0), p2(0,0);
    p1.cercle_communication(cr);
    p1.ligne_reseau(p2, cr);
    
    test.affiche(cr);
    return true;
}

//=================================================================================//

Interface::Interface()
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
	tree_view_update();

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



// ===================== the parts to adapt have a comment ==================
void Interface::tree_view_update()
{
  Glib::RefPtr<Gtk::ListStore> ref_tree_model = Gtk::ListStore::create(_columns);
  _tree_view.set_model(ref_tree_model);
 static std::vector<SimData> report(max_tab);
  if(true) // here there should be a test about the existence of a simulation
  {
	// here a call to a method from your simulation class should create and 
	// return a vector like report, except its number of lines will be
	// determined by the simulation (not a constant like in this toy example).
	if(count > report.size()) 
		std::cout << max_tab << " lines max are displayed" << std::endl;

	//for(size_t i = 0 ; i < count and i <report.size() ; ++i){
	report[0].uid = 1;
	  report[0].nbP = 8;
	  report[0].nbF = 9;
	  report[0].nbT = 4;
	  report[0].nbC = 4;
	  report[0].ressource   = 1000*2;
	  report[0].ressource_p = 10*1;
		
	  auto row = *(ref_tree_model->append());
	  row[_columns._col_nbP] = report[0].nbP;
	  row[_columns._col_nbF] = report[0].nbF;
	  row[_columns._col_nbT] = report[0].nbT;
      row[_columns._col_nbC] = report[0].nbC;
	  row[_columns._col_resource] = report[0].ressource;
	  row[_columns._col_resource_percentage] = report[0].ressource_p;
	
	
	// please note that you have to bring the mouse cursor to the right side 
	// of the scroll window to see the scrolling widget
  }
}
