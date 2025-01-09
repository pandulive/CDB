#include "RegisterPage.h"
#include "Database.h"
#include <gtkmm.h>
#include <iostream>

RegisterPage::RegisterPage(Database& db) : Gtk::Box(Gtk::ORIENTATION_VERTICAL), database(db) {
    set_spacing(15);
    set_margin_top(50);
    set_margin_bottom(50);
    set_margin_left(50);
    set_margin_right(50);
    set_valign(Gtk::ALIGN_CENTER);
    set_halign(Gtk::ALIGN_CENTER);

    // Title Label
    auto* title_label = Gtk::make_managed<Gtk::Label>("<big><b>User Registration</b></big>");
    title_label->set_use_markup(true);
    title_label->set_margin_bottom(20);
    pack_start(*title_label, Gtk::PACK_SHRINK);

    // First Name Entry
    first_name_entry = Gtk::make_managed<Gtk::Entry>();
    first_name_entry->set_placeholder_text("First Name");
    first_name_entry->set_width_chars(20);
    first_name_entry->set_hexpand(true);
    pack_start(*first_name_entry, Gtk::PACK_SHRINK);

    // Last Name Entry
    last_name_entry = Gtk::make_managed<Gtk::Entry>();
    last_name_entry->set_placeholder_text("Last Name");
    last_name_entry->set_width_chars(20);
    last_name_entry->set_hexpand(true);
    pack_start(*last_name_entry, Gtk::PACK_SHRINK);

    // Mobile No Entry
    mobile_no_entry = Gtk::make_managed<Gtk::Entry>();
    mobile_no_entry->set_placeholder_text("Mobile No");
    mobile_no_entry->set_width_chars(20);
    mobile_no_entry->set_hexpand(true);
    pack_start(*mobile_no_entry, Gtk::PACK_SHRINK);

    // Address Entry
    address_entry = Gtk::make_managed<Gtk::Entry>();
    address_entry->set_placeholder_text("Address");
    address_entry->set_width_chars(20);
    address_entry->set_hexpand(true);
    pack_start(*address_entry, Gtk::PACK_SHRINK);

    // Username Entry
    username_entry = Gtk::make_managed<Gtk::Entry>();
    username_entry->set_placeholder_text("Username");
    username_entry->set_width_chars(20);
    username_entry->set_hexpand(true);
    pack_start(*username_entry, Gtk::PACK_SHRINK);

    // Password Entry
    password_entry = Gtk::make_managed<Gtk::Entry>();
    password_entry->set_placeholder_text("Password");
    password_entry->set_width_chars(20);
    password_entry->set_visibility(false);
    password_entry->set_hexpand(true);
    pack_start(*password_entry, Gtk::PACK_SHRINK);

    // Register Button
    register_button = Gtk::make_managed<Gtk::Button>("Register");
    register_button->set_margin_top(20);
    register_button->set_relief(Gtk::RELIEF_NONE);
    register_button->set_name("register_button");
    register_button->signal_clicked().connect(sigc::mem_fun(*this, &RegisterPage::on_register_button_clicked));
    pack_start(*register_button, Gtk::PACK_SHRINK);

    // Error Label
    error_label = Gtk::make_managed<Gtk::Label>();
    error_label->set_name("error_label");
    error_label->set_markup("<span foreground='red'></span>");
    pack_start(*error_label, Gtk::PACK_SHRINK);
}

void RegisterPage::on_register_button_clicked() {
    std::string first_name = first_name_entry->get_text();
    std::string last_name = last_name_entry->get_text();
    std::string mobile_no = mobile_no_entry->get_text();
    std::string address = address_entry->get_text();
    std::string username = username_entry->get_text();
    std::string password = password_entry->get_text();

    if (database.register_user(first_name, last_name, mobile_no, address, username, password, "user")) {
        error_label->set_markup("<span foreground='green'>Registration successful!</span>");
    } else {
        error_label->set_markup("<span foreground='red'>Failed to register user!</span>");
    }
}
