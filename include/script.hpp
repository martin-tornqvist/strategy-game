#ifndef SCRIPT_HPP
#define SCRIPT_HPP

#include "json.hpp"
#include "rl_utils.hpp"

typedef nlohmann::json JsonObj;

namespace script
{

JsonObj read(const std::string script_name);

std::string get_str(const std::string name,
                    const JsonObj& obj);

int get_int(const std::string name,
            const JsonObj& obj);

char get_char(const std::string name,
              const JsonObj& obj);

bool get_bool(const std::string name,
              const JsonObj& obj);

P get_p(const std::string name1,
        const std::string name2,
        const JsonObj& obj);

} // script

#endif // SCRIPT_HPP
