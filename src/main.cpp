#include "MainWindow.h"

int main(int argc, char* argv[]) {
    auto app = Gtk::Application::create(argc, argv, "org.gtkmm.example");

    MainWindow main_window;

    std::cout << "[DEBUG] MainWindow instance created." << std::endl;
    return app->run(main_window);
}
