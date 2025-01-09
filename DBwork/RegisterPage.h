#ifndef REGISTERPAGE_H
#define REGISTERPAGE_H

#include <gtkmm.h>

class RegisterPage : public Gtk::Box
{
public:
    // Constructor to initialize the RegisterPage
    RegisterPage();

    // Signal handler for the Register button click
    sigc::signal<void> signal_register_success();

    // Signal handler for the Back button click
    sigc::signal<void> signal_back();

private:
    // Widgets for the registration page
    Gtk::Entry* first_name_entry;
    Gtk::Entry* last_name_entry;
    Gtk::Entry* mobile_entry;
    Gtk::Entry* address_entry;
    Gtk::Entry* password_entry;
    Gtk::Button* register_button;
    Gtk::Button* back_button;
    Gtk::Label* error_label;

    // Signal handler functions for button clicks
    void on_register_button_clicked();
    void on_back_button_clicked();
};

#endif // REGISTERPAGE_H
