#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

class Level
{
    public:
        Level(RenderWindow* app, CircleShape* player);
        ~Level();

        void draw(int level);
    private:
        Vector2f levelRect[6] = {Vector2f(600,600),Vector2f(600,600),Vector2f(600,800),Vector2f(800,900),Vector2f(800,1000),Vector2f(800,900)};
};