#include "io.hpp"

#include <SDL.h>
#include <SDL_image.h>

#include "global.hpp"
#include "script.hpp"

namespace io
{

namespace
{

bool is_inited_ = false;

SDL_Window*   sdl_window_   = nullptr;
SDL_Renderer* sdl_renderer_ = nullptr;

SDL_Event sdl_event_;

std::string font_name   = "";

P font_img_px_d_;
P cell_px_d_;
P scr_d_;
P scr_px_d_;

int font_w_ = 0;

Array2<bool> font_img_px_data_;

bool is_inited()
{
    return is_inited_;
}

void load_script_data()
{
    TRACE_FUNC_BEGIN;

    auto j = script::read("font.json");

    font_name       = script::get_str("font_name", j);
    font_img_px_d_  = script::get_p("image_width", "image_height", j);
    cell_px_d_      = script::get_p("char_width", "char_height", j);

    j = script::read("render.json");

    scr_d_ = script::get_p("screen_width", "screen_height", j);

    TRACE_FUNC_END;
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
    SDL_RenderDrawPoint(sdl_renderer_,
                        px_pos.x,
                        px_pos.y);
}

void load_font_data()
{
    TRACE_FUNC_BEGIN;

    font_img_px_data_.resize(font_img_px_d_);

    const std::string font_path = "fonts/" + font_name;

    SDL_Surface* font_surface_tmp = IMG_Load(font_path.c_str());

    Uint32 bg_clr = SDL_MapRGB(font_surface_tmp->format, 0, 0, 0);

    for (int x = 0; x < font_img_px_d_.x; ++x)
    {
        for (int y = 0; y < font_img_px_d_.y; ++y)
        {
            const bool set_px = get_px(font_surface_tmp, x, y) != bg_clr;

            font_img_px_data_(x, y) = set_px;
        }
    }

    SDL_FreeSurface(font_surface_tmp);

    TRACE_FUNC_END;
}

P get_glyph_pos(const char c)
{
    return P(c % font_w_,
             c / font_w_);
}

void put_glyph_pxs_on_screen(const char glyph,
                             const P& px_pos,
                             const Clr& clr)
{
    const P sheet_pos = get_glyph_pos(glyph);

    const P sheet_px_pos_0(sheet_pos * cell_px_d_);

    const P sheet_px_pos_1(sheet_px_pos_0 + cell_px_d_ - 1);

    P scr_px_pos(px_pos);

    set_render_clr(clr);

    for (int sheet_px_x = sheet_px_pos_0.x;
         sheet_px_x <= sheet_px_pos_1.x;
         ++sheet_px_x)
    {
        scr_px_pos.y = px_pos.y;

        for (int sheet_px_y = sheet_px_pos_0.y;
             sheet_px_y <= sheet_px_pos_1.y;
             ++sheet_px_y)
        {
            const bool v = font_img_px_data_(sheet_px_x, sheet_px_y);

            if (v)
            {
                put_px(scr_px_pos);
            }
            ++scr_px_pos.y;
        }
        ++scr_px_pos.x;
    }
}

void draw_glyph_at_px(const char glyph,
                      const P& px_pos,
                      const Clr& clr,
                      const Clr& bg_clr = clr_black)
{
    draw_rect(px_pos,
              cell_px_d_,
              bg_clr);

    put_glyph_pxs_on_screen(glyph,
                            px_pos,
                            clr);
}

} // namespace

void init()
{
    TRACE_FUNC_BEGIN;

    cleanup();

    load_script_data();

    // Calculate values based on script parameters
    scr_px_d_   = scr_d_ * cell_px_d_;
    font_w_     = font_img_px_d_.x / cell_px_d_.x;

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
                                   scr_px_d_.x,
                                   scr_px_d_.y,
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

    if (p.y < 0 || p.y >= scr_d_.x)
    {
        return;
    }

    P px_pos(p * cell_px_d_);

    for (auto it = begin(str); it != end(str); ++it)
    {
        if (px_pos.x < 0 || px_pos.x >= scr_px_d_.x)
        {
            return;
        }

        const char c = *it;

        // Format character?
        if (c == '\n')
        {
            px_pos.x     = p.x * cell_px_d_.x;
            px_pos.y    += cell_px_d_.y;

            continue;
        }

        draw_glyph_at_px(c,
                         px_pos,
                         clr,
                         bg_clr);

        px_pos.x += cell_px_d_.x;
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

    const R screen_r(P(0, 0), scr_d_);

    if (!screen_r.is_p_inside(p))
    {
        return;
    }

    const P px_pos(p * cell_px_d_);

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
        else // "Long wait" - pump events so game doesn't appear frozen
        {
            const unsigned int wait_until = SDL_GetTicks() + ms;

            while (SDL_GetTicks() < wait_until)
            {
                SDL_PumpEvents();
            }
        }
    }
}

InputData get_input()
{
    InputData d;

    if (!is_inited())
    {
        return d;
    }

    bool is_done = false;

    while (!is_done)
    {
        auto poll_result = SDL_PollEvent(&sdl_event_);

        if (poll_result == 0)
        {
            continue;
        }

        switch (sdl_event_.type)
        {
        case SDL_QUIT:
        {
        }
        break; // SDL_QUIT

        case SDL_WINDOWEVENT:
        {
            switch (sdl_event_.window.event)
            {
            case SDL_WINDOWEVENT_RESIZED:
                // on_window_resized();
                break;
            }
        }
        break; // SDL_WINDOWEVENT

        case SDL_MOUSEBUTTONDOWN:
        {
            const auto button = sdl_event_.button.button;

            if (button == SDL_BUTTON_LEFT ||
                button == SDL_BUTTON_RIGHT)
            {
                d.mouse_btn =
                    button == SDL_BUTTON_LEFT ?
                    MouseBtn::left :
                    MouseBtn::right;

                d.mouse_px_p.set(sdl_event_.button.x,
                                 sdl_event_.button.y);

                is_done = true;
            }
        }
        break; // SDL_MOUSEBUTTONDOWN

        case SDL_MOUSEMOTION:
        {
            d.mouse_px_p.set(sdl_event_.motion.x,
                             sdl_event_.motion.y);
            is_done = true;
        }
        break; // SDL_MOUSEMOTION

        // Text keyboard input (i.e. letter characters and such)
        case SDL_TEXTINPUT:
        {
            d.key   = sdl_event_.text.text[0];
            is_done = true;
        }
        break; // SDL_TEXTINPUT

        // Misc keyboard input (i.e. function keys)
        case SDL_KEYDOWN:
        {
            d.key   = sdl_event_.key.keysym.sym;
            is_done = true;

            // Re-initialize the io module (including script parsing)?
            // if (d.key == SDLK_F5)
            // {
            //     // Clear the input data first
            //     d = InputData();

            //     init();
            // }
        }
        break; // SDL_KEYDOWN

        default:
            break;

        } // event type switch

        sleep(1u);

    } // while

    return d;
}

void flush_input()
{
    while (SDL_PollEvent(&sdl_event_))
    {
        // Do nothing
    }
}

} // io
