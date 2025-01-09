#include "AdminPage.h"
#include "Database.h"
#include <iostream>
#include <gtkmm/button.h>
#include <gtkmm/label.h>

AdminPage::AdminPage() : Gtk::Box(Gtk::ORIENTATION_VERTICAL), m_logout_button("Logout"), m_register_user_button("Register New User") {
    set_spacing(10);
    set_margin_top(50);
    set_margin_bottom(50);
    set_margin_left(50);
    set_margin_right(50);

    // Title Label
    m_title_label.set_text("Admin Page");
    m_title_label.set_markup("<b><big>Welcome Admin!</big></b>");
    pack_start(m_title_label, Gtk::PACK_SHRINK);

    // Register User Button
    m_register_user_button.signal_clicked().connect(sigc::mem_fun(*this, &AdminPage::on_register_user_button_clicked));
    pack_start(m_register_user_button, Gtk::PACK_SHRINK);

    // Logout Button
    m_logout_button.signal_clicked().connect(sigc::mem_fun(*this, &AdminPage::on_logout_button_clicked));
    pack_start(m_logout_button, Gtk::PACK_SHRINK);

    show_all_children();
}

void AdminPage::on_register_user_button_clicked() {
    std::cout << "Navigating to user registration page..." << std::endl;
    signal_register_user.emit();  // Signal emitted to trigger registration page transition
}

void AdminPage::on_logout_button_clicked() {
    std::cout << "Logging out..." << std::endl;
    signal_logout.emit();  // Signal emitted to trigger logout action
}
