#include "MainWindow.h"

int main(int argc, char* argv[]) {
    auto app = Gtk::Application::create(argc, argv, "org.gtkmm.stack_example");

    MainWindow window;
    return app->run(window);
}
