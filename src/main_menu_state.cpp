#include "main_menu_state.hpp"
#include "map_state.hpp"

void MainMenuState::render() const
{
    io::draw_text("Main menu placeholder\n\n[n] Continue\n[q] Quit",
                  P(0, 0),
                  clr_white,
                  clr_black);
}

void MainMenuState::on_input(const InputData& input)
{
    switch (input.key)
    {
    case 'n':
    {
        std::unique_ptr<State> map_state(new MapState);

        states::push_state(std::move(map_state));
    }
    break;

    case 'q':
    {
        states::pop_state();
    }
    break;
    }
}
