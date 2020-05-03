#pragma once

#include <iostream>

#include "Headers/World.hpp"
#include "Platform/Platform.hpp"
using namespace sf;

class Application
{
public:
    Application();
    ~Application();
    void loop(World& world);
    static const int DIMW = 800;
private:
    void update(World& world);
    void draw(World& world, float fps);

    RenderWindow window;
    Clock clock;
    Event event;
    util::Platform platform;
};
