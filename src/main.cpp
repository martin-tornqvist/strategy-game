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

    // Init
    io::init();
    game::init();

    // Run game session until done
    game::run();

    // Cleanup
    game::cleanup();
    io::cleanup();

    TRACE_FUNC_END;

    return 0;
}
