#include "MainWindow.h"
#include "LoginPage.h"
#include "RegisterPage.h"
#include "AdminPage.h"
#include "Database.h"
#include <gtkmm.h>
#include <iostream>

MainWindow::MainWindow(Database& db)
    : window_box(Gtk::ORIENTATION_VERTICAL), database(db)
{
    set_title("User Authentication System");
    set_default_size(800, 600);

    // Stack widget to manage different pages
    stack = Gtk::make_managed<Gtk::Stack>();
    stack->set_halign(Gtk::ALIGN_CENTER);
    stack->set_valign(Gtk::ALIGN_CENTER);

    // Create and add pages to the stack
    login_page = Gtk::make_managed<LoginPage>(database);
    register_page = Gtk::make_managed<RegisterPage>(database);
    admin_page = Gtk::make_managed<AdminPage>();

    stack->add_named(*login_page, "login_page");
    stack->add_named(*register_page, "register_page");
    stack->add_named(*admin_page, "admin_page");

    // When login is successful, transition to admin page
    login_page->signal_login_success().connect(sigc::mem_fun(*this, &MainWindow::on_login_success));

    // When register button is clicked, transition to the register page
    login_page->signal_register_button_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_register_button_clicked));

    // Handle back from register page
    register_page->signal_back_button_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_back_from_register_page));

    // Handle back from admin page
    admin_page->signal_logout_button_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_logout_button_clicked));

    // Add the stack to the main window
    window_box.pack_start(*stack, Gtk::PACK_EXPAND_WIDGET);
    add(window_box);
    show_all_children();
}

void MainWindow::on_login_success() {
    // Switch to admin page
    stack->set_visible_child("admin_page");
}

void MainWindow::on_register_button_clicked() {
    // Switch to registration page
    stack->set_visible_child("register_page");
}

void MainWindow::on_back_from_register_page() {
    // Switch back to login page
    stack->set_visible_child("login_page");
}

void MainWindow::on_logout_button_clicked() {
    // Switch back to login page after logout
    stack->set_visible_child("login_page");
}

