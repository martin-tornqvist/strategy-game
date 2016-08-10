#ifndef IO_HPP
#define IO_HPP

#include "colors.hpp"
#include "rl_utils.hpp"

namespace io
{

void init();

void cleanup();

void clear_scr();

void draw_text(const std::string& str,
               const P& p,
               const Clr& clr,
               const Clr& bg_clr);

void draw_rect(const P& p,
               const P& d,
               const Clr& clr);

} // io

#endif // IO_HPP
