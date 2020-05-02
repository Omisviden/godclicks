#pragma once

#include <string>
#include <vector>

class Menu
{
public:
    Menu();
    ~Menu();
private:

    struct Menu_item
    {
        int i;
        float conductivity;
    };

    std::string name;
    int grid[G_INIT_SIZE][G_INIT_SIZE];
    int heatmap[G_INIT_SIZE][G_INIT_SIZE];
    int grid_size;

    int interpolate(int a, int b, float weight);
    int compute_heat(int i, float conductivity);
    void update_water(int i);
    void update_steam(int i);
    void update_ice(int i);
public:
    std::vector<Menu_item> items; // push_back == append
    void add_menu_item();

};
