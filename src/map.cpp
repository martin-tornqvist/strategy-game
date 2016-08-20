#include "map.hpp"

#include "map_ter.hpp"
#include "script.hpp"

namespace map
{

P map_dims;

Array2< std::unique_ptr<MapTer> > ter;

namespace
{

void load_script_data()
{
    TRACE_FUNC_BEGIN;

    auto j = script::read("map.json");

    map_dims = script::get_p("width", "height", j);

    TRACE_FUNC_END;
}

} // namespace

void init()
{
    TRACE_FUNC_BEGIN;

    load_script_data();

    ter.resize(map_dims.x, map_dims.y);

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

            io::draw_char(d.symbol,
                          ter->p(),
                          d.clr,
                          d.clr_bg);
        }
    });
}

} // map
