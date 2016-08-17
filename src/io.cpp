#include "io.hpp"

#include <SDL.h>
#include <SDL_image.h>
#include <fstream>

#include "global.hpp"
#include "game.hpp"
#include "json.hpp"

namespace io
{

namespace
{

bool is_inited_ = false;

SDL_Window*   sdl_window_   = nullptr;
SDL_Renderer* sdl_renderer_ = nullptr;

SDL_Event sdl_event_;

// Configured by script
std::string font_name   = "";
int font_img_px_w_      = 0;
int font_img_px_h_      = 0;
int cell_px_w_          = 0;
int cell_px_h_          = 0;
int scr_px_w_           = 0;
int scr_px_h_           = 0;

Array2<bool> font_px_data_;

// Calculated
int font_x_cells_ = 0;

//------------------------------------------------------------------------------
// TODO: The following should be configured from a script
//------------------------------------------------------------------------------
const int scr_w_ = map_w;
const int scr_h_ = map_h;
//------------------------------------------------------------------------------

bool is_inited()
{
    return is_inited_;
}

void load_script_data()
{
    std::ifstream ifs("script/font.json");

    nlohmann::json j(ifs);

    font_name       = j["font_name"];
    font_img_px_w_  = j["image_width"];
    font_img_px_h_  = j["image_height"];
    cell_px_w_      = j["char_width"];
    cell_px_h_      = j["char_height"];

    scr_px_w_ = scr_w_ * cell_px_w_;
    scr_px_h_ = scr_h_ * cell_px_h_;
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

    switch (bpp)
    {
    case 1:
        return *p;

    case 2:
        return *(Uint16*)p;

    case 3:
        if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
        {
            return p[0] << 16 | p[1] << 8 | p[2];
        }
        else // Little endian
        {
            return p[0] | p[1] << 8 | p[2] << 16;
        }

    case 4:
        return *(Uint32*)p;

    default:
        return -1;
    }

    return -1;
}

void put_px(const P& px_pos) {
    SDL_RenderDrawPoint(sdl_renderer_, px_pos.x, px_pos.y);
}

void load_font_data()
{
    TRACE_FUNC_BEGIN;

    // Assuming font dimensions have been set up by now
    if (font_img_px_w_ == 0 || font_img_px_h_ == 0)
    {
        ASSERT(false);

        return;
    }

    font_px_data_.resize(font_img_px_w_,
                         font_img_px_h_);

    font_x_cells_ = font_img_px_w_ / cell_px_w_;

    SDL_Surface* font_surface_tmp = IMG_Load(font_name.c_str());

    Uint32 bg_clr = SDL_MapRGB(font_surface_tmp->format, 0, 0, 0);

    for (int x = 0; x < font_surface_tmp->w; ++x)
    {
        for (int y = 0; y < font_surface_tmp->h; ++y)
        {
            const bool set_px = get_px(font_surface_tmp, x, y) != bg_clr;

            font_px_data_(x, y) = set_px;
        }
    }

    SDL_FreeSurface(font_surface_tmp);

    TRACE_FUNC_END;
}

P get_glyph_pos(const char glyph)
{
    return P(glyph % font_x_cells_,
             glyph / font_x_cells_);
}

void put_glyph_pxs_on_screen(const char glyph,
                             const P& px_pos,
                             const Clr& clr)
{
    const P sheet_pos = get_glyph_pos(glyph);

    const P sheet_px_pos_0(sheet_pos.x * cell_px_w_,
                           sheet_pos.y * cell_px_h_);

    const P sheet_px_pos_1(sheet_px_pos_0.x + cell_px_w_ - 1,
                           sheet_px_pos_0.y + cell_px_h_ - 1);

    int screen_px_x = px_pos.x;
    int screen_px_y = px_pos.y;

    set_render_clr(clr);

    for (int sheet_px_x = sheet_px_pos_0.x;
         sheet_px_x <= sheet_px_pos_1.x;
         ++sheet_px_x)
    {
        screen_px_y = px_pos.y;

        for (int sheet_px_y = sheet_px_pos_0.y;
             sheet_px_y <= sheet_px_pos_1.y;
             ++sheet_px_y)
        {
            const bool v = font_px_data_(sheet_px_x, sheet_px_y);

            if (v)
            {
                put_px(P(screen_px_x, screen_px_y));
            }
            ++screen_px_y;
        }
        ++screen_px_x;
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

    load_script_data();

    if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
    {
        TRACE << "Failed to init SDL: " << SDL_GetError() << std::endl;
        ASSERT(false);
    }

    if (IMG_Init(IMG_INIT_PNG) == -1)
    {
        TRACE << "Failed to init SDL IMG: " << IMG_GetError() << std::endl;
        ASSERT(false);
    }

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

    IMG_Quit();

    SDL_Quit();

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

void update_screen()
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
    if (!is_inited())
    {
        return;
    }

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

void draw_char(const char glyph,
               const P& p,
               const Clr& clr,
               const Clr& bg_clr)
{
    if (!is_inited())
    {
        return;
    }

    const R screen_r(0, 0, scr_w_, scr_h_);

    if (!screen_r.is_p_inside(p))
    {
        return;
    }

    const P px_pos(p.x * cell_px_w_,
                   p.y * cell_px_h_);

    draw_glyph_at_px(glyph,
                     px_pos,
                     clr,
                     bg_clr);
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

void sleep(const unsigned int ms)
{
    if (is_inited())
    {
        if (ms == 1)
        {
            SDL_Delay(1);
        }
        else // "Long wait" pump events so game doesn't appear frozen
        {
            const unsigned int wait_until = SDL_GetTicks() + ms;

            while (SDL_GetTicks() < wait_until)
            {
                SDL_PumpEvents();
            }
        }
    }
}

void read_input()
{
    if (!is_inited())
    {
        return;
    }

    while (SDL_PollEvent(&sdl_event_) != 0)
    {
        if(sdl_event_.type == SDL_QUIT)
        {
            game::quit();
        }

        if (sdl_event_.type == SDL_KEYDOWN)
        {
            switch (sdl_event_.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                game::quit();
                break;

            default:
                break;
            }
        }
    }
}

} // io
