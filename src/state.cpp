#include "state.hpp"

#include "io.hpp"


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

    io::flush_input();

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

    io::flush_input();

    TRACE_FUNC_END;
}

bool is_empty()
{
    return states_.empty();
}

} // states
