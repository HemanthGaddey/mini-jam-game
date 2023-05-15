#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

class Spike{
    public:
    Spike(RenderWindow*, float, float, int);
    ~Spike();

    void draw();
    Texture texture;
    Sprite sprite;
    Vector2f position;
    RenderWindow* app;
};