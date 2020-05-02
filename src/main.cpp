#include <iostream>

#include "Application.hpp"
#include "World.hpp"

int main(int argc, char * argv[])
{
    // Application handles rendering & interaction with SDL and OpenGL
    Application app;

    // World handles simulation logic
    World* pworld = new World();
    World& world = *pworld;

    // World grid size must divide app window size (800 by default)
    world.set_grid_size(app.DIMW / 4);

    app.loop(world);
    return 0;
}
