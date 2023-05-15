#include "Minion.hpp"
#include <SFML/System/Vector2.hpp>
#include "portal.hpp"
#include "spike.hpp"
#include <vector>

class Map
{
    public:
    sf::Vector2f size;
    std::vector<Portal> portals;
    std::vector<Minion> minions;
    std::vector<Spike> spikes;

    public:
    Map(sf::Vector2f mapSize);
};
