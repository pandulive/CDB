#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_
#include <gtkmm.h>
#include <iostream>
#include "LoginPage.h"
#include "AdminPanel.h"

class MainWindow : public Gtk::Window {
public:
MainWindow();
    LoginPage login_page;
    AdminPanel admin_panel;
    void on_login_success();
    void on_logout();

protected:
    
};

#endif // end_H_