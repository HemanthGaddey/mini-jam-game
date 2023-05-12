#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <time.h>
#include <string>
#include <sstream>

#include "portal.hpp"
using namespace sf;

Font font;

struct point{int x,y;};

float dist(Vector2f a,Vector2f b){
    return sqrt(pow((a.x-b.x),2)+pow((a.y-b.y),2));
}

float dot(Vector2f a, Vector2f b){
    return a.x*b.x + a.y*b.y;
}

void debug(RenderWindow* app, Vector2f a, Vector2f b){
    Text text;
    text.setFont(font);
    text.setCharacterSize(34);
    text.setFillColor(Color::White);
    text.setString(std::to_string(a.x)+" "+std::to_string(a.y));
    text.setPosition(100,100);
    (*app).draw(text);
    text.setString(std::to_string(b.x)+" "+std::to_string(b.y));
    text.setPosition(100,150);
    (*app).draw(text);
    text.setString(std::to_string(dot(a,b)));
    text.setPosition(100,200);
    (*app).draw(text);
}

sf::Texture Portal::tPortal;
sf::Sprite Portal::sPortal;

int main()
{
    srand(time(0));

    Clock clock;
    Time shakeStart = clock.getElapsedTime();
    int magnitude = 10;

    RenderWindow app(VideoMode(600,600),"Doodle Game!!");
    app.setFramerateLimit(60);
    Vector2f camPos(300,300);
    Vector2f camCoverage(600,600);
    View cam(camPos,camCoverage);

    if(!font.loadFromFile("OpenSans-Light.ttf"))
        std::cerr<<"Failed to load Fonts!"<<std::endl;
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
    Vector2f grapplepos;
    Vector2f initplayerpos;
    int g=1;
    float grappleAngle = 0;
    float grappleSpeed = 0;
    float grappleAcc = 0;

    Texture tBackground;
    tBackground.loadFromFile("background.jpg");
    Sprite sBackground;
    sBackground.setTexture(tBackground);
    //sBackground.setOrigin(tBackground.getSize().x/2,tBackground.getSize().y/2);

    Portal::tPortal.loadFromFile("portal.png");
    Portal::sPortal.setTexture(Portal::tPortal);
    Portal::sPortal.setOrigin(sf::Vector2f(1850, 475));
    Portal::sPortal.setScale(sf::Vector2f(0.027, 0.027));

    Portal portal1(sf::Vector2f(500, 300), sf::Color::Red, 80, 300.0f);

    while(app.isOpen()){
        mouse = Mouse::getPosition(app);
        Event e;
        while(app.pollEvent(e)){
            if(e.type == Event::Closed){
                app.close();
            }
            else if(e.type == Event::MouseButtonReleased){
                if(e.mouseButton.button == Mouse::Left){
                    if(dist(Vector2f(mouse.x,mouse.y),playerpos) > player.getRadius()){
                        grapple = true;
                        grapplepos.x = mouse.x; grapplepos.y = mouse.y;
                        initplayerpos = playerpos;
                        grappleAngle = atan((mouse.y-playerpos.y)/(mouse.x-playerpos.x));
                        grappleSpeed = 12;
                        grappleAcc = 5;
                        (mouse.x > playerpos.x)? g=1:g=-1;
                        magnitude = 1;
                        shakeStart = clock.getElapsedTime();
                    }
                }
            }
        }
        if(Keyboard::isKeyPressed(Keyboard::Space)){
            //CAM SHAKE
            magnitude = 10;
            shakeStart = clock.getElapsedTime();
        }
        if(Keyboard::isKeyPressed(Keyboard::Right));
        if(Keyboard::isKeyPressed(Keyboard::Q))app.close();


        if((clock.getElapsedTime()-shakeStart) <= milliseconds(70)){
            cam.setCenter(camPos.x+magnitude*((float)rand()/RAND_MAX),camPos.y+magnitude*((float)rand()/RAND_MAX));
            magnitude *= 0.5;
        }
        else
            cam.setCenter(camPos);

        float angle = (atan((mouse.y-playerpos.y)/(mouse.x-playerpos.x)));
        (mouse.x > playerpos.x)? t=1:t=-1;
        hookpos.x = playerpos.x+t*(player.getRadius()+hook.getRadius()+1)*cos(angle);
        hookpos.y = playerpos.y+t*(player.getRadius()+hook.getRadius()+1)*sin(angle);

        if(grapple){
            Vector2f a(grapplepos.x-playerpos.x,grapplepos.y-playerpos.y);
            Vector2f b(playerpos.x-initplayerpos.x,playerpos.y-initplayerpos.y);
            if(dot(a,b)<0){
                playerpos = grapplepos;
                grapple=false;
                magnitude = 10;
                shakeStart = clock.getElapsedTime();
            }
            else{
                hookpos.x = grapplepos.x;
                hookpos.y = grapplepos.y;

                playerpos.x += g*grappleSpeed*cos(grappleAngle);
                playerpos.y += g*grappleSpeed*sin(grappleAngle);

                grappleSpeed += grappleAcc;
            }
        }

        player.setPosition(playerpos.x-player.getRadius(),playerpos.y-player.getRadius());
        hook.setPosition(hookpos.x-hook.getRadius(), hookpos.y-hook.getRadius());
        sf::Vertex line[] ={
            sf::Vertex(sf::Vector2f(playerpos.x, playerpos.y)),
            sf::Vertex(sf::Vector2f(hookpos.x, hookpos.y))
        };
        app.setView(cam);
        app.clear();

        //Start Drawing here
        app.draw(sBackground);
        if(grapple)app.draw(line, 5, Lines);
        app.draw(player);
        app.draw(hook);
        //debug(&app,grapplepos,initplayerpos);

        // Drawing Portals
        portal1.draw(app);

        //Stop Drawing here
        app.display();
    }
    return 0;
}
