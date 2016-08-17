#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <string>

#include "io.hpp"
#include "rl_utils.hpp"

class Ent
{
public:
    Ent() :
        p_  () {}

    Ent(const P& p) :
        p_  (p) {}

    virtual ~Ent() {}

    const P& p()
    {
        return p_;
    }

    virtual RenderData render_data() const
    {
        return RenderData();
    }

    virtual std::string name() const
    {
        return "";
    }

    virtual void save() {}
    virtual void load() {}

protected:
    P p_;
};

#endif // ENTITY_HPP
