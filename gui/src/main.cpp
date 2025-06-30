#include "../include/Application.hpp"

int main(int argc, char* argv[])
{
    Zappy::Application app;
    
    if (!app.initialize(argc, argv)) {
        return 84;
    }
    
    app.run();
    return 0;
}