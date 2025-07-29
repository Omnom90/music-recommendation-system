#include <iostream>
#include "user_interface.h"
using namespace std;

int main() {
    cout << "Welcome to the Music Recommender System!" << endl;

    UserInterface ui;
    ui.run();

    cout << "Thank You For Using My System!" << endl;

    return 0;
}