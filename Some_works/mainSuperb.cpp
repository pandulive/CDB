#include <gtkmm.h>
#include <iostream>

class MainWindow : public Gtk::Window {
public:
    MainWindow() {
        set_title("Brand Login");

        // Set default window size
        set_default_size(800, 600);

        // Main horizontal paned widget
        Gtk::Paned* paned = Gtk::make_managed<Gtk::Paned>(Gtk::ORIENTATION_HORIZONTAL);
        add(*paned);

        // Left side: Brand logo
        Gtk::Box* left_box = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_VERTICAL);
        left_box->set_size_request(200, -1); // Fixed width for the left pane

        // Setting individual margins for the left box
        left_box->set_margin_top(10);
        left_box->set_margin_bottom(10);
        left_box->set_margin_left(10);
        left_box->set_margin_right(10);

        Gtk::Image* logo_image = Gtk::make_managed<Gtk::Image>("brand_logo.png");
        left_box->pack_start(*logo_image, Gtk::PACK_EXPAND_WIDGET);

        paned->add1(*left_box);

        // Right side: Login Page
        Gtk::Box* right_box = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_VERTICAL);

        // Setting individual margins for the right box
        right_box->set_margin_top(20);
        right_box->set_margin_bottom(20);
        right_box->set_margin_left(20);
        right_box->set_margin_right(20);
        right_box->set_spacing(10);

        // Add login fields to the right pane
        Gtk::Label* title_label = Gtk::make_managed<Gtk::Label>("<big><b>Login</b></big>", Gtk::ALIGN_CENTER);
        title_label->set_use_markup(true);
        right_box->pack_start(*title_label, Gtk::PACK_SHRINK);

        Gtk::Box* username_box = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_HORIZONTAL);
        Gtk::Label* username_label = Gtk::make_managed<Gtk::Label>("Username:");
        username_label->set_xalign(0.0);
        Gtk::Entry* username_entry = Gtk::make_managed<Gtk::Entry>();
        username_box->pack_start(*username_label, Gtk::PACK_SHRINK);
        username_box->pack_start(*username_entry, Gtk::PACK_EXPAND_WIDGET);
        right_box->pack_start(*username_box, Gtk::PACK_SHRINK);

        Gtk::Box* password_box = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_HORIZONTAL);
        Gtk::Label* password_label = Gtk::make_managed<Gtk::Label>("Password:");
        password_label->set_xalign(0.0);
        Gtk::Entry* password_entry = Gtk::make_managed<Gtk::Entry>();
        password_entry->set_visibility(false);
        password_box->pack_start(*password_label, Gtk::PACK_SHRINK);
        password_box->pack_start(*password_entry, Gtk::PACK_EXPAND_WIDGET);
        right_box->pack_start(*password_box, Gtk::PACK_SHRINK);

        Gtk::Button* login_button = Gtk::make_managed<Gtk::Button>("Login");
        login_button->get_style_context()->add_class("suggested-action");
        login_button->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_login_clicked));
        right_box->pack_start(*login_button, Gtk::PACK_SHRINK);

        paned->add2(*right_box);

        // Set resizing behavior for panes
        paned->set_position(200); // Ensure the left pane starts with a fixed size
        paned->set_wide_handle(true); // This makes the handle between panes wide for easier resizing

        show_all_children();
    }

private:
    void on_login_clicked() {
        std::cout << "Login button clicked!" << std::endl;
    }
};

int main(int argc, char* argv[]) {
    auto app = Gtk::Application::create(argc, argv, "org.gtkmm.split_example");

    MainWindow window;
    return app->run(window);
}
