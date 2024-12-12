#include"MainWindow.h"


   MainWindow:: MainWindow() {
        set_title("Main Application");
        set_default_size(1280, 720);

        // Connect login success signal
        login_page.signal_login_success.connect(
            sigc::mem_fun(*this, &MainWindow::on_login_success));

        // Connect logout signal
        admin_panel.signal_logout.connect(
            sigc::mem_fun(*this, &MainWindow::on_logout));

        // Start with the login page
        add(login_page);
        login_page.show_all();

        std::cout << "[DEBUG] MainWindow created with LoginPage." << std::endl;
    
    }



    void MainWindow::on_login_success() {
         std::cout << "[DEBUG] Switching to AdminPanel." << std::endl;

        // Remove current child (LoginPage) and add AdminPanel
        remove();
        add(admin_panel);
        admin_panel.show_all();
    }

    void MainWindow::on_logout() {
       std::cout << "[DEBUG] Logging out. Returning to LoginPage." << std::endl;

        // Remove current child (AdminPanel) and add LoginPage
        remove();
        add(login_page);
        login_page.show_all();
    }


