#ifndef PORTAL
#define PORTAL

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>

class Portal
{
    public:
        sf::Color portalColor;
        sf::Vector2f position;
        float size;
        float angle; // angle in degrees from positive x axis in clockwise direction (y axis is fliped)
        Portal* otherSide;

        float a, b, c;
        /* sf::Vector2f offset; */

        static sf::Texture tPortal;
        static sf::Sprite sPortal;

    public:
        void draw(sf::RenderWindow& app);
        Portal(sf::Vector2f _position, sf::Color _color, float _angle = 0, float _size = 100.0f);
        void pair(Portal* otherSide);
        sf::Vector2f transform(sf::Vector2f initPos);
};

#endif
