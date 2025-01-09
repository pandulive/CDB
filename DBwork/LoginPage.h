#ifndef LOGINPAGE_H
#define LOGINPAGE_H

#include <gtkmm.h>

class LoginPage : public Gtk::Box
{
public:
    // Constructor to initialize the LoginPage
    LoginPage();

    // Signal handler for successful login
    sigc::signal<void> signal_login_success();

private:
    // Widgets for the login page
    Gtk::Entry* username_entry;
    Gtk::Entry* password_entry;
    Gtk::Button* login_button;
    Gtk::Label* error_label;

    // Signal handler functions for button clicks
    void on_login_button_clicked();
};

#endif // LOGINPAGE_H
