#pragma once

#include "entities/none.h"
#include "entities/blocked.h"
#include "entities/portal.h"

namespace orpg {

    using entities_t = std::variant<none, chest, var2, blocked, portal>;

    auto &operator<<(std::ostream &os, entities_t const &obj) {
        std::visit([&os](auto e) { os << e; }, obj);
        return os;
    }
}
