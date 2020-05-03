#include "Headers/Application.hpp"

using namespace sf;
using namespace std;

int main()
{
	Application app;
	World w;
	w.set_name("Test");
	app.loop(w);
	return 0;
}
