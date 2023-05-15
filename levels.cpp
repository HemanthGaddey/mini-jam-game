#include "levels.hpp"

Level::Level(RenderWindow* app, CircleShape* player)
{

}

Level::~Level()
{
}

Level::draw(int level){
    switch(level){
        case 1:
           RectangleShape lvl1rect(levelRect[0]);
           lvl1rect.setPosition(300,300);
           (*app).draw(lvl1rect);
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
    }
}