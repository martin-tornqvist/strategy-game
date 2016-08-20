#include "map_state.hpp"

#include "mapgen.hpp"
#include "map.hpp"

void MapState::render() const
{
    map::draw();
}

void MapState::on_input(const InputData& input)
{
    if (input.key == 'q')
    {
        states::pop_state();
    }
}

void MapState::on_pushed()
{
    // Generate a map
    mapgen::run();
}
