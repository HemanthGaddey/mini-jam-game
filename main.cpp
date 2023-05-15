#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <cmath>
#include <time.h>
#include <string>
#include <sstream>
#include "MainMenu.hpp"
#include "portal.hpp"  //for my IDE portal.hpp dosent worl so i had toreplace it with .h, i've changed all of them for you're convinence.
#include "wire.hpp"
#include "Minion.hpp"
#include "map.hpp"
#define PI 3.14159265

using namespace sf;

Music mainMenuTrack, gameplayTrack, hitSound, grappleSound;

Vector2f initOffset(100,100);
Vector2f levelRect[6] = {Vector2f(600,600),Vector2f(600,600),Vector2f(600,800),Vector2f(800,900),Vector2f(800,1000),Vector2f(800,900)};
Vector2f spawnPoint[6] = {Vector2f(300,300),Vector2f(600,600),Vector2f(600,800),Vector2f(800,900),Vector2f(800,1000),Vector2f(800,900)};
int current_level = 0;

Font font;
const int MAX_VIEW_SIZE = 800;
const int MIN_VIEW_SIZE = 200;

int score = 0;

typedef enum {
    STATE_MAIN_MENU,
    STATE_PLAYING,
    STATE_PAUSE,
    STATE_DEAD,
    STATE_WON
}GameState;

float dist(Vector2f a,Vector2f b){
    return sqrt(pow((a.x-b.x),2)+pow((a.y-b.y),2));
}

float dot(Vector2f a, Vector2f b){
    return a.x*b.x + a.y*b.y;
}

bool circleRectCollision(float cx, float cy, float radius, float rx, float ry, float rw, float rh) {
  // temporary variables to set edges for testing
  float testX = cx;
  float testY = cy;

  // which edge is closest?
  if (cx < rx)         
    testX = rx;     
  else if (cx > rx+rw) 
    testX = rx+rw; 
  if (cy < ry)         
    testY = ry;     
  else if (cy > ry+rh) 
    testY = ry+rh;   

  // get distance from closest edges
  float distX = cx-testX;
  float distY = cy-testY;
  float distance = sqrt( (distX*distX) + (distY*distY) );

  // if the distance is less than the radius, collision:
  if (distance <= radius) {
    return true;
  }
  return false;
}

sf::Vector2f rotateVector(const sf::Vector2f& vector, float angle)
{
    // This function will rotate input vector by angle in clockwise direction
    float theta = angle * (3.14159f / 180.0f); // Convert degrees to radians

    float rotatedX = vector.x * std::cos(theta) + vector.y * std::sin(theta);
    float rotatedY = -1 * vector.x * std::sin(theta) + vector.y * std::cos(theta);

    return sf::Vector2f(rotatedX, rotatedY);
}

