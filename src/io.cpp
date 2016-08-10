#include "io.hpp"

#include <SDL.h>
#include <SDL_image.h>

#include "global.hpp"

namespace io
{

namespace
{

bool is_inited_ = false;

SDL_Window*   sdl_window_   = nullptr;
SDL_Renderer* sdl_renderer_ = nullptr;

//SDL_Event sdl_event_;

//------------------------------------------------------------------------------
// TODO: The following should be configured from a script or something
//------------------------------------------------------------------------------
bool font_px_data_[256][128];

const std::string font_name = "curses_8x16.png";

const int font_x_cells_ = 32;
const int cell_px_w_    = 8;
const int cell_px_h_    = 16;
//const int scr_w_        = map_w;
const int scr_h_        = map_h;
const int scr_px_w_     = map_w * cell_px_w_;
const int scr_px_h_     = map_h * cell_px_h_;
//------------------------------------------------------------------------------

bool is_inited()
{
    return is_inited_;
}

void set_render_clr(const Clr& clr)
{
    SDL_SetRenderDrawColor(sdl_renderer_,
                           clr.r,
                           clr.g,
                           clr.b,
                           255);
}

Uint32 get_px(SDL_Surface* const surface, const int px_x, const int px_y)
{
    int bpp = surface->format->BytesPerPixel;

    // Here p is the address to the pixel we want to retrieve
    Uint8* p = (Uint8*)surface->pixels + px_y * surface->pitch + px_x * bpp;

    switch(bpp) {
    case 1:   return *p;          break;
    case 2:   return *(Uint16*)p; break;
    case 3: {
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            return p[0] << 16 | p[1] << 8 | p[2];
        }   else {
            return p[0] | p[1] << 8 | p[2] << 16;
        }
    } break;
    case 4:   return *(Uint32*)p; break;
    default:  return -1;          break;
    }
    return -1;
}

void put_px(const P& px_pos) {
    SDL_RenderDrawPoint(sdl_renderer_, px_pos.x, px_pos.y);
}

void load_font_data()
{
    TRACE_FUNC_BEGIN;

    SDL_Surface* font_surface_tmp = IMG_Load(font_name.c_str());

    Uint32 bg_clr = SDL_MapRGB(font_surface_tmp->format, 0, 0, 0);

    for (int x = 0; x < font_surface_tmp->w; ++x)
    {
        for (int y = 0; y < font_surface_tmp->h; ++y)
        {
            font_px_data_[x][y] = get_px(font_surface_tmp, x, y) != bg_clr;
        }
    }

    SDL_FreeSurface(font_surface_tmp);

    TRACE_FUNC_END;
}

P get_glyph_pos(const char glyph)
{
    const int x = glyph % font_x_cells_;
    const int y = glyph / font_x_cells_;

    return P(x, y);
}

void put_glyph_pxs_on_screen(const char glyph, const P& px_pos, const Clr& clr)
{
    const P sheet_pos = get_glyph_pos(glyph);

    const P sheet_px_pos_0(sheet_pos.x * cell_px_w_,
                           sheet_pos.y * cell_px_h_);

    const P sheet_px_pos_1(sheet_px_pos_0.x + cell_px_w_ - 1,
                           sheet_px_pos_0.y + cell_px_h_ - 1);

    int screen_px_x = px_pos.x;
    int screen_px_y = px_pos.y;

    set_render_clr(clr);

    for (int sheet_px_x = sheet_px_pos_0.x; sheet_px_x <= sheet_px_pos_1.x; sheet_px_x++)

    {
        screen_px_y = px_pos.y;

        for (int sheet_px_y = sheet_px_pos_0.y; sheet_px_y <= sheet_px_pos_1.y; sheet_px_y++)
        {
            if (font_px_data_[sheet_px_x][sheet_px_y])
            {
                put_px(P(screen_px_x, screen_px_y));
            }
            ++screen_px_x;
        }
        ++screen_px_y;
    }
}

void draw_glyph_at_px(const char glyph,
                      const P& px_pos,
                      const Clr& clr,
                      const Clr& bg_clr = clr_black)
{
    draw_rect(px_pos,
              P(cell_px_w_, cell_px_h_),
              bg_clr);

    put_glyph_pxs_on_screen(glyph, px_pos, clr);
}

} // namespace

void init()
{
    TRACE_FUNC_BEGIN;

    cleanup();

    TRACE << "Setting up rendering window" << std::endl;

    const std::string title = "Strategy game 0.0.1";

    sdl_window_ = SDL_CreateWindow(title.data(),
                                   SDL_WINDOWPOS_UNDEFINED,
                                   SDL_WINDOWPOS_UNDEFINED,
                                   scr_px_w_,
                                   scr_px_h_,
                                   SDL_WINDOW_SHOWN);

    sdl_renderer_ = SDL_CreateRenderer(sdl_window_,
                                       -1,
                                       SDL_RENDERER_ACCELERATED);

    load_font_data();

    is_inited_ = true;

    TRACE_FUNC_END;
}

void cleanup()
{
    TRACE_FUNC_BEGIN;

    is_inited_ = false;

    if (sdl_renderer_)
    {
        SDL_DestroyRenderer(sdl_renderer_);

        sdl_renderer_ = nullptr;
    }

    if (sdl_window_)
    {
        SDL_DestroyWindow(sdl_window_);

        sdl_window_ = nullptr;
    }

    TRACE_FUNC_END;
}

void clear_scr()
{
    if (is_inited())
    {
        set_render_clr(clr_black);

        SDL_RenderClear(sdl_renderer_);
    }
}

void render_present()
{
    if (is_inited())
    {
        SDL_RenderPresent(sdl_renderer_);
    }
}

void draw_text(const std::string& str,
               const P& p,
               const Clr& clr,
               const Clr& bg_clr)
{
    if (is_inited())
    {
        if (p.y < 0 || p.y >= scr_h_)
        {
            return;
        }

        P px_pos(p.x * cell_px_w_,
                 p.y * cell_px_h_);

        for (const char glyph : str)
        {
            if (px_pos.x < 0 || px_pos.x >= scr_px_w_)
            {
                return;
            }

            draw_glyph_at_px(glyph,
                             px_pos,
                             clr,
                             bg_clr);

            px_pos.x += cell_px_w_;
        }
    }
}

void draw_rect(const P& p,
               const P& d,
               const Clr& clr)
{
    if (is_inited())
    {
        const SDL_Rect sdl_rect =
        {
            (Sint16)p.x,
            (Sint16)p.y,
            (Uint16)d.x,
            (Uint16)d.y
        };

        set_render_clr(clr);

        SDL_RenderFillRect(sdl_renderer_, &sdl_rect);
    }
}

} //io
