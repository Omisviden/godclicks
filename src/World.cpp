#include "Headers/World.hpp"
#include <string>
#include <iostream>
#include <math.h>
#include <stdlib.h>

using namespace std;

World::World() : grid_size(300), name("Untitled World")
{
    // Initialize both grids
    for(int i = 0; i < grid_size; i++)
    {
        for(int j = 0; j < grid_size; j++)
        {
            this->grid[i][j] = -1;
            this->heatmap[i][j] = -20;
        }
    }
}

void World::set_name(string name)
{
    this->name = name;
}

void World::set_grid_size(int grid_size)
{
    this->grid_size = grid_size;
}

int World::get_grid_size()
{
    return this->grid_size;
}

int World::get_grid_index(int x, int y)
{
    return this->grid[x][y];
}

void World::update_particles()
{
    int w_size = this->particles.size();
    for(int i = 0; i < w_size; i++)
    {
        switch(this->particles[i].type)
        {
        case 1:
            update_water(i);
            break;
        case 2:

            update_steam(i);
            break;
        case 3:
            update_ice(i);
            break;

        default:
            cout << "Error: Unkown particle of type "
                 << this->particles[i].type << " at index "
                 << i << endl;
        }
    }
}

void World::create_particle(int x, int y, int r, int g, int b, int heat, int type, float conductivity)
{
    Particle p;
    p.x = x;
    p.y = y;
    p.r = r;
    p.g = g;
    p.b = b;
    p.type = type;
    this->grid[x][y] = type;
    p.heat = heat;
    p.conductivity = conductivity;
    this->particles.push_back(p);
}

// Linear interpolation between a and b
int World::interpolate(int a, int b, float weight)
{
    return round((1-weight)*a + weight*b);
}

// Heat conduction simulation of particle at index i
int World::compute_heat(int i, float conductivity)
{
    Particle *p = &this->particles[i];
    int x = p->x;
    int y = p->y;
    int grid_size = this->grid_size;

    int heat_sum = 0;
    int c = 0;
    for(int i = -1; i < 2; i++)
    {
        for(int j = -1; j < 2; j++)
        {
            if(x+i > 0 && x+i < grid_size && y+j > 0 && y+j < grid_size)
            {
                if(this->grid[x+i][y+j] != -1 && this->grid[x+i][y+j] != 2)
                {
                    heat_sum += this->heatmap[x+i][y+j];
                    c++; // Haha
                }
            }
        }
    }

    if(c)
        return interpolate(this->heatmap[x][y], heat_sum/c, conductivity);
    else
        return this->heatmap[x][y];
}

