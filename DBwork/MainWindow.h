#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <gtkmm.h>
#include "LoginPage.h"
#include "RegisterPage.h"
#include "AdminPage.h"
#include "Database.h"

class MainWindow : public Gtk::Window
{
public:
    // Constructor that accepts the database object
    MainWindow(Database& db);

    // Destructor
    virtual ~MainWindow() {}

private:
    Gtk::Box window_box;        // Container to hold the stack
    Gtk::Stack* stack;          // Stack to manage pages (Login, Register, Admin)
    LoginPage* login_page;      // Login page widget
    RegisterPage* register_page; // Register page widget
    AdminPage* admin_page;      // Admin page widget
    Database& database;         // Database reference

    // Signal handlers
    void on_login_success();
    void on_register_button_clicked();
    void on_back_from_register_page();
    void on_logout_button_clicked();
};

#endif // MAINWINDOW_H
