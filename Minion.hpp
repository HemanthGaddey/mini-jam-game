#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include <iostream>

using namespace sf;

class Minion
{
    private:
        float s = 5;

        Vector2f initPos, finalPos, u;
        float d = 0.f;
        int dir = 1;

        float dist(Vector2f a,Vector2f b){
            return sqrt(pow((a.x-b.x),2)+pow((a.y-b.y),2));
        }

    public:
        Minion(RenderWindow* app, std::vector<Vector2f> travellingPoints);
        ~Minion();

        CircleShape character;

        RenderWindow* app;

        Vector2f position;
        std::vector<Vector2f> travellingPoints;
        int currentDestination = 1;

        void draw();
};