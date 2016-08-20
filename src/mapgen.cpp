#include "mapgen.hpp"

#include "map.hpp"
#include "map_ter.hpp"

namespace mapgen
{

void run()
{
    const TerData& d = ter_data::get();

    for (int x = 0; x < map::map_dims.x; ++x)
    {
        for (int y = 0; y < map::map_dims.y; ++y)
        {
            map::ter(x, y).reset(new MapTer(d, P(x, y)));
        }
    }
}

} // mapgen
