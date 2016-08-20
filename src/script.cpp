#include "script.hpp"

#include <fstream>

namespace script
{

nlohmann::json read(const std::string script_name)
{
    const std::string path = "scripts/" + script_name;

    std::ifstream ifs(path);

    nlohmann::json j(ifs);

    return j;
}

std::string get_str(const std::string name,
                    const JsonObj& obj)
{
    return obj[name];
}

int get_int(const std::string name,
            const JsonObj& obj)
{
    return obj[name];
}

char get_char(const std::string name,
              const JsonObj& obj)
{
    const std::string str = obj[name];

    return str[0];
}

bool get_bool(const std::string name,
              const JsonObj& obj)
{
    return obj[name];
}

P get_p(const std::string name1,
        const std::string name2,
        const JsonObj& obj)
{
    const int x = obj[name1];
    const int y = obj[name2];

    return P(x, y);
}

} // script
