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

void read_input();

} // io

#endif // IO_HPP
