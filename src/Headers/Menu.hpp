#pragma once

#include <vector>

using namespace sf;

class Menu
{
public:
    Menu();
	~Menu();
    void set_name(std::string name);
	void set_selected(int id);
private:

    struct MenuItem
    {
        int id;
        Image icon;
    };

    std::string name;
public:
	std::vector<MenuItem> list;
    void add_menu_item(int id, Image icon);
};
