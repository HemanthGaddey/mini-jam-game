#include "MainMenu.hpp"
#include "SFML/Graphics.hpp"


MainMenu::MainMenu(float width, float height)
{
	if (!font.loadFromFile("OpenSans-Light.ttf"))
	{
		// handle error
	}

	// Logo
	tLogo.loadFromFile("logo.png");
	sLogo.setTexture(tLogo);
	sf::Vector2<unsigned int> logoSize = tLogo.getSize();
	/* sLogo.setPosition(sf::Vector2f((width - logoSize.x) * 0.5f, 0.05*height)); */
	sLogo.setPosition(sf::Vector2f(150, 0.05*height));

	// Menu entries
	menu[0].setFont(font);
	menu[0].setFillColor(sf::Color::Red);
	menu[0].setString("Play");
	menu[0].setPosition(sf::Vector2f(width * 0.15, height * (0.35 + (0.11 * 0))));

	menu[1].setFont(font);
	menu[1].setFillColor(sf::Color::White);
	menu[1].setString("Options");
	menu[1].setPosition(sf::Vector2f(width * 0.15, height * (0.35 + (0.11 * 1))));

	menu[2].setFont(font);
	menu[2].setFillColor(sf::Color::White);
	menu[2].setString("Exit");
	menu[2].setPosition(sf::Vector2f(width * 0.15, height * (0.35 + (0.11 * 2))));

	marker.setFont(font);
	marker.setFillColor(sf::Color::Red);
	marker.setString("=>");
	marker.setPosition(sf::Vector2f(width * 0.05, height * (0.35 + (0.11 * 0))));

	selectedItemIndex = 0;
}


MainMenu::~MainMenu()
{
}

void MainMenu::draw(sf::RenderWindow &window)
{
	for (int i = 0; i < MAX_NUMBER_OF_ITEMS; i++)
	{
		window.draw(menu[i]);
	}
	window.draw(marker);
	window.draw(sLogo);
}

void MainMenu::MoveUp()
{
	if (selectedItemIndex - 1 >= 0)
	{
		menu[selectedItemIndex].setFillColor(sf::Color::White);
		selectedItemIndex--;
		menu[selectedItemIndex].setFillColor(sf::Color::Red);
		marker.setPosition(sf::Vector2f(marker.getPosition().x, menu[selectedItemIndex].getPosition().y));
	}
}

void MainMenu::MoveDown()
{
	if (selectedItemIndex + 1 < MAX_NUMBER_OF_ITEMS)
	{
		menu[selectedItemIndex].setFillColor(sf::Color::White);
		selectedItemIndex++;
		menu[selectedItemIndex].setFillColor(sf::Color::Red);
		marker.setPosition(sf::Vector2f(marker.getPosition().x, menu[selectedItemIndex].getPosition().y));
	}
}
