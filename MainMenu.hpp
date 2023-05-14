#pragma once
#include "SFML/Graphics.hpp"
#include <SFML/Graphics/Sprite.hpp>

#define MAX_NUMBER_OF_ITEMS 3

class MainMenu
{
public:
	MainMenu(float width, float height);
	~MainMenu();

	void draw(sf::RenderWindow &window);
	void MoveUp();
	void MoveDown();
	int GetPressedItem() { return selectedItemIndex; }

private:
	int selectedItemIndex;
	sf::Font font;
	sf::Text menu[MAX_NUMBER_OF_ITEMS];
	sf::Text marker;
	sf::Texture tLogo;
	sf::Sprite sLogo;
};
