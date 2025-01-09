#include <gtkmm.h>
#include <iostream>
#include <sqlite3.h> // For SQLite integration
#include <openssl/sha.h> // For SHA-256 hashing
#include <iomanip>
#include <sstream>

// Helper Function: Hash Password with SHA-256
std::string hash_password(const std::string& password) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char*)password.c_str(), password.size(), hash);

    std::ostringstream hashed_password;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        hashed_password << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    return hashed_password.str();
}

// SQLite Database Helper Class
class Database {
public:
    Database(const std::string& db_name) {
        if (sqlite3_open(db_name.c_str(), &db)) {
            std::cerr << "Failed to open database: " << sqlite3_errmsg(db) << std::endl;
        } else {
            std::cout << "Database opened successfully." << std::endl;
        }
    }

    ~Database() {
        sqlite3_close(db);
    }

    bool validate_user(const std::string& username, const std::string& password, std::string& role) {
        std::string hashed_password = hash_password(password);
        std::string query = "SELECT role FROM users WHERE username = ? AND password = ?";
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 2, hashed_password.c_str(), -1, SQLITE_STATIC);

            if (sqlite3_step(stmt) == SQLITE_ROW) {
                role = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
                sqlite3_finalize(stmt);
                return true;
            }
        }

        sqlite3_finalize(stmt);
        return false;
    }

private:
    sqlite3* db;
};

// Login Page Class
class LoginPage : public Gtk::Box {
public:
    LoginPage(Database& db_ref) : Gtk::Box(Gtk::ORIENTATION_VERTICAL), db(db_ref) {
        set_spacing(10);
        set_margin_top(50);
        set_margin_bottom(50);
        set_margin_left(50);
        set_margin_right(50);

        // Title
        auto* title_label = Gtk::make_managed<Gtk::Label>("<big><b>Login Page</b></big>");
        title_label->set_use_markup(true);
        pack_start(*title_label, Gtk::PACK_SHRINK);

        // Username Entry
        username_entry = Gtk::make_managed<Gtk::Entry>();
        username_entry->set_placeholder_text("Username");
        pack_start(*username_entry, Gtk::PACK_SHRINK);

        // Password Entry
        password_entry = Gtk::make_managed<Gtk::Entry>();
        password_entry->set_placeholder_text("Password");
        password_entry->set_visibility(false);
        pack_start(*password_entry, Gtk::PACK_SHRINK);

        // Login Button
        login_button = Gtk::make_managed<Gtk::Button>("Login");
        login_button->signal_clicked().connect(sigc::mem_fun(*this, &LoginPage::on_login_clicked));
        pack_start(*login_button, Gtk::PACK_SHRINK);

        // Error Label
        error_label = Gtk::make_managed<Gtk::Label>();
        error_label->set_use_markup(true);
        pack_start(*error_label, Gtk::PACK_SHRINK);
    }

    sigc::signal<void, std::string> signal_login_success() { return login_success_signal; }

private:
    Database& db;
    Gtk::Entry* username_entry;
    Gtk::Entry* password_entry;
    Gtk::Button* login_button;
    Gtk::Label* error_label;

    sigc::signal<void, std::string> login_success_signal;

    void on_login_clicked() {
        std::string username = username_entry->get_text();
        std::string password = password_entry->get_text();
        std::string role;

        if (db.validate_user(username, password, role)) {
            login_success_signal.emit(role);
        } else {
            error_label->set_markup("<span foreground='red'>Invalid username or password</span>");
        }
    }
};

// Main Window Class
class MainWindow : public Gtk::Window {
public:
    MainWindow() : db("users.db") {
        set_title("Authentication Example");
        set_default_size(800, 600);

        // Load Logo Page + Login Page initially
        home_layout = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_HORIZONTAL);

        // Left Pane: Logo
        auto* logo_label = Gtk::make_managed<Gtk::Label>("<big><b>Logo Here</b></big>");
        logo_label->set_use_markup(true);
        home_layout->pack_start(*logo_label, Gtk::PACK_EXPAND_WIDGET);

        // Right Pane: Login Page
        login_page = Gtk::make_managed<LoginPage>(db);
        login_page->signal_login_success().connect(sigc::mem_fun(*this, &MainWindow::on_login_success));
        home_layout->pack_start(*login_page, Gtk::PACK_EXPAND_WIDGET);

        add(*home_layout);
        show_all_children();
    }

private:
    Database db;
    Gtk::Box* home_layout;
    LoginPage* login_page;

    void on_login_success(const std::string& role) {
        // Remove Home Layout
        remove();

        // Add Admin Page
        auto* admin_layout = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_VERTICAL);
        auto* welcome_label = Gtk::make_managed<Gtk::Label>(
            "<big><b>Welcome " + role + "!</b></big>");
        welcome_label->set_use_markup(true);
        admin_layout->pack_start(*welcome_label, Gtk::PACK_EXPAND_WIDGET);

        auto* logout_button = Gtk::make_managed<Gtk::Button>("Logout");
        logout_button->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_logout_clicked));
        admin_layout->pack_start(*logout_button, Gtk::PACK_SHRINK);

        add(*admin_layout);
        show_all_children();
    }

    void on_logout_clicked() {
        // Remove Admin Page
        remove();

        // Restore Home Layout
        add(*home_layout);
        show_all_children();
    }
};

int main(int argc, char* argv[]) {
    auto app = Gtk::Application::create(argc, argv, "org.gtkmm.auth_example");

    // Set up SQLite database with sample users (for testing)
    sqlite3* db;
    if (sqlite3_open("users.db", &db) == SQLITE_OK) {
        const char* create_table_query =
            "CREATE TABLE IF NOT EXISTS users ("
            "username TEXT PRIMARY KEY, "
            "password TEXT NOT NULL, "
            "role TEXT NOT NULL);";
        sqlite3_exec(db, create_table_query, nullptr, nullptr, nullptr);

        const char* insert_query =
            "INSERT OR IGNORE INTO users (username, password, role) VALUES "
            "('admin', '8c6976e5b5410415bde908bd4dee15dfb167a9c873fc4bb8a81f6f2ab448a918', 'admin'), " // 'admin' hashed
            "('user', '04f8996da763b7a969b1028ee3007569eaf3a635486ddab211d512c85b9df8fb', 'user');";  // 'user' hashed
        sqlite3_exec(db, insert_query, nullptr, nullptr, nullptr);

        sqlite3_close(db);
    }

    MainWindow window;
    return app->run(window);
}
