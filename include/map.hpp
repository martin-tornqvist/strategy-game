#ifndef MAP_HPP
#define MAP_HPP

#include <memory>

#include "rl_utils.hpp"
#include "global.hpp"

class MapTer;

namespace map
{

extern P map_dims;

extern Array2< std::unique_ptr<MapTer> > ter;

void init();

void cleanup();

void draw();

} // map

#endif // MAP_HPP
