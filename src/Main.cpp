#include "Headers/Application.hpp"

using namespace std;

int main()
{
	// Application handles SFML interactions such as event handling, rendering to the window and layering
	Application app;

	// World handles simulation logic
	World w;

	w.set_grid_size(app.DIMW / 4);
	w.set_name("Test");

	// The main loop is contained in App for better control (and perhaps enabling multiple simulations to run at once in the future)
	app.loop(w);

	return 0;
}
