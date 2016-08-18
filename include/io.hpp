#ifndef IO_HPP
#define IO_HPP

#include "colors.hpp"
#include "rl_utils.hpp"

struct RenderData
{
    RenderData() :
        glyph   (0),
        clr     (clr_white),
        clr_bg  (clr_black) {}

    char glyph;
    Clr clr;
    Clr clr_bg;
};

enum class MouseBtn
{
    none,
    left,
    right
};

struct InputData
{
    InputData() :
        key         (-1),
        mouse_px_p  (),
        mouse_btn   (MouseBtn::none) {}

    int key;

    P mouse_px_p;

    MouseBtn mouse_btn;
};

namespace io
{

void init();

void cleanup();

void clear_scr();

void update_screen();

void draw_text(const std::string& str,
               const P& p,
               const Clr& clr,
               const Clr& bg_clr = clr_black);

void draw_char(const char glyph,
               const P& p,
               const Clr& clr,
               const Clr& bg_clr = clr_black);

void draw_rect(const P& p,
               const P& d,
               const Clr& clr);

void sleep(const unsigned int ms);

InputData get_input();

} // io

#endif // IO_HPP
