#include <gtkmm.h>

class MainWindow : public Gtk::Window {
public:
    MainWindow();

private:
    Gtk::Stack stack;          // Stack widget to hold multiple pages
    Gtk::Box login_page;       // Login page
    Gtk::Box register_page;    // Registration page

    void on_switch_to_register();  // Callback to switch pages
    void on_switch_to_login();     // Callback to return to login
};