sf::Vector2f movePlayer(Clock* clocku_,
        const sf::Vector2f& initPos,
        const sf::Vector2f& movement,
        const std::vector<Portal>& portals,
        std::vector<Minion>& minions,
        sf::Vector2f *grapplePos = NULL)
{
    sf::Vector2f finalPos = initPos + movement;
    float a1, b1, c1;
    a1 = movement.y / movement.x;
    b1 = -1;
    c1 = initPos.y - (a1 * initPos.x);
    float angle = std::atan(a1);
    for (int i = 0; i < minions.size(); i++)
    {
        // Check if player path intersects with portal

        // Reject if lines are parallel and broken wires
        if(minions[i].connectingWire.broken || (angle <= minions[i].connectingWire.angle + 0.05 && angle >= minions[i].connectingWire.angle - 0.05))
            continue;

        // Get point of intersection
        sf::Vector2f pointIntersection(((b1 * minions[i].connectingWire.c - c1 * minions[i].connectingWire.b) / (a1 * minions[i].connectingWire.b - b1 * minions[i].connectingWire.a)),
                ((c1 * minions[i].connectingWire.a - a1 * minions[i].connectingWire.c) / (a1 * minions[i].connectingWire.b - b1 * minions[i].connectingWire.a)));

        // Check if point of intersection lies on the line segments
        if(  (initPos.x < pointIntersection.x && finalPos.x < pointIntersection.x)
           ||(initPos.x > pointIntersection.x && finalPos.x > pointIntersection.x)
           ||(initPos.y < pointIntersection.y && finalPos.y < pointIntersection.y)
           ||(initPos.y > pointIntersection.y && finalPos.y > pointIntersection.y)
        )
            continue;
        if (dist(minions[i].connectingWire.position, pointIntersection) > minions[i].connectingWire.size * 0.5)
            continue;

        // If it does, break the wire
        score++;
        minions[i].connectingWire.breakWire();
        minions[i].alive = false;
        minions[i].deathTime = clocku_->getElapsedTime();
    }
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
    Vector2f actualMousePos;

    if(!mainMenuTrack.openFromFile("img/music1.ogg"));
        std::cerr<<"Error music1.ogg not loaded!!\n";
    if(!gameplayTrack.openFromFile("img/music2.ogg"));
        std::cerr<<"Error music2.ogg not loaded!!\n";
    if(!grappleSound.openFromFile("img/Hookthrown.ogg"));
        std::cerr<<"Error Hookthrown.ogg not loaded!!\n";
    if(!hitSound.openFromFile("img/hitHurt.ogg"));
        std::cerr<<"Error hitHurt.ogg not loaded!!\n";
    mainMenuTrack.setLoop(true);
    gameplayTrack.setLoop(true);

    int pradius = 40;
    CircleShape playerBoundingBox(pradius);
    playerBoundingBox.setPointCount(300);
    Vector2f playerpos(300,300);
    Texture tplayer,tcharacter;
    if(!tcharacter.loadFromFile("img/Extras.png")){std::cerr<<"FAILED to load extras.png!!!\n";}
    if(!tplayer.loadFromFile("img/player_robot.png"))
        std::cerr<<"ERROR player_robot.png not loaded!!\n";
    tplayer.setSmooth(true);
    Sprite player(tplayer,sf::IntRect(9,30,291,341));
    player.setScale(0.3f,0.3f);
    player.setOrigin(player.getLocalBounds().width/2.f,player.getLocalBounds().height/2.f);
    int PlayerRadius = 50;
    // CircleShape player(25);
    // player.setPointCount(300);
    // player.setFillColor(Color::Cyan);

    RectangleShape finishBox(Vector2f(2*pradius,pradius*2)); //fINISH lINE
    finishBox.setFillColor(Color::Green);
    finishBox.setPosition(levelRect[current_level].x-pradius*2,levelRect[current_level].y/2.f-pradius*2);

    int t=1;
    Vector2f hookpos(300+PlayerRadius+10,300);
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
    tBackground.loadFromFile("img/Space_background.png");
    Sprite sBackground;
    sBackground.setTexture(tBackground);
    //sBackground.setOrigin(tBackground.getSize().x/2,tBackground.getSize().y/2);

    // Portal sprite needs to be asymmetric for player to get idea of direction
    Portal::tPortal.loadFromFile("portal.png");
    Portal::sPortal.setTexture(Portal::tPortal);
    Portal::sPortal.setOrigin(sf::Vector2f(1850, 475));
    Portal::sPortal.setScale(sf::Vector2f(0.027, 0.027));

    // Creating Levels
    //Level 1
    std::vector<Map> levels;
    levels.push_back(Map(sf::Vector2f(1000, 1000)));

    levels[0].portals.push_back(Portal(sf::Vector2f(250, 100), sf::Color::Red,   0, 300.0f));
    levels[0].portals.push_back(Portal(sf::Vector2f(100, 350), sf::Color::Red, -90, 300.0f));
    levels[0].portals.push_back(Portal(sf::Vector2f(350, 500), sf::Color::Red, 180, 300.0f));
    levels[0].portals.push_back(Portal(sf::Vector2f(500, 250), sf::Color::Red,  90, 300.0f));
    levels[0].portals[0].pair(&levels[0].portals[3]);
    levels[0].portals[1].pair(&levels[0].portals[2]);
    levels[0].portals[2].pair(&levels[0].portals[1]);
    levels[0].portals[3].pair(&levels[0].portals[0]);

    std::vector<Vector2f> minionPoints1{Vector2f(100, 300), Vector2f(300, 100), Vector2f(100, 100)};
    std::vector<Vector2f> minionPoints2{Vector2f(600, 300), Vector2f(300, 100), Vector2f(600, 100)};
    std::vector<Vector2f> minionPoints3{Vector2f(100, 300), Vector2f(300, 600), Vector2f(100, 600)};
    std::vector<Vector2f> minionPoints4{Vector2f(600, 300), Vector2f(300, 600), Vector2f(600, 600)};
    levels[0].minions.push_back(Minion(&clock, &tcharacter, &app, minionPoints1, sf::Vector2f(0, 0)));
    levels[0].minions.push_back(Minion(&clock, &tcharacter, &app, minionPoints2, sf::Vector2f(700, 0)));
    levels[0].minions.push_back(Minion(&clock, &tcharacter, &app, minionPoints3, sf::Vector2f(0, 700)));
    levels[0].minions.push_back(Minion(&clock, &tcharacter, &app, minionPoints4, sf::Vector2f(700, 700)));

    //Level 2
    levels.push_back(Map(sf::Vector2f(1000, 1000)));

    std::vector<Vector2f> minionPoints5{Vector2f(100, 100), Vector2f(500, 100), Vector2f(500, 500), Vector2f(100, 500)};
    levels[1].minions.push_back(Minion(&clock, &tcharacter, &app, minionPoints5, sf::Vector2f(300, 300)));

    //Level 3


    //Adding Main Menu features
    MainMenu menu(app.getSize().x,app.getSize().y); 
    GameState gameState = STATE_MAIN_MENU;
    // Level counter will start from 0, Add one to it while displaying to players
    int currentLevel = 0;

    // Adding HUD
    sf::Text levelText;
    levelText.setFont(font);
    levelText.setFillColor(sf::Color::Blue);
    levelText.setString("Level: " + std::to_string(currentLevel + 1));
    levelText.setCharacterSize(30);
    levelText.setStyle(sf::Text::Bold);
    levelText.setPosition(sf::Vector2f(15, 15));

    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setFillColor(sf::Color::Yellow);
    scoreText.setString("Score: "+std::to_string(score));
    scoreText.setCharacterSize(30);
    scoreText.setStyle(sf::Text::Bold);
    scoreText.setPosition(sf::Vector2f(15, 550));

    while(app.isOpen()){
        mouse = Mouse::getPosition(app);
        actualMousePos = camPos - (camCoverage * 0.5f);
        actualMousePos.x += mouse.x * camCoverage.x / app.getSize().x;
        actualMousePos.y += mouse.y * camCoverage.y / app.getSize().y;

        Event e;

        while(app.pollEvent(e))
        {
            switch(e.type)
            {
            case sf::Event::Closed:
                app.close();
                break;
            case sf::Event::KeyPressed:
                if(gameState == STATE_DEAD){
                    gameState = STATE_PLAYING;
                    playerpos = spawnPoint[current_level];
                }
                break;
            case sf::Event::KeyReleased:
                switch (e.key.code)
                {
                if(gameState == STATE_PAUSE)
                    gameState = STATE_PLAYING;
                case sf::Keyboard::W:
                    if(gameState == STATE_MAIN_MENU)
                        menu.MoveUp();
                    break;

                case sf::Keyboard::S:
                    if(gameState == STATE_MAIN_MENU)
                        menu.MoveDown();
                    break;

                case sf::Keyboard::Space:
                case sf::Keyboard::Return:
                    if(gameState == STATE_MAIN_MENU)
                    {
                        switch (menu.GetPressedItem())
                        {
                        case 0:
                            gameState = STATE_PLAYING;
                            break;
                        case 1:
                            std::cout<<"Option"<<"\n";
                            break;
                        case 2:
                            app.close();
                            break;
                        }
                    }
                    break;
                // case sf::Keyboard::Escape:
                //     GameState = STATE_PAUSE;
                //     break;
                }
                break;
            case Event::MouseButtonReleased:
                if(e.mouseButton.button == Mouse::Left && gameState==STATE_PLAYING){
                    if(dist(Vector2f(actualMousePos.x,actualMousePos.y),playerpos) > PlayerRadius){
                        grapple = true;
                        grappleSound.setPlayingOffset(sf::seconds(0));
                        grappleSound.play();
                        grapplepos.x = actualMousePos.x;
                        grapplepos.y = actualMousePos.y;
                        initplayerpos = playerpos;
                        grappleAngle = atan((actualMousePos.y-playerpos.y)/(actualMousePos.x-playerpos.x));
                        grappleSpeed = 8;
                        grappleAcc = 2;
                        (actualMousePos.x > playerpos.x) ? g=1:g=-1;
                        magnitude = 1;
                        shakeStart = clock.getElapsedTime();
                    }
                }
                break;
            case Event::MouseWheelScrolled:
                camCoverage += sf::Vector2f(-50, -50)*e.mouseWheelScroll.delta;
                break;
            }
        }
        if(gameState == STATE_PLAYING){
            if(Keyboard::isKeyPressed(Keyboard::W)){
                camPos.y -= 20;
            }
            if(Keyboard::isKeyPressed(Keyboard::A)){
                camPos.x -= 20;
            }
            if(Keyboard::isKeyPressed(Keyboard::S)){
                camPos.y += 20;
            }
            if(Keyboard::isKeyPressed(Keyboard::D)){
                camPos.x += 20;
            }

            if(camCoverage.x < MIN_VIEW_SIZE)
                camCoverage.x = MIN_VIEW_SIZE;
            if(camCoverage.x > MAX_VIEW_SIZE)
                camCoverage.x = MAX_VIEW_SIZE;

            if(camCoverage.y < MIN_VIEW_SIZE)
                camCoverage.y = MIN_VIEW_SIZE;
            if(camCoverage.y > MAX_VIEW_SIZE)
                camCoverage.y = MAX_VIEW_SIZE;

            if(camPos.x < camCoverage.x * 0.5f)
                camPos.x = camCoverage.x * 0.5f;
            if(camPos.x > levels[currentLevel].size.x - camCoverage.x * 0.5f)
                camPos.x = levels[currentLevel].size.x - camCoverage.x * 0.5f;
            if(camPos.y < camCoverage.y * 0.5f)
                camPos.y = camCoverage.y * 0.5f;
            if(camPos.y > levels[currentLevel].size.y - camCoverage.y * 0.5f)
                camPos.y = levels[currentLevel].size.y - camCoverage.y * 0.5f;

            if(Keyboard::isKeyPressed(Keyboard::Space)){
                //CAM SHAKE
                magnitude = 10;
                shakeStart = clock.getElapsedTime();
            }
            if(Keyboard::isKeyPressed(Keyboard::Right));
        }
        if(Keyboard::isKeyPressed(Keyboard::Q))app.close();


        if((clock.getElapsedTime()-shakeStart) <= milliseconds(70)){
            cam.setCenter(camPos.x+magnitude*((float)rand()/RAND_MAX),camPos.y+magnitude*((float)rand()/RAND_MAX));
            magnitude *= 0.5;
        }
        else
            cam.setCenter(camPos);

        cam.setCenter(camPos);
        cam.setSize(camCoverage);

        // Deciding position of grapple
        int t;
        float angle = (atan((actualMousePos.y-playerpos.y)/(actualMousePos.x-playerpos.x)));
        (actualMousePos.x > playerpos.x)? t=1:t=-1;
        hookpos.x = playerpos.x+t*(PlayerRadius+hook.getRadius()+1)*cos(angle);
        hookpos.y = playerpos.y+t*(PlayerRadius+hook.getRadius()+1)*sin(angle);

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
                playerpos = movePlayer(&clock,
                        playerpos,
                        playerMovement,
                        levels[currentLevel].portals,
                        levels[currentLevel].minions,
                        &grapplepos);

                grappleSpeed += grappleAcc;
            }
        }

        scoreText.setString("Score: "+std::to_string(score)); //Updating score

        player.setPosition(playerpos.x,playerpos.y);
        int a = (actualMousePos.x-playerpos.x), b = (actualMousePos.y-playerpos.y);
        float pangle = 0;
        if(a>0 && b>0)
            pangle = angle*180.f/PI;
        else if(a>0 && b<0)
            pangle = 360+angle*180.f/PI;
        else if(a<0 && b<0)
            pangle = 180+angle*180.f/PI;
        else 
            pangle = 180+angle*180.f/PI;    
        player.setRotation(pangle);
        if(grapple && (clock.getElapsedTime()-shakeStart <= milliseconds(70))){
            player.setTextureRect(IntRect(329,20,300,360));
        }
        else if(grapple){
            player.setTextureRect(IntRect(650,9,289,380));
        }
        else player.setTextureRect(IntRect(9,30,291,341));
        hook.setPosition(hookpos.x-hook.getRadius(), hookpos.y-hook.getRadius());
        sf::Vertex line[] ={
            sf::Vertex(sf::Vector2f(playerpos.x, playerpos.y)),
            sf::Vertex(sf::Vector2f(hookpos.x, hookpos.y))
        };
        app.setView(cam);

        //Player OUTSIDE FLOOR:
        if(playerpos.x >= (levelRect[current_level].x+pradius) || playerpos.x <= -pradius || playerpos.y >= (levelRect[current_level].x+pradius) || playerpos.y<=-pradius){
            gameState = STATE_DEAD;
        }
        //Detecting Collisions
        //Player - FinishBox
        if(circleRectCollision(playerpos.x,playerpos.y,pradius,finishBox.getPosition().x,finishBox.getPosition().y,finishBox.getSize().x,finishBox.getSize().y)){
            playerpos = Vector2f(300,300);
            player.setPosition(300,300);
            current_level += 1;
            grapple=false; 
            continue;
        }
        //Player - Minion:
           for(auto i: levels[current_level].minions){
            if(dist(Vector2f(player.getPosition()),Vector2f(i.character.getPosition())) < pradius+i.radius && gameState!=STATE_DEAD && i.alive==true){
                //gameState = STATE_DEAD; break;
            }
           }
        //Player - Outer Wall:
            if(playerpos.x > (levelRect[current_level].x-pradius))playerpos.x=(levelRect[current_level].x-pradius);
            else if(playerpos.x < pradius)playerpos.x=pradius;
            else if(playerpos.y > (levelRect[current_level].y-pradius))playerpos.y=(levelRect[current_level].y-pradius);
            else if(playerpos.y < pradius)playerpos.y = pradius;
            
        if(gameState == STATE_MAIN_MENU) //displays menu
        {
            app.clear();

            if(mainMenuTrack.getStatus() != sf::Music::Status::Playing)
                mainMenuTrack.play();
            if(gameplayTrack.getStatus() == sf::Music::Status::Playing)
                mainMenuTrack.pause();

            menu.draw(app);
            app.display();
        }
        else if(gameState == STATE_PAUSE){  //DIsplays pause menu
            app.clear(Color::Black);

            if(mainMenuTrack.getStatus() != sf::Music::Status::Playing)
                mainMenuTrack.play();
            if(gameplayTrack.getStatus() == sf::Music::Status::Playing)
                gameplayTrack.pause();

            Text text;
            text.setFont(font);
            text.setCharacterSize(34);
            text.setFillColor(Color::White);
            text.setString("Paused");
            text.setPosition(300,100);
            app.draw(text);
            text.setString("Press any key to continue.");
            text.setPosition(50,300);
            app.draw(text);
            app.display();
        }
        else if (gameState == STATE_DEAD){
            app.clear(Color::Black);

            mainMenuTrack.pause();
            gameplayTrack.pause();

            Text text;
            text.setFont(font);
            text.setCharacterSize(34);
            text.setFillColor(Color::Red);
            text.setString("YOU DIED!");
            text.setPosition(210,100);
            app.draw(text);
            text.setFillColor(Color::White);
            text.setString("Press any key to continue..");
            text.setPosition(90,300);
            app.draw(text);
            text.setCharacterSize(24);
            text.setString(std::to_string(clock.getElapsedTime().asSeconds())+"s");
            text.setPosition(10,500);
            app.draw(text);
            app.display();
            player.setPosition(Vector2f(300,300));
        }
        else if(gameState == STATE_WON){
            app.clear(Color::Black);
            Text text;
            text.setFont(font);
            text.setCharacterSize(34);
            text.setFillColor(Color::Green);
            text.setString("Game Won!! 🗿");
            text.setPosition(300,100);
            app.draw(text);
            text.setString("Thank you for your Patience! :D");
            text.setPosition(100,300);
            app.draw(text);
            app.display();
        }
        else    //STATE_PLAYING
        {
            app.clear();

            //BGM

            if(mainMenuTrack.getStatus() == sf::Music::Status::Playing)
                mainMenuTrack.pause();
            if(gameplayTrack.getStatus() != sf::Music::Status::Playing)
                gameplayTrack.play();

            //Start Drawing here

            //Draw background
            app.draw(sBackground);

            //Draw Floor
            RectangleShape floor(levelRect[current_level]);
            floor.setFillColor(Color(200,190,190));
            floor.setPosition(0,0);
            floor.setOutlineColor(Color(69,69,69));
            floor.setOutlineThickness(10);
            app.draw(floor);
            app.draw(finishBox);

            //Draw Player
            if(grapple)app.draw(line, 5, Lines);
            app.draw(player);
            app.draw(hook); 

            // Drawing Portals
            if(levels[current_level].portals.size() > 0)
            for(auto it : levels[currentLevel].portals)
                it.draw(app);

            // Drawing enemies
            if(levels[current_level].minions.size() > 0)
            for(int i = 0; i < levels[currentLevel].minions.size(); i++)
                levels[currentLevel].minions[i].draw();

            // Finally draw hud elements at the top
            app.draw(levelText);
            app.draw(scoreText);

            /******///playerBoundingBox.setPosition(playerpos.x-pradius,playerpos.y-pradius);app.draw(playerBoundingBox);

            //Stop Drawing here
            app.display();
        }
    }
    return 0;
}
