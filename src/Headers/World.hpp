#pragma once

#include <string>
#include <vector>

#define G_INIT_SIZE 500

using namespace sf;

class World
{
public:
    World();
    void update_world();
    void set_name(std::string name);
    void set_grid_size(int grid_size);
    int get_grid_size();
    int get_grid_index(int x, int y);
    void update_particles();
private:

    struct Particle
    {
        int x, y, r, g, b, heat, type;
        float conductivity;
    };

    int grid_size;
    std::string name;
    int grid[G_INIT_SIZE][G_INIT_SIZE];
    int heatmap[G_INIT_SIZE][G_INIT_SIZE];

    int interpolate(int a, int b, float weight);
    int compute_heat(int i, float conductivity);
    void update_water(int i);
    void update_steam(int i);
    void update_ice(int i);
public:
    std::vector<Particle> particles; // push_back == append
    void create_particle(int x, int y, int r, int g, int b, int heat, int type, float conductivity);
};
