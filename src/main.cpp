
#include "MainWindow.h"

int main(int argc, char* argv[]) {
    auto app = Gtk::Application::create(argc, argv, "org.gtkmm.example");

    auto css_provider = Gtk::CssProvider::create();
    css_provider->load_from_path("../src/style.css");
    auto screen = Gdk::Screen::get_default();
    auto style_context = Gtk::StyleContext::create();
    style_context->add_provider_for_screen(screen, css_provider, GTK_STYLE_PROVIDER_PRIORITY_USER);

    MainWindow main_window;
    return app->run(main_window);
}
