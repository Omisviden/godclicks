#include "Application.hpp"
#include "World.hpp"

using namespace std;

Application::Application()
{

    m_window = SDL_CreateWindow("SDL2 Window",
                                SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED,
                                DIMW, DIMW,
                                0);

    if(!m_window)
    {
        cout << "Failed to create window" << endl;
        cout << "SDL2 Error: " << SDL_GetError() << endl;
        return;
    }

    m_window_renderer = SDL_CreateRenderer(m_window, -1,
                                           SDL_RENDERER_ACCELERATED |
                                           SDL_RENDERER_PRESENTVSYNC);

    if(!m_window_renderer)
    {
        cout << "Failed to get window's surface";
        cout << "SDL2 Error: " << SDL_GetError() <<endl;
        return;
    }
}

Application::~Application()
{
    SDL_DestroyRenderer(m_window_renderer);
    SDL_DestroyWindow(m_window);
}

void Application::loop(World &world)
{
    bool keep_window_open = true;

    // These booleans keep track of whether a pressed mouse button was released
    // for repeat actions such as placing a particle or dragging something
    bool left_clicking = false;
    bool right_clicking = false;

    int grid_size = world.get_grid_size();

    while(keep_window_open)
    {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        // This loop iterates over queued events and treats them adequately
        while(SDL_PollEvent(&m_window_event) > 0)
        {
            switch(m_window_event.type)
            {
                case SDL_QUIT:
                    keep_window_open = false;
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    if(m_window_event.button.button == SDL_BUTTON_LEFT)
                        left_clicking = true;
                    if(m_window_event.button.button == SDL_BUTTON_RIGHT)
                        right_clicking = true;
                    break;

                case SDL_MOUSEBUTTONUP:
                    if(m_window_event.button.button == SDL_BUTTON_LEFT)
                        left_clicking = false;
                    if(m_window_event.button.button == SDL_BUTTON_RIGHT)
                        right_clicking = false;
                    break;
            }
        }
        if(left_clicking)
        {
            // The new particle's x and y is proportional to the grid size / window size ratio
            int x = mouseX * grid_size / this->DIMW;
            int y = mouseY * grid_size / this->DIMW;

            //TODO: Menu selection + different particle types
            if(world.get_grid_index(x, y) == -1)
            {
                world.particles.push_back(world.create_particle(max(1, min(x, grid_size-2)), max(1, min(y, grid_size-2)), 0, 119, 190, 20, 1, 0.5f));
            }
        }
    update(world);
    draw(world, 60);
    }
}

void Application::update(World &world)
{
    world.update_particles();
}

void Application::draw(World &world, float fps)
{
    float frame_delay = 1000 / fps;
    float frame_start = SDL_GetTicks();
    SDL_RenderClear(m_window_renderer);

    // Draw pixel for each particle in world grid
    for(unsigned int i = 0; i < world.particles.size(); i++)
    {
        int grid_size = world.get_grid_size();
        int x = world.particles[i].x * (this->DIMW / grid_size);
        int y = world.particles[i].y * (this->DIMW / grid_size);
        int r = world.particles[i].r;
        int g = world.particles[i].g;
        int b = world.particles[i].b;

        SDL_Rect rect;
        rect.x = x;
        rect.y = y;
        rect.w = 1 + this->DIMW / grid_size;
        rect.h = 1 + this->DIMW / grid_size;

        SDL_SetRenderDrawColor(m_window_renderer, r, g, b, 255);
        SDL_RenderFillRect(m_window_renderer, &rect);
    }

    SDL_SetRenderDrawColor(m_window_renderer, 0, 0, 0, 255);
    SDL_RenderPresent(m_window_renderer);

    float frame_time = SDL_GetTicks() - frame_start;
    if (frame_delay > frame_time)
    {
        SDL_Delay(frame_delay - frame_time);
    }
}
