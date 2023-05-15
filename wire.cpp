#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <cmath>
#include "wire.hpp"

#define DEBUG  1

#if DEBUG
#include <iostream>
#endif

float dist(sf::Vector2f a, sf::Vector2f b);

/* Wire::Wire(Wire& wire){ */
/* #if DEBUG */
/*         std::cout << "Copying wire"; */
/* #endif */

/* } */

/* Wire::Wire(sf::Vector2f _position, float _angle, float _size) */
/* : position(_position), angle(_angle), size(_size) */
/* { */
    
/* #if DEBUG */
/*         std::cout << "Creating new wire\n"; */
/* #endif */
/*     a = tan(angle); */
/*     b = -1; */
/*     c = position.y - a * position.x; */
/* } */

Wire::Wire(sf::Vector2f _point1, sf::Vector2f _point2)
{
    updateWire(_point1, _point2);
}

bool Wire::breakWire()
{
    if(broken){
        return false;
    }
    else
    {
#if DEBUG
        std::cout << "Breaking wire \n";
#endif
        broken = true;
        return true;
    }
}

void Wire::updateWire(sf::Vector2f _point1, sf::Vector2f _point2)
{
    point1 = _point1;
    point2 = _point2;
    position = (point1 + point2)*0.5f;
    size = dist(point1, point2);
    a = (point1.y - point2.y)/(point1.x - point1.y);
    angle = atan(a);
    b = -1;
    c = position.y - a * position.x;
}

void Wire::draw(sf::RenderWindow& app)
{
    if(!broken){
        sf::Vertex line[] ={
            sf::Vertex(point1, sf::Color::Black),
            sf::Vertex(point2, sf::Color::Black)
        };
        app.draw(line, 2, sf::Lines);
    }
}
