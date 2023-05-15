#include "Minion.hpp"

Minion::Minion(Clock* clock_, Texture* t, RenderWindow* app, std::vector<Vector2f> travellingPoints, Vector2f connectionPoint)
    :connectingWire(connectionPoint, travellingPoints[0]), connectionPoint(connectionPoint)
{
    this->app = app;
    //if(!tcharacter.loadFromFile("img/Extras.png")){std::cerr<<"FAILED to load extras.png!!!\n";}
    character = Sprite(*t,sf::IntRect(650,10,140,130));
    character.setOrigin(character.getLocalBounds().width/2.f,character.getLocalBounds().height/2.f);
    character.setScale(0.5f,0.5f);

    boundingBox.setRadius(radius);
    boundingBox.setPointCount(300);

    this->travellingPoints = travellingPoints;
    this->position =  travellingPoints[0];
    this->currentDestination = 1;
    initPos = travellingPoints[0];
    finalPos = travellingPoints[1];
    
    this->u = ((float)s/dist(initPos,finalPos))*(finalPos-initPos); 
    
    connectingWire.updateWire(position, connectionPoint);

    clocku = clock_;
}

Minion::~Minion(){
}

void Minion::draw()
{
    if(dist(finalPos,position) <= s){
        /* std::cout<<"--Closing in--"<<std::endl; */
        move(finalPos);

        if(currentDestination >= (travellingPoints.size()-1)){
            this->dir = -1;
        }
        else if(currentDestination <= 0){
            dir = +1;
        } 

        /* std::cout<<"new dir="<<dir<<std::endl; */
        /* std::cout<<"rn, CurrentDestination="<<currentDestination<<std::endl; */
        initPos = travellingPoints[currentDestination];
        currentDestination = currentDestination + dir;
        /* std::cout<<"new, CurrentDestination="<<currentDestination<<std::endl; */
        finalPos = travellingPoints[currentDestination];
        
        
        /* std::cout<<">>declared all variables"<<std::endl; */
        this->u = ((float)s/dist(initPos,finalPos))*(finalPos-initPos); 
        /* std::cout<<"New u: ("<<u.x<<","<<u.y<<")"<<std::endl; */
    }

    /* std::cout<<"("<<position.x<<","<<position.y<<") ("<<currentDestination<<") ("<<dir<<") ("<<initPos.x<<","<<initPos.y<<") ("<<finalPos.x<<","<<finalPos.y<<")"<<std::endl; */

    move(position + u);

    character.setPosition(position.x,position.y);
    boundingBox.setPosition(position.x-radius,position.y-radius);
    connectingWire.draw(*app);

    //Death animation:
    Time t = clocku->getElapsedTime()-deathTime;
    if(t < milliseconds(200)){
        if(t.asMilliseconds()%(50)){
            character.setTextureRect(sf::IntRect(650,10,140,130));
        }
        else{
            character.setTextureRect(sf::IntRect(809,9,141,130));
        }
    }else if(t < milliseconds(500)){
        character.setTextureRect(sf::IntRect(970,10,140,130));
    }
    if(alive && t > milliseconds(500))
        (*app).draw(character);
    //(*app).draw(boundingBox);
}
void Minion::move(sf::Vector2f newPosition)
{
    position = newPosition;
    connectingWire.updateWire(position, connectionPoint);
}
