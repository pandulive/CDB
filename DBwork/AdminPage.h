#ifndef ADMINPAGE_H
#define ADMINPAGE_H

#include <gtkmm.h>

class AdminPage : public Gtk::Box {
public:
    AdminPage();

    sigc::signal<void> signal_register_user();
    sigc::signal<void> signal_logout();

private:
    void on_register_user_button_clicked();
    void on_logout_button_clicked();

    Gtk::Button m_register_user_button, m_logout_button;
    Gtk::Label m_welcome_label;

    sigc::signal<void> m_register_user_signal;
    sigc::signal<void> m_logout_signal;
};

#endif // ADMINPAGE_H
