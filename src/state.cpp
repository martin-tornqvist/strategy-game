#include "state.hpp"

#include "io.hpp"

//-----------------------------------------------------------------------------
// State implementations
//-----------------------------------------------------------------------------
void MainMenuState::render() const
{
    io::draw_text("This is the main menu",
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

void MainMenuState::on_pushed()
{

}

void MapState::render() const
{
    io::draw_text("This is the map",
                  P(1, 1),
                  clr_green_lgt,
                  clr_black);
}

void MapState::on_input(const InputData& input)
{
    if (input.key == 'b')
    {
        states::pop_state();
    }
}

void MapState::on_pushed()
{

}

//-----------------------------------------------------------------------------
// State keeping
//-----------------------------------------------------------------------------
namespace states
{

namespace
{

std::vector< std::unique_ptr<State> > states_;

} // namespace

void init()
{
    TRACE_FUNC_BEGIN;

    cleanup();

    TRACE_FUNC_END;
}

void cleanup()
{
    TRACE_FUNC_BEGIN;

    states_.resize(0);

    TRACE_FUNC_END;
}

void render()
{
    if (!states_.empty())
    {
        states_.back()->render();
    }
}

void handle_input(const InputData& input)
{
    if (!states_.empty())
    {
        states_.back()->on_input(input);
    }
}

void push_state(std::unique_ptr<State> state)
{
    TRACE_FUNC_BEGIN;

    states_.push_back(std::move(state));
    states_.back()->on_pushed();

    TRACE_FUNC_END;
}

void pop_state()
{
    TRACE_FUNC_BEGIN;

    if (!states_.empty())
    {
        states_.back()->on_popped();
        states_.pop_back();
    }

    TRACE_FUNC_END;
}

bool is_empty()
{
    return states_.empty();
}

} // states
