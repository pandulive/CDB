#include "LoginPage.h"
#include "Database.h"
#include <gtkmm.h>
#include <iostream>

LoginPage::LoginPage(Database& db) : Gtk::Box(Gtk::ORIENTATION_VERTICAL), database(db) {
    set_spacing(15);
    set_margin_top(50);
    set_margin_bottom(50);
    set_margin_left(50);
    set_margin_right(50);
    set_valign(Gtk::ALIGN_CENTER);
    set_halign(Gtk::ALIGN_CENTER);

    // Title Label
    auto* title_label = Gtk::make_managed<Gtk::Label>("<big><b>Welcome!</b></big>");
    title_label->set_use_markup(true);
    title_label->set_margin_bottom(20);
    pack_start(*title_label, Gtk::PACK_SHRINK);

    // Username Entry
    auto* username_box = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_HORIZONTAL);
    auto* username_icon = Gtk::make_managed<Gtk::Image>(Gtk::Stock::DIALOG_AUTHENTICATION, Gtk::ICON_SIZE_BUTTON);
    username_icon->set_margin_right(10);
    username_box->pack_start(*username_icon, Gtk::PACK_SHRINK);

    username_entry = Gtk::make_managed<Gtk::Entry>();
    username_entry->set_placeholder_text("Username");
    username_entry->set_width_chars(20); // Adjust to the desired width in characters
    username_entry->set_hexpand(true);
    username_box->pack_start(*username_entry, Gtk::PACK_EXPAND_WIDGET);
    pack_start(*username_box, Gtk::PACK_SHRINK);

    // Password Entry
    auto* password_box = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_HORIZONTAL);
    auto* password_icon = Gtk::make_managed<Gtk::Image>(Gtk::Stock::DIALOG_AUTHENTICATION, Gtk::ICON_SIZE_BUTTON);
    password_icon->set_margin_right(10);
    password_box->pack_start(*password_icon, Gtk::PACK_SHRINK);

    password_entry = Gtk::make_managed<Gtk::Entry>();
    password_entry->set_placeholder_text("Password");
    password_entry->set_width_chars(20);
    password_entry->set_visibility(false);
    password_box->pack_start(*password_entry, Gtk::PACK_EXPAND_WIDGET);
    pack_start(*password_box, Gtk::PACK_SHRINK);

    // Login Button
    login_button = Gtk::make_managed<Gtk::Button>("Login");
    login_button->set_margin_top(20);
    login_button->set_relief(Gtk::RELIEF_NONE); // Flat button
    login_button->set_name("login_button");
    login_button->signal_clicked().connect(sigc::mem_fun(*this, &LoginPage::on_login_button_clicked));
    pack_start(*login_button, Gtk::PACK_SHRINK);

    // Error Label
    error_label = Gtk::make_managed<Gtk::Label>();
    error_label->set_name("error_label"); // CSS name
    error_label->set_markup("<span foreground='red'></span>");
    pack_start(*error_label, Gtk::PACK_SHRINK);
}

void LoginPage::on_login_button_clicked() {
    std::string username = username_entry->get_text();
    std::string password = password_entry->get_text();

    // Use Database to check credentials
    if (database.check_credentials(username, password)) {
        error_label->set_markup("");
        login_success_signal.emit();
    } else {
        error_label->set_markup("<span foreground='red'>Invalid username or password</span>");
    }
}
