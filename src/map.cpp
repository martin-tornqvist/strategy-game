#include "map.hpp"

#include "map_ter.hpp"

namespace map
{

Array2< std::unique_ptr<MapTer> > ter;

void init()
{
    ter.resize(map_w, map_h);

    for (int x = 0; x < map_w; ++x)
    {
        for (int y = 0; y < map_h; ++y)
        {
            map::ter(x, y).reset(new MapTer(P(x, y)));
        }
    }
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

    io::draw_char(ter(0, 0)->p().x + '0',
                  P(15, 15),
                  clr_magenta);
}

} // map
