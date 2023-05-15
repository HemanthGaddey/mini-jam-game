#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <cmath>
#include "portal.hpp"

#define DEBUG  0

#if DEBUG
#include <iostream>
#endif

sf::Vector2f rotateVector(sf::Vector2f& vector, float angle);

void Portal::draw(sf::RenderWindow& app)
{
    /* sf::Vector2f offset = sf::Vector2f(size * 0.5 * std::cos(angle), size * 0.5f * std::sin(angle)); */
    /* sf::Vertex line[] ={ */
    /*     sf::Vertex(position + offset), */
    /*     sf::Vertex(position + offset) */
    /* }; */
    /* app.draw(line, 2, sf::Lines); */

    sPortal.setPosition(position);
    sPortal.setRotation(angle);
    sPortal.setScale(sf::Vector2f(size/3700, size/3700));
    app.draw(sPortal);
}

Portal::Portal(sf::Vector2f _position, sf::Color _color, float _angle, float _size)
: position(_position), portalColor(_color), angle(_angle), size(_size)
{
    a = tan(angle);
    b = -1;
    c = position.y - a * position.x;
}

void Portal::pair(Portal *_otherSide)
{
    otherSide = _otherSide;
#if DEBUG
    std::cout << "Portal at (" << position.x <<  ", " << position.y << 
        ") is paired with poratal at (" << _otherSide->position.x << ", " << _otherSide->position.y << ")" << std::endl;
#endif
}

sf::Vector2f Portal::transform(sf::Vector2f initPos)
{
    sf::Vector2f displacement = initPos - position;
    sf::Vector2f rotatedDisplacement = rotateVector(displacement, angle * -1);

    return rotateVector(rotatedDisplacement, otherSide->angle) + otherSide->position;
}
