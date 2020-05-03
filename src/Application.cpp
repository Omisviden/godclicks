#include "Platform/Platform.hpp"

#include "Headers/Application.hpp"
#include "Headers/World.hpp"

using namespace std;
using namespace sf;

Application::Application() : window(), clock(), event(), platform()
{
/*
#if defined(_DEBUG)
	cout << "Hello World!" << std::endl;
#endif
*/
	// in Windows at least, this must be called before creating the window
	float screen_scaling_factor = platform.getScreenScalingFactor(window.getSystemHandle());
	// Use the screenScalingFactor
	this->platform.setIcon(window.getSystemHandle());
    this->window.create(VideoMode(DIMW * screen_scaling_factor, DIMW * screen_scaling_factor), "God Clicks 0.1");
    this->clock.restart();
}

Application::~Application()
{
    this->window.~RenderWindow();
}

void Application::loop(World &world)
{

    // These booleans keep track of whether a pressed mouse button was released
    // for repeat actions such as placing a particle or dragging something

    int grid_size = world.get_grid_size();

    bool left_clicking = false;
    bool right_clicking = false;

    while(window.isOpen())
    {
        Vector2<int> mousePos = Mouse::getPosition(window);
        int x = mousePos.x * grid_size / this->DIMW;
        int y = mousePos.y * grid_size / this->DIMW;

        // This loop iterates over queued events and treats them adequately
        while(window.pollEvent(event))
        {
            switch(event.type)
            {
                case Event::Closed:
                    window.close();
                    break;
                case Event::MouseButtonPressed:
                    if(event.mouseButton.button == Mouse::Left)
                        left_clicking = true;
                    else if(event.mouseButton.button == Mouse::Right)
                        right_clicking = true;
                    break;
                case Event::MouseButtonReleased:
                    if(event.mouseButton.button == Mouse::Left)
                        left_clicking = false;
                    else if(event.mouseButton.button == Mouse::Right)
                        right_clicking = false;
                    break;
                default:
                    break;
            }

            //TODO: Menu selection + different particle types
            if(left_clicking)
            {
                if(world.get_grid_index(x, y) == -1)
                {
                    int px = max(1, min(x, grid_size-2));
                    int py = max(1, min(y, grid_size-2));
                    World::Particle particle = world.create_particle(px, py, 0, 119, 190, 20, 1, 0.5f);
                    world.particles.push_back(particle);
                }
            }
            if(right_clicking)
            {
                '''Right clicking does not do anything yet!'''
            }
        }
    update(world);
    draw(world, 60);
    }
    window.display();
}

void Application::update(World &world)
{
    world.update_particles();
}

void Application::draw(World &world, float fps)
{
    Time frame_delay = milliseconds(1000 / fps);
    Time frame_start = clock.getElapsedTime();

    window.clear(Color::Black);

    // Draw pixel for each particle in world grid
    for(unsigned int i = 0; i < world.particles.size(); i++)
    {
        int grid_size = world.get_grid_size();
        int x = world.particles[i].x * (this->DIMW / grid_size);
        int y = world.particles[i].y * (this->DIMW / grid_size);
        int r = world.particles[i].r;
        int g = world.particles[i].g;
        int b = world.particles[i].b;

        RectangleShape rect;
        rect.setPosition(x, y);
        rect.setSize(Vector2f(1 + this->DIMW / grid_size, 1 + this->DIMW / grid_size));
        rect.setFillColor(Color(r, g, b));
        window.draw(rect);
    }

    Time frame_time = clock.getElapsedTime() - frame_start;
    if (frame_delay > frame_time)
    {
        sleep(frame_delay - frame_time);
    }
}
