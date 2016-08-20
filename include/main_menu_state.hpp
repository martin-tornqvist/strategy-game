#ifndef MAIN_MENU_STATE_HPP
#define MAIN_MENU_STATE_HPP

#include "state.hpp"

class MainMenuState : public State
{
public:
    MainMenuState() {}

    ~MainMenuState() {}

    void render() const override;

    void on_input(const InputData& input) override;
};

#endif // MAIN_MENU_STATE_HPP
