#include <gtkmm.h>
#include <iostream>

class UserRegistrationPage : public Gtk::Box {
public:
    UserRegistrationPage(Gtk::Stack* stack) : Gtk::Box(Gtk::ORIENTATION_VERTICAL), stack(stack) {
        set_spacing(15);
        set_margin_top(50);
        set_margin_bottom(50);
        set_margin_left(50);
        set_margin_right(50);

        // Create a label for registration title
        auto* title_label = Gtk::make_managed<Gtk::Label>("User Registration");
        title_label->set_use_markup(true);
        title_label->set_margin_bottom(20);
        pack_start(*title_label, Gtk::PACK_SHRINK);

        // Create entry fields for first name, last name, etc.
        first_name_entry = Gtk::make_managed<Gtk::Entry>();
        first_name_entry->set_placeholder_text("First Name");
        pack_start(*first_name_entry, Gtk::PACK_SHRINK);

        last_name_entry = Gtk::make_managed<Gtk::Entry>();
        last_name_entry->set_placeholder_text("Last Name");
        pack_start(*last_name_entry, Gtk::PACK_SHRINK);

        mobile_no_entry = Gtk::make_managed<Gtk::Entry>();
        mobile_no_entry->set_placeholder_text("Mobile No");
        pack_start(*mobile_no_entry, Gtk::PACK_SHRINK);

        address_entry = Gtk::make_managed<Gtk::Entry>();
        address_entry->set_placeholder_text("User Address");
        pack_start(*address_entry, Gtk::PACK_SHRINK);

        password_entry = Gtk::make_managed<Gtk::Entry>();
        password_entry->set_placeholder_text("Password");
        password_entry->set_visibility(false);
        pack_start(*password_entry, Gtk::PACK_SHRINK);

        // Add a Register Button
        register_button = Gtk::make_managed<Gtk::Button>("Register");
        register_button->signal_clicked().connect(sigc::mem_fun(*this, &UserRegistrationPage::on_register_button_clicked));
        pack_start(*register_button, Gtk::PACK_SHRINK);

        // Add Back Button
        back_button = Gtk::make_managed<Gtk::Button>("Back");
        back_button->signal_clicked().connect(sigc::mem_fun(*this, &UserRegistrationPage::on_back_button_clicked));
        pack_start(*back_button, Gtk::PACK_SHRINK);

        // Add Close Button
        close_button = Gtk::make_managed<Gtk::Button>("Close");
        close_button->signal_clicked().connect(sigc::mem_fun(*this, &UserRegistrationPage::on_close_button_clicked));
        pack_start(*close_button, Gtk::PACK_SHRINK);
    }

private:
    Gtk::Entry* first_name_entry;
    Gtk::Entry* last_name_entry;
    Gtk::Entry* mobile_no_entry;
    Gtk::Entry* address_entry;
    Gtk::Entry* password_entry;
    Gtk::Button* register_button;
    Gtk::Button* back_button;
    Gtk::Button* close_button;
    Gtk::Stack* stack;

    void on_register_button_clicked() {
        std::string first_name = first_name_entry->get_text();
        std::string last_name = last_name_entry->get_text();
        std::string mobile_no = mobile_no_entry->get_text();
        std::string address = address_entry->get_text();
        std::string password = password_entry->get_text();

        // Here you can add your logic to store the user data (e.g., to a database).
        std::cout << "Registered User: " << first_name << " " << last_name << std::endl;
    }

    void on_back_button_clicked() {
        // Switch back to the login page
        stack->set_visible_child("login_page");
    }

    void on_close_button_clicked() {
        // Close the application
        Gtk::Main::quit();
    }
};

class MainWindow : public Gtk::Window {
public:
    MainWindow() {
        set_title("Dynamic Resizing Example");
        set_default_size(800, 600);

        // Initialize Stack
        stack = Gtk::make_managed<Gtk::Stack>();
        add(*stack);

        // Create the Login page and add it to the stack
        login_page = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_VERTICAL);
        auto* login_label = Gtk::make_managed<Gtk::Label>("Login Page");
        login_label->set_use_markup(true);
        login_page->pack_start(*login_label, Gtk::PACK_SHRINK);

        // Create Username and Password fields for login
        username_entry = Gtk::make_managed<Gtk::Entry>();
        username_entry->set_placeholder_text("Username");
        login_page->pack_start(*username_entry, Gtk::PACK_SHRINK);

        password_entry = Gtk::make_managed<Gtk::Entry>();
        password_entry->set_placeholder_text("Password");
        password_entry->set_visibility(false);
        login_page->pack_start(*password_entry, Gtk::PACK_SHRINK);

        login_button = Gtk::make_managed<Gtk::Button>("Login");
        login_button->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_login_clicked));
        login_page->pack_start(*login_button, Gtk::PACK_SHRINK);

        stack->add(*login_page, "login_page");

        // Create the Registration page and add it to the stack
        registration_page = Gtk::make_managed<UserRegistrationPage>(stack);
        stack->add(*registration_page, "registration_page");

        // Set the Login page as the default page
        stack->set_visible_child(*login_page);

        // Show all widgets
        show_all_children();
    }

private:
    Gtk::Stack* stack;
    Gtk::Box* login_page;
    UserRegistrationPage* registration_page;
    Gtk::Button* login_button;
    Gtk::Entry* username_entry;
    Gtk::Entry* password_entry;

    void on_login_clicked() {
        std::string username = username_entry->get_text();
        std::string password = password_entry->get_text();

        // Simple validation for username and password
        if (username == "admin" && password == "password") {
            std::cout << "Login Successful!" << std::endl;
            // Transition to the User Registration Page
            stack->set_visible_child(*registration_page);
        } else {
            std::cout << "Invalid username or password!" << std::endl;
        }
    }
};

int main(int argc, char* argv[]) {
    auto app = Gtk::Application::create(argc, argv, "org.gtkmm.dynamic_page_example");

    MainWindow window;
    return app->run(window);
}
