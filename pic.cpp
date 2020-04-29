#include"pic.h"

#include<iostream>
#include<math.h>

// Implémentation de la méthode Particle in Cell pour la simulation

using namespace std;

struct Particle;
struct World;

/// Pseudo constructeur de particule
Particle create_particle(int x, int y, int r, int g, int b, int heat, int type)
{
    Particle p;
    p.x = x;
    p.y = y;
    p.r = r;
    p.g = b;
    p.b = b;
    p.type = type;
    p.heat = heat;
    return p;
}


/// Recyclage du TP2 Interpolation
int interpolate(int a, int b, float t)
{
    return round((1-t)*a + t*b);
}

int compute_heat(World& w, int i, float interpolation_factor, int DIMW)
{
    int x = w.particles[i].x;
    int y = w.particles[i].y;

    // Implémentation d'une version simplifiée d'échanges de chaleur
    int heat_sum = 0;
    int c = 0;
    for(int i = -1; i < 2; i++)
    {
        for(int j = -1; j < 2; j++)
        {
            if(x+i > 0 && x+i < DIMW && y+j > 0 && y+j < DIMW)
            {
                    if(w.indice[x+i][y+j] != -1 && w.indice[x+i][y+j] != 2)
                    {
                        heat_sum += w.heatmap[x+i][y+j];
                        c++; // Haha
                    }
            }
        }
    }

    if(c)
        return interpolate(w.heatmap[x][y], heat_sum/c, interpolation_factor);
    else
        return w.heatmap[x][y];
}

/// Gestion des particules basique

void update_water(World& w, int i, int DIMW)
{
    int x = w.particles[i].x;
    int y = w.particles[i].y;

    w.heatmap[x][y] = w.particles[i].heat;

    w.heatmap[x][y] = compute_heat(w, i, 0.07f, DIMW);
    w.particles[i].heat = w.heatmap[x][y];

    // Changement d'état de l'eau
    if(w.particles[i].heat <= 0)
    {
        w.particles[i].type = 4;
        w.indice[x][y] = 4;
        w.particles[i].r = 10;
        w.particles[i].g = 10;
        w.particles[i].b = 100;
    }
    else if(w.particles[i].heat >=   100)
    {
        w.particles[i].type = 3;
        w.indice[x][y] = 4;
        w.particles[i].r = 170;
        w.particles[i].g = 170;
        w.particles[i].b = 170;
    }

    if(w.indice[x][y] == -1)
        cout << "Error: Mismatched particle and grid index at x: " << x << " y: " << y << endl;
    if(w.heatmap[x][y] != w.particles[i].heat)
        cout << "Error: Mismatched particle and grid heat at x: " << x << " y: " << y << endl;

    if(y > 0 && x > 0 && y < DIMW && x < DIMW)
    {
        if(w.indice[x][y-1] == -1)
        {
            w.indice[x][y] = -1;
            w.indice[x][y-1] = 1;
            w.particles[i].y--;
            swap(w.heatmap[x][y], w.heatmap[x][y-1]);
        }
        else if(w.indice[x+1][y-1] == -1 && x < DIMW)
        {
            w.indice[x][y] = -1;
            w.indice[x+1][y-1] = 1;
            w.particles[i].x++;
            w.particles[i].y--;
            swap(w.heatmap[x][y], w.heatmap[x+1][y-1]);

        }
        else if(w.indice[x-1][y-1] == -1 && x > 0)
        {
            w.indice[x][y] = -1;
            w.indice[x-1][y-1] = 1;
            w.particles[i].x--;
            w.particles[i].y--;
            swap(w.heatmap[x][y], w.heatmap[x-1][y-1]);

        }
        else
        {
        // Une chance sur deux que l'eau ne bouge pas
        // Une chance sur quatre que l'eau bouge à gauche ou à droite
        // Cela lui donne une certaine viscosité
            if(w.indice[x-1][y] == -1 && w.indice[x+1][y] == -1)
            {
                if(rand() % 2)
                {
                w.indice[x][y] = -1;
                w.indice[x-1][y] = 1;
                w.particles[i].x--;
                swap(w.heatmap[x][y], w.heatmap[x-1][y]);

                }
                else
                {
                w.indice[x][y] = -1;
                w.indice[x+1][y] = 1;
                w.particles[i].x++;
                swap(w.heatmap[x][y], w.heatmap[x+1][y]);

                }
            }
            else if(w.indice[x-1][y] == -1)
            {
                w.indice[x][y] = -1;
                w.indice[x-1][y] = 1;
                w.particles[i].x--;
                swap(w.heatmap[x][y], w.heatmap[x-1][y]);

            }
            else if(w.indice[x+1][y] == -1)
            {

                w.indice[x][y] = -1;
                w.indice[x+1][y] = 1;
                w.particles[i].x++;
                swap(w.heatmap[x][y], w.heatmap[x+1][y]);

            }
        }
    }
}

