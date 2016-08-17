#ifndef MAP_TER_HPP
#define MAP_TER_HPP

#include "entity.hpp"

class MapTer: public Ent
{
public:
    MapTer() :
        Ent() {}

    MapTer(const P& p) :
        Ent(p) {}

    ~MapTer() {}

    RenderData render_data() const override;

    std::string name() const override;
};

#endif // MAP_TER_HPP
