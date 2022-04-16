#pragma once

#include "entities/none.h"
#include "entities/blocked.h"
#include "entities/portal.h"
#include "entities/chest.h"

namespace orpg {

    using entities_t = std::variant<none, blocked, chest, portal>;

    auto &operator<<(std::ostream &os, entities_t const &obj) {
        std::visit([&os](auto e) { os << e; }, obj);
        return os;
    }
}