void World::update_water(int i)
{
    Particle *p = &this->particles[i];
    int x = p->x;
    int y = p->y;
    int grid_size = this->grid_size;

    this->heatmap[x][y] = p->heat;

    this->heatmap[x][y] = compute_heat(i, 0.5f); //TODO: Update conductivity
    p->heat = this->heatmap[x][y];

    // Changement d'�tat de l'eau
    if(p->heat <= 0)
    {
        p->type = 4;
        this->grid[x][y] = 4;
        p->r = 10;
        p->g = 10;
        p->b = 100;
    }
    else if(p->heat >= 100)
    {
        p->type = 3;
        this->grid[x][y] = 4;

        p->r = 170;
        p->g = 170;
        p->b = 170;

    }

    if(this->grid[x][y] == -1)
        cout << "Error: Mismatched particle and grid index at x: " << x << " y: " << y << endl;
    if(this->heatmap[x][y] != p->heat)
        cout << "Error: Mismatched particle and grid heat at x: " << x << " y: " << y << endl;

    if(y > 1 && x > 1 && y < grid_size-1 && x < grid_size-1)
    {
        if(this->grid[x][y+1] == -1)
        {
            this->grid[x][y] = -1;
            this->grid[x][y+1] = 1;
            p->y++;
            swap(this->heatmap[x][y], this->heatmap[x][y+1]);
        }
        else if(this->grid[x+1][y+1] == -1 && x < grid_size-1)
        {
            this->grid[x][y] = -1;
            this->grid[x+1][y+1] = 1;
            p->x++;
            p->y++;
            swap(this->heatmap[x][y], this->heatmap[x+1][y+1]);

        }
        else if(this->grid[x-1][y+1] == -1 && x > 0)
        {
            this->grid[x][y] = -1;
            this->grid[x-1][y+1] = 1;
            p->x--;
            p->y++;
            swap(this->heatmap[x][y], this->heatmap[x-1][y+1]);
        }
        else
        {
                    if(this->grid[x-1][y] == -1 && this->grid[x+1][y] == -1)
        {
            if(rand() % 2)
            {
            this->grid[x][y] = -1;
            this->grid[x-1][y] = 1;
            p->x--;
            swap(this->heatmap[x][y], this->heatmap[x-1][y]);

            }
            else
            {
            this->grid[x][y] = -1;
            this->grid[x+1][y] = 1;
            p->x++;
            swap(this->heatmap[x][y], this->heatmap[x+1][y]);
            }
        }
        else if(this->grid[x-1][y] == -1)
        {
            this->grid[x][y] = -1;
            this->grid[x-1][y] = 1;
            p->x--;
            swap(this->heatmap[x][y], this->heatmap[x-1][y]);

        }
        else if(this->grid[x+1][y] == -1)
        {

            this->grid[x][y] = -1;
            this->grid[x+1][y] = 1;
            p->x++;
            swap(this->heatmap[x][y], this->heatmap[x+1][y]);
        }
        }
    }
    else if(x > 0 && y > 0 && y < grid_size && x < grid_size)
    {
        if(this->grid[x-1][y] == -1 && this->grid[x+1][y] == -1)
        {
            if(rand() % 2)
            {
            this->grid[x][y] = -1;
            this->grid[x-1][y] = 1;
            p->x--;
            swap(this->heatmap[x][y], this->heatmap[x-1][y]);

            }
            else
            {
            this->grid[x][y] = -1;
            this->grid[x+1][y] = 1;
            p->x++;
            swap(this->heatmap[x][y], this->heatmap[x+1][y]);
            }
        }
        else if(this->grid[x-1][y] == -1)
        {
            this->grid[x][y] = -1;
            this->grid[x-1][y] = 1;
            p->x--;
            swap(this->heatmap[x][y], this->heatmap[x-1][y]);

        }
        else if(this->grid[x+1][y] == -1)
        {

            this->grid[x][y] = -1;
            this->grid[x+1][y] = 1;
            p->x++;
            swap(this->heatmap[x][y], this->heatmap[x+1][y]);
        }
    }
}
void World::update_steam(int i)
{
    Particle *p = &this->particles[i];
    int x = p->x;
    int y = p->y;
    int grid_size = this->grid_size;

    if(this->grid[x][y] == -1)
        cout << "Error: Mismatched particle and grid index at x: " << x << " y: " << y << endl;

    this->heatmap[x][y] = p->heat;
    this->heatmap[x][y] = compute_heat(i, 0.1f);
    p->heat = this->heatmap[x][y];

    if(p->heat < 100)
    {
        p->type = 1;
        p->r = 0;
        p->g = 119;
        p->b = 190;
        this->grid[x][y] = 1;
    }
    if(y > 0 && x > 0 && y < grid_size && x < grid_size)
    {
        if(this->grid[x][y+1] == -1)
        {
            this->grid[x][y] = -1;
            this->grid[x][y+1] = 3;
            p->y++;
            swap(this->heatmap[x][y], this->heatmap[x][y+1]);
        }
        else if(this->grid[x+1][y+1] == -1 && x < grid_size)
        {
            this->grid[x][y] = -1;
            this->grid[x+1][y+1] = 3;
            p->x++;
            p->y++;
            swap(this->heatmap[x][y], this->heatmap[x+1][y+1]);
        }
        else if(this->grid[x-1][y+1] == -1 && x > 0)
        {
            this->grid[x][y] = -1;
            this->grid[x-1][y+1] = 3;
            p->x--;
            p->y++;
            swap(this->heatmap[x][y], this->heatmap[x-1][y+1]);

        }
        else
        {
            if(this->grid[x-1][y] == -1 && this->grid[x+1][y] == -1)
            {
                if(rand() % 2)
                {
                this->grid[x][y] = -1;
                this->grid[x-1][y] = 3;
                p->x--;
                swap(this->heatmap[x][y], this->heatmap[x-1][y]);

                }
                else
                {
                this->grid[x][y] = -1;
                this->grid[x+1][y] = 3;
                p->x++;
                swap(this->heatmap[x][y], this->heatmap[x+1][y]);

                }
            }
            else if(this->grid[x-1][y] == -1)
            {
                this->grid[x][y] = -1;
                this->grid[x-1][y] = 3;
                p->x--;
                swap(this->heatmap[x][y], this->heatmap[x-1][y]);
            }
            else if(this->grid[x+1][y] == -1)
            {
                this->grid[x][y] = -1;
                this->grid[x+1][y] = 3;
                p->x++;
                swap(this->heatmap[x][y], this->heatmap[x+1][y]);
            }
        }
    }
}

void World::update_ice(int i)
{
    Particle *p = &this->particles[i];
    int x = p->x;
    int y = p->y;

    this->heatmap[x][y] = p->heat;
    this->heatmap[x][y] = compute_heat(i, 0.7f); //TODO: Update conductivity
    p->heat = this->heatmap[x][y];

    if(p->heat > 0)
    {
        p->type = 1;
        this->grid[x][y] = 1;
        p->r = 0;
        p->g = 119;
        p->b = 190;
    }

    if(this->grid[x][y] == -1)
        cout << "Error: Mismatched particle and grid index at x: " << x << " y: " << y << endl;
    if(this->heatmap[x][y] != p->heat)
        cout << "Error: Mismatched particle and grid heat at x: " << x << " y: " << y << endl;

}
