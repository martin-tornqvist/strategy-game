#include "map.hpp"

#include <fstream>

#include "map_ter.hpp"
#include "json.hpp"

namespace map
{

P map_dims;

Array2< std::unique_ptr<MapTer> > ter;

namespace
{

void load_script_data()
{
    TRACE_FUNC_BEGIN;

    std::ifstream ifs("scripts/map.json");

    nlohmann::json j(ifs);

    map_dims.x = j["map_width"];
    map_dims.y = j["map_height"];

    TRACE_FUNC_END;
}

} // namespace

void init()
{
    TRACE_FUNC_BEGIN;

    load_script_data();

    ter.resize(map_dims.x, map_dims.y);

    for (int x = 0; x < map_dims.x; ++x)
    {
        for (int y = 0; y < map_dims.y; ++y)
        {
            map::ter(x, y).reset(new MapTer(P(x, y)));
        }
    }

    TRACE_FUNC_END;
}

void cleanup()
{
    ter.resize(0, 0);
}

void draw()
{
    ter.for_each([](auto& ter)
    {
        if (ter)
        {
            const auto& d = ter->render_data();

            io::draw_char(d.glyph,
                          ter->p(),
                          d.clr,
                          d.clr_bg);
        }
    });
}

} // map
