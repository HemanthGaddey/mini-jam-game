#include "spike.hpp"

Spike::Spike(RenderWindow* app_, float x, float y, int rotation=0){
    app = app_;
    texture.loadFromFile("img/spikes.png");
    sprite.setTexture(texture);
    sprite.setTextureRect(IntRect(60,50,410,240));
    sprite.setScale(0.5f,0.3f);
    sprite.setPosition(x,y);
    sprite.setRotation(rotation);
}

void Spike::draw(){
    app->draw(sprite);
}