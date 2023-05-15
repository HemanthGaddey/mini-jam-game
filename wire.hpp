#ifndef WIRE
#define WIRE

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>

class Wire{
    public:
        sf::Vector2f position;
        sf::Vector2f point1;
        sf::Vector2f point2;
        float size;
        float angle; // angle in degrees from positive x axis in clockwise direction (y axis is fliped)
        bool broken = false;

        float a, b, c;
        /* sf::Vector2f offset; */

    public:
        void draw(sf::RenderWindow& app);
        Wire(sf::Vector2f _position, float _angle = 0, float _size = 100.0f);
        Wire(sf::Vector2f point1, sf::Vector2f point2);
        void updateWire(sf::Vector2f point1, sf::Vector2f point2);
        bool breakWire();
};

#endif
