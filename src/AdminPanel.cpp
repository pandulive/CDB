#include"AdminPanel.h"

   AdminPanel::AdminPanel(): Gtk::Box(Gtk::ORIENTATION_VERTICAL){
        set_spacing(10);

        // Admin label
        admin_label.set_text("Welcome to the Admin Panel");
        pack_start(admin_label, Gtk::PACK_SHRINK);

        // Logout button
        logout_button.set_label("Logout");
        logout_button.signal_clicked().connect(
            sigc::mem_fun(*this, &AdminPanel::on_logout_clicked));
        pack_start(logout_button, Gtk::PACK_SHRINK);

        std::cout << "[DEBUG] AdminPanel created." << std::endl;
   }

    
    void AdminPanel:: on_logout_clicked() {
        std::cout << "[DEBUG] Logout button clicked." << std::endl;
        signal_logout.emit(); // Emit logout signal
    }

