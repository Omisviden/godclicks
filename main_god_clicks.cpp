#include <Grapic.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

#include"pic.h"

struct Particle;
struct Vec;
struct World;


// ******************************
// *                            *
// *         GOD CLICKS         *
// *       Simulation de        *
// *       fluides et de        *
// *    conduction thermique    *
// *     par Thibaut Fayard     *
// *                            *
// ******************************


// Couleur du sable: 239, 221, 111
// Couleur de l'eau: 0, 119, 190

// Mémo types de particule:
// 1: eau; 2: roche; 3: vapeur; 4: glace; 5: point chaud; 6: point froid:

using namespace grapic;
using namespace std;

const int DIMW = 500;
const float TIME_STEP = 0;
const int FPS = 500;
const double FRAME_DELAY = 1000000 / FPS;

void init(World &w)
{
    for(int i = 0; i < DIMW; i++)
    {
        for(int j = 0; j < DIMW; j++)
        {
            w.indice[i][j] = -1;
            w.heatmap[i][j] = 0;
        }
    }
    w.np = 0;
    w.last_updated = elapsedTime();
}

void draw(const World& w)
{
    for(int i = 0; i < w.np; i++)
    {
        put_pixel(w.particles[i].x, w.particles[i].y, w.particles[i].r, w.particles[i].g, w.particles[i].b);
    }
}

void update_pixel(World& w, int i)
{
    switch(w.particles[i].type)
    {
    case 1:
         update_water(w, i, DIMW);
        break;
    case 2:
        break;
    case 3:
        update_steam(w, i, DIMW);
        break;
    case 4:
        update_ice(w, i, DIMW);
        break;
    case 5:
        break;
    case 6:
        break;
    default:
        cout << "Error: Unknown type '" << w.particles[i].type << "' at index " << i << endl;
    }
}

/// Gestion des inputs et de la création de particule
void update(World& w, int selected_menu_item, int brush_size)
{

    brush_size = 2 + brush_size*2; // 0 -> 1; 1 -> 3; 2 -> 5 ; 3 -> 7;

    int x,y;
    mousePos(x, y);

    if(isKeyPressed(SDLK_r))
    {
        init(w);
    }
    if(isMousePressed(SDL_BUTTON_LEFT))
    {
        if(x < DIMW && y < DIMW && w.indice[x][y] == -1)
        {
            switch(selected_menu_item)
            {
            case 1: // Eau
                for(int i = -brush_size/2; i < brush_size/2; i++)
                {
                    for(int j = -brush_size/2; j < brush_size/2; j++)
                    {
                        if(x+i < DIMW && y+i < DIMW && w.indice[x+i][y+j] == -1)
                        {
                            w.particles[w.np] = create_particle(x+i, y+j, 0, 119, 190, 30, 1);
                            w.indice[x+i][y+j] = 1;
                            w.heatmap[x+i][y+j] = 30;
                            w.np++;
                        }
                    }
                }
                break;
            case 2: // Roche
                for(int i = 0; i < brush_size; i++)
                {
                    for(int j = 0; j < brush_size; j++)
                    {
                        if(x+i < DIMW && y+i < DIMW && w.indice[x+i][y+j] == -1)
                        {
                            w.particles[w.np] = create_particle(x+i, y+j, 75, 75, 71, 15, 2);
                            w.indice[x+i][y+j] = 2;
                            w.heatmap[x+i][y+j] = 15;
                            w.np++;
                        }
                    }
                }
                break;
            case 3: // Vapeur
                for(int i = 0; i < brush_size; i++)
                {
                    for(int j = 0; j < brush_size; j++)
                    {
                        if(x+i < DIMW && y+i < DIMW && w.indice[x+i][y+j] == -1)
                        {
                            w.particles[w.np] = create_particle(x+i, y+j, 170, 170, 170, 120, 3);
                            w.indice[x+i][y+j] = 3;
                            w.heatmap[x+i][y+j] = 120;
                            w.np++;
                        }
                    }
                }
                break;
            case 4: // Glace
                for(int i = 0; i < brush_size; i++)
                {
                    for(int j = 0; j < brush_size; j++)
                    {
                        if(x+i < DIMW && y+i < DIMW && w.indice[x+i][y+j] == -1)
                        {
                            w.particles[w.np] = create_particle(x+i, y+j, 10, 10, 100, -10, 4);
                            w.indice[x+i][y+j] = 4;
                            w.heatmap[x+i][y+j] = -10;
                            w.np++;
                        }
                    }
                }
                break;
            case 5: // Point de chaleur
                for(int i = 0; i < brush_size; i++)
                {
                    for(int j = 0; j < brush_size; j++)
                    {
                        if(x+i < DIMW && y+i < DIMW && w.indice[x+i][y+j] == -1)
                        {
                            w.particles[w.np] = create_particle(x+i, y+j, 240, 10, 10, 400, 5);
                            w.indice[x+i][y+j] = 5;
                            w.heatmap[x+i][y+j] = 400;
                            w.np++;
                        }
                    }
                }
                break;
            case 6: // Point de froid
                for(int i = 0; i < brush_size; i++)
                {
                    for(int j = 0; j < brush_size; j++)
                    {
                        if(x+i < DIMW && y+i < DIMW && w.indice[x+i][y+j] == -1)
                        {
                            w.particles[w.np] = create_particle(x+i, y+j, 10, 10, 240, -50, 6);
                            w.indice[x+i][y+j] = 6;
                            w.heatmap[x+i][y+j] = -50;
                            w.np++;
                        }
                    }
                }
                break;
            }
        }
    }

    if(elapsedTime() > w.last_updated + TIME_STEP)
    {
        for(int i = 0; i < w.np; i++)
        {
            update_pixel(w, i);
        }
        w.last_updated = elapsedTime();
    }

}


int main(int, char**)
{
    winInit("God Clicks", DIMW, DIMW);

    World* pw = new World();
    World & w = *pw;
    init(w);
    backgroundColor(40, 40, 40);

    Menu elements;
    menu_add( elements, "Eau");
    menu_add( elements, "Roche");
    menu_add( elements, "Vapeur");
    menu_add( elements, "Glace");
    menu_add( elements, "Point chaud");
    menu_add( elements, "Point froid");

    Menu brush_size;
    menu_add( brush_size, "1x1");
    menu_add( brush_size, "3x3");
    menu_add( brush_size, "5x5");
    menu_add( brush_size, "7x7");

    bool stop = false;
    double frame_start;
    int frame_time;

	while(!stop)
    {
        // On stocke le temps en microsecondes pour usleep()
        frame_start = elapsedTime() * 1000000;

        winClear();
        update(w, menu_select(elements)+1, menu_select(brush_size));
        draw(w);
        menu_draw(elements, 5, 5, 100, 102);
        menu_draw(brush_size, 5, 105, 105, 202);

        frame_time = elapsedTime() - frame_start;
        print(DIMW-10, 10, frame_time);
        if(frame_time < FRAME_DELAY)
        {
            usleep(FRAME_DELAY - frame_time);
        }
        stop = winDisplay();
    }
    return 0;
}
