
#include"LoginPage.h"


   LoginPage::LoginPage (): Gtk::Box(Gtk::ORIENTATION_VERTICAL){
        set_spacing(10);

        // Username field
        username_label.set_text("Username:");
        pack_start(username_label, Gtk::PACK_SHRINK);
        pack_start(username_entry, Gtk::PACK_SHRINK);

        // Password field
        password_label.set_text("Password:");
        pack_start(password_label, Gtk::PACK_SHRINK);
        pack_start(password_entry, Gtk::PACK_SHRINK);
        password_entry.set_visibility(false);

        // Login button
        login_button.set_label("Login");
        login_button.signal_clicked().connect(
            sigc::mem_fun(*this, &LoginPage::on_login_clicked));
        pack_start(login_button, Gtk::PACK_SHRINK);

        std::cout << "[DEBUG] LoginPage created." << std::endl;
    }

    void LoginPage::on_login_clicked() {
        
        std::string username = username_entry.get_text();
        std::string password = password_entry.get_text();

        std::cout << "[DEBUG] Attempting login with Username: " << username
                  << ", Password: " << password << std::endl;

        if (username == "admin" && password == "password") {
            signal_login_success.emit(); // Emit success signal
        } else {
            std::cerr << "[DEBUG] Login failed!" << std::endl;
        }
    }
    