void update_steam(World& w, int i, int DIMW)
{
    int x = w.particles[i].x;
    int y = w.particles[i].y;

    if(w.indice[x][y] == -1)
        cout << "Error: Mismatched particle and grid index at x: " << x << " y: " << y << endl;

    w.heatmap[x][y] = w.particles[i].heat;
    w.heatmap[x][y] = compute_heat(w, i, 0.1f, DIMW);
    w.particles[i].heat = w.heatmap[x][y];

    if(w.particles[i].heat < 100)
    {
        w.particles[i].type = 1;
        w.particles[i].r = 0;
        w.particles[i].g = 119;
        w.particles[i].b = 190;
        w.indice[x][y] = 1;
    }
    if(y > 0 && x > 0 && y < DIMW && x < DIMW)
    {
        if(w.indice[x][y+1] == -1)
        {
            w.indice[x][y] = -1;
            w.indice[x][y+1] = 3;
            w.particles[i].y++;
            swap(w.heatmap[x][y], w.heatmap[x][y+1]);
        }
        else if(w.indice[x+1][y+1] == -1 && x < DIMW)
        {
            w.indice[x][y] = -1;
            w.indice[x+1][y+1] = 3;
            w.particles[i].x++;
            w.particles[i].y++;
            swap(w.heatmap[x][y], w.heatmap[x+1][y+1]);
        }
        else if(w.indice[x-1][y+1] == -1 && x > 0)
        {
            w.indice[x][y] = -1;
            w.indice[x-1][y+1] = 3;
            w.particles[i].x--;
            w.particles[i].y++;
            swap(w.heatmap[x][y], w.heatmap[x-1][y+1]);

        }
        else
        {
            if(w.indice[x-1][y] == -1 && w.indice[x+1][y] == -1)
            {
                if(rand() % 2)
                {
                w.indice[x][y] = -1;
                w.indice[x-1][y] = 3;
                w.particles[i].x--;
                swap(w.heatmap[x][y], w.heatmap[x-1][y]);

                }
                else
                {
                w.indice[x][y] = -1;
                w.indice[x+1][y] = 3;
                w.particles[i].x++;
                swap(w.heatmap[x][y], w.heatmap[x+1][y]);

                }
            }
            else if(w.indice[x-1][y] == -1)
            {
                w.indice[x][y] = -1;
                w.indice[x-1][y] = 3;
                w.particles[i].x--;
                swap(w.heatmap[x][y], w.heatmap[x-1][y]);
            }
            else if(w.indice[x+1][y] == -1)
            {
                w.indice[x][y] = -1;
                w.indice[x+1][y] = 3;
                w.particles[i].x++;
                swap(w.heatmap[x][y], w.heatmap[x+1][y]);
            }
        }
    }
}

void update_ice(World& w, int i, int DIMW)
{
    int x = w.particles[i].x;
    int y = w.particles[i].y;

    w.heatmap[x][y] = w.particles[i].heat;
    w.heatmap[x][y] = compute_heat(w, i, 0.7f, DIMW);
    w.particles[i].heat = w.heatmap[x][y];

    if(w.particles[i].heat > 0)
    {
        w.particles[i].type = 1;
        w.indice[x][y] = 1;
        w.particles[i].r = 0;
        w.particles[i].g = 119;
        w.particles[i].b = 190;
    }

    if(w.indice[x][y] == -1)
        cout << "Error: Mismatched particle and grid index at x: " << x << " y: " << y << endl;
    if(w.heatmap[x][y] != w.particles[i].heat)
        cout << "Error: Mismatched particle and grid heat at x: " << x << " y: " << y << endl;

}

//TODO: Équations de fluides avancées
//void update_water(World& w, int i)
//{
//
//}
