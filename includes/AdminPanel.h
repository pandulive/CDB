#ifndef ADMINPANEL_H_
#define ADMINPANEL_H_
#include <gtkmm.h>
#include <iostream>

class AdminPanel : public Gtk::Box {
public:
    AdminPanel();

    sigc::signal<void> signal_logout; // Signal for logout

private:
    Gtk::Label admin_label;
    Gtk::Button logout_button;
    
    void on_logout_clicked();
};

#endif // end_H_