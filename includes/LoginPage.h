#ifndef LOGINPAGE_H_
#define LOGINPAGE_H_
#include <gtkmm.h>
#include <iostream>

class LoginPage : public Gtk::Box {
public:
sigc::signal<void> signal_login_success;
LoginPage();


    
private:
    
    Gtk::Label username_label;
    Gtk::Entry username_entry;
    Gtk::Label password_label;
    Gtk::Entry password_entry;
    Gtk::Button login_button;
    Gtk::Label error_label;
    std::string username; 
    std::string password;

    void on_login_clicked();

};
    

#endif // end_H_