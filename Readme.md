Compilations

g++ -o login_app main.cpp `pkg-config gtkmm-3.0 --cflags --libs` -lsqlite3


g++ -o gtk_app main.cpp `pkg-config gtkmm-3.0 --cflags --libs`

g++ -o hash_example hash_example.cpp -lssl -lcrypto
g++ -o hash_example hash_example.cpp -lssl -lcrypto -Wall -Wno-deprecated-declarations
 

mainSuperb4 is Transition :- It is for Password encryption


g++ -o login_app main.cpp `pkg-config gtkmm-3.0 --cflags --libs` -lsqlite3 -lssl -lcrypto -Wall -Wno-deprecated-declarations


g++ main.cpp MainWindow.cpp LoginPage.cpp Database.cpp RegisterPage.cpp AdminPage.cpp     -o admin_panel `pkg-config --cflags --libs gtkmm-3.0` -lsqlite3 -lssl -lcrypto -Wall -Wno-deprecated-declarations