
#include"LoginPage.h"



   LoginPage::LoginPage (): Gtk::Box(Gtk::ORIENTATION_VERTICAL){
        set_spacing(15);
        set_margin_top(20); // Add outer margin
        set_margin_bottom(20);

        // Frame for login fields
        Gtk::Frame* login_frame = Gtk::make_managed<Gtk::Frame>("Login");
        Gtk::Box* login_box = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_VERTICAL);
        login_box->set_spacing(10);
        login_box->set_margin_top(10);
        login_box->set_margin_bottom(10);
        login_frame->add(*login_box);
        pack_start(*login_frame, Gtk::PACK_SHRINK);

        // Username field
        Gtk::Box* username_box = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_HORIZONTAL);
        Gtk::Label* username_label = Gtk::make_managed<Gtk::Label>("Username:");
        username_label->set_xalign(0.0); // Left align
        username_entry.set_placeholder_text("Enter username");
        username_box->pack_start(*username_label, Gtk::PACK_SHRINK);
        username_box->pack_start(username_entry, Gtk::PACK_EXPAND_WIDGET);
        login_box->pack_start(*username_box, Gtk::PACK_SHRINK);

        // Password field
        Gtk::Box* password_box = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_HORIZONTAL);
        Gtk::Label* password_label = Gtk::make_managed<Gtk::Label>("Password:");
        password_label->set_xalign(0.0); // Left align
        password_entry.set_placeholder_text("Enter password");
        password_entry.set_visibility(false);
        password_box->pack_start(*password_label, Gtk::PACK_SHRINK);
        password_box->pack_start(password_entry, Gtk::PACK_EXPAND_WIDGET);
        login_box->pack_start(*password_box, Gtk::PACK_SHRINK);

        // Login button
        login_button.set_label("Login");
        login_button.get_style_context()->add_class("suggested-action"); // Highlight button
        login_button.signal_clicked().connect(
            sigc::mem_fun(*this, &LoginPage::on_login_clicked));
        login_box->pack_start(login_button, Gtk::PACK_SHRINK);

        // Error label
        error_label.set_halign(Gtk::ALIGN_CENTER);
        error_label.get_style_context()->add_class("error-text");
        pack_start(error_label, Gtk::PACK_SHRINK);

        std::cout << "[DEBUG] LoginPage created with UI enhancements." << std::endl;

    }

    void LoginPage::on_login_clicked() {
        
        std::string username = username_entry.get_text();
        std::string password = password_entry.get_text();

        if (username == "admin" && password == "password") {
            signal_login_success.emit();
        } else {
            error_label.set_text("Invalid username or password.");
            error_label.get_style_context()->add_class("error-text");
        }
    }
    

