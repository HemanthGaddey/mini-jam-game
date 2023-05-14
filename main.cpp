#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <time.h>
#include <string>
#include <sstream>
#include "MainMenu.hpp"
#include "portal.hpp"  //for my IDE portal.hpp dosent worl so i had toreplace it with .h, i've changed all of them for you're convinence.
using namespace sf;

Font font;

struct point{int x,y;};

float dist(Vector2f a,Vector2f b){
    return sqrt(pow((a.x-b.x),2)+pow((a.y-b.y),2));
}

float dot(Vector2f a, Vector2f b){
    return a.x*b.x + a.y*b.y;
}


sf::Vector2f rotateVector(const sf::Vector2f& vector, float angle)
{
    // This function will rotate input vector by angle in clockwise direction
    float theta = angle * (3.14159f / 180.0f); // Convert degrees to radians

    float rotatedX = vector.x * std::cos(theta) + vector.y * std::sin(theta);
    float rotatedY = -1 * vector.x * std::sin(theta) + vector.y * std::cos(theta);

    return sf::Vector2f(rotatedX, rotatedY);
}

sf::Vector2f movePlayer(const sf::Vector2f& initPos,
        const sf::Vector2f& movement,
        const std::vector<Portal>& portals,
        sf::Vector2f *grapplePos = NULL)
{
    sf::Vector2f finalPos = initPos + movement;
    float a1, b1, c1;
    a1 = movement.y / movement.x;
    b1 = -1;
    c1 = initPos.y - (a1 * initPos.x);
    float angle = std::atan(a1);
    for (auto it : portals)
    {
        // Check if player path intersects with portal

        // Reject if lines are parallel
        if(angle <= it.angle + 0.05 && angle >= it.angle - 0.05)
            continue;

        // Get point of intersection
        sf::Vector2f pointIntersection(((b1 * it.c - c1 * it.b) / (a1 * it.b - b1 * it.a)),
                ((c1 * it.a - a1 * it.c) / (a1 * it.b - b1 * it.a)));

        // Check if point of intersection lies on the line segments
        if(  (initPos.x < pointIntersection.x && finalPos.x < pointIntersection.x)
           ||(initPos.x > pointIntersection.x && finalPos.x > pointIntersection.x)
           ||(initPos.y < pointIntersection.y && finalPos.y < pointIntersection.y)
           ||(initPos.y > pointIntersection.y && finalPos.y > pointIntersection.y)
        )
            continue;
        if (dist(it.position, pointIntersection) > it.size * 0.5)
            continue;

        // If it does, teleport player
        if (grapplePos != NULL)
            *grapplePos = it.transform(*grapplePos);
        return it.transform(finalPos);
    }
    return finalPos;
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
    CircleShape player(25);
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

    // Portal sprite needs to be asymmetric for player to get idea of direction
    Portal::tPortal.loadFromFile("portal.png");
    Portal::sPortal.setTexture(Portal::tPortal);
    Portal::sPortal.setOrigin(sf::Vector2f(1850, 475));
    Portal::sPortal.setScale(sf::Vector2f(0.027, 0.027));

    std::vector<Portal> portals;
    portals.push_back(Portal(sf::Vector2f(250, 100), sf::Color::Red,   0, 300.0f));
    portals.push_back(Portal(sf::Vector2f(100, 350), sf::Color::Red, -90, 300.0f));
    portals.push_back(Portal(sf::Vector2f(350, 500), sf::Color::Red, 180, 300.0f));
    portals.push_back(Portal(sf::Vector2f(500, 250), sf::Color::Red,  90, 300.0f));
    portals[0].pair(&portals[3]);
    portals[1].pair(&portals[2]);
    portals[2].pair(&portals[1]);
    portals[3].pair(&portals[0]);



    MainMenu menu(app.getSize().x,app.getSize().y); //Main Menu features
    bool flag = false; //shows up menu when false and the game when set to true.

    while(app.isOpen()){
        mouse = Mouse::getPosition(app);
        Event e;

        while(app.pollEvent(e))
            {
            switch(e.type)
            {
            case sf::Event::KeyReleased:
            switch (e.key.code)
				{
                case sf::Event::Closed:
                app.close();

				case sf::Keyboard::W:
					menu.MoveUp();
					break;

				case sf::Keyboard::S:
					menu.MoveDown();
					break;

				case sf::Keyboard::Space:
					switch (menu.GetPressedItem())
					{
					case 0:
					    std::cout<<"play!!"<<"\n";
					    flag = true; //starts the game
					    break;
                    case 1:
                        std::cout<<"Option"<<"\n";
                        break;
                    case 2:
                        app.close();
                        break;
					}
                    break;
				}
				case Event::MouseButtonReleased:
                if(e.mouseButton.button == Mouse::Left || flag==true){
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

            break;
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

        // Deciding position of grapple
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

                // This is the only place where player is b
                sf::Vector2f playerMovement(g*grappleSpeed*cos(grappleAngle), g*grappleSpeed*sin(grappleAngle));
                playerpos = movePlayer(playerpos,
                        playerMovement,
                        portals,
                        &grapplepos);

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


        if(flag==false) //displays  menu
        {
            app.clear();
            menu.draw(app);
            app.display();
        }
        else
        {
        app.clear();
        //Start Drawing here
        app.draw(sBackground);
        if(grapple)app.draw(line, 5, Lines);
        app.draw(player);
        app.draw(hook);
        //debug(&app,grapplepos,initplayerpos);

        // Drawing Portals
        for(auto it : portals)
            it.draw(app);

        //Stop Drawing here
        app.display();
        }
    }
    return 0;
}
