#include "io.hpp"
#include "rl_utils.hpp"
#include "state.hpp"
#include "map.hpp"

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
    states::init();
    map::init();

    std::unique_ptr<State> main_menu_state(new MainMenuState);

    states::push_state(std::move(main_menu_state));

    // Loop while there is at least one state
    while (!states::is_empty())
    {
        io::clear_scr();

        states::render();

        io::update_screen();

        InputData input = io::get_input();

        states::handle_input(input);
    }

    // Cleanup
    map::cleanup();
    states::cleanup();
    io::cleanup();

    TRACE_FUNC_END;

    return 0;
}
