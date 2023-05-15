#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include <iostream>
#include "wire.hpp"

using namespace sf;

class Minion
{
    private:
        float s = 5;
        Vector2f initPos, finalPos, u, connectionPoint;
        float d = 0.f;
        int dir = 1;

        float dist(Vector2f a,Vector2f b){
            return sqrt(pow((a.x-b.x),2)+pow((a.y-b.y),2));
        }

    public:
        Minion(Clock* clock_, Texture* t,RenderWindow* app, std::vector<Vector2f> travellingPoints, Vector2f connectionPoint = Vector2f(0.0, 0.0));
        ~Minion();

        bool alive = true;
        Time deathTime;
        Clock* clocku;

        int radius = 35;
        CircleShape boundingBox;
        Sprite character;

        RenderWindow* app;

        Vector2f position;
        Wire connectingWire;
        std::vector<Vector2f> travellingPoints;
        int currentDestination = 1;
        void move(sf::Vector2f newPosition);

        void draw();
};
