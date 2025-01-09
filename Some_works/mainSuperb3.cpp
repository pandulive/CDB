#include <gtkmm.h>
#include <iostream>
#include <gdkmm/pixbuf.h>
#include <algorithm>

class LogoArea : public Gtk::DrawingArea {
public:
    LogoArea(const std::string& image_path) {
        try {
            logo_pixbuf = Gdk::Pixbuf::create_from_file(image_path);
        } catch (const Glib::FileError& e) {
            std::cerr << "FileError: " << e.what() << std::endl;
        } catch (const Gdk::PixbufError& e) {
            std::cerr << "PixbufError: " << e.what() << std::endl;
        }
        set_hexpand(true);
        set_vexpand(true);
    }

protected:
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override {
        if (!logo_pixbuf) {
            std::cerr << "Error: No image loaded." << std::endl;
            return false;
        }

        // Get the allocation size (current size of the DrawingArea)
        Gtk::Allocation allocation = get_allocation();
        int area_width = allocation.get_width();
        int area_height = allocation.get_height();

        // Ensure valid width and height
        if (area_width <= 0 || area_height <= 0) {
            std::cerr << "Error: Invalid allocation size: " << area_width << "x" << area_height << std::endl;
            return false; // Prevent division by zero or invalid scaling
        }

        // Scale the image proportionally
        double scale_x = static_cast<double>(area_width) / logo_pixbuf->get_width();
        double scale_y = static_cast<double>(area_height) / logo_pixbuf->get_height();
        double scale = std::min(scale_x, scale_y);

        int scaled_width = logo_pixbuf->get_width() * scale;
        int scaled_height = logo_pixbuf->get_height() * scale;

        // Center the image in the area
        int offset_x = (area_width - scaled_width) / 2;
        int offset_y = (area_height - scaled_height) / 2;

        // Draw the scaled image
        auto scaled_pixbuf = logo_pixbuf->scale_simple(scaled_width, scaled_height, Gdk::INTERP_BILINEAR);
        Gdk::Cairo::set_source_pixbuf(cr, scaled_pixbuf, offset_x, offset_y);
        cr->paint();

        return true;
    }

private:
    Glib::RefPtr<Gdk::Pixbuf> logo_pixbuf;
};


class LoginPage : public Gtk::Box {
public:
    LoginPage() : Gtk::Box(Gtk::ORIENTATION_VERTICAL) {
        
        set_spacing(15);
        set_margin_top(50);
        set_margin_bottom(50);
        set_margin_left(50);
        set_margin_right(50);
        set_valign(Gtk::ALIGN_CENTER);
        set_halign(Gtk::ALIGN_CENTER);
        set_name("login_panel");

        // Title Label
        auto* title_label = Gtk::make_managed<Gtk::Label>("<big><b>Welcome Pandu!</b></big>");
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
        login_button->set_name("login_button"); // CSS name
        login_button->signal_clicked().connect(sigc::mem_fun(*this, &LoginPage::on_login_button_clicked));

        pack_start(*login_button, Gtk::PACK_SHRINK);

        // Error Label
        error_label = Gtk::make_managed<Gtk::Label>();
        error_label->set_name("error_label"); // CSS name
        error_label->set_markup("<span foreground='red'></span>");
        pack_start(*error_label, Gtk::PACK_SHRINK);

        // Apply CSS
        apply_css();
    }

    sigc::signal<void> signal_login_success() { return login_success_signal; }

private:
    Gtk::Entry* username_entry;
    Gtk::Entry* password_entry;
    Gtk::Button* login_button;
    Gtk::Label* error_label;
    sigc::signal<void> login_success_signal;

    void on_login_button_clicked() {
        std::string username = username_entry->get_text();
        std::string password = password_entry->get_text();

        if (username == "admin" && password == "password") {
            error_label->set_markup("");
            login_success_signal.emit();
        } else {
            error_label->set_markup("<span foreground='red'>Invalid username or password</span>");
        }
    }

    void apply_css() {
        auto css_provider = Gtk::CssProvider::create();
        css_provider->load_from_data(
            "button#login_button {"
            "  background-color: #4CAF50;"
            "  color: white;"
            "  font-weight: bold;"
            "  border-radius: 5px;"
            "  padding: 10px;"
            "}"
            "button#login_button:hover {"
            "  background-color: #45a049;"
            "}"
            "label#error_label {"
            "  font-size: 12px;"
            "  margin-top: 5px;"
            "}"
            
            "#login_panel {"
            "  background-color: #0000ff;" // Light gray background
            "  border-radius: 10px;"       // Optional: Rounded corners
            "  padding: 20px;"             // Optional: Padding inside the panel
            "}"
        );

        auto screen = Gdk::Screen::get_default();
        Gtk::StyleContext::add_provider_for_screen(screen, css_provider, GTK_STYLE_PROVIDER_PRIORITY_USER);
    }
};

class MainWindow : public Gtk::Window {
public:
    MainWindow() {
        set_title("Dynamic Resizing Example");
        set_default_size(800, 600);

        paned = Gtk::make_managed<Gtk::Paned>(Gtk::ORIENTATION_HORIZONTAL);
        add(*paned);

        // Left panel with logo
        logo_area = Gtk::make_managed<LogoArea>("logo.png");
        logo_area->set_size_request(300, 200);  // Set a fixed size for testing

        paned->pack1(*logo_area, false, false);

        // Right panel with login
        login_page = Gtk::make_managed<LoginPage>();
        login_page->signal_login_success().connect(sigc::mem_fun(*this, &MainWindow::on_login_success));
        paned->pack2(*login_page, false, false);

        show_all_children();
    }

private:
    Gtk::Paned* paned;
    LogoArea* logo_area;
    LoginPage* login_page;

    void on_login_success() {
        std::cout << "Login Successful!" << std::endl;
        // Transition to the main admin page
    }
};

int main(int argc, char* argv[]) {
    auto app = Gtk::Application::create(argc, argv, "org.gtkmm.dynamic_logo_resize");

    MainWindow window;
    return app->run(window);
}
