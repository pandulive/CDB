#include <gtkmm.h>
#include <iostream>
#include <gdkmm/pixbuf.h>

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
    }

protected:
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override {
        if (!logo_pixbuf) {
            std::cerr << "Error: No image loaded." << std::endl;
            return false;
        }

        // Get the allocation size
        Gtk::Allocation allocation = get_allocation();
        int area_width = allocation.get_width();
        int area_height = allocation.get_height();

        if (area_width == 0 || area_height == 0) {
            std::cerr << "Error: Invalid allocation size." << std::endl;
            return false; // Prevent division by zero
        }

        // Scale the image proportionally
        double scale_x = static_cast<double>(area_width) / logo_pixbuf->get_width();
        double scale_y = static_cast<double>(area_height) / logo_pixbuf->get_height();
        double scale = std::min(scale_x, scale_y);

        int scaled_width = logo_pixbuf->get_width() * scale;
        int scaled_height = logo_pixbuf->get_height() * scale;

        // Center the image
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

class MainWindow : public Gtk::Window {
public:
    MainWindow() {
        set_title("Dynamic Resizing Example");
        set_default_size(800, 600);

        // Main Paned Widget
        paned = Gtk::make_managed<Gtk::Paned>(Gtk::ORIENTATION_HORIZONTAL);
        add(*paned);

        // Left Pane: Logo Area
        auto* logo_area = Gtk::make_managed<LogoArea>("brand_logo.png");
        paned->add1(*logo_area);

        // Right Pane: Login Placeholder
        Gtk::Box* right_box = Gtk::make_managed<Gtk::Box>();
        right_box->set_orientation(Gtk::ORIENTATION_VERTICAL);
        right_box->set_spacing(10);

        Gtk::Label* title_label = Gtk::make_managed<Gtk::Label>("<big><b>Login</b></big>");
        title_label->set_use_markup(true);
        right_box->pack_start(*title_label, Gtk::PACK_SHRINK);

        Gtk::Entry* username_entry = Gtk::make_managed<Gtk::Entry>();
        username_entry->set_placeholder_text("Username");
        right_box->pack_start(*username_entry, Gtk::PACK_SHRINK);

        Gtk::Entry* password_entry = Gtk::make_managed<Gtk::Entry>();
        password_entry->set_placeholder_text("Password");
        password_entry->set_visibility(false);
        right_box->pack_start(*password_entry, Gtk::PACK_SHRINK);

        Gtk::Button* login_button = Gtk::make_managed<Gtk::Button>("Login");
        login_button->signal_clicked().connect([]() {
            std::cout << "Login button clicked!" << std::endl;
        });
        right_box->pack_start(*login_button, Gtk::PACK_SHRINK);

        paned->add2(*right_box);

        paned->set_position(300); // Initial position
        show_all_children();
    }

private:
    Gtk::Paned* paned;
};

int main(int argc, char* argv[]) {
    auto app = Gtk::Application::create(argc, argv, "org.gtkmm.dynamic_logo_resize");

    MainWindow window;
    return app->run(window);
}
