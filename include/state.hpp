#ifndef STATE_HPP
#define STATE_HPP

#include "io.hpp"

#include <vector>
#include <memory>

class State
{
public:
    State() {}

    virtual ~State() {}

    virtual void render() const {}

    virtual void on_input(const InputData& input)
    {
        (void)input;
    }

    virtual void on_pushed() {}

    virtual void on_popped() {}
};

namespace states
{

void init();

void cleanup();

void render();

void handle_input(const InputData& input);

void push_state(std::unique_ptr<State> state);

void pop_state();

bool is_empty();

} // states

#endif // STATE_HPP
