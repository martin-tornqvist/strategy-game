#include "map_ter.hpp"

#include "script.hpp"

RenderData MapTer::render_data() const
{
    return data_.render_data;
}

std::string MapTer::name() const
{
    return data_.name;
}

namespace ter_data
{

namespace
{

std::vector<TerData> data_;

const std::map<std::string, Clr>  str_to_clr =
{
    {"gray",    clr_gray},
    {"green",   clr_green}
};

void load_script_data()
{
    TRACE_FUNC_BEGIN;

    auto j = script::read("terrain.json");

    for (auto& jj : j)
    {
        TerData d;

        d.name                  = script::get_str("name", jj);
        d.render_data.clr       = str_to_clr.at(script::get_str("color", jj));
        d.render_data.symbol    = script::get_char("symbol", jj);

        data_.push_back(d);
    }

    // font_name           = j["font_name"];
    // font_img_px_d_.x    = j["image_width"];
    // font_img_px_d_.y    = j["image_height"];
    // cell_px_d_.x        = j["char_width"];
    // cell_px_d_.y        = j["char_height"];


    TRACE_FUNC_END;
}

} // namespace

void init()
{
    TRACE_FUNC_BEGIN;

    cleanup();

    load_script_data();

    TRACE_FUNC_END;
}

void cleanup()
{
    TRACE_FUNC_BEGIN;

    data_.clear();

    TRACE_FUNC_END;
}

const TerData& get()
{
    // Placeholder
    return data_[0];
}

} // ter_data
