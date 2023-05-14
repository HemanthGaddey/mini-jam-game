#include <SFML/System/Vector2.hpp>
#include <portal.hpp>
#include <vector>

class Map
{
    public:
    sf::Vector2f size;
    std::vector<Portal> portals;

    public:
    Map(sf::Vector2f mapSize);
};
