#ifndef MAP_TER_HPP
#define MAP_TER_HPP

#include <string>

#include "entity.hpp"

struct TerData
{
    RenderData render_data;
    std::string name;
};

class MapTer: public Ent
{
public:
    MapTer(const TerData& data, const P& p) :
        Ent(p),
        data_(data) {}

    ~MapTer() {}

    RenderData render_data() const override;

    std::string name() const override;

private:
    const TerData& data_;
};

namespace ter_data
{

void init();

void cleanup();

const TerData& get(/* TODO: Add some sort of criteria */);

} // ter_data

#endif // MAP_TER_HPP
