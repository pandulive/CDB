#include "MainWindow.h"

MainWindow::MainWindow() {
    set_title("Stack Window Example");
    set_default_size(600, 400);

    // ===== Login Page =====
    login_page.set_spacing(10);
    auto login_label = Gtk::make_managed<Gtk::Label>("Login Page");
    auto switch_to_register = Gtk::make_managed<Gtk::Button>("Go to Register");
    switch_to_register->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_switch_to_register));

    login_page.pack_start(*login_label);
    login_page.pack_start(*switch_to_register);

    // ===== Register Page =====
    register_page.set_spacing(10);
    auto register_label = Gtk::make_managed<Gtk::Label>("Register Page");
    auto switch_to_login = Gtk::make_managed<Gtk::Button>("Back to Login");
    switch_to_login->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_switch_to_login));

    register_page.pack_start(*register_label);
    register_page.pack_start(*switch_to_login);

    // Add pages to the stack
    stack.add(login_page, "login");
    stack.add(register_page, "register");

    // stack.add(*register_page, "register_page");
    // stack.add(*login_page, "login_page");

    // Set initial visible child
    stack.set_visible_child("login");

    add(stack);  // Add the stack to the main window

    show_all_children();
}

// ===== Callbacks =====
void MainWindow::on_switch_to_register() {
    stack.set_visible_child("register");
}

void MainWindow::on_switch_to_login() {
    stack.set_visible_child("login");
}
