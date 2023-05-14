#include "Minion.hpp"

Minion::Minion(RenderWindow* app, std::vector<Vector2f> travellingPoints)
{
    this->app = app;

    character.setRadius(25);
    character.setPointCount(300);
    character.setFillColor(Color::Red);

    this->travellingPoints = travellingPoints;
    this->position =  travellingPoints[0];
    this->currentDestination = 1;
    initPos = travellingPoints[0];
    finalPos = travellingPoints[1];
    
    this->u = ((float)s/dist(initPos,finalPos))*(finalPos-initPos); 
}

Minion::~Minion(){
    free(app);
}

void Minion::draw()
{
    if(dist(finalPos,position) <= s){
        std::cout<<"--Closing in--"<<std::endl;
        position = finalPos;

        if(currentDestination >= (travellingPoints.size()-1)){
            this->dir = -1;
        }
        else if(currentDestination <= 0){
            dir = +1;
        } 

        std::cout<<"new dir="<<dir<<std::endl;
        std::cout<<"rn, CurrentDestination="<<currentDestination<<std::endl;
        initPos = travellingPoints[currentDestination];
        currentDestination = currentDestination + dir;
        std::cout<<"new, CurrentDestination="<<currentDestination<<std::endl;
        finalPos = travellingPoints[currentDestination];
        
        
        std::cout<<">>declared all variables"<<std::endl;
        this->u = ((float)s/dist(initPos,finalPos))*(finalPos-initPos); 
        std::cout<<"New u: ("<<u.x<<","<<u.y<<")"<<std::endl;
    }

    std::cout<<"("<<position.x<<","<<position.y<<") ("<<currentDestination<<") ("<<dir<<") ("<<initPos.x<<","<<initPos.y<<") ("<<finalPos.x<<","<<finalPos.y<<")"<<std::endl;

    position = position + u;
    character.setPosition(position.x-character.getRadius(),position.y-character.getRadius());
    (*app).draw(character);
}