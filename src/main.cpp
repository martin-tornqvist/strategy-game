#include "io.hpp"
#include "rl_utils.hpp"

#include "game.hpp"

#ifdef _WIN32
#undef main
#endif
int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    TRACE_FUNC_BEGIN;

    // Init IO
    io::init();
    io::clear_scr();

    // Run game session until done
    game::run_session();

    // Cleanup IO
    io::cleanup();

    TRACE_FUNC_END;

    return 0;
}
