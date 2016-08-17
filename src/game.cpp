#include "game.hpp"
#include "io.hpp"
#include "rl_utils.hpp"
#include "map.hpp"
#include "map_ter.hpp"

namespace game
{

namespace
{

bool quit_;

} // namespace

void init()
{
    TRACE_FUNC_BEGIN;

    quit_ = false;

    TRACE_FUNC_END;
}

void cleanup()
{
    TRACE_FUNC_BEGIN;

    TRACE_FUNC_END;
}

void run()
{
    TRACE_FUNC_BEGIN;

    io::clear_scr();

    map::init();

    while (!quit_)
    {
        map::draw();

        io::draw_text("Hello and welcome to [strategy-game]!",
                      P(0, 0),
                      clr_white);

        io::update_screen();

        io::read_input();
    }

    map::cleanup();

    TRACE_FUNC_END;
}

void quit()
{
    quit_ = true;
}

} // game
