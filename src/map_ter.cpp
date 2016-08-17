#include "map_ter.hpp"

RenderData MapTer::render_data() const
{
    RenderData d;

    d.glyph = '.';
    d.clr   = clr_green_lgt;

    return d;
}

std::string MapTer::name() const
{
    return "Grass";
}
