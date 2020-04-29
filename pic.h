struct Particle
{
    int x, y, r, g, b, type, heat;
};

struct World
{
    int np;
    float last_updated;
    Particle particles[500000];
    int indice[500][500];
    int heatmap[500][500];

};

Particle create_particle(int x, int y, int r, int g, int b, int heat, int type);

void update_water(World& w, int i, int DIMW);
void update_steam(World& w, int i, int DIMW);
void update_ice(World& w, int i, int DIMW);
