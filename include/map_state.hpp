#ifndef MAP_STATE_HPP
#define MAP_STATE_HPP

#include "state.hpp"

class MapState : public State
{
public:
    MapState() {}

    ~MapState() {}

    void render() const override;

    void on_input(const InputData& input) override;

    void on_pushed() override;
};

#endif // MAP_STATE_HPP
