//COmmand to run: g++ -c doodlejump.cpp && g++ doodlejump.o -o sfml-app -lsfml-graphics -lsfml-window -lsfml-system && ./sfml-app
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <time.h>
using namespace sf;

struct point{int x,y;};

int main()
{
    srand(time(0));

    RenderWindow app(VideoMode(600,600),"Doodle Game!!");
    app.setFramerateLimit(60);

    Vector2i mouse;

    Vector2f playerpos(300,300);
    CircleShape player(50);
    player.setPointCount(300);
    player.setFillColor(Color::Cyan);

    int t=1;
    Vector2f hookpos(300+50+10,300);
    CircleShape hook(10);
    hook.setPointCount(7);
    hook.setFillColor(Color::White);


    bool grapple = false;
    Vector2i grapplepos;
    float grappleSpeedX = 4,grappleSpeedY = 4;
    float grappleSpeeddX = 0.01,grappleSpeeddY = 0.01;
    while(app.isOpen()){
        mouse = Mouse::getPosition(app);
        Event e;
        while(app.pollEvent(e)){
            if(e.type == Event::Closed){
                app.close();
            }
            else if(e.type == Event::MouseButtonReleased){
                if(e.mouseButton.button == Mouse::Left){
                    grapple = true;
                    grapplepos.x = mouse.x;
                    grapplepos.y = mouse.y;
                }
            }
        }
        if(Keyboard::isKeyPressed(Keyboard::Left));
        if(Keyboard::isKeyPressed(Keyboard::Right));
        if(Keyboard::isKeyPressed(Keyboard::Q))app.close();
       
        
        player.setPosition(playerpos.x-player.getRadius(),playerpos.y-player.getRadius());
        float angle = (atan((mouse.y-playerpos.y)/(mouse.x-playerpos.x)));
        (mouse.x > playerpos.x)? t=1:t=-1;
        hookpos.x = playerpos.x+t*(player.getRadius()+hook.getRadius()+1)*cos(angle);
        hookpos.y = playerpos.y+t*(player.getRadius()+hook.getRadius()+1)*sin(angle);
        hook.setPosition(hookpos.x-hook.getRadius(), hookpos.y-hook.getRadius());

        if(grapple){

        }

        app.clear();
        //Start Drawing here

        app.draw(player);
        app.draw(hook);

        //Stop Drawing here
        app.display();
    }
    return 0;
}