#include "Headers/Application.hpp"

using namespace std;

int main()
{
	Application app;
	World w;

	w.set_grid_size(app.DIMW / 2);
	w.set_name("Test");

	app.loop(w);

	return 0;
}
