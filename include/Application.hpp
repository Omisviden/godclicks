#pragma once

#include <SDL2/SDL.h>
#include <iostream>
#include <memory>
#include "World.hpp"

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
    SDL_Window   *m_window;
    SDL_Renderer *m_window_renderer;
    SDL_Event     m_window_event;
};
