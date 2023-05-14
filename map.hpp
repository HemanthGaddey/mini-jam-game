#include "Minion.hpp"
#include <SFML/System/Vector2.hpp>
#include "portal.hpp"
#include <vector>

class Map
{
    public:
    sf::Vector2f size;
    std::vector<Portal> portals;
    std::vector<Minion> minions;

    public:
    Map(sf::Vector2f mapSize);
};
